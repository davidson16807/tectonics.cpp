# tectonics.cpp
## A C++ library for simulating worlds

**tectonics.cpp** is a header-only C++ library that's intended to supplant the backend to [tectonics.js](http://davidson16807.github.io/tectonics.js/). It contains only the best parts of tectonics.js - the parts that either have proven out over time or have emerged with confidence as replacements through good design. 

This library is not just designed for use by tectonics.js. Numerous space simulators can make use of the functionality here. The components here are lightweight and loosely coupled. Parts of the library can be easily taken out, mixed and matched, and modified. The library is also rigorously covered by unit tests. It's a shame really if this library only gets used once. 

Permission to use the code is provided under the Creative Commons Attribution 4.0 license. You may modify and redistribute the code as you see fit. You need only attribute the work to me as the project developer, which I appreciate since it helps me develop professionally. Drop me a line if you do so since it's encouraging to hear my work is getting used! 

# Getting Started
Several make file commands exist for building and testing the library. See `Makefile` for details. 

Running the graphics demo requires two other libraries to be installed on your machine: [GLEW](http://glew.sourceforge.net/) and [GLFW](https://www.glfw.org/). On Ubuntu you can install both with the following commands:

`
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
`

# Design Methodology
The design methodology used by the library adheres to principles in both functional and object oriented programming and can be best described using concepts from [category theory](https://en.wikipedia.org/wiki/Category_theory). Design begins by decomposing behavior into a [commutative diagram](https://en.wikipedia.org/wiki/Commutative_diagram) expressing a category, typically where objects are data structures and arrows can be treated as [pure functions](https://en.wikipedia.org/wiki/Pure_function) mapping between them. If two data structures represent the same concept but are each only able to support distinct functionality, then both data structures are implemented and a function is provided to map between them. If a mapping is needed but none can be found, then mappings are found that come closer to the desired mapping, until the desired mapping becomes apparent through function composition. Data structures are consolidated if a single data structure can be found that accounts for the behavior of both without introducing redundancy or a loss of functionality. 

Categories are implemented either as namespaces or classes. If the category is implemented as a class, then object oriented [polymorphism](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) provides a natural way to implement [functors](https://en.wikipedia.org/wiki/Functor) between categories. Oftentimes, the implementation of a category rests on the composition of the functions and objects from another underlying category, in which case there is a category of categories that describes dependencies between them. Arrows within this category of categories are then implemented as the constructors of classes that represent categories, which evokes the behavior of [dependency injection](https://en.wikipedia.org/wiki/Dependency_injection) within object oriented programming, but allows this design to be arrived at in a strictly procedural manner. 

In order to create diagrams where commutativity is meaningful, it sometimes helps to remove certain function parameters from consideration and treat them as constants within a given category. In this case a value for the parameter might be fed into the constructor of the class that represents the category, in which case the parameter becomes a private `const` member. This evokes [encapsulation](https://en.wikipedia.org/wiki/Encapsulation_(computer_programming)) within object oriented programming, but since the member is `const`, the approach strictly avoids the problems that arise from that methodology due to state management. Another way to view this is to see the class as a cohesive set of functions that have collectively undergone [currying](https://en.wikipedia.org/wiki/Currying). 

# Subcomponents
tectonics.cpp can be broken down into several subcomponents. Users can pick and choose which subcomponents they'd like to include in their project at their discretion. Some optional dependencies exist between subcomponents, which are indicated clearly by the folder structure (e.g. `many/glm` stores behavior that requires `glm`). Using optional dependencies this way allows the use of functionality that wouldn't otherwise be possible without requiring people to copy over other unwanted components. 

The most important top-level folders are:

* `lib` for header-only 3rd party libraries
* `inc` for mature components that are ready for use by other code, such as for `math`, `grid`s, or scientific `unit`s and `model`s
* `sketch` for long-running experimental code that cannot be suitably handled by software branches
* `src` for code that is not expected to be useful to projects outside tectonics, such as the code for handling the UI that's used within `demo`
* `demo` for code that requires a specific 3d library to run, usually to demonstrate functionality in the library
* `doc` for project-wide documentation that cannot be suitably put alongside any specific folder or file in the repository

# Standards
* **Semantic Versioning 2.0.0**
* **"GLSL/GLM" Linear Algebra API**
* **"MKS/SI" International System of Units**
