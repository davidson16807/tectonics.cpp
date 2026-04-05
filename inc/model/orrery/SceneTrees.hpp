#pragma once

// std libraries
#include <vector>   // std::vector
#include <algorithm>// std::fill

// 3rd-party libraries
#include <glm/vec3.hpp>          // *vec3

// in-house libraries
#include <index/iterated/Nary.hpp>    // iterated::Identity

#include "TrackPosition.hpp"

namespace orrery
{

    template<typename id, typename scalar, glm::qualifier precision = glm::defaultp>
    class SceneTrees{

        using vec3 = glm::vec<3,scalar,precision>;

        using TrackPositions = std::vector<TrackPosition<id,scalar>>;

        using ids = std::vector<id>;
        using vec3s = std::vector<vec3>;
        using bools = std::vector<bool>;

        const iterated::Identity copy;

    public:

        SceneTrees() 
        {}

        // (Nℝ³)ᵐ(ℝ³)ⁿ → (ℝ³)ⁿ
        // the contents of `updated` after invocation are those of `parent_offsets` merged with `updates`
        void update(
            const TrackPositions& updates,
            const vec3s& parent_offsets,
            vec3s& updated
        ) const {
            if(&updated != &parent_offsets) {
                updated.clear();
                updated.reserve(parent_offsets.size());
                for (std::size_t i = 0; i < parent_offsets.size(); ++i) {
                    updated.emplace_back(parent_offsets[i]);
                }
            }
            for (std::size_t i = 0; i < updates.size(); ++i) {
                updated[updates[i].node] = updates[i].position;
            }
        }

        // the contents of `results` after invocation indicate whether each node in a scene tree
        // is an ancestor of the node whose first ancestor is `parent_id`.
        // nodes have parents given by `parent_ids`
        void is_ancestor(
            const ids& parent_ids,
            const id parent_id,
            bools& results
        ) const {
            std::fill(results.begin(), results.end(), false);
            id ancestor_id(parent_id);
            while(ancestor_id > 0)
            {
                results[ancestor_id] = true;
                ancestor_id = parent_ids[ancestor_id];
            }
            results[ancestor_id] = true;
        }

        // the contents of `results` after invocation are the positions of nodes in a scene tree
        // in a coordinate system where the node given by `origin_id` is the origin.
        // nodes have parents given by `parent_ids` and are offset from their parents by `parent_offsets`.
        template<typename ovec3s> // output vec3s
        void positions(
            const vec3s& parent_offsets,
            const ids& parent_ids,
            const bools& is_origin_ancestor,
            const id origin_id,
            ovec3s& results
        ) const {

            using ovec3 = typename ovec3s::value_type;

            auto size = parent_offsets.size();
            copy(parent_offsets, results);

            id ancestor_id(origin_id);
            vec3 ancestor_offset(0);
            vec3 parent_to_child_offset(0);
            while(ancestor_id > 0)
            {
                parent_to_child_offset = parent_offsets[ancestor_id];
                results[ancestor_id] = ovec3(ancestor_offset);
                ancestor_offset -= parent_to_child_offset;
                ancestor_id = parent_ids[ancestor_id];
            }
            results[ancestor_id] = ovec3(ancestor_offset);

            for (std::size_t i = 0; i < size; ++i)
            {
                results[i] = 
                    is_origin_ancestor[i]? 
                        results[i] 
                      : results[parent_ids[i]] + ovec3(parent_offsets[i]);
            }

        }

    };

}

