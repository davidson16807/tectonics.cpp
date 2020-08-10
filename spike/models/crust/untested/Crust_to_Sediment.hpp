#pragma once

// in-house libraries
#include <many/types.hpp>         // floats, etc.
#include <rasters/Grid.hpp>       // Grid
#include <models/crust/Crust.hpp> // Crust
#include <models/crust/Sediment.hpp> // Sediment

namespace crust
{
    template<int L, int M>
    void get_sediment(
        const Crust<L,M>& crust, 
        Sediment<M>& out
    ) {
        assert(crust.size() == out.size());
        Strata<L,M> strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            out[i] = strata.count > 0? strata.content[0] : stratum::Stratum();
        }
    }
}
