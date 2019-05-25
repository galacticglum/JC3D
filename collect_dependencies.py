import argparse
import coloredlogs, logging

from json import load
from enum import Enum
from pathlib import Path

# Define and parse arguments
parser = argparse.ArgumentParser(description='''Collects and processes the dependencies
    specified in the \'dependencies.json\' file.''')

args = parser.parse_args()

# Setup the logging
field_styles = dict(coloredlogs.DEFAULT_FIELD_STYLES)
field_styles['levelname'] = {
    'color': 'white',
    'background': 'black'
}

logger = logging.getLogger('collect_dependencies')
coloredlogs.install(fmt='%(name)s - %(levelname)s - %(message)s', field_styles=field_styles)

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
            

working_directory = Path().absolute()
get_dependencies(working_directory)