`index/` contains functions that operate on collections, such as vectors and arrays. This is typically used in tectonics.cpp to quickly test and debug operations on things like rasters and trees in a way that can easily handle a variety of singleton data types. However it is also designed to allow use in production code. Usage of functions in `index/` in production code is acceptable or even desireable if output collections are going to be stored or reused, but if you find yourself storing a lot of collections that are only used as intermediate calculations, consider migrating that code to use a traditional for loop - smaller memory footprints typically result in faster code.

All methods act as conceptually pure functions, where the content of output reference parameters after invocation are a strict function of arguments from the method and its class's constructor. In this sense methods act like curried functions. Classes merely exist since it is a performant way for operations to be adapted to many kinds of singleton data types using the "Adapter" pattern. Classes generally correspond to abstract mathematical structures like Orders, Groups, or Metrics.

Collections may represent multidimensional data but data must be stored 1 dimensionally. Multiple namespaces exist within `index/` to handle data that is conceptually not 1-dimensional, and analogous code outside of `index/` handles data for even more specialized stuctures, such as `raster/`. 

Namespaces within `index/` include:

* `adapted::` functions that operate on singletons in a standardized way so that other namespaces can handle collections for many types of singleton data (e.g. `si::` units, `glm::` vectors, etc.)
* `iterated::` functions that calculate a singleton for each singleton in one or more collections of the same size (e.g. `add`, `greater_than`, `cos`, etc.)
* `aggregated::` functions that calculate a singleton from a collection (e.g. `max`, `count`, `sum`, etc.)
* `branched::` functions that are analogous to `aggregated::` that calculate a singleton for each branch in a tree of a standard 1-dimensional representation
* `inherited::` functions that are analogous to `aggregated::` that calculate a singleton for each depth traversal in a tree of the same representation used by `branched::`
* `grouped::` functions that are analogous to `aggregated::` that calculate a singleton for each group in a collection of groups that are represented 1-dimensionally
* `known::` classes that represent functions that are analogous to `iterated::` but are optimized for use when all arguments are known procedurally and require no storage. Always use this if and only if all arguments can be described procedurally.
* `procedural::` classes that represent collections that are known procedurally and require no storage. Always try to use this if the collection can be described procedurally.
* `whole::` Deprecated. It represents and earlier attempt to implement the `aggregated::` namespace of this library, but it could not easily handle other data types like `si::` units or `glm::` vectors

