Tectonics.cpp guarantees all core library functionality will go through at least a minimal set of unit tests, where appropriate.
This minimal set tests for the following physical, mathmatical, and computational properties. 
If a function is expected or required to have one of these properties, 
then a unit test must be written to demonstrate the function has that property for at least one circumstance.


functions
	determinism
	nondeterminism
	purity
	conceptual purity
	regularity

groups
	closure
	identity
	associativity
	invertibility
	commutativity
	distributivity

relations
	transitivity
	reflexivity
	symmetry

metric spaces
	nilpotence
	symmetry
	triangle inequality

geometric algebra
	idempotence
	antisymmetry

miscellaneous math
	idempotence
	orbit
	involutivity
	congruence
	monotonicity
	increasing
	decreasing
	adjoint
	domination
	subjugation
	theorems for free
	miscellaneous equivalence relations
	range restrictions

physics
	translational invariance
	rotational invariance
	resolution invariance
	mass conservation
	energy conservation
	reproducing real world values to within order of magnitude, factor of 2, 10%, etc.

definitions
	limiting cases
	degenerate cases
	special cases
	edge cases

performance
	precision requirements
	memory footprint size requirements

happy path (barring no other test)