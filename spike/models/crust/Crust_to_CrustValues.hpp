#pragma once

#include <many/types.hpp>         // floats, etc.

#include <rasters/Grid.hpp>       // Grid

#include <models/crust/Crust.hpp> // Crust

namespace crust
{

    // derived attributes: icosphere<Strata> -> icosphere<StrataValues<T>>
    void get_thermal_conductivity(const tmany<StrataStore>& crust, tmany<StrataValues<float>>& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            out[i].count = strata.count;
            for (int j = 0; j < strata.count; ++j)
            {
                out[i].values[j] = strata.strata[j].thermal_conductivity();
            }
        }
    }
    /*
    `get_overburden_mass()` calculates the mass of rock above each layer of rock.
    It is meant for use in calculating pressure.
    Total pressure requires providing too many other variables (such as ocean depth, gravity, etc.),
    so this function exists to reduce the problem scope and complexity of the method signature
    */
    void get_overburden_mass(const tmany<StrataStore>& crust, tmany<StrataValues<float>>& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            out[i].count = strata.count;
            float overburden_mass(0);
            for (int j = 0; j < strata.count; ++j)
            {
                out[i].values[j] = overburden_mass;
                overburden_mass += strata.strata[j].mass();
            }
        }
    }
    void get_depths(
        const tmany<StrataStore>& crust, const std::array<float, mass_pool_count>& mass_pool_densities, 
        tmany<StrataValues<float>>& out
    ) {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            out[i].count = strata.count;
            float depth(0);
            for (int j = 0; j < strata.count; ++j)
            {
                out[i].values[j] = depth;
                depth += strata.strata[j].thickness();
            }
        }
    }



    // setters: icosphere<Strata> x icosphere<StrataValues<T>> -> icosphere<Strata>  (remember, no state modification!)
    void set_max_temperature_received(
        const tmany<StrataStore>& crust, const tmany<StrataValues<float>>& max_temperature_received, 
        tmany<StrataStore>& out
    ) {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            for (int j = 0; j < Strata::max_stratum_count; ++j)
            {
                for (int k = 0; k < Strata::max_pool_count; ++k)
                {
                    strata.strata[j].mass_pools[k].max_temperature_received = max_temperature_received;
                }
            }
            out[i].compress(strata);
        }
    }
    void set_max_temperature_received(
        const tmany<StrataStore>& crust, const tmany<StrataValues<float>>& max_pressure_received, 
        tmany<StrataStore>& out
    ) {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            for (int j = 0; j < Strata::max_stratum_count; ++j)
            {
                for (int k = 0; k < Strata::max_pool_count; ++k)
                {
                    strata.strata[j].mass_pools[k].max_pressure_received = max_pressure_received;
                }
            }
            out[i].compress(strata);
        }
    }



    void get_sediment(const tmany<StrataStore>& crust, const tmany<Stratum>& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            out[i] = strata.count > 0? strata.strata[0] : Stratum();
        }
    }
}
