This folder contains unit tests. Every component that is incorporated into libtectonics goes through unit testing. Unit tests are made possible using the [catch](https://github.com/catchorg/Catch2) framework. 

Unit tests check for utility code check for algabraic properties such as idempotence, associativity, or invertability, in addition to happy path and edge case behavior. 

Unit tests for components such as for the ocean or atmosphere work by reproducing basic facts about Earth or the other bodies in the solar sytem, to within at least an order of magnitude. These unit tests only care about capturing obvious errors in model behavior. They are not meant to perform model validation. They must be written in a way that allows model output to change liberally. 