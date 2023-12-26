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
    template<typename id=int, glm::qualifier Q=glm::defaultp>
    class UnboundedIndexing
    {

    public:
        PositiveIndexing<id,Q> positive_indexing;

        constexpr UnboundedIndexing()
        {
        }

        template<int L>
        constexpr id memory_id(const glm::vec<L,id,glm::defaultp> grid_id) const {
            id octant_id = 0;
            for (int i = 0; i < L; ++i)
            {
                octant_id += (2<<i) * (grid_id[i] < 0);
            }
            return octant_id + (2<<L)*positive_indexing.memory_id(glm::abs(grid_id));
        }

    };

}

