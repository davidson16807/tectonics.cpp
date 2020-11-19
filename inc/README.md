This folder contains subcomponents of the "tectonics.cpp" suite:
* `series` for manipulating arrays of arithmetic values (optionally leverages `glm`)
* `rasters` for storing maps of values on the surface of 3d meshes (requires `series` and `glm`)
* `academics` for side-effect-free utility functions expressing simple mathematic and scientific relationships ported from glsl (optionally leverages `glm` and `rasters`)
* `components` for loosely coupled, unopinionated data structures for deriving properties of celestial bodies from simple state variables like mass pools (requires `academics`, optionally leverages `glm` and `rasters`)

See `lib` for 3rd party libraries.