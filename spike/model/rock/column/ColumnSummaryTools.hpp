#pragma once

namespace rock{


    class ColumnSummaryTools
    {

        const StratumSummaryTools ops;
        const si::force<float> gravity;
        const si::density<float> fluid_density;
        const float threshold;

    public:

        ColumnSummaryTools(const si::force<float> gravity, const si::density<float> fluid_density, const float threshold): 
            ops(StratumSummaryTools()), 
            gravity(gravity),
            fluid_density(fluid_density),
            threshold(threshold)
        {}

        ColumnSummary combine (const ColumnSummary& column, const StratumSummary& stratum) const
        {
            // NOTE: we'll need to add a `bottom` attribute as well if we plan to model the melting of crust
            return top.density() > stratum.density && stratum.thickness > si::length<float>(threshold)?
                ColumnSummary(stratum, ops.combine(top, rest))
              : ColumnSummary(top, ops.combine(stratum, rest));
        }

        StratumSummary condense (const ColumnSummary& column) const
        {
            return ops.combine(column.top, column.rest);
        }

        // `isostatic_displacement` returns displacement using an isostatic model
        si::length<float> isostatic_displacement(const ColumnSummary& summary) const
        {
            StratumSummary condensed = condense(summary);
            return condensed.thickness() * (1.0f - condensed.density()/fluid_density);
        }

    }

}

