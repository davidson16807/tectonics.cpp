The `rasters` subcomponent provides pure functions for representing scalar or vector fields. These functions accept `series<T>` objects as input, which represent values that map to vertices within a `Grid` object that is also passed as input.  This `Grid` object stores information about the vertices, edges, and faces of a 3d mesh. 

This subcomponent requires the use of `many` and `glm`. 