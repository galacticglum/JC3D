# JesusChristIn3D
Jesus Christ, it's a 3D rendering engine; a modular and extensible real-time physically based rendering engine library built in modern C++ and OpenGL. The engine is built in the form of a library that can be leveraged and extended by other developers. A minimal debug editor is also included that allows the world to be inspected/modified.

The goal of the project is twofold: to build a codebase that is both modular and extensible (such that it is easy to use and read) and that it has the capability to render a moderately sized test scene in real-time. About the former, the engine should be structured in a way that it can be expanded upon and that makes the API easy to use (i.e. modularity, flexibility, and maintainability).

## Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

* [Python 3.x](https://www.python.org/downloads/)
* [CMake](https://cmake.org/runningcmake/)

### Setting up the dependency managment system
JesusChrstIn3D uses a custom Python build tool which requires external dependencies to function. Fortunately, this process is very simple using Python virtual environments. In order to use the dependency management tool, you must setup a virtual environment:
* Open a terminal or command prompt window
* Run ``pip install virtualenv`` to install the virtualenv Python package.
* Navigate to the root project directory (e.g. ``.../JesusChristIn3D/``; this directory should contain the ``README.md`` and build tool batch files: ``Windows-Clean.bat`` and ``Windows-GenProject.bat``).
* Run ``virtualenv venv`` to create a new virtualenv in the current directory with the name of ``venv``.

Once you have setup the virtual environment, make sure it works by activating it (see [virtualenv documentation](https://packaging.python.org/guides/installing-using-pip-and-virtual-environments/): for specific instructions on how to activate the virtual environment on your platform). You an deactivate the virtual environment by using the command ``deactivate``.

collect_dependencies is the build script responsible for collecting the dependencies of the project. In particular, it scans for a ``dependencies.json`` file in the current directory. This file contains dependency information which it uses to download, extract, build, and finally organize the dependencies into a file sturcture that the CMake script(s) can reference.

In the case of the engine, the main ``dependencies.json`` actually specifies no dependencies itself; rather, it defines ``/.Engine`` as a subdirectory. The configuration file  ``/Engine/dependencies.json`` is the one that actually contains the dependencies for the Engine project. The dependency data is put into a new folder called ``Dependencies`` with its parent being the same directory as where the ``dependencies.json`` file that produced the dependency is located.

Setting up collect_dependencies is rather simple due to Python's workflow:
* Open a terminal or command prompt window and activate the virtual environment
* Run the command ``pip install --editable scripts`` from the root project directory.

### Collecting the dependencies
From the root project directory, with the virtual environment activated, run ``collect_dependencies`` to collect the dependencies. An optional ``-f`` parameter can be provided to force a full dependency clean. Alternatively, you can run ``collect_dependencies clean`` to clean the dependencies.

Rerun collect_dependencies to gather new dependencies or, using the force parameter (``-f``), to update preexisting ones.

### Generating the Visual Studio Projects (Windows)
From the root project directory, run the ``Windows-GenProject.bat`` file which weill generate the solution and project files in a new ``Build`` folder. The solution file (``JesusChristIn3D.sln``) is located inside the ``Build`` directory.

### Adding files to the project
Since the project is managed by CMake, files should be added through the filesystem **NOT** through Visual Studio. When a file is added, the rpoject should be regenerated...

## FAQ
### "I added content but it is copied over" **OR** "a binnary is not found!"
By default, the Sandbox project is configured to recursively copy the ``Sandbox/Content`` directory and any dependency binaries to the output directory; however, these commands will run only after a successful build. If Visual Studio skips a build, these commands will not run. Therefore, if you add content and it doesn't copy, either manually copy the content to the output directory or **force** a rebuild of the entire solution.

### "During dependency collection, I get a ``zipfile.BadZipFile`` error"
This error indicates that the dependency archive that was gathered is **NOT** a valid zipfile; however, this error can occur even if the archive URL specifies a valid zipfile. In this case, this is most likely due to an unresponsive server which triggers a timeout and causes an error:
* First try rerunning the ``collect_dependencies`` command. Rerunning the command, assuming no force parameter is applied, will only gather the dependencies that have yet to be gathered (according to the ``dependency.lock`` file).
* Try changing the archive URL of the dependency to a server that is faster and more reliable.

### "When I run the engine, the window layout is all messed up!"
The first time that you run the Sandbox project, you will find that the window layout is very messy. This is because no default layout is specified with ImGui; however, since ImGui uses an ini file to save UI information, you can use the specified [imgui.ini](https://drive.google.com/file/d/1fgVzpSlPZfj_FRiHzyvwH4HX1yN7Uqxm/view?usp=sharing) file to get a nice window layout. Simply place the file in the output directory.

For reference: the output directory of the sandbox project is ``Build/Sandbox/bin/<config>/``. Place the attached file in the same directory as ``Sandbox.exe``.
