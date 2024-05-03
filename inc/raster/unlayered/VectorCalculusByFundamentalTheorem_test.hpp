#pragma once

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

    struct VectorCalculusByFundamentalTheoremDebug
    {

        /*
        NOTE: See VectorCalculusByFundamentalTheorem.md for more explanation!
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
    };

}

