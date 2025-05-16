#pragma once

// in house libraries
#include "CrustSummary.hpp"

namespace rock{

    /*
    `CrustSummaryAiryIsostaticDisplacement` returns displacement from the base of the crust using an Airy isostatic model.
    See rock/formation/FormationSummaryAiryIsostaticDisplacement.txt for explanation.
    */
    class CrustSummaryAiryIsostaticDisplacement
    {
        const si::density<float> mantle_density;
    public:
        CrustSummaryAiryIsostaticDisplacement(
            const si::density<float> mantle_density,
        ):
            mantle_density(mantle_density),
        {}
        si::length<float> operator()(
            const CrustSummary& summary,
            lengths& out
        ) const {
            const si::area_density<float> max_crust_area_density(0);
            const si::area_density<float> area_density(0);
            for (int i = 0; i < summary.size(); ++i)
            {
                area_density = summary[i].area_density();
                max_crust_area_density = area_density > max_crust_area_density? area_density : max_crust_area_density;
            }
            const si::area_density<float> mantle_area_density(0);
            for (int i = 0; i < summary.size(); ++i)
            {
                mantle_area_density = max_crust_area_density - summary.area_density();
                out[i] = mantle_area_density / mantle_density + summary.thickness();
            }
        }
    };

}