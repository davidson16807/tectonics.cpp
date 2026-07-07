#pragma once

namespace light{

    template<typename Luminosity, typename Temperature>
    struct PointSource{

        Luminosity luminosity;
        Temperature temperature;

        PointSource(
            const Luminosity luminosity,
            const Temperature temperature
        ): 
            luminosity(luminosity),
            temperature(temperature)
        {}
    };

}
