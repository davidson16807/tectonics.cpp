The `inc/` folder stores mature subcomponents of the "tectonics.cpp" suite. 
Maturity is defined by the following criteria:
* code is unit tested as thoroughly as can be done to test for mathematical properties as outlined in `doc/basic-unit-tests.md`
* code is ready to be relied upon (if not already relied upon) by code elsewhere, such as in `spike/`, `src/`,  `demo/`, `inc/` itself, or even other projects.

Code within `inc/` is typically less subject to change, however this is not a strict requirement for inclusion.

`inc/` includes the following subfolders:
* `test/` for simplifying the writing of unit tests that adhere to our test philosophy, see `doc/basic-unit-tests.md`
* `math/` for functions and structures/classes with `operator()` methods that map ℝ→𝕋 (`glm` is optional)
* `index/` for structures/classes with `operator[]` methods that map ℕ→𝕋 (`glm` is optional)
* `field/` for functions and structures/classes with `operator()` methods that map ℝ²→𝕋 and ℝ³→𝕋, usually representing a function over a physical space (`glm` is required)
* `grid/` for data structures, methods, and `index`es that involve the properties of graph-like structures (`glm` is required, `buffer` is optional)
* `raster/` for spatially-aware operators that act on `raster`s, which are `index`es where elements are associated with entities in `grid`s like vertices, edges, etc. (`grid` is required)
* `buffer/` for methods that transfer the contents of `raster`s into `buffer`s, which are `index`es that are formatted for consumption by graphics libraries (`glm` is required)
* `units/` for classes that represent systems of measurement and enforce type-safe dimensionality
* `relation/` for classes that represent relations between quantities of various dimensionality
* `model/` for data structures that represent concepts in science, while also including their operators and the maps that exist between them (requirements depend on the model, see there for details)

the following shorthands are used above:
* ℝ indicates floating point values
* ℕ indicates positive integers
* 𝕋 indicates some arbitrary type
