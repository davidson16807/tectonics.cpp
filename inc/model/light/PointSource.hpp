#pragma once

namespace light{

    template<typename Power, typename Temperature>
    struct PointSource{

        Power luminosity;
        Temperature temperature;

        PointSource(
            const Power luminosity,
            const Temperature temperature
        ): 
            luminosity(luminosity),
            temperature(temperature)
        {}
    };

}
