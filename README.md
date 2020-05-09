# tectonics.cpp
## A C++ library for simulating worlds

**tectonics.cpp** is a header-only C++ library that's intended to supplant the backend to [tectonics.js](http://davidson16807.github.io/tectonics.js/). It contains only the best parts of tectonics.js - the parts that either have proven out over time or have emerged with confidence as replacements through good design. 

This library is not just designed for use by tectonics.js. Numerous space simulators can make use of the functionality here. The components here are lightweight and loosely coupled. Parts of the library can be easily taken out, mixed and matched, and modified. The library is also rigorously covered by unit tests. It's a shame really if this library only gets used once. 

Permission to use the code is provided under the Creative Commons Attribution 4.0 license. You may modify and redistribute the code as you see fit. You need only attribute the work to me as the project developer, which I appreciate since it helps me develop professionally. Drop me a line if you do so since it's encouraging to hear my work is getting used! 

# Subcomponents
libtectonics can be broken down into several subcomponents. Users can pick and choose which subcomponents they'd like to include in their project at their discretion. Some optional dependencies exist between subcomponents, which are indicated clearly by the #include path of the dependent code (e.g. `many/glm` stores behavior that requires `glm`). Using optional dependencies allow the use of functionality that wouldn't otherwise be possible. 

* `many` creates a series of templated functions and data structures that wrap `std::vector<T>` objects to enable arithmetic operations to be performed on their contents as if they were primitive data types like a `float` or an `int`. It is meant to foster performant code through data oriented design. Other libraries exist that fulfill this role (e.g. [std::valarray](https://en.cppreference.com/w/cpp/numeric/composite) or [xtensor](https://github.com/QuantStack/xtensor)), however there are reasons we avoid these libraries. See documentation under `inc/many/README.md` for details.

	- `many/glm` creates pure functions that allow a `std::vector<glm::vec*>` to be manipulated as if it were itself a `glm:vec*`. 

* `rasters` handles operations involving scalar fields on the surface of unstructured 3d meshes. While pre-existing libraries exist that allow for vector calculus operations on unstructured meshes (e.g. [libMesh](https://libmesh.github.io/externalsoftware.html)), their scope goes way beyond the problem, they do not integrate with glm, and I feel there is need for a light-weight, header-only library that fulfills those requirements. This subcomponent requires both `many` and `glm`. 

	- `rasters/glm` handles operations involving vector fields on the surface of unstructured 3d meshes. 

* `academics` stores pure, "side-effect-free" utility functions that express simple scientific relationships (e.g. geometric relations, thermodynamics, orbital mechanics, etc.). Because these functions are so simple, they can be easily ported to other C-like languages. To take advantage of this opportunity, the `academics` subcomponent is written using macros to allow it to be used in C, C++, and GLSL. Experimental macro definitions also exist to port code to Javascript, however no guarantees are made as to quality of output. 

	- `academics/glm` stores pure utility functions that express simple scientific relationships involving vectors (e.g. orbital mechanics, rayleigh scattering). As with `academics`, macros exist to allow for use in C++, and GLSL. The C++ implementation requires the `glm` library. No macros exist to port the code to Javascript. 

	- `academics/rasters` stores semantically pure utility functions that express simple scientific relationships involving scalar fields on the surface of 3d meshes (e.g. insolation). We say "semantically" pure due to the use of output reference parameters for performance reasons. However, these output reference parameters are clearly indicated by their "non-const" nature.

* `components/` stores loosely coupled, unopinionated data structures that express the properties of celestial bodies (oceans, atmospheres, etc.) from simple state variables like mass pools. Models exist in complete isolation of one another. This subcomponent requires `academics`.

* `components/glm` stores data structures that require the use of vectors (e.g. stellar systems). 

* `components/rasters` stores data structures that require the use of scalar rasters.

# Design Principles
* **Side Effect Free Programming** The user should feel free to invoke methods without fear of unintentional state modification. All functions must be semantically pure - their output can be determined strictly using their input parameters. For practical reasons, we do allow the use of output reference parameters. This is done for performance reasons, to prevent the unnecessary allocation of memory for temp variables. This is why we say functions must be "semantically" pure. However, output reference parameters are clearly indicated by the lack of a "const" keyword. The intention is for users to be able to easily reason about their code. 
* **Orthogonal, Extensive, State Variables** The user should feel free to modify member attributes without fear of generating invalid state. This means state variable should be orthogonal - any state variable can be changed without having to change another. It should be theoretically possible for any combination of state variable values to exist in the real world. Wherever possible, state variables should also be extensive, to allow them to be added together. This makes them great when designing models that must obey conservation laws. They are also great for deriving intensive properties, such as how density is derived from mass and volume, or refractive index is derived from particle count and refractivity. 
* **Separation of independent variables from derived variables** It should be clear both to users and developers as to which variables are derived from others. If a derived attribute can be calculated quickly, it is best to calculate it on the fly within a method of a class. If calculation takes longer, it must be calculated within a static method that is free from side effects, and it is up to the user to memoize that function's output.
* **Isolation of library dependencies** If a class requires the use of a library to perform some functionality, it should only contain functionality that requires that library. Other functionality that does not require that library must be placed within another, similarly named class. 
* **Loosely Coupled Classes** A common problem with Object Oriented Programming occurs when class attributes reference objects belonging to other classes. This results in a situation where, in order to use any single part of the library, you need to implement objects for every other class. This makes testing components extremely difficult. This has been aleviated somewhat through the use of dependency injection, but that still requires a class to at least have knowledge of an interface. It still makes it very difficult to rip out classes from the library and port them to other code bases. Our solution is to use class references or dependency injection only when absolutely necessary, and to otherwise pass attributes of dependant classes as parameters to an object's methods. 

# Standards
* **Semantic Versioning 2.0.0**
* **"GLSL/GLM" Linear Algebra API**
* **"MKS/SI" International System of Units**
