#pragma once

// std libraries
#include <set>                // set
#include <unordered_map>      // unordered_map

// in-house libraries
#include <models/strata/Strata.hpp> // Strata

#include "DensityTower.hpp"   // DensityTower
#include "Crust.hpp"          // Crust

namespace crust
{

    template <std::size_t L, std::size_t M, typename Tgrid>
    void get_DensityTower(
        const Crust<L,M,Tgrid>& crust, 
        const std::array<float, M>& mineral_densities,
        const std::array<float, M>& liquid_densities,
        const std::array<float, M>& liquid_mass_pools,
        float radius_of_world,
        float age_of_world,
        DensityTower<L,Tgrid>& tower
    ){
        // assert grids share the same cache
        assert(crust.grid.cache == tower.grid.cache);
        // iterate through cells in the raster
        std::size_t N(crust.grid.cache->vertex_count);
        strata::Strata<L,M> strata;
        for (std::size_t i = 0; i < N; ++i)
        {
            crust[i].unpack(strata);

            float stratum_volume(0.0f);
            float stratum_thickness(0.0f);

            float overburden_mass(0.0f);
            float overburden_volume(0.0f);
            float overburden_thickness(0.0f);
            float overburden_density(0.0f);
            
            float cell_area(0.0f);

            std::size_t liquid_id(0);
            // iterate through strata in cell
            for (std::size_t j = 0; j < strata.count; ++j)
            {
                // estimate overburden with layer
                // NOTE: we do not consider the contribution of liquid layers to overburden
                cell_area = crust.grid.cache->vertex_areas[i]*radius_of_world*radius_of_world;
                stratum_volume = strata.content[j].volume(age_of_world, mineral_densities);
                stratum_thickness = stratum_volume / cell_area;
                overburden_mass += strata.content[j].mass();
                overburden_volume += strata.content[j].volume(age_of_world, mineral_densities);
                overburden_thickness = overburden_volume / cell_area;
                overburden_density = overburden_mass / overburden_volume;
                for (; (overburden_density > liquid_densities[liquid_id] || 
                        liquid_mass_pools[liquid_id] <= 0.0f) && liquid_id < M; liquid_id++){}
                tower[i].values[j].stratum_thickness = stratum_thickness;
                tower[i].values[j].overburden_density = overburden_density;
                tower[i].values[j].overburden_thickness = overburden_thickness;
                tower[i].values[j].liquid_id_below = liquid_id;
            }
            tower[i].count = strata.count;
        }
    }
}
