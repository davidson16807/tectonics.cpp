#pragma once

#include <many/types.hpp>         // floats, etc.

#include <rasters/Grid.hpp>       // Grid

#include <models/rock/Strata.hpp> // Strata

/*
"crust.hpp" deals with morphisms in a mathematical category that is composed from
types reflecting the distribution of strata over the suface of a mesh and their properties,
depicted in the following category diagram:

              plates          
            ↗    ⇅   ↖        
     deltas ⇇ master ⇉ derived
        ↑   ↙   ⇅⇅           
     sediment  values         
                 ⇅            
               blocks         
                              
with the following definitions:
* "plates"   std::vector<tmany<StrataStore>>   rock layer objects within cells of several SpheroidGrids, one per plate
* "master"   tmany<StrataStore>                rock layer objects within cells of a single global SpheroidGrid
* "deltas"   tmany<StrataStore>                rock layer objects within cells of a single global SpheroidGrid, indicating differences in timesteps
* "derived"  tmany<float>                      floats mapping to cells within a single global SpheroidGrid
* "sediment" tmany<Stratum>                    Stratum objects representing the topmost layer within cells of a single global SpheroidGrid
* "values"   tmany<StrataValues<float>>        floats mapping to rock layers within cells of a single global SpheroidGrid
* "blocks"   tmany<float>                      floats mapping to cells within a single global LayeredSpheroidGrid, like Minecraft blocks
*/
namespace rock
{
    // derived attributes: icosphere<Strata> -> icosphere<T>
    void get_mass_pool(const tmany<StrataStore>& crust, OxygenPlanetStratumMassPoolTypes type, const tmany<float>& out)
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


    // icosphere<Strata> <-> layered<Strata>
    template <typename T>
    void get_blocks_from_strata(
        const tmany<float>& surface_displacement, 
        const SpheroidGrid& strata_grid, const tmany<StrataValues<float>>& strata_depths, const tmany<StrataValues<T>>& strata_values, 
        const LayeredSpheroidGrid& block_grid, tmany<T>& block_values
    ){
        assert(strata_values.size() == strata_grid.vertex_count);
        assert(strata_values.size() == strata_depths.vertex_count);
        assert(block_values.size() == block_grid.vertex_count);
        assert(strata_grid.vertex_count == block_grid.vertex_count);
        for (int i = 0; i < strata_depths.size(); ++i)
        {
            int surface_block_i(surface_displacement[i] / block_grid.layer_height);
            float strata_depth(0);
            int strata_i(0);
            int block_i (0);
            for (; strata_i < strata_depths[i].count && (surface_block_i+block_i) < block_grid.layer_count; ++strata_i)
            {
                strata_depth += strata_depths[i].values[strata_i];
                for (; block_i * block_grid.layer_height < strata_depth && (surface_block_i+block_i) < block_grid.layer_count; ++block_i)
                {
                    block_values[i*block_grid.layer_count + surface_block_i+block_i] = strata_values[i].values[strata_i];
                }
            }
        }
    }
    template <typename T>
    void get_strata_from_blocks(
        const tmany<float>& surface_displacement, 
        const LayeredSpheroidGrid& block_grid, const tmany<T>& block_values, 
        const SpheroidGrid& strata_grid, const tmany<StrataValues<float>>& strata_depths, tmany<StrataValues<T>>& strata_values
    ){
        assert(strata_values.size() == strata_grid.vertex_count);
        assert(strata_values.size() == strata_depths.vertex_count);
        assert(block_values.size() == block_grid.vertex_count);
        assert(strata_grid.vertex_count == block_grid.vertex_count);
        for (int i = 0; i < strata_depths.size(); ++i)
        {
            int surface_block_i(surface_displacement[i] / block_grid.layer_height);
            int strata_i(0);
            int block_i (0);
            for (; (surface_block_i+block_i) < block_grid.layer_count && strata_i < strata_depths[i].count; ++block_i)
            {
                float strata_depth(0);
                for (; strata_depths[i].values[strata_i] < block_i * block_grid.layer_height && strata_i < strata_depths[i].count; ++strata_i)
                {
                    strata_depth += strata_depths[i].values[strata_i];
                    strata_values[i].values[strata_i] = block_values[i*block_grid.layer_count + surface_block_i+block_i];
                }
            }
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

