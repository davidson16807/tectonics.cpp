#pragma once

#include <glm/vec3.hpp>     // *vec3

namespace rasters
{
	/*
	NOTE: 
	The naive implementation is to estimate the gradient based on each individual neighbor,
	 then take the average between the estimates.
	This is wrong! If dx is very small, such as where dy is big
	 then the gradient estimate along that dimension will be very big.
	This will result in very strange behavior.
	
	The intuition for the correct solution is as follows:
	Imagine a large assembly of puzzle pieces that have been fitted together. 
	It needn't be a fully finished puzzle board.
	Imagine dust blowing across the assembly. 
	We want to know how much dust blows across the assembly, and in which direction.
	This is known as the "flow", and it is represented as a vector. 
	If we know the flow for each puzzle piece, we could sum them together to find the net flow across the assembly. 
	Also, if we know the flow along each edge of each puzzle piece, we could similarly sum them together to find the net flow. 
	However, we also know that if two puzzle pieces are adjacent, the amount flowing out of one edge is cancelled out by the amount flowing into another. 
	So all we really need to know is the flow along the edge of each puzzle piece forming the boundary of the assembly. 
	This works no matter what the shape of the puzzle pieces are, and no matter what the shape of the assembly is. 
	It also works for other dimensions, so instead of dust blowing across puzzle pieces, we could consider air blowing through arbitrary regions of space. 

	TODO: 
	explain how flow scales linearly with volume
	explain how to figure out the flow across an edge
	explain how gradient times surface normal equals the difference between two points in space

	The above explanation can be restated mathematically by the Gauss-Green theorem: 
	  ∫∫∫ᵥ ∇ϕ dV = ∫∫ₐ ϕn̂ da
	for small V, we can assume linearity, so:
	  ∇ϕ = 1/V ∫∫ₐ ϕn̂ da
	and this is how we are able to find the gradient for an unstructured mesh. 

	The same logic applies equally well for divergence: 
	  ∫∫∫ᵥ ∇⋅ϕ dV = ∫∫ₐ ϕ⋅n̂ da
	  ∇⋅ϕ = 1/V ∫∫ₐ ϕ⋅n̂ da

	And curl: 
	  ∫∫∫ᵥ ∇×ϕ dV = ∫∫ₐ ϕ×n̂ da
	  ∇×ϕ = 1/V ∫∫ₐ ϕ×n̂ da

	See Sozer (2014) for more information, 
	See Book 2 Chapter 3 of the Feynman Lectures (http://www.feynmanlectures.caltech.edu/II_03.html) for intuition.

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
	
	*/
	template<typename T, qualifier Q>
	void gradient(
		const SpheroidGrid& grid, 
		const tmany<T>& scalar_field, 
		tmany<glm::vec<3,T,Q>>& out, 
		tmany<T>& arrow_differential, 
		tmany<glm::vec<3,T,Q>>& arrow_flow
	) {
		assert(scalar_field.size()       == grid.vertex_count);
		assert(out.size()                == grid.vertex_count);
		assert(arrow_differential.size() == grid.arrow_count );
		assert(arrow_flow.size()         == grid.arrow_count );
		uvec2 arrow;
		for (unsigned int i = 0; i < grid.arrow_vertex_ids.size(); ++i)
		{
			arrow                 = grid.arrow_vertex_ids[i]; 
			arrow_differential[i] = scalar_field[arrow.y] - scalar_field[arrow.x]; // differential across dual of the arrow
		}
		mult 	 (arrow_differential, grid.arrow_dual_normals,   arrow_flow);      // flux across dual of the arrow
		mult 	 (arrow_flow,         grid.arrow_dual_lengths,   arrow_flow);      // flow across dual of the arrow
		fill 	 (out,                vec3());
		aggregate_into(arrow_flow,    grid.arrow_vertex_id_from, [](glm::vec<3,T,Q> a, glm::vec<3,T,Q> b){ return a+b; }, out); // flow out from the vertex
		div      (out,                grid.vertex_dual_areas,    out);             // gradient
	}

