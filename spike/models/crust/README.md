the "crust" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe rock layers over the suface of a mesh, 
and the properties of those strata. 

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZARgBoAGAXVJADcBDAGwFcYkQBhAJ2bhwAIAOoLwBbePwAKjejnggAvqXSZc+QijLFqdJq3bTZ8pSux4CRAEykAzDoYs2iTjz5CRWcXH7deOAGp6xsogGGbqRABspNo0DvrOvm7CYhIAMlgAjsxYUIohYWoWKAAcMfZB7EkBQd4pnhIAyjhcsvQAQhDMYFD0XFjBpkUaJKSWFY5Vrjj5Q+Yj1rG6k84AZowQsggmoarzUWMTCSAAIjBgZgCeACoQAO4wXLO74cXI1uNxlYnTgY7bBT2ERQ0QALEcnCBmq0cB0uj0+gMAXNgcgynYvisQGl6JdHjAoAAleh8R7uVLeaFtTrdXr9QYvYZEcjlTHHACyJLgjTQ-TAAHNnoV9ihQazlsdquSGt5OXBubysAKhUC3mKMRLIYY5MjGSLkCzPpqpn5pV4fNMVa8RtENfFIRlsrl2VgAB44ZhcGBmiRnC64S6SLgQVZYRhsBQ6An8+QoUCrYOiJAskA4CBIMjGxBgZiMRg0GQAIxgjEkqo0IHDqxmOwTECTiEzaaQAFY2U4c3mC-Ri6Xy+wqzMaHAABZYavJ2uJpDWVPpxBtrOd-OVnslsvWgcwCdT+tIGw0ZuIADs7aQy+7vY3TOcg+edYbYrnGZT9vPuZXRfX-dv26HIFHccZkbXcG0XI9oiXD9L2-Tdfx3EIH1bQ95zKKCu1XK8f0rP972nBcUOQ9DPzXPs4JwicCwgCA0GZUgWlYShQJfQjECfN9s2gzDYJvCj-0AicQMQ-DwPnYhiGYxtZyPYgDwAsdBPISTiCfI8W0k09n0bdThL3bTWIATkjBQgA)

Or it can be viewed with the following tikzcd markup:

```
\begin{tikzcd}
                             & Crust \times Plates \arrow[d]                                                     &                                     &  &                                    &  &                                               &  &                                                               \\
MassSpring                   & Plates \arrow[d]                                                                  & floats                              &  & Crust \times MassSpring \arrow[dd] &  & Crust \times Liquid \arrow[d]                 &  & CrustValues \times StrataBoundaries \arrow[dd, shift right]   \\
Crust \times Crust \arrow[r] & Crust \arrow[ru] \arrow[r] \arrow[loop, distance=2em, in=235, out=305] \arrow[lu] & CrustValues                         &  &                                    &  & DensityTower \arrow[d]                        &  &                                                               \\
                             &                                                                                   & Crust \times CrustValues \arrow[lu] &  & Plates                             &  & LiquidMixture \times DensityProfile \arrow[d] &  & LayeredRaster \times StrataBoundaries \arrow[uu, shift right] \\
                             &                                                                                   &                                     &  &                                    &  & StrataBoundaries                              &  &                                                              
\end{tikzcd}
```

Trivial product morphisms are omitted for brevity.

The category above uses several types, which are defined as follows:
* "plates"   `std::vector<series<StrataStore>>` rock layer objects within cells of several SpheroidGrids, one per plate
* "crust"    `series<StrataStore>`              rock layer objects within cells of a single global SpheroidGrid
* "floats"   `series<float>`                    floats mapping to cells within a single global SpheroidGrid
* "top"      `series<Stratum>`                  Stratum objects representing the topmost layer within cells of a single global SpheroidGrid
* "values"   `series<StrataValues<float>>`      floats mapping to rock layers within cells of a single global SpheroidGrid
* "liquid"   `Atmosphere`                      a transparent data structure storing global mass pools for compounds in the atmosphere
* "sinking"  `std::vector<series<std::uint_8>>` a list of rasters, one for each fluid, storing indices of layers below which the fluid is less dense
* "offsets"  `std::vector<Displacements>`      a list of data structures, one for each fluid, storing the "sealevel" for the fluid and a raster of isostatic displacements for the strata that floats on top
* "blocks"   `series<float>`                    floats mapping to layers of constant height in cells of a single global LayeredSpheroidGrid, like Minecraft blocks
