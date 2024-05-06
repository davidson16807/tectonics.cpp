#pragma once

namespace rock{


    class ColumnSummaryTools
    {
        const StratumSummaryTools ops;
        const float threshold;
    public:
        ColumnSummaryTools(const float threshold): 
            ops(StratumSummaryTools()), 
            threshold(threshold)
        {}
        ColumnSummary absorb() (const ColumnSummary& column, const StratumSummary& stratum) const
        {
            return top.density() > stratum.density && stratum.thickness > si::length<float>(threshold)?
                ColumnSummary(stratum, ops.combine(top, rest))
              : ColumnSummary(top, ops.combine(stratum, rest));
        }
        StratumSummary condense() (const ColumnSummary& column) const
        {
            return ops.combine(column.top, column.rest);
        }
    }

}
