#pragma once

// C libraries
#include <cmath>
#include <cstddef>

// in-house libraries
#include "../Grid/morphologic.hpp"

#include "LayeredRaster.hpp"

namespace rasters
{
    template <typename Tgrid>
    void dilate(
        const LayeredRaster<bool, Tgrid>& a, 
        LayeredRaster<bool, Tgrid>& out
    ){
        auto& arrow_vertex_id_from = a.grid.cache->arrow_vertex_id_from;
        auto& arrow_vertex_id_to = a.grid.cache->arrow_vertex_id_to;
        const std::size_t L = a.grid.layering->layer_count;
        many::fill( out, false );
        for (std::size_t i = 0; i < arrow_vertex_id_from.size(); ++i)
        {
            for (std::size_t j = 0; j < L; ++j)
            {
                out[arrow_vertex_id_from[i*L+j]] = 
                    out[arrow_vertex_id_from[i*L+j]] || 
                    a  [arrow_vertex_id_from[i*L+j]] || 
                    a  [arrow_vertex_id_to  [i*L+j]] ;
            }
        }
        for (std::size_t i = 0; i < a.grid.cache->vertex_count; ++i)
        {
            for (std::size_t j = 1; j < L; ++j)
            {
                out[i*L+j-1] = out[i*L+j-1] || a[i*L+j] || a[i*L+j-1]; 
                out[i*L+j]   = out[i*L+j]   || a[i*L+j] || a[i*L+j-1]; 
            }
        }
    }

    template <typename Tgrid>
    void erode(
        const LayeredRaster<bool, Tgrid>& a, 
        LayeredRaster<bool, Tgrid>& out
    ){
        auto& arrow_vertex_id_from = a.grid.cache->arrow_vertex_id_from;
        auto& arrow_vertex_id_to = a.grid.cache->arrow_vertex_id_to;
        const std::size_t L = a.grid.layering->layer_count;
        many::fill(out, true);
        for (std::size_t i = 0; i < arrow_vertex_id_from.size(); ++i)
        {
            for (std::size_t j = 0; j < L; ++j)
            {
                out[arrow_vertex_id_from[i*L+j]] = 
                    out[arrow_vertex_id_from[i*L+j]] && 
                    a  [arrow_vertex_id_from[i*L+j]] && 
                    a  [arrow_vertex_id_to  [i*L+j]] ;
            }
        }
        for (std::size_t i = 0; i < a.grid.cache->vertex_count; ++i)
        {
            for (std::size_t j = 1; j < L; ++j)
            {
                out[i*L+j-1] = out[i*L+j-1] && a[i*L+j] && a[i*L+j-1]; 
                out[i*L+j]   = out[i*L+j]   && a[i*L+j] && a[i*L+j-1]; 
            }
        }
    }
}