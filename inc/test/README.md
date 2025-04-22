`test/` provides a `test`:: that consists of classes and functions that comprehensively test functions in a standardized and rigorous way. This typically involves satisfying common and extremely useful mathematical properties, like determinism, closure, invertibility, identity, etc. This approach to testing is commonly referred to elsewhere as "property based testing" ("PBT").

There are several advantages to using this approach:
* Developers need not violate encapsulation when writing unit tests. They need only define their classes with some way to test objects for equality and depict them as strings.
* Developers need not predict what output ought to be returned when given precise sample input. They need only provide samples of input, which is typically a much easier problem.
* Developers need not change sample output if the definition of a function changes. When properties are combined together, those properties are typically difficult to satisfy by accident, yet they remain general enough that changes in the definition of a function do not necessarily break unit tests.
* Developers need not devise one-off ways of testing functionality. They have a library of useful properties that mathematicians have discovered for them, which have proven to be broadly applicable yet extremely useful and not easily satisfied by accident.
* When combined with test driven development ("TDD"), PBT encourages the design of functionality that satisfies useful mathematical properties, which allows the code that uses that functionality to be easily reasoned with.
* When combined with functionality in `test::`, unit tests can check that properties are satisfied for a very large number of sample values or sample value combinations. Tests become much more thorough.

The following files are noteworthy:
* `adapter.hpp` defines classes that make use of the "Adapter" pattern to standardize the way that values are printed and tested for equality, which is used throughout the rest of `test::`
* `predicate.hpp` introduces a standardized way to test for arbitrary conditions ("predicates"), which is used to define `properties.hpp`
* `equality.hpp` introduces a standardized way to test that two values are equal, which is used to define `properties.hpp`
* `properties.hpp` defines tests for specific mathematical properties like determinism, closure, invertibility, identity, etc.
* `structures/` defines classes for testing larger mathematical structures like groups, orders, metrics, rings, and fields
