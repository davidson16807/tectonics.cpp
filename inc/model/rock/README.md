
Tectonics.cpp needs a way to represent rock within model state.
Ideally, the representation should be as comprehensive as possible.
It would therefore provide a description for all the possible subidivisions of crust:

* **density tower** all mass that comprises a world, expressed as a density partitioned series of mass pools of any phase (solid, liquid, or gas)
* **crust**       	a subsection of a density tower that is composed strictly of solid mass
* **plate** 	  	a subsection of a crust that is characterized by a unique orientation and velocity
* **column** 	  	a subsection of a plate that is characterized by a unique point on the globe, expressed as cell contents within a **raster**
* **stratum** 	  	a subsection of a column that is characterized by a unique depth, thickness, and particle makeup
* **particle**    	a subsection of a stratum that exists as a cohesive unit, ranging in size from a boulder to a particle of clay
* **clast**   	  	a subsection of a particle characterized by homogeneous grain makeup (originally a smaller particle prior to lithification)
* **grain**         a subsection of a clast that is comprised of a single mineral
* **polymorph**     a mineral that assumes a single uniform crystal structure (e.g. diamond or graphite)
* **mineral**       a repeating pattern of molecules characterized by a unique molecular ratio

The density tower and any crust(s) within it each have exactly one representation within model state.
Columns and strata have exactly one representation within model state, albeit at a very coarse resolution.
Particles, clasts, and grains are too numerous to model individually so they must be modeled statistically.

Since model state updates in real time using several out-of-order traversals, 
it is critical to reduce the memory footprint of model state.
Therefore, any structure smaller than a column is represented at a very coarse resolution
within stack memory, using arrays of constant size.
We will now iterate through each of these structures and account for how this can be done.

### Columns
The number of columns is inherently constant for a grid of known resolution.

### Strata
The number of strata varies in principle, and the number that can we can afford to represent is very low,
so we only represent strata if it serves some functional distinction.
These functional distinctions include a "bedrock" layer that is only created upon the rifting of plates,
a "sediment" layer that may accumulate during weathering and sediment transport,
and a "sedimentary" layer that accumulates during lithification.
Each of these three layers may be diminished by weathering, 
the sediment layer is diminished by lithification,
and the bedrock and sedimentary layers are subject to metamorphosis,
however all layers may contain weathered and unweathered material,
and all layers may contain metamorphosed and unmetamorphosed material,
since weathering and metamorphosis have effects upon rock 
that persist across sediment transport and lithification.

### Particles
Particles are distinguished by size, and size distribution must be kept at vary low resolution.
We also lack an accurate model to express how particle size changes in response to weathering, 
and we do not trust ourselves to invent our own (nor do we have the time to do so).
We therefore classify particles by their history,
namely whether the rock experienced mechanical weathering or chemical weathering.
This, combined with the type of mineral and the size of grain,
can then be used to guess the size of the rock:

unweathered extrusive rock                        boulder size   small grains
unweathered intrusive rock                        boulder size   large grains
mechanically weathered extrusive rock             gravel size    small grains
mechanically weathered intrusive rock             gravel size    large grains
resistant, chemically weathered extrusive rock    silt size      small grains
resistant, chemically weathered intrusive rock    sand size      large grains
susceptible, chemically weathered rock            clay size      small grains

We would ideally like to distinguish mechanical vs. chemical weathering,
however we must keep our memory footprint small, 
and mechanical weathering in nonsediment layers is much less noteworthy
(accounting only for the presence of "conglomerate" rock, which is likely uninteresting to users),
so we therefore only represent the effects of chemical weathering in nonsediment layers.
Mechanical weathering is still adequately represented in nonsediment layers,
however it is represented using the "unweathered" rock bins,
since these bins would otherwise be never used within the sediment layer.

