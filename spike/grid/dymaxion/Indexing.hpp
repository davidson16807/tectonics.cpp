#pragma once

// std libraries
#include <limits>

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries
#include <grid/bijective/Interleaving.hpp>
#include "Projection.hpp"

namespace dymaxion
{

    /*
    A `Indexing` class represents a bijection
    between points in space ("points") and integers ("memory id"):

        point ↔ memory_id

    */
    template<typename id=int, typename scalar=double, glm::qualifier Q=glm::defaultp>
    class Indexing
    {

        using ivec2 = glm::vec<2,id,glm::defaultp>;
        using vec2  = glm::vec<2,scalar,glm::defaultp>;
        using IdPoint = Point<id,id>;
        using ScalarPoint = Point<id,scalar>;

        static constexpr vec2 half_cell = vec2(0.5);
        static constexpr scalar s1 = 1;
        static constexpr scalar s2 = 2;

        const Projection<id,scalar,Q> projection;

    public:
        const id vertex_count_per_square_side;
        const scalar vertex_count_per_square_side_scalar;
        const id vertex_count_per_square;
        const id vertex_count;

    private:
        const bijective::Interleaving<id> row_interleave;
        const bijective::Interleaving<id> square_interleave;

    public:
        static constexpr id square_count = 10;

        constexpr Indexing(const id vertex_count_per_square_side) : 
            projection(Projection<id,scalar,Q>()),
            vertex_count_per_square_side(vertex_count_per_square_side),
            vertex_count_per_square_side_scalar(vertex_count_per_square_side),
            vertex_count_per_square(vertex_count_per_square_side * vertex_count_per_square_side),
            vertex_count(square_count*vertex_count_per_square),
            row_interleave(vertex_count_per_square_side),
            square_interleave(vertex_count_per_square)
        {
        }

        constexpr ScalarPoint standardize(const ScalarPoint grid_position) const {
            return 
                (projection.standardize(grid_position / vertex_count_per_square_side_scalar)
                    // apply epsilon while in the [0,1] domain so that the cast to `id` is correct
                    + vec2(std::numeric_limits<scalar>::epsilon()))
                * vertex_count_per_square_side_scalar;
        }

        constexpr IdPoint standardize(const IdPoint grid_id) const {
            return IdPoint(standardize(ScalarPoint(grid_id)));
        }

        constexpr id memory_id(const IdPoint grid_id) const {
            const ScalarPoint standardized(standardize(grid_id));
            const IdPoint clamped(clamp(IdPoint(standardized), 0, vertex_count_per_square_side-1));
            return square_interleave.interleaved_id(
                    clamped.square_id, 
                    row_interleave.interleaved_id(clamped.square_position.y, clamped.square_position.x)
                );
        }

        inline constexpr IdPoint grid_id(const id memory_id) const {
            id square_element_id(square_interleave.element_id(memory_id));
            return IdPoint(
                square_interleave.block_id(memory_id), 
                vec2(row_interleave.element_id(square_element_id), row_interleave.block_id(square_element_id))
            );
        }

    };

}
