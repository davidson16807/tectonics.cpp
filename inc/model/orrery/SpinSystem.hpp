#pragma once

// C libraries
#include <cmath>    // std::abs

// std libraries
#include <limits>   // std::numeric_limits
#include <vector>   // std::vector
#include <algorithm>// std::min/max
#include <utility>  // std::pair

// 3rd-party libraries
#include <glm/mat4x4.hpp> // *mat4x4

// in-house libraries
#include "Spin.hpp"
#include "CommonComponents.hpp"

namespace orrery
{

    template<typename id, typename scalar, typename duration, glm::qualifier precision = glm::defaultp>
    class SpinSystem{

        using Spins = CommonComponents<id, orrery::Spin<scalar,duration>>;

        using mat4 = glm::mat<4,4,scalar,precision>;

        using mat4s = std::vector<mat4>;

    public:

        SpinSystem(){}

        void fixed_for_inertial(
            const Spins& spins,
            const scalar time_offset,
            mat4s& results
        ) const {
            for (std::size_t i = 0; i < results.size(); ++i) {
                results[i] = !spins.has(i)? mat4(1) : mat4(spins.component_for_entity(i).fixed_for_inertial(time_offset));
            }
        }

        void inertial_for_fixed(
            const Spins& spins,
            const scalar time_offset,
            mat4s& results
        ) const {
            for (std::size_t i = 0; i < results.size(); ++i) {
                results[i] = !spins.has(i)? mat4(1) : mat4(spins.component_for_entity(i).inertial_for_fixed(time_offset));
            }
        }

    };

}
