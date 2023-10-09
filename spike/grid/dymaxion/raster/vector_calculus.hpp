#pragma once

#include "../Grid.hpp"

namespace dymaxion 
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
        typename In::value_type target_value;
        scalar vertex_dual_area;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            vertex_dual_area = grid.vertex_dual_area(i);
            // std::cout << vertex_dual_area << std::endl;
            source_value = field[i];
            for (j = 0; j < N; ++j)
            {
                target_value = field[grid.arrow_target_memory_id(i,j)];
                out[i] += (target_value - source_value)
                        * grid.arrow_dual_length(i,j) * grid.arrow_normal(i,j);
            }
            out[i] /= vertex_dual_area;
        }
    }

    template<typename id, typename scalar, typename In, typename Out>
    void square_id(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = grid.square_id(i);
        }
    }

    template<typename id, typename scalar, typename In, typename Out>
    void is_arrow_irregular0(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = grid.is_arrow_irregular(i,0);
        }
    }

    template<typename id, typename scalar, typename In, typename Out>
    void is_arrow_irregular1(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = grid.is_arrow_irregular(i,1);
        }
    }

    template<typename id, typename scalar, typename In, typename Out>
    void is_arrow_irregular2(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = grid.is_arrow_irregular(i,2);
        }
    }

    template<typename id, typename scalar, typename In, typename Out>
    void is_arrow_irregular3(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = grid.is_arrow_irregular(i,3);
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
    void arrow_dual_length2(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 2;
            out[i] += grid.arrow_dual_length(i,j);
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void arrow_dual_length3(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 3;
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
    void arrow_length2(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 2;
            out[i] += grid.arrow_length(i,j);
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void arrow_length3(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 3;
            out[i] += grid.arrow_length(i,j);
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void arrow_normal0(const Grid<id, scalar>& grid, const In& field, Out& out) {
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
    void arrow_normal1(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 1;
            out[i] += grid.arrow_normal(i,j);
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void arrow_normal2(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 2;
            out[i] += grid.arrow_normal(i,j);
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void arrow_normal3(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            j = 3;
            out[i] += grid.arrow_normal(i,j);
        }
    }

    template<typename id, typename scalar, typename In, typename Out>
    void differential0(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            j = 0;
            out[i] = field[grid.arrow_target_memory_id(i,j)] - field[i];
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void differential1(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            j = 1;
            out[i] = field[grid.arrow_target_memory_id(i,j)] - field[i];
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void differential2(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            j = 2;
            out[i] = field[grid.arrow_target_memory_id(i,j)] - field[i];
        }
    }
    template<typename id, typename scalar, typename In, typename Out>
    void differential3(const Grid<id, scalar>& grid, const In& field, Out& out) {
        // assert(grid.compatible(field));
        id i, j;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            j = 3;
            out[i] = field[grid.arrow_target_memory_id(i,j)] - field[i];
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
        bool is_irregular;
        id i, j, offset_id;
        const id N = grid.arrows_per_vertex;
        scalar vertex_dual_area;
        typename In::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            vertex_dual_area = grid.vertex_dual_area(i);
            source_value = field[i];
            for (j = 0; j < N; ++j)
            {
                is_irregular = grid.is_arrow_irregular(i,j);
                offset_id = is_irregular? j+N/2 : j;
                out[i] += glm::dot(scalar(1-2*is_irregular) * (field[grid.arrow_target_memory_id(i,offset_id)] - source_value),
                              grid.arrow_dual_length(i,j) * grid.arrow_normal(i,j));
            }
            out[i] /= vertex_dual_area;
        }
    }


    template<typename id, typename scalar, typename In, typename Out>
    void curl(const Grid<id,scalar>& grid, const In& field, Out& out) {
        // assert(compatible(field, out));
        // assert(grid.compatible(field));
        bool is_irregular;
        id i, j, offset_id;
        const id N = grid.arrows_per_vertex;
        typename In::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            source_value = field[i];
            for (j = 0; j < N; ++j)
            {
                is_irregular = grid.is_arrow_irregular(i,j);
                offset_id = is_irregular? j+N/2 : j;
                out[i] += glm::cross(scalar(1-2*is_irregular) * (field[grid.arrow_target_memory_id(i,offset_id)] - source_value),
                              grid.arrow_dual_length(i,j) * grid.arrow_normal(i,j));
            }
            out[i] /= grid.vertex_dual_area(i);
        }
    }


    template<typename id, typename scalar, typename In, typename Out>
    void laplacian(const Grid<id,scalar>& grid, const In& field, Out& out) {
        bool is_irregular;
        id i, j, offset_id;
        const id N = grid.arrows_per_vertex;
        typename In::value_type source_value;
        for (i = 0; i < grid.vertex_count(); ++i)
        {
            out[i] = typename Out::value_type(0);
            source_value = field[i];
            for (j = 0; j < N; ++j)
            {
                is_irregular = grid.is_arrow_irregular(i,j);
                offset_id = is_irregular? j+N/2 : j;
                out[i] += scalar(1-2*is_irregular) * (field[grid.arrow_target_memory_id(i,offset_id)] - source_value)
                        *  grid.arrow_dual_length(i,j) / grid.arrow_length(i,j);
            }
            out[i] /= grid.vertex_dual_area(i);
        }
    }

}

