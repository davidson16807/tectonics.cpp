#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <series/types.hpp>
#include <series/glm/types.hpp>
#include <series/glm/random.hpp>

#include <meshes/mesh.hpp>

#include <rasters/entities/Grid/Raster.hpp>
#include <rasters/entities/Grid/convenience.hpp>
#include <rasters/entities/Grid/operators.hpp>

#include "LayeredGrid.hpp"
#include "LayeredRaster.hpp"
#include "layering.hpp"

#include <rasters/entities/Grid/Grid_test_utils.hpp>

rasters::LayeredGrid<unsigned int,float> layered_diamond_grid(diamond_grid, 1.0f, 0.0f, 2u);

rasters::LayeredGrid<unsigned int,float> layered_tetrahedron_grid(tetrahedron_grid, 1.0f, 0.0f, 2u);

rasters::LayeredGrid<unsigned int,float> layered_nonspheroid_octahedron_grid(nonspheroid_octahedron_grid, 1.0f, 0.0f, 2u);

rasters::LayeredGrid<unsigned int,float> layered_nonspheroid_icosahedron_grid(nonspheroid_icosahedron_grid, 1.0f, 0.0f, 2u);

template <typename Tid, typename Tgrid, typename Tgenerator>
rasters::LayeredRaster<float,rasters::LayeredGrid<Tid,Tgrid>> get_random_LayeredRaster(rasters::LayeredGrid<Tid,Tgrid> layered_grid, Tgenerator generator){
    rasters::Grid<Tid,Tgrid> grid(layered_grid);

    auto a_flat      = rasters::make_Raster<float>(grid);
    series::get_elias_noise(grid.metrics->vertex_positions, generator, a_flat);

    auto a_offset    = rasters::make_Raster<float>(grid);
    series::get_elias_noise(grid.metrics->vertex_positions, generator, a_offset);

    // offset the middle layer to test vector calculus operations across layers
    auto a           = rasters::make_LayeredRaster<float>(layered_grid);
    rasters::repeat_layers(a_flat, a);
    rasters::set_layer(a, 1, a_flat+a_offset, a);

    return a;
}

template <typename Tid, typename Tgrid, typename Tgenerator>
rasters::LayeredRaster<glm::vec3, rasters::LayeredGrid<Tid,Tgrid>> get_random_vector_LayeredRaster(rasters::LayeredGrid<Tid,Tgrid> layered_grid, Tgenerator generator)
{
    auto u = rasters::make_LayeredRaster<glm::vec3>(layered_grid);

    series::set_x(u, get_random_LayeredRaster(layered_grid, generator), u);
    series::set_y(u, get_random_LayeredRaster(layered_grid, generator), u);
    series::set_z(u, get_random_LayeredRaster(layered_grid, generator), u);

    return u;
}