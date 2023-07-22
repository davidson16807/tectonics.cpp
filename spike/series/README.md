This folder contains a library that introduces an unopinionated templated data structure, `series::series<T>` (analogous to `Series` in pandas), that wraps `std::vector<T>` objects to enable arithmetic operations and `glm` inspired functions to be performed on their contents as if they were primitive data types like a `float` or an `int`. A set of functions is provided which operate over the `series::series<T>` data structure using method signatures that resemble those of the `glm` library. It is meant to foster performant code through data oriented design. 

`glm` can be optionally used to enhance functionality. Functionality that leverages `glm` can be found within `inc/many/glm`. 

#Frequently Asked Questions
##Why not use existing libraries?
Other standards and libraries exist that fulfill the same role (e.g. [std::valarray](https://en.cppreference.com/w/cpp/numeric/composite) and [xtensor](https://github.com/QuantStack/xtensor)), however there are reasons we avoid these libraries. `std::valarray` is nonconformant to basic std container behavior and has problems being bound to web assembly wrappers. `xtensor` shares similar issues, and does not use the common `glm` frame of mind that we leverage elsewhere in the code (instead opting for an API inspired by `numpy`). 

##Why not add operator overloads to `std::vector<T>`? 
Operator overloads have the potential to introduce obscure and undesireable behavior. Problems are only made worse if operator overloads are defined on classes within `std`, so we go to great effort in creating our own library to do this. Even then, we forbid operator overloads from being used within the library's code, and we explicitly require the user to include the `convenience.hpp` header file in order to use them. Users can still retrieve the underlying `std::vector<T>` behind instances of `series<T>` by calling the `.vector()` method. 

