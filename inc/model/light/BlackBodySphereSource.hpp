#pragma once

namespace light{

    template<typename Power, typename Temperature, typename Radius>
    class BlackBodySphereSource{

        Power luminosity;
        Temperature temperature;
        Radius radius;

    public:

        BlackBodySphereSource(
            const Power luminosity,
            const Temperature temperature,
            const Radius radius
        ): 
            luminosity(luminosity),
            temperature(temperature),
            radius(radius)
        {}

        Power luminosity() const
        {
            // TODO: return an array of power for wavelength bins according to Wein's law.
            return luminosity;
        }
    };

}