	template<typename T, qualifier Q>
	void gradient(const SpheroidGrid& grid, const tmany<T>& scalar_field, tmany<glm::vec<3,T,Q>>& out)
	{
		tmany<T>               arrow_differential (grid.arrow_count);
		tmany<glm::vec<3,T,Q>> arrow_flow         (grid.arrow_count);
		gradient(grid, scalar_field, out, arrow_differential, arrow_flow);
	}

	template<typename T, qualifier Q>
	void divergence(
		const SpheroidGrid& grid, 
		const tmany<glm::vec<3,T,Q>>& vector_field, 
		tmany<T>& out, 
		tmany<glm::vec<3,T,Q>>& arrow_differential, 
		tmany<T>& arrow_projection
	) {
		assert(vector_field.size()       == grid.vertex_count);
		assert(out.size()                == grid.vertex_count);
		assert(arrow_differential.size() == grid.arrow_count );
		assert(arrow_projection.size()   == grid.arrow_count );
		uvec2 arrow;
		for (unsigned int i = 0; i < grid.arrow_vertex_ids.size(); ++i)
		{
			arrow                 = grid.arrow_vertex_ids[i]; 
			arrow_differential[i] = vector_field[arrow.y] - vector_field[arrow.x]; // differential across dual of the arrow
		}
		dot 	 (arrow_differential, grid.arrow_dual_normals,   arrow_projection);      // flux across dual of the arrow
		mult 	 (arrow_projection,   grid.arrow_dual_lengths,   arrow_projection);      // flow across dual of the arrow
		fill 	 (out,                0.f);
		aggregate_into(arrow_projection, grid.arrow_vertex_id_from, [](T a, T b){ return a+b; }, out);  // flow out from the vertex
		div      (out,                grid.vertex_dual_areas,    out);             // divergence
	}

	template<typename T, qualifier Q>
	void divergence(const SpheroidGrid& grid, const tmany<glm::vec<3,T,Q>>& vector_field, tmany<T>& out)
	{
		tmany<glm::vec<3,T,Q>> arrow_differential (grid.arrow_count);
		tmany<T>               arrow_projection   (grid.arrow_count);
		divergence(grid, vector_field, out, arrow_differential, arrow_projection);
	}

	template<typename T, qualifier Q>
	void curl(
		const SpheroidGrid& grid, 
		const tmany<glm::vec<3,T,Q>>& vector_field, 
		tmany<glm::vec<3,T,Q>>& out, 
		tmany<glm::vec<3,T,Q>>& arrow_differential, 
		tmany<glm::vec<3,T,Q>>& arrow_rejection
	) {
		assert(vector_field.size()       == grid.vertex_count);
		assert(out.size()                == grid.vertex_count);
		assert(arrow_differential.size() == grid.arrow_count );
		assert(arrow_rejection.size()    == grid.arrow_count );
		uvec2 arrow;
		for (unsigned int i = 0; i < grid.arrow_vertex_ids.size(); ++i)
		{
			arrow                 = grid.arrow_vertex_ids[i]; 
			arrow_differential[i] = vector_field[arrow.y] - vector_field[arrow.x]; // differential across dual of the arrow
		}
		cross 	 (arrow_differential, grid.arrow_dual_normals,   arrow_rejection);      // flux across dual of the arrow
		mult 	 (arrow_rejection,    grid.arrow_dual_lengths,   arrow_rejection);      // flow across dual of the arrow
		fill 	 (out,                vec3());
		aggregate_into(arrow_rejection, grid.arrow_vertex_id_from, [](glm::vec<3,T,Q> a, glm::vec<3,T,Q> b){ return a+b; }, out);  // flow out from the vertex
		div      (out,                grid.vertex_dual_areas,    out);             // curl
	}

	template<typename T, qualifier Q>
	void curl(const SpheroidGrid& grid, const tmany<glm::vec<3,T,Q>>& vector_field, tmany<glm::vec<3,T,Q>>& out)
	{
		tmany<glm::vec<3,T,Q>> arrow_differential (grid.arrow_count);
		tmany<glm::vec<3,T,Q>> arrow_rejection    (grid.arrow_count);
		curl(grid, vector_field, out, arrow_differential, arrow_rejection);
	}
}