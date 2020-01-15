#pragma once

//#include <iostream>     // std::cout

#include <many/types.hpp>    // floats, etc.
#include <many/common.hpp>       // max
#include <many/statistic.hpp>    // mean
#include <many/glm/types.hpp>// *vec*s

#include "SpheroidGrid.hpp"

namespace rasters {

	/*
	The LayeredSpheroidGrid class is an extension of the Grid class.
	It represents a grid of stratified cells above the surface of a roughly spherical object.
	As an example, it could be used to represent state within a 3d global climate model,
	where several layers of air are represented above the surface of the world. 
	As with Spheroidgrid, LayeredSpheroidGrid specifically requires that all 
	face normals have positive cosine similarity with their midpoints.
	This means the grid can still work with slightly lumpy, slightly concave, and slightly noncentered shapes, 
	but it must be trivial to map lat long coordinates to vertices and determine surface normals from vertices.
	*/
	class LayeredSpheroidGrid : public Grid
	{
	public:
		SpheroidGridVoronoi voronoi;

		~SpheroidGrid()
		{

		}
		explicit SpheroidGrid(const many::vec3s& vertices, const many::uvec3s& faces)
			: Grid(vertices, faces),
			  voronoi(vertices, 
					min(arrow_lengths / 8.f), 
					max(arrow_lengths * 1.f)
				)
		{

		}
		void get_ids(const vec3s& points, tmany<uint>& out) const
		{
			voronoi.get_values(points, out);
		}
	};
}




