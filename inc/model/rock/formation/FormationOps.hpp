#pragma once

// C libraries
#include <cmath>
#include <cstddef>   // size_t

// std libraries
#include <vector>

// in-house libraries
#include <model/rock/stratum/StratumOps.hpp>

namespace rock
{

    using bools = std::vector<bool>;

    // NOTE: `M` is mineral count
    template<std::size_t M>
    class FormationOps
    {
        const StratumOps<M> ops;

    public:
        FormationOps():
            ops()
        {}

        // Returns the empty Formation.
        void empty(Formation<M>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = StratumStore<M>();
            }
        }

        // equal to a[i] if mask[i], otherwise empty
        void mask(const Formation<M>& a, const bools& mask, Formation<M>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = mask[i]? a[i] : StratumStore<M>();
            }
        }

        void combine(const Formation<M>& a, const Formation<M>& b, Formation<M>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                ops.combine(a[i], b[i], out[i]);
            }
        }

        // AKA, the identity function.
        void copy(const Formation<M>& a, Formation<M>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = a[i];
            }
        }

        // AKA, the identity function.
        template<typename Grid, typename FormationField>
        void copy(const Grid& grid, const FormationField& a, Formation<M>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = a[i];
            }
        }

        template<typename Grid, typename FormationField>
        void ternary(const Grid& grid, const bools& condition, const Formation<M>& formation, const FormationField& field, Formation<M>& out) const 
        {
            for (std::size_t i(0); i < out.size(); i++)
            {
                out[i] = condition[i]? formation[i] : field(grid, i);
            }
        }

        template<typename Grid, typename FormationField>
        void ternary(const Grid& grid, const bools& condition, const FormationField& field, const Formation<M>& formation, Formation<M>& out) const 
        {
            for (std::size_t i(0); i < out.size(); i++)
            {
                out[i] = condition[i]? field(grid, i) : formation[i];
            }
        }

        void ternary(const bools& condition, const Formation<M>& formation1, const Formation<M>& formation2, Formation<M>& out) const 
        {
            for (std::size_t i(0); i < out.size(); i++)
            {
                out[i] = condition[i]? formation1[i] : formation2[i];
            }
        }

    };

}

