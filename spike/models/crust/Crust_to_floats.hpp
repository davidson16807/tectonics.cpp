#pragma once

// in-house libraries
#include <many/types.hpp>         // floats, etc.

#include <models/crust/Crust.hpp> // Crust

namespace crust
{
    // derived attributes: icosphere<Strata> -> icosphere<T>
    void get_mass_pool(const tmany<StrataStore>& crust, OxygenPlanetMassPoolTypes type, const tmany<float>& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            out[i] = strata.mass_pool(type);
        }
    }
    void get_mass(const tmany<StrataStore>& crust, const tmany<float>& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            out[i] = strata.mass();
        }
    }
    void get_volume(const tmany<StrataStore>& crust, const tmany<float>& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            out[i] = strata.volume();
        }
    }
    void get_density(const tmany<StrataStore>& crust, float age, const tmany<float>& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            out[i] = strata.density(age);
        }
    }
    void get_thickness(const tmany<StrataStore>& crust, const tmany<float>& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].decompress(strata);
            out[i] = strata.thickness();
        }
    }
}
