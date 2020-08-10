#pragma once

#include <many/types.hpp>         // floats, etc.
#include <rasters/SpheroidGrid.hpp>        // SpheroidGrid
#include <rasters/LayeredSpheroidGrid.hpp> // LayeredSpheroidGrid

#include <models/crust/Crust.hpp> // Crust

namespace crust
{
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
}
