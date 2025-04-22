`model/` contains namespaces for data structures and calculations that represent concepts in science. Some more fundamental namespaces, such as `compound` and `mix`, have broad practical use outside of simulating planets. Others namespaces, such as `rock`, contain representations of state that are suitable for planet simulations that must update in real time on consumer hardware. As a result, these namespaces introduce more assumptions than most folders under `inc/`, typically to achieve fast approximations and small storage footprints. 

`model/` includes the following namespaces (loosely ordered from most to least fundamental):

* `compound::` for calculating properties of arbitrary chemical compounds at arbitrary pressures, temperatures, wavenumbers, and phases

* `mix::` for calculating properties of arbitrary chemical mixtures

* `rock::` for storing the state of a planet's crust within a real time simulation that runs on consumer hardware

The following namespaces are planned, or at the very least, are currently very underdeveloped:

* `air` for modeling the composition and properties of gaseous mass pools on a world

* `sea` for modeling the composition and properties of liquid mass pools on a world

* `star` for modeling the composition and properties of bodies that fuse ordinary hydrogen through gravity

* `orbit` for modeling trajectories of gravitationally bound bodies
