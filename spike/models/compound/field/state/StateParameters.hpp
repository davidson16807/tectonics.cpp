#pragma once

#include <units/si.hpp>

namespace compound { 
namespace field{

    struct StateParameters
    {
        si::pressure pressure;
        si::temperature temperature;

        constexpr StateParameters(
            const si::pressure pressure, 
            const si::temperature temperature
        ): 
            pressure(pressure), 
            temperature(temperature) 
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