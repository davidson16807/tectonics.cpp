This folder contains unit tests. Every component that is incorporated into tectonics.cpp goes through unit testing. Unit tests are made possible using the [catch](https://github.com/catchorg/Catch2) framework. 

Where applicable, a feature needs the following tests at minimum to be taken outside the `spike` folder:
* Tests for idempotence, associativity, commutativity or invertability, if the feature can be described as an algabraic structure with those respective properties.
* Tests for invertibility if the feature can be described as a categorical isomorphism.
* Tests for happy path behavior
* Tests for edge case behavior, if the feature is a non-total function
* Tests for reproducing basic facts about celestial bodies to within at least an order of magnitude, if the feature can easily describe those facts.

These unit tests only care about capturing obvious errors in model behavior. They are not meant to perform model validation. They must be written in a way that allows model output to change liberally. 