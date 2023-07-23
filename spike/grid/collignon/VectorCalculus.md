
The naive implementation is to estimate gradient/divergence/etc. based on each individual neighbors,
 then take the average between the estimates.
This is wrong! If dx is very small, such as where dy is big,
 then the gradient estimate along that dimension will be very big.
This will result in very strange behavior.

The intuition for the correct solution is as follows:
Imagine a large assembly of puzzle pieces that have been fitted together. 
It needn't be a fully finished puzzle board.
Imagine dust blowing across the assembly. 
We want to know how much dust blows across the assembly, and in which direction.
This is known as the "flow", and it is represented as a vector. 
We know that if two puzzle pieces are adjacent, the amount flowing out of one edge is cancelled out by the amount flowing into another. 
So all we really need to consider is the flow along the boundaries of the entire assembly. 
We can see this is the case without even establishing what the puzzle looks like,
so this works no matter what the shape of the puzzle pieces are, and no matter what the shape of the assembly is. 
It also works for other use cases in other dimensions, 
so instead of dust blowing across puzzle pieces, we could consider air blowing through arbitrary regions of space. 

Now, what if our puzzle pieces are infinitesimally small?
We can see that the net flow across two small puzzle pieces is twice the flow across one small puzzle piece.
Because they're so small, scaling laws concerning surface area and volume don't matter here.
In other words, flow scales linearly with volume at small scales.

TODO: 
explain how to figure out the flow across an edge
explain how gradient times surface normal equals the difference between two points in space

The above explanation can be restated mathematically by the Gauss-Green theorem: 
  ∫∫∫ᵥ ∇ϕ ∂V = ∫∫ₛ ϕn̂ ∂s
for infinitesimal volumes, we can assume that contents are uniform, so results scale with volume, and:
  ∇ϕ = 1/V ∫∫ₛ ϕn̂ ∂s
and this is how we are able to find the gradient for an unstructured mesh. 

The same logic applies equally well for divergence: 
  ∫∫∫ᵥ ∇⋅ϕ ∂V = ∫∫ₛ ϕ⋅n̂ ∂s
  ∇⋅ϕ = 1/V ∫∫ₛ ϕ⋅n̂ ∂s

And curl: 
  ∫∫∫ᵥ ∇×ϕ ∂V = ∫∫ₛ ϕ×n̂ ∂s
  ∇×ϕ = 1/V ∫∫ₛ ϕ×n̂ ∂s

See Sozer (2014) for more information, 
See Book 2 Chapter 3 of the Feynman Lectures (http://www.feynmanlectures.caltech.edu/II_03.html) for intuition.

We've now discussed both the intuition and the mathematics, here's the implementation:

Instead of puzzle pieces, let's picture vertices.
Each vertex has a vector mapped to it, analogous to the flow of dust across a puzzle piece.
Each vertex also has a region around it that's analogous to the puzzle piece, 
known as a "dual" in tectonics.cpp, since it is analogous to the dual of a graph in graph theory. 
The vertices of the dual are set to the midpoints of the faces that connect the original vertices.
The duel of a vertex is a boundary, and as such it has its own surface normal.
We want to find the divergence, which is a single floating point value 
that's positive when the vectors are pointing away from a region.
To find this, we need to consider both the vertex itself and the vertices surrounding it. 
We take the difference in vectors across each neighboring pair of vertices
then 

Imagine a vertex with several neighors surrounding it. 
The vertex is known as a "centroid".
The neighbors should be approximately the same distance from the centroid. 
Now imagine a circle surrounding the centroid. 
The edge of the circle lies halfway between the centroid and its neighbors.
Now imagine unit vectors along the edge of the circle, 
 one for each neighbor, pointing from the centroid to the neighbor.
These represent the surface normals of the circle. 
Now imagine each neighbor owning a certain portion of the circle, 
 like slices of a pie. 
Multiply the surface normals by the difference in the scalar field from the centroid to the neighbor.
This value is the flux. 
Multiply the flux for each neighbor by the length of the neighbor's pie slice crust.
This value is the flow.
Divide flow by the area of the pie.
This value is the gradient.

If you want to consider the 3d gradient, 
 simply apply the same process to a sphere, 
 only now weighting by outer surface area of the spherical sector,
 and dividing by the volume of the sphere.

If the neighbors are not spaced equally apart from the centroid,
 or are not spaced evenly apart from one another,
 you can still apply the same technique to an arbitrary shape,
 so create a polygon from the midpoints of the faces surrounding the centroid
 then calculate the area and perimeter lengths from the polygon.
This polygon is part of the "dual" of the graph that's formed from the centroid and its neighbors.
