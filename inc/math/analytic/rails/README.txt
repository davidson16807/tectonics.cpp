
This directory, "rails", stores classes that represent mathematical expressions
centered around creating piecewise functions. 
It uses a dedicated paradigm to implement piecewise functions in such a way
that makes it easy to perform common operations on piecewise functions,
such as arithmetic, integration, derivation, and function composition.
This paradigm centers around a well known mathematical concept called the 
[boxcar function](https://en.wikipedia.org/wiki/Boxcar_function),
around which additional concepts are introduced, starting with what is known as a "railcar".

a "railcar" can be considered a function f(x) of type 𝔽 multiplied 
by what is commonly called the "boxcar" function, 
which we define here as boxcar(x)=1 for lo<x≤hi and boxcar(x)=0 otherwise.†
Here the variables "lo" and "hi" are known as the "couplers" for a given railcar.
The discontinuities of a railcar resemble the front and back of a boxcar, 
but the plotted shape of the railcar does not strictly resemble a box (hence "railcar").

An equivalent definition is to say railcar(x)=f(x) if lo<x≤hi and railcar(x)=0 otherwise, 
for some function f∈𝔽. 
The railcar here is said to be a "railcar of 𝔽", or an "𝔽 railcar", that "contains" f(x), 
such that f(x) is "inside" the railcar, or f(x) is its "contents", and so forth.
By this definition, a boxcar is a railcar of the identity function.

The intent in introducing the concept of a "railcar" is to represent piecewise functions as sums of railcars,
thereby providing an easy way to close sums under common operations for some kinds of underlying functions.
The name "railcar" is chosen partly because it leverages the existing concept of a "boxcar" function,
but also because it provide a rich and intuitive metaphor that resolves ambiguities in conversation, see below.

Two railcars, A and B, are said to "overlap" if max(A.lo,B.lo) < min(A.hi,B.hi).
In contrast, A and B are said to have a "gap" if max(A.lo,B.lo) > min(A.hi,B.hi),
and A and B are said to be "coupled" or "adjacent" if max(A.lo,B.lo) = min(A.hi,B.hi).
If A and B are coupled, the value x = max(A.lo,B.lo) = min(A.hi,B.hi) is said to be the "coupling" of A and B.
A sum of railcars is known as a "train" if no overlaps or gaps exist between railcars
A sum of railcars is known as a "railyard" if overlaps and gaps are permitted to exist.
Terminology regarding the "contents" of railcars will also be used to describe the trains and railyards that house them,
so for instance a "polynomial train" is a train whose railcars contain polynomials.
A "spline" therefore is a polynomial train where adjacent cars share the same first derivative at their coupling.
A spline of order N additionally satisfies the same condition for derivatives up to order N.

Railyards can be useful since they easily extend the properties of their contents, for instance:
 * for any class of function 𝔽, the railyard of 𝔽 is closed under addition
 * for any class of function 𝔽, if 𝔽 is closed under subtraction, multiplication, 
   derivation, or integration, so to will the railyard of 𝔽.
Trains can be useful since they minimize the number of terms that need to be calculated,
The boxcars that underlie trains are disjoint, which permits trains to perform certain operations 
that would not be allowed on railyards.
As an example: 
 * rational trains are closed under division, however rational railyards are not
 * the degree of a rational train's polynomials cannot be known at compile time 
   if that train has been converted from a railyard.
Railyards make it easy to performantly implement certain kinds of operations that 
would not be performant if an implementation of a train were to guarantee that no overlaps or gaps exist. 
As an example, adding two railyards simply requires contatenating two lists of cars together,
whereas adding two trains together in such a scenario requires constructing a totally new list of cars.
However, it is almost certainly more performant to call the operator() method on a train as opposed to a railyard,
which is typically the only performance that matters.

† The choice of the range inclusivity "lo<x≤hi", as opposed to other choices like "lo≤x<hi", is mostly arbitrary 
and exists only because a choice was needed that would allow the representation of continuous piecewise functions.
Were this a mathematical exercise, we could define any given railyard as following a certain "coupling standard", 
but in a software framework this only introduces added work and potential intercompatiblity issues.