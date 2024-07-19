#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries
#include <model/rock/stratum/StratumSummaryTools.hpp>

#include "FormationSummary.hpp"

namespace rock
{

    class FormationSummaryTools
    {
        using length = si::length<float>;

        using lengths = std::vector<length>;

        const StratumSummaryTools strata;
    public:
        FormationSummaryTools(const StratumSummaryTools& strata):
            strata(strata)
        {}
        void combine (const FormationSummary& a, const FormationSummary& b, FormationSummary& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = strata.combine(a[i],b[i]);
            }
        }

        // `isostatic_displacement` returns displacement using an isostatic model
        void isostatic_displacement(const FormationSummary& summary, lengths& out) const
        {
            for (auto i = 0*out.size(); i < out.size(); ++i)
            {
                out[i] = strata.isostatic_displacement(summary[i]);
            }
        }
    };

}

