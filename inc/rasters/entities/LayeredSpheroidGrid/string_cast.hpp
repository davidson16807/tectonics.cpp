#pragma once

#include "../LayeredGrid/LayeredRaster.hpp"
#include "LayeredSpheroidGrid.hpp"
#include "../SpheroidGrid/string_cast.hpp"

#include "LayeredSpheroidGrid_test_utils.hpp"

namespace rasters
{


	template <typename T, typename Tid, typename Tfloat> 
	std::string to_string(
		const LayeredRaster<T,LayeredSpheroidGrid<Tid,Tfloat>>& a, 
		const uint line_char_width = 80
	) {
		assert(a.size() == a.grid.cache->vertex_count * a.grid.layering->layer_count);
		raster<T,SpheroidGrid<Tid,Tfloat>> layer_raster(a.grid);
		std::string out("");
		for (uint i = 0; i < a.grid.layering->layer_count; ++i)
		{
			get_layer(a, i, layer_raster);
			out += to_string(layer_raster, min(a), max(a), line_char_width);
			out += "\n";
		}
		return out;
	}

	template <typename T, typename Tid, typename Tfloat, glm::qualifier Q> 
	std::string to_string(
		const LayeredRaster<glm::vec<2,T,Q>,LayeredSpheroidGrid<Tid,Tfloat>>& a, 
		const uint line_char_width = 80, 
		const glm::vec3 up = glm::vec3(0,0,1)
	) {
		assert(a.size() == a.grid.cache->vertex_count * a.grid.layering->layer_count);
		raster<glm::vec<2,T,Q>,SpheroidGrid<Tid,Tfloat>> layer_raster(a.grid);
		std::string out("");
		for (uint i = 0; i < a.grid.layering->layer_count; ++i)
		{
			get_layer(a, i, layer_raster);
			out += to_string(layer_raster, line_char_width);
			out += "\n";
		}
		return out;
	}

	template <typename T, typename Tid, typename Tfloat, glm::qualifier Q> 
	std::string to_string(
		const LayeredRaster<glm::vec<3,T,Q>,LayeredSpheroidGrid<Tid,Tfloat>>& a, 
		const uint line_char_width = 80, 
		const glm::vec3 up = glm::vec3(0,0,1)
	) {
		assert(a.size() == a.grid.cache->vertex_count * a.grid.layering->layer_count);
		raster<glm::vec<3,T,Q>,SpheroidGrid<Tid,Tfloat>> layer_raster(a.grid);
		std::string out("");
		for (uint i = 0; i < a.grid.layering->layer_count; ++i)
		{
			get_layer(a, i, layer_raster);
			out += to_string(a.grid.cache->vertex_normals, layer_raster, line_char_width, up);
			out += "\n";
		}
		return out;
	}

}//namespace rasters



