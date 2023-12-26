#pragma once

// std libraries
#include <limits>

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries
#include <grid/cartesian/Interleaving.hpp>

namespace cartesian
{

    /*
    `BoundedIndexing` represents a bijection
    between vectors on a square grid and integers ("memory id"):

        grid_id ↔ memory_id

    `BoundedIndexing` builds on top of `Interleaving` 
    to introduce the concepts of vectors and to make decisions on data locality.
    As such, `BoundedIndexing` must be made specific to `cartesian::`.
    */
    template<int L, typename id=int, glm::qualifier Q=glm::defaultp>
    class BoundedIndexing
    {
        using ivec = glm::vec<L,id,glm::defaultp>;

        const cartesian::Interleaving<id> interleave;

    public:
        const id vertices_per_side;
        const id vertex_count;

        constexpr BoundedIndexing(const id vertices_per_side) : 
            interleave(vertices_per_side),
            vertices_per_side(vertices_per_side),
            vertex_count(vertices_per_side*vertices_per_side)
        {
        }

        constexpr id memory_id(const ivec grid_id) const {
            return interleave.interleaved_id(grid_id.y, grid_id.x);
        }

        inline constexpr ivec grid_id(const id memory_id) const {
            return ivec(
                interleave.element_id(memory_id), 
                interleave.block_id(memory_id)
            );
        }

    };

}

