#pragma once

namespace orrery{

    template<typename Luminosity, typename Temperature>
    struct PointLightSource{

        Luminosity luminosity;
        Temperature temperature;

        PointLightSource(
            const Luminosity luminosity,
            const Temperature temperature
        ): 
            luminosity(luminosity),
            temperature(temperature)
        {}
    };

    template<typename scalar, typename Time, typename LightSource, glm::qualifier precision = glm::defaultp>
    struct LightExposure{

        using vec3 = glm::vec<3,scalar,precision>;

        LightSource source;
        vec3 offset;
        Time time;

        LightExposure(
            const LightSource& source,
            const vec3& offset,
            const Time& time
        ):
            source(source),
            offset(offset),
            time(time)
        {}

    };
    
}
