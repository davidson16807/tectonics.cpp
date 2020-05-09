This folder contains header files, for several libraries.

The following subcomponents are provided as part of the "libtectonics" suite:
* `many` for manipulating arrays of arithmetic values (optionally leverages `glm`)
* `rasters` for storing maps of values on the surface of 3d meshes (requires `many` and `glm`)
* `academics` for side-effect-free utility functions expressing simple scientific relationships (optionally leverages `glm` and `rasters`)
* `components` for loosely coupled, unopinionated data structures for deriving properties of celestial bodies from simple state variables like mass pools (requires `academics`, optionally leverages `glm` and `rasters`)

The following subcomponents are not provided as part of "libtectonics":
* `glm` is an optional dependency that can be used to provide enhanced functionality relating to linear algebra.
* `cache` is a unit testing framework that is used only for the development of the library.
