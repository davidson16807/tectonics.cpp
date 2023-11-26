dymaxion:: contains functions that store properties of rasters 
to special kinds of series (known as "buffers")	that are typically sent to the GPU in 3d rendering. 
The name "buffer" is evocative of the buffers that are used in OpenGL. 

Design of dymaxion::buffers is informed by the following observations:

1)  Tectonics.cpp must allow use with wrappers such as WebAssembly and WebGL
2)  geometry shaders are unsupported in WebGL
3)  Due to points 1) and 2), Tectonics.cpp must allow some way to send procedural grids to buffer memory
4)  Buffer memory has unique structure requirements that are imposed by the graphics library,
    e.g. vector buffers are flat lists of x/y/z attributes and vectors are duplicated once per triangle
5)  For the above reasons, buffer storage is much more costly than any procedural grid they may represent
6)  Storing to buffers can be accomplished by an in-order memory traversal 
7)  For all the reasons above, there are benefits to performance and ease of implementation 
    if temporary variables are stored across multiple indices.
    To illustrate, due to vector buffer structure in point 4), and in-order traversal in point 6),
    temporary variables can be used to avoid recalculating the same vector once for each component and simplify code.
8)  Due to point 7), buffers are stored using an approach that's similar to the one we use for `rasters::`,
    and not the procedural `operator[]` overload approach that we use for `series::`.
    This involves functions that store to buffers as output reference parameters.
9)  Within `rasters::`, since we want to improve our ability to reason with model logic, 
    it's best to store output all at once. However in 3d rendering, we may only care to render a portion of a sphere 
    (e.g. if the user is standing on the surface of a world)
10) Due to points 5) and 9), knowledge of grid representation is necessary to easily implement a performance oriented solution
    (e.g. dymaxion::Grid is composed of squares, so only a few squares may need to be loaded at high resolution)
    This is in contrast to `rasters::`, where knowledge of the grid has little affect on performance, 
    so it is best to be grid-agnostic (barring knowledge of certain types of grid where necessary, e.g. layered vs. unlayered).
11) Combining points 5), 9), and 10), it is useful for buffer storage to have knowledge of the grid,
    so we represent buffer storage in a different namespace that is grid specific, e.g. `dymaxion::`.
12) Due to point 1), method signatures for buffer storage should not involve pointers or output references to primitives,
    even though doing so may be useful. This is why method signatures look the way they do.
