The `relation` namespace exclusively represents data structures and functions that form a mathematical category.
The data structures are representations of functions on the electromagnetic spectrum or pressure/temperature "state" space. 
It is necessary to store the functions as data structures, rather than using `std::function<T>` or lambdas, 
since there are several use cases for the functions, they must each be performant, and they each require different implementations to achieve performance.
