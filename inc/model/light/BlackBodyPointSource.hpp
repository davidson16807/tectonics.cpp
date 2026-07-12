#pragma once

namespace light{

    template<typename Power, typename Temperature>
    struct BlackBodyPointSource{

        Power luminosity;
        Temperature temperature;

        BlackBodyPointSource(
            const Power luminosity,
            const Temperature temperature
        ): 
            luminosity(luminosity),
            temperature(temperature)
        {}
    };

}
