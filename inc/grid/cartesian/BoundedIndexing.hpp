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

    template<int L, typename id=int, glm::qualifier precision=glm::defaultp>
    struct BoundedIndexing
    {
        using ivec = glm::vec<L,id,precision>;

        const id side_length;
        const id size;

        constexpr BoundedIndexing(const id side_length) : 
            side_length(side_length),
            size(pow(side_length,L))
        {
        }
        constexpr id memory_id(const ivec grid_id) const {
            id output(0);
            for (id i = 0; i < L; ++i)
            {
                output += grid_id[i] * pow(L,i);
            }
            return output;
        }
        inline ivec grid_id(const id memory_id) const
        {
            ivec output(0);
            for (id i = 0; i < L; ++i)
            {
                output[i] = id(memory_id/pow(side_length,i)) % side_length;
            }
            return output;
        }
    };
}

