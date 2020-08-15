We wish to develop a data structure that represents "layered" 3 dimensional grids in which each vertex of the underlying mesh owns several vertically stratified layers. Let's call it "LayeredGrid", as a working title. Each layer can be thought of as a grid cell, which corresponds to its own unique value within a raster. It is assumed the layers are stratified along a dimension that is orthogonal to any dimensions represented by the mesh. 

There are several considerations we'd like to emphasize in this design. Ordered from highest to lowest priority, they are:

Memory Footprint
	We'd like the structure to be efficiently traversible, avoiding out-of-order memory access. 
Code Reuse
	We'd like to be able to reuse our existing code base to describe behavior using rasters based around a LayeredGrid
Run Time Flexibility
	We'd like the user to be able to dynamically change the number of layers during runtime

We have several design decisions to make. First, we must decide how dimensions are ordered within memory. We have two options: 

1.) **Vertical collection of horizontal structure**: Create a collection of several rasters, each one representing a vertical layer within the model
2.) **Horizontal structure of vertical collection**: Create a raster like data structure in which each cell is a collection of several vertical layers at a single place along the globe

Our highest priority is to minimize out-of-order memory access. Our most demanding tasks that involve out-of-order memory access are typically vector calculus operations like gradient or divergence. This new data structure will need to decompose these operations into two subcomponents: a out-of-order horizontal traversal, and an in-order vertical traversal. 

If we choose option 1.), the footprint of out-of-order horizontal traversals will be minimized, but our in-order vertical traversals will have to jump across huge regions of memory. This will nullify any benefit we got out of in order traversal. 

If we choose option 2.), the footprint of the out-of-order horizontal traversals will be larger, but they were out-of-order anyways, and our in-order vertical transfers will have an enormously smaller footprint, and therefore it will be much more efficient. 

We therefore choose option 2.). 

We now know we must design a horizontal structure of vertical collections. However, we have several options to pursue when implementing this. We may choose either a flat data storage method or a nested data storage method, and if we choose the latter we must choose whether to store the vertical collection as noncontiguous memory that's referenced using pointers or as contiguous memory that's constant in size. In other words, we must choose between the following options:

* `series<tprofile<T,N>>` 
	contiguous nested storage
* `series<series<T>>`
	noncontiguous nested storage
* `series<T>` 
	flat data storage

Here, `tprofile<T,N>` is a placeholder name for our vertical collection, representing a contiguous block of memory of constant size N. 

So let's look at our design considerations again and map them back to the options they allow:

Memory Footprint
	`series<tprofile<T,N>>`and`series<T>` can both be implemented in a contiguous block of memory.
	`series<series<T>>` is forbidden because it requires pointers everywhere to noncontiguous memory
Code Reuse
	`series<series<T>>` and `series<tprofile<T,N>>`may allow us to reuse existing code, if possible (see comments below).
	`series<T>` is forbidden because it would require existing code to have intimate knowledge of this data structure.
Run Time Flexibility
	`series<series<T>>` and `series<T>` can both be implemented in a way that allows resizing the block of memory. 
	`series<tprofile<T,N>>` is forbidden because the size of `N` must be known at compile time

Our foremost concern is memory footprint, so we will dismiss `series<series<T>>` out of hand for that reason. That leaves `series<tprofile<T,N>>` and `series<T>`. 

<work-in-progress>
	Before we continue, we will note one extra thing about our "Code Reuse" consideration. Let's examine our existing code base concerning spatially aware raster operations. We will group them by the types of Grid they use:

	Grid
		dilate, erode, opening, closing

	SpheroidGrid
		gradient, divergence, curl, laplacian, random
</work-in-progress>