#pragma once

// in-house libraries
#include <models/compound/field/state/OptionalStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField.hpp>
#include <models/compound/field/constant/OptionalConstantField.hpp>

#include <models/compound/relation/SplineRelation.hpp>

namespace compound { 
namespace phase { 
    struct PartlyKnownSolid
    {
        field::OptionalStateField<si::specific_heat_capacity> isobaric_specific_heat_capacity;
        field::OptionalStateField<si::thermal_conductivity> thermal_conductivity;
        field::OptionalStateField<si::dynamic_viscosity> dynamic_viscosity;
        field::OptionalStateField<si::density> density;
        field::OptionalStateField<si::pressure> vapor_pressure;
        field::OptionalSpectralField<double> refractive_index;
        field::OptionalSpectralField<double> extinction_coefficient;
        field::OptionalSpectralField<si::attenuation> absorption_coefficient;

        field::OptionalStateField<si::pressure> bulk_modulus;     // K   bulk modulus
        field::OptionalStateField<si::pressure> tensile_modulus;  // E   tensile Modulus, Young's modulus, modulus of elasticity
        field::OptionalStateField<si::pressure> shear_modulus;    // G,μ shear modulus, dynamic viscosity, or modulus of rigidity
        field::OptionalStateField<si::pressure> pwave_modulus;    // M   P-wave modulus
        field::OptionalStateField<si::pressure> lame_parameter;   // λ   Lamé's first parameter
        field::OptionalStateField<double> poisson_ratio;          // ν   Poisson's ratio
        
        field::OptionalStateField<si::pressure> compressive_fracture_strength;
        field::OptionalStateField<si::pressure> tensile_fracture_strength;
        field::OptionalStateField<si::pressure> shear_fracture_strength;
        field::OptionalStateField<si::pressure> compressive_yield_strength;
        field::OptionalStateField<si::pressure> tensile_yield_strength;
        field::OptionalStateField<si::pressure> shear_yield_strength;
        
        field::OptionalConstantField<double> chemical_susceptibility_estimate;

        /*
        Return a `PartlyKnownLiquid` that has the properties of `known` where present, otherwise substitute with properties of `base`
        */
        PartlyKnownSolid value_or(const PartlyKnownSolid& fallback)  const {
            // copy what you do know
            PartlyKnownSolid guess = *this;

            guess.isobaric_specific_heat_capacity = isobaric_specific_heat_capacity .value_or(fallback.isobaric_specific_heat_capacity);
            guess.vapor_pressure                  = vapor_pressure                  .value_or(fallback.vapor_pressure);
            guess.thermal_conductivity            = thermal_conductivity            .value_or(fallback.thermal_conductivity);
            guess.dynamic_viscosity               = dynamic_viscosity               .value_or(fallback.dynamic_viscosity);
            guess.density                         = density                         .value_or(fallback.density);
            guess.refractive_index                = refractive_index                .value_or(fallback.refractive_index);
            guess.extinction_coefficient          = extinction_coefficient          .value_or(fallback.extinction_coefficient);
            guess.absorption_coefficient          = absorption_coefficient          .value_or(fallback.absorption_coefficient);

            guess.bulk_modulus    = bulk_modulus    .value_or(fallback.bulk_modulus   );
            guess.tensile_modulus = tensile_modulus .value_or(fallback.tensile_modulus);
            guess.shear_modulus   = shear_modulus   .value_or(fallback.shear_modulus  );
            guess.pwave_modulus   = pwave_modulus   .value_or(fallback.pwave_modulus  );
            guess.lame_parameter  = lame_parameter  .value_or(fallback.lame_parameter );
        
            guess.compressive_fracture_strength = compressive_fracture_strength .value_or(fallback.compressive_fracture_strength);
            guess.tensile_fracture_strength     = tensile_fracture_strength     .value_or(fallback.tensile_fracture_strength    );
            guess.shear_fracture_strength       = shear_fracture_strength       .value_or(fallback.shear_fracture_strength      );
            guess.compressive_yield_strength    = compressive_yield_strength    .value_or(fallback.compressive_yield_strength   );
            guess.tensile_yield_strength        = tensile_yield_strength        .value_or(fallback.tensile_yield_strength       );
            guess.shear_yield_strength          = shear_yield_strength          .value_or(fallback.shear_yield_strength         );

            guess.chemical_susceptibility_estimate = chemical_susceptibility_estimate.value_or(fallback.chemical_susceptibility_estimate);
            return guess;
        }



    };
    /*
    Synonymous to `value_or()`, analogous with similar functions for CompletedSolid
    */
    PartlyKnownSolid value_or(const PartlyKnownSolid& known, const PartlyKnownSolid& fallback)
    {
        return known.value_or(fallback);
    }
}}