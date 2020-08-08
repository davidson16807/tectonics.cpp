the "stratum" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe a vertical rock profile composed of stratum,
and the properties of that stratum. 
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZAFgBoAmAXVJADcBDAGwFcYkQBlHAJ3p2YC2XCNzYBfUuky58hFGQCM1Ok1bsuvfgJASp2PASIBWUkpoMWbRJx59BAFQCeacZJAZ9s46QAMyi2rWAGaMEHw6bh4yhigAzL7+qlY2moIABAA6GXgC8Gkadtq67tIGcsgKFImW7ACy9HBwAAoQEIwRetHllWYqNdb1jS1tHSWeMcgAbKbVgSAAShAAxgDWTi6jUWVEPgnmSXUNza2MmdlYuXBpg8cjxVteKNN++-0gABYN7zrKMFAA5vAiKAgtwINpELsQDgIEhKiA4O8sEEcHDiqDwXCaDCkFDEcjUYgFOiwRD4TjEOQSZiidjYYhYtSIWRofTiW4MRCTKykNMEUiUbimby6UhufjBZThbSeTLQhA0EQFAAOXZBJhwGDKRj0ABGMEYLXGchA3Cw-3eqOl5FFiAA7NLlbbJtK+RS+fLFSgVWqNVqaDr9YbSo9TebLaNOUgbbKAJxiShiIA)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```
\begin{tikzcd}
MassPool \times MassPool \arrow[rd] &                                                                              &  & Stratum \times Stratum \arrow[rd] &                                                                                                & float                            & hash     \\
                                    & MassPool \arrow[d, shift right=2] \arrow[loop, distance=2em, in=125, out=55] &  &                                   & Stratum \arrow[d, shift right] \arrow[r] \arrow[ru] \arrow[loop, distance=2em, in=125, out=55] & StratumType \arrow[r] \arrow[ru] & RockType \\
                                    & MassPool \arrow[u]                                                           &  &                                   & StratumStore \arrow[u, shift right]                                                            &                                  &         
\end{tikzcd}
```

Trivial product morphisms are omitted for brevity.

The category above uses several types, which are defined as follows:
* "Stratum"        `Stratum`                     a stack allocated array of Stratum objects with a static upper size bound
* "StratumStore"   `StrataStore`                 a compressed version of Stratum for storing large Stratum rasters
* "enums"          miscellaneous                 a set of enums that classify individual rock layers into geologically meaningful categories 
* "RockType"       `RockType`                    a large enums that classifies individual rock layers into rock types that can be interpreted by a human
