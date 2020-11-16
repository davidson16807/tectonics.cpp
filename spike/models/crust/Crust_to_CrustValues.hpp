#pragma once

#include <models/strata/Strata_to_StrataValues.hpp> 

#include "Crust.hpp"
#include "CrustValues.hpp"

namespace crust
{


    template<int L, int M, typename Tgrid>
    void get_max_pressures_received(
        const Crust<L,M,Tgrid>& crust,
        CrustValues<float,L,Tgrid>& output
    ) {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i=0; i< crust.size(); i++)
        {
            crust[i].unpack(strata);
            strata::get_max_pressures_received(strata, output[i]);
        }
    }
    template<int L, int M, typename Tgrid>
    void get_max_temperatures_received(
        const Crust<L,M,Tgrid>& crust,
        CrustValues<float,L,Tgrid>& output
    ) {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i=0; i< crust.size(); i++)
        {
            crust[i].unpack(strata);
            strata::get_max_temperatures_received(strata, output[i]);
        }
    }
    template<int L, int M, typename Tgrid>
    void get_mass(
        const Crust<L,M,Tgrid>& crust,
        CrustValues<float,L,Tgrid>& output
    ) {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i=0; i< crust.size(); i++)
        {
            crust[i].unpack(strata);
            strata::get_mass(strata, output[i]);
        }
    }
    template<int L, int M, typename Tgrid>
    void get_volumes(
        const Crust<L,M,Tgrid>& crust,
        const float age_of_world, 
        const std::array<float, M>& mass_pool_densities,
        CrustValues<float,L,Tgrid>& output
    ) {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i=0; i< crust.size(); i++)
        {
            crust[i].unpack(strata);
            strata::get_volumes(strata, age_of_world, mass_pool_densities, output[i]);
        }
    }
    template<int L, int M, typename Tgrid>
    void get_densities(
        const Crust<L,M,Tgrid>& crust,
        const float age_of_world, 
        const std::array<float, M>& mass_pool_densities,
        CrustValues<float,L,Tgrid>& output
    ) {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i=0; i< crust.size(); i++)
        {
            crust[i].unpack(strata);
            strata::get_densities(strata, age_of_world, mass_pool_densities, output[i]);
        }
    }
    template<int L, int M, typename Tgrid>
    void get_thermal_conductivities(
        const Crust<L,M,Tgrid>& crust,
        const float age_of_world,
        const std::array<float, M>& mass_pool_densities,
        const std::array<float, M>& mass_pool_thermal_conductivities,
        CrustValues<float,L,Tgrid>& output
    ) {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (std::size_t i=0; i< crust.size(); i++)
        {
            crust[i].unpack(strata);
            strata::get_thermal_conductivities(
                strata, 
                age_of_world, 
                mass_pool_densities, 
                mass_pool_thermal_conductivities, 
                output[i]
            );
        }
    }

    /*
    `get_overburden_mass()` calculates the mass of rock above each layer of rock.
    It is meant for use in calculating pressure.
    Total pressure requires providing too many other variables (such as ocean depth, gravity, etc.),
    so this function exists to reduce the problem scope and complexity of the method signature
    */
    template<int L, int M, typename Tgrid>
    void get_overburden_mass(
        const Crust<L,M,Tgrid>& crust,
        CrustValues<float,L,Tgrid>& output
    ) {
        assert(crust.size() == output.size());
        strata::Strata<L,M> strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            strata::get_overburden_mass(strata, output[i]);
        }
    }
    /*
    `get_overburden_volume()` calculates the mass of rock above each layer of rock.
    It is meant for use in calculating depth.
    Total depth requires providing too many other variables (such as ocean depth, surface area, etc.),
    so this function exists to reduce the problem scope and complexity of the method signature
    */
    template<int L, int M, typename Tgrid>
    void get_overburden_volume(
        const Crust<L,M,Tgrid>& crust,
        const float age_of_world, 
        const std::array<float, M>& mass_pool_densities, 
        CrustValues<float,L,Tgrid>& output
    ) {
        assert(crust.grid == output.grid);
        strata::Strata<L,M> strata;
        for (int i = 0; i < crust.size(); ++i)
        {
            crust[i].unpack(strata);
            strata::get_overburden_volume(strata, age_of_world, mass_pool_densities, output[i]);
        }
    }

}
