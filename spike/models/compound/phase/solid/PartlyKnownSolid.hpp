#pragma once

// in-house libraries
#include <models/compound/field/state/OptionalStateField.hpp>
#include <models/compound/field/spectral/OptionalSpectralField.hpp>
#include <models/compound/field/constant/OptionalConstantField.hpp>

namespace compound { 
namespace phase { 
    struct PartlyKnownSolid
    {
        field::OptionalStateField<si::specific_heat_capacity> specific_heat_capacity;
        field::OptionalStateField<si::thermal_conductivity> thermal_conductivity;
        field::OptionalStateField<si::dynamic_viscosity> dynamic_viscosity;
        field::OptionalStateField<si::density> density;
        field::OptionalStateField<si::pressure> vapor_pressure;
        field::OptionalSpectralField<double> refractive_index;
        field::OptionalSpectralField<double> spectral_reflectance;

        field::OptionalSpectralField<si::pressure> bulk_modulus;     // K   bulk modulus
        field::OptionalSpectralField<si::pressure> tensile_modulus;  // E   tensile Modulus, Young's modulus, modulus of elasticity
        field::OptionalSpectralField<si::pressure> shear_modulus;    // G,μ shear modulus, dynamic viscosity, or modulus of rigidity
        field::OptionalSpectralField<si::pressure> pwave_modulus;    // M   P-wave modulus
        field::OptionalSpectralField<si::pressure> lame_parameter;   // λ   Lamé's first parameter
        field::OptionalSpectralField<double> poisson_ratio;          // ν   Poisson's ratio
        
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

            guess.specific_heat_capacity = specific_heat_capacity .value_or(fallback.specific_heat_capacity);
            guess.vapor_pressure         = vapor_pressure         .value_or(fallback.vapor_pressure);
            guess.thermal_conductivity   = thermal_conductivity   .value_or(fallback.thermal_conductivity);
            guess.dynamic_viscosity      = dynamic_viscosity      .value_or(fallback.dynamic_viscosity);
            guess.density                = density                .value_or(fallback.density);
            guess.refractive_index       = refractive_index       .value_or(fallback.refractive_index);
            guess.spectral_reflectance   = spectral_reflectance   .value_or(fallback.spectral_reflectance);

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

        /*
        Return a `PartlyKnownLiquid` that for each property copies whichever observation offers the most information from the two arguments available.
        If arguments offer different observations with the same amount of information, default to the observation from the first argument.
        */
        PartlyKnownSolid fuse(const PartlyKnownSolid& other)  const {
            // copy what you do know
            PartlyKnownSolid guess = *this;

            guess.specific_heat_capacity = specific_heat_capacity .compare(other.specific_heat_capacity);
            guess.vapor_pressure         = vapor_pressure         .compare(other.vapor_pressure);
            guess.thermal_conductivity   = thermal_conductivity   .compare(other.thermal_conductivity);
            guess.dynamic_viscosity      = dynamic_viscosity      .compare(other.dynamic_viscosity);
            guess.density                = density                .compare(other.density);
            guess.refractive_index       = refractive_index       .compare(other.refractive_index);
            guess.spectral_reflectance   = spectral_reflectance   .compare(other.spectral_reflectance);

            guess.bulk_modulus    = bulk_modulus    .compare(other.bulk_modulus   );
            guess.tensile_modulus = tensile_modulus .compare(other.tensile_modulus);
            guess.shear_modulus   = shear_modulus   .compare(other.shear_modulus  );
            guess.pwave_modulus   = pwave_modulus   .compare(other.pwave_modulus  );
            guess.lame_parameter  = lame_parameter  .compare(other.lame_parameter );
        
            guess.compressive_fracture_strength = compressive_fracture_strength .compare(other.compressive_fracture_strength);
            guess.tensile_fracture_strength     = tensile_fracture_strength     .compare(other.tensile_fracture_strength    );
            guess.shear_fracture_strength       = shear_fracture_strength       .compare(other.shear_fracture_strength      );
            guess.compressive_yield_strength    = compressive_yield_strength    .compare(other.compressive_yield_strength   );
            guess.tensile_yield_strength        = tensile_yield_strength        .compare(other.tensile_yield_strength       );
            guess.shear_yield_strength          = shear_yield_strength          .compare(other.shear_yield_strength         );

            guess.chemical_susceptibility_estimate = chemical_susceptibility_estimate.value_or(other.chemical_susceptibility_estimate);
            return guess;
        }

        bool operator==(const PartlyKnownSolid& other) const
        {
            return 
                specific_heat_capacity == other.specific_heat_capacity &&
                vapor_pressure         == other.vapor_pressure         &&
                thermal_conductivity   == other.thermal_conductivity   &&
                dynamic_viscosity      == other.dynamic_viscosity      &&
                density                == other.density                &&
                refractive_index       == other.refractive_index       &&
                spectral_reflectance   == other.spectral_reflectance   &&

                bulk_modulus           == other.bulk_modulus           &&
                tensile_modulus        == other.tensile_modulus        &&
                shear_modulus          == other.shear_modulus          &&
                pwave_modulus          == other.pwave_modulus          &&
                lame_parameter         == other.lame_parameter         &&

                chemical_susceptibility_estimate == other.chemical_susceptibility_estimate;
        }

        // friend class PartlyKnownSolid;
    };
}}