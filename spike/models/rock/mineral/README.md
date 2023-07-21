the "mineral" namespace exclusively represents data structures and functions that define a mathematical category.
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
* "Mineral"        `Mineral`                     a stack allocated array of Mineral objects with a static upper size bound
* "MineralStore"   `MineralStore`                a compressed version of Mineral for storing large aggregates of Mineral (see "crust" namespace)
