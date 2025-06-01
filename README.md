# tectonics.cpp
## A C++ library for simulating worlds

**tectonics.cpp** is a header-only C++ library that's intended to supplant the backend to [tectonics.js](http://davidson16807.github.io/tectonics.js/). It contains only the best parts of tectonics.js - the parts that either have proven out over time or have emerged with confidence as replacements through good design. 

This library is not just designed for use by tectonics.js. Numerous space simulators can make use of the functionality here. The components here are lightweight and loosely coupled. Parts of the library can be easily taken out, mixed and matched, and modified. The library is also rigorously covered by unit tests. It's a shame really if this library only gets used once. 

As of completing major milestones in May 2025, permission to use all code going forward is provided under the Creative Commons Attribution-NonCommercial-Sharealike 4.0 license. This code is written with fellow hobby developers in mind, and noncommercial hobby projects are always welcome to use it freely. Drop me a line if you use it, since it's encouraging to hear my work is getting used, and it's always fun to hear from someone with fellow interests! Commercial licensing is also available by prior agreement - drop me a line if interested. 

# Overview
tectonics.cpp is adheres to a strict design philosophy that is meant to prioritize functional programming concerns in a way that also leverages object-oriented paradigms. See `doc/general-design-approach.md` for more information.

tectonics.cpp can be broken down into several subcomponents. Users can pick and choose which subcomponents they'd like to include in their project at their discretion. Some optional dependencies exist between subcomponents, which are indicated clearly by the folder structure (e.g. `index/adapted/glm` stores behavior that requires `glm`). Using optional dependencies this way allows the use of functionality that wouldn't otherwise be possible without requiring people to copy over other unwanted components. 

The most important top-level folders are:

* `lib` for header-only 3rd party libraries
* `inc` for mature components that are ready for use by other code, such as for `math`, `grid`s, or scientific `unit`s and `model`s
* `sketch` for long-running prototypes and outlines that cannot be suitably handled by software branches
* `src` for code that is not expected to be useful to projects outside tectonics, such as the code for handling the UI that's used within `demo`
* `demo` for code that requires 3d libraries to run, usually to demonstrate functionality in the library
* `doc` for project-wide documentation that cannot be suitably put alongside any specific folder or file in the repository

Unit tests and documentation are placed alongside the folder or file they describe. File-specific documentation is stored in image or markdown files that share the same name as the thing it documents (e.g. `grid/dymaxion/Projection.svg` documents `grid/dymaxion/Projection.hpp`). Folder-specific documentation is stored in the folder it documents using one of several standardized names, in uppercase (e.g. `README.md`, `CATEGORY.png`, `DEPENDENCY.png`, etc.)

# Getting Started
Tectonics.cpp has been tested on many combinations of operating system, build system, and compiler. Here are a few of the most common approaches.

Numerous makefiles exist throuhgout the library. These are used to build demos and run isolated unit tests. Simply run `make` in any project folder with a `Makefile` to run all the functionality that's defined within that folder. Alternatively, you can run `scons -u` from any directory and generate an executable. Building graphical demos requires two additional libraries: [GLEW](http://glew.sourceforge.net/) and [GLFW](https://www.glfw.org/). For more OS-specific details, see below.

## Ubuntu

### make with g++

In case you haven't done so already, install `g++`:

`sudo apt install g++`

Run `make` under any folder with a `Makefile`. To run graphical demos under `demo/`, you will first need to install glew and glfw with the following commands:

```bash
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
```

### Scons with g++

First, you'll need to install Scons on an instance of Python. Python comes with Ubuntu but installing libraries like `scons` on this instance is not recommended, so first you'll need to setup a virtual environment. Navigate to a folder that's easy for you to access and run:

```bash
python3 -m venv venv-path
source venv-path/bin/activate
python -m pip install scons
```

