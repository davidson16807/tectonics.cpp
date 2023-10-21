`procedural::` contains operations that can be performed on procedural grids,
where procedural grids are defined by the following properties:
* they require no memory to store properties of individual cells
* they permit a map, point⟶vertex, that maps any point in space to the nearest vertex
* they permit a map, vertex×id⟶vertex, that maps any vertex to one of their neighbors
* they share a interface that's analogous to `collignon::Grid.hpp` or `dymaxion::Grid.hpp`