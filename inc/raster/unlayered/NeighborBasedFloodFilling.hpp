#pragma once

// standard libraries
#include <utility> // std::pair
#include <queue> // std::priority_queue

// in-house libraries
#include <index/iterated/Nary.hpp>

#include "FloodFillCandidateComparison.hpp"

namespace unlayered
{

    /*
    properties used:
        arrow_target_vertex_id
    */

    template<typename id, typename scalar>
    struct NeighborBasedFloodFillState
    {
        using Candidate = std::pair<id,scalar>;
        std::priority_queue<Candidate, std::vector<Candidate>, FloodFillCandidateComparison<id,scalar>> candidates;
        std::vector<bool> is_considered;
        std::vector<bool> is_included;
        NeighborBasedFloodFillState(const id seed_id, const id vertex_count):
            candidates(FloodFillCandidateComparison<id,scalar>{}),
            is_considered(vertex_count, true),
            is_included(vertex_count, false)
        {
            candidates.emplace(seed_id, scalar(0));
            is_included[seed_id] = true;
        }
        NeighborBasedFloodFillState(const NeighborBasedFloodFillState<id,scalar>& state):
            candidates(state.candidates),
            is_considered(state.is_considered),
            is_included(state.is_included)
        {}
    };

    template<typename id, typename scalar, typename IsSimilar, typename Priority>
    struct NeighborBasedFloodFilling
    {
        const IsSimilar is_similar;
        const Priority priority;
        const iterated::Identity copy;

        NeighborBasedFloodFilling(
            const IsSimilar& is_similar, 
            const Priority& priority
        ):
            is_similar(is_similar),
            priority(priority),
            copy()
        {}

        /*
        `advance()` needs to offer fine grain control over the implementation of larger flood-fill algorithms that map raster→raster.
        Therefore, instead of mapping raster→raster, it maps state→state, 
        where state stores the interim raster of a larger algorihtm, along with other attributes.
        However because of this use case, `advance()` is intended for use in a larger for loop where performance is critical, 
        so copying input to output would be prohibitive for all intended usage. 
        This is why state stores both input and output. If not for this, `advance()` would be conceptually pure.
        */
        template<typename Grid, typename Raster, typename Mask>
        void advance(
            const Grid& grid, 
            const Raster& raster, 
            Mask& is_considered, 
            NeighborBasedFloodFillState<id,scalar>& io
        ) const {
            const id N(grid.arrows_per_vertex);
            id cell_id(0);
            id neighbor_id(0);
            id j;
            glm::vec3 neighbor_position;
            if(io.candidates.size() > 0){ // and while region is not too big
                cell_id = io.candidates.top().first;
                io.candidates.pop();
                for (j = 0; j < N; ++j)
                {
                    neighbor_id = grid.arrow_target_id(cell_id,j);
                    neighbor_position = grid.vertex_position(neighbor_id);
                    if (is_considered[neighbor_id])
                    {
                        if (is_similar(
                                grid.vertex_position(cell_id), raster[cell_id], 
                                neighbor_position, raster[neighbor_id]))
                        {
                            // add neighbor to region
                            is_considered[neighbor_id] = 0;
                            io.candidates.emplace(neighbor_id, 
                                priority(
                                    grid.vertex_position(cell_id), raster[cell_id], 
                                    neighbor_position, raster[neighbor_id])
                            );
                        }
                    }
                }
                io.is_considered[cell_id] = 0;
            }

        }

    };

    template<typename id, typename scalar, typename IsSimilar, typename Priority>
    NeighborBasedFloodFilling<id,scalar,IsSimilar,Priority> neighbor_based_flood_filling(
        const IsSimilar& is_similar,
        const Priority& priority
    ){
        return NeighborBasedFloodFilling<id,scalar,IsSimilar,Priority>(is_similar);
    }

}

