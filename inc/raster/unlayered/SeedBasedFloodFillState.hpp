#pragma once

// standard libraries
#include <deque> // std::deque

// in-house libraries
#include <index/iterated/Nary.hpp>

namespace unlayered
{

    /*
    properties used:
        arrow_target_vertex_id
    */

    template<typename id>
    struct SeedBasedFloodFillState
    {
        std::deque<id> frontier;
        std::vector<bool> is_considered;
        std::vector<bool> is_included;
        SeedBasedFloodFillState(const id seed_id, const id vertex_count):
            frontier(),
            is_considered(vertex_count, true),
            is_included(vertex_count, false)
        {
            frontier.push_back(seed_id);
            is_included[seed_id] = true;
        }
        SeedBasedFloodFillState(const id seed_id, const std::vector<bool>& is_considered):
            frontier(),
            is_considered(is_considered),
            is_included(is_considered.size(), false)
        {
            is_included[seed_id] = true;
        }
        SeedBasedFloodFillState(const SeedBasedFloodFillState<id>& state):
            frontier(state.frontier),
            is_considered(state.is_considered),
            is_included(state.is_included)
        {}
        SeedBasedFloodFillState<id>& operator=(const SeedBasedFloodFillState<id>& state)
        {
            state.frontier = state.frontier;
            state.is_considered = state.is_considered;
            state.is_included = state.is_included;
            return *this;
        }
    };

    template<typename id, typename IsSimilar>
    struct SeedBasedFloodFilling
    {
        const IsSimilar is_similar;
        const iterated::Identity copy;

        SeedBasedFloodFilling(const IsSimilar is_similar):
            is_similar(is_similar),
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
        template<typename Grid, typename Raster>
        void advance(const Grid& grid, const Raster& raster, SeedBasedFloodFillState<id>& state_io) const {
            const id N(grid.arrows_per_vertex);
            id cell_id(0);
            id neighbor_id(0);
            id j;
            if(state_io.frontier.size() > 0){ // and while region is not too big
                cell_id = state_io.frontier.front();
                state_io.frontier.pop_front();
                for (j = 0; j < N; ++j)
                {
                    neighbor_id = grid.arrow_target_id(cell_id,j);
                    if (state_io.is_considered[neighbor_id] && !state_io.is_included[neighbor_id])
                    {
                        if (is_similar(
                                grid.vertex_position(cell_id), raster[cell_id], 
                                grid.vertex_position(neighbor_id), raster[neighbor_id]))
                        {
                            // add neighbor to region
                            state_io.is_included[neighbor_id] = 1;
                            state_io.frontier.push_back(neighbor_id);
                        }
                    }
                }
                state_io.is_considered[cell_id] = 0;
            }

        }

    };

    template<typename id, typename IsSimilar>
    SeedBasedFloodFilling<id,IsSimilar> edge_based_flood_filling(const IsSimilar is_similar)
    {
        return SeedBasedFloodFilling<id,IsSimilar>(is_similar);
    }

}

