import click
import logging
import re
import colorama
import copy
import shutil
import hashlib
import tempfile
import json
import time
import datetime
import requests
import zipfile

from enum import IntEnum
from pathlib import Path
from jsonschema import validate as validate_json

def colourize_string(string, colour):
    return '{color_begin}{string}{color_end}'.format(
        string=string,
        color_begin=colour,
        color_end=colorama.Style.RESET_ALL)

def initialize_logger_format(logger):
    """
    Initialize the specified logger with a coloured format.

    """

    # specify colors for different logging levels
    LOG_COLORS = {
        logging.FATAL: colorama.Fore.LIGHTRED_EX,
        logging.ERROR: colorama.Fore.RED,
        logging.WARNING: colorama.Fore.YELLOW,
        logging.DEBUG: colorama.Fore.LIGHTWHITE_EX
    }

    LOG_LEVEL_FORMATS = {
        logging.INFO: '%(message)s'
    }

    class CustomFormatter(logging.Formatter):
        def format(self, record, *args, **kwargs):
            # if the corresponding logger has children, they may receive modified
            # record, so we want to keep it intact
            new_record = copy.copy(record)
            if new_record.levelno in LOG_COLORS:
                # we want levelname to be in different color, so let's modify it
                new_record.levelname = "{color_begin}{level}{color_end}".format(
                    level=new_record.levelname,
                    color_begin=LOG_COLORS[new_record.levelno],
                    color_end=colorama.Style.RESET_ALL,
                )

            original_format = self._style._fmt
            self._style._fmt = LOG_LEVEL_FORMATS.get(record.levelno, original_format)

            # now we can let standart formatting take care of the rest
            result = super(CustomFormatter, self).format(new_record, *args, **kwargs)

            self._style._fmt = original_format
            return result

    handler = logging.StreamHandler()
    handler.setFormatter(CustomFormatter('%(name)s - %(levelname)s: %(message)s'))
    logger.addHandler(handler)

def _rmtree(path, ignore_errors=False, onerror=None, timeout=10):
    """
    A wrapper method for 'shutil.rmtree' that waits up to the specified
    `timeout` period, in seconds.

    """

    shutil.rmtree(path, ignore_errors, onerror)

    if path.is_dir():
        logger.warning(f'''shutil.rmtree - Waiting for \'{path}\' to be removed...''')
        # The destination path has yet to be deleted. Wait, at most, the timeout period.
        timeout_time = time.time() + timeout
        while time.time() <= timeout_time:
            if not path.is_dir():
                break


logger = logging.getLogger(__name__)
initialize_logger_format(logger)

def get_container_directory(base_directory, directory_name=None, default_directory_name='dependencies'):
    container_directory_name = default_directory_name if directory_name is None else directory_name
    return Path(base_directory) / container_directory_name

class DependencySourceType(IntEnum):
    Git = 1
    Archive = 2

    @classmethod
    def get_names_for_schema(cls):
        def to_snake_case(name):
            s1 = re.sub(r'(.)([A-Z][a-z]+)', r'\1_\2', name)
            return re.sub(r'([a-z0-9])([A-Z])', r'\1_\2', s1).lower()
        
        return [to_snake_case(value.name) for value in DependencySourceType]

    @classmethod
    def from_snake_case_name(cls, name):
        name = ''.join(x.capitalize() or '_' for x in name.split('_'))
        return cls[name]

class UnsupportedSourceTypeError(Exception):
    """
    Raised when the source type of a dependency is not supported.

    """

    def __init__(self, source_type):
        super(UnsupportedSourceTypeError, self).__init__(f'Unsupported source type: \'{source_type.name}\'')

