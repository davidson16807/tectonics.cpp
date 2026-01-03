#pragma once

// C libraries
#include <cstddef>   // std::size_t

// std libraries
#include <vector>    // std::vector
#include <algorithm> // std::max

// in house libraries
#include "Crust.hpp"

namespace rock{

    // NOTE: `M` is mineral count, `F` is formation count
    template <std::size_t M, std::size_t F>
    class CrustMass
    {

        using mass = si::mass<float>;
        using masses = std::vector<mass>;

    public:
        CrustMass(){}

        // AKA, the identity function.
        void operator()(const Crust<M,F>& crust, masses& out) const {
            for (std::size_t j = 0; j < F; ++j)
            {
                for (std::size_t i = 0; i < out.size(); ++i)
                {
                    out[i] += crust[j][i].mass();
                }
            }
        }

        mass operator()(const Crust<M,F>& crust) const {
            mass out(0);
            for (std::size_t j = 0; j < F; ++j)
            {
                for (std::size_t i = 0; i < crust[j].size(); ++i)
                {
                    out += crust[j][i].mass();
                }
            }
            return out;
        }

    };

}

