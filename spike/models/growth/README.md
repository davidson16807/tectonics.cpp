The `bodies` namespace describes a progression of life cycle stages with distinct read only physical attributes that can be easily interpreted to generate performance characteristics for that life cycle stage.

Each class within the `bodies` namespace represents a lifecycle stage for which a distinct set of physical attributes can be sensibly interpreted. For instance, it does not make sense to talk about segmentation within a single cell organism, so the `Unicell` class omits those attributes. Other attributes within the `Genome` are valid for any life cycle stage, such as `Mineralization` and `Pigmentation`.

Each transition in life stage is modeled as a function that maps a "maybe" tuple `boolxL₁xG` to another "maybe" tuple `boolxL₂`, where `Lₙ` is a life stage, `G` is a genome, and `bool` indicates whether the contents of the life stage object are valid.

The following is my best attempt to represent the progession with a category diagram:

`
             Genome
               ↓
       Unicell x Genome
               ↓
bool x Blastula x Genome   →   bool x Diploblast x Genome
               ↓
bool x Triploblast x Genome
`