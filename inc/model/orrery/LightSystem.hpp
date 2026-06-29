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
#include "DenseContiguousComponents.hpp"

namespace orrery
{

    template<typename Luminosity, typename Temperature>
    class PointLightSource{

        Luminosity luminosity;
        Temperature temperature;

    }

    template<typename scalar, typename Time, typename LightSource, glm::qualifier precision = glm::defaultp>
    class LightExposure{

        using vec3 = glm::vec3<3,scalar,precision>;

        vec3 offset;

        LightSource source;

        Time time;

    }

    template<typename scalar, glm::qualifier precision = glm::defaultp>
    class LightSystem{

        using vec3 = glm::vec3<3,scalar,precision>;

        using vec3s = std::vector<vec3>;

    public:

        LightSystem(){}

        template <typename PointLightSources, typename LightExposures>
        void sample(
            const PointLightSources& sources,
            const vec3s& positions,
            LightExposures& exposures
        ) const {
            exposures.clear();
            vec3 L,V;
            PointLightSource source;  
            for (std::size_t i = 0; i < positions.size(); ++i) {
                for (int j = 0; j < sources.size(); ++j)
                {
                    V = positions[i];
                    L = positions[sources.entity_for_index(j)];
                    source = sources.component_for_index(j);
                    exposures.add(i, LightExposure(V-L, source, time));
                }
            }
        }

    };

}

