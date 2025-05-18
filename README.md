# tectonics.cpp
## A C++ library for simulating worlds

**tectonics.cpp** is a header-only C++ library that's intended to supplant the backend to [tectonics.js](http://davidson16807.github.io/tectonics.js/). It contains only the best parts of tectonics.js - the parts that either have proven out over time or have emerged with confidence as replacements through good design. 

This library is not just designed for use by tectonics.js. Numerous space simulators can make use of the functionality here. The components here are lightweight and loosely coupled. Parts of the library can be easily taken out, mixed and matched, and modified. The library is also rigorously covered by unit tests. It's a shame really if this library only gets used once. 

As of completing major milestones in May 2025, permission to use all code going forward is provided under the Creative Commons Attribution-NonCommercial-Sharealike 4.0 license. This code is written with fellow hobby developers in mind, and noncommercial hobby projects are always welcome to use it freely. Drop me a line if you use it, since it's encouraging to hear my work is getting used, and it's always fun to hear from someone with fellow interests! Commercial licensing is also available by prior agreement - drop me a line if interested. 

# Getting Started
Make file commands exist throuhgout the library for building demos and running unit tests. Simply run `make` in any project folder with a `Makefile` to run all the functionality that's defined within that folder. 

Building with scons is also supported. Simply run `scons -u` from any project folder to run the functionality that's defined within that folder. 

Running the graphics demo requires two other libraries to be installed on your machine: [GLEW](http://glew.sourceforge.net/) and [GLFW](https://www.glfw.org/). On Ubuntu you can install both with the following commands:

`
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
`

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
