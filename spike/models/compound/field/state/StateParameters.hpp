#pragma once

#include <units/si.hpp>

namespace compound { 
namespace field{

    struct StateParameters
    {
        si::pressure<double> pressure;
        si::temperature<double> temperature;

        constexpr StateParameters(
            const si::pressure<double> pressure, 
            const si::temperature<double> temperature
        ): 
            pressure(pressure), 
            temperature(temperature) 
        {}
        constexpr StateParameters(): 
            pressure(si::standard_pressure), 
            temperature(si::standard_temperature) 
        {}

        constexpr bool operator==(const StateParameters& other) const 
        {
            return 
               pressure == other.pressure 
            && temperature == other.temperature;
        }
        constexpr bool operator!=(const StateParameters& other) const 
        {
            return !((*this)==other);
        }
    };
    struct StateParametersAggregate{
        constexpr StateParametersAggregate(){}

        constexpr bool operator==(const StateParametersAggregate& other) const 
        {
            return true;
        }
        constexpr bool operator!=(const StateParametersAggregate& other) const 
        {
            return !((*this)==other);
        }
    };
}}