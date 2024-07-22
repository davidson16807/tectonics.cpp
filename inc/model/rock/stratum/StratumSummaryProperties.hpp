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

