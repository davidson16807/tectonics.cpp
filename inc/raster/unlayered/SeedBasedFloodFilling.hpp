#pragma once

// standard libraries
#include <utility> // std::pair
#include <queue> // std::priority_queue

// in-house libraries
#include <index/iterated/Nary.hpp>

namespace unlayered
{

    template<typename id, typename scalar>
    struct FloodFillCandidateComparison
    {
        FloodFillCandidateComparison(){}
        bool operator()(const std::pair<id,scalar> a, const std::pair<id,scalar> b)
        {
            return a.second > b.second;
        }
    };

    template<typename id, typename scalar>
    struct SeedBasedFloodFillState
    {
        using Candidate = std::pair<id,scalar>;
        std::priority_queue<Candidate, std::vector<Candidate>, FloodFillCandidateComparison<id,scalar>> candidates;
        std::vector<bool> is_considered;
        std::vector<bool> is_included;
        const id seed_id;
        SeedBasedFloodFillState(const id seed_id_, const id vertex_count):
            candidates(FloodFillCandidateComparison<id,scalar>{}),
            is_considered(vertex_count, true),
            is_included(vertex_count, false),
            seed_id(seed_id_)
        {
            candidates.emplace(seed_id_, scalar(0));
            is_included[seed_id_] = true;
        }
        SeedBasedFloodFillState(const id seed_id_, const std::vector<bool>& is_considered):
            candidates(FloodFillCandidateComparison<id,scalar>{}),
            is_considered(is_considered),
            is_included(is_considered.size(), false),
            seed_id(seed_id_)
        {
            candidates.emplace(seed_id_, scalar(0));
            is_included[seed_id_] = true;
        }
        SeedBasedFloodFillState(const SeedBasedFloodFillState& state):
            candidates(state.candidates),
            is_considered(state.is_considered),
            is_included(state.is_included),
            seed_id(state.seed_id)
        {}
    };

    /*
    grid attributes used:
        arrow_target_vertex_id
    */

    /*
    properties to satisfy:
    * contiguous (requires floodfill)
    * symmetric with respect to plate seeds
    * cells with largest vectors must occur at boundaries 
    * lowest complexity allowable
    * in-order traversal if possible

    ideas:
    * priority queue for candidates, sorting on seed proximity or vector magnitude
    */

    template<typename id, typename scalar, typename IsSimilar>
    struct SeedBasedFloodFilling
    {
        const IsSimilar is_similar;
        const iterated::Identity copy;

        /* VectorFloodFill(
            [](auto U, auto V){return glm::similarity (U,V) > Math.cos(Math.PI * 60/180)}) */
        SeedBasedFloodFilling(const IsSimilar is_similar):
            is_similar(is_similar),
            copy()
        {}

        template<typename Grid, typename Raster>
        void advance(const Grid& grid, const Raster& raster, SeedBasedFloodFillState<id,scalar>& io) const {

            // copy(procedural::uniform(true), io.is_considered);
            // copy(procedural::uniform(0), io.is_included);
            // io.is_considered[seed_id] = false;

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
                        neighbor_position = grid.vertex_position(grid.arrow_target_id(cell_id,j));
                        if (io.is_considered[neighbor_id]) {
                            io.candidates.emplace(neighbor_id, 
                                glm::distance(
                                    grid.vertex_position(grid.arrow_target_id(cell_id,j)),
                                    grid.vertex_position(io.seed_id)
                            ));
                            io.is_considered[neighbor_id] = 0;
                        }
                    }

                }

            }

        }

    };

    template<typename id, typename scalar, typename IsSimilar>
    auto seed_based_flood_filling(const IsSimilar is_similar) {
        return SeedBasedFloodFilling<id,scalar,IsSimilar>(is_similar);
    }

}

