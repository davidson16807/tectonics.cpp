#pragma once

// std libraries
#include <vector>   // std::vector
#include <algorithm>// std::fill

// 3rd-party libraries
#include <glm/vec3.hpp>          // *vec3

// in-house libraries
#include <index/iterated/Nary.hpp>    // iterated::Identity

#include "EntityComponents.hpp"

namespace orrery
{

    template<typename id, typename scalar, glm::qualifier precision = glm::defaultp>
    class SceneTrees{

        using vec3 = glm::vec<3,scalar,precision>;
        using vec4 = glm::vec<4,scalar,precision>;
        using mat4 = glm::mat<4,4,scalar,precision>;

        using TrackPositions = EntityComponents<int,vec3>;

        using bools = std::vector<bool>;
        using ids = std::vector<id>;
        using vec3s = std::vector<vec3>;
        using mat4s = std::vector<mat4>;

        const iterated::Identity copy;

    public:

        SceneTrees() 
        {}

        // (Nℝ³)ᵐ(ℝ³)ⁿ → (ℝ³)ⁿ
        // the contents of `updated` after invocation are those of `child_from_parent` merged with `updates`
        void update(
            const TrackPositions& updates,
            const vec3s& child_from_parent,
            vec3s& updated
        ) const {
            if(&updated != &child_from_parent) {
                updated.clear();
                updated.reserve(child_from_parent.size());
                for (std::size_t i = 0; i < child_from_parent.size(); ++i) {
                    updated.emplace_back(child_from_parent[i]);
                }
            }
            for (std::size_t i = 0; i < updates.size(); ++i) {
                updated[updates.entity_for_index(i)] = updates.component_for_index(i);
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
        // nodes have parents given by `parent_ids` and are offset from their parents by `child_from_parent`.
        template<typename ovec3s> // output vec3s
        void offsets_from_origin(
            const vec3s& child_from_parent,
            const ids& parent_ids,
            const bools& is_origin_ancestor,
            const id origin_id,
            ovec3s& results
        ) const {

            using ovec3 = typename ovec3s::value_type;

            auto size = child_from_parent.size();
            copy(child_from_parent, results);

            id ancestor_id(origin_id);
            vec3 ancestor_offset(0);
            vec3 parent_to_child_offset(0);
            while(ancestor_id > 0)
            {
                parent_to_child_offset = child_from_parent[ancestor_id];
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
                      : results[parent_ids[i]] + ovec3(child_from_parent[i]);
            } 

        }

        // the contents of `results` after invocation are transformations that map 
        // the fixed coordinate system for each node in a scene tree
        // to an inertial coordinate system for the node given by `reference_id`.
        void inertial_reference_for_fixed_frames(
            const vec3s& offsets_from_origin_,
            const mat4s& inertial_for_fixed_,
            const id reference_id,
            const mat4s& results
        ) const {
            for (std::size_t i = 0; i < inertial_for_fixed_.size(); i++)
            {
                results[i] = inertial_for_fixed_[i];
                results[i][3] = vec4(offsets_from_origin_[i],1);
            }
        }

    };

}

