#pragma once

#include "../LayeredSpheroidGrid/LayeredSpheroidGrid.hpp"

namespace rasters
{
	template<typename T, glm::qualifier Q>
	void gradient(
		const LayeredSpheroidGrid& grid, 
		const many::tmany<T>& scalar_field, 
		many::tmany<glm::vec<3,T,Q>>& out, 
		many::tmany<T>& arrow_differential, 
		many::tmany<T>& layer_differential, 
		many::tmany<glm::vec<3,T,Q>>& arrow_flow,
		many::tmany<glm::vec<3,T,Q>>& layer_flow
	) {
		assert(scalar_field.size()       == grid.vertex_count * grid.layer_count );
		assert(out.size()                == grid.vertex_count * grid.layer_count );
		assert(arrow_differential.size() == grid.arrow_count  * grid.layer_count );
		assert(arrow_flow.size()         == grid.arrow_count  * grid.layer_count );
		assert(layer_differential.size() == grid.vertex_count * grid.layer_count );
		assert(layer_flow.size()         == grid.vertex_count * grid.layer_count );
		uint L = grid.layer_count;
		uvec2 arrow;
		for (unsigned int i = 0; i < grid.arrow_vertex_ids.size(); ++i)
		{
			arrow = grid.arrow_vertex_ids[i]; 
			for (unsigned int j = 0; j < L; ++j)
			{
				arrow_differential[i*L+j] = scalar_field[arrow.y*L+j] - scalar_field[arrow.x*L+j]; // differential across dual of the arrow
			}
		}
		mult 	(arrow_differential, grid.arrow_dual_normals,   arrow_flow);      // flux across dual of the arrow 
		mult 	(arrow_flow,         grid.arrow_dual_lengths,   arrow_flow);
		mult 	(arrow_flow,         grid.layer_height,         arrow_flow);      // flow across dual of the arrow 

		fill    (layer_differential, T(0));
		for (unsigned int i = 0; i < grid.vertex_count; ++i)
		{
			for (unsigned int j = 1; j < L; ++j)
			{
				layer_differential[i*L+j-1] += scalar_field[i*L+j] - scalar_field[i*L+j-1]; // /2; NOTE: 2 cancels out
				layer_differential[i*L+j]   += scalar_field[i*L+j] - scalar_field[i*L+j-1]; // /2; NOTE: 2 cancels out
			}
		}
		mult    (layer_differential, grid.vertex_normals,       layer_flow);      // flux across layer boundary 
		mult    (layer_flow,         grid.vertex_dual_areas,    layer_flow);      // flow across layer boundary

		copy 	(out,                layer_flow);
		for (unsigned int i = 0; i < grid.arrow_vertex_id_from.size(); ++i)
		{
			for (unsigned int j = 0; j < L; ++j)
			{
				out[grid.arrow_vertex_id_from[i]*L+j] += arrow_flow[i*L+j];
			}
		}

		div 	(out,                grid.vertex_dual_areas,    out);
		div 	(out,                grid.layer_height,         out);             // gradient
	}

	template<typename T>
	many::tmany<glm::vec3> gradient(const LayeredSpheroidGrid& grid, const many::tmany<T>& scalar_field)
	{
		many::tmany<T>         arrow_differential (grid.arrow_count  * grid.layer_count);
		many::tmany<T>         layer_differential (grid.vertex_count * grid.layer_count);
		many::tmany<glm::vec3> arrow_flow         (grid.arrow_count  * grid.layer_count);
		many::tmany<glm::vec3> layer_flow         (grid.vertex_count * grid.layer_count);
		many::tmany<glm::vec3> out                (grid.vertex_count * grid.layer_count);
		gradient(grid, scalar_field, out, arrow_differential, layer_differential, arrow_flow, layer_flow);
		return out;
	}
}