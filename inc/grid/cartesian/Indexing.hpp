#pragma once

// std libraries
#include <limits>

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries
#include <grid/cartesian/Interleaving.hpp>
#include "Projection.hpp"

namespace cartesian
{

    /*
    `Indexing` represents a bijection
    between vectors on a square grid and integers ("memory id"):

        grid_id ↔ memory_id

    `Indexing` builds on top of `Interleaving` 
    to introduce the concepts of vectors and to make decisions on data locality.
    As such, `Indexing` must be made specific to `cartesian::`.
    */
    template<typename id=int, glm::qualifier Q=glm::defaultp>
    class Indexing
    {
        using ivec2 = glm::vec<2,id,glm::defaultp>;

        const cartesian::Interleaving<id> interleave;

    public:
        const id vertices_per_side;
        const id vertex_count;

        constexpr Indexing(const id vertices_per_side) : 
            interleave(vertices_per_side),
            vertices_per_side(vertices_per_side),
            vertex_count(vertices_per_side*vertices_per_side)
        {
        }

        constexpr id memory_id(const ivec2 grid_id) const {
            return interleave.interleaved_id(grid_id.y, grid_id.x);
        }

        inline constexpr ivec2 grid_id(const id memory_id) const {
            return ivec2(
                interleave.element_id(memory_id), 
                interleave.block_id(memory_id)
            );
        }

    };

}

