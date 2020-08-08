#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include "Strata.hpp"
#include "StrataValues.hpp"

namespace strata
{
    template<int L, int M>
    void set_max_temperatures_received(
        const Strata<L,M>& strata,
        const StrataValues<float, L>& temperatures,
        Strata<L,M>& output
    ) {
        if(&strata != &output){output = strata;}
        assert(strata.count == temperatures.count);
        for (int i=0; i<strata.count; i++)
        {
            output.content[i].max_temperature_received = 
                std::max(strata.content[i].max_temperature_received, temperatures.values[i]);
        }
    }
    template<int L, int M>
    void set_max_pressures_received(
        const Strata<L,M>& strata,
        const StrataValues<float, L>& pressures,
        Strata<L,M>& output
    ) {
        if(&strata != &output){output = strata;}
        assert(strata.count == pressures.count);
        for (int i=0; i<strata.count; i++)
        {
            output.content[i].max_pressure_received = 
                std::max(strata.content[i].max_pressure_received, pressures.values[i]);
        }
    }
}