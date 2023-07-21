the "stratum" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe a vertical rock profile composed of stratum,
and the properties of that stratum. 
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZARgBoAmAXVJADcBDAGwFcYkQBlHAJ3p2YC2XCNzYBfUuky58hFGWLU6TVuy69+AkBKnY8BIuVKKaDFm0ScefQQBUAnmnGSQGPbMOkADErOrLAGaMEHzaLm4yBihexr4qFlYaggAEADqpeALwyeo2Wjqu0vpyyADMsabx7ABKEADGANYOTmG6kSXlPpXm7AAW9HC92kowUADm8ESgAdwQWogxIDgQSGQgg1gBOKsFM3OrNMtIixtbOy5782tHiOS7s1eHK4il9-uIACxP59MPB0vPNbBCBoIjEAAcMQCTDgMCUjHoACMYIwAApFDyWbhYMa9bZveZGAFIACsBKQRJuADYxJQxEA)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```
\begin{tikzcd}
                                 &                                                                                                & float                            & hash     \\
Stratum \times Stratum \arrow[r] & Stratum \arrow[d, shift right] \arrow[r] \arrow[ru] \arrow[loop, distance=2em, in=125, out=55] & StratumType \arrow[r] \arrow[ru] & RockType \\
                                 & StratumStore \arrow[u, shift right]                                                            &                                  &         
\end{tikzcd}
```

Trivial product morphisms are omitted for brevity.

The category above uses several types, which are defined as follows:
* "Stratum"        `Stratum`                     a stack allocated array of Stratum objects with a static upper size bound
* "StratumStore"   `StrataStore`                 a compressed version of Stratum for storing large Stratum rasters
* "enums"          miscellaneous                 a set of enums that classify individual rock layers into geologically meaningful categories 
* "RockType"       `RockType`                    a large enums that classifies individual rock layers into rock types that can be interpreted by a human
