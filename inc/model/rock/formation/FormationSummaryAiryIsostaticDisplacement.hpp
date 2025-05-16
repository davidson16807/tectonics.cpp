#pragma once

// in house libraries
#include "FormationSummary.hpp"

namespace rock{

    /*
    `FormationSummaryAiryIsostaticDisplacement` returns displacement from the base of the crust using an Airy isostatic model.
    See FormationSummaryAiryIsostaticDisplacement.txt for explanation.
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