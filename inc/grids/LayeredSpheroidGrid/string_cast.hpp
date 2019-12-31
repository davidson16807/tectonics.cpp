#pragma once

#include "../LayeredSpheroidGrid/LayeredSpheroidGrid.hpp"
#include "../SpheroidGrid/string_cast.hpp"

namespace rasters
{


	template <typename T>
	std::string to_string(
		const LayeredSpheroidGrid& grid, 
		const many::tmany<T>& a, 
		const uint line_char_width = 80
	) {
		assert(a.size() == grid.vertex_count * grid.layer_count);
		many::tmany<T> layer_raster(grid.vertex_count);
		std::string out("");
		for (uint i = 0; i < grid.layer_count; ++i)
		{
			get_layer(a, i, layer_raster);
			out += to_string(grid.voronoi, layer_raster, min(a), max(a), line_char_width);
			out += "\n";
		}
		return out;
	}

	template <typename T, glm::qualifier Q>
	std::string to_string(
		const LayeredSpheroidGrid& grid, 
		const many::tmany<glm::vec<2,T,Q>>& a, 
		const uint line_char_width = 80, 
		const glm::vec3 up = vec3(0,0,1)
	) {
		assert(a.size() == grid.vertex_count * grid.layer_count);
		many::tmany<glm::vec<2,T,Q>> layer_raster(grid.vertex_count);
		std::string out("");
		for (uint i = 0; i < grid.layer_count; ++i)
		{
			get_layer(a, i, layer_raster);
			out += to_string(grid.voronoi, layer_raster, line_char_width);
			out += "\n";
		}
		return out;
	}

	template <typename T, glm::qualifier Q>
	std::string to_string(
		const LayeredSpheroidGrid& grid, 
		const many::tmany<glm::vec<3,T,Q>>& a, 
		const uint line_char_width = 80, 
		const glm::vec3 up = vec3(0,0,1)
	) {
		assert(a.size() == grid.vertex_count * grid.layer_count);
		many::tmany<glm::vec<3,T,Q>> layer_raster(grid.vertex_count);
		std::string out("");
		for (uint i = 0; i < grid.layer_count; ++i)
		{
			get_layer(a, i, layer_raster);
			out += to_string(grid.voronoi, grid.vertex_normals, layer_raster, line_char_width, up);
			out += "\n";
		}
		return out;
	}

}//namespace rasters



