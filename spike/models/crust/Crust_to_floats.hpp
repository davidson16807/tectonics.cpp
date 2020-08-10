#pragma once

// in-house libraries
#include <many/types.hpp>         // floats, etc.
#include <models/crust/Crust.hpp> // Crust

namespace crust
{
    template<int L, int M>
    void get_mass_pool(const Crust<L,M>& crust, stratum::OxygenPlanetMineralTypes type, many::floats& output)
    {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            output[i] = strata.mass_pool(type);
        }
    }
    template<int L, int M>
    void get_mass(const Crust<L,M>& crust, many::floats& output)
    {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            output[i] = strata.mass();
        }
    }
    template<int L, int M>
    void get_volume(const Crust<L,M>& crust, many::floats& output)
    {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            output[i] = strata.volume();
        }
    }
    template<int L, int M>
    void get_density(const Crust<L,M>& crust, float age, many::floats& output)
    {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            output[i] = strata.density(age);
        }
    }
    template<int L, int M>
    void get_thickness(const Crust<L,M>& crust, many::floats& output)
    {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            output[i] = strata.thickness();
        }
    }
}
