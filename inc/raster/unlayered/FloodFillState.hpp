#pragma once

#include <utility> // std::pair

#include "FloodFillCandidateComparison.hpp"

namespace unlayered
{

    template<typename id, typename scalar>
    struct FloodFillState
    {

        using Candidate = std::pair<id,scalar>;
        using Queue = std::priority_queue<Candidate, std::vector<Candidate>, FloodFillCandidateComparison<id,scalar>>;

        Queue candidates;
        std::vector<bool> is_included;
        // std::uint8_t plate_id;
        id seed_id;
        FloodFillState(
            const id seed_id_, 
            // const std::uint8_t plate_id_, 
            const id vertex_count
        ):
            candidates(FloodFillCandidateComparison<id,scalar>{}),
            is_included(vertex_count, false),
            // plate_id(plate_id_),
            seed_id(seed_id_)
        {
    std::cout << "8a" << std::endl;
            candidates.emplace(seed_id_, scalar(0));
    std::cout << "8b" << std::endl;
            is_included[seed_id_] = true;
    std::cout << "8c" << std::endl;
        }
        FloodFillState(const FloodFillState& state):
            candidates(state.candidates),
            is_included(state.is_included),
            // plate_id(state.plate_id),
            seed_id(state.seed_id)
        {
    std::cout << "8d " << std::to_string(state.candidates.size()) << std::endl;
        }
        void reset(
            const id seed_id_
            // const std::uint8_t plate_id_
        )
        {
            candidates = Queue(FloodFillCandidateComparison<id,scalar>{});
            candidates.emplace(seed_id_, scalar(0));
            std::fill(is_included.begin(), is_included.end(), false);
            // plate_id = plate_id_;
            seed_id = seed_id_;
            is_included[seed_id_] = true;
        }
    };

}

