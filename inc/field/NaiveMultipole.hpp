#pragma once

#include <vector>

#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include "Monopole.hpp"

namespace field {

    /*
    `NaiveMultipole` represents a mathematical field of arbitrary dimensionality
    where the value at each point is the sum of monopole fields.
    This is to say that the value at each point is the weighted sum of the inverse square of distances to particles.
    `NaiveMultipole` implements a naive O(N²) algorithm and exists primarily to verify correctness of `BarnesHutMultipole`
    You almost certainly want to use `BarnesHutMultipole` unless particle count is very low and accuracy is extremely important.
    */
    template<typename scalar, typename vector>
    class NaiveMultipole {

        std::vector<Monopole<scalar,vector>> monopoles;

    public:

        NaiveMultipole(): monopoles() {}

        void clear() { monopoles.clear(); }

        /*
        `add` modifies the field to also consider a particle of given `position` and `weight`
        */
        void add(const vector& position, const scalar weight) {
            monopoles.emplace_back(position, weight);
        }

        /*
        `operator()` returns the value acting at a given `position` in the field
        */
        [[nodiscard]]
        constexpr inline scalar operator()(const vector& position ) const
        {
            scalar result(0);
            for (const auto& monopole : monopoles) {
                result += monopole.weight / glm::distance2(position, monopole.position);
            }
            return result;
        }

    };

}

