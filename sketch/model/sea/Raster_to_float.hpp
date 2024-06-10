#pragma once

// std libraries
#include <array>   // array

// 3rd party libraries
#include <rasters/entities/Grid/Raster.hpp>

namespace sea
{
    template <typename Tgrid>
    void get_volumes(
        const rasters::Raster<float,Tgrid>& terrain_height, 
        const float sea_height, 
        const float world_radius,
        rasters::Raster<float,Tgrid> volumes
    ){
        series::sub (terrain_height, sea_height,                volumes); // elevation above sea level
        series::min (volumes, 0,                                volumes); // negative ocean depth
        series::mult(volumes, volumes.grid.cache->vertex_areas, volumes); // negative volume, scaled to unit sphere
        series::mult(volumes, -world_radius * world_radius,     volumes); // volumes
    }

    /*
    "solve_max_depth" finds the maximum depth of an ocean using iterative numerical approximation.
    It accepts a raster that stores the height of a terrain relative to its lowest point 
    and a float that indicates the volume of water to be filled along the terrain.
    The model assumes that water will eventually be able to fill every space available 
    and achieve a globally uniform water level.
    */
    template <typename Tgrid>
    float solve_sealevel(
        const rasters::Raster<float,Tgrid>& terrain_height, 
        const float sea_volume, 
        const float world_radius, 
        rasters::Raster<float,Tgrid>& scratch, 
        const unsigned int iterations = 10
    ){
        float surface_area_of_world(terrain_height.grid.cache->total_area * world_radius * world_radius);
        // the depth of the ocean if the entire globe is at a single height
        float uniform_ocean_depth(sea_volume / surface_area_of_world);
        // lowest possible value - the value sealevel takes if all but an infinitesimal point on the globe is at the lowest height observed
        float sealevel_min(series::min(terrain_height) + uniform_ocean_depth);
        // highest possible value - the value sealevel takes if all but an infinitesimal point on the globe is at the highest height observed
        float sealevel_max(series::max(terrain_height) + uniform_ocean_depth);
        // our current guess for sealevel, which we improve iteratively
        float sealevel_guess(0.0f);
        // the volume of the sea, presuming our guess were correct
        float sea_volume_of_guess(0.0f);

        for (unsigned int i = 0; i < iterations; i++) 
        {
            sealevel_guess = (sealevel_max + sealevel_min) / 2;
            get_volumes(terrain_height, sealevel_guess, world_radius, scratch);
            sea_volume_of_guess = series::sum(scratch);
            sealevel_min = sea_volume_of_guess < sea_volume? sealevel_guess : sealevel_min;
            sealevel_max = sea_volume_of_guess > sea_volume? sealevel_guess : sealevel_max;
        }
        return sealevel_guess;
    }
}