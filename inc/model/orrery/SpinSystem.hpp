#pragma once

// C libraries
#include <cmath>    // std::abs

// std libraries
#include <limits>   // std::numeric_limits
#include <vector>   // std::vector
#include <algorithm>// std::min/max
#include <utility>  // std::pair

// 3rd-party libraries
#include <glm/mat3x3.hpp> // *mat3x3

// in-house libraries
#include "Spin.hpp"
#include "DenseContiguousComponents.hpp"

namespace orrery
{

    template<typename id, typename scalar, typename duration, glm::qualifier precision = glm::defaultp>
    class SpinSystem{

        using Spins = DenseContiguousComponents<id, orrery::Spin<scalar,duration>>;

        using mat3 = glm::mat<3,3,scalar,precision>;

    public:

        SpinSystem(){}

        template<typename matrices>
        void fixed_for_inertial(
            const Spins& spins,
            const scalar time_offset,
            matrices& results
        ) const {
            for (std::size_t i = 0; i < results.size(); ++i) {
                results[i] = !spins.has(i)? mat3(1) : spins.component_for_entity(i).fixed_for_inertial(time_offset);
            }
        }

        template<typename matrices>
        void inertial_for_fixed(
            const Spins& spins,
            const scalar time_offset,
            matrices& results
        ) const {
            for (std::size_t i = 0; i < results.size(); ++i) {
                results[i] = !spins.has(i)? mat3(1) : spins.component_for_entity(i).inertial_for_fixed(time_offset);
            }
        }

    };

}
