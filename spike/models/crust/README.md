the "rock" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe the distribution of strata over the suface of a mesh, 
and the properties of those strata. 

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZABgBpiBdUkANwEMAbAVxiRBwjQAIAdHvALbwuaBnRzwQAX1LpMufIRQBGclVqMWbAMYAnJnBy9+WIXBFiJCGXOx4CRVcvX1mrRCFHjJNz-PtKyABMpADMLpruIHoGRnyCwq4s1rJ+dopEACykQRFuOvqGxgnmDFgAjkxYUNKpGOkOKABsOXlaHknC8abCEABmfXAwOCm2Co3IquHUnQWxtWMBjqSZbVF9DBDio2njgdnTGvke2GAA1jC6O-V7RGSrM5FsHGgLu0soIQ9H7T8+df4Mihst9Zh5+oNhtdARMWqCnuCBkMRsUeuYAEabbRnazqGBQADmkhQoD6uggAiQZHYECQIT+SDATAYDGoYnRMAYAAUYUoQAwYH0cG8yRSkKoaXTHscmSy2XQOdzeWwBUKReTKYh6RwkC0GYhZaz+QrOTyGnzVcLqHAABZYNWIAC0yl8os1oWoOsQerBhvlirNtw8lvVYsQ2UliAAHNL2n7jQHlcHBcLXRqkABWT20xDKem+5lG9mmpP8lMga12h0u1Ju3XZqX6+PFpXmlXlyv24VOmuk9PehuIADssfczZNraDZbVabDeq9ympBblCZLbeTM9r-fnOZ9CPHifX06t-IgnCIQSHZBw+hgFFnmp3SAAnKPGYX-WupyGH0gY5HlAlZciwnQMPmPUNNTzQcsybD9V0ncCQ07aspHvIA)

Or it can be viewed with the following tikzcd markup:

```
% https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZABgBpiBdUkANwEMAbAVxiRBwjQAIAdHvALbwuaBnRzwQAX1LpMufIRQBGclVqMWbAMYAnJnBy9+WIXBFiJCGXOx4CRVcvX1mrRCFHjJNz-PtKyABMpADMLpruIHoGRnyCwq4s1rJ+dopEACykQRFuOvqGxgnmDFgAjkxYUNKpGOkOKABsOXlaHknC8abCEABmfXAwOCm2Co3IquHUnQWxtWMBjqSZbVF9DBDio2njgdnTGvke2GAA1jC6O-V7RGSrM5FsHGgLu0soIQ9H7T8+df4Mihst9Zh5+oNhtdARMWqCnuCBkMRsUeuYAEabbRnazqGBQADmkhQoD6uggAiQZHYECQIT+SDATAYDGoYnRMAYAAUYUoQAwYH0cG8yRSkKoaXTHscmSy2XQOdzeWwBUKReTKYh6RwkC0GYhZaz+QrOTyGnzVcLqHAABZYNWIAC0yl8os1oWoOsQerBhvlirNtw8lvVYsQ2UliAAHNL2n7jQHlcHBcLXRqkABWT20xDKem+5lG9mmpP8lMga12h0u1Ju3XZqX6+PFpXmlXlyv24VOmuk9PehuIADssfczZNraDZbVabDeq9ympBblCZLbeTM9r-fnOZ9CPHifX06t-IgnCIQSHZBw+hgFFnmp3SAAnKPGYX-WupyGH0gY5HlAlZciwnQMPmPUNNTzQcsybD9V0ncCQ07aspHvIA
\begin{tikzcd}
top \times plates \arrow[rd] & crust \times plates \arrow[d]                                                                            &                               &  &                               &  &                                               \\
                             & plates \arrow[dd, shift left]                                                                            &                               &  &                               &  &                                               \\
                             &                                                                                                          &                               &  & crust \times liquid \arrow[d] &  & values \times offsets \arrow[dd, shift right] \\
                             & crust \arrow[uu, shift left] \arrow[d] \arrow[rd] \arrow[loop, distance=2em, in=145, out=215] \arrow[ld] & crust \times values \arrow[l] &  & sinking \arrow[d]             &  &                                               \\
top                          & floats                                                                                                   & values                        &  & offsets                       &  & offsets \times blocks \arrow[uu, shift right]
\end{tikzcd}
```

Trivial product morphisms are omitted for brevity.

The category above uses several types, which are defined as follows:
* "plates"   `std::vector<tmany<StrataStore>>` rock layer objects within cells of several SpheroidGrids, one per plate
* "crust"    `tmany<StrataStore>`              rock layer objects within cells of a single global SpheroidGrid
* "floats"   `tmany<float>`                    floats mapping to cells within a single global SpheroidGrid
* "top"      `tmany<Stratum>`                  Stratum objects representing the topmost layer within cells of a single global SpheroidGrid
* "values"   `tmany<StrataValues<float>>`      floats mapping to rock layers within cells of a single global SpheroidGrid
* "liquid"   `Atmosphere`                      a transparent data structure storing global mass pools for compounds in the atmosphere
* "sinking"  `std::vector<tmany<std::uint_8>>` a list of rasters, one for each fluid, storing indices of layers below which the fluid is less dense
* "offsets"  `std::vector<Displacements>`      a list of data structures, one for each fluid, storing the "sealevel" for the fluid and a raster of isostatic displacements for the strata that floats on top
* "blocks"   `tmany<float>`                    floats mapping to layers of constant height in cells of a single global LayeredSpheroidGrid, like Minecraft blocks