Once scons is installed, run `scons -u` from any project folder and scons will build the functionality that's defined within the folder. This will create a `test.out` file that you can execute. Remember that whenever you start a new terminal session, you'll need to restart your virtual environment before you use scons. This is done by calling `source venv-path/bin/activate` again.

## Windows

### g++ in Cygwin

Download and execute the Cygwin installer:

https://cygwin.com/install.html

* search for "g++" and under "gcc-g++" select the most recent non-test version
* search for "make" and under "make" select the most recent non-test version

From the start menu, open "Cygwin Terminal" and use `cd` to navigate to where you want the tectonics.cpp repo to be located (note that the contents of the `C:/` drive are located under `/cygdrive/c/`), then clone the repo:

```bash
git clone https://github.com/davidson16807/tectonics.cpp.git
```

Use `cd` to navigate to a directory in the repo that has a `Makefile` in it, type `make`, and hit enter.

Additional setup is needed to run graphical demos under `demo/`. You will need the Windows binaries for glew and glfw. Windows binaries for glew and glfw can be found under `demo/windows-*` folders, or the latest binaries can be downloaded here:

https://glew.sourceforge.net/
https://www.glfw.org/

Sample commands below will assume binaries are placed under C:/glew-2.1 and C:/glfw-3.4. Once their contents are unzipped, specify paths for header and library files:

```bash
export CPLUS_INCLUDE_PATH=/cygdrive/c/glfw-3.4.bin.WIN64/include/:/cygdrive/c/glew-2.1/include/
export LIBRARY_PATH=/cygdrive/c/glfw-3.4.bin.WIN64/lib-mingw-w64/:/cygdrive/c/glew-2.1/bin/Release/x64/
```

You may want to add these to your ~/.bashrc file so you don't have to repeat this last step with every new terminal session:

```bash
echo 'export CPLUS_INCLUDE_PATH=/cygdrive/c/glfw-3.4.bin.WIN64/include/:/cygdrive/c/glew-2.1/include/' >> ~/.bashrc
echo 'export LIBRARY_PATH=/cygdrive/c/glfw-3.4.bin.WIN64/lib-static-ucrt/:/cygdrive/c/glew-2.1/bin/Release/x64/' >> ~/.bashrc
source ~/.bashrc
```

You will also need to copy glfw3.dll from the lib-static-ucrt folder of the distributable to the same folder that stores the Makefile you want to build from.

Once the setup above is complete, call `make`, and the demo should execute. 

