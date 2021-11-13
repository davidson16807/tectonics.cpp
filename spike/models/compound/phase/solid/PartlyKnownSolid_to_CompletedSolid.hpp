#pragma once

// in-house libraries
#include "PartlyKnownSolid.hpp"
#include "CompletedSolid.hpp"

#include <models/compound/field/state/OptionalStateField_to_CompletedStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField_to_CompletedSpectralField.hpp>

namespace compound { 
namespace phase { 
    /*
    Populate the missing `*_function` attributes with functions returning representative values if they are available.
    The function is idempotent.
    */
    CompletedSolid complete(const PartlyKnownSolid& known, const CompletedSolid& fallback)
    {
        return CompletedSolid{
            field::complete(known.specific_heat_capacity,           fallback.specific_heat_capacity           ),
            field::complete(known.thermal_conductivity,             fallback.thermal_conductivity             ),
            field::complete(known.dynamic_viscosity,                fallback.dynamic_viscosity                ),
            field::complete(known.density,                          fallback.density                          ),
            field::complete(known.vapor_pressure,                   fallback.vapor_pressure                   ),
            field::complete(known.refractive_index,                 fallback.refractive_index                 ),
            field::complete(known.extinction_coefficient,           fallback.extinction_coefficient           ),
            field::complete(known.spectral_reflectance,             fallback.spectral_reflectance             ),

            field::complete(known.bulk_modulus,                     fallback.bulk_modulus                     ),
            field::complete(known.tensile_modulus,                  fallback.tensile_modulus                  ),
            field::complete(known.shear_modulus,                    fallback.shear_modulus                    ),
            field::complete(known.pwave_modulus,                    fallback.pwave_modulus                    ),
            field::complete(known.lame_parameter,                   fallback.lame_parameter                   ),
            field::complete(known.poisson_ratio,                    fallback.poisson_ratio                    ),

            field::complete(known.compressive_fracture_strength,    fallback.compressive_fracture_strength    ),
            field::complete(known.tensile_fracture_strength,        fallback.tensile_fracture_strength        ),
            field::complete(known.shear_fracture_strength,          fallback.shear_fracture_strength          ),
            field::complete(known.compressive_yield_strength,       fallback.compressive_yield_strength       ),
            field::complete(known.tensile_yield_strength,           fallback.tensile_yield_strength           ),
            field::complete(known.shear_yield_strength,             fallback.shear_yield_strength             ),

            field::complete(known.chemical_susceptibility_estimate, fallback.chemical_susceptibility_estimate )
        };
    }
}}