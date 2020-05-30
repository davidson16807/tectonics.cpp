The "genes" namespace describes a nested data structure that organizes all attributes that contribute to the fitness of an organism. It stores all attributes in a format that is safe for anyone to edit: any combination of attribute values can be used to express a physically valid organism. 

The "genes" namespace cannot be easily manipulated by an optimization algorithm, so classes within the namespace provide `encode()` and `decode()` methods that convert to and from arrays of numeric integers. 

The "genes" namespace is designed to be valid for any combination of values, but this comes at the cost of readability. Another namespace, "bodies" provides equivalent read-only classes that provide more physically meaningful interpretations for when writing fitness functions. 

The full category diagram looks something like this (including descriptions of types):

`
array<uint32, N/8>     "canonical"      for storage on disk
     ⇅
array<uint4, N>        "dense"          for representation in memory
     ⇅
array<int16, M>        "traversible"    for manipulation by an optimization algorithm
     ⇅
genes::Body            "organized"      for manipulation by developers and users
     ↓
bodies::Body           "understandable" for access by developers when writing fitness functions
     ↓
[performance metrics]                   metrics such as top speed, body temperature, etc.
     ↓ 
[fitness]                               a optimizable metric for an ecological niche
`

For more information, see [designing-an-evolutionary-model.md](designing-an-evolutionary-model.md)