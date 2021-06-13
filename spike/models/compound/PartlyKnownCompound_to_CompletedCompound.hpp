#pragma once

// C libraries
#include "math.h"

// std libraries
#include <vector>

// 3rd party libraries
#include "CompletedCompound.hpp"
#include "PartlyKnownCompound.hpp"
#include "StateParameterSamples.hpp"

namespace compound
{
    /*
    Set the values of unknown properties to match those of a default.
    This is an overload that accepts a `CompletedCompound` as the default. 
    Since all properties of a `CompletedCompound` are known by definition,
    a `CompletedCompound` object will result.
    In effect, the `CompletedCompound` is underwriting for any values that the 
    `PartlyKnownCompound` does not provide.
    */
    CompletedCompound complete(
        const PartlyKnownCompound& known,
        const CompletedCompound& fallback
    ){
        StateParameterSamples samples(known);
        std::vector<phase::CompletedSolid> solids;
        if (fallback.solids.size() > 0)
        {
            for (std::size_t i = 0; i < known.solids.size(); i++)
            {
                solids.push_back(phase::complete(known.solids[i], fallback.solids[0]));
            }
        }
        CompletedCompound compound(
            known.molar_mass,
            known.atoms_per_molecule,
            known.molecular_diameter,
            field::complete(known.molecular_degrees_of_freedom,       fallback.molecular_degrees_of_freedom       ),
            field::complete(known.acentric_factor,                    fallback.acentric_factor                    ),
            known.critical_point_pressure,
            known.critical_point_volume,
            known.critical_point_temperature,
            known.critical_point_compressibility,
            field::complete(known.latent_heat_of_vaporization,        fallback.latent_heat_of_vaporization        ),
            field::complete(known.latent_heat_of_fusion,              fallback.latent_heat_of_fusion              ),
            field::complete(known.triple_point_pressure,              fallback.triple_point_pressure              ),
            field::complete(known.triple_point_temperature,           fallback.triple_point_temperature           ),
            field::complete(known.freezing_point_sample_pressure,     samples.melting.pressure                    ),
            field::complete(known.freezing_point_sample_temperature,  samples.melting.temperature                 ),
            field::complete(known.boiling_point_sample_pressure,      samples.boiling.pressure                    ),
            field::complete(known.boiling_point_sample_temperature,   samples.boiling.temperature                 ),
            known.simon_glatzel_slope(),
            known.simon_glatzel_exponent(),
            field::complete(known.molecular_absorption_cross_section, fallback.molecular_absorption_cross_section ),
            phase::complete(known.gas,                                fallback.gas    ),
            phase::complete(known.liquid,                             fallback.liquid ),
            solids
        );


        return compound;
    }
}