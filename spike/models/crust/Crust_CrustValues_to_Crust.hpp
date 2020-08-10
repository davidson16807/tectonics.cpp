#pragma once

// std libraries
#include <vector>

// in-house libraries
#include <models/strata/Strata.hpp> // Strata
#include <models/strata/Strata_StrataValues_to_Strata.hpp> // set_max_temperature_received

#include "Crust.hpp"                // Crust
#include "CrustValues.hpp"          // CrustValues

namespace crust
{
    template<int L, int M>
    void set_max_pressures_received(
        const Crust<L,M>& crust, 
        const CrustValues<float,L>& max_pressures_received, 
        Crust<L,M>& output
    ) {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            strata::set_max_pressures_received(strata, max_pressures_received[i], strata);
            output[i].pack(strata);
        }
    }
    template<int L, int M>
    void set_max_temperatures_received(
        const Crust<L,M>& crust, 
        const CrustValues<float,L>& max_temperatures_received, 
        Crust<L,M>& output
    ) {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            strata::set_max_pressures_received(strata, max_temperatures_received[i], strata);
            output[i].pack(strata);
        }
    }
}
