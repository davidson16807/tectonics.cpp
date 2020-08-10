#pragma once

// std libraries
#include <vector>

// in-house libraries
#include <models/strata/Strata.hpp> // Strata
#include "Crust.hpp"                // Crust

namespace crust
{
    template<int L, int M>
    void set_max_temperature_received(
        const Crust<L,M>& crust, 
        const CrustValues<float,L>& max_temperature_received, 
        Crust<L,M>& out
    ) {
        assert(crust.size() == out.size());
        strata::Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            strata::set_max_temperature_received(strata, max_temperature_received[i], strata);
            out[i].pack(strata);
        }
    }
    template<int L, int M>
    void set_max_temperature_received(
        const Crust<L,M>& crust, 
        const CrustValues<float,L>& max_pressure_received, 
        Crust<L,M>& out
    ) {
        assert(crust.size() == out.size());
        strata::Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            strata::set_max_pressure_received(strata, max_pressure_received[i], strata);
            out[i].pack(strata);
        }
    }
}
