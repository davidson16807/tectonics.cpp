#pragma once

// in-house libraries
#include <math/glm/special.hpp>

#include "../Grid.hpp"

namespace procedural 
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

    struct VectorCalculusByCurvilinearMethod
    {

        /*
        NOTE: See VectorCalculusByCurvilinearMethod.md for more explanation!
        */

        template<typename Grid, typename In, typename Out>
        void square_id(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = grid.square_id(i);
            }
        }

        template<typename Grid, typename In, typename Out>
        void arrow_dual_length0(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 0;
                out[i] += grid.arrow_dual_length(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_dual_length1(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 1;
                out[i] += grid.arrow_dual_length(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_dual_length2(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 2;
                out[i] += grid.arrow_dual_length(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_dual_length3(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 3;
                out[i] += grid.arrow_dual_length(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_length0(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 0;
                out[i] += grid.arrow_length(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_length1(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 0;
                out[i] += grid.arrow_length(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_length2(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 2;
                out[i] += grid.arrow_length(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_length3(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 3;
                out[i] += grid.arrow_length(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_normal0(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 0;
                out[i] += grid.arrow_normal(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_normal1(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 1;
                out[i] += grid.arrow_normal(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_normal2(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 2;
                out[i] += grid.arrow_normal(i,j);
            }
        }
        template<typename Grid, typename In, typename Out>
        void arrow_normal3(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                j = 3;
                out[i] += grid.arrow_normal(i,j);
            }
        }

        template<typename Grid, typename In, typename Out>
        void differential0(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                j = 0;
                out[i] = field[grid.arrow_target_id(i,j)] - field[i];
            }
        }
        template<typename Grid, typename In, typename Out>
        void differential1(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                j = 1;
                out[i] = field[grid.arrow_target_id(i,j)] - field[i];
            }
        }
        template<typename Grid, typename In, typename Out>
        void differential2(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                j = 2;
                out[i] = field[grid.arrow_target_id(i,j)] - field[i];
            }
        }
        template<typename Grid, typename In, typename Out>
        void differential3(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                j = 3;
                out[i] = field[grid.arrow_target_id(i,j)] - field[i];
            }
        }

        template<typename Grid, typename In, typename Out>
        void vertex_dual_area(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                out[i] = typename Out::value_type(0);
                out[i] += grid.vertex_dual_area(i);
            }
        }

        template<typename Grid, typename In, typename Out>
        void gradient(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            id i, j, i2;
            const id N = grid.arrows_per_vertex;
            const id halfN = N/2;
            const id fourthN = N/4;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);

                dvdzeta = grid.vertex_position(grid.arrow_target_id(i2,j-halfN))         - grid.vertex_position(grid.arrow_target_id(i2,j+halfN));
                dvdeta  = grid.vertex_position(grid.arrow_target_id(i2,j-halfN+fourthN)) - grid.vertex_position(grid.arrow_target_id(i2,j+halfN+fourthN));
                local_basis_jacobian = mat2(dvdzeta.x, dvdeta.x, dvdzeta.y, dvdeta.y);

                dfdzeta = field[grid.arrow_target_id(i2,j-halfN)]         - field[grid.arrow_target_id(i2,j+halfN)];
                dfdeta  = field[grid.arrow_target_id(i2,j-halfN+fourthN)] - field[grid.arrow_target_id(i2,j+halfN+fourthN)];

                out[i] = glm::inverse(local_basis_jacobian) * vec2(dfdzeta, dfdeta);;
            }
        }

        template<typename Grid, typename In, typename Out>
        void jacobian(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            id i, j, i2;
            const id N = grid.arrows_per_vertex;
            const id halfN = N/2;
            const id fourthN = N/4;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);

                dvdzeta = grid.vertex_position(grid.arrow_target_id(i2,j-halfN))         - grid.vertex_position(grid.arrow_target_id(i2,j+halfN));
                dvdeta  = grid.vertex_position(grid.arrow_target_id(i2,j-halfN+fourthN)) - grid.vertex_position(grid.arrow_target_id(i2,j+halfN+fourthN));
                local_basis_jacobian = mat2(dvdzeta.x, dvdeta.x, dvdzeta.y, dvdeta.y);

                dudzeta = field[grid.arrow_target_id(i2,j-halfN)]         - field[grid.arrow_target_id(i2,j+halfN)];
                dudeta  = field[grid.arrow_target_id(i2,j-halfN+fourthN)] - field[grid.arrow_target_id(i2,j+halfN+fourthN)];

                out[i] = glm::inverse(local_basis_jacobian) * mat2(dudzeta, dudeta);
            }
        }

        template<typename Grid, typename In, typename Out>
        void divergence(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(grid.compatible(field));
            id i, j, i2;
            const id N = grid.arrows_per_vertex;
            typename In::value_type source_value;
            const id halfN = N/2;
            const id fourthN = N/4;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);

                dvdzeta = grid.vertex_position(grid.arrow_target_id(i2,j-halfN))         - grid.vertex_position(grid.arrow_target_id(i2,j+halfN));
                dvdeta  = grid.vertex_position(grid.arrow_target_id(i2,j-halfN+fourthN)) - grid.vertex_position(grid.arrow_target_id(i2,j+halfN+fourthN));
                local_basis_jacobian = mat2(dvdzeta.x, dvdeta.x, dvdzeta.y, dvdeta.y);

                dudzeta = field[grid.arrow_target_id(i2,j-halfN)]         - field[grid.arrow_target_id(i2,j+halfN)];
                dudeta  = field[grid.arrow_target_id(i2,j-halfN+fourthN)] - field[grid.arrow_target_id(i2,j+halfN+fourthN)];

                field_jacobian = glm::inverse(local_basis_jacobian) * mat2(dudzeta, dudeta);

                out[i] = math::glm::trace(field_jacobian);
            }
        }

        template<typename Grid, typename In, typename Out>
        void curl(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            // assert(compatible(field, out));
            // assert(grid.compatible(field));
            id i, j, i2;
            const id N = grid.arrows_per_vertex;
            typename In::value_type source_value;
            const id halfN = N/2;
            const id fourthN = N/4;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);

                dvdzeta = grid.vertex_position(grid.arrow_target_id(i2,j-halfN))         - grid.vertex_position(grid.arrow_target_id(i2,j+halfN));
                dvdeta  = grid.vertex_position(grid.arrow_target_id(i2,j-halfN+fourthN)) - grid.vertex_position(grid.arrow_target_id(i2,j+halfN+fourthN));
                local_basis_jacobian = mat2(dvdzeta.x, dvdeta.x, dvdzeta.y, dvdeta.y);

                dudzeta = field[grid.arrow_target_id(i2,j-halfN)]         - field[grid.arrow_target_id(i2,j+halfN)];
                dudeta  = field[grid.arrow_target_id(i2,j-halfN+fourthN)] - field[grid.arrow_target_id(i2,j+halfN+fourthN)];

                field_jacobian = glm::inverse(local_basis_jacobian) * mat2(dudzeta, dudeta);

                out[i] = math::glm::permutation(field_jacobian);
            }
        }

    };

}

