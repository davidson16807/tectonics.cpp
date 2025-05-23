#pragma once

// in house libraries
#include "FormationSummary.hpp"

namespace rock{

    /*
    `FormationSummaryIsostaticDisplacement` returns displacement from the base of the crust using an Airy isostatic model.
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
    See FormationSummaryIsostaticDisplacement.svg for illustration
    */
    class FormationSummaryAiryIsostaticDisplacement
    {
        using area_density = si::area_density<float>;
        using length = si::length<float>;
        using lengths = std::vector<length>;

        const si::density<float> mantle_density;
    public:
        FormationSummaryAiryIsostaticDisplacement(
            const si::density<float> mantle_density
        ):
            mantle_density(mantle_density)
        {}
        void operator()(
            const FormationSummary& summary,
            lengths& out
        ) const {
            area_density max_crust_area_density(0);
            area_density stratum_area_density(0);
            for (std::size_t i = 0; i < summary.size(); ++i)
            {
                stratum_area_density = summary[i].area_density();
                max_crust_area_density = stratum_area_density > max_crust_area_density? stratum_area_density : max_crust_area_density;
            }
            area_density mantle_area_density(0);
            for (std::size_t i = 0; i < summary.size(); ++i)
            {
                mantle_area_density = max_crust_area_density - summary[i].area_density();
                out[i] = mantle_area_density / mantle_density + summary[i].thickness();
            }
        }
    };

}