#pragma once

#include "../Grid/Raster.hpp"

namespace rasters
{
    template<typename T, typename Tgrid1, typename Tgrid2, rasters::mapping Tmap>
    void nearest_neighbor_interpolation(
        const Raster<T, Tgrid1, Tmap>& input, 
        Raster<T, Tgrid2, Tmap>& output,
        Raster<unsigned int, Tgrid2, Tmap>& scratch
    ) {
        input.grid.voronoi->get_values(output.grid.metrics->vertex_positions, scratch);
        series::get(input, scratch, output);
    }

    template<typename T, typename Tgrid1, typename Tgrid2, rasters::mapping Tmap>
    void nearest_neighbor_interpolation(
        const Raster<T, Tgrid1, Tmap>& input, 
        Raster<T, Tgrid2, Tmap>& output
    ) {
        auto scratch = make_Raster<unsigned int>(output.grid);
        nearest_neighbor_interpolation(input, output, scratch);
    }
}