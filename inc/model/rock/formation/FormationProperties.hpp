#pragma once

#include <algorithm> // std::max

// in house libraries
#include "Formation.hpp"

namespace rock{

    // NOTE: `M` is mineral count
    template<int M>
    class FormationMass
    {

        using mass = si::mass<float>;
        using masses = std::vector<mass>;

    public:
        FormationMass(){}

        // AKA, the identity function.
        void operator(const Formation<M>& formation, masses& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] += formation[j][i].mass();
            }
        }

        mass operator(const Formation<M>& formation) const {
            mass out(0);
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out += formation[i].mass();
            }
            return out;
        }

    };

}