class Dependency(object):
    """
    Data class that stores information about a specific dependency.
    
    """

    DEPENDENCY_SCHEMA = {
        'type': 'object',
        'properties': {
            'sub_dependencies': {
                'type': 'object',
            },
            'url': {
                'type': 'string',
                'format': 'uri'
            },
            'url_type': {
                'type': 'string',
                'enum': DependencySourceType.get_names_for_schema()
            },
            'libraries': {
                'type': 'array',
                'items': {
                    'type': 'object'
                }
            },
            'include_dirs': {
                'type': 'array',
                'items': {
                    'type': 'string'
                }
            }
        },
    }

    BINARY_JSON_SCHEMA = {
        'type': 'object',
        'properties': {
            'filepath': {
                'type': 'string'
            },
            'platforms': {
                'type': 'array',
                'items': {
                    'type': 'string'
                }
            },
            'configs': {
                'type': 'array',
                'items': {
                    'type': 'string'
                }
            }
        },
        'required': ['filepath', 'platforms', 'configs']
    }

    def __init__(self, name, source_url, source_url_type, args, container_directory):
        self.name = name
        self.source_url = source_url
        self.source_url_type = source_url_type
        self.args = args

        self.colourized_name = colourize_string(self.name, colorama.Fore.LIGHTWHITE_EX)
        self.container_directory = container_directory
        self.destination_path = Path(container_directory) / self.name

        # sub dependencies should be saved in the same directory as their parent.
        # To do this, the containing directory is the sub dependency parent directory.
        sub_dependency_container_directory = self.destination_path

        # load the sub dependencies
        self.sub_dependencies = Dependency.get_dependencies(args.get('sub_dependencies', dict()), sub_dependency_container_directory)

    def _get_lock_filepath(self):
        """
        Retrieves the filepath to this `Dependency` lock file.
        """

        # The lock file contains a hash of the Dependency object that was used to download
        # the contents of the folder. If the hash in the lock file is not the same as the
        # current one, the dependency is regathered.
        return self.destination_path / 'dependency.lock'

    def is_locked(self):
        """
        Indicates whether this `Dependency` is locked.

        """

        lock_filepath = self._get_lock_filepath()
        if self.destination_path.is_dir():
            if lock_filepath.is_file():
                # The lock file is stored as JSON
                lock_data = json.load(lock_filepath.open())
                if lock_data.get('dependency_hash') == self.get_hash():
                    return True

        return False

    def lock(self):
        """
        Locks this `Dependency`. Creates a new updated lock file.

        """
        # Create a new updated lock file
        lock_filepath = self._get_lock_filepath()
        json.dump({'dependency_hash': self.get_hash()}, lock_filepath.open('w+'))

    def unlock(self):
        """
        Unlocks this `Dependency`. Removes the lock file if it exists.

        """

        # Delete the lock file
        lock_filepath = self._get_lock_filepath()
        if lock_filepath.is_file():
            lock_filepath.unlink()

    def _get(self):
        """
        Retrieve and extract the dependency.
        """

        if self.source_url_type == DependencySourceType.Git:
            # TODO: Implement git dependencies.
            raise UnsupportedSourceTypeError(DependencySourceType.Git)
        elif self.source_url_type == DependencySourceType.Archive:
            # Extract and build filesystem
            with tempfile.NamedTemporaryFile(delete=False) as tmp_file_handle:
                logger.info(f'{self.colourized_name} - Downloading archive ({self.source_url})')

                start_time = time.time()
                response = requests.get(self.source_url, stream=True)
                total_length = response.headers.get('content-length')

                # no content length header
                if total_length is None:
                    tmp_file_handle.write(response.content)
                else:
                    total_length = int(total_length)
                    with click.progressbar(length=total_length, label='Downloading...') as bar:
                        for chunk in response.iter_content(chunk_size=4096):
                            tmp_file_handle.write(chunk)
                            bar.update(len(chunk))

            try:
                if not zipfile.is_zipfile(tmp_file_handle.name):
                    raise zipfile.BadZipFile()
            except:
                logger.exception(f'Invalid archive file provided for \'{self.name}\' dependency.')
                return False

            logger.info(f'{self.colourized_name} - Extracting archive')
            with zipfile.ZipFile(tmp_file_handle.name) as zip_file:
                archive_extract_items = self.args.get('archive_extract_items', None)
                file_list = []
                
                ARCHIVE_EXTRACT_ITEMS_SCHEMA = {
                    'type': 'object',
                    'properties': {
                        'dirs': {
                            'type': 'array',
                            'items': {
                                'type': 'string'
                            }
                        },
                        'files': {
                            'type': 'array',
                            'items': {
                                'type': 'string'
                            }
                        }
                    }
                }

                try:
                    validate_json(instance=archive_extract_items, schema=ARCHIVE_EXTRACT_ITEMS_SCHEMA)
                    dirs = archive_extract_items.get('dirs', list())
                    files = archive_extract_items.get('files', list())

                    if len(dirs) == len(files) == 0:
                        raise

                    for target_dir in dirs:
                        for file in zip_file.namelist():
                            if file.startswith(target_dir):
                                file_list.append(file)

                    file_list += files
                except:
                    file_list = zip_file.namelist()

                with click.progressbar(file_list, label='Extracting...') as bar:
                    for name in bar:
                        zip_file.extract(name, self.destination_path)

            # Delete temporary file
            tmp_file_path = Path(tmp_file_handle.name)
            if tmp_file_path.is_file():
                tmp_file_path.unlink()
            
        return True

    def _collect_libraries(self):
        libraries = self.args.get('libraries', list())
        root_library_path = self.destination_path / 'lib'

        count = 0
        for library in libraries:
            try:
                validate_json(instance=library, schema=Dependency.BINARY_JSON_SCHEMA)
            except:
                logger.exception(f'{self.colourized_name} - Invalid library entry for dependency of name \'{self.name}\'.')
                continue

            # filepaths in the json are given relative to the dependency destination directory ("<containing_directory>/<dependency_name>")
            absolute_filepath = self.destination_path / library['filepath']

            for config in library['configs']:
                for platform in library['platforms']:
                    library_path = root_library_path / config / platform
                    
                    # create the library file destination tree
                    library_path.mkdir(exist_ok=True, parents=True)
                    shutil.copy(absolute_filepath,  library_path / absolute_filepath.name)

            count += 1
        
        logger.info(f'{self.colourized_name} - Finished library collection. Collected {count} libraries.')

    def _collect_includes(self):
        include_dirs = self.args.get('include_dirs', list())
        root_include_path = self.destination_path / 'include'

        count = 0
        for include_dir in include_dirs:
            include_path = self.destination_path / include_dir
            if not include_path.is_dir():
                logger.error(f'{self.colourized_name} - Invalid include directory (\'{include_dir}\') provided for dependency of name \'{self.name}\'.')
                continue

            shutil.copytree(include_path, root_include_path)
            count += 1

        logger.info(f'{self.colourized_name} - Finished include directory collection. Collected {count} include directories.')

    def _collect_binaries(self):
        binaries = self.args.get('binaries', list())
        root_binary_path = self.destination_path / 'bin'

        count = 0
        for binary in binaries:
            try:
                validate_json(instance=binary, schema=Dependency.BINARY_JSON_SCHEMA)
            except:
                logger.exception(f'{self.colourized_name} - Invalid binary entry for dependency of name \'{self.name}\'.')
                continue

            # filepaths in the json are given relative to the dependency destination directory ("<containing_directory>/<dependency_name>")
            absolute_filepath = self.destination_path / binary['filepath']

            for config in binary['configs']:
                for platform in binary['platforms']:
                    binary_path = root_binary_path / config / platform
                    
                    # create the binary file destination tree
                    binary_path.mkdir(exist_ok=True, parents=True)
                    shutil.copy(absolute_filepath,  binary_path / absolute_filepath.name)

            count += 1
        
        logger.info(f'{self.colourized_name} - Finished binary collection. Collected {count} binaries.')

    def process(self):
        """
        Processes the dependency.

        """
    
        if self.destination_path.is_dir():
            if self.is_locked():
                logger.info(f'{self.colourized_name} - Skipped: dependency already installed')
                return
            
            _rmtree(self.destination_path)

        # Process the sub dependencies. We have to load the sub dependencies first since
        # they are dependencies of this dependency!
        for sub_dependency in self.sub_dependencies.values():
            sub_dependency.process()

        # Create the destination directory if it does not exist
        self.destination_path.mkdir(parents=True, exist_ok=True)

        self.unlock()
        if not self._get(): return
        self.lock()
        
        self._collect_libraries()
        self._collect_binaries()
        self._collect_includes()

    def get_hash(self):
        return hashlib.md5(json.dumps({
            'source_url': self.source_url,
            'source_url_type': self.source_url_type,
            'args': self.args
        }, sort_keys=True).encode('utf-8')).hexdigest()

    def __str__(self): return str((self.name, str(self.source_url_type), self.source_url))
    def __repr__(self): return self.__str__()

    @staticmethod
    def get_dependencies(dependencies_json, container_directory, onerror=None):
        result = {}
        for dependency_name in dependencies_json:
            try:
                validate_json(instance=dependencies_json[dependency_name], schema=Dependency.DEPENDENCY_SCHEMA)

                source_url = dependencies_json[dependency_name].get('url')
                source_url_type = dependencies_json[dependency_name].get('url_type')
                if source_url_type is not None:
                    source_url_type = DependencySourceType.from_snake_case_name(source_url_type)

                dependency = Dependency(dependency_name, source_url, source_url_type, 
                    dependencies_json[dependency_name], container_directory)
                
                result[dependency_name] = dependency
            except:
                if onerror is not None:
                    onerror(dependency_name)
                continue

        return result

