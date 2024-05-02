#pragma once

// C libraries
#include <cmath>

// std libraries
#include <array>

// in-house libraries
#include <model/rock/stratum/StratumOps.hpp>

namespace rock
{

    template<int M>
    class FormationOps
    {
        const StratumOps<M> strata;

    public:
        FormationOps(const StratumOps& strata):
            strata(strata)
        {}

        void combine(const Formation<M>& a, const Formation<M>& b, Formation<M>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                strata.combine(a[i], b[i], out[i]);
            }
        }

    };

}

