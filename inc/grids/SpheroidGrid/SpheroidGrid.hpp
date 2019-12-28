#pragma once

//#include <iostream>     // std::cout

#include <many/types.hpp>    // floats, etc.
#include <many/common.hpp>       // max
#include <many/statistic.hpp>    // mean
#include <many/glm/types.hpp>// *vec*s

#include "../Grid/Grid.hpp"
#include "SpheroidGridVoronoi.hpp"

namespace rasters {

	// The SpheroidGrid class is an extension of the Grid class.
	// It offers additional functionality by assuming the grid works on the surface of a roughly spherical shape.
	// It specifically requires that all face normals have positive cosine similarity with their midpoints.
	// This means the grid can still work with slightly lumpy, slightly concave, and slightly noncentered shapes, 
	//  but it must be trivial to map lat long coordinates to vertices and determine surface normals from vertices.
	class SpheroidGrid : public Grid
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




