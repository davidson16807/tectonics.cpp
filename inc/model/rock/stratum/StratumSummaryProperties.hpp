#pragma once

// in house libraries
#include "StratumSummary.hpp"

namespace rock{

    class StratumSummaryIsostaticDisplacement
    {
        const si::density<float> mantle_density;
        const si::area_density<float> max_crust_area_density;
    public:
        StratumSummaryIsostaticDisplacement(
            const si::density<float> mantle_density,
            const si::area_density<float> max_crust_area_density
        ):
            mantle_density(mantle_density),
            max_crust_area_density(max_crust_area_density)
        {}
        si::length<float> operator()(
            const StratumSummary& summary
        ) const {
            auto crust_area_density = summary.area_density();
            auto mantle_area_density = max_crust_area_density - crust_area_density;
            return mantle_area_density / mantle_density + summary.thickness();
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

    struct StratumSummaryThickness
    {
        StratumSummaryThickness()
        {}
        auto operator()(const StratumSummary& summary) const
        {
            return summary.thickness();
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

