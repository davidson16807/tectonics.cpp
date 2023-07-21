#pragma once

// in-house libraries
#include <rasters/entities/Grid/Raster.hpp>         // floats, etc.

#include <models/crust/Crust.hpp> // Crust

namespace crust
{
    template<std::size_t L, std::size_t M, typename Tgrid>
    void get_mass_pool(
        const Crust<L,M,Tgrid>& crust, 
        stratum::OxygenPlanetMineralTypes type, 
        rasters::Raster<float,Tgrid>& output
    ){
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            output[i] = strata.mass_pool(type);
        }
    }
    template<std::size_t L, std::size_t M, typename Tgrid>
    void get_mass(const Crust<L,M,Tgrid>& crust, rasters::Raster<float,Tgrid>& output)
    {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            output[i] = strata.mass();
        }
    }
    template<std::size_t L, std::size_t M, typename Tgrid>
    void get_volume(const Crust<L,M,Tgrid>& crust, rasters::Raster<float,Tgrid>& output)
    {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            output[i] = strata.volume();
        }
    }
    template<std::size_t L, std::size_t M, typename Tgrid>
    void get_density(const Crust<L,M,Tgrid>& crust, float age, rasters::Raster<float,Tgrid>& output)
    {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            output[i] = strata.density(age);
        }
    }
    template<std::size_t L, std::size_t M, typename Tgrid>
    void get_thickness(const Crust<L,M,Tgrid>& crust, rasters::Raster<float,Tgrid>& output)
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
