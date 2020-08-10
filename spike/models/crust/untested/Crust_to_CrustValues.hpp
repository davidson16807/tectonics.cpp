#pragma once

#include <models/crust/Crust.hpp> // Crust

namespace crust
{

    template<int L, int M>
    void get_thermal_conductivity(
        const Crust<L,M>& crust, CrustValues<L>& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            out[i].count = strata.count;
            for (int j = 0; j < strata.count; ++j)
            {
                out[i].values[j] = strata.content[j].thermal_conductivity();
            }
        }
    }
    /*
    `get_overburden_mass()` calculates the mass of rock above each layer of rock.
    It is meant for use in calculating pressure.
    Total pressure requires providing too many other variables (such as ocean depth, gravity, etc.),
    so this function exists to reduce the problem scope and complexity of the method signature
    */
    template<int L, int M>
    void get_overburden_mass(
        const Crust<L,M>& crust, 
        CrustValues<L>& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            out[i].count = strata.count;
            float overburden_mass(0);
            for (int j = 0; j < strata.count; ++j)
            {
                out[i].values[j] = overburden_mass;
                overburden_mass += strata.content[j].mass();
            }
        }
    }
    template<int L, int M>
    void get_depths(
        const Crust<L,M>& crust, 
        const std::array<float, M>& mass_pool_densities, 
        CrustValues<L>& out
    ) {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            out[i].count = strata.count;
            float depth(0);
            for (int j = 0; j < strata.count; ++j)
            {
                out[i].values[j] = depth;
                depth += strata.content[j].thickness();
            }
        }
    }
}
