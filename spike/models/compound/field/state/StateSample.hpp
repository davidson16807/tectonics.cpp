#pragma once

#include <units/si.hpp>

namespace compound { 
namespace field{
    template<typename T>
    struct StateSample
    {
        T entry;
        si::pressure pressure;
        si::temperature temperature;

        StateSample(
            const T entry, 
            const si::pressure pressure, 
            const si::temperature temperature
        ): 
            entry(entry), 
            pressure(pressure), 
            temperature(temperature) 
        {}
        StateSample(
            const T entry
        ): 
            entry(entry), 
            pressure(si::standard_pressure), 
            temperature(si::standard_temperature) 
        {}

        constexpr bool operator==(const StateSample<T>& other) const 
        {
            return 
               entry == other.entry 
            && pressure == other.pressure 
            && temperature == other.temperature;
        }
        constexpr bool operator!=(const StateSample<T>& other) const 
        {
            return !((*this)==other);
        }
    };

}}