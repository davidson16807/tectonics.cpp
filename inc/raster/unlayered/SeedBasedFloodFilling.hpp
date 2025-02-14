#pragma once

// standard libraries
#include <queue> // std::priority_queue

// in-house libraries
#include <index/iterated/Nary.hpp>

#include "FloodFillState.hpp"

namespace unlayered
{


    template<typename id, typename scalar, typename IsSimilar, typename Priority>
    struct SeedBasedFloodFilling
    {
        const IsSimilar is_similar;
        const Priority priority;
        const iterated::Identity copy;

        /* VectorFloodFill(
            [](auto U, auto V){return glm::similarity (U,V) > Math.cos(Math.PI * 60/180)}) */
        SeedBasedFloodFilling(
            const IsSimilar is_similar, 
            const Priority& priority
        ):
            is_similar(is_similar),
            priority(priority),
            copy()
        {}

        template<typename Grid, typename Raster, typename Mask>
        void advance(
            const Grid& grid, 
            const Raster& raster, 
            Mask& is_considered, 
            FloodFillState<id,scalar>& io
        ) const {

            int cell_id(0);
            int neighbor_id(0);
            glm::vec3 neighbor_position;

            const int N = grid.arrows_per_vertex;
            if(io.candidates.size() > 0){
                cell_id = io.candidates.top().first;
                io.candidates.pop();

                if (is_similar(
                        grid.vertex_position(cell_id), raster[cell_id], 
                        grid.vertex_position(io.seed_id), raster[io.seed_id])) {
                    io.is_included[cell_id] = true;

                    for (int j = 0; j < N; ++j)
                    {
                        neighbor_id = grid.arrow_target_id(cell_id,j);
                        neighbor_position = grid.vertex_position(neighbor_id);
                        if (is_considered[neighbor_id]) {
                            io.candidates.emplace(neighbor_id, 
                                priority(
                                    grid.vertex_position(cell_id), raster[cell_id], 
                                    grid.vertex_position(io.seed_id), raster[io.seed_id])
                            );
                            is_considered[neighbor_id] = 0;
                        }
                    }

                }

            }

        }

    };

    template<typename id, typename scalar, typename IsSimilar, typename Priority>
    auto seed_based_flood_filling(const IsSimilar is_similar, const Priority priority) {
        return SeedBasedFloodFilling<id,scalar,IsSimilar,Priority>(is_similar, priority);
    }

}
