#pragma once

#include <series/types.hpp>                                 // floats, etc.
#include <rasters/entities/Grid/Raster.hpp>               // Raster
#include <rasters/entities/LayeredGrid/LayeredRaster.hpp> // Raster

#include <models/crust/Crust.hpp> // Crust
#include <models/crust/CrustValues.hpp> // Crust

namespace crust
{
    template <typename T, std::size_t L, typename Tgrid1, typename Tgrid2>
    void get_LayeredRaster_from_CrustValues(
        const CrustValues<float, L,Tgrid1>& strata_values, 
        const StrataBoundaries<L,Tgrid1>& strata_boundaries, 
        const float radius_to_bottom_of_crust,
        rasters::LayeredRaster<T, Tgrid2>& layered_raster
    ){
        /*
        LayeredRaster boundaries are regularly spaced, whereas strata boundaries are variable
        This means it is easier to index a LayeredRaster than Strata.
        This is why we iterate through Strata first and only then lookup the raster layer
        */
        assert(strata_values.grid.cache == strata_boundaries.grid.cache);
        assert(strata_values.grid.cache == layered_raster.grid.cache);
        std::size_t N(strata_values.grid.cache->vertex_count);
        float layer_height(layered_raster.grid.layering->layer_height);
        std::size_t layer_i (0);
        float radius_to_upper_boundary(0.0f);
        float radius_to_layer(0.0f);
        for (std::size_t i = 0; i < N; ++i)
        {
            for (std::size_t j = 0; j < L; ++j)
            {
                layer_i = (strata_boundaries[i].values[j].lower - radius_to_bottom_of_crust) / layer_height;
                radius_to_upper_boundary = strata_boundaries[i].values[j].upper;
                radius_to_layer = layer_i * layer_height + radius_to_bottom_of_crust;
                while (radius_to_layer < radius_to_upper_boundary)
                {
                    // TODO: consider passing an interpolation technique as a lambda
                    layered_raster[i*L+layer_i] = strata_values[i].values[j];
                    radius_to_layer += layer_height;
                    layer_i++;
                }
            }
        }
    }
    template <typename T, std::size_t L, typename Tgrid1, typename Tgrid2>
    void get_CrustValues_from_LayeredRaster(
        const rasters::LayeredRaster<T, Tgrid1>& layered_raster,
        const StrataBoundaries<L,Tgrid2>& strata_boundaries, 
        const float radius_to_bottom_of_crust,
        CrustValues<float, L,Tgrid2>& strata_values
    ){
        assert(strata_values.grid.cache == strata_boundaries.grid.cache);
        assert(strata_values.grid.cache == layered_raster.grid.cache);
        std::size_t N(strata_values.grid.cache->vertex_count);
        float layer_height(layered_raster.grid.layering->layer_height);
        std::size_t layer_i (0);
        float radius_to_upper_boundary(0.0f);
        float radius_to_layer(0.0f);
        for (std::size_t i = 0; i < N; ++i)
        {
            strata_values[i].count = strata_boundaries[i].count;
            for (std::size_t j = 0; j < L; ++j)
            {
                layer_i = (strata_boundaries[i].values[j].lower - radius_to_bottom_of_crust) / layer_height;
                radius_to_upper_boundary = strata_boundaries[i].values[j].upper;
                radius_to_layer = layer_i * layer_height + radius_to_bottom_of_crust;
                while (radius_to_layer < radius_to_upper_boundary)
                {
                    // TODO: consider passing an interpolation technique as a lambda
                    strata_values[i].values[j] = layered_raster[i*L+layer_i];
                    radius_to_layer += layer_height;
                    layer_i++;
                }
            }
        }
    }
}
