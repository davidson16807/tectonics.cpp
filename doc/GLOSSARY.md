**model** A representation of business logic in memory, in contrast with the "view"

**view** A depiction of a model to the user

**simulation** a pure function that maps the state of a model at one point in time to another

**pure function** A function for which the return value can be determined strictly by the parameters.

**conceptually pure function** A function for which all inputs and outputs are displayed at the point of invocation, the memory footprint of input is not used to store output, and output after invocation is determined strictly from input in the method signature. A conceptually pure function can be reasoned with as if it were a pure function, even if it does not meet the strict definition of one. For instance, a conceptually pure function may use reference parameters to store output, and a conceptually pure method can use its owner as input or output. It is important to note that a conceptually pure function allows the behavior of the application to be predicted merely by looking at the line of code at which it is invoked, so a function is not conceptually pure if it accesses variables that are not displayed at invocation (e.g. global variables, or even attributes of referenced objects).

**conceptually pure method** a method that can be interpreted as a conceptually pure function

**operator** A conceptually pure function of the form 𝕋×𝕋×⋯×𝕋→𝕋.

**transformation** An operator of the form 𝕋→𝕋.

**predicate** A conceptually pure function of the form 𝕋→𝟚.

**derived attribute** A nonisomorphic conceptually pure function of the form 𝕋→S.

**category** A mathematical category. In the context of the application, most categories are small categories (that is to say, they can be depicted using weakly connected graphs, where all nodes are reachable from another if arrow direction is ignored). These categories are typically represented by a namespace or class for which methods or functions are conceptually pure and map between objects. 

**class category** A category whose arrows are represented by conceptually pure methods within a class. This can be useful if a category is implemented using methods or functions from another category, forming a dependency hierarchy. If this is the case, the constructor of the dependant class may require an instance of the dependancy, forming a category of categories. This is similar in practice to dependency injection in traditional object oriented programming, but with a much stronger mathematical underpinning.

**field** A field is an abstract concept that can be thought of as a map that has a value for every point in space. It can be approximated using a **raster**. The values stored may be numbers ("scalar field"), vectors ("vector field") or something else. 

**grid** A grid is a collection of interconnected entities (**vertices**, **cells**, etc.) together with a manner of indexing that maps ids for those entities to their positions in a 1d array. These positions can then be used to associate those entities to values in a **raster**. 

**raster** A raster is a collection of values that maps entities on a **grid**. It is an approximation of a mathematical **field**. A raster is stored using a simple std::vector wrapped in a `series` object. This is done to promote code reuse by avoiding unecessary data type conversion, and also to achieve polymorphism when using the Grid class. The values stored in the rasters may be numbers ("scalar raster"), vectors ("vector raster") or something else. 

**vertex** An entity on a **grid** that is associated with a single point in space.

**edge** A pair of vertices for which order doesn't matter.

**arrow** A tuple of two vertices, such that order matters.

**face** A collection of three vertices. Order typically does not matter, unless it is in a **buffer**, in which case it indicates a counter-clockwise motion around the surface normal when viewed from the front side of the face (reflecting the industry-standard "winding order").

**buffer** An **index**, usually a `std::vector<T>`, that is explicitly formatted for use with a graphics library such as OpenGL

**index** Any data structure or class that has a `operator[]` method, either one that refers to a location in memory or one that generates values procedurally