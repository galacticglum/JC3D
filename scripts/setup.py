from setuptools import setup

setup(
    name='collect_dependencies',
    version='1.0',
    py_modules=['collect_dependencies'],
    install_requires=[
        'Click',
        'jsonschema',
        'requests',
        'colorama'
    ],
    entry_points='''
        [console_scripts]
        collect_dependencies=collect_dependencies:cli
    ''',
)