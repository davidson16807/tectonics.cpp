#pragma once

// in house libraries
#include "StratumSummary.hpp"

namespace rock{

    /*
    `StratumSummaryIsostaticDisplacement` returns displacement using an Airy isostatic model.
    This model rests on two principles:
    1)  a "column" is composed of solid, liquid, and a lighter fluid that we'll call "gas" here
    2)  gravity is the only force acting on the column, which is presumed constant
    3)  columns are in isostatic equilibrium so pressure on columns is constant throughout the crust
    4)  by 2-3), area density is constant throughout the crust
    5)  pools are ordered by density: liquid > solid > gas
    6)  by 3,5), pools overlay each other in a definite order: liquid ← solid ← gas
    7)  the density of gas is negligeable
    8)  by 6,7), the lowest lying solid always appears in the column with the largest solid area density,
        which we'll call the "largest column". We'll call its area density the `max_crust_area_density`,
        and we'll call the depth of its base the "lowest base",
    9)  by 7), the amount of liquid in a column is whatever needed 
        for the combined area density of pools to match `max_crust_area_density`
    10) the height from the top of a column to the lowest base is whatever height follows from 9)
    See StratumSummaryIsostaticDisplacement.svg for illustration
    */
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

