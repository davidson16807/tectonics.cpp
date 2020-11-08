#pragma once

#include "LayeredRaster.hpp"

namespace rasters
{
	template<typename Tgrid, typename T, glm::qualifier Q>
	void gradient(
		const LayeredRaster<T,Tgrid>& scalar_field, 
		LayeredRaster<glm::vec<3,T,Q>,Tgrid>& out, 
		LayeredRaster<T,Tgrid,mapping::arrow>& arrow_differential, 
		LayeredRaster<T,Tgrid>& layer_differential, 
		LayeredRaster<glm::vec<3,T,Q>,Tgrid,mapping::arrow>& arrow_flow,
		LayeredRaster<glm::vec<3,T,Q>,Tgrid>& layer_flow
	) {
		many::fill (arrow_differential, T(0));
		uint L = scalar_field.grid.layering->layer_count;
		glm::uvec2 arrow;
		for (unsigned int i = 0; i < scalar_field.grid.cache->arrow_vertex_ids.size(); ++i)
		{
			arrow = scalar_field.grid.cache->arrow_vertex_ids[i]; 
			for (unsigned int j = 0; j < L; ++j)
			{
				arrow_differential[i*L+j] = scalar_field[arrow.y*L+j] - scalar_field[arrow.x*L+j]; // differential across dual of the arrow
			}
		}
		many::mult (arrow_differential, scalar_field.grid.cache->arrow_dual_normals,   arrow_flow);      // flux across dual of the arrow 
		many::mult (arrow_flow,         scalar_field.grid.cache->arrow_dual_lengths,   arrow_flow);
		many::mult (arrow_flow,         scalar_field.grid.layering->layer_height,      arrow_flow);      // flow across dual of the arrow 

		many::fill (layer_differential, T(0));
		for (unsigned int i = 0; i < scalar_field.grid.cache->vertex_count; ++i)
		{
			for (unsigned int j = 1; j < L; ++j)
			{
				layer_differential[i*L+j-1] += scalar_field[i*L+j] - scalar_field[i*L+j-1]; // /2; NOTE: 2 cancels out
				layer_differential[i*L+j]   += scalar_field[i*L+j] - scalar_field[i*L+j-1]; // /2; NOTE: 2 cancels out
			}
		}
		many::mult (layer_differential, scalar_field.grid.cache->vertex_normals,       layer_flow);      // flux across layer boundary 
		many::mult (layer_flow,         scalar_field.grid.cache->vertex_dual_areas,    layer_flow);      // flow across layer boundary

		many::copy (out,                layer_flow);
		for (unsigned int i = 0; i < scalar_field.grid.cache->arrow_vertex_id_from.size(); ++i)
		{
			for (unsigned int j = 0; j < L; ++j)
			{
				out[scalar_field.grid.cache->arrow_vertex_id_from[i]*L+j] += arrow_flow[i*L+j];
			}
		}

		many::div  (out,                scalar_field.grid.cache->vertex_dual_areas,    out);
		many::div  (out,                scalar_field.grid.layering->layer_height,      out);             // gradient
	}

	template<typename Tgrid, typename T>
	LayeredRaster<glm::vec3,Tgrid> gradient(const LayeredRaster<T,Tgrid>& scalar_field)
	{
		LayeredRaster<T,Tgrid,mapping::arrow>         arrow_differential (scalar_field.grid);
		LayeredRaster<T,Tgrid>                        layer_differential (scalar_field.grid);
		LayeredRaster<glm::vec3,Tgrid,mapping::arrow> arrow_flow         (scalar_field.grid);
		LayeredRaster<glm::vec3,Tgrid>                layer_flow         (scalar_field.grid);
		LayeredRaster<glm::vec3,Tgrid>                out                (scalar_field.grid);
		gradient(scalar_field, out, arrow_differential, layer_differential, arrow_flow, layer_flow);
		return out;
	}


