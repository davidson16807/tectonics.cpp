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
#include <glm/vec3.hpp>   // *vec3

// in-house libraries
#include <model/light/Exposure.hpp>
#include <model/light/BlackBodyPointSource.hpp>

#include "Spin.hpp"
#include "EntityComponents.hpp"

namespace orrery
{

    template<typename id, typename scalar, typename Time, typename LightSource, glm::qualifier precision = glm::defaultp>
    class LightSystem{

        using vec3 = glm::vec<3,scalar,precision>;
        using mat3 = glm::mat<3,3,scalar,precision>;

        using LightExposure = light::Exposure<scalar,Time,LightSource>;

        using ids = std::vector<id>;
        using LightExposures = EntityComponents<id,LightExposure>;

    public:

        LightSystem(){}

        template <typename vec3s, typename mat3s, typename PointLightSources>
        void sample(
            const vec3s& positions,
            const mat3s& fixed_for_inertial,
            const id& targets,
            const PointLightSources& sources,
            const Time time,
            LightExposures& exposures
        ) const {
            exposures.clear();
            id entity = target;
            for (id j = 0; j < sources.entity_count(); ++j)
            {
                auto L = positions[sources.entity_for_index(j)];
                auto V = positions[entity];
                auto frame = fixed_for_inertial[entity];
                exposures.add(entity, LightExposure(sources.component_for_index(j), frame*(V-L), time));
            }
        }

        template <typename vec3s, typename mat3s, typename PointLightSources>
        void sample(
            const vec3s& positions,
            const mat3s& fixed_for_inertial,
            const ids& targets,
            const PointLightSources& sources,
            const Time time,
            LightExposures& exposures
        ) const {
            exposures.clear();
            id entity;
            for (std::size_t i = 0; i < targets.size(); ++i) {
                for (id j = 0; j < sources.entity_count(); ++j)
                {
                    entity = targets[i];
                    auto L = positions[sources.entity_for_index(j)];
                    auto V = positions[entity];
                    auto frame = fixed_for_inertial[entity];
                    exposures.add(entity, LightExposure(sources.component_for_index(j), frame*(V-L), time));
                }
            }
        }

    };

}

