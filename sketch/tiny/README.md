Several places in the library require storing hundreds of MB worth of contiguous data structures.
The data structures are often accessed out of order, and so data locality has a huge impact on performance.
Optimizing for performance often reduces to a matter of minimizing memory footprint.
To reduce memory footprint, the attributes of the most frequently used data structures 
 are densely packed into small data types such as uint8s or uint4s.
There are typically many such attributes within a single data structure,
 and they typically represent real numbers that in ideal situations would be stored within floats.
Each attribute has values that commonly fall within a known range.
Some may require precision on a linear scale, while others may require precision on a log scale.
We may try manipulating some attributes within these data structures, 
 but when doing so we must consider how they are represented as integers,
 since the amount incremented over a single timestep may be to small to represent within a uint8s or uint4s.
We must also avoid overflow or underflow errors when doing so.
Since uint4s are represented differently within memory depending on platform,
we must also find a way to store these attributes on a hard drive using a single canonical representation.

A "uint4_byte" is a data structure that addresses these considerations.
It represents a 4 byte word that is populated by an array of 4 bit unsigned integers.
It is the smallest size collection of uint4s that can be addressed
It hides the underyling uint4s since their modification is highly error prone.
It instead offers conversions to and from several less restrictive formats,
 each of which suite different use cases.

There are 2 formats it is intended to convert between:
* a "canonical" byte representation that can be stored on a hard drive and shared across computers 
* an integer representation of a parameter space that can be easily modified 
  without causing errors related to precision, overflow, or underflow.

So the category diagram looks something like the following:

`canonical <=> uint4 <=> int8`