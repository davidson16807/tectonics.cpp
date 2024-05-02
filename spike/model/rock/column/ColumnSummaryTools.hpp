#pragma once

namespace rock{


    class ColumnSummaryTools
    {
        const StratumSummaryOps ops;
        const float threshold;
    public:
        ColumnSummaryTools(const float threshold): 
            ops(StratumSummaryOps()), 
            threshold(threshold)
        {}
        ColumnSummary absorb() (const ColumnSummary& column, const StratumSummary& stratum) const
        {
            return top.density() > stratum.density && stratum.thickness > si::length<float>(threshold)?
                ColumnSummary(stratum, ops.combine(top, rest))
              : ColumnSummary(top, ops.combine(stratum, rest));
        }
    }

}
