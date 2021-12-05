#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "PartlyKnownCompound.hpp"
#include "state_functions.hpp"
#include "spectral_functions.hpp"

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
    PartlyKnownCompound unknown_hydrogen {
        /*molar_mass*/                        1.0 * si::gram/si::mole,
        /*atoms_per_molecule*/                1u,
        /*molecular_diameter*/                compound::missing(),
        /*molecular_degrees_of_freedom*/      compound::missing(),
        /*acentric_factor*/                   compound::missing(),

        /*critical_point_pressure*/           compound::missing(),
        /*critical_point_volume*/             compound::missing(),
        /*critical_point_temperature*/        compound::missing(),
        /*critical_point_compressibility*/    compound::missing(),

        /*latent_heat_of_vaporization*/       compound::missing(),
        /*latent_heat_of_fusion*/             compound::missing(),
        /*triple_point_pressure*/             compound::missing(),
        /*triple_point_temperature*/          compound::missing(),
        /*freezing_point_sample_pressure*/    compound::missing(),
        /*freezing_point_sample_temperature*/ compound::missing(),
        /*boiling_point_sample_pressure*/     compound::missing(),
        /*boiling_point_sample_temperature*/  compound::missing(),
        /*simon_glatzel_slope*/               compound::missing(),
        /*simon_glatzel_exponent*/            compound::missing(),

        /*molecular_absorption_cross_section*/ compound::missing(),

        /*gas*/
        compound::phase::PartlyKnownGas {
            /*specific_heat_capacity*/ compound::missing(),
            /*thermal_conductivity*/   compound::missing(),
            /*dynamic_viscosity*/      compound::missing(),
            /*density*/                compound::missing(),
            /*refractive_index*/       compound::missing()
        },

        /*liquid*/
        compound::phase::PartlyKnownLiquid {
            /*specific_heat_capacity*/ compound::missing(),
            /*thermal_conductivity*/   compound::missing(),
            /*dynamic_viscosity*/      compound::missing(),
            /*density*/                compound::missing(),
            /*vapor_pressure*/         compound::missing(),
            /*surface_tension*/        compound::missing(),
            /*refractive_index*/       compound::missing(),
            /*extinction_coefficient*/ compound::missing()
        },

        /*solid*/ 
        std::vector<compound::phase::PartlyKnownSolid>{
            compound::phase::PartlyKnownSolid {
                /*specific_heat_capacity*/            compound::missing(),
                /*thermal_conductivity*/              compound::missing(),
                /*dynamic_viscosity*/                 compound::missing(),
                /*density*/                           compound::missing(),
                /*vapor_pressure*/                    compound::missing(),
                /*refractive_index*/                  compound::missing(),
                /*extinction_coefficient*/            compound::missing(),
                /*absorption_coefficient*/            compound::missing(),

                /*bulk_modulus*/                      compound::missing(),
                /*tensile_modulus*/                   compound::missing(),
                /*shear_modulus*/                     compound::missing(),
                /*pwave_modulus*/                     compound::missing(),
                /*lame_parameter*/                    compound::missing(),
                /*poisson_ratio*/                     compound::missing(),

                /*compressive_fracture_strength*/     compound::missing(),
                /*tensile_fracture_strength*/         compound::missing(),
                /*shear_fracture_strength*/           compound::missing(),
                /*compressive_yield_strength*/        compound::missing(),
                /*tensile_yield_strength*/            compound::missing(),
                /*shear_yield_strength*/              compound::missing(),

                /*chemical_susceptibility_estimate*/  compound::missing()
            }
        }
    };



    // "test_water" is meant to serve as a right identity of value_or. 
    // We know all properties for it, so elsewhere in code it is represented by a CompletedCompound,
    // but we store it here as a PartlyKnownCompound since it can serve as a right identity.
    PartlyKnownCompound test_water {
        /*molar_mass*/                        18.015 * si::gram/si::mole,
        /*atoms_per_molecule*/                3u,
        /*molecular_diameter*/                265.0 * si::picometer,                                    // wikipedia,  Ismail (2015)
        /*molecular_degrees_of_freedom*/      6.8,
        /*acentric_factor*/                   0.345,

        /*critical_point_pressure*/           22.06 * si::megapascal,
        /*critical_point_volume*/             56.0 * si::centimeter3/si::mole,                         
        /*critical_point_temperature*/        647.01 * si::kelvin,
        /*critical_point_compressibility*/    0.230,                                                    // engineering toolbox

        /*latent_heat_of_vaporization*/       22.6e5 * si::joule/si::kilogram, 
        /*latent_heat_of_fusion*/             6.01*si::kilojoule/(18.015*si::gram),
        /*triple_point_pressure*/             0.6116e3*si::pascal,
        /*triple_point_temperature*/          273.15 * si::kelvin,
        /*freezing_point_sample_pressure*/    si::atmosphere,
        /*freezing_point_sample_temperature*/ si::standard_temperature,
        /*boiling_point_sample_pressure*/     si::atmosphere,
        /*boiling_point_sample_temperature*/  100.0*si::celcius,
        /*simon_glatzel_slope*/               7070e5,
        /*simon_glatzel_exponent*/            4.46,

        /*molecular_absorption_cross_section*/ 
        get_molecular_absorption_cross_section_function
            ( 1.0/si::meter, si::meter2,
              std::vector<double>{  9.73e1, 6.05e2, 7.37e3, 1.65e4, 2.98e4, 6.50e4, 9.73e4, 1.38e5, 1.62e5, 2.63e5, 3.35e5, 4.39e5, 5.15e5, 5.89e5, 6.93e5, 9.82e5, 1.25e6, 1.64e6, 2.08e6, 2.38e6, 2.41e6, 2.44e6, 2.47e6, 2.53e6, 2.59e6, 2.63e6, 2.73e6, 2.78e6, 2.93e6, 2.98e6, 3.05e6, 3.08e6, 5.11e6, 5.63e6, 6.04e6, 6.45e6, 6.86e6, 8.04e6, 9.68e6, 1.08e7, 1.24e7, 1.37e7, 2.37e7, 3.94e7, 6.98e7, 1.69e8 },
              std::vector<double>{  -24.98, -24.44, -23.93, -23.46, -23.46, -22.97, -23.70, -23.77, -23.11, -24.44, -22.46, -25.14, -24.47, -25.68, -25.10, -27.10, -28.15, -29.10, -30.25, -29.38, -29.28, -29.28, -29.47, -29.22, -29.47, -29.55, -29.28, -29.21, -29.27, -28.95, -28.71, -28.69, -25.41, -21.62, -21.41, -21.51, -21.76, -21.09, -20.98, -20.74, -20.82, -20.75, -20.83, -21.08, -21.54, -22.44 }),

        /*gas*/
        phase::PartlyKnownGas {
            /*specific_heat_capacity*/ // 2.080 * si::joule / (si::gram * si::kelvin),                     // wikipedia
                get_exponent_pressure_temperature_function
                    (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                    0.01766, 0.80539, 0.00707, 0.69586, 1.42782,
                    300.0, 1273.2, 0.0, 10.0), 
                    // water, mean error: 0.8%, max error: 3.4%, range: 300-1273.2K, 0-10MPa, stp estimate: 1.781
            /*thermal_conductivity*/   // 0.016 * si::watt / (si::meter * si::kelvin),                     // wikipedia
                get_sigmoid_exponent_pressure_temperature_function
                    (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                    0.00054, 1.09614, 0.00000, 0.00000, 0.09827, 691.90362, 883.95160, 0.08323), 
                    // water, mean error: 2.5%, max error: 9.7%, range: 300-1273.2K, 0-10MPa, stp estimate: 0.018
            /*dynamic_viscosity*/      // 1.24e-5 * si::pascal * si::second,                               // engineering toolbox, at 100 C
                get_exponent_pressure_temperature_function
                    (si::kelvin, si::megapascal, si::micropascal*si::second, 
                    0.00019, 3.33694, 0.02183, 1.08016, -0.58257,
                    300.0, 1273.2, 0.0, 10.0), 
                    // water, mean error: 1.2%, max error: 3.5%, range: 300-1273.2K, 0-10MPa, stp estimate: 8.765
            /*density*/                0.6* si::kilogram/si::meter3,
            /*refractive_index*/       1.000261                                                         // engineering toolbox
        },

        /*liquid*/
        phase::PartlyKnownLiquid {
            /*specific_heat_capacity*/ 4.1813 * si::joule / (si::gram * si::kelvin),                    // wikipedia
            /*thermal_conductivity*/   // 0.6062 * si::watt / (si::meter * si::kelvin), 
                get_dippr_liquid_thermal_conductivity_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     -0.432, 0.0057255, -0.000008078, 1.861e-9, 0.0, 273.15, 633.15), 
            /*dynamic_viscosity*/      
                get_dippr_liquid_viscosity_temperature_function
                    (si::kelvin, si::pascal* si::second, 
                     -52.843, 3703.6, 5.866, -5.879e-29, 10.0, 
                     273.15, 646.15), // 273.16-646.15K
            /*density*/                // 997.0 * si::kilogram/si::meter3,                                
                field::StateFunction<si::density>([](const si::pressure p, const si::temperature T) {
                    // Perry equation 119, specialized for water
                    // valid for 273.16-647.096K
                    si::temperature Tc = 647.096 * si::kelvin;
                    double tau = 1.0 - (T/Tc);
                    double rho = 17.874 
                        + 35.618*pow(tau,0.33) 
                        + 19.655*pow(tau,0.66)
                        - 9.1306*pow(tau, 5.0/3.0) 
                        - 31.367*pow(tau, 16.0/3.0) 
                        - 813.56*pow(tau,43.0/3.0) 
                        - 17421000.0*pow(tau,110.0/3.0);
                    return rho * 18.015 * si::gram / si::decimeter3;
                }),
            /*vapor_pressure*/         
                // get_antoine_vapor_pressure_function(
                //     si::celcius, si::millimeter_mercury, 
                //     7.94917, 1657.462, 1474.68, 213.69), // Physical and Chemical Equilibrium for Chemical Engineers, Second Edition. 
                // get_dippr_liquid_vapor_pressure_temperature_function
                //     (si::kelvin, si::pascal,
                //      73.649, -7258.2, -7.3037, 4.1653e-6),//273.16-647.1K
                field::StateFunction<si::pressure>([](const si::pressure p, const si::temperature T) {
                    // Buck equation
                    double C = T/si::celcius;
                    return 0.61121*exp((18.678-C/234.5) * (C/(257.14+C))) * si::kilopascal; 
                }),
            /*surface_tension*/            
                get_refprop_liquid_surface_tension_temperature_function
                    (si::kelvin, si::millinewton/si::meter,
                     647.01, -0.1306, 2.471, 0.2151, 1.233, 0.0, 0.0,
                     233.22, 646.15), // Mulero (2012)
            /*refractive_index*/       //1.33336,
                get_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{0.2,   0.3,   0.425,  0.55, 0.675,   0.8, 0.925,   1.4,   2.4,   2.8,  3.05,  3.3,   5.6,   6.6,   7.6,  13.0,  15.5,  18.0,  21.0,  26.0,  32.0,  42.0,  60.0, 110.0, 160.0,200.0},
                     std::vector<double>{1.396, 1.349, 1.338, 1.333, 1.331, 1.329, 1.328, 1.321, 1.279, 1.142, 1.426, 1.45, 1.289, 1.334, 1.302, 1.146, 1.297, 1.423, 1.487, 1.539, 1.546, 1.522, 1.703, 1.966, 2.081, 2.13}),
            /*extinction_coefficient*/ 
                get_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{0.2,     0.3,     0.425,   0.55,     0.675,    0.8,      0.925,    1.4,      2.8,   3.0,   3.3,    3.6,     4.1,     4.6,    5.6,    6.1,      6.6,    9.2,   10.5,  13.0,  15.5,  18.0,  32.0,  60.0, 200.0},
                     std::vector<double>{1.1e-07, 1.6e-08, 1.3e-09, 1.96e-09, 2.23e-08, 1.25e-07, 1.06e-06, 0.000138, 0.115, 0.272, 0.0368, 0.00515, 0.00562, 0.0147, 0.0142, 0.131, 0.0356, 0.0415, 0.0662, 0.305, 0.414, 0.426, 0.324, 0.587, 0.504})
        },

        /*solid*/ 
        std::vector<phase::PartlyKnownSolid>{
            phase::PartlyKnownSolid {
                /*specific_heat_capacity*/            2.05 * si::joule / (si::gram * si::kelvin),       // wikipedia
                /*thermal_conductivity*/              2.09 * si::watt / (si::meter * si::kelvin),       // wikipedia
                /*dynamic_viscosity*/                 1e13 * si::poise,                                 // reference by Carey (1953)
                /*density*/                           0916.9 * si::kilogram/si::meter3,
                /*vapor_pressure*/                    //138.268 * si::megapascal,
                    get_interpolated_temperature_function
                        (si::celcius, si::millimeter_mercury,
                         std::vector<double>{-17.3,  11.2,   51.6,   100.0  }, 
                         std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
                /*refractive_index*/                  1.3098,
                /*extinction_coefficient*/            0.3,
                /*absorption_coefficient*/            25.0 / si::centimeter,

                /*bulk_modulus*/                      8.899 * si::gigapascal,                           // gammon (1983)
                /*tensile_modulus*/                   9.332 * si::gigapascal,                           // gammon (1983)
                /*shear_modulus*/                     3.521 * si::gigapascal,                           // gammon (1983)
                /*pwave_modulus*/                     13.59 * si::gigapascal,                           // gammon (1983)
                /*lame_parameter*/                    6.552 * si::gigapascal,                           // gammon (1983)
                /*poisson_ratio*/                     0.3252,                                           // gammon (1983)

                /*compressive_fracture_strength*/     6.0 * si::megapascal,                             //engineering toolbox
                /*tensile_fracture_strength*/         1.0 * si::megapascal,                             //engineering toolbox
                /*shear_fracture_strength*/           1.1 * si::megapascal,                             // Frederking (1989)
                /*compressive_yield_strength*/        6.0 * si::megapascal,                             // brittle, effectively the same as fracture strength
                /*tensile_yield_strength*/            1.0 * si::megapascal,                             // brittle, effectively the same as fracture strength
                /*shear_yield_strength*/              1.1 * si::megapascal,                             // brittle, effectively the same as fracture strength

                /*chemical_susceptibility_estimate*/  false
            }
        }
    };

    PartlyKnownCompound partly_known_dummy_compound (
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
        phase::PartlyKnownGas {
            /*specific_heat_capacity*/ state_invariant(1.0 * si::joule / (si::kilogram * si::kelvin)),
            /*thermal_conductivity*/   state_invariant(1.0 * si::watt / (si::meter * si::kelvin)),
            /*dynamic_viscosity*/      state_invariant(1.0 * si::pascal * si::second),
            /*density*/                state_invariant(1.0 * si::kilogram/si::meter3),
            /*refractive_index*/       spectral_invariant(1.1)
        },

        /*liquid*/
        phase::PartlyKnownLiquid {
            /*specific_heat_capacity*/ state_invariant(1.0 * si::joule / (si::kilogram * si::kelvin)),
            /*thermal_conductivity*/   state_invariant(1.0 * si::watt / (si::meter * si::kelvin)),
            /*dynamic_viscosity*/      state_invariant(1.0 * si::kilogram / (si::meter * si::second)),
            /*density*/                state_invariant(1.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/         state_invariant(1.0 * si::pascal),
            /*surface_tension*/        state_invariant(1.0 * si::millinewton/si::meter),
            /*refractive_index*/       spectral_invariant(1.1),
            /*extinction_coefficient*/ spectral_invariant(0.9)
        },

        /*solid*/ 
        std::vector<phase::PartlyKnownSolid>{
            phase::PartlyKnownSolid {
                /*specific_heat_capacity*/            state_invariant(1.0 * si::joule/(si::kilogram*si::kelvin)),
                /*thermal_conductivity*/              state_invariant(1.0 * si::watt / (si::meter * si::kelvin)),
                /*dynamic_viscosity*/                 state_invariant(1.0 * si::pascal*si::second),
                /*density*/                           state_invariant(1.0 * si::kilogram/si::meter3),
                /*vapor_pressure*/                    state_invariant(1.0 * si::pascal),
                /*refractive_index*/                  spectral_invariant(1.1),
                /*extinction_coefficient*/            spectral_invariant(0.3),
                /*absorption_coefficient*/            spectral_invariant(25.0 / si::centimeter),
                /*bulk_modulus*/                      state_invariant(1.0 * si::gigapascal),
                /*tensile_modulus*/                   state_invariant(1.0 * si::gigapascal),
                /*shear_modulus*/                     state_invariant(1.0 * si::gigapascal),
                /*pwave_modulus*/                     state_invariant(1.0 * si::gigapascal),
                /*lame_parameter*/                    state_invariant(1.0 * si::gigapascal),
                /*poisson_ratio*/                     state_invariant(1.0),
                /*compressive_fracture_strength*/     state_invariant(1.0 * si::megapascal),
                /*tensile_fracture_strength*/         state_invariant(1.0 * si::megapascal),
                /*shear_fracture_strength*/           state_invariant(1.0 * si::megapascal),
                /*compressive_yield_strength*/        state_invariant(1.0 * si::megapascal),
                /*tensile_yield_strength*/            state_invariant(1.0 * si::megapascal),
                /*shear_yield_strength*/              state_invariant(1.0 * si::megapascal),
                /*chemical_susceptibility_estimate*/  1.0
            }
        }
    );


}
