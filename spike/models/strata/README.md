the "strata" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe a vertical rock profile composed of strata,
and the properties of that strata. 
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZAFgBoAmAXVJADcBDAGwFcYkQBlHAJ3p2YC2XCNzYBfUuky58hFGQCM1Ok1bsuvfgJASp2PASIBWUkpoMWbRJx59BAFQCeacZJAZ9s46QAMyi2rWAGaMEHw6bh4yhigAzL7+qlY2moIABAA6GXgC8Gkadtq67tIGcsgAnKaJluq2OPQRetHlVVTmSXWa9MKiTSWeMcgKFDWBIACy9HBwAAoQEIz9UWVEI2YqtdZTM-OLy6VeKAAcCR1bKXz0mdlYuXD59Y3FK0fDPmeb4yFhOAeD5QUHw2AWSBQaADUkggXochgA2arncYAJQgAGMANZOFz-FpED5+ZHJHZzBZLWEAtYfdpfMFPG45PLg+hQyww5QwKAAc3gRFAQW4EG0iA+IBwECQIxAcAAFlggn9EApioLhVKaBKkGK5QqlSq3GqRdKtYhyKqhcbNZLELELerEGRxTaDQLLUgAOzWpCnGXyxXa+0i32mr1+vVIc2G92IEzOpCI8MB5VBhPe2M0XXJ10gI1IKrx2OpjOFoHFuOmhTSrNKnzFk0umihCBoNbHD5BJhwGDKRj0ABGMEY8yp1m4WG5sr+5fTcebrZQCnbpE7jG7vYHQ5H+LHE6n-TzZvTCijbodCniheOxZDNt987bHa7Pabm+HcLkIHHk+n0fPTtNIwxEoMQgA)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```
% 
\begin{tikzcd}
MassPool \arrow[rd] &                                                                              &  & Stratum \times Stratum \arrow[rd] &                                                                                                & float                 &          &  & Strata \times Strata \arrow[rd] &                                                                                               & float                                 \\
                    & MassPool \arrow[d, shift right=2] \arrow[loop, distance=2em, in=125, out=55] &  &                                   & Stratum \arrow[d, shift right] \arrow[r] \arrow[ru] \arrow[loop, distance=2em, in=125, out=55] & StratumType \arrow[r] & RockType &  &                                 & Strata \arrow[d, shift right] \arrow[ru] \arrow[r] \arrow[loop, distance=2em, in=125, out=55] & StrataValues                          \\
                    & MassPool \arrow[u]                                                           &  &                                   & StratumStore \arrow[u, shift right]                                                            &                       &          &  &                                 & StrataStore \arrow[u, shift right]                                                            & Strata \times StrataValues \arrow[lu]
\end{tikzcd}
```

Trivial product morphisms are omitted for brevity.

The category above uses several types, which are defined as follows:
* "Stratum"        `Stratum`                     a stack allocated array of Stratum objects with a static upper size bound
* "StratumStore"   `StrataStore`                 a compressed version of Stratum for storing large Stratum rasters
* "enums"          miscellaneous                 a set of enums that classify individual rock layers into geologically meaningful categories 
* "RockType"       `RockType`                    a large enums that classifies individual rock layers into rock types that can be interpreted by a human
