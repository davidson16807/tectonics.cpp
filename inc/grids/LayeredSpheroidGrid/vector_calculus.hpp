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


	template<typename T, glm::qualifier Q>
	void divergence(
		const LayeredSpheroidGrid& grid, 
		const many::tmany<glm::vec<3,T,Q>>& vector_field, 
		many::tmany<glm::vec<3,T,Q>>& out, 
		many::tmany<glm::vec<3,T,Q>>& arrow_differential, 
		many::tmany<glm::vec<3,T,Q>>& layer_differential, 
		many::tmany<T>& arrow_projection,
		many::tmany<T>& layer_projection
	) {
		assert(vector_field.size()       == grid.vertex_count * grid.layer_count );
		assert(out.size()                == grid.vertex_count * grid.layer_count );
		assert(arrow_differential.size() == grid.arrow_count  * grid.layer_count );
		assert(arrow_projection.size()   == grid.arrow_count  * grid.layer_count );
		assert(layer_differential.size() == grid.vertex_count * grid.layer_count );
		assert(layer_projection.size()   == grid.vertex_count * grid.layer_count );
		uint L = grid.layer_count;
		uvec2 arrow;
		for (unsigned int i = 0; i < grid.arrow_vertex_ids.size(); ++i)
		{
			arrow = grid.arrow_vertex_ids[i]; 
			for (unsigned int j = 0; j < L; ++j)
			{
				arrow_differential[i*L+j] = vector_field[arrow.y*L+j] - vector_field[arrow.x*L+j]; // differential across dual of the arrow
			}
		}
		dot 	(arrow_differential, grid.arrow_dual_normals,   arrow_projection);      // flux across dual of the arrow 
		mult 	(arrow_projection,   grid.arrow_dual_lengths,   arrow_projection);
		mult 	(arrow_projection,   grid.layer_height,         arrow_projection);      // flow across dual of the arrow 

		fill    (layer_differential, T(0));
		for (unsigned int i = 0; i < grid.vertex_count; ++i)
		{
			for (unsigned int j = 1; j < L; ++j)
			{
				layer_differential[i*L+j-1] += vector_field[i*L+j] - vector_field[i*L+j-1]; // /2; NOTE: 2 cancels out
				layer_differential[i*L+j]   += vector_field[i*L+j] - vector_field[i*L+j-1]; // /2; NOTE: 2 cancels out
			}
		}
		dot     (layer_differential, grid.vertex_normals,       layer_projection);      // flux across layer boundary 
		mult    (layer_projection,   grid.vertex_dual_areas,    layer_projection);      // flow across layer boundary

		copy 	(out,                layer_projection);
		for (unsigned int i = 0; i < grid.arrow_vertex_id_from.size(); ++i)
		{
			for (unsigned int j = 0; j < L; ++j)
			{
				out[grid.arrow_vertex_id_from[i]*L+j] += arrow_projection[i*L+j];
			}
		}

		div 	(out,                grid.vertex_dual_areas,    out);
		div 	(out,                grid.layer_height,         out);             // divergence
	}

	template<typename T, glm::qualifier Q>
	many::tmany<T> divergence(const LayeredSpheroidGrid& grid, const many::tmany<glm::vec<3,T,Q>>& vector_field)
	{
		many::tmany<glm::vec<3,T,Q>> arrow_differential (grid.arrow_count  * grid.layer_count);
		many::tmany<glm::vec<3,T,Q>> layer_differential (grid.vertex_count * grid.layer_count);
		many::tmany<T>               arrow_projection   (grid.arrow_count  * grid.layer_count);
		many::tmany<T>               layer_projection   (grid.vertex_count * grid.layer_count);
		many::tmany<T>               out                (grid.vertex_count * grid.layer_count);
		divergence(grid, vector_field, out, arrow_differential, layer_differential, arrow_projection, layer_projection);
		return out;
	}


	template<typename T, glm::qualifier Q>
	void curl(
		const LayeredSpheroidGrid& grid, 
		const many::tmany<glm::vec<3,T,Q>>& vector_field, 
		many::tmany<glm::vec<3,T,Q>>& out, 
		many::tmany<glm::vec<3,T,Q>>& arrow_differential, 
		many::tmany<glm::vec<3,T,Q>>& layer_differential, 
		many::tmany<glm::vec<3,T,Q>>& arrow_rejection,
		many::tmany<glm::vec<3,T,Q>>& layer_rejection
	) {
		assert(vector_field.size()       == grid.vertex_count * grid.layer_count );
		assert(out.size()                == grid.vertex_count * grid.layer_count );
		assert(arrow_differential.size() == grid.arrow_count  * grid.layer_count );
		assert(arrow_rejection.size()    == grid.arrow_count  * grid.layer_count );
		assert(layer_differential.size() == grid.vertex_count * grid.layer_count );
		assert(layer_rejection.size()    == grid.vertex_count * grid.layer_count );
		uint L = grid.layer_count;
		uvec2 arrow;
		for (unsigned int i = 0; i < grid.arrow_vertex_ids.size(); ++i)
		{
			arrow = grid.arrow_vertex_ids[i]; 
			for (unsigned int j = 0; j < L; ++j)
			{
				arrow_differential[i*L+j] = vector_field[arrow.y*L+j] - vector_field[arrow.x*L+j]; // differential across dual of the arrow
			}
		}
		cross 	(arrow_differential,grid.arrow_dual_normals,   arrow_rejection);      // flux across dual of the arrow 
		mult 	(arrow_rejection,   grid.arrow_dual_lengths,   arrow_rejection);
		mult 	(arrow_rejection,   grid.layer_height,         arrow_rejection);      // flow across dual of the arrow 

		fill    (layer_differential, T(0));
		for (unsigned int i = 0; i < grid.vertex_count; ++i)
		{
			for (unsigned int j = 1; j < L; ++j)
			{
				layer_differential[i*L+j-1] += vector_field[i*L+j] - vector_field[i*L+j-1]; // /2; NOTE: 2 cancels out
				layer_differential[i*L+j]   += vector_field[i*L+j] - vector_field[i*L+j-1]; // /2; NOTE: 2 cancels out
			}
		}
		cross 	(layer_differential,grid.vertex_normals,       layer_rejection);      // flux across layer boundary 
		mult    (layer_rejection,   grid.vertex_dual_areas,    layer_rejection);      // flow across layer boundary

		copy 	(out,                layer_rejection);
		for (unsigned int i = 0; i < grid.arrow_vertex_id_from.size(); ++i)
		{
			for (unsigned int j = 0; j < L; ++j)
			{
				out[grid.arrow_vertex_id_from[i]*L+j] += arrow_rejection[i*L+j];
			}
		}

		div 	(out,                grid.vertex_dual_areas,    out);
		div 	(out,                grid.layer_height,         out);             // curl
	}

	template<typename T, glm::qualifier Q>
	many::tmany<glm::vec<3,T,Q>> curl(const LayeredSpheroidGrid& grid, const many::tmany<glm::vec<3,T,Q>>& vector_field)
	{
		many::tmany<glm::vec<3,T,Q>> arrow_differential (grid.arrow_count  * grid.layer_count);
		many::tmany<glm::vec<3,T,Q>> layer_differential (grid.vertex_count * grid.layer_count);
		many::tmany<glm::vec<3,T,Q>> arrow_projection   (grid.arrow_count  * grid.layer_count);
		many::tmany<glm::vec<3,T,Q>> layer_projection   (grid.vertex_count * grid.layer_count);
		many::tmany<glm::vec<3,T,Q>> out                (grid.vertex_count * grid.layer_count);
		curl(grid, vector_field, out, arrow_differential, layer_differential, arrow_projection, layer_projection);
		return out;
	}

	template<typename T>
	void laplacian(
		const LayeredSpheroidGrid& grid, 
		const many::tmany<T>& vector_field, 
		many::tmany<T>& out, 
		many::tmany<T>& arrow_differential, 
		many::tmany<T>& layer_differential, 
		many::tmany<T>& weighted_arrow_differential,
		many::tmany<T>& weighted_layer_differential
	) {
		assert(vector_field.size()                == grid.vertex_count * grid.layer_count );
		assert(out.size()                         == grid.vertex_count * grid.layer_count );
		assert(arrow_differential.size()          == grid.arrow_count  * grid.layer_count );
		assert(weighted_arrow_differential.size() == grid.arrow_count  * grid.layer_count );
		assert(layer_differential.size()          == grid.vertex_count * grid.layer_count );
		assert(weighted_layer_differential.size() == grid.vertex_count * grid.layer_count );
		uint L = grid.layer_count;
		uvec2 arrow;
		for (unsigned int i = 0; i < grid.arrow_vertex_ids.size(); ++i)
		{
			arrow = grid.arrow_vertex_ids[i]; 
			for (unsigned int j = 0; j < L; ++j)
			{
				arrow_differential[i*L+j] = vector_field[arrow.y*L+j] - vector_field[arrow.x*L+j]; // differential across dual of the arrow
			}
		}
		mult 	(weighted_arrow_differential, grid.arrow_dual_lengths,   weighted_arrow_differential);
		mult 	(weighted_arrow_differential, grid.layer_height,         weighted_arrow_differential);      // flow across dual of the arrow 

		fill    (layer_differential, T(0));
		for (unsigned int i = 0; i < grid.vertex_count; ++i)
		{
			for (unsigned int j = 1; j < L; ++j)
			{
				layer_differential[i*L+j-1] += vector_field[i*L+j] - vector_field[i*L+j-1]; // /2; NOTE: 2 cancels out
				layer_differential[i*L+j]   += vector_field[i*L+j] - vector_field[i*L+j-1]; // /2; NOTE: 2 cancels out
			}
		}
		mult    (weighted_layer_differential,   grid.vertex_dual_areas,    weighted_layer_differential);      // flow across layer boundary

		copy 	(out,                weighted_layer_differential);
		for (unsigned int i = 0; i < grid.arrow_vertex_id_from.size(); ++i)
		{
			for (unsigned int j = 0; j < L; ++j)
			{
				out[grid.arrow_vertex_id_from[i]*L+j] += weighted_arrow_differential[i*L+j];
			}
		}

		div 	(out,                grid.vertex_dual_areas,    out);
		div 	(out,                grid.layer_height,         out);             // laplacian
	}
	template<typename T>
	many::tmany<T> laplacian(const LayeredSpheroidGrid& grid, const many::tmany<T>& vector_field)
	{
		many::tmany<T> arrow_differential (grid.arrow_count  * grid.layer_count);
		many::tmany<T> layer_differential (grid.vertex_count * grid.layer_count);
		many::tmany<T> arrow_projection   (grid.arrow_count  * grid.layer_count);
		many::tmany<T> layer_projection   (grid.vertex_count * grid.layer_count);
		many::tmany<T> out                (grid.vertex_count * grid.layer_count);
		laplacian(grid, vector_field, out, arrow_differential, layer_differential, arrow_projection, layer_projection);
		return out;
	}

	template<unsigned int L, typename T, glm::qualifier Q>
	void laplacian(
		const LayeredSpheroidGrid& grid, 
		const many::tmany<glm::vec<L,T,Q>>& vector_field, 
		many::tmany<glm::vec<L,T,Q>>& out, 
		many::tmany<glm::vec<L,T,Q>>& arrow_differential, 
		many::tmany<glm::vec<L,T,Q>>& layer_differential, 
		many::tmany<glm::vec<L,T,Q>>& weighted_arrow_differential,
		many::tmany<glm::vec<L,T,Q>>& weighted_layer_differential
	) {
		assert(vector_field.size()                == grid.vertex_count * grid.layer_count );
		assert(out.size()                         == grid.vertex_count * grid.layer_count );
		assert(arrow_differential.size()          == grid.arrow_count  * grid.layer_count );
		assert(weighted_arrow_differential.size() == grid.arrow_count  * grid.layer_count );
		assert(layer_differential.size()          == grid.vertex_count * grid.layer_count );
		assert(weighted_layer_differential.size() == grid.vertex_count * grid.layer_count );
		uint Li = grid.layer_count;
		uvec2 arrow;
		for (unsigned int i = 0; i < grid.arrow_vertex_ids.size(); ++i)
		{
			arrow = grid.arrow_vertex_ids[i]; 
			for (unsigned int j = 0; j < Li; ++j)
			{
				arrow_differential[i*Li+j] = vector_field[arrow.y*Li+j] - vector_field[arrow.x*Li+j]; // differential across dual of the arrow
			}
		}
		mult 	(weighted_arrow_differential, grid.arrow_dual_lengths,   weighted_arrow_differential);
		mult 	(weighted_arrow_differential, grid.layer_height,         weighted_arrow_differential);      // flow across dual of the arrow 

		fill    (layer_differential, glm::vec<L,T,Q>(0.f));
		for (unsigned int i = 0; i < grid.vertex_count; ++i)
		{
			for (unsigned int j = 1; j < Li; ++j)
			{
				layer_differential[i*Li+j-1] += vector_field[i*Li+j] - vector_field[i*Li+j-1]; // /2; NOTE: 2 cancels out
				layer_differential[i*Li+j]   += vector_field[i*Li+j] - vector_field[i*Li+j-1]; // /2; NOTE: 2 cancels out
			}
		}
		mult    (weighted_layer_differential,   grid.vertex_dual_areas,    weighted_layer_differential);      // flow across layer boundary

		copy 	(out,                weighted_layer_differential);
		for (unsigned int i = 0; i < grid.arrow_vertex_id_from.size(); ++i)
		{
			for (unsigned int j = 0; j < Li; ++j)
			{
				out[grid.arrow_vertex_id_from[i]*Li+j] += weighted_arrow_differential[i*Li+j];
			}
		}

		div 	(out,                grid.vertex_dual_areas,    out);
		div 	(out,                grid.layer_height,         out);             // laplacian
	}
	template<unsigned int L, typename T, glm::qualifier Q>
	many::tmany<glm::vec<L,T,Q>> laplacian(const LayeredSpheroidGrid& grid, const many::tmany<glm::vec<L,T,Q>>& vector_field)
	{
		many::tmany<glm::vec<L,T,Q>> arrow_differential (grid.arrow_count  * grid.layer_count);
		many::tmany<glm::vec<L,T,Q>> layer_differential (grid.vertex_count * grid.layer_count);
		many::tmany<glm::vec<L,T,Q>> arrow_projection   (grid.arrow_count  * grid.layer_count);
		many::tmany<glm::vec<L,T,Q>> layer_projection   (grid.vertex_count * grid.layer_count);
		many::tmany<glm::vec<L,T,Q>> out                (grid.vertex_count * grid.layer_count);
		laplacian(grid, vector_field, out, arrow_differential, layer_differential, arrow_projection, layer_projection);
		return out;
	}
}