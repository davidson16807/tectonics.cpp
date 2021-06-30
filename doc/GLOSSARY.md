**pure function** A function for which the return value can be determined strictly by the parameters.

**conceptually pure function** A function for which all inputs and outputs are displayed at the point of invocation, the memory footprint of input is not used to store output, and output after invocation is determined strictly from input in the method signature. A conceptually pure function can be reasoned with as if it were a pure function, even if it does not meet the strict definition of one. For instance, a conceptually pure function may use reference parameters to store output, and a conceptually pure method can use its owner as input or output. It is important to note that a conceptually pure function allows the behavior of the application to be predicted merely by looking at the line of code at which it is invoked, so a function is not conceptually pure if it accesses variables that are not displayed at invocation (e.g. global variables, or even attributes of referenced objects).

**conceptually pure method** a method that can be interpreted as a conceptually pure function

**operator** A conceptually pure function of the form T×T×⋯×T→T.

**transformation** An operator of the form T→T.

**predicate** A conceptually pure function of the form T→bool.

**derived attribute** A nonisomorphic conceptually pure function of the form T→S.

**category** A mathematical category. In the context of the application, most categories are small categories (that is to say, they can be depicted using weakly connected graphs, where all nodes are reachable from another if arrow direction is ignored). These categories are typically represented by a namespace or class for which methods or functions are conceptually pure and map between objects. 

**class category** A category whose arrows are represented by conceptually pure methods within a class. This can be useful if a category is implemented using methods or functions from another category, forming a dependency hierarchy. If this is the case, the constructor of the dependant class may require an instance of the dependancy, forming a category of categories. This is similar in practice to dependency injection in traditional object oriented programming, but with a much stronger mathematical underpinning.

**class homset** A homset within a category where arrows are implemented as conceptually pure methods within a class. This can be useful as it provides tha advantages of both class categories and namespace categories, minimizing the number of imports that are needed to make use of functionality while still allowing dependencies to be modeled using constructors. s

**namespace category** A category whose arrows are represented by conceptually pure functions within a namespace. This can be useful to minimize the number imports that are needed to make use of functionality. In this case, each homset within the category can be represented by a single file, so the requirements for each file are minimized to that of the two objects being mapped, and the contents of each file are maximized to provide as much functionality as can be created by introducing its requirement.

**field** A field is an abstract concept that can be thought of as a map that has a value for every point in space. It can be approximated using a **raster**. The values stored may be numbers ("scalar field"), vectors ("vector field") or something else. 

**grid** A grid is a collection of interconnected grid cells that are intended to store values in a **raster**. It may be represented by an underlying **mesh** in which grid cells are represented by vertices, or there may be a more complex relationship, where a vertex in a mesh corresponds to several grid cells (see `LayeredGrid` for an example). A grid caches additional attributes that may be used when performing operations on rasters. These additional attributes are derived from vertices and faces. Another term for it may be a "mesh cache"

**mesh** A mesh is a collection of interconnected vertices. It is a tuple of vertices and faces. Vertices are stored as vec3s representing positions in a 3d cartesian coordinate system. Faces are stored as uvec3s representing the indices of vertices that are interconnected as a face within the mesh. A mesh is a simple data structure. It does not contain any additional attributes that are derived from vertices and faces. This is a job for **grid**

**raster** A raster is a collection of values that map to cells on a **grid**. It is an approximation of a mathematical **field**. A raster is stored using a simple std::vector wrapped in a `series` object. This is done to promote code reuse by avoiding unecessary data type conversion, and also to achieve polymorphism when using the Grid class. The values stored in the rasters may be numbers ("scalar raster"), vectors ("vector raster") or something else. 
