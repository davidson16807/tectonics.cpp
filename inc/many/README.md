The `many` subcomponent provides data structures and functions to easily handle large containers of primitive or simple data types, including `int`, `float`, or `glm::vec3`
We ideally want performing operations on containers to be as easy as performing operations on the individual data types themselves. 
The intention is to abstract away the arrays of primitives that are used to address data locality issues.

Some of the objectives we want to accomplish:
 1.) performant
       since this is all done to address data locality issues, which are chiefly performance concerns
 2.) easy to use
       since we want to perform operations easily, as stated previously
 3.) easy to reason with, extend, and control
       since we intend to reuse this functionality to represent rasters
 4.) easy to wrap
       since we must target webassembly as a deployment platform, and may also provide other wrappers
 5.) works well with glm 
       since we already depend on glsl for shader code, and glm allows us to reuse the knowledge base surrounding glsl

We could reuse existing libraries to accomplish some of these objectives (`xtensor`, `std::composite`, etc.) but none of them can easily achieve objectives #3, #4, and #5.
So we write our own solution. 

Our solution works in two ways:
 * It provides a set of functions for handling containers as if they were primitives or glm vectors. 
     To achieve #1 and #3, these functions work by passing output to reference parameters. 
 * It provides a data structure, `tmany<T>`, that makes use of operator overloads to provide convenience wrappers for
     the aforementioned functions.

