#pragma once

// std libraries
#include <limits>

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries
#include "PositiveIndexing.hpp"

namespace cartesian
{

    /*
    `UnboundedIndexing` represents a bijection
    between vectors on a cartesian grid and integers ("memory id"):

        grid_id ↔ memory_id

    `UnboundedIndexing` builds on top of `PositiveIndexing` 
    */
    template<typename id=int, glm::qualifier precision=glm::defaultp>
    class UnboundedIndexing
    {

    public:
        PositiveIndexing<id,precision> positive_indexing;

        template<int L, typename scalar>
        constexpr id memory_id(const glm::vec<L,scalar,precision> grid_position) const {
            id octant_id = 0;
            glm::vec<L,id,precision> grid_id(grid_position);
            for (int i = 0; i < L; ++i)
            {
                octant_id += (2<<i) * (grid_id[i] < 0);
            }
            return octant_id + (2<<L)*positive_indexing.memory_id(glm::abs(grid_id));
        }

    };

}

