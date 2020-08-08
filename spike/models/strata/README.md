the "strata" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe a vertical rock profile composed of strata,
and the properties of that strata. 
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZAFgBoAmAXVJADcBDAGwFcYkQBlHAJ3p2YC2XCNzYBfUuky58hFGQCM1Ok1bsuvfgJASp2PASIBWUkpoMWbRJx59BAFQCeacZJAZ9s46QAMyi2rWAGaMEHw6bh4yhigAzL7+qlY2moIABAA6GXgC8Gkadtq67tIGcsgAnKaJluq2OPQRetHlVVTmSXWa9MKiTSWeMcgKFDWBKYUAsvRwcAAKEBCMva7NZUQjZiq11gVa07MLS-1R6ygAHAkdOxMNmdlYuXD59Y3Fp14oCj5X2+MhYRwJ1Kn2GPy2AWSe3oADUkgh3iChgA2arXcYAJQgAGMANZOFzAwblH5+dFQ+qCA7zRaMe45PJ7KkzGnHRHEoiosl-ZIACxmvJ0yhgUAA5vAiKAgtwINpED8QDgIEgRiA4LysEEgYgFMVpbKVTQlUgFerNdrdW59XLVcbEOQ9TKbUblYhYo6DYgyIrXZapU6kAB2F1IS5qjVak0euVhu3B8PmpAOq0BxAmH1IVEJyM66OZkNpmhmnN+kDWpBVDNpvOFqvfGvpu0KUvlnUF1WhCBoDbnH5BJhwGDKRj0ABGMEYCw51m4WFFvKBDYL6c73a+vdI-cYg+HY4nU5a7Fn88XKc95Hbyf9noU8Sr5xrF7rxDElDEQA)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```
\begin{tikzcd}
MassPool \times MassPool \arrow[rd] &                                          &  & Stratum \times Stratum \arrow[rd] &                                                                                                & float                            & hash     &  & Strata \times Strata \arrow[rd] &                                                                                               & float        \\
                                                  & MassPool \arrow[d, shift right=2] &  &                                   & Stratum \arrow[d, shift right] \arrow[r] \arrow[ru] \arrow[loop, distance=2em, in=125, out=55] & StratumType \arrow[r] \arrow[ru] & RockType &  &                                 & Strata \arrow[d, shift right] \arrow[ru] \arrow[r] \arrow[loop, distance=2em, in=125, out=55] & StrataValues \\
                                                  & MassPoolStore \arrow[u]           &  &                                   & StratumStore \arrow[u, shift right]                                                            &                                  &          &  &                                 & StrataStore \arrow[u, shift right]                                                            &             
\end{tikzcd}
```

Trivial product morphisms are omitted for brevity.

The category above uses several types, which are defined as follows:
* "Stratum"        `Stratum`                     a stack allocated array of Stratum objects with a static upper size bound
* "StratumStore"   `StrataStore`                 a compressed version of Stratum for storing large Stratum rasters
* "enums"          miscellaneous                 a set of enums that classify individual rock layers into geologically meaningful categories 
* "RockType"       `RockType`                    a large enums that classifies individual rock layers into rock types that can be interpreted by a human
