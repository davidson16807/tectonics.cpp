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

        ColumnSummary absorb (const ColumnSummary& column, const StratumSummary& stratum) const
        {
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

        /* 
        `upper_rock_overburden` approximates the overburden pressure that is imparted at the top of a plate by the rest of the rock within `summary`.
        It does not consider pressure that is exerted by fluid above it, such as ocean or atmosphere.
        For cumulative overburden, you will need to calculate the amount of fluid above based upon results from `isostatic_displacement`.
        */
        si::pressure<float> upper_rock_overburden(const ColumnSummary& summary, const int plate_id) const 
        {
            /* 
            ColumnSummary is a crude approximation since it is subject to out-of-order traversals and therefore needs to fit in a small footprint.
            Subducted plates are lumped together into a single StratumSummary,
            therefore anything that is not the topmost plate is simply treated as having the overburden of the topmost plate.
            */
            return gravity * summary.is_top(plate_id)? si::area_density<float>(0.0f) : summary.top.area_density();
        }

        /* 
        `lower_rock_overburden` approximates the overburden pressure that is imparted at the bottom of a plate by the rest of the rock within `summary`.
        It does not consider pressure that is exerted by fluid above it, such as ocean or atmosphere.
        For cumulative overburden, you will need to calculate the amount of fluid above based upon results from `isostatic_displacement`.
        */
        si::pressure<float> lower_rock_overburden(const ColumnSummary& summary, const int plate_id) const 
        {
            /* 
            ColumnSummary is a crude approximation since it is subject to out-of-order traversals and therefore needs to fit in a small footprint.
            Subducted plates are lumped together into a single StratumSummary,
            therefore anything that is not the topmost plate is simply treated as having the overburden of the topmost plate.
            */
            return gravity * summary.is_top(plate_id)? top.area_density() : summary.area_density();
        }

    }

}

