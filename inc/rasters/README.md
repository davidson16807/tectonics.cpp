The `rasters` subcomponent provides a data structure for representing scalar or vector fields on the surface of a 3d mesh. This data structure, known as `raster<T>`, extends the `many<T>` class within the `composites` subcomponent to include references to a `Grid` class. The `Grid` class stores information about the vertices, edges, and faces of a 3d mesh. Values within the raster are stored within a `std::vector<T>` object where each index represents a cell within the grid. 

This subcomponent requires the use of `composites`. Additionally, `glm` can be optionally used to provide enhanced functionality relating to vector fields.