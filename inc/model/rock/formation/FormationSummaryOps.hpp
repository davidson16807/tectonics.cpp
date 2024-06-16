#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries
#include <model/rock/stratum/StratumSummaryTools.hpp>

#include "FormationSummary.hpp"

namespace rock
{

    class FormationSummaryOps
    {
        const StratumSummaryTools strata;
    public:
        FormationSummaryOps():
            strata(StratumSummaryTools())
        {}
        void combine (const FormationSummary& a, const FormationSummary& b, FormationSummary& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = strata.combine(a[i],b[i]);
            }
        }

        // `isostatic_displacement` returns displacement using an isostatic model
        void isostatic_displacement(const FormationSummary& summary, std::vector<si::length<float>> out) const
        {
            for (auto i = 0*out.size(); i < out.size(); ++i)
            {
                out[i] = ops.isostatic_displacement(summary[i]);
            }
        }
    };

}

