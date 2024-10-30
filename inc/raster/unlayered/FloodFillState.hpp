#pragma once

#include <utility> // std::pair

#include "FloodFillCandidateComparison.hpp"

namespace unlayered
{

    template<typename id, typename scalar>
    struct FloodFillState
    {
        using Candidate = std::pair<id,scalar>;
        std::priority_queue<Candidate, std::vector<Candidate>, FloodFillCandidateComparison<id,scalar>> candidates;
        std::vector<bool> is_included;
        // const std::uint8_t plate_id;
        const id seed_id;
        FloodFillState(
            const id seed_id_, 
            // const std::uint8_t plate_id, 
            const id vertex_count
        ):
            candidates(FloodFillCandidateComparison<id,scalar>{}),
            is_included(vertex_count, false),
            // plate_id(plate_id),
            seed_id(seed_id_)
        {
            candidates.emplace(seed_id_, scalar(0));
            is_included[seed_id_] = true;
        }
        FloodFillState(const FloodFillState& state):
            candidates(state.candidates),
            is_included(state.is_included),
            // plate_id(state.plate_id),
            seed_id(state.seed_id)
        {}
    };

}

