#pragma once

// in house libraries
#include "StratumSummary.hpp"

namespace rock{

    // `isostatic_displacement` returns displacement using an isostatic model
    class StratumSummaryIsostaticDisplacement
    {
        const si::density<float> mantle_density;

    public:
        StratumSummaryIsostaticDisplacement(const si::density<float> mantle_density):
            mantle_density(mantle_density)
        {}
        auto operator()(const StratumSummary& summary) const
        {
            return summary.thickness() * (1.0f - summary.density()/mantle_density);
        }
    };

    struct StratumSummaryBuoyancyPressure
    {

        using length            = si::length<double>;
        using acceleration      = si::acceleration<double>;
        using density           = si::density<double>;
        using pressure          = si::pressure<double>;

        const acceleration gravity;
        const density mantle_density;

        StratumSummaryBuoyancyPressure(
            const acceleration gravity,
            const density mantle_density
        ):
            gravity(gravity),
            mantle_density(mantle_density)
        {}
        auto operator()(const StratumSummary& summary) const
        {
            auto density_difference = si::max(
                0.0*si::kilogram/si::meter3, 
                density(summary.density() - mantle_density)
            );
            return pressure(
                gravity * density_difference   // Δρ density difference
              * length(summary.thickness()) // V  volume
            );
        }
    };

    struct StratumSummaryDensity
    {
        StratumSummaryDensity()
        {}
        auto operator()(const StratumSummary& summary) const
        {
            return summary.density();
        }
    };

    struct StratumSummaryThickness
    {
        StratumSummaryThickness()
        {}
        auto operator()(const StratumSummary& summary) const
        {
            return summary.thickness();
        }
    };

    struct StratumSummaryAreaDensity
    {
        StratumSummaryAreaDensity()
        {}
        auto operator()(const StratumSummary& summary) const
        {
            return summary.area_density();
        }
    };

}

