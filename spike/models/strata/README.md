the "strata" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe a vertical rock profile composed of rock layers,
an efficient storage structure for that type, and the properties that derive from that type.
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZAFgBoAmAXVJADcBDAGwFcYkQBlHAJ3p2YC2XCNzYBfUuky58hFGQCM1Ok1bsuvfgJASp2PASIBWUkpoMWbRJx59BAFQCeacZJAZ9s46QAMyi2rWAGaMEHw6bh4yhigAzL7+qlY2moIABAA6GXgC8Gkadtq67tIGcsgAnKaJluq2OPQRetHlVVTmSXWa9MKiTSWeMcgKFDWBIACy9HBwAAoQEIz9UWVEI2YqtdZTM-OLy6VeKAAcCR1bKXz0mdlYuXD59Y3FK0fDPmeb4yFhOAeD5QUHw2AWSBQaADUkggXochgA2arncYAJQgAGMANZOFz-FpED5+ZHJHZzBaMG45PKkvZLWEAtYfdpfMFPSl3PLg+hQywwyJw8qIoks9gAC2mop0yhgUAA5vAiKAgtwINpEB8QDgIEgRiA4KKsEE-ogFMVlaqdTQtUgNfrDcbTW5zWrddbEOQzSqXVbtYhYp6LYgyJrfY6lV6kAB2H1IU56g1Gm0BtVxt3R+P2pAep0RxAmENIREZxMm5OFmN5mh2kthkDOpBVAt5suVptAlv5t0KXXV40+Fuu0M0UIQNBrY4fIJMOAwZSMegAIxgjHmDOs3CwstFfw7FfzI7HKAUE9IU8YM7ni+Xq-x6832-69fdFYU2fDgYU8Sbxxbqd9cYPcdJ2nWdhyvFcBXYDctx3HMP2DN0jBbcgXyQygxCAA)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```
\begin{tikzcd}
MassPool \times MassPool \arrow[rd] &                                                                              &  & Stratum \times Stratum \arrow[rd] &                                                                                                & float                            & hash     &  & Strata \times Strata \arrow[rd] &                                                                                               & float                                 \\
                                    & MassPool \arrow[d, shift right=2] \arrow[loop, distance=2em, in=125, out=55] &  &                                   & Stratum \arrow[d, shift right] \arrow[r] \arrow[ru] \arrow[loop, distance=2em, in=125, out=55] & StratumType \arrow[r] \arrow[ru] & RockType &  &                                 & Strata \arrow[d, shift right] \arrow[ru] \arrow[r] \arrow[loop, distance=2em, in=125, out=55] & StrataValues                          \\
                                    & MassPool \arrow[u]                                                           &  &                                   & StratumStore \arrow[u, shift right]                                                            &                                  &          &  &                                 & StrataStore \arrow[u, shift right]                                                            & Strata \times StrataValues \arrow[lu]
\end{tikzcd}
```

Trivial product and identity morphisms are omitted for brevity.

The category above uses several types, which are defined as follows:
* "Stratum"        `Stratum`                     a stack allocated array of Stratum objects with a static upper size bound
* "StratumStore"   `StrataStore`                 a compressed version of Stratum for storing large Stratum rasters
* "StrataValues"   `StrataValues`                a generic array mapping values to rock layers within a strata object
