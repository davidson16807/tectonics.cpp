#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "PartlyKnownCompound.hpp"
#include "field/missing.hpp"

// test utility libraries
#include <units/si_test_utils.hpp>

#include "field/state/OptionalStateField_test_utils.hpp"
#include "field/spectral/OptionalSpectralField_test_utils.hpp"

#include "phase/gas/PartlyKnownGas_test_utils.hpp"
#include "phase/liquid/PartlyKnownLiquid_test_utils.hpp"
#include "phase/solid/PartlyKnownSolid_test_utils.hpp"

namespace compound 
{
    bool operator==(const compound::PartlyKnownCompound& first, const compound::PartlyKnownCompound& second)
    {
        if(
            first.molar_mass                         != second.molar_mass                         ||
            first.atoms_per_molecule                 != second.atoms_per_molecule                 ||
            first.molecular_diameter                 != second.molecular_diameter                 ||
            first.molecular_degrees_of_freedom       != second.molecular_degrees_of_freedom       ||
            first.acentric_factor                    != second.acentric_factor                    ||
            first.critical_point_pressure            != second.critical_point_pressure            ||
            first.critical_point_volume              != second.critical_point_volume              ||
            first.critical_point_temperature         != second.critical_point_temperature         ||
            first.critical_point_compressibility     != second.critical_point_compressibility     ||
            first.latent_heat_of_vaporization        != second.latent_heat_of_vaporization        ||
            first.latent_heat_of_fusion              != second.latent_heat_of_fusion              ||
            first.triple_point_pressure              != second.triple_point_pressure              ||
            first.triple_point_temperature           != second.triple_point_temperature           ||
            first.freezing_point_sample_pressure     != second.freezing_point_sample_pressure     ||
            first.freezing_point_sample_temperature  != second.freezing_point_sample_temperature  ||
            first.boiling_point_sample_pressure      != second.boiling_point_sample_pressure      ||
            first.boiling_point_sample_temperature   != second.boiling_point_sample_temperature   ||
            first.simon_glatzel_slope                != second.simon_glatzel_slope                ||
            first.simon_glatzel_exponent             != second.simon_glatzel_exponent             ||
            first.molecular_absorption_cross_section != second.molecular_absorption_cross_section ||
            first.gas                                != second.gas                                ||
            first.liquid                             != second.liquid                             
        ) {
            return false;
        }
        for (std::size_t i = 0; i <first.solids.size(); i++)
        {
            if (first.solids[i] != second.solids[i])
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const compound::PartlyKnownCompound& first, const compound::PartlyKnownCompound& second)
    {
        return !(first==second);
    }

    int PartlyKnownCompound_attribute_index_sum(const compound::PartlyKnownCompound& compound)
    {
        int sum = 
            compound.molecular_degrees_of_freedom       .index() +
            compound.acentric_factor                    .index() +
            compound.latent_heat_of_vaporization        .index() +
            compound.latent_heat_of_fusion              .index() +
            compound.triple_point_pressure              .index() +
            compound.triple_point_temperature           .index() +
            compound.freezing_point_sample_pressure     .index() +
            compound.freezing_point_sample_temperature  .index() +
            compound.boiling_point_sample_pressure      .index() +
            compound.boiling_point_sample_temperature   .index() +
            compound.simon_glatzel_slope                .index() +
            compound.simon_glatzel_exponent             .index() +
            compound.molecular_absorption_cross_section .index() +
            PartlyKnownGas_attribute_index_sum(compound.gas) +
            PartlyKnownLiquid_attribute_index_sum(compound.liquid) ;
        for (std::size_t i = 0; i < compound.solids.size(); ++i)
        {
            PartlyKnownSolid_attribute_index_sum(compound.solids[i]);
        }
        return sum;
    }

    int PartlyKnownCompound_attribute_known_count(const compound::PartlyKnownCompound& compound)
    {
        int sum = 
            compound.molecular_degrees_of_freedom       .has_value() +
            compound.acentric_factor                    .has_value() +
            compound.latent_heat_of_vaporization        .has_value() +
            compound.latent_heat_of_fusion              .has_value() +
            compound.triple_point_pressure              .has_value() +
            compound.triple_point_temperature           .has_value() +
            compound.freezing_point_sample_pressure     .has_value() +
            compound.freezing_point_sample_temperature  .has_value() +
            compound.boiling_point_sample_pressure      .has_value() +
            compound.boiling_point_sample_temperature   .has_value() +
            compound.simon_glatzel_slope                .has_value() +
            compound.simon_glatzel_exponent             .has_value() +
            compound.molecular_absorption_cross_section .has_value() +
            PartlyKnownGas_attribute_known_count(compound.gas) +
            PartlyKnownLiquid_attribute_known_count(compound.liquid) ;
        for (std::size_t i = 0; i < compound.solids.size(); ++i)
        {
            PartlyKnownSolid_attribute_known_count(compound.solids[i]);
        }
        return sum;
    }

    // "unknown_hydrogen" is meant to serve as a left identity of value_or. 
    // The constructor forbids us to set molar_mass or atoms_per_molecule to indicate missing values,
    // since they are almost always known and can be used to guarantee a wide range of derived properties.
    // However, since all other compounds also require these properties, 
    // and therefore will automatically guarantee the properties that derive from them,
    // unknown properties like this one nevertheless behave like left identities.
    // So it's as if compounds are monoids for which there is a set of left identities available,
    // those being properties for which everything is missing except required properties.
    compound::PartlyKnownCompound unknown_hydrogen {
        /*molar_mass*/                        1.0 * si::gram/si::mole,
        /*atoms_per_molecule*/                1u,
        /*molecular_diameter*/                compound::field::missing(),
        /*molecular_degrees_of_freedom*/      compound::field::missing(),
        /*acentric_factor*/                   compound::field::missing(),

        /*critical_point_pressure*/           compound::field::missing(),
        /*critical_point_volume*/             compound::field::missing(),
        /*critical_point_temperature*/        compound::field::missing(),
        /*critical_point_compressibility*/    compound::field::missing(),

        /*latent_heat_of_vaporization*/       compound::field::missing(),
        /*latent_heat_of_fusion*/             compound::field::missing(),
        /*triple_point_pressure*/             compound::field::missing(),
        /*triple_point_temperature*/          compound::field::missing(),
        /*freezing_point_sample_pressure*/    compound::field::missing(),
        /*freezing_point_sample_temperature*/ compound::field::missing(),
        /*boiling_point_sample_pressure*/     compound::field::missing(),
        /*boiling_point_sample_temperature*/  compound::field::missing(),
        /*simon_glatzel_slope*/               compound::field::missing(),
        /*simon_glatzel_exponent*/            compound::field::missing(),

        /*molecular_absorption_cross_section*/ compound::field::missing(),

        /*gas*/
        compound::phase::PartlyKnownGas {
            /*specific_heat_capacity*/ compound::field::missing(),
            /*thermal_conductivity*/   compound::field::missing(),
            /*dynamic_viscosity*/      compound::field::missing(),
            /*density*/                compound::field::missing(),
            /*refractive_index*/       compound::field::missing()
        },

        /*liquid*/
        compound::phase::PartlyKnownLiquid {
            /*specific_heat_capacity*/ compound::field::missing(),
            /*thermal_conductivity*/   compound::field::missing(),
            /*dynamic_viscosity*/      compound::field::missing(),
            /*density*/                compound::field::missing(),
            /*vapor_pressure*/         compound::field::missing(),
            /*surface_tension*/        compound::field::missing(),
            /*refractive_index*/       compound::field::missing()
        },

        /*solid*/ 
        std::vector<compound::phase::PartlyKnownSolid>{
            compound::phase::PartlyKnownSolid {
                /*specific_heat_capacity*/            compound::field::missing(),
                /*thermal_conductivity*/              compound::field::missing(),
                /*dynamic_viscosity*/                 compound::field::missing(),
                /*density*/                           compound::field::missing(),
                /*vapor_pressure*/                    compound::field::missing(),
                /*refractive_index*/                  compound::field::missing(),
                /*spectral_reflectance*/              compound::field::missing(),

                /*bulk_modulus*/                      compound::field::missing(),
                /*tensile_modulus*/                   compound::field::missing(),
                /*shear_modulus*/                     compound::field::missing(),
                /*pwave_modulus*/                     compound::field::missing(),
                /*lame_parameter*/                    compound::field::missing(),
                /*poisson_ratio*/                     compound::field::missing(),

                /*compressive_fracture_strength*/     compound::field::missing(),
                /*tensile_fracture_strength*/         compound::field::missing(),
                /*shear_fracture_strength*/           compound::field::missing(),
                /*compressive_yield_strength*/        compound::field::missing(),
                /*tensile_yield_strength*/            compound::field::missing(),
                /*shear_yield_strength*/              compound::field::missing(),

                /*chemical_susceptibility_estimate*/  compound::field::missing()
            }
        }
    };
}
