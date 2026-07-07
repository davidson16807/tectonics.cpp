#pragma once

namespace light{

    template<typename scalar, typename Time, typename Source, glm::qualifier precision = glm::defaultp>
    struct Exposure{

        using vec3 = glm::vec<3,scalar,precision>;

        Source source;
        vec3 offset;
        Time time;

        Exposure(
            const Source& source,
            const vec3& offset,
            const Time& time
        ):
            source(source),
            offset(offset),
            time(time)
        {}

    };

}
