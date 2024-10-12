#pragma once

// std libraries
#include <set>                // set
#include <unordered_map>      // unordered_map

// in-house libraries
#include <models/sea/Raster_to_float.hpp>

#include "DensityTower.hpp"   // DensityTower
#include "LiquidMixture.hpp"  // LiquidMixture
#include "StrataBoundaries.hpp"

namespace crust
{

    template <std::size_t L, std::size_t M, typename Tgrid>
    void get_StrataBoundaries(
        const DensityTower<L,Tgrid>& tower, 
        const std::vector<LiquidMixture>& mixtures,
        const std::array<float, M>& liquid_mass_pools,
        const std::array<float, M>& liquid_densities,
        const float radius_to_bottom_of_crust,
        StrataBoundaries<L,Tgrid>& strata_boundaries,
        rasters::Raster<float, Tgrid> scratch,
        rasters::Raster<float, Tgrid> radius_to_top_of_last_contiguous_strata
    ){
        // assert grids share the same cache
        assert(tower.grid.cache == strata_boundaries.grid.cache);
        // assert liquids are sorted by density
        for (std::size_t i = 1; i < M; ++i)
        {
            assert(liquid_densities[i-1] >= liquid_densities[i]);
        }
        /* 
        */
        std::size_t N(tower.grid.cache->vertex_count);
        // iterate through liquids
        procedural::fill(radius_to_top_of_last_contiguous_strata, radius_to_bottom_of_crust);
        float radius_to_top_of_last_mixture(radius_to_bottom_of_crust);
        for (std::size_t mixture_id = 0; mixture_id < mixtures.size(); ++mixture_id)
        {
            float mixture_density(-1.0f);
            LiquidMixture mixture = mixtures[mixture_id];
            // i==M indicates that no liquid exists which can settle beneath the stratum
            radius_to_top_of_last_mixture = mixture.lightest_liquid_id != M? sea::solve_sealevel(
                radius_to_top_of_last_contiguous_strata, 
                mixture.density(liquid_mass_pools, liquid_densities), 
                radius_to_top_of_last_mixture, 
                scratch
            ) : radius_to_top_of_last_mixture;

            // iterate through cells in crust
            for (std::size_t cell_id = 0; cell_id < N; ++cell_id)
            {
                /*
                NOTE: if two strata share the same id within `liquid_ids_below_strata`, 
                that indicates there is no liquid that can separate them.
                In other words, all strata that share the same id within `liquid_ids_below_strata` 
                are part of a single contiguous solid layer.
                We use this concept to determine the displacement of the contiguous solid layer 
                above the fluid layer below it, 
                which then goes on to determine the displacement of the intermixed fluid layer above it.
                */
                std::uint8_t stratum_id_at_top_of_contiguous_strata(0);
                for (; tower[cell_id].values[stratum_id_at_top_of_contiguous_strata].liquid_id_below != mixture.lightest_liquid_id && 
                       stratum_id_at_top_of_contiguous_strata < tower[cell_id].count; stratum_id_at_top_of_contiguous_strata++){}
                std::uint8_t stratum_id_below_contiguous_strata(stratum_id_at_top_of_contiguous_strata);
                for (; tower[cell_id].values[stratum_id_below_contiguous_strata].liquid_id_below == mixture.lightest_liquid_id && 
                       stratum_id_below_contiguous_strata < tower[cell_id].count; stratum_id_below_contiguous_strata++){}

                // NOTE: we neglect the contribution of lighter liquids to overburden thickness
                float overburden_density(tower[cell_id].values[stratum_id_below_contiguous_strata].overburden_density);
                float overburden_thickness(tower[cell_id].values[stratum_id_below_contiguous_strata].overburden_thickness);

                // start at bottom of contiguous strata and increment upward
                // avoid division by 0 if no liquid exists which can settle beneath the stratum 
                float radius( radius_to_top_of_last_mixture 
                    - mixture_density > 0.0f? overburden_thickness * overburden_density / mixture_density : 0.0f );
                for (std::size_t stratum_id = stratum_id_below_contiguous_strata+1; 
                     stratum_id <= stratum_id_at_top_of_contiguous_strata && stratum_id < tower[cell_id].count; 
                     ++stratum_id)
                {
                    strata_boundaries[cell_id].values[stratum_id].lower = radius;
                    radius += tower[cell_id].values[stratum_id].stratum_thickness;
                    strata_boundaries[cell_id].values[stratum_id].upper = radius;
                }
                strata_boundaries[cell_id].count = tower[cell_id].count;

                // store displacement for future reference when calculating height of next mixture,
                // if strata is not present or somehow sinks below mixture, set displacement to indicate the height of that mixture
                radius_to_top_of_last_contiguous_strata[cell_id] = std::max(radius - radius_to_top_of_last_mixture, 0.0f);
            }
        }
    }

}