class DependencyDirectory(object):
    """
    Data class that stores information about a dependency directory.
    
    """

    DEPENDENCIES_CONFIG_SCHEMA = {
        'type': 'object',
        'properties': {
            'subdirectories': {
                'type': 'array',
                'items': {
                    'type': 'string'
                }
            },
            'container_directory_name': {
                'type': 'string'
            },
            'dependencies': {
                'type': 'object'
            }
        }
    }

    def __init__(self, directory):
        self.directory = directory
        self.config_path = DependencyDirectory._get_dependencies_config_path(directory)
        self.json_data = DependencyDirectory._get_dependencies_config(directory)

        self.is_valid = self.json_data is not None 
        if self.json_data is None: return

        self.container_directory = get_container_directory(directory, self.json_data.get('container_directory_name'))

        # Handle an error with getting a dependency
        def _dependency_onerror(dependency_name):
            logger.exception(f'Invalid dependency in \'{self.config_path.absolute()}\' with name \'{dependency_name}\'')

        self.dependencies = Dependency.get_dependencies(self.json_data.get('dependencies', dict()), self.container_directory, onerror=_dependency_onerror)

        # Load the subdirectories
        self.subdirectories = []
        subdirectory_paths = self.json_data.get('subdirectories', list())
        for subdirectory_path in subdirectory_paths:
            self.subdirectories.append(DependencyDirectory(subdirectory_path))

    def process(self):
        """
        Process this `DependencyDirectory`.

        """

        # Process the dependencies
        if not self.is_valid: return
        for dependency in self.dependencies.values():
            dependency.process()

        # Invoke processing on each subdirectory
        for subdirectory in self.subdirectories:
            subdirectory.process()

    def clean(self):
        """
        Clean this `DependencyDirectory`.

        """

        if not self.is_valid: return
        if self.container_directory.is_dir():
            _rmtree(self.container_directory)

        for subdirectory in self.subdirectories:
            subdirectory.clean()

    @staticmethod
    def _get_dependencies_config_path(directory):
        return Path(directory, 'dependencies').with_suffix('.json')

    @staticmethod
    def _get_dependencies_config(directory):
        """
        Retrieves the dependencies.json data in the specified directory.

        :param directory:
            The directory to search for dependencies.json.
        :returns:
            A `dict` object containing the deserialized JSON data of the dependencies.json file or `None`
            if the file could not be found.

        """

        dependencies_config = DependencyDirectory._get_dependencies_config_path(directory)
        if dependencies_config is None:
            logger.error(f'Could not find \'dependencies.json\' file in {directory}.')
            return None

        with open(dependencies_config.absolute(), 'r') as dependencies_file:
            json_data = json.load(dependencies_file)
            
            try:
                validate_json(instance=json_data, schema=DependencyDirectory.DEPENDENCIES_CONFIG_SCHEMA)
                return json_data
            except:
                logger.exception(f'Invalid dependencies.json file (\'{dependencies_config.absolute()}\').')
                return None

