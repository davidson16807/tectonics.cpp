#pragma once

#include <series/types.hpp>
#include <series/glm/types.hpp>

#include "LayeredGrid.hpp"
#include "LayeredRaster.hpp"

namespace rasters 
{

	// e.g. f = F₀
	template <typename Tgrid1, typename Tgrid2, typename T, rasters::mapping Tmap>
	void get_layer(
		const LayeredRaster<T, Tgrid1, Tmap>& a, 
		const std::size_t layer_id, 
		Raster<T, Tgrid2, Tmap>& output
	){
		assert(a.grid.structure == output.grid.structure);
		assert(a.grid.metrics == output.grid.metrics);
		std::size_t L = a.grid.layering->layer_count;
		std::size_t V = a.grid.structure->vertex_count;
		for (std::size_t i = 0; i < V; ++i)
		{
			output[i] = a[i*L+layer_id];
		}
	}

	// e.g. F₀ = f
	template <typename Tgrid1, typename Tgrid2, typename T, rasters::mapping Tmap>
	void set_layer(
		const LayeredRaster<T, Tgrid1, Tmap>& a, 
		const std::size_t layer_id,
		const Raster<T, Tgrid2, Tmap>& value, 
		LayeredRaster<T, Tgrid1, Tmap>& output
	){
		assert(a.grid.structure == value.grid.structure);
		assert(a.grid.metrics == value.grid.metrics);
		assert(a.grid == output.grid);
		if (&a != &output){ series::copy(output, a); }
		std::size_t L = a.grid.layering->layer_count;
		std::size_t V = a.grid.structure->vertex_count;
		for (std::size_t i = 0; i < V; ++i)
		{
			output[i*L+layer_id] = value[i];
		}
	}

	// e.g. Fᵢ = f
	template <typename Tgrid1, typename Tgrid2, typename T, rasters::mapping Tmap>
	void repeat_layers(
		const Raster<T, Tgrid2, Tmap>& a, 
		LayeredRaster<T, Tgrid1, Tmap>& output
	){
		assert(a.grid.structure == output.grid.structure);
		assert(a.grid.metrics == output.grid.metrics);
		std::size_t L = output.grid.layering->layer_count;
		for (std::size_t i = 0; i < a.size(); ++i)
		{
			for (std::size_t j = 0; j < L; ++j)
			{
				output[i*L+j] = a[i];
			}
		}
	}


}
