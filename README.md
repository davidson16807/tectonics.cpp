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

In order to create diagrams where commutativity is meaningful, it sometimes helps to remove certain function parameters from consideration and treat them as constants within a given category. In this case a value for the parameter might be fed into the constructor of the class that represents the category, in which case the parameter becomes a private `const` member. This evokes [encapsulation](https://en.wikipedia.org/wiki/Encapsulation_(computer_programming)) within object oriented programming, but since the member is const, the approach strictly avoids the problems that arise from that methodology due to state management. Another way to view this is to see the class as a cohesive set of functions that have collectively undergone [currying](https://en.wikipedia.org/wiki/Currying). 

# Subcomponents
tectonics.cpp can be broken down into several subcomponents. Users can pick and choose which subcomponents they'd like to include in their project at their discretion. Some optional dependencies exist between subcomponents, which are indicated clearly by the #include path of the dependent code (e.g. `many/glm` stores behavior that requires `glm`). Using optional dependencies allow the use of functionality that wouldn't otherwise be possible. 

* `series` creates a series of templated functions and data structures that wrap `std::vector<T>` objects to enable arithmetic operations to be performed on their contents as if they were primitive data types like a `float` or an `int`. It is meant to foster performant code through data oriented design. Other libraries exist that fulfill this role (e.g. [std::valarray](https://en.cppreference.com/w/cpp/numeric/composite) or [xtensor](https://github.com/QuantStack/xtensor)), however there are reasons we avoid these libraries. See documentation under `inc/many/README.md` for details.

	- `many/glm` creates pure functions that allow a `std::vector<glm::vec*>` to be manipulated as if it were itself a `glm:vec*`. 

* `rasters` handles operations involving scalar fields on the surface of unstructured 3d meshes. While pre-existing libraries exist that allow for vector calculus operations on unstructured meshes (e.g. [libMesh](https://libmesh.github.io/externalsoftware.html)), their scope goes way beyond the problem, they do not integrate with glm, and I feel there is need for a light-weight, header-only library that fulfills those requirements. This subcomponent requires both `series` and `glm`. 

	- `rasters/glm` handles operations involving vector fields on the surface of unstructured 3d meshes. 

* `academics` stores pure, "side-effect-free" utility functions that express simple scientific relationships (e.g. geometric relations, thermodynamics, orbital mechanics, etc.). Because these functions are so simple, they can be easily ported to other C-like languages. To take advantage of this opportunity, the `academics` subcomponent is written using macros to allow it to be used in C, C++, and GLSL. Experimental macro definitions also exist to port code to Javascript, however no guarantees are made as to quality of output. 

	- `academics/glm` stores pure utility functions that express simple scientific relationships involving vectors (e.g. orbital mechanics, rayleigh scattering). As with `academics`, macros exist to allow for use in C++, and GLSL. The C++ implementation requires the `glm` library. No macros exist to port the code to Javascript. 

	- `academics/rasters` stores semantically pure utility functions that express simple scientific relationships involving scalar fields on the surface of 3d meshes (e.g. insolation). We say "semantically" pure due to the use of output reference parameters for performance reasons. However, these output reference parameters are clearly indicated by their "non-const" nature.

* `components/` stores loosely coupled, unopinionated data structures that express the properties of celestial bodies (oceans, atmospheres, etc.) from simple state variables like mass pools. Models exist in complete isolation of one another. This subcomponent requires `academics`.

* `components/glm` stores data structures that require the use of vectors (e.g. stellar systems). 

* `components/rasters` stores data structures that require the use of scalar rasters.

# Standards
* **Semantic Versioning 2.0.0**
* **"GLSL/GLM" Linear Algebra API**
* **"MKS/SI" International System of Units**
