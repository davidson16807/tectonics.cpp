#pragma once

#include <units/si.hpp>

namespace compound { 
namespace field{

    struct SpectralParameters
    {
        si::wavenumber nlo;
        si::wavenumber nhi;
        si::pressure pressure;
        si::temperature temperature;

        SpectralParameters(
            const si::wavenumber nlo,
            const si::wavenumber nhi,
            const si::pressure pressure, 
            const si::temperature temperature
        ): 
            nlo(nlo),
            nhi(nhi),
            pressure(pressure), 
            temperature(temperature) 
        {}

        constexpr bool operator==(const SpectralParameters& other) const 
        {
            return 
               pressure == other.pressure 
            && temperature == other.temperature;
        }
        constexpr bool operator!=(const SpectralParameters& other) const 
        {
            return !((*this)==other);
        }
    };

    struct SpectralParametersAggregate{};
}}