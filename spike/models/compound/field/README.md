The `field` namespace exclusively represents data structures and functions that form a mathematical category.
The data structures are analogous to fields that can be filled out within a form.
The data structures are specially designed for use within the compound library to represent values that may or may not be optional when specifying a compound.
We say the data structures are specially designed because Fields may represent functions of the electromagnetic spectrum or pressure/temperature "state" space. 
The ability to infer values from other properties of compounds under different regions of parameter space is necessary,
and requires functions and data structures beyond what is provided by existing solutions like `std::optional<T>`