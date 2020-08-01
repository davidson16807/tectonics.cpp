the "strata" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe a vertical rock profile composed of strata,
and the properties of that strata. 
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZAZgBoAmAXVJADcBDAGwFcYkQBlHAJ3p2YC2XCNzYBfUuky58hFGQCM1Ok1bsuvfgJASp2PASIAWUkpoMWbRCBhhBCXSAz7Zx0gAZlFtdYBmjCD4dSSdpAzlkclMvVStOHj5BAAIAHRS8AXgkjUTtR2cZQxQADmjzWPUEnHpgvUKI0qpyy0rNemFRWtCXIuR3ChiW6xytAFl6ODgABQgIRg7xEILwon6zFSH4zUFxyZm5ruXXFAB2Mo2fLb56VPSsTLhsqpr8sOPkAE4PQcv-QJxDm9el91t44iN6AA1WIOJZAiIAVnOYPYACUIABjADWABUAJ5ocTKGBQADm8CIoF83Ag2kQ-RAOAgSAUNDgAAssL4AYgFI5qbSWTQmUgGRyuTy+SEBXTWYzmYhyPyabLhQriMrBYgTPKWZq6WddYhSiBxdzRfqkCaRYhDWaeUrpSqkEijQA2Nmc828y2ID1G132vVOrVfAO+102hTuCNqllSqnO3lx5MgAIQNBEBTFfq+JhwGDKRj0ABGMEYMx6chA3CwpPZANjAZo6czKGzufzhZbpfLlfq7Fr9cbIbpUSNCiVlDEQA)

Or it can be viewed with the following tikzcd markup:

```
\begin{tikzcd}
                                         &  &                                  &                                                                                                & float           &          &  &                                &                                                                                               & float        \\
StratumMassPool \arrow[d, shift right=2] &  & Stratum \times Stratum \arrow[r] & Stratum \arrow[d, shift right] \arrow[r] \arrow[ru] \arrow[loop, distance=2em, in=125, out=55] & enums \arrow[r] & RockType &  & Strata \times Strata \arrow[r] & Strata \arrow[d, shift right] \arrow[ru] \arrow[r] \arrow[loop, distance=2em, in=125, out=55] & StrataValues \\
StratumMassPoolStore \arrow[u]           &  &                                  & StratumStore \arrow[u, shift right]                                                            &                 &          &  &                                & StrataStore \arrow[u, shift right]                                                            &             
\end{tikzcd}
```

Trivial product morphisms are omitted for brevity.

The category above uses several types, which are defined as follows:
* "Stratum"        `Stratum`                     a stack allocated array of Stratum objects with a static upper size bound
* "StratumStore"   `StrataStore`                 a compressed version of Stratum for storing large Stratum rasters
* "enums"          miscellaneous                 a set of enums that classify individual rock layers into geologically meaningful categories 
* "RockType"       `RockType`                    a large enums that classifies individual rock layers into rock types that can be interpreted by a human
