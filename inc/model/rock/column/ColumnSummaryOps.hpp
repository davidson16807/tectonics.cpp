#pragma once

#include <model/rock/stratum/StratumSummaryOps.hpp>

namespace rock{

    class ColumnSummaryOps
    {
        using length = si::length<float>;

        const StratumSummaryOps ops;
        const length threshold;

    public:

        ColumnSummaryOps(const StratumSummaryOps ops, const length threshold): 
            ops(ops),
            threshold(threshold)
        {}

        ColumnSummary absorb (const ColumnSummary& column, const StratumSummary& stratum) const
        {
            // NOTE: we'll need to add a `bottom` attribute as well if we plan to model the melting of crust
            return stratum.density() < column.top.density() && stratum.thickness() > threshold?
                ColumnSummary(stratum, ops.combine(column.top, column.rest))
              : ColumnSummary(column.top, ops.combine(stratum, column.rest));
        }

        ColumnSummary absorb (const StratumSummary& stratum1, const StratumSummary& stratum2) const
        {
            /* 
            this function is included so that `absorb` can serve as a monoid, 
            but it also serves pragmatic purposes of convenience
            */
            return absorb(ColumnSummary(stratum1), stratum2);
        }

        ColumnSummary absorb (const ColumnSummary& column1, const ColumnSummary& column2) const
        {
            /* 
            this function is included so that `absorb` can serve as a monoid, 
            but it also serves pragmatic purposes of convenience
            */
            return absorb(absorb(column1, column2.top), column2.rest);
        }

        StratumSummary flatten (const ColumnSummary& column) const
        {
            return ops.combine(column.top, column.rest);
        }

    };

}

