#pragma once

#include "../LayeredGrid/LayeredRaster.hpp"

namespace rasters
{
    template<typename T, typename Tgrid1, typename Tgrid2, rasters::mapping Tmap>
    void nearest_neighbor_interpolation(
        const LayeredRaster<T, Tgrid1, Tmap>& input, 
        LayeredRaster<T, Tgrid2, Tmap>& output,
        Raster<unsigned int, Tgrid2, Tmap>& scratch
    ) {
        // TODO: relax this assertion to work for arbitrary combinations of layer schemes
        assert(*input.grid.layering == *output.grid.layering);
        input.grid.voronoi->get_values(output.grid.cache->vertex_positions, scratch);
        int V = output.grid.cache->vertex_count;
        int L = output.grid.layering->layer_count;
        for (int i = 0; i < V; ++i)
        {
            for (int j = 0; j < L; ++j)
            {
                output[i*L+j] = input[scratch[i]*L+j];
            }
        }
    }

    template<typename T, typename Tgrid1, typename Tgrid2, rasters::mapping Tmap>
    void nearest_neighbor_interpolation(
        const LayeredRaster<T, Tgrid1, Tmap>& input, 
        LayeredRaster<T, Tgrid2, Tmap>& output
    ) {
        auto scratch = make_Raster<unsigned int>(output.grid);
        nearest_neighbor_interpolation(input, output, scratch);
    }
}