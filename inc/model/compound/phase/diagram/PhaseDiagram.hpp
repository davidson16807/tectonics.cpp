#pragma once

#include <units/si.hpp>

#include <model/compound/phase/boundary/LiquidusTemperatureBoundary.hpp>
#include <model/compound/phase/boundary/VaporusTemperatureBoundary.hpp>
#include <model/compound/phase/boundary/LinearTemperatureBoundary.hpp>
#include <model/compound/phase/boundary/LinearPressureBoundary.hpp>
#include <model/compound/phase/region/LinearPhaseRegion.hpp>

namespace compound {
namespace phase {

    struct PhaseDiagram
    {
        std::vector<LinearPhaseRegion> regions;
        LinearPhaseRegion supercriticality;
        VaporusTemperatureBoundary vaporus;
        LiquidusTemperatureBoundary liquidus;
        int default_phase_id;
        PhaseDiagram(
            const LinearPhaseRegion& supercriticality,
            const VaporusTemperatureBoundary& vaporus,
            const LiquidusTemperatureBoundary& liquidus,
            const std::vector<LinearPhaseRegion>& regions,
            const int default_phase_id
        ):
            regions(regions),
            supercriticality(supercriticality),
            vaporus(vaporus),
            liquidus(liquidus),
            default_phase_id(default_phase_id),
        {
        }
        int operator()(const si::pressure<double> p, const si::temperature<double> T)
        {
            if (supercriticality(p,T))
            {
                return phase::supercritical;
            }
            if (T > vaporus(p))
            {
                return phase::vapor;
            }
            if (T > liquidus(p))
            {
                return phase::liquid;
            }
            for (std::size_t i = 0; i < regions.size(); ++i)
            {
                if (regions[i](p,T))
                {
                    return regions[i].phase_id;
                }
            }
            return default_phase_id;
        }
    }


    /*
    `basic_phase` returns the phase of a basic phase diagram given a set of parameters 
    that should be applicable to virtually any solid/liquid/vapor phase diagram.
    It models boiling/sublimation points using the Clapeyron equation,
    and a linear relation for melting points.
    */
    PhaseDiagram basic_phase_diagram(
        const point<double> triple,
        const point<double> critical,
        const point<double> freezing,
        const si::specific_energy<double> latent_heat_of_vaporization_at_boiling_point,
        const si::molar_mass<double> molar_mass
    ){
        return PhaseDiagram(
            supercriticality(critical),
            basic_vaporus(
                latent_heat_of_vaporization_at_boiling_point, 
                molar_mass, critical),
            basic_liquidus(triple, freezing),
            {}
        );
    }


    /*
    `simon_glatzel_phase` returns the phase of a basic phase diagram given a set of parameters 
    that should be applicable to virtually any solid/liquid/vapor phase diagram.
    It models boiling/sublimation points using the Clapeyron equation,
    and melting points using the empirical Simon-Glatzel equation.
    */
    PhaseDiagram simon_glatzel_phase_diagram(
        const point<double> triple,
        const point<double> critical,
        const si::specific_energy<double> latent_heat_of_vaporization_at_boiling_point,
        const si::molar_mass<double> molar_mass,
        const si::pressure<double>   simon_glatzel_scale,  
        const double                 simon_glatzel_exponent
    ){
        return PhaseDiagram(
            supercriticality(critical),
            basic_vaporus(
                latent_heat_of_vaporization_at_boiling_point, 
                molar_mass, critical),
            simon_glatzel_liquidus(
                triple, simon_glatzel_scale, simon_glatzel_exponent),
            {}
        );
    }

}}