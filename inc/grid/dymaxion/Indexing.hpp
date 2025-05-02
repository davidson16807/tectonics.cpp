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
    template<typename id, typename id2, typename scalar, glm::qualifier Q=glm::defaultp>
    class Indexing
    {

        using ivec2 = glm::vec<2,id2,glm::defaultp>;
        using vec2  = glm::vec<2,scalar,glm::defaultp>;
        using ipoint = Point<id2,id>;
        using point = Point<id2,scalar>;

        const Projection<id,id2,scalar,Q> projection;

    public:
        const scalar vertices_per_square_side_scalar;
        const id2 vertices_per_square;
        const id2 vertex_count;
        const id vertices_per_square_side;

    private:
        const cartesian::Interleaving<id2> row_interleave;
        const cartesian::Interleaving<id2> square_interleave;

    public:
        static constexpr id2 square_count = 10;

        constexpr Indexing(const id vertices_per_square_side) : 
            projection(Projection<id,id2,scalar,Q>()),
            vertices_per_square_side_scalar(vertices_per_square_side),
            vertices_per_square(vertices_per_square_side * vertices_per_square_side),
            vertex_count(square_count*vertices_per_square),
            vertices_per_square_side(vertices_per_square_side),
            row_interleave(vertices_per_square_side),
            square_interleave(vertices_per_square)
        {
        }

        /*
        `memory_id_when_standard` is the faster pragmatic alternative to `memory_id` that does not call standardize.
        Undefined behavior results when the standardized_grid_id is not already in a standardized form.
        Use this only if you are certain that a grid_id will always be standardized!
        */
        constexpr id2 memory_id_when_standard(const ipoint standardized_grid_id) const {
            const ipoint clamped(clamp(standardized_grid_id, id(0), id(vertices_per_square_side-1)));
            return square_interleave.interleaved_id(
                    clamped.square_id, 
                    row_interleave.interleaved_id(clamped.square_position.y, clamped.square_position.x)
                );
        }

        constexpr ipoint standardize(const ipoint grid_id) const {
            point standardized =(
                projection.standardize((point(grid_id)+vec2(0.5)) / vertices_per_square_side_scalar)
            ) * vertices_per_square_side_scalar;
            return ipoint(standardized);
        }

        constexpr id2 memory_id(const ipoint grid_id) const {
            return memory_id_when_standard(standardize(grid_id));
        }

        inline constexpr ipoint grid_id(const id2 memory_id) const {
            id2 square_element_id(square_interleave.element_id(memory_id));
            return ipoint(
                square_interleave.block_id(memory_id), 
                vec2(row_interleave.element_id(square_element_id), row_interleave.block_id(square_element_id))
            );
        }

    };

}