### Clasts
The number of possible types of clast is prohibitively large at even a coarse resolution,
and the number of possible clasts within a particle will vary from sample to sample,
so we therefore do not represent the subdivision of particles into clasts.
All grains within a particle that share the same size class will be counted together, regardless of clast.
However, this does not mean we cannot ascertain whether a particle has clasts.
Any "weathered" particle in the "sedimentary" layer will have first undergone weathering, 
then lithification, so it therefore must contain clasts,
and these clasts would depend on the type of weathering, 
assuming the mechanical/chemical distinction were implemented.

### Grains
Much like particles, grains are characterized by size, the number of bins must be kept small,
and we lack an accurate model to predict the size of grains,
so we therefore classify grains based upon their history.
A single grain is essentially a crystal, 
and the size of a crystal is principally determined by the rate of cooling during its formation.
(the available space is another factor, but this is poorly represented by our model,
and is typically only relevant to the user when representing gemstone formation).
In normal circumstances, the rate of cooling 

### Polymorph
Lastly, the number of polymorphs will vary based upon the mineral.
We could presume an upper bound to the number of polymorphs however this number is still too large to represent.
Ideally we would at least like to represent broad classes of metamorphic facies (e.g. none, low, intertermediate, high),
but even this leaves the number of classes too high.
We therefore only represent whether some kind of metamorphosis has taken place, regardless of degree.
Users can then make inferences about what kind of facies would occur and in which frequency.
Some layers, chiefly the "bedrock" layer, will never be subject to transport,
so their metamorphic facies can be inferred by the maximum temperature and overburden pressure that was received at a given depth.
This can be modeled for any point in the layer using only the maximum temperature and overburden pressure that was received at the top of the layer.
Since this value is constant throughout an entire layer, storing it is relatively cheap,
so we include this value with each layer.

### Minerals
The number of minerals present is of constant size. 
Here, the concept of "mineral" is one-to-one with that of a chemical compound.
(the name "mineral" is only chosen to disambiguate from the "compound" namespace)

# Representation
Even when working with so many crude approximations,
it still takes ridiculous amounts of memory to store each `Mineral` using only floats,
so we store each mineral in a raster as a much smaller object known as a `MineralStore`.
Mass within a `MineralStore` is still stored as a float 
since mass must be conserved with high accuracy for a model to run over long time scales,
however the bins that characterize the size and frequencies of particles, grains, and facies
are stored in footprints of only half a byte. This still allows a worst case error of merely 0.06%.

Since the interpretation of attributes within `MineralStore` is error prone,
we only convert back to `Mineral` only when we want to perform some operation on it,
and to encourage correct usage, we encapsulate the class so internals of a `MineralStore` cannot be read directly.
The interpretation of attributes also comes with some performance penalty,
so to encourage users not to spam calls to getters, 
we only expose methods to convert to and from `StratumStore`.

Together, `Mineral` and `MineralStore` form a mathematical category.
The category is composed from types that describe a solid mass pool that is composed entirely of of a single compound, i.e. a "mineral",
It also includes information may inform us of the size of grains and particles of that compound within a layer of rock.
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZABgBoAmAXVJADcBDAGwFcYkQBZLMGAJyYDKOCLzYBfUuky58hFGQCM1Ok1bsuPfoxASp2PASJliyhizaJO3PkwAEAHXt4AtvFsab2scphQA5vBEoABmvBDOSGQgwkgKNHAAFljBOJG6IKHhsTQxiFGJyamI5OmZEcU5ELGlYeVx0VWI9YwQEGhEAJxkwUxwMMqM9ABGMIwACtIGciC8WH4Jqd5iQA)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```
\begin{tikzcd}
Mineral \times Mineral \arrow[d]                                            \\
Mineral \arrow[d, shift right=2] \arrow[loop, distance=2em, in=35, out=325] \\
MineralStore \arrow[u]                                                     
\end{tikzcd}
```

Trivial product and identity morphisms are omitted for brevity.

The category above uses two major types, which are defined as follows:
* "Mineral"      `Mineral`      a stack allocated array consisting of a mass and bins that partition that mass
* "MineralStore" `MineralStore` a compressed version of Mineral for storing large aggregates such as `Strata` or `Plate`
