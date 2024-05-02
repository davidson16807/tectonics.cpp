A `Formation` is a subsection of a plate that consists of a raster of strata that is characterized by a unique manner of formation.
In principle, this is meant to represent the same concept that geologists refer to as a "geological formation".
In practice, the number of formations must be kept very small to promote data locality, 
so formations are only used to distinguish things such as whether rock is sediment, sedimentary, or igneous bedrock.

Formations are important to the model since they can isolate that parts of a plate that are subject to certain processes,
thereby promoting data locality. For instance, only sediment mass pools are considered during out-of-order traversals due to erosion,
so sediment can be tracked within a distinct formation to promote data locality.
Bedrock is modified very infrequently, so keeping it separate from sedimentary rock as it own formation
allows us to represent rock columns with greater fidelity without significant loss of performance.