If you have trouble, you can troubleshoot with the following direct call to g++ (make sure you're in the same folder as a test.cpp file):

```bash
g++ -std=c++17 -o test.exe test.cpp -g -D GLM_FORCE_SWIZZLE -D GLEW_STATIC -I ../inc/ -I /cygdrive/c/glew-2.1/include/ -I /cygdrive/c/glfw-3.4.bin.WIN64/include/ -L /cygdrive/c/glew-2.1/bin/Release/x64/ -L /cygdrive/c/glfw-3.4.bin.WIN64/lib-static-ucrt/ -lglfw3 -lopengl32 -lgdi32 -lglew32
```

### g++ in MinGW/MSYS

As of writing in May 2025, the g++ that comes with the official [MinGW](https://sourceforge.net/projects/mingw/) installation is outdated and unable to build the vast majority of tectonics.cpp. We recommend downloading the following unofficial build of MINGW that comes with a modern g++:

https://nuwen.net/mingw.html

Download and run the executable. The executable will unzip MinGW to a directory of your choosing. Choose a directory you will easily remember. Sample commands below will assume MinGW is unzipped to C:/MinGW/

We also recommend installing gitbash, which you can get here:

https://git-scm.com/downloads/win

We recommend accepting default settings during installation, though you may be able to get by with other settings.

Add the paths to MinGW to your system PATH. You can do this with the following commands in gitbash:

```bash
export PATH="/c/MinGW/bin:$PATH"
export PATH="/c/MinGW/msys/1.0/bin:$PATH"
```

You may want to add these to your ~/.bashrc file so you don't have to repeat this last step with every new terminal session:

```bash
echo 'export PATH="/c/MinGW/bin:$PATH"' >> ~/.bashrc
echo 'export PATH="/c/MinGW/msys/1.0/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

If this works you should be able to find `g++` and `make` in git bash:

```bash
which g++
which make
```

You should also be able to specify paths manually by opening "Control Panel" > "Environment Variables".

Now that setup is complete, open gitbash and clone tectonics.cpp:

```bash
git clone https://github.com/davidson16807/tectonics.cpp.git
```

Use `cd` to move to a directory within `tectonics.cpp` that has a `Makefile` in it, type `make`, and hit enter.

Additional setup is needed to run graphical demos under `demo/`. Windows binaries for glew and glfw can be found under `demo/windows-*` folders, or the latest binaries can be downloaded [here](https://glew.sourceforge.net/) and [here](https://www.glfw.org/)

Sample commands below will assume binaries are placed under C:/glew-2.1 and C:/glfw-3.4. Once their contents are unzipped, specify paths for header and library files:

```bash
export CPLUS_INCLUDE_PATH=/c/glfw-3.4.bin.WIN64/include/:/c/glew-2.1/include/
export LIBRARY_PATH=/c/glfw-3.4.bin.WIN64/lib-mingw-w64/:/c/glew-2.1/bin/Release/x64/
```

Here is the command you can run to copy the above to your ~/.bashrc file:

```bash
echo 'export CPLUS_INCLUDE_PATH=/c/glfw-3.4.bin.WIN64/include/:/c/glew-2.1/include/' >> ~/.bashrc
echo 'export LIBRARY_PATH=/cygwin/c/glfw-3.4.bin.WIN64/lib-static-ucrt/:/cygwin/c/glew-2.1/bin/Release/x64/' >> ~/.bashrc
source ~/.bashrc
```

You will also need to copy glfw3.dll from the lib-mingw-w64 folder of the distributable to the same folder that stores the Makefile you want to build from.

If you have trouble, you can troubleshoot with the following direct call to g++ (make sure you're in the same folder as a test.cpp file):

```bash
g++ -std=c++17 -o test.exe test.cpp -g -D GLM_FORCE_SWIZZLE -D GLEW_STATIC -I ../inc/ -I /c/glew-2.1/include/ -I /c/glfw-3.4.bin.WIN64/include/ -L /c/glew-2.1/bin/Release/x64/ -L /c/glfw-3.4.bin.WIN64/lib-mingw-w64/ -lglfw3 -lopengl32 -lgdi32 -lglew32
```

### MSVC with Scons

For this you can either install [Visual Studio](https://visualstudio.microsoft.com/vs/community/) or the [MSVC redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#visual-studio-2015-2017-2019-and-2022). The MSVC redistributable is much smaller and easier to install so it is reccomended if you have no plan to develop using Visual Studio. If you prefer to install Visual Studio, the free Community edition is sufficient.

You will also need to install Python. You will need version 3 or higher, and you will need to check the "Add Python to PATH" option in the installer:

https://www.python.org/downloads/windows/

After Python is installed, open your terminal of choice and install the `scons` library for Python:

```bash
python -m pip install scons
```

Now within the terminal, navigate to any folder in the tectonics repo and run `scons -u`. This will create an executable in the same folder that you can run.

Additional setup is needed to run graphical demos under `demo/`. You will need the Windows binaries for glew and glfw. Windows binaries for glew and glfw can be found under `demo/windows-*` folders, or the latest binaries can be downloaded [here](https://glew.sourceforge.net/) and [here](https://www.glfw.org/)

Copy glew32.dll and glfw3.dll to the folder with the Makefile that you are building from, then run `scons -u`. This will create a new executable, "tests.exe", which you can run.

# Using Visual Studio

Download and install Visual Studio. The free Community edition will be sufficient:

https://visualstudio.microsoft.com/vs/community/

This approach will require a VS config file that will provided in the future.
