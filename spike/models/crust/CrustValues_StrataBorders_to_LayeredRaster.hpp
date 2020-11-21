#pragma once

#include <series/types.hpp>                                 // floats, etc.
#include <rasters/entities/LayeredGrid/LayeredRaster.hpp> // Raster

#include "CrustValues.hpp" // Crust
#include "StrataBorders.hpp" // Crust

namespace crust
{
    template <typename T, std::size_t L1, typename Tgrid1, typename Tgrid2>
    void get_LayeredRaster_from_CrustValues(
        const CrustValues<float, L1,Tgrid1>& strata_values, 
        const StrataBorders<L1,Tgrid1>& strata_boundaries, 
        rasters::LayeredRaster<T, Tgrid2>& layered_raster
    ){
        /*
        LayeredRaster boundaries are regularly spaced, whereas strata boundaries are variable
        This means it is easier to index a LayeredRaster than Strata.
        This is why we iterate through Strata first and only then lookup the raster layer
        */
        assert(strata_values.grid.cache == strata_boundaries.grid.cache);
        assert(strata_values.grid.cache == layered_raster.grid.cache);
        float height_to_upper_boundary(0.0f);
        float height_to_layer(0.0f);
        float layer_height(layered_raster.grid.layering->layer_height);
        std::size_t N(strata_values.grid.cache->vertex_count);
        std::size_t L2(layered_raster.grid.layering->layer_count);
        std::size_t j2 (0);
        for (std::size_t i = 0; i < N; ++i)
        {
            for (std::size_t j1 = 0; j1 < strata_boundaries[i].count; ++j1)
            {
                j2 = (strata_boundaries[i].values[j1].lower) / layer_height;
                height_to_upper_boundary = strata_boundaries[i].values[j1].upper;
                height_to_layer = j2 * layer_height;
                while (height_to_layer < height_to_upper_boundary)
                {
                    // TODO: consider passing an interpolation technique as a lambda
                    layered_raster[i*L2+j2] = strata_values[i].values[j1];
                    height_to_layer += layer_height;
                    j2++;
                }
            }
        }
    }
    template <typename T, std::size_t L1, typename Tgrid1, typename Tgrid2>
    void get_CrustValues_from_LayeredRaster(
        const rasters::LayeredRaster<T, Tgrid2>& layered_raster,
        const StrataBorders<L1,Tgrid1>& strata_boundaries, 
        CrustValues<float, L1,Tgrid1>& strata_values
    ){
        assert(strata_values.grid.cache == strata_boundaries.grid.cache);
        assert(strata_values.grid.cache == layered_raster.grid.cache);
        float layer_height(layered_raster.grid.layering->layer_height);
        std::size_t N(strata_values.grid.cache->vertex_count);
        std::size_t L2(layered_raster.grid.layering->layer_count);
        std::size_t j2 (0);
        for (std::size_t i = 0; i < N; ++i)
        {
            strata_values[i].count = strata_boundaries[i].count;
            for (std::size_t j1 = 0; j1 < strata_boundaries[i].count; ++j1)
            {
                // TODO: consider passing an interpolation technique as a lambda
                j2 = (strata_boundaries[i].values[j1].lower) / layer_height;
                strata_values[i].values[j1] = layered_raster[i*L2+j2];
            }
        }
    }
}
