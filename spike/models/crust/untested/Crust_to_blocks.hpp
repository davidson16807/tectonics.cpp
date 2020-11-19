#pragma once

#include <series/types.hpp>         // floats, etc.
#include <rasters/SpheroidGrid.hpp>        // SpheroidGrid
#include <rasters/LayeredSpheroidGrid.hpp> // LayeredSpheroidGrid

#include <models/crust/Crust.hpp> // Crust

namespace crust
{
    template <typename T, typename Tgrid>
    void get_blocks_from_strata(
        const rasters::Raster<StrataValues<T>, Tgrid>& strata_values, 
        const rasters::Raster<StrataValues<float>, Tgrid>& strata_depths, 
        const rasters::Raster<float, Tgrid>& surface_displacement, 
        rasters::LayeredRaster<T, Tgrid>& block_values
    ){
        assert(strata_values.grid.cache == strata_depths.grid.cache);
        assert(strata_values.grid.cache == surface_displacement.grid.cache);
        assert(strata_values.grid.cache == block_values.grid.cache);
        float top_block_height = block_values.grid.layering->profile_top_height;
        for (int i = 0; i < strata_depths.size(); ++i)
        {
            int surface_block_i((top_block_height - surface_displacement[i]) / block_values.grid.layering->layer_height);
            float strata_depth(surface_displacement[i]);
            int strata_i(0);
            int block_i (0);
            for (; strata_i < strata_depths[i].count && 
                   (surface_block_i+block_i) < block_values.grid.cache->layer_count; ++strata_i)
            {
                strata_depth += strata_depths[i].values[strata_i];
                for (; block_i * block_values.grid.cache->layer_height < strata_depth && 
                       (surface_block_i+block_i) < block_values.grid.cache->layer_count; ++block_i)
                {
                    block_values[i*block_values.grid.cache->layer_count + surface_block_i+block_i] = strata_values[i].values[strata_i];
                }
            }
        }
    }
    template <typename T, typename Tgrid>
    void get_strata_from_blocks(
        const rasters::LayeredRaster<T, Tgrid>& block_values, 
        const rasters::Raster<StrataValues<float>, Tgrid>& strata_depths, 
        const rasters::Raster<float, Tgrid>& surface_displacement, 
        rasters::Raster<StrataValues<T>, Tgrid>& strata_values
    ){
        assert(strata_values.grid.cache == strata_depths.grid.cache);
        assert(strata_values.grid.cache == surface_displacement.grid.cache);
        assert(strata_values.grid.cache == block_values.grid.cache);
        for (int i = 0; i < strata_depths.size(); ++i)
        {
            int surface_block_i(surface_displacement[i] / block_values.grid.cache->layer_height);
            int strata_i(0);
            int block_i (0);
            for (; (surface_block_i+block_i) < block_values.grid.cache->layer_count && strata_i < strata_depths[i].count; ++block_i)
            {
                float strata_depth(0);
                for (; strata_depths[i].values[strata_i] < block_i * block_values.grid.cache->layer_height && strata_i < strata_depths[i].count; ++strata_i)
                {
                    strata_depth += strata_depths[i].values[strata_i];
                    strata_values[i].values[strata_i] = block_values[i*block_values.grid.cache->layer_count + surface_block_i+block_i];
                }
            }
        }
    }
}
