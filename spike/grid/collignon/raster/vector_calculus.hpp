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

    template<typename id, typename scalar, typename In, typename Out>
    void gradient(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        const id N = grid.arrows_per_vertex;
        typename In::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            source_value = field[i];
            for (j = 0; j < N; ++j)
            {
                out[i] += (field[grid.arrow_target_memory_id(i,j)] - source_value) 
                        * grid.arrow_dual_length(i,j) * grid.arrow_normal(i,j);
            }
            out[i] /= grid.vertex_dual_area(i);
        }
    }

    template<typename id, typename scalar, typename In, typename Out>
    void arrow_dual_length0(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 0;
            out[i] += grid.arrow_dual_length(i,j);
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void arrow_dual_length1(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 1;
            out[i] += grid.arrow_dual_length(i,j);
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void arrow_length0(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 0;
            out[i] += grid.arrow_length(i,j);
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void arrow_length1(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 0;
            out[i] += grid.arrow_length(i,j);
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void arrow_normal(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 0;
            out[i] += grid.arrow_normal(i,j);
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void vertex_dual_area(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            out[i] += grid.vertex_dual_area(i);
        }
    }


    template<typename id, typename scalar, typename In, typename Out>
    void divergence(const Grid<id,scalar>& grid, const In& field, Out& out) {
    	// assert(grid.compatible(field));
    	id i, j;
    	const id N = grid.arrows_per_vertex;
        typename In::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
        	source_value = field[i];
        	for (j = 0; j < N; ++j)
        	{
        		out[i] += glm::dot((field[grid.arrow_target_memory_id(i,j)] - source_value),
        					  grid.arrow_dual_length(i,j) * grid.arrow_normal(i,j));
        	}
        	out[i] /= grid.vertex_dual_area(i);
        }
    }


    template<typename id, typename scalar, typename In, typename Out>
    void curl(const Grid<id,scalar>& grid, const In& field, Out& out) {
    	// assert(compatible(field, out));
    	// assert(grid.compatible(field));
    	id i, j;
    	const id N = grid.arrows_per_vertex;
        typename In::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
        	source_value = field[i];
        	for (j = 0; j < N; ++j)
        	{
        		out[i] += glm::cross((field[grid.arrow_target_memory_id(i,j)] - source_value),
        					  grid.arrow_dual_length(i,j) * grid.arrow_normal(i,j));
        	}
        	out[i] /= grid.vertex_dual_area(i);
        }
    }


    template<typename id, typename scalar, typename In, typename Out>
    void laplacian(const Grid<id,scalar>& grid, const In& field, Out& out) {
    	// assert(compatible(field, out));
    	// assert(grid.compatible(field));
    	id i, j;
    	const id N = grid.arrows_per_vertex;
        typename In::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
        	source_value = field[i];
        	for (j = 0; j < N; ++j)
        	{
        		out[i] += (field[grid.arrow_target_memory_id(i,j)] - source_value)
        				*  grid.arrow_dual_length(i,j) / grid.arrow_length(i,j);
        	}
        	out[i] /= grid.vertex_dual_area(i);
        }
    }

}

