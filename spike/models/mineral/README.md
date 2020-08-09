the "mineral" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe a solid mass pool that is composed entirely of of a single compound, i.e. a "mineral",
It also includes information may inform us of the size of grains and particles of that compound within a layer of rock.
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZABgBoAmAXVJADcBDAGwFcYkQBZeuOABQgiMQAX1LpMufIRRkAjNTpNW7Lj36CRYkBmx4CRMsQUMWbRJ258BjAAQAdO3gC28G6qsbhCmFADm8IlAAMwAnCCckMhAcCCRZGjgACywgnEjRYLCIxHjo2MQopJS0xHIMkFDwpHIaGLjyyuzcupyaRgE0IgBOMiCmOBgFRnoAIxhGfl0pdhCsX0S0r2EgA)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```
\begin{tikzcd}
MassPool \times MassPool \arrow[d]                                           \\
MassPool \arrow[d, shift right=2] \arrow[loop, distance=2em, in=35, out=325] \\
MassPool \arrow[u]                                                          
\end{tikzcd}
```

Trivial product morphisms are omitted for brevity.

The category above uses several types, which are defined as follows:
* "Mineral"        `Mineral`                     a stack allocated array of Mineral objects with a static upper size bound
* "StratumStore"   `StrataStore`                 a compressed version of Stratum for storing large Stratum rasters
* "enums"          miscellaneous                 a set of enums that classify individual rock layers into geologically meaningful categories 
* "RockType"       `RockType`                    a large enums that classifies individual rock layers into rock types that can be interpreted by a human
