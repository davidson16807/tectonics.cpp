The "orbit" namespace exclusively represents data structures and functions that define mathematical categories, 
representing gravitationally bound orbits.

The data structures are as follows:
* **Elements** representing elliptical orbits using the 6-Element formulation
* **Universals** representing both hyperbolic and elliptical orbits using the Universal Variable Formulation

The categories are as follows:
* **Properties** representing maps between properties of an orbit
* **ElementAndState** representing a bijection between representations using the 6-Element formulation and the Universal Variable Formulation

All classes represent categories. Methods are conceptually pure, meaning their output is determined strictly by their parameters and the properties of `this`. 