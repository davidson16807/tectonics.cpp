the "crust" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe rock layers over the suface of a mesh, 
and the properties of those strata. 

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZARgBoAGAXVJADcBDAGwFcYkQBhAJ2bhwAIAOoLwBbePwAKjejnggAvqXSZc+QijLFqdJq3bTZ8pSux4CRcqQDMOhizaJOPPkJFZxcft144AanrGyiAYZupEAGyk2jT2+k4+rsJiEgAyWACOzFhQisGhahYoABzRdoHsif6BXskeEgDyAGZNcDA4CCYhquYaJKQATOUOlS44eaaFfQNlsRVOTYwQsp35PeEoUUNzI04AIjBgZgCeklwQTViMbF0FvUQztjvxzr4BDquT95s2wy-NrXan26YSKyFKT10uxAALaHTcKS8ACElgBjADWwLuG2QVhiUJeAFl6HA4ABlNBcLBgADmExBUyIABZZgTHK8ku5PPxiaSKVTafTsWCWZC4uzDHJOjoYFAafIUKAmudREgrCAcBAkGQ2UgwMxGIwaDIAEYwRiSdZFEDXJrjLrKiCqxA6zVIACsz0c+sNxvoZotVo0NpgdpANDgAAssGHEOQHSqkDMNVrEJ7dYgfUabf7zZbQcHbfbgo7ndYaG7EAB2L16g3Z015oPsIv00tIFkp7Xq8V1305gP5xlOVsR6Ox4gJp0eiupqIZrN+wfNkeh4tKxNp2dIUoL+tLpsFltrtub9OV9O9zP7geH4chsPGiAQNCWUg4HgwShT501ruIABOWtr37RtAyPVcwx-btt0QTsr0XW9wPvUcQCjGNxhdaCt3-YhJxLTdiGTStiHLNDx0w+NvyAA)

Or it can be viewed with the following tikzcd markup:

```
\begin{tikzcd}
                                    & Crust \times Plates \arrow[d]                                                      &             &  &                                    &  &                               &  &                                                    \\
MassSpring                          & Plates \arrow[d]                                                                   & floats      &  & Crust \times MassSpring \arrow[dd] &  & Crust \times Liquid \arrow[d] &  & CrustValues \times Offsets \arrow[dd, shift right] \\
                                    & Crust \arrow[ru] \arrow[rd] \arrow[loop, distance=2em, in=235, out=305] \arrow[lu] &             &  &                                    &  & DensityProfile \arrow[d]      &  &                                                    \\
Crust \times CrustValues \arrow[ru] &                                                                                    & CrustValues &  & Plates                             &  & Offsets                       &  & Offsets \times Blocks \arrow[uu, shift right]     
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
