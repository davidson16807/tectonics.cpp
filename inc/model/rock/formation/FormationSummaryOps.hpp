#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries
#include <model/rock/stratum/StratumSummaryOps.hpp>

namespace rock
{

    class FormationSummaryOps
    {
        const StratumSummaryOps strata;
    public:
        FormationSummaryOps():
            strata(StratumSummaryOps())
        {}
        void combine (const FormationSummary& a, const FormationSummary& b, FormationSummary& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = strata.combine(a[i],b[i]);
            }
        }
    };

}
