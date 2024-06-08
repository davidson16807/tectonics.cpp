#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace rock
{

    template<typename FormationSummarization>
    class FormationSummaryOps
    {
        const StratumSummaryTools ops;
    public:
        FormationSummaryOps():
            ops(StratumSummaryTools())
        {}
        void combine (const FormationSummary& a, const FormationSummary& b, FormationSummary& out) const
        {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                out[i] = ops.combine(a[i],b[i]);
            }
        }
    };

}

