# GEOMETRY LIBRARY

## DESCRIPTION
This folder contains a basic geometry library for use in shaders and other C-style languages. 
It is motivated for use in raycasting, raytracing, and geometric analysis (e.g. determining phase from phase diagrams)
As a result, the library supports operations in both 2d and 3d and is limited to the following tasks:
* **mensuration**: finding perimeters, areas, surface areas, and volumes of primitive shapes
* **point-shape intersection**: testing whether points intersect with primitive shapes
* **signed distance functions**: finding signed distances from points to primitive shapes
* **surface normal calculation**: finding surface normals of primitive shapes as the gradient of their signed distance function
* **line-shape intersection** (work in progress): finding signed distances along lines to the intersection with primitive shapes
* **line-csg intersection** (work in progress): finding signed distances along lines to the intersection with shapes formed by constructive solid geometry

## DESIGN PRINCIPLES:
* **Functionally pure**: 
  Output is determined solely from the input, global variables are never used
   "underscored_lower_case" is used to indicate functional purity
* **Performant**:
  Suitable for use in shader code
* **Compositional**:
  Given several functions that return intersections for a simpler geometry,
   it should be easy and performant to construct similar functions for more complex geometry.
* **Limited Scope**:
  The library should only implement the minimum functionality needed to satisfy 
   the use cases that were mentioned above.
  This implies limiting scope to point-shape and line-shape intersections
  Implementing other kinds of intersections (e.g. plane-shape or shape-shape)
   is likely to require far more complex functionality that is not likely to prove useful,
   and may even exceed what's allowed within shader code
* **Language agnostic**: 
  Supports C and GLSL. Leverages language-specific paradigms (e.g. classes, templates) 
  but uses glsl/glm libraries to standardize implementation.
  Translations for GLSL can be found in tectonics.glsl.
  Transpilers to other languages are available under the tectonics.cpp/tool/ folder.
* **Versatile**:
  Avoids use of custom data structures. 
  Specifies shapes by vector and scalar parameters. 
  Intercompatible user-defined data structures without requiring the user to copy or convert data. , unless doing so requires a
   language specific concept such as pass-by-reference.
* **Obvious**: 
  Behavior can be deduced from the function signature alone,
  Arguments are described in comments when brevity is needed,
  Function names err on the side of verbosity, 
  Function names explain their exact intent,
  They do not abbreviate and do not avoid using prepositions or conjunctions
  Function names can be reconstructed after seeing only a few examples
  Behavior is consistant across all functions expressing geometric relations
  Spatial input makes no assumptions about the reference frame, 
  No concepts or standards need to be introduced to the user before usage (e.g. function chaining, sentinel values, other standards)
  No data structures need to be introduced to the user before usage, with the exception of vecNs

## CONVENTIONS:
* Function names are verbose and include prepositions and conjunctions
* Parameter names are verbose
* Variable names are single letter when brevity is needed
* Vector names are single letter, upper case, with lower case subscripts (e.g. `A`)
* Scalar names are single letter, lower case, with lower case subscripts (e.g. `a`)
* Dot or cross products are denoted by concatenating vector names 
   (e.g. `AB = dot(A,B)`, `AxB = cross(A,B)` )
