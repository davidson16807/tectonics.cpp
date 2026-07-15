#pragma once

namespace light{

    template<typename Irradiance, typename Temperature>
    class BlackBodyDirectionalSource{

        Irradiance irradiance;
        Temperature temperature;

    public:

        BlackBodyDirectionalSource(
            const Irradiance irradiance,
            const Temperature temperature
        ): 
            irradiance(irradiance),
            temperature(temperature)
        {}

        Irradiance irradiance() const
        {
            // TODO: return an array of power for wavelength bins according to Wein's law.
            return irradiance;
        }

    };

}