	template<typename Tgrid, typename T, glm::qualifier Q>
	void divergence(
		const LayeredRaster<glm::vec<3,T,Q>,Tgrid>& vector_field, 
		LayeredRaster<T,Tgrid>& out, 
		LayeredRaster<glm::vec<3,T,Q>,Tgrid,mapping::arrow>& arrow_differential, 
		LayeredRaster<glm::vec<3,T,Q>,Tgrid>& layer_differential, 
		LayeredRaster<T,Tgrid,mapping::arrow>& arrow_projection,
		LayeredRaster<T,Tgrid>& layer_projection
	) {
		many::fill (arrow_differential, glm::vec<3,T,Q>(0.f));
		uint L = vector_field.grid.layering->layer_count;
		glm::uvec2 arrow;
		for (unsigned int i = 0; i < vector_field.grid.cache->arrow_vertex_ids.size(); ++i)
		{
			arrow = vector_field.grid.cache->arrow_vertex_ids[i]; 
			for (unsigned int j = 0; j < L; ++j)
			{
				arrow_differential[i*L+j] = vector_field[arrow.y*L+j] - vector_field[arrow.x*L+j]; // differential across dual of the arrow
			}
		}
		many::dot  (arrow_differential, vector_field.grid.cache->arrow_dual_normals,   arrow_projection);      // flux across dual of the arrow 
		many::mult (arrow_projection,   vector_field.grid.cache->arrow_dual_lengths,   arrow_projection);
		many::mult (arrow_projection,   vector_field.grid.layering->layer_height,      arrow_projection);      // flow across dual of the arrow 

		many::fill (layer_differential, glm::vec<3,T,Q>(0.f));
		for (unsigned int i = 0; i < vector_field.grid.cache->vertex_count; ++i)
		{
			for (unsigned int j = 1; j < L; ++j)
			{
				layer_differential[i*L+j-1] += vector_field[i*L+j] - vector_field[i*L+j-1]; // /2; NOTE: 2 cancels out
				layer_differential[i*L+j]   += vector_field[i*L+j] - vector_field[i*L+j-1]; // /2; NOTE: 2 cancels out
			}
		}
		many::dot  (layer_differential, vector_field.grid.cache->vertex_normals,       layer_projection);      // flux across layer boundary 
		many::mult (layer_projection,   vector_field.grid.cache->vertex_dual_areas,    layer_projection);      // flow across layer boundary

		many::copy (out,                layer_projection);
		for (unsigned int i = 0; i < vector_field.grid.cache->arrow_vertex_id_from.size(); ++i)
		{
			for (unsigned int j = 0; j < L; ++j)
			{
				out[vector_field.grid.cache->arrow_vertex_id_from[i]*L+j] += arrow_projection[i*L+j];
			}
		}

		many::div  (out,                vector_field.grid.cache->vertex_dual_areas,    out);
		many::div  (out,                vector_field.grid.layering->layer_height,      out);             // divergence
	}

	template<typename Tgrid, typename T, glm::qualifier Q>
	LayeredRaster<T,Tgrid> divergence(const LayeredRaster<glm::vec<3,T,Q>,Tgrid>& vector_field)
	{
		LayeredRaster<glm::vec<3,T,Q>,Tgrid, mapping::arrow> arrow_differential (vector_field.grid);
		LayeredRaster<glm::vec<3,T,Q>,Tgrid> layer_differential (vector_field.grid);
		LayeredRaster<T,Tgrid, mapping::arrow>               arrow_projection   (vector_field.grid);
		LayeredRaster<T,Tgrid>               layer_projection   (vector_field.grid);
		LayeredRaster<T,Tgrid>               out                (vector_field.grid);
		divergence(vector_field, out, arrow_differential, layer_differential, arrow_projection, layer_projection);
		return out;
	}


	template<typename Tgrid, typename T, glm::qualifier Q>
	void curl(
		const LayeredRaster<glm::vec<3,T,Q>,Tgrid>& vector_field, 
		LayeredRaster<glm::vec<3,T,Q>,Tgrid>& out, 
		LayeredRaster<glm::vec<3,T,Q>,Tgrid,mapping::arrow>& arrow_differential, 
		LayeredRaster<glm::vec<3,T,Q>,Tgrid>& layer_differential, 
		LayeredRaster<glm::vec<3,T,Q>,Tgrid,mapping::arrow>& arrow_rejection,
		LayeredRaster<glm::vec<3,T,Q>,Tgrid>& layer_rejection
	) {
		many::fill (arrow_differential, glm::vec<3,T,Q>(0.f));
		uint L = vector_field.grid.layering->layer_count;
		glm::uvec2 arrow;
		for (unsigned int i = 0; i < vector_field.grid.cache->arrow_vertex_ids.size(); ++i)
		{
			arrow = vector_field.grid.cache->arrow_vertex_ids[i]; 
			for (unsigned int j = 0; j < L; ++j)
			{
				arrow_differential[i*L+j] = vector_field[arrow.y*L+j] - vector_field[arrow.x*L+j]; // differential across dual of the arrow
			}
		}
		many::cross(arrow_differential,vector_field.grid.cache->arrow_dual_normals,   arrow_rejection);      // flux across dual of the arrow 
		many::mult (arrow_rejection,   vector_field.grid.cache->arrow_dual_lengths,   arrow_rejection);
		many::mult (arrow_rejection,   vector_field.grid.layering->layer_height,      arrow_rejection);      // flow across dual of the arrow 

		many::fill (layer_differential, glm::vec<3,T,Q>(0.f));
		for (unsigned int i = 0; i < vector_field.grid.cache->vertex_count; ++i)
		{
			for (unsigned int j = 1; j < L; ++j)
			{
				layer_differential[i*L+j-1] += vector_field[i*L+j] - vector_field[i*L+j-1]; // /2; NOTE: 2 cancels out
				layer_differential[i*L+j]   += vector_field[i*L+j] - vector_field[i*L+j-1]; // /2; NOTE: 2 cancels out
			}
		}
		many::cross(layer_differential,vector_field.grid.cache->vertex_normals,       layer_rejection);      // flux across layer boundary 
		many::mult (layer_rejection,   vector_field.grid.cache->vertex_dual_areas,    layer_rejection);      // flow across layer boundary

		many::copy (out,                layer_rejection);
		for (unsigned int i = 0; i < vector_field.grid.cache->arrow_vertex_id_from.size(); ++i)
		{
			for (unsigned int j = 0; j < L; ++j)
			{
				out[vector_field.grid.cache->arrow_vertex_id_from[i]*L+j] += arrow_rejection[i*L+j];
			}
		}

		many::div  (out,                vector_field.grid.cache->vertex_dual_areas,    out);
		many::div  (out,                vector_field.grid.layering->layer_height,      out);             // curl
	}

	template<typename Tgrid, typename T, glm::qualifier Q>
	LayeredRaster<glm::vec<3,T,Q>,Tgrid> curl(const LayeredRaster<glm::vec<3,T,Q>,Tgrid>& vector_field)
	{
		LayeredRaster<glm::vec<3,T,Q>,Tgrid, mapping::arrow> arrow_differential (vector_field.grid);
		LayeredRaster<glm::vec<3,T,Q>,Tgrid> layer_differential (vector_field.grid);
		LayeredRaster<glm::vec<3,T,Q>,Tgrid, mapping::arrow> arrow_projection   (vector_field.grid);
		LayeredRaster<glm::vec<3,T,Q>,Tgrid> layer_projection   (vector_field.grid);
		LayeredRaster<glm::vec<3,T,Q>,Tgrid> out                (vector_field.grid);
		curl(vector_field, out, arrow_differential, layer_differential, arrow_projection, layer_projection);
		return out;
	}

	// ∇²ϕ = 1/V Σᵢ Δϕᵢ/|Δxᵢ| ΔSᵢ
	template<typename Tgrid, typename T>
	void laplacian(
		const LayeredRaster<T,Tgrid>& scalar_field, 
		LayeredRaster<T,Tgrid>& out, 
		LayeredRaster<T,Tgrid,mapping::arrow>& arrow_scratch, 
		LayeredRaster<T,Tgrid>& layer_scratch
	) {
		many::fill (arrow_scratch, T(0));
		uint L = scalar_field.grid.layering->layer_count;
		glm::uvec2 arrow;
		for (unsigned int i = 0; i < scalar_field.grid.cache->arrow_vertex_ids.size(); ++i)
		{
			arrow = scalar_field.grid.cache->arrow_vertex_ids[i]; 
			for (unsigned int j = 0; j < L; ++j)
			{
				arrow_scratch[i*L+j] = scalar_field[arrow.y*L+j] - scalar_field[arrow.x*L+j]; // differential across dual of the arrow
			}
		}
		many::div  (arrow_scratch, scalar_field.grid.cache->arrow_lengths,        arrow_scratch); // slope across arrow
		many::mult (arrow_scratch, scalar_field.grid.cache->arrow_dual_lengths,   arrow_scratch);
		many::mult (arrow_scratch, scalar_field.grid.layering->layer_height,      arrow_scratch); // flow across dual of the arrow 

		many::fill (layer_scratch, T(0));
		for (unsigned int i = 0; i < scalar_field.grid.cache->vertex_count; ++i)
		{
			for (unsigned int j = 1; j < L; ++j)
			{
				layer_scratch[i*L+j-1] += scalar_field[i*L+j] - scalar_field[i*L+j-1]; // /2; NOTE: 2 cancels out
				layer_scratch[i*L+j]   += scalar_field[i*L+j] - scalar_field[i*L+j-1]; // /2; NOTE: 2 cancels out
			}
		}
		many::div  (layer_scratch, scalar_field.grid.layering->layer_height,         layer_scratch); // slope across layers
		many::mult (layer_scratch, scalar_field.grid.cache->vertex_dual_areas,    layer_scratch); // flow across layer boundary

		many::copy (out,                layer_scratch);
		for (unsigned int i = 0; i < scalar_field.grid.cache->arrow_vertex_id_from.size(); ++i)
		{
			for (unsigned int j = 0; j < L; ++j)
			{
				out[scalar_field.grid.cache->arrow_vertex_id_from[i]*L+j] += arrow_scratch[i*L+j];
			}
		}

		many::div  (out,                scalar_field.grid.cache->vertex_dual_areas,    out);
		many::div  (out,                scalar_field.grid.layering->layer_height,      out);             // laplacian
	}
	template<typename Tgrid, typename T>
	LayeredRaster<T,Tgrid> laplacian(const LayeredRaster<T,Tgrid>& scalar_field)
	{
		LayeredRaster<T,Tgrid, mapping::arrow> arrow_scratch    (scalar_field.grid);
		LayeredRaster<T,Tgrid> layer_scratch    (scalar_field.grid);
		LayeredRaster<T,Tgrid> out              (scalar_field.grid);
		laplacian(scalar_field, out, arrow_scratch, layer_scratch);
		return out;
	}

	template<typename Tgrid, unsigned int L, typename T, glm::qualifier Q>
	void laplacian(
		
		const LayeredRaster<glm::vec<L,T,Q>,Tgrid>& vector_field, 
		LayeredRaster<glm::vec<L,T,Q>,Tgrid>& out, 
		LayeredRaster<glm::vec<L,T,Q>,Tgrid, mapping::arrow>& arrow_scratch,
		LayeredRaster<glm::vec<L,T,Q>,Tgrid>& layer_scratch
	) {
		many::fill (arrow_scratch, glm::vec<L,T,Q>(0.f));
		uint Li = vector_field.grid.layering->layer_count;
		glm::uvec2 arrow;
		for (unsigned int i = 0; i < vector_field.grid.cache->arrow_vertex_ids.size(); ++i)
		{
			arrow = vector_field.grid.cache->arrow_vertex_ids[i]; 
			for (unsigned int j = 0; j < Li; ++j)
			{
				arrow_scratch[i*Li+j] = vector_field[arrow.y*Li+j] - vector_field[arrow.x*Li+j]; // differential across dual of the arrow
			}
		}
		many::div  (arrow_scratch, vector_field.grid.cache->arrow_lengths, arrow_scratch);
		many::mult (arrow_scratch, vector_field.grid.cache->arrow_dual_lengths,   arrow_scratch);
		many::mult (arrow_scratch, vector_field.grid.layering->layer_height,      arrow_scratch); // flow across dual of the arrow 

		many::fill (layer_scratch, glm::vec<L,T,Q>(0.f));
		for (unsigned int i = 0; i < vector_field.grid.cache->vertex_count; ++i)
		{
			for (unsigned int j = 1; j < Li; ++j)
			{
				layer_scratch[i*Li+j-1] += vector_field[i*Li+j] - vector_field[i*Li+j-1]; // /2; NOTE: 2 cancels out
				layer_scratch[i*Li+j]   += vector_field[i*Li+j] - vector_field[i*Li+j-1]; // /2; NOTE: 2 cancels out
			}
		}
		many::div  (layer_scratch, vector_field.grid.layering->layer_height,      layer_scratch); // slope across layers
		many::mult (layer_scratch, vector_field.grid.cache->vertex_dual_areas,    layer_scratch); // flow across layer boundary

		many::copy (out,                layer_scratch);
		for (unsigned int i = 0; i < vector_field.grid.cache->arrow_vertex_id_from.size(); ++i)
		{
			for (unsigned int j = 0; j < Li; ++j)
			{
				out[vector_field.grid.cache->arrow_vertex_id_from[i]*Li+j] += arrow_scratch[i*Li+j];
			}
		}

		many::div  (out,                vector_field.grid.cache->vertex_dual_areas,    out);
		many::div  (out,                vector_field.grid.layering->layer_height,      out);             // laplacian
	}
	template<typename Tgrid, unsigned int L, typename T, glm::qualifier Q>
	LayeredRaster<glm::vec<L,T,Q>,Tgrid> laplacian(const LayeredRaster<glm::vec<L,T,Q>,Tgrid>& vector_field)
	{
		LayeredRaster<glm::vec<L,T,Q>,Tgrid, mapping::arrow> arrow_scratch (vector_field.grid);
		LayeredRaster<glm::vec<L,T,Q>,Tgrid> layer_scratch (vector_field.grid);
		LayeredRaster<glm::vec<L,T,Q>,Tgrid> out           (vector_field.grid);
		laplacian(vector_field, out, arrow_scratch, layer_scratch);
		return out;
	}
}