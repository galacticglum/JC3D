import click
import logging
import click_log

from json import load
from enum import Enum
from pathlib import Path

logger = logging.getLogger(__name__)
click_log.basic_config(logger)

class DependencySourceType(Enum):
    """
    A git repository containing the library source code.

    """
    Git = 1

    """
    An archived file containing the library source code.

    """
    Archive = 2

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

    def __init__(self, name, source_url, source_url_type):
        self.name = name
        self.source_url = source_url
        self.source_url_type = source_url_type

def find_dependencies_config(directory):
    """
    Finds the dependencies.json in the specified directory.

    :param directory:
        The directory to search for dependencies.json.
    :returns:
        A `pathlib.Path` object pointing to the dependencies.json file or None`

    """

    path = Path(directory, 'dependencies').with_suffix('.json')
    return path if path.is_file() else None

def get_dependencies(directory):
    dependencies_config = find_dependencies_config(directory)
    if dependencies_config == None:
        logger.error(f'Could not find \'dependencies.json\' file in {directory}.')
        return None

    with open(dependencies_config.absolute(), 'r') as dependencies_file:
        json_data = load(dependencies_file)

        # Process the dependencies
        for depend in json_data['dependencies']:
            pass

@click.command()
@click_log.simple_verbosity_option(logger)
def cli():
    """
    Collects and processes the dependencies specified in the 'dependencies.json' file.
    
    """

    pass
