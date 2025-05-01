#pragma once

// #include <glm/vec3.hpp>
#include <glm/geometric.hpp> // *glm::cross

namespace unlayered
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

    struct VectorCalculusByFundamentalTheorem
    {

        /*
        NOTE: See VectorCalculusByFundamentalTheorem.md for more explanation!
        */

        template<typename Grid, typename In, typename Out>
        void gradient(const Grid& grid, const In& field, Out& out) const {
            using id2 = typename Grid::size_type;
            using id = typename Grid::dimension_type;
            id2 i, i2;
            id j;
            const id N = grid.arrows_per_vertex;
            typename In::value_type source_value;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);
                out[i] = typename Out::value_type(0);
                source_value = field[i2];
                for (j = 0; j < N; ++j)
                {
                    out[i] += (field[grid.arrow_target_id(i2,j)] - source_value)
                             * grid.arrow_dual_length(i2,j) * grid.arrow_normal(i2,j);
                }
                out[i] /= grid.vertex_dual_area(i2);
            }
        }

        template<typename Grid, typename In, typename Out>
        void divergence(const Grid& grid, const In& field, Out& out) const {
            using id2 = typename Grid::size_type;
            using id = typename Grid::dimension_type;
            // assert(grid.compatible(field));
            id2 i, i2;
            id j;
            const id N = grid.arrows_per_vertex;
            typename In::value_type source_value;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);
                out[i] = typename Out::value_type(0);
                source_value = field[i2];
                for (j = 0; j < N; ++j)
                {
                    out[i] += glm::dot(field[grid.arrow_target_id(i2,j)] - source_value,
                                       grid.arrow_normal(i2,j)) * grid.arrow_dual_length(i2,j);
                }
                out[i] /= grid.vertex_dual_area(i2);
            }
        }

        template<typename Grid, typename In, typename Out>
        void curl(const Grid& grid, const In& field, Out& out) const {
            using id2 = typename Grid::size_type;
            using id = typename Grid::dimension_type;
            // assert(compatible(field, out));
            // assert(grid.compatible(field));
            id2 i, i2;
            id j;
            const id N = grid.arrows_per_vertex;
            typename In::value_type source_value;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i2);
                out[i] = typename Out::value_type(0);
                source_value = field[i2];
                for (j = 0; j < N; ++j)
                {
                    out[i] += glm::cross(field[grid.arrow_target_id(i2,j)] - source_value,
                                         grid.arrow_normal(i2,j)) * grid.arrow_dual_length(i2,j);
                }
                out[i] /= grid.vertex_dual_area(i2);
            }
        }


        template<typename Grid, typename In, typename Out>
        void laplacian(const Grid& grid, const In& field, Out& out) const {
            using id2 = typename Grid::size_type;
            using id = typename Grid::dimension_type;
            id2 i, i2;
            id j;
            const id N = grid.arrows_per_vertex;
            typename In::value_type source_value;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);
                out[i] = typename Out::value_type(0);
                source_value = field[i2];
                for (j = 0; j < N; ++j)
                {
                    out[i] += (field[grid.arrow_target_id(i2,j)] - source_value)
                            *  grid.arrow_dual_length(i2,j) / grid.arrow_length(i2,j);
                }
                out[i] /= grid.vertex_dual_area(i2);
            }
        }

    };

}

