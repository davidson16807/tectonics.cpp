# libtectonics
## A C++ library for simulating worlds

**libtectonics** is a header-only C++ library that's intended to supplant the backend to [tectonics.js](http://davidson16807.github.io/tectonics.js/). It contains only the best parts of tectonics.js - the parts that either have proven out over time or have emerged with confidence as replacements through good design. 

This library is not just designed for use by tectonics.js. Numerous space simulators can make use of the functionality here. The components here are lightweight and loosely coupled. Parts of the library can be easily taken out, mixed and matched, and modified. The library is also rigorously covered by unit tests. It's a shame really if this library only gets used once. 

Permission to use the code is provided under the Creative Commons Attribution 4.0 license. You may modify and redistribute the code as you see fit. You need only attribute the work to me as the project developer, which I appreciate since it helps me develop professionally. Drop me a line if you do so since it's encouraging to hear my work is getting used! 

# Dependencies
Some functionality within the library makes use of dependencies. I make exclusive use of header-only libraries, and make sure to cordon off any code that uses the dependency from the rest of the library. You should never have to add a dependency to your project just because you want to use a feature in a library that makes no use of it.

I try to limit the use of dependencies, but there are times where dependencies are essential. The problems that face libtectonics often deal with complex subjects like linear algebra. As the developer, I want to limit project scope, so I don't want to go implement my own library, and as the user, you definitely don't want to familiarize yourself with some random guy's obscure linear algebra library. Everybody benefits from using a single common library. However, as with linear algebra libraries, there's often no standard implementation in C++, so we must choose from one of several existing dependencies. I have chosen to use the following dependencies, for the stated reasons:

* **glm** This library is a C++ implementation of the built in functions used within the glsl shading language. I use glm because libtectonics is meant for use in graphical space simulators, and I feel most users will already be familiar with at least one shading language that's similar to glsl. Using glm allows the reuse of a very well established set of conventions. If you've never used glm or glsl, it's worthwhile to start learning. Similar glm-derived libraries exist for other languages, too, so it's often very easy to port libtectonics code to other languages. 

* **catch** This library is used to provide unit tests for libtectonics. It is not required to use the library. 

# Subcomponents
libtectonics can be broken down into several subcomponents. Users can pick and choose which subcomponents they'd like to include in their project at their discretion. Some optional dependencies exist between subcomponents, which are indicated clearly by the #include path of the dependent code (e.g. `composites/glm` stores behavior that requires `glm`). Using optional dependencies allow the use of functionality that wouldn't otherwise be possible. 

