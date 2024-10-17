#pragma once

// standard libraries
#include <deque> // std::deque

// in-house libraries
#include <index/iterated/Nary.hpp>

namespace unlayered
{

    template<typename id>
    struct SeedBasedFloodFillState
    {
        std::deque<id> candidates;
        std::vector<bool> is_considered;
        std::vector<bool> is_included;
        const id seed_id;
        SeedBasedFloodFillState(const id seed_id, const id vertex_count):
            candidates(),
            is_considered(vertex_count, true),
            is_included(vertex_count, false),
            seed_id(seed_id)
        {
            candidates.push_back(seed_id);
            is_included[seed_id] = true;
        }
        SeedBasedFloodFillState(const id seed_id, const std::vector<bool>& is_considered):
            candidates(),
            is_considered(is_considered),
            is_included(is_considered.size(), false),
            seed_id(seed_id)
        {
            is_included[seed_id] = true;
        }
        SeedBasedFloodFillState(const SeedBasedFloodFillState<id>& state):
            candidates(state.candidates),
            is_considered(state.is_considered),
            is_included(state.is_included),
            seed_id(seed_id)
        {}
    };

    /*
    properties used:
        arrow_target_vertex_id
    */

    template<typename id, typename IsSimilar>
    struct FloodFilling
    {
        const IsSimilar is_similar;
        const iterated::Identity copy;

        /* VectorFloodFill(
            [](auto U, auto V){return glm::similarity (U,V) > Math.cos(Math.PI * 60/180)}) */
        FloodFilling(const IsSimilar is_similar):
            is_similar(is_similar),
            copy()
        {}

        template<typename Grid, typename Raster, typename State>
        void advance(const Grid& grid, const Raster& raster, State& io) const {

            // copy(procedural::uniform(true), io.is_considered);
            // copy(procedural::uniform(0), io.is_included);
            // io.is_considered[seed_id] = false;

            id cell_id(0);
            id neighbor_id(0);

            const id N = grid.arrows_per_vertex;
            if(io.candidates.size() > 0){
                cell_id = io.candidates.front();
                io.candidates.pop_front();

                if (is_similar(
                        grid.vertex_position(cell_id), raster[cell_id], 
                        grid.vertex_position(io.seed_id), raster[io.seed_id])) {
                    io.is_included[cell_id] = true;

                    for (int j = 0; j < N; ++j)
                    {
                        neighbor_id = grid.arrow_target_id(cell_id,j);
                        if (io.is_considered[neighbor_id]) {
                            io.candidates.push_back(neighbor_id);
                            io.is_considered[neighbor_id] = 0;
                        }
                    }

                }

            }

        }

    };

    template<typename id, typename IsSimilar>
    auto flood_filling(const IsSimilar is_similar) {
        return FloodFilling<id,IsSimilar>(is_similar);
    }

}

