#pragma once

#include <glm/vec3.hpp>     // *vec3

namespace rasters
{
	template<typename T, qualifier Q>
	void gradient(const SpheroidGrid& grid, const tmany<T>& scalar_field, tmany<glm::vec<3,T,Q>>& out, tmany<T>& arrow_differential, tmany<glm::vec<3,T,Q>>& arrow_flow)
	{
		assert(scalar_field.size()       == grid.vertex_count);
		assert(out.size()                == grid.vertex_count);
		assert(arrow_differential.size() == grid.arrow_count );
		assert(arrow_flow.size()         == grid.arrow_count );
		/*
		NOTE: 
		The naive implementation is to estimate the gradient based on each individual neighbor,
		 then take the average between the estimates.
		This is wrong! If dx is very small, such as where dy is big
		 then the gradient estimate along that dimension will be very big.
		This will result in very strange behavior.
		
		The intuition for the correct solution is as follows:
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
		
		The math behind this relies on the Gauss-Green theorem: 
		  ∫∫∫ᵥ ∇ϕ dV = ∫∫ₐ ϕn̂ da
		for small V, we can assume linearity, so:
		  ∇ϕ = 1/V ∫∫ₐ ϕn̂ da
		See Sozer (2014) for more information.
		*/
		uvec2 arrow;
		for (unsigned int i = 0; i < grid.arrow_vertex_ids.size(); ++i)
		{
			arrow                 = grid.arrow_vertex_ids[i]; 
			arrow_differential[i] = scalar_field[arrow.y] - scalar_field[arrow.x]; // differential across dual of the arrow
		}
		mult 	 (arrow_differential, grid.arrow_dual_normals,   arrow_flow);      // flux across dual of the arrow
		mult 	 (arrow_flow,         grid.arrow_dual_lengths,   arrow_flow);      // flow across dual of the arrow
		aggregate_into(arrow_flow,         grid.arrow_vertex_id_from, [](glm::vec<3,T,Q> a, glm::vec<3,T,Q> b){ return a+b; }, out); // flow out from the vertex
		div      (out,                grid.vertex_dual_areas,    out);             // gradient
	}

	template<typename T, qualifier Q>
	void gradient(const SpheroidGrid& grid, const tmany<T>& scalar_field, tmany<glm::vec<3,T,Q>>& out)
	{
		tmany<T>               arrow_differential (grid.arrow_count);
		tmany<glm::vec<3,T,Q>> arrow_flow         (grid.arrow_count);
		gradient(grid, scalar_field, out, arrow_differential, arrow_flow);
	}
}