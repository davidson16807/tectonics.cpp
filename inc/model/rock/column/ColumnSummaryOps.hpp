#pragma once

#include <model/rock/stratum/StratumSummaryOps.hpp>

#include "ColumnSummary.hpp"

namespace rock{

    class ColumnSummaryOps
    {
        using length = si::length<float>;

        const StratumSummaryOps ops;
        const length threshold;

        inline bool exists(const StratumSummary& stratum) const
        {
            return stratum.thickness() > threshold;
        }

        inline bool above(const StratumSummary& top, const StratumSummary& bottom) const
        {
            return exists(top) && (!exists(bottom) || (exists(bottom) && top.density() < bottom.density()));
        }

    public:

        ColumnSummaryOps(const length threshold): 
            ops(),
            threshold(threshold)
        {}

        inline StratumSummary flatten (const ColumnSummary& column) const
        {
            return ops.combine(column.top, column.rest);
        }

        ColumnSummary absorb (const StratumSummary& stratum1, const StratumSummary& stratum2) const
        {
            return above(stratum1, stratum2)?
                ColumnSummary(stratum1, stratum2):
                ColumnSummary(stratum2, stratum1);
        }

        ColumnSummary absorb (const ColumnSummary& column1, const ColumnSummary& column2) const
        {
            return above(column1.top, column2.top)?
                ColumnSummary(column1.top, ops.combine(column1.rest, flatten(column2))):
                ColumnSummary(column2.top, ops.combine(column2.rest, flatten(column1)));
        }

        ColumnSummary absorb (const ColumnSummary& column, const StratumSummary& stratum) const
        {
            return above(column.top, stratum)?
                ColumnSummary(column.top, ops.combine(column.rest, stratum)):
                ColumnSummary(stratum,    flatten(column));
        }

        ColumnSummary absorb (const StratumSummary& stratum, const ColumnSummary& column) const
        {
            return above(column.top, stratum)?
                ColumnSummary(column.top, ops.combine(column.rest, stratum)):
                ColumnSummary(stratum,    flatten(column));
        }

    };

}

