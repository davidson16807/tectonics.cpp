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
    `OrthantIndexing` represents a bijection
    between orthants of a cartesian grid and integers ("memory id"):

        grid_id ↔ memory_id

    `OrthantIndexing` builds on top of `Interleaving` 
    to introduce the concepts of vectors and to make decisions on data locality.
    As such, `OrthantIndexing` must be made specific to `cartesian::`.
    */
    template<int L, typename id=int, glm::qualifier precision=glm::defaultp>
    struct OrthantIndexing
    {
        using ivec = glm::vec<L,id,precision>;
        static constexpr id size = 1<<L;
        constexpr id memory_id(const ivec grid_id) const {
            id output(0);
            for (int i = 0; i < L; ++i)
            {
                output += grid_id[i]<<i;
            }
            return output;
        }
        inline ivec grid_id(const id memory_id) const
        {
            ivec output(0);
            for (int i = 0; i < L; ++i)
            {
                output[i] = memory_id & (1<<i);
            }
            return output;
        }
    };

}

