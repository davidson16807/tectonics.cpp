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

        constexpr SpectralParameters(
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