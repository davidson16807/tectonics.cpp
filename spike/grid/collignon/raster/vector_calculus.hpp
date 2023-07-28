#pragma once

#include "../Grid.hpp"

namespace collignon 
{

    /*
    properties used:
        arrow_dual_lengths
        arrow_dual_normals
        arrow_lengths
        vertex_dual_areas
        arrow_vertex_ids
		arrow_target_vertex_id
        arrow_source_vertex_id
    */

    /*
    NOTE: SEE VECTOR CALCULUS MD FOR MORE EXPLANATION!
    */

    template<typename id, typename scalar, typename Tin, typename Tout>
    void gradient(const Grid<id, scalar>& grid, const Tin& field, Tout& out) {
    	// assert(compatible(field, out));
    	// assert(grid.compatible(field));
    	id i, j;
    	const id N = grid.arrows_per_vertex;
        typename Tin::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Tout::value_type(0);
        	source_value = field[i];
        	for (j = 0; j < N; ++j)
        	{
        		out[i] += (field[grid.arrow_target_memory_id(i*N,j)] - source_value) 
                        * grid.arrow_dual_length(i,j) * grid.arrow_normal(i,j);
        	}
        	out[i] /= grid.vertex_dual_area(i);
        }
    }


    template<typename id, typename scalar, typename Tin, typename Tout>
    void divergence(const Grid<id,scalar>& grid, const Tin& field, Tout& out) {
    	// assert(compatible(field, out));
    	// assert(grid.compatible(field));
    	id i, j;
    	const id N = grid.arrows_per_vertex;
        typename Tin::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Tout::value_type(0);
        	source_value = field[i];
        	for (j = 0; j < N; ++j)
        	{
        		out[i] += glm::dot((field[grid.arrow_target_memory_id(i*N,j)] - source_value),
        					  grid.arrow_dual_length(i,j) * grid.arrow_normal(i,j));
        	}
        	out[i] /= grid.vertex_dual_area(i);
        }
    }


    template<typename id, typename scalar, typename Tin, typename Tout>
    void curl(const Grid<id,scalar>& grid, const Tin& field, Tout& out) {
    	// assert(compatible(field, out));
    	// assert(grid.compatible(field));
    	id i, j;
    	const id N = grid.arrows_per_vertex;
        typename Tin::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Tout::value_type(0);
        	source_value = field[i];
        	for (j = 0; j < N; ++j)
        	{
        		out[i] += glm::cross((field[grid.arrow_target_memory_id(i*N,j)] - source_value),
        					  grid.arrow_dual_length(i,j) * grid.arrow_normal(i,j));
        	}
        	out[i] /= grid.vertex_dual_area(i);
        }
    }


    template<typename id, typename scalar, typename Tin, typename Tout>
    void laplacian(const Grid<id,scalar>& grid, const Tin& field, Tout& out) {
    	// assert(compatible(field, out));
    	// assert(grid.compatible(field));
    	id i, j;
    	const id N = grid.arrows_per_vertex;
        typename Tin::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Tout::value_type(0);
        	source_value = field[i];
        	for (j = 0; j < N; ++j)
        	{
        		out[i] += (field[grid.arrow_target_memory_id(i*N,j)] - source_value)
        				*  grid.arrow_dual_length(i,j) / grid.arrow_length(i,j);
        	}
        	out[i] /= grid.vertex_dual_area(i);
        }
    }

}

