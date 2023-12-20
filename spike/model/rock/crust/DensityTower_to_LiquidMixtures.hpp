#pragma once

// std libraries
#include <set>      // set

// in-house libraries
#include "DensityTower.hpp"  // Crust
#include "LiquidMixture.hpp" // Crust

namespace crust
{
    namespace
    {
        struct LiquidIdDescendingDensityComparator 
        {
            // NOTE: liquid ids are assigned based on density, so no other information about the ids is needed
            bool operator() (const std::uint8_t& lhs, const std::uint8_t& rhs) const
            {
                return lhs>rhs;
            }
        };
    }

    /*
    NOTE: `liquid_ids_below` maps each stratum to 
    the lightest liquid that is dense enough to settle beneath that stratum, if present.
    If we presume the crust is homogenous (that is, represented by only a single `Strata` object)
    then the unique ids in `liquid_ids_below` (stored in `liquid_ids_present`) 
    indicate the lightest possible liquid that can exist within a contiguous set of liquid layers.
    Since liquid ids are assigned based on density, two adjacent ids in `liquid_ids_present` 
    will indicate all the possible liquids within the model that could exist in contiguous layers.
    
    We also presume that if a liquid layer is large enough to be considered by the model,
    it will also be large enough that any small heat gradient across a contiguous set of them
    will create enough convection to mix the liquid layers together over large timescales, 
    so we treat any contiguous set of liquid layers as a single intermixed liquid body.
    An example of such an intermixed liquid body may be the lakes of Titan (composed of series kinds of hydrocarbons),
    or a magma ocean (composed of series kinds of mafic and felsic compounds).
    
    We also apply this concept to situations where the crust is nonhomogeneous, 
    even though it may not strictly apply, since it is an effective approximation 
    that allows us to avoid implementing a more realistic liquid mixture model.
    */
    template <std::size_t L, std::size_t M, typename Tgrid>
    std::vector<LiquidMixture> get_LiquidMixtures(const DensityTower<L,Tgrid>& tower) {
        std::set<unsigned int,LiquidIdDescendingDensityComparator> id_of_lightest_liquid_in_mixed_body;

        // iterate through cells in the raster
        // find a `std::set` containing all unique values within a `CrustValues` object
        std::size_t N = tower.grid.cache->vertex_count;
        for (std::size_t i = 0; i < N; ++i)
        {
            // iterate through strata in cell
            for (std::size_t j = 0; j < tower[i].count; ++j)
            {
                id_of_lightest_liquid_in_mixed_body.emplace(tower[i].values[j].liquid_id_below);
            }
        }

        /*
        Now move it to a format that's easier to work with.
        We do not bother to represent the intermediate `std::set` 
        in our category diagram by creating a separate function for its generation.
        `std::set` is not packed tightly in memory, so it's not performant for any behavior besides insertion,
        the data structure is effectively static so it will not be used for insertion,
        and from experience we know it's much more cumbersome to use than a vector of liquid id ranges.
        We instead choose to encapsulate it within the function.
        */
        std::vector<LiquidMixture> liquid_mixtures(id_of_lightest_liquid_in_mixed_body.size());
        std::uint8_t densest_liquid_id(M-1);
        for (auto i = id_of_lightest_liquid_in_mixed_body.begin(); i != id_of_lightest_liquid_in_mixed_body.end(); ++i)
        {
            std::uint8_t lightest_liquid_id = *i;
            liquid_mixtures.emplace_back(lightest_liquid_id, densest_liquid_id);
            // `lightest_liquid_id` now becomes `densest_liquid_id` for the next contiguous solid layer
            densest_liquid_id = lightest_liquid_id;
        }

        return liquid_mixtures;
    }
}
