#pragma once

namespace light{

    template<typename Power, typename Temperature>
    class BlackBodyPointSource{

        Power luminosity;
        Temperature temperature;

    public:

        BlackBodyPointSource(
            const Power luminosity,
            const Temperature temperature
        ): 
            luminosity(luminosity),
            temperature(temperature)
        {}

        Power luminosity() const
        {
            // TODO: return an array of power for wavelength bins according to Wein's law.
            return luminosity;
        }
    };

}
