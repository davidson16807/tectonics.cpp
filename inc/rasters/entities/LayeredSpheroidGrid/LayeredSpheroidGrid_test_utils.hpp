#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++

// in-house libraries
#include "LayeredSpheroidGrid.hpp"

#include <rasters/entities/LayeredGrid/LayeredGrid_test_utils.hpp>
#include <rasters/entities/SpheroidGrid/SpheroidGrid_test_utils.hpp>

rasters::LayeredSpheroidGrid<int,float> layered_octahedron_grid(octahedron_grid, 1.0f, 0.0f, 2u);

rasters::LayeredSpheroidGrid<int,float> layered_icosahedron_grid(icosahedron_grid, 1.0f, 0.0f, 2u);

template <typename Tid, typename Tgrid, typename Tgenerator>
rasters::LayeredRaster<float,rasters::LayeredSpheroidGrid<Tid,Tgrid>> get_random_LayeredSpheroidRaster(rasters::LayeredSpheroidGrid<Tid,Tgrid> grid, Tgenerator generator){
	auto flat_grid = rasters::SpheroidGrid<Tid,Tgrid>(grid);

    auto a_flat      = rasters::make_Raster<float>(flat_grid);
    many::get_elias_noise(grid.cache->vertex_positions, generator, a_flat);

    auto a           = rasters::make_LayeredRaster<float>(grid);
    rasters::repeat_layers(a_flat, a);

    // offset the middle layer to test vector calculus operations across layers
    auto a_offset    = rasters::make_Raster<float>(flat_grid);
    for (int i = 0; i < grid.layering->layer_count; ++i)
    {
	    many::get_elias_noise(grid.cache->vertex_positions, generator, a_offset);
	    rasters::set_layer(a, i, a_flat+a_offset, a);
    }

    return a;
}
