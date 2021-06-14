#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "CompletedCompound.hpp"
#include "field/missing.hpp"

// test utility libraries
#include <units/si_test_utils.hpp>

#include "field/constant/OptionalConstantField_test_utils.hpp"
#include "field/state/OptionalStateField_test_utils.hpp"
#include "field/spectral/OptionalSpectralField_test_utils.hpp"

#include "phase/gas/CompletedGas_test_utils.hpp"
#include "phase/liquid/CompletedLiquid_test_utils.hpp"
#include "phase/solid/CompletedSolid_test_utils.hpp"

namespace compound 
{
    bool operator==(const CompletedCompound& first, const CompletedCompound& second)
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

    bool operator!=(const CompletedCompound& first, const CompletedCompound& second)
    {
        return !(first==second);
    }


    CompletedCompound known_dummy_compound (
        /*molar_mass*/                        1.0 * si::kilogram/si::mole,
        /*atoms_per_molecule*/                1u,
        /*molecular_diameter*/                1 * si::picometer,
        /*molecular_degrees_of_freedom*/      1.0,
        /*acentric_factor*/                   1.0,

        /*critical_point_pressure*/           1.0 * si::megapascal,
        /*critical_point_volume*/             1.0 * si::meter3/si::mole,
        /*critical_point_temperature*/        1.0 * si::kelvin,
        /*critical_point_compressibility*/    1.0,

        /*latent_heat_of_vaporization*/       1.0 * si::joule/si::kilogram,
        /*latent_heat_of_fusion*/             1.0 * si::joule/si::kilogram,
        /*triple_point_pressure*/             1.0 * si::pascal,
        /*triple_point_temperature*/          si::standard_temperature,
        /*freezing_point_sample_pressure*/    si::standard_pressure,
        /*freezing_point_sample_temperature*/ 0.0*si::celcius,
        /*boiling_point_sample_pressure*/     si::standard_pressure,
        /*boiling_point_sample_temperature*/  100.8 * si::celcius, 
        /*simon_glatzel_slope*/               1.0,
        /*simon_glatzel_exponent*/            1.0,

        /*molecular_absorption_cross_section*/ 1e-25 * si::meter2,

        /*gas*/
        phase::CompletedGas {
            /*specific_heat_capacity*/ 1.0 * si::joule / (si::kilogram * si::kelvin),
            /*thermal_conductivity*/   1.0 * si::watt / (si::meter * si::kelvin),
            /*dynamic_viscosity*/      1.0 * si::pascal * si::second,
            /*density*/                1.0 * si::kilogram/si::meter3,
            /*refractive_index*/       1.1,
        },

        /*liquid*/
        phase::CompletedLiquid {
            /*specific_heat_capacity*/ 1.0 * si::joule / (si::kilogram * si::kelvin),
            /*thermal_conductivity*/   1.0 * si::watt / (si::meter * si::kelvin),
            /*dynamic_viscosity*/      1.0 * si::kilogram / (si::meter * si::second),
            /*density*/                1.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/         1.0 * si::pascal,
            /*surface_tension*/        1.0 * si::millinewton/si::meter,
            /*refractive_index*/       1.1
        },

        /*solid*/ 
        std::vector<phase::CompletedSolid>{
            phase::CompletedSolid {
                /*specific_heat_capacity*/            1.0 * si::joule/(si::kilogram*si::kelvin),
                /*thermal_conductivity*/              1.0 * si::watt / (si::meter * si::kelvin),
                /*dynamic_viscosity*/                 1.0 * si::pascal*si::second,
                /*density*/                           1.0 * si::kilogram/si::meter3,
                /*vapor_pressure*/                    1.0 * si::pascal,
                /*refractive_index*/                  1.1,
                /*spectral_reflectance*/              0.9,
                /*bulk_modulus*/                      1.0 * si::gigapascal,
                /*tensile_modulus*/                   1.0 * si::gigapascal,
                /*shear_modulus*/                     1.0 * si::gigapascal,
                /*pwave_modulus*/                     1.0 * si::gigapascal,
                /*lame_parameter*/                    1.0 * si::gigapascal,
                /*poisson_ratio*/                     1.0,
                /*compressive_fracture_strength*/     1.0 * si::megapascal,
                /*tensile_fracture_strength*/         1.0 * si::megapascal,
                /*shear_fracture_strength*/           1.0 * si::megapascal,
                /*compressive_yield_strength*/        1.0 * si::megapascal,
                /*tensile_yield_strength*/            1.0 * si::megapascal,
                /*shear_yield_strength*/              1.0 * si::megapascal,
                /*chemical_susceptibility_estimate*/  1.0
            }
        }
    );



}



