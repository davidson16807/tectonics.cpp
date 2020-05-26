The "genes" namespace describes a nested data structure that organizes all attributes that contribute to the fitness of an organism.

The "genes" namespace serves to create an organized listing of attributes that can be easily referenced by a developer when writing fitness functions. This contrasts with an array of attribute values, which is easily manipulated by an optimization algorithm but extremely difficult to reference within fitness functions. 

The evolutionary model decouples these concerns by creating morphisms between arrays and nested data structures. Several other morphisms are needed to address validation and ease-of-use concerns. The full category diagram looks something like this:

`array<uint4>  <=>  array<int16>  ->  genes::Body  -> bodies::Body -> performance_metrics -> fitness`

For more information, see [designing-an-evolutionary-model.md](designing-an-evolutionary-model.md)