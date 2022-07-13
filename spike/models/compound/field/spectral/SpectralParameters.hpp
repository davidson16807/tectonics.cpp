#pragma once

#include <units/si.hpp>

namespace compound { 
namespace field{

    struct SpectralParameters
    {
        si::wavenumber<double> nlo;
        si::wavenumber<double> nhi;
        si::pressure<double> pressure;
        si::temperature<double> temperature;

        constexpr SpectralParameters(
            const si::wavenumber<double> nlo,
            const si::wavenumber<double> nhi,
            const si::pressure<double> pressure, 
            const si::temperature<double> temperature
        ): 
            nlo(nlo),
            nhi(nhi),
            pressure(pressure), 
            temperature(temperature) 
        {}

        constexpr SpectralParameters(): 
            nlo(14286.0/si::centimeter),
            nhi(25000.0/si::centimeter),
            pressure(si::standard_pressure), 
            temperature(si::standard_temperature) 
        {}

        constexpr bool operator==(const SpectralParameters& other) const 
        {
            return 
               nlo == other.nlo 
            && nhi == other.nhi
            && pressure == other.pressure 
            && temperature == other.temperature;
        }
        constexpr bool operator!=(const SpectralParameters& other) const 
        {
            return !((*this)==other);
        }
    };

    struct SpectralParametersAggregate{
        constexpr SpectralParametersAggregate(){}

        constexpr bool operator==(const SpectralParametersAggregate& other) const 
        {
            return true;
        }
        constexpr bool operator!=(const SpectralParametersAggregate& other) const 
        {
            return !((*this)==other);
        }
    };
}}