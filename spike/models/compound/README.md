The "compound" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe the chemical properties of a compound and whether they are known or unknown.
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZABgBoBGAXVJADcBDAGwFcYkQBpEAX1PU1z5CKMgCZqdJq3YAFAAQAdBXgC28OV179seAkXKlxNBizaIQ8pavUyefEBh1D9FCSennbWhwN3CSpMRuUmYgADI8EjBQAObwRKAAZgBOECpIBiA4EEhkkqbsAMYQTPCFbN4paUiiNNlIAMw0jPQARjCMMr7O5owwiTggxiFFJX1w5XZJqemITVk5c80QEGj6ABxkiUxwMBIt7Z3deubJWDEAFoPDBeZYYIkwyVMgVbN59YgALDceIIX0QoXCqUbhAA)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```% https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZABgBoBGAXVJADcBDAGwFcYkQBpEAX1PU1z5CKMgCZqdJq3YAFAAQAdBXgC28OV179seAkXKlxNBizaIQ8pavUyefEBh1D9FCSennbWhwN3CSpMRuUmYgADI8EjBQAObwRKAAZgBOECpIBiA4EEhkkqbsAMYQTPCFbN4paUiiNNlIAMw0jPQARjCMMr7O5owwiTggxiFFJX1w5XZJqemITVk5c80QEGj6ABxkiUxwMBIt7Z3deubJWDEAFoPDBeZYYIkwyVMgVbN59YgALDceIIX0QoXCqUbhAA
\begin{tikzcd}
L                                &                                                        \\
K \arrow[u, "cache"]             & P \arrow["infer"', loop, distance=2em, in=125, out=55] \\
P \times K \arrow[u, "substitute"] & P \times P \arrow[u, "substitute"]                      
\end{tikzcd}
```

Trivial product and identity morphisms are omitted for brevity.

The category above uses two types, which are defined as follows:
* "L" lookups               an assortment of lookups for compound properties used by performance critical code
* "K" `CompletedCompound`   an object representing a compound for which useful estimates exist for all requried chemical properties
* "P" `PartlyKnownCompound` an object representing a compound for which some estimates may be provided for chemical properties

The
`substitute` and `infer` both have the increasing property with respect to the number of known compounds
`substitute` and `infer` both have an identity element, which is a `PartlyKnownCompound` where no chemical properties are known
