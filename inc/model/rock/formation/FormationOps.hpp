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
        const StratumOps<M> ops;

    public:
        FormationOps(const StratumOps<M>& ops):
            ops(ops)
        {}

        // Returns the empty Formation.
        void empty(Formation<M>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = StratumStore<M>();
            }
        }

        void combine(const Formation<M>& a, const Formation<M>& b, Formation<M>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                ops.combine(a[i], b[i], out[i]);
            }
        }

    };

}

