#pragma once

// in-house libraries
#include <many/types.hpp>         // floats, etc.
#include <models/crust/Crust.hpp> // Crust

namespace crust
{
    void get_mass_pool(const Crust& crust, stratum::OxygenPlanetMassPoolTypes type, const many::floats& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            out[i] = strata.mass_pool(type);
        }
    }
    void get_mass(const Crust& crust, const many::floats& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            out[i] = strata.mass();
        }
    }
    void get_volume(const Crust& crust, const many::floats& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            out[i] = strata.volume();
        }
    }
    void get_density(const Crust& crust, float age, const many::floats& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            out[i] = strata.density(age);
        }
    }
    void get_thickness(const Crust& crust, const many::floats& out)
    {
        assert(crust.size() == out.size());
        Strata strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            out[i] = strata.thickness();
        }
    }
}