* `composites` creates a series of templated data structures that wrap std::vector objects to enable arithmetic operations to be performed on their contents as if they were primitive data types like floats or ints. It also encourages the creation of performant code through a mindset of data oriented design. Other libraries exist that also fulfill this role, however they all fail for one reason or another. [std::valarray](https://en.cppreference.com/w/cpp/numeric/valarray) is a C++ standard, but it does not allow for storing glm data types and it has several severe restrictions relating to its flawed design history, and [xtensor](https://github.com/QuantStack/xtensor) is indeed a very beautiful library, but it is presently difficult to expose its data types to other platforms such as WebAssembly, and it comes with its own linear algebra implementation that does not leverage glm. 

** `composites/glm` creates data structures and operations that allow a `std::vector<glm::vec*>` to be manipulated as if it were itself a `glm:vec*`. 

* `rasters` handles operations involving scalar fields on the surface of unstructured 3d meshes. While pre-existing libraries exist that allow for vector calculus operations on unstructured meshes (e.g. [libMesh](https://libmesh.github.io/externalsoftware.html)), their scope goes way beyond the problem, they do not integrate with glm, and I feel there is need for a light-weight, header-only library that fulfills those requirements. This subcomponent requires `composites`. 

** `rasters/glm` handles operations involving vector fields on the surface of unstructured 3d meshes. It makes use of `glm` to represent vectors. 

* `academics` stores pure functions that express simple scientific relationships (e.g. geometric relations, black body equilibrium temperature, etc.). Because these functions are so simple, they can be easily ported to other C-like languages. To take advantage of this opportunity, the `academics` subcomponent is written using macros to allow it to be used in C, C++, and GLSL. Experimental macro definitions also exist to port code to Javascript, however no guarantees are made as to quality of output. 

** `academics/glm` stores pure functions that express simple scientific relationships involving vectors (e.g. orbital mechanics, rayleigh scattering). As with `academics`, macros exist to allow for use in C++, and GLSL. The C++ implementation requires the `glm` library. No macros exist to port the code to Javascript. 

** `academics/rasters` stores semantically pure functions that express simple scientific relationships involving scalar fields on the surface of 3d meshes (e.g. insolation). We say "semantically" pure because we still make use of output reference parameters for performance reasons. 

** `academics/rasters/glm` stores semantically pure functions involving vector fields on the surface of 3d meshes (e.g. advection). 

* `models/` stores simple data structures that express the properties of celestial bodies (oceans, atmospheres, etc.) from simple state variables like mass pools. Models exist in complete isolation of one another. This subcomponent requires `academics`.

* `models/glm` stores simple data structures that require the use of vectors (e.g. stellar systems). 

* `models/rasters` stores simple data structures that require the use of scalar rasters

* `models/rasters/glm` stores simple data structures that require the use of vector rasters (e.g. climate models, tectonic models)

# Design Principles
* **Side Effect Free Programming** The user should feel free to invoke methods without fear of unintentional state modification. All functions must be semantically pure - their output can be determined strictly using their input parameters. For practical reasons, we do allow the use of output reference parameters. This is done for performance reasons, to prevent the unnecessary allocation of memory for temp variables. This is why we say functions must be "semantically" pure. However, output reference parameters are clearly indicated by the lack of a "const" keyword. The intention is for users to be able to easily reason about their code. 
* **Limited Encapsulation** The user of a class is free to modify member attributes without fear of creating unintended side effects or invalid state. Encapsulation is only used to abstract away design details that are of no concern to the user, like pointer management, is_dirty flags, or representation in memory. 
* **Orthogonal, Extensive, State Variables** This goes along with "Limited Encapsulation." The user should feel free to modify member attributes without fear of generating invalid state. No two state variables should be bound to a relationship that requires one to change along with the other. It should at least be theoretically possible for any combination of state variable values to exist in the real world. It is intended that state variables to be chosen wisely, so there is never any need to encapsulate away mistakes at a later date. 
* **Extensive State Variables** Extensive state variables have a distinct advantage in that they are additive. This makes them great when designing models that must obey conservation laws. They are also great for deriving intensive properties, such as how density is derived from mass and volume, or refractive index is derived from particle count and refractivity. 
* **Separation of independent variables from derived variables** It should be clear both to users and developers as to which variables are derived from others. If a derived attribute can be calculated quickly, it is best to calculate it on the fly within a method of a class. If calculation takes longer, it must be calculated within a static method that is free from side effects, and it is up to the user to memoize that function's output.
* **Isolation of library dependencies** If a class requires the use of a library to perform some functionality, it should only contain functionality that requires that library. Other functionality that does not require that library must be placed within another, similarly named class. 
* **Loosely Coupled Classes** A common problem with Object Oriented Programming occurs when class attributes reference objects belonging to other classes. This results in a situation where, in order to use any single part of the library, you need to implement objects for every other class. This makes testing components extremely difficult. This has been aleviated somewhat through the use of dependency injection, but that still requires a class to have knowledge of another, or at least knowledge of its interface. It still makes it very difficult to rip out classes from the library and port them to other code bases. Our solution is to only use class references or dependency injection when absolutely necessary. 

# Standards
* **GNU C++ Standard Project Layout**
* **Semantic Versioning 2.0.0**
* **"GLSL/GLM" Linear Algebra API**
* **"MKS/SI" International System of Units**
