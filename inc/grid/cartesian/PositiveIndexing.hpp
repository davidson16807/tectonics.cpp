#pragma once

// std libraries
#include <limits>

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries

namespace cartesian
{

    /*
    `PositiveIndexing` represents a bijection
    between vectors on the positive orthant 
    of a cartesian grid and integers ("memory id"):

        grid_id ↔ memory_id

    `PositiveIndexing` builds on top of `Interleaving` 
    to introduce the concepts of vectors and to make decisions on data locality.
    As such, `PositiveIndexing` must be made specific to `cartesian::`.
    */
    template<typename id=int, glm::qualifier Q=glm::defaultp>
    class PositiveIndexing
    {

    public:

        constexpr PositiveIndexing()
        {
        }

        constexpr id memory_id(const glm::vec<4,id,glm::defaultp> grid_id) const {
            return memory_id(glm::vec<2,id,glm::defaultp>(memory_id(grid_id.xyz()), grid_id.w));
        }

        constexpr id memory_id(const glm::vec<3,id,glm::defaultp> grid_id) const {
            return memory_id(glm::vec<2,id,glm::defaultp>(memory_id(grid_id.xy()), grid_id.z));
        }

        constexpr id memory_id(const glm::vec<2,id,glm::defaultp> grid_id) const {
            id sum = grid_id.x + grid_id.y;
            return (sum+1) * (sum/2) + grid_id.y;
        }

    };

}

