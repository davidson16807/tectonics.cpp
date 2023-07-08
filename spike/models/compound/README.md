# The `compound` Library

## Overview

This library offers estimates for the chemical properties of compounds that occur commonly in earth and planetary science.

The library consists of two sections:
	* a section that only provides published values that were reported in scientific literature, 
	  or approximations of those values if they were reported in tables or figures.
	* a section that is guaranteed to provide reasonable estimates for any compound regardless of phase, pressure, or temperature, 
	  even if it must be inferred from what is known about other properties or compounds

The first section offers the following guarantees:
	* The library will never deliberately throw exceptions.
	* The library will never provide estimates that are not reflected within scientific literature.

The second section offers the following guarantees:
	* The library will never deliberately throw exceptions.
	* The library will never produce NaNs, nulls, monostates, or any other value aside from the best informed estimate.
	* The library will never produce values that fall outside the orders of magnitude that are reported for that property in literature.
	* The library will never extrapolate values past the ranges of pressure or temperature that were reported in literature - 
	  if an estimate is requested outside the range of reported values, it will return the nearest estimate from within that range.

Both sections of the the library use function approximations to represent values that were reported in literature using tables or figures.
The approximations are designed so that they can be manipulated by the user to suit their application.
For instance, if the user requires the approximation be used in a high performance application but only requires it to run in earth-like conditions,
they can invoke a method that restrict the approximation to a given range. 
This will create a new approximation that is automatically simplified to suit the range.
Approximations store estimates of the their valid range and the maximum error that occurs within that range,
so users can track how errors accumulate across multiple manipulations, 
and whether a given approximation is suitable for their needs.

# Implementation

The "compound" library is implemented as a set of data structures and functions that define a mathematical category.
The category is composed from types that describe the chemical properties of a compound and whether they are known or unknown.
Types and morphisms within the category are each represented within their own file.

```
% https://q.uiver.app/?q=WzAsOSxbMywxLCJQIFxcdGltZXMgQyJdLFsyLDFdLFszLDIsInN0ciJdLFsxLDEsIlAgXFx0aW1lcyBQIl0sWzAsMCwiUCJdLFszLDAsIkMiXSxbMSwyLCJzdHIiXSxbMCwyLCJTUFMiXSxbMiwwLCJQIl0sWzAsNSwiY29tcGxldGUiXSxbMyw0LCJpbmZlciJdLFszLDZdLFswLDJdLFszLDddLFszLDgsInNwZWN1bGF0ZSJdXQ==
\begin{tikzcd}
	P && P & C \\
	& {P \times P} & {} & {P \times C} \\
	SPS & str && str
	\arrow["complete", from=2-4, to=1-4]
	\arrow["infer", from=2-2, to=1-1]
	\arrow[from=2-2, to=3-2]
	\arrow[from=2-4, to=3-4]
	\arrow[from=2-2, to=3-1]
	\arrow["speculate", from=2-2, to=1-3]
\end{tikzcd}
```

Trivial product and identity morphisms are omitted for brevity.

The category above uses two types, which are defined as follows:
* "SPS" `StateParameterSamples` an assortment of lookups for compound properties used by performance critical code
* "C" `CompletedCompound`       an object representing a compound for which useful estimates exist for all requried chemical properties
* "P" `PartlyKnownCompound`     an object representing a compound for which some estimates may be provided for chemical properties

The
`speculate` and `infer` both have the increasing property with respect to the number of known compounds
`speculate` and `infer` both have an identity element, which is a `PartlyKnownCompound` where no chemical properties are known

