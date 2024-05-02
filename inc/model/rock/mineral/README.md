A `Mineral` represents a mass pool that has a unique position on the globe, manner of formation, chemical compound, and phase.
It represents the mass of the pool, the frequency of weathered vs unweathered material within it, and its distribution of grain sizes.
Grain sizes fall into four categories: 

* amorphous (e.g. obsidian)
* extrusive (e.g. grains of basalt)
* intrusive (e.g. grains of gabbro)
* crystalline (e.g. quartz crystals)

Even when working with such a crude approximation,
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
