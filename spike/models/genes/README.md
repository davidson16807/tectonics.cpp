The "genes" namespace describes a nested data structure that organizes all attributes that contribute to the fitness of an organism.

The "genes" namespace serves to create an organized listing of attributes that can be easily referenced by a developer when writing fitness functions. This contrasts with an array of attribute values, which is easily manipulated by an optimization algorithm but extremely difficult to reference within fitness functions. 

The evolutionary model decouples these concerns by creating morphisms between arrays and nested data structures. Several other morphisms are needed to address validation and ease-of-use concerns. The full category diagram looks something like this (including descriptions of types):

`
array<uint32, N/8>     "canonical"      for storage on disk
     ⇅
array<uint4, N>        "dense"          for representation in memory
     ⇅
array<int16, M>        "traversible"    for manipulation by an optimization algorithm
     ⇅
genes::Body            "organized"      for manipulation by developers and users
     ↓
life::Body             "understandable" for access by developers when writing fitness functions
     ↓
[performance metrics]                   metrics such as top speed, body temperature, etc.
     ↓ 
[fitness]                               a optimizable metric for an ecological niche
`

For more information, see [designing-an-evolutionary-model.md](designing-an-evolutionary-model.md)