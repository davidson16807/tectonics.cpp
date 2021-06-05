#pragma once

// C libraries
#include "math.h"

// 3rd party libraries
#include "CompletedCompound.hpp"
#include "PartlyKnownCompound.hpp"

namespace matter
{
    /*
    Set the values of unknown properties to match those of a default.
    This is an overload that accepts a `CompletedCompound` as the default. 
    Since all properties of a `CompletedCompound` are known by definition,
    a `CompletedCompound` object will result.
    In effect, the `CompletedCompound` is underwriting for any values that the 
    `PartlyKnownCompound` does not provide.
    */
    void coalesce(
        const PartlyKnownCompound& known,
        const CompletedCompound& base,
        CompletedCompound& guess
    ){
        // copy known values from base
        guess = base;

        // by this point we can guarantee that `guess` has all the values needed to represent a valid compound
        // from this point on we merely refine our estimates

        // copy over attributes that CompletedCompound represents as `std::optional`s 
        // CompletedCompound is able to handle missing values for these attributes
        guess.critical_point_pressure    = known.critical_point_pressure;
        guess.critical_point_temperature = known.critical_point_temperature;
        guess.simon_glatzel_slope        = known.simon_glatzel_slope;
        guess.simon_glatzel_exponent     = known.simon_glatzel_exponent;

        // copy known values where present or set values to defaults from a template
        guess.triple_point_pressure             = coalesce(known.triple_point_pressure,             base.triple_point_pressure);
        guess.triple_point_temperature          = coalesce(known.triple_point_temperature,          base.triple_point_temperature);
        guess.freezing_point_sample_pressure    = coalesce(known.freezing_point_sample_pressure,    base.freezing_point_sample_pressure);
        guess.freezing_point_sample_temperature = coalesce(known.freezing_point_sample_temperature, base.freezing_point_sample_temperature);
        guess.melting_point_sample_pressure     = coalesce(known.melting_point_sample_pressure,     base.melting_point_sample_pressure);
        guess.melting_point_sample_temperature  = coalesce(known.melting_point_sample_temperature,  base.melting_point_sample_temperature);
        guess.latent_heat_of_vaporization       = coalesce(known.latent_heat_of_vaporization,       base.latent_heat_of_vaporization      );
        guess.latent_heat_of_evaporation        = coalesce(known.latent_heat_of_evaporation,        base.latent_heat_of_evaporation      );
        guess.thermal_conductivity_as_liquid    = coalesce(known.thermal_conductivity_as_gas,       base.thermal_conductivity_as_gas);
        guess.thermal_conductivity_as_liquid    = coalesce(known.thermal_conductivity_as_liquid,    base.thermal_conductivity_as_liquid);
        guess.thermal_conductivity_as_solid     = coalesce(known.thermal_conductivity_as_solid,     base.thermal_conductivity_as_solid);
        guess.atoms_per_molecule                = coalesce(known.atoms_per_molecule,                base.atoms_per_molecule)

    }
}