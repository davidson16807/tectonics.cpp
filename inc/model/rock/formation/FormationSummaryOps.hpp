#pragma once

// C libraries
#include <cstddef> // std::size_t

// std libraries
#include <vector>  // std::vector

// 3rd party libraries

// in-house libraries
#include <model/rock/stratum/StratumSummaryOps.hpp>

#include "FormationSummary.hpp"

namespace rock
{

    class FormationSummaryOps
    {
        using length = si::length<float>;

        using lengths = std::vector<length>;

        const StratumSummaryOps strata;
    public:
        FormationSummaryOps(const StratumSummaryOps& strata):
            strata(strata)
        {}
        // returns the identity formation
        void empty (FormationSummary& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = StratumSummary();
            }
        }
        void combine (const FormationSummary& a, const FormationSummary& b, FormationSummary& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = strata.combine(a[i],b[i]);
            }
        }
    };

}

