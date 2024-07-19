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
        si::length<float> operator()(const StratumSummary& summary) const
        {
            return summary.thickness() * (1.0f - summary.density()/mantle_density);
        }
    };

    struct StratumSummaryThickness
    {
        StratumSummaryThickness()
        {}
        si::length<float> operator()(const StratumSummary& summary) const
        {
            return summary.thickness();
        }
    };

    struct StratumSummaryAreaDensity
    {
        StratumSummaryAreaDensity()
        {}
        si::length<float> operator()(const StratumSummary& summary) const
        {
            return summary.thickness();
        }
    };

}

