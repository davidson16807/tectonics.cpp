#pragma once

// std libraries
#include <limits>

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries
#include <grid/cartesian/Interleaving.hpp>
#include "Projection.hpp"

namespace dymaxion
{

    /*
    A `Indexing` class represents a bijection
    between points in space ("points") and integers ("memory id"):

        point ↔ memory_id

    */
    template<typename id, typename scalar, glm::qualifier Q=glm::defaultp>
    class Indexing
    {

        using ivec2 = glm::vec<2,id,glm::defaultp>;
        using vec2  = glm::vec<2,scalar,glm::defaultp>;
        using ipoint = Point<id,id>;
        using point = Point<id,scalar>;

        const Projection<id,scalar,Q> projection;

    public:
        const id vertices_per_square_side;
        const id vertices_per_square_side_1;
        const scalar vertices_per_square_side_scalar;
        const id vertices_per_square;
        const id vertex_count;

    private:
        const cartesian::Interleaving<id> row_interleave;
        const cartesian::Interleaving<id> square_interleave;

    public:
        static constexpr id square_count = 10;
        static constexpr scalar half2 = 0.5;

        constexpr Indexing(const id vertices_per_square_side) : 
            projection(Projection<id,scalar,Q>()),
            vertices_per_square_side(vertices_per_square_side),
            vertices_per_square_side_1(vertices_per_square_side-1),
            vertices_per_square_side_scalar(vertices_per_square_side),
            vertices_per_square(vertices_per_square_side * vertices_per_square_side),
            vertex_count(square_count*vertices_per_square),
            row_interleave(vertices_per_square_side),
            square_interleave(vertices_per_square)
        {
        }

        inline constexpr ipoint standardize(const ipoint grid_id) const {
            return ipoint(
                projection.standardize(
                    point(grid_id.square_id, (vec2(grid_id.square_position)+half2) / vertices_per_square_side_scalar)
                ) * vertices_per_square_side_scalar
            );
        }

        inline constexpr id memory_id(const ipoint grid_id) const {
            const ipoint standardized(standardize(grid_id));
            return  standardized.square_id * vertices_per_square + 
                    std::min(standardized.square_position.y, vertices_per_square_side_1) * vertices_per_square_side + 
                    std::min(standardized.square_position.x, vertices_per_square_side_1);
            // const ipoint clamped(clamp(standardized, 0, vertices_per_square_side-1));
            // return square_interleave.interleaved_id(
            //         clamped.square_id, 
            //         row_interleave.interleaved_id(clamped.square_position.y, clamped.square_position.x)
            //     );
        }

        inline constexpr ipoint grid_id(const id memory_id) const {
            id square_element_id(square_interleave.element_id(memory_id));
            return ipoint(
                square_interleave.block_id(memory_id), 
                ivec2(row_interleave.element_id(square_element_id), row_interleave.block_id(square_element_id))
            );
        }

    };

}
