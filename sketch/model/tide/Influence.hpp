#pragma once

namespace gravity{

    template<typename scalar, typename Time, typename Mass, glm::qualifier precision = glm::defaultp>
    struct Influence{

        using vec3 = glm::vec<3,scalar,precision>;

        Mass mass;
        vec3 offset;

        Influence(
            const Mass& mass,
            const vec3& offset,
        ):
            mass(mass),
            offset(offset),
        {}

    };

}
