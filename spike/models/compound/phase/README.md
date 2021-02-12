The "phase" namespace exclusively represents data structures and functions that define a mathematical category.
The category is composed from types that describe the chemical properties of a compound in a given phase,
and whether they are known or unknown.
Types and morphisms within the category are each represented within their own file.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZABgBoAWAXVJADcBDAGwFcYkQBxEAX1PU1z5CKAEwVqdJq3YAZHnxAZseAkXLiaDFm0QgAyvP7KhRAIykArBK3TdHAAQAdR3gC28e3N5HBqlAGZLaykdEBknFyx3OHsDb0UBFWESUn9g7XYHZzcPDgB9EUME4z9kMTTNENkInJiZAqKlX2T1CskM3T0aqI89BvimpKIyEXTbTn6FQZNRUlHKjrDJnyGUdXn28b7CgcSZlKsF8azI6PsuXZLksUPN0PDsnrrGvdL1W5tQrsezuKnX5JkUxjUL5fwvK5EMTAo73PLgy7NNSkGF3dh9BH-SFrUjEEHovLkHgSGBQADm8CIoAAZgAnCCuJDqEA4CBIUzxOkMpk0VlIESc+mMxCBFlsxAchRc4WivmIMggRj0ABGMEYAAUAexaVgyQALHBFaXs3ni8yKiAQNBmAAcZGpTDgMAkStVGq1uh1+sNgu5iDEYv5NEYlutKFMdtIDsYTpdKrVmuxIC9BqNQqQCrlCpDVtt9sdzuD8fdSZTPql6cQFlNSBtvuFADYa4gAJz1pAAdmbpmI7YlsvFAorftMAblkppldM5qzfZt3fIfZ73YbfZb3YsS6bgYlHe4lG4QA)

Or it can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

```% https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZABgBoAWAXVJADcBDAGwFcYkQBxEAX1PU1z5CKAEwVqdJq3YAZHnxAZseAkXLiaDFm0QgAyvP7KhRAIykArBK3TdHAAQAdR3gC28e3N5HBqlAGZLaykdEBknFyx3OHsDb0UBFWESUn9g7XYHZzcPDgB9EUME4z9kMTTNENkInJiZAqKlX2T1CskM3T0aqI89BvimpKIyEXTbTn6FQZNRUlHKjrDJnyGUdXn28b7CgcSZlKsF8azI6PsuXZLksUPN0PDsnrrGvdL1W5tQrsezuKnX5JkUxjUL5fwvK5EMTAo73PLgy7NNSkGF3dh9BH-SFrUjEEHovLkHgSGBQADm8CIoAAZgAnCCuJDqEA4CBIUzxOkMpk0VlIESc+mMxCBFlsxAchRc4WivmIMggRj0ABGMEYAAUAexaVgyQALHBFaXs3ni8yKiAQNBmAAcZGpTDgMAkStVGq1uh1+sNgu5iDEYv5NEYlutKFMdtIDsYTpdKrVmuxIC9BqNQqQCrlCpDVtt9sdzuD8fdSZTPql6cQFlNSBtvuFADYa4gAJz1pAAdmbpmI7YlsvFAorftMAblkppldM5qzfZt3fIfZ73YbfZb3YsS6bgYlHe4lG4QA
\begin{tikzcd}
                                             &                                  &                                              &                                  & S_4                                          \\
G_3                                          &                                  & L_3                                          &                                  & S_3 \arrow[u]                                \\
G_2 \arrow[u]                                &                                  & L_2 \arrow[u]                                &                                  & S_2 \arrow[u]                                \\
G \times G_2 \arrow[u]                       &                                  & L \times L_2 \arrow[u]                       &                                  & S \times S_2 \arrow[u]                       \\
G \arrow[loop, distance=2em, in=125, out=55] &                                  & L \arrow[loop, distance=2em, in=125, out=55] &                                  & S \arrow[loop, distance=2em, in=125, out=55] \\
G \times G \arrow[u]                         & G \times L \arrow[ru] \arrow[lu] & L \times L \arrow[u]                         & L \times S \arrow[lu] \arrow[ru] & S \times S \arrow[u]                        
\end{tikzcd}
```

Trivial product and identity morphisms are omitted for brevity.

The category above uses 3 "stages" of types, which are defined as follows:
##Stage 1: "Partly Known" entries
This stage represents compounds in which one or more properties lack published values. 
It separates concerns related to data entry, namely the need to separate known good values from published research 
versus handwaved estimates that are derived simply to avoid dealing with missing data while the model is running.
Objects in stage 1 can be composed in several ways, each building up on the number of properties that can be inferred about a compound, 
up until it is ready to progress to stage 2. This allows us to document the process by which estimates are inferred,
and separate out any entries that are less credible.
The mapping to stage 2 requires an existing stage 2 object to provide defaults if any values are still missing in the stage 1 object.
* "L"  `PartlyKnownLiquid`
* "G"  `PartlyKnownGas`
* "S"  `PartlyKnownSolid`

##Stage 2: "Completed" entries
This stage features entries for compounds whose properties have one way or another been populated to the point where 
they can guarantee at least representative values for every property that could be needed by the model.
This could be accomplished either because published values really do provide everything the model needs,
or because the missing values have been filled in with guesses.
Either way, this stage guarantees that sensible values will be available for the compound upon request. 
It provides a way to guarantee values 
* "L2" `CompletedLiquid`
* "G2" `CompletedGas`
* "S2" `CompletedSolid`

##Stage 3: "Authoritative" entries
This stage separates concerns regarding ease of use vs ease of data entry. 
Stage 3 is easy to use whereas data entry was easier for previous stages.
Previous stages supplied information as single representative values 
with the option to supply a function that describes a property over a range of conditions if that was needed.
However we don't want to perform a check on a data type every time we want to access a value, 
so stage 3 stores properties in a single function that either returns the underlying representative value or function.
We could take the object oriented route and encapsulate the functions/values in a class with a single callable method, 
but we enjoy being able to supply functions to our methods without requiring them to depend on a class or interface,
so it is represented only as a function.
* "L3" `AuthoritativeLiquid`
* "G3" `AuthoritativeGas`
* "S3" `AuthoritativeSolid`

##Stage 4: Acceleration structures
This stage is optional and addresses performance concerns.
The functions used to describe properties in stage 3 might be too slow for some of our high performance needs,
so we might want to whittle them down to a lookup table or even to a single representative value that's appropriate 
for some average temperature on a world. This stage describes such acceleration structures.
Stage 4 will likely only apply to solids since gas and liquids produce mixtures that are fairly uniform (e.g. air, saltwater, or magma)

