#pragma once

#include "../Grid.hpp"

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

    struct VectorCalculusByMultilinearMethod
    {

        /*
        NOTE: See VectorCalculusByMultilinearMethod.md for more explanation!
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
            vec3   sample, samplesum;
            scalar weight, weightsum, distance;
            vec3   A, B, O;
            scalar a, b, o;
            const id N = grid.arrows_per_vertex;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);

                O  = grid.vertex_position(i2);
                o = field[i2];
                B = grid.vertex_position(grid.arrow_target_id(i2,N-1));
                b = field[grid.arrow_target_id(i2,N-1)];

                samplesum = vec3(0,0,0);
                weightsum = scalar(0);

                for (j = 0; j < N; j++)
                {
                    A = B;
                    a = b;
                    B = grid.vertex_position(grid.arrow_target_id(i2,j));
                    b = field[grid.arrow_target_id(i2,j)];

                    sample   = glm::inverse(glm::transpose(mat3(A,O,B))) * vec3(a,o,b);
                    distance = glm::distance((A+O+B)/3.0, origin);
                    weight   = 1/(distance*distance);
                    samplesum += sample*weight
                    weightsum += weight;
                }

                out[i] = samplesum / weightsum;
            }
        }

        template<typename Grid, typename In, typename Out>
        void divergence(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            id i, j, i2;
            vec3   sample, samplesum;
            scalar weight, weightsum, distance;
            vec3   A, B, O;
            scalar a1, b1, o1;
            scalar a2, b2, o2;
            scalar a3, b3, o3;
            const id N = grid.arrows_per_vertex;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);

                O  = grid.vertex_position(i2);
                o1 = field[i2].x;
                o2 = field[i2].y;
                o3 = field[i2].z;
                B  = grid.vertex_position(grid.arrow_target_id(i2,N-1));
                b1 = field[grid.arrow_target_id(i2,N-1)].x;
                b2 = field[grid.arrow_target_id(i2,N-1)].y;
                b3 = field[grid.arrow_target_id(i2,N-1)].z;

                samplesum = mat3(0,0,0,0,0,0,0,0,0);
                weightsum = scalar(0);

                for (j = 0; j < N; j++)
                {
                    A   = B;
                    a1  = b1;
                    a2  = b2;
                    a3  = b3;
                    B   = grid.vertex_position(grid.arrow_target_id(i2,j));
                    b1  = field[grid.arrow_target_id(i2,j)].x;
                    b2  = field[grid.arrow_target_id(i2,j)].y;
                    b3  = field[grid.arrow_target_id(i2,j)].z;

                    sample = mat3(
                        glm::inverse(glm::transpose(mat3(A,O,B))) * vec3(a1,o1,b1),
                        glm::inverse(glm::transpose(mat3(A,O,B))) * vec3(a2,o2,b2),
                        glm::inverse(glm::transpose(mat3(A,O,B))) * vec3(a3,o3,b3)
                    );
                    distance = glm::distance((A+O+B)/3.0, origin);
                    weight   = 1/(distance*distance);
                    samplesum += sample*weight
                    weightsum += weight;
                }

                out[i] = math::trace(samplesum / weightsum);
            }
        }


        template<typename Grid, typename In, typename Out>
        void curl(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            id i, j, i2;
            vec3   sample, samplesum;
            scalar weight, weightsum, distance;
            vec3   A, B, O;
            scalar a1, b1, o1;
            scalar a2, b2, o2;
            scalar a3, b3, o3;
            const id N = grid.arrows_per_vertex;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);

                O  = grid.vertex_position(i2);
                o1 = field[i2].x;
                o2 = field[i2].y;
                o3 = field[i2].z;
                B  = grid.vertex_position(grid.arrow_target_id(i2,N-1));
                b1 = field[grid.arrow_target_id(i2,N-1)].x;
                b2 = field[grid.arrow_target_id(i2,N-1)].y;
                b3 = field[grid.arrow_target_id(i2,N-1)].z;

                samplesum = mat3(0,0,0,0,0,0,0,0,0);
                weightsum = scalar(0);

                for (j = 0; j < N; j++)
                {
                    A   = B;
                    a1  = b1;
                    a2  = b2;
                    a3  = b3;
                    B   = grid.vertex_position(grid.arrow_target_id(i2,j));
                    b1  = field[grid.arrow_target_id(i2,j)].x;
                    b2  = field[grid.arrow_target_id(i2,j)].y;
                    b3  = field[grid.arrow_target_id(i2,j)].z;

                    sample = mat3(
                        glm::inverse(glm::transpose(mat3(A,O,B))) * vec3(a1,o1,b1),
                        glm::inverse(glm::transpose(mat3(A,O,B))) * vec3(a2,o2,b2),
                        glm::inverse(glm::transpose(mat3(A,O,B))) * vec3(a3,o3,b3)
                    );
                    distance = glm::distance((A+O+B)/3.0, origin);
                    weight   = 1/(distance*distance);
                    samplesum += sample*weight
                    weightsum += weight;
                }

                out[i] = math::permutation(samplesum / weightsum);
            }
        }


        template<typename Grid, typename In, typename Out>
        void laplacian(const Grid& grid, const In& field, Out& out) const {
            using id = typename Grid::size_type;
            id i, j, i2;
            vec3   sample, samplesum;
            scalar weight, weightsum, distance;
            vec3   A, B, O;
            scalar a, b, o;
            const id N = grid.arrows_per_vertex;
            for (i = 0; i < grid.vertex_count(); ++i)
            {
                i2 = grid.vertex_representative(i);

                O  = grid.vertex_position(i2);
                o = field[i2];
                B = grid.vertex_position(grid.arrow_target_id(i2,N-1));
                b = field[grid.arrow_target_id(i2,N-1)];

                samplesum = vec3(0,0,0);
                weightsum = scalar(0);

                for (j = 0; j < N; j++)
                {
                    A = B;
                    a = b;
                    B = grid.vertex_position(grid.arrow_target_id(i2,j));
                    b = field[grid.arrow_target_id(i2,j)];

                    sample   = glm::inverse(glm::transpose(mat3(A,O,B))) * vec3(a,o,b);
                    distance = glm::distance((A+O+B)/3.0, origin);
                    weight   = 1/(distance*distance);
                    samplesum += sample*weight
                    weightsum += weight;
                }

                out[i] = samplesum / weightsum;
            }
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
    };

}

