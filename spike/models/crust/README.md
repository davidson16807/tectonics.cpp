the "crust" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe rock layers over the suface of a mesh, 
and the properties of those strata. 

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZARgBoAGAXVJADcBDAGwFcYkQBhAJ2bhwAIAOoLwBbePwAKjejnggAvqXSZc+QijLFqdJq3bTZ8pSux4CRAEykAzDoYs2iTjz5CRWcXH7deOAGp6xsogGGbqRABspNo0DvrOvm7CYhIAMlgAjsxYUIohYWoWKAAcMfZB7EkBQd4pnhIA8gBmzXAwOAgmoarmGiSklhWOVa44+aZF-daxuiPOzYwQsl0FvREo0UNxlc4AIjBgZgCeklwQzViMbN2FfVaDwwkufoGOq5P3m7ZPTiAtbQ6Hx64WKyDKdh283+rXanXcqW8ACElgBjADWwLuG2Q5HKUOeAFl6HA4ABlNBcLBgADmExBUyIABZ8XNntUEQ1vMTSRSqbT6diwSzIWy-oY5Fj1mC8dsxaM-JyvD4xoodDAoDT5ChQM1zqIkHiQDgIEgyPLEGBmIxGDQZAAjGCMSTSjQga7NcbdPUQA2Ic0mpAAVgJTitNrt9Edztd7A94xocAAFlhPYbvfqkNZjabECGLeHbe6o06XaC3fH6T6-TYaIHEAB2UNIQuR6NlxnOSsZ31IFk5s1G+Jh61Fh2l2NdmBpxMptP+nt+-P16IF0dtifluPTr0havBuu5spriPF9uT907quZvOHg8nsclmNbqcz90QCBoIh4nA8GCURckCbAdEAATmbS11zPTdO0vNNAP9I1637YcWyg8dn1gytZ1TcYFz3G9l1zYhiAQ4hs3rYhaxAZNcPTAje39ft6yDBQAKAA)

Or it can be viewed with the following tikzcd markup:

```
\begin{tikzcd}
                             & Crust \times Plates \arrow[d]                                                     &                                     &  &                                    &  &                               &  &                                                    \\
MassSpring                   & Plates \arrow[d]                                                                  & floats                              &  & Crust \times MassSpring \arrow[dd] &  & Crust \times Liquid \arrow[d] &  & CrustValues \times Offsets \arrow[dd, shift right] \\
Crust \times Crust \arrow[r] & Crust \arrow[ru] \arrow[r] \arrow[loop, distance=2em, in=235, out=305] \arrow[lu] & CrustValues                         &  &                                    &  & DensityProfile \arrow[d]      &  &                                                    \\
                             &                                                                                   & Crust \times CrustValues \arrow[lu] &  & Plates                             &  & Offsets                       &  & Offsets \times Blocks \arrow[uu, shift right]     
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
