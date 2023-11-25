the "strata" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe a vertical rock profile composed of rock layers,
an efficient storage structure for that type, and the properties that derive from that type.
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZARgBpiBdUkANwEMAbAVxiRAGUcAnOnOkAL6l0mXPkIoyAJiq1GLNpx59OELqyEjseAkQAM5WfWatEHbrzoACADo28AW3hWllwcJAZt4olNJ6jeVMQADMGCF53LTFdFD9KamMFM1c+ADUghE1PUR0JZD8ZRKDFCz5beywnOBcyugyTLNkYKABzeCJQEK4IByQDEBwIJDIQOAALLBCcEezu3pHqIf7qCamZxGI5nr7EP0HhxD1thaOlw4BmE92B5cQAFlXJ6f7rlYP38Ig0ImIADgMIUYcBgsgYdAARjAGAAFXI+MxcLCtcYzN6IACs51eFAEQA)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```
\begin{tikzcd}
                               &                                                                                               & float                                 \\
Strata \times Strata \arrow[r] & Strata \arrow[d, shift right] \arrow[ru] \arrow[r] \arrow[loop, distance=2em, in=125, out=55] & StrataValues                          \\
                               & StrataStore \arrow[u, shift right]                                                            & Strata \times StrataValues \arrow[lu]
\end{tikzcd}
```

Trivial product and identity morphisms are omitted for brevity.

The category above uses several types, which are defined as follows:
* "Stratum"        `Stratum`                     a stack allocated array of Stratum objects with a static upper size bound
* "StratumStore"   `StrataStore`                 a compressed version of Stratum for storing large Stratum rasters
* "StrataValues"   `StrataValues`                a generic array mapping values to rock layers within a strata object
