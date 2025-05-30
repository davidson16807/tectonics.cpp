# tectonics.cpp
## A C++ library for simulating worlds

**tectonics.cpp** is a header-only C++ library that's intended to supplant the backend to [tectonics.js](http://davidson16807.github.io/tectonics.js/). It contains only the best parts of tectonics.js - the parts that either have proven out over time or have emerged with confidence as replacements through good design. 

This library is not just designed for use by tectonics.js. Numerous space simulators can make use of the functionality here. The components here are lightweight and loosely coupled. Parts of the library can be easily taken out, mixed and matched, and modified. The library is also rigorously covered by unit tests. It's a shame really if this library only gets used once. 

As of completing major milestones in May 2025, permission to use all code going forward is provided under the Creative Commons Attribution-NonCommercial-Sharealike 4.0 license. This code is written with fellow hobby developers in mind, and noncommercial hobby projects are always welcome to use it freely. Drop me a line if you use it, since it's encouraging to hear my work is getting used, and it's always fun to hear from someone with fellow interests! Commercial licensing is also available by prior agreement - drop me a line if interested. 

# Getting Started
Tectonics.cpp has been tested on many combinations of operating system, build system, and compiler. Here are a few of the most common approaches.

## Ubuntu

### make with g++

Numerous makefiles exist throuhgout the library. These are used to build demos and run isolated unit tests. Simply run `make` in any project folder with a `Makefile` to run all the functionality that's defined within that folder. 

Makefiles under the `demo/` folder build demos for graphical functionality. To run any of these makefiles, you'll first need to install two other libraries: [GLEW](http://glew.sourceforge.net/) and [GLFW](https://www.glfw.org/). On Ubuntu you can install both with the following commands:

```bash
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
```

### Scons with g++

First, you'll need to install Scons on an instance of Python. Python comes with Ubuntu but installing libraries on the system-wide Python instance is risky and frowned upon, so first you'll need to setup a virtual environment. Navigate to a folder that's easy for you to access and run:

```bash
python3 -m venv venv-path
source venv-path/bin/activate
python -m pip install scons
```

Once scons is installed, simply run `scons -u` from any project folder and scons will run the functionality that's defined within the folder. Remember that whenever you start a new terminal session, you'll need to restart your virtual environment before you use scons. This is done by calling `source venv-path/bin/activate` again.

## Windows

### g++ in Cygwin

To run the graphical demos under `demo/`, you will first need to download the binaries for glew and glfw:

	https://glew.sourceforge.net/
	https://www.glfw.org/

Unzip the downloads to paths that you can remember, then setup variables for the compiler that point to those paths:

```bash
export CPLUS_INCLUDE_PATH=/cygdrive/c/glfw-3.4.bin.WIN64/include/:/cygdrive/c/glew-2.1.0/include/
export LIBRARY_PATH=/cygdrive/c/glfw-3.4.bin.WIN64/lib-mingw-w64/:/cygdrive/c/glew-2.1.0/bin/Release/x64/
```

It may be better to add this to your ~/.bashrc file so you don't have to repeat the last step with every new terminal session:

```bash
echo 'export CPLUS_INCLUDE_PATH=/c/glfw-3.4.bin.WIN64/include/:/c/glew-2.1.0/include/' >> ~/.bashrc
echo 'export LIBRARY_PATH=/cygwin/c/glfw-3.4.bin.WIN64/lib-static-ucrt/:/cygwin/c/glew-2.1.0/bin/Release/x64/' >> ~/.bashrc
```

You will also need to copy glfw3.dll from the lib-static-ucrt folder of the distributable to the same folder that stores the Makefile

Once the setup above is complete, call `make`, and the demo should execute. 

If you have trouble, try something like the following (replacing paths for glew and glfw with the paths that you setup earlier):

```bash
g++ -std=c++17 -o test.exe test.cpp -g -D GLM_FORCE_SWIZZLE -D GLEW_STATIC -I ../inc/ -I /cygdrive/c/glew-2.1.0/include/ -I /cygdrive/c/glfw-3.4.bin.WIN64/include/ -L /cygdrive/c/glew-2.1.0/bin/Release/x64/ -L /cygdrive/c/glfw-3.4.bin.WIN64/lib-static-ucrt/ -lglfw3 -lopengl32 -lgdi32 -lglew32
```

### g++ in MinGW/MSYS

If you have trouble, try something like the following (replacing paths for glew and glfw with the paths that you setup earlier):

```bash
export CPLUS_INCLUDE_PATH=/cygdrive/c/glfw-3.4.bin.WIN64/include/:/cygdrive/c/glew-2.1.0/include/
export LIBRARY_PATH=/cygdrive/c/glfw-3.4.bin.WIN64/lib-mingw-w64/:/cygdrive/c/glew-2.1.0/bin/Release/x64/
```

```bash
g++ -std=c++17 -o test.exe test.cpp -g -D GLM_FORCE_SWIZZLE -D GLEW_STATIC -I ../inc/ -I /c/glew-2.1.0/include/ -I /c/glfw-3.4.bin.WIN64/include/ -L /c/glew-2.1.0/bin/Release/x64/ -L /c/glfw-3.4.bin.WIN64/lib-mingw-w64/ -lglfw3 -lopengl32 -lgdi32 -lglew32
```

### MSVC with Scons

# Design Methodology
tectonics.cpp is adheres to a strict design philosophy that is meant to prioritize functional programming concerns in a way that also leverages object-oriented paradigms. See `doc/general-design-approach.md` for more information.

# Subcomponents
tectonics.cpp can be broken down into several subcomponents. Users can pick and choose which subcomponents they'd like to include in their project at their discretion. Some optional dependencies exist between subcomponents, which are indicated clearly by the folder structure (e.g. `index/adapted/glm` stores behavior that requires `glm`). Using optional dependencies this way allows the use of functionality that wouldn't otherwise be possible without requiring people to copy over other unwanted components. 

The most important top-level folders are:

* `lib` for header-only 3rd party libraries
* `inc` for mature components that are ready for use by other code, such as for `math`, `grid`s, or scientific `unit`s and `model`s
* `sketch` for long-running prototypes and outlines that cannot be suitably handled by software branches
* `src` for code that is not expected to be useful to projects outside tectonics, such as the code for handling the UI that's used within `demo`
* `demo` for code that requires 3d libraries to run, usually to demonstrate functionality in the library
* `doc` for project-wide documentation that cannot be suitably put alongside any specific folder or file in the repository

Unit tests and documentation are placed alongside the folder or file they describe. File-specific documentation is stored in image or markdown files that share the same name as the thing it documents (e.g. `grid/dymaxion/Projection.svg` documents `grid/dymaxion/Projection.hpp`). Folder-specific documentation is stored in the folder it documents using one of several standardized names, in uppercase (e.g. `README.md`, `CATEGORY.png`, `DEPENDENCY.png`, etc.)

# Standards
* **Semantic Versioning 2.0.0**
* **"GLSL/GLM" Linear Algebra API**
* **"MKS/SI" International System of Units**
