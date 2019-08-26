This folder contains header files. Header files for libraries are placed within subdirectories named after the respective library. 

The following subcomponents are provided as part of the "libtectonics" suite:
	* `many` for manipulating arrays of arithmetic values
	* `rasters` for storing maps of values on the surface of 3d meshes (requires `many`)
	* `academics` for side-effect-free utility functions expressing simple scientific relationships
	* `components` for loosely coupled, unopinionated data structures for deriving properties of celestial bodies from simple state variables like mass pools (requires `academics`)

`glm` is an optional dependency that can be used to provide enhanced functionality relating to linear algebra.
`cache` is a unit testing framework that is used only for the development of the library.