def _set_level(ctx, param, value):
    x = getattr(logging, value.upper(), None)
    if x is None:
        raise click.BadParameter(f'Must be CRITICAL, ERROR, WARNING, INFO or DEBUG, not \'{value}\'')
    
    logger.setLevel(x)

def get_cwd():
    """
    Retrieves the absolute path to the current working directory.

    """

    return Path().absolute()

@click.group(invoke_without_command=True)
@click.option('--force', '-f', is_flag=True, default=False, help='Cleans all existing dependencies and regathers them.')
@click.option('--verbosity', '-v', default='INFO', help='Either CRITICAL, ERROR, WARNING, INFO, or DEBUG.', callback=_set_level)
@click.pass_context
def cli(ctx, force, verbosity):
    """
    Collects and processes the dependencies specified in the 'dependencies.json' file.
    
    """

    if ctx.invoked_subcommand is not None: return

    cwd_dependency_directory = DependencyDirectory(get_cwd())
    if force:
        cwd_dependency_directory.clean()
    
    cwd_dependency_directory.process()

@cli.command()
@click.option('--no-prompt', is_flag=True, default=False, help='Suppresses the confirmation prompt that is presented by the clean operation.')
def clean(no_prompt):
    """
    Cleans all the dependencies in the working directory and any subdirectories.

    """

    if no_prompt or click.confirm(('Are you sure you want to clean the dependencies? '
        'This will remove all dependencies in the current working directory and any subdirectories.')):

        cwd_dependency_directory = DependencyDirectory(get_cwd())
        cwd_dependency_directory.clean()
