#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "PartlyKnownCompound.hpp"
#include "state_functions.hpp"
#include "spectral_functions.hpp"

namespace compound {
namespace published {





// ethanol, C2H6O
// for small bodies in the outer solar system and interstellar space
// one of the most abundant compounds following from elemental abundances in the universe
PartlyKnownCompound ethanol (
    /*molar_mass*/                        46.068 * si::gram/si::mole,
    /*atoms_per_molecule*/                9u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.637,

    /*critical_point_pressure*/           6.25 * si::megapascal,
    /*critical_point_volume*/             169.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        351.44 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       42.32 * si::kilojoule/(46.068*si::gram), 
    /*latent_heat_of_fusion*/             4.931*si::kilojoule/(46.068*si::gram),
    /*triple_point_pressure*/             0.00043 * si::pascal,  // wikipedia data page
    /*triple_point_temperature*/          150.0 * si::kelvin,  // wikipedia data page
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -114.14 * si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  352.2 * si::kelvin, // NIST webbook
    
    /*phase*/                             
    get_simon_glatzel_phase_function(
            /*triple_point_pressure*/       0.00043 * si::pascal,  // wikipedia data page
            /*triple_point_temperature*/    150.0 * si::kelvin,  // wikipedia data page
            /*critical_point_pressure*/     6.25 * si::megapascal,
            /*critical_point_temperature*/  351.44 * si::kelvin,
            /*latent_heat_of_vaporization*/ 42.32 * si::kilojoule/(46.068*si::gram), 
            /*molar_mass*/                  46.068 * si::gram/si::mole,
            /*simon_glatzel_slope*/         10600e5 * si::pascal,
            /*simon_glatzel_intercept*/     0.0 * si::pascal,
            /*simon_glatzel_exponent*/      1.61
        ),
    
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ 78.28 * si::joule / (46.068*si::gram*si::kelvin), // wikipedia data page
        /*thermal_conductivity*/   // 14.4 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 -0.010109, 0.6475, -7332.0, -268e3,
                 293.15, 1000.0), // 293.15-1000K
        /*dynamic_viscosity*/      
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::pascal * si::second,
                1.0613e-7, 0.8066, 52.7, 0.0,
                200.0, 1000.0), // 200-1000K
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 112.4 * si::joule / (46.068*si::gram*si::kelvin), // wikipedia data page
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (46.06844 * si::kilogram * si::kelvin), 
                102640.0, -139.63, -0.030341, 0.0020386, 0.0,
                159, 390), 
            // get_interpolated_temperature_function
            //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
            //       std::vector<double>{250.0,280.0,300.0,440.0,480.0,500.0},
            //       std::vector<double>{2.032,2.351,2.597,4.109,5.091,7.255}),
        /*thermal_conductivity*/   // 0.167 * si::watt / ( si::meter * si::kelvin ),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.2468, -0.000264,  0.0,  0.0,  0.0,
                  159.05, 353.15), 
        /*dynamic_viscosity*/      
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 7.875, 781.98, -3.0418, 0.0, 0.0,
                  200, 440), // 200-440K
        /*density*/                // 0789.3 * si::kilogram/si::meter3,
            get_dippr_temperature_function_105
                (si::kelvin, 46.068 * si::gram/si::decimeter3,
                 1.6288, 0.27469, 514.0, 0.23178,
                 159.05, 514), // 159.05-514K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 73.304, -7122.3, -7.1424, 2.8853e-6, 2.0,
                 159.05, 514.0), 
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 351.44, 0.05, 0.952, 0.0, 0.0, 0.0, 0.0,
                 180.12, 513.15), // Mulero (2012)
        /*refractive_index*/       1.361,  // wikipedia data page
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     double invl2 = 1.0/(l*l);
        //     return 1.35265f
        //         + 0.00306 * invl2
        //         + 0.00002 * invl2*invl2;
        // }),
        /*extinction_coefficient*/        missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid> {
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    (si::kelvin, si::joule/(46.068*si::gram*si::kelvin),
                     std::vector<double>{0.0, 159.2}, 
                     std::vector<double>{0.0,111.46}), // wikipedia data page
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(0.739 * si::gram / si::centimeter3), // Hudson (2020)
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  1.319, // Hudson (2020)
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
); 

// formaldehyde, CH2O
// for small bodies in the outer solar system and interstellar space
// one of the most abundant compounds following from elemental abundances in the universe
PartlyKnownCompound formaldehyde (
    /*molar_mass*/                        30.026 * si::gram/si::mole, // wikipedia
    /*atoms_per_molecule*/                4u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.282,

    /*critical_point_pressure*/           6.788 * si::megapascal,
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        410.3 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       23.3 * si::kilojoule/(30.026*si::gram),//pubchem
    /*latent_heat_of_fusion*/             7.53 * si::kilojoule/(30.026*si::gram),// Vasil'ev et al. (1998)
    /*triple_point_pressure*/             missing(), //71549032.0 * si::pascal, // unverified
    /*triple_point_temperature*/          155.10 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 181.0 * si::kelvin,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -21.0 * si::celcius, // Perry

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  2.51e6, 2.67e6, 2.70e6, 2.74e6, 2.83e6, 2.86e6, 2.95e6, 2.98e6, 3.06e6, 3.09e6, 3.18e6, 3.62e6, 4.02e6, 4.44e6 },
          std::vector<double>{   -26.5,  -26.1,  -25.2,  -26.1,  -23.6,  -25.4,  -23.2,  -25.0,  -23.1,  -24.5,  -23.2,  -23.6,  -24.5,  -25.7 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 5.2201e-06, 1.417, 0.0, 0.0,
                 253.85, 1000.0), // 253.85-1000K
        /*dynamic_viscosity*/      
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::pascal * si::second,
                1.5948e-5, 0.21516, 1151.1, 0.0,
                167.55, 1000.0), // 167.55-1000K
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (30.02598* si::kilogram * si::kelvin), 
                70077.0, -661.79, 5.9749, -0.01813, 0.00001983,
                155.15, 253.85), 
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.336003243, -0.00054, 0.0, 0.0, 0.0,
                  155.15, 253.85), 
        /*dynamic_viscosity*/      // 0.1421 * si::millipascal * si::second, //pubchem
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -7.6591, 603.36, -0.53378, 0.0, 0.0,
                  155.15, 253.85), // 155.15-253.85K
        /*density*/                // 0.8153 * si::kilogram/si::meter3,  // wikipedia
            get_dippr_temperature_function_105
                (si::kelvin, 30.026 * si::gram/si::decimeter3,
                 3.897011, 0.331636, 420.0, 0.28571,
                 155.15, 420.0), // 155.15-420.0K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 49.3632, -3847.87, -4.09834, 4.64e-17, 6.0,
                  155.15, 420), // 155.15-420K
        /*surface_tension*/        state_invariant(27.3797 * si::dyne/si::centimeter), // 25 °C, PubChem
        /*refractive_index*/       1.3714,  // wikipedia
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           missing(),
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{ -88.0,  51.6,   100.0  }, 
                     std::vector<double>{ 10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);


// formic acid, CH2O2
// for small bodies in the outer solar system and interstellar space
// one of the most abundant compounds following from elemental abundances in the universe
PartlyKnownCompound formic_acid (
    /*molar_mass*/                        46.026 * si::gram/si::mole,
    /*atoms_per_molecule*/                5u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.473,

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             115.9*9*si::centimeter3/si::mole,
    /*critical_point_temperature*/        588.0 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       20.10 * si::kilojoule/(46.026*si::gram),
    /*latent_heat_of_fusion*/             12.68*si::kilojoule/(46.026*si::gram),
    /*triple_point_pressure*/             2.2 * si::kilopascal,
    /*triple_point_temperature*/          281.4 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 8.3 * si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  100.8 * si::celcius, // Perry

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ 45.68 * si::joule / (46.026*si::gram*si::kelvin), // wikipedia data page
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::pascal * si::second,
                5.0702e-8, 0.9114, 0.0, 0.0,
                281.45, 1000.0), // 281.45-1000K
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 101.3 * si::joule / (46.026*si::gram*si::kelvin), // wikipedia data page
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (46.0257* si::kilogram * si::kelvin), 
                78060.0, 71.54, 0.0, 0.0, 0.0,
                281.45, 380.0), 
        /*thermal_conductivity*/   // 0.267 * si::watt / (si::meter * si::kelvin),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.302, -0.00018, 0.0,  0.0,  0.0,
                  281.45, 373.71), 
        /*dynamic_viscosity*/      // 1.607 * si::millipascal*si::second,
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -48.529, 3394.7, 5.3903, 0.0, 0.0,
                  281.45, 373.71), // 281.45-373.71K
        /*density*/                // 1220.0 * si::kilogram/si::meter3,
            get_dippr_temperature_function_105
                (si::kelvin, 46.026 * si::gram/si::decimeter3,
                 1.938, 0.24225, 588.0, 0.24435,
                 281.45, 588.0), // 281.45-588.0K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 43.8066, -5131.03, -3.18777, 2.37819e-6, 2.0,
                  281.45, 588), // 281.45-588K
        /*surface_tension*/        
            get_interpolated_temperature_function
                (si::celcius, si::millinewton/si::meter,
                 std::vector<double>{ 25.0,  50.0,  75.0 }, 
                 std::vector<double>{37.13, 34.38, 31.64 }), 
        /*refractive_index*/       1.3714,
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    (si::kelvin, si::joule / (46.026*si::gram*si::kelvin),
                     std::vector<double>{0.0, 281.5}, 
                     std::vector<double>{0.0,  74.5}), // wikipedia data page
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(0.979 * si::gram / si::centimeter3), // Hudson (2020)
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::pascal,
                      std::vector<double>{-56.0,     -40.4,     -0.8}, 
                     std::vector<double>{1.0 ,       100.0,     1000.0}),
            /*refractive_index*/                  1.291, // Hudson (2020)
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);

// POORLY CHARACTERIZED COMPOUNDS:
// perflouromethane, tetraflouromethane, carbon tetraflouride, CF4
// for modeling industrial emissions and the terraforming of Mars as suggested by Zubrin (1996) 
// We went out of our way searching for an IR graph since 
// we use CF4 in the model to study pollution and Martian terraformation
PartlyKnownCompound perflouromethane(
    /*molar_mass*/                        88.0  * si::gram/si::mole,
    /*atoms_per_molecule*/                5u,
    /*molecular_diameter*/                470.0 * si::picometer, // Motkuri (2014)
    /*molecular_degrees_of_freedom*/      missing(),
    /*acentric_factor*/                   0.186,

    /*critical_point_pressure*/           3.73 * si::megapascal,
    /*critical_point_volume*/             140.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        227.54 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       137000.0 * si::joule / si::kilogram,
    /*latent_heat_of_fusion*/             0.704*si::kilojoule/(88.0*si::gram),
    /*triple_point_pressure*/             0.1012e3 * si::pascal,
    /*triple_point_temperature*/          89.54 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -183.60*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -128.05*si::celcius,

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 7.99e6, 8.62e6, 9.25e6, 1.00e7, 1.05e7, 1.08e7, 1.13e7, 1.20e7, 1.28e7, 1.33e7, 1.79e7, 2.14e7, 3.37e7, 5.79e7, 1.71e8 },
          std::vector<double>{  -20.19, -20.19, -19.97, -19.91, -19.92, -20.05, -20.10, -20.12, -20.17, -23.26, -23.10, -22.58, -21.35, -21.52, -20.18, -20.24, -21.06, -20.16, -20.43, -20.13, -20.31, -20.33, -20.68, -21.63 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ 
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.00845, 1.30975, 0.00914, 0.70892, 0.16266,
                 148.94, 598.94,  0.0, 3.0, 0.053), 
                // tetrafluoromethane, mean error: 2.6%, max error: 5.3%, range: 148.94-598.94K, 0-3MPa, stp estimate: 0.651
        /*thermal_conductivity*/   // 16.0 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00019, 1.49795, 0.00010, 0.95277, -0.00704,
                 148.94, 598.94,  0.0, 3.0, 0.084), 
                // tetrafluoromethane, mean error: 3.3%, max error: 8.4%, range: 148.94-598.94K, 0-3MPa, stp estimate: 0.015
        /*dynamic_viscosity*/      
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.22721, 1.71531, 0.16433, 0.82188, -0.84952,
                 148.94, 598.94,  0.0, 3.0, 0.049), 
                // tetrafluoromethane, mean error: 2.0%, max error: 4.9%, range: 148.94-598.94K, 0-3MPa, stp estimate: 15.680
        /*density*/                missing(),
        /*refractive_index*/       1.0004823
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (88.0043* si::kilogram * si::kelvin), 
                104600.0, -500.6, 2.2851, 0.0, 0.0,
                89.56, 145.1), 
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.20771, -0.00078883,  0.0,  0.0,  0.0,
                  89.56, 145.1), 
        /*dynamic_viscosity*/      
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -9.9212, 300.5, 0.0, 0.0, 0.0,
                  89.56, 145.1), // 89.56-145.1K
        /*density*/                // 1890.0 * si::kilogram/si::meter3, //pubchem
            get_dippr_temperature_function_105
                (si::kelvin, 88.0  * si::gram/si::decimeter3,
                 1.955, 0.27884, 227.51, 0.28571,
                 89.56, 227.51), // 89.56-227.51K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 61.89, -2296.3, -7.086, 0.000034687, 2.0,
                  89.56, 227.51), // 89.56-227.51K
        /*surface_tension*/        missing(),
        /*refractive_index*/       missing(),
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(1980.0 * si::kilogram/si::meter3), // pubchem
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-184.6, -169.3, -150.7, -127.7 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);

// benzene, C6H6
// representative of aromatic compounds, predominantly low-grade coal,
// and serves as a template for compounds in prebiotic chemistry such as nucleic acids or tholins
PartlyKnownCompound benzene (
    /*molar_mass*/                        79.102 * si::gram/si::mole, // wikipedia
    /*atoms_per_molecule*/                12u, // wikipedia
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0, // wikipedia
    /*acentric_factor*/                   0.211,

    /*critical_point_pressure*/           4.9 * si::megapascal,
    /*critical_point_volume*/             257.0*si::centimeter3/si::mole,
    /*critical_point_temperature*/        562.0 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       33.83 * si::kilojoule / (79.109 * si::gram), 
    /*latent_heat_of_fusion*/             9.87*si::kilojoule/(79.102*si::gram),
    /*triple_point_pressure*/             4.83 * si::kilopascal, // wikipedia data page
    /*triple_point_temperature*/          278.5 * si::kelvin, // wikipedia data page
    /*freezing_point_sample_pressure*/    si::atmosphere, 
    /*freezing_point_sample_temperature*/ 5.49 * si::celcius, 
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  80.1 * si::celcius, // Perry

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  3.63e6, 3.73e6, 3.75e6, 3.76e6, 3.82e6, 3.86e6, 3.87e6, 3.92e6, 3.95e6, 3.97e6, 4.03e6, 4.04e6, 4.07e6, 4.12e6, 4.14e6, 4.16e6, 4.21e6, 4.23e6, 4.25e6, 4.33e6, 4.52e6, 4.75e6, 4.91e6, 5.03e6, 5.33e6, 5.58e6, 5.85e6, 6.72e6, 7.54e6, 8.11e6, 8.79e6, 9.59e6, 1.03e7, 1.12e7, 1.41e7, 3.33e7, 2.11e8 },
          std::vector<double>{   -25.0,  -23.9,  -23.0,  -23.8,  -23.2,  -21.7,  -23.2,  -22.6,  -21.7,  -23.1,  -22.5,  -21.7,  -23.1,  -22.7,  -21.9,  -23.2,  -22.9,  -22.6,  -23.3,  -23.3,  -24.0,  -23.3,  -20.7,  -20.6,  -20.8,  -19.5,  -20.2,  -21.1,  -20.4,  -20.5,  -20.1,  -20.2,  -20.0,  -20.2,  -19.8,  -20.5,  -22.3 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 82.44 * si::joule / (79.109 * si::gram * si::kelvin),
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.11074, 0.98216, 0.00048, 1.26428, 0.48272,
                 300, 628.7,  0.0, 1.0, 0.071), 
                // benzene, mean error: 2.6%, max error: 7.1%, range: 300-628.7K, 0-1MPa, stp estimate: 1.069
        /*thermal_conductivity*/   
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 0.00001652, 1.3117, 491, 0.0,
                 339.15, 1000.0), // 339.15-1000K
        /*dynamic_viscosity*/      // 0.75e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::pascal * si::second,
                3.1340e-8, 0.9676, 7.9, 0.0,
                278.68, 1000.0), // 278.68-1000K
        /*density*/                missing(),
        /*refractive_index*/       missing(),
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 134.8 * si::joule / (79.109 * si::gram * si::kelvin),
            get_interpolated_temperature_function
                ( si::kelvin, si::joule/(si::gram * si::kelvin),
                  std::vector<double>{278.7,300.0,320.0,450.0,500.0,550.0},
                  std::vector<double>{1.708,1.706,1.766,2.279,2.637,4.765}),
        /*thermal_conductivity*/   // 0.1411 * si::watt / ( si::meter * si::kelvin ),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.23444, -0.00030572, 0.0,  0.0,  0.0,
                  278.68, 413.1), 
        /*dynamic_viscosity*/      // 0.601 * si::millipascal * si::second, // engineering toolbox, at 300K
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 7.5117, 294.68, -2.794, 0.0, 0.0,
                  278.68, 545), // 278.68-545K
        /*density*/                // 879.0 * si::kilogram/si::meter3, // Perry
            get_dippr_temperature_function_105
                (si::kelvin, 79.102 * si::gram/si::decimeter3,
                 1.0259, 0.266666, 562.05, 0.28394,
                 278.68, 562.05), // 278.68-562.05K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 83.107, -6486.2, -9.2194, 6.9844e-06, 2.0,
                  278.68, 562.05), // 278.68-562.05K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 562.0, 0.07298, 1.232, -0.0007802, 0.8635, -0.0001756, 0.3065,
                 272.95, 553.15), // Mulero (2012)
        /*refractive_index*/       1.5011,
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     double l2 = l*l;
        //     double invl2 = 1.0/(l*l);
        //     return sqrt(
        //         2.170184597f
        //         + 0.00059399 * l2
        //         + 0.02303464 * invl2
        //         - 0.000499485 * invl2*invl2
        //         + 0.000178796 * invl2*invl2*invl2
        //     );
        // }),
        /*extinction_coefficient*/        missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    (si::kelvin, si::joule / (79.109*si::gram*si::kelvin),
                     std::vector<double>{0.0, 278.7}, 
                     std::vector<double>{0.0, 118.4}), // wikipedia data page
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           missing(),
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-36.7,  -11.5,  26.1,   80.1   }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);

// pyramidine, C4H4N2
// representative of prebiotic chemistry and tholins,
// since it's been observed from Kawai (2019) that tholins may contain them among other nucleotides
PartlyKnownCompound pyrimidine (
    /*molar_mass*/                        80.088 * si::gram/si::mole, // wikipedia
    /*atoms_per_molecule*/                10u, // wikipedia
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0, // wikipedia
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       49.79*79 * si::kilojoule / (80.088*si::gram),
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          155.1 * si::kelvin, // NIST
    /*freezing_point_sample_pressure*/    si::atmosphere, 
    /*freezing_point_sample_temperature*/ 293.0 * si::kelvin, // wikipedia
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  397.0 * si::kelvin, // wikipedia

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      missing(),
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      missing(),
        /*density*/                state_invariant(1.016 * si::gram/si::centimeter3), // wikipedia
        /*vapor_pressure*/         missing(),
        /*surface_tension*/        
            get_interpolated_temperature_function
                (si::celcius, si::millinewton/si::meter,
                 std::vector<double>{ 25.0,  50.0,  75.0, 100.0 }, 
                 std::vector<double>{30.33, 27.80, 25.28, 22.75 }), 
        /*refractive_index*/       missing(),
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           missing(),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);

// halite, NaCl, sodium chloride, table salt
// for salt bed flats and potentially modeling ocean salinity
PartlyKnownCompound  halite (
    /*molar_mass*/                        90.442*si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      missing(),
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           26.0 * si::megapascal, // wikipedia data page
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        3900.0 * si::kelvin, // wikipedia data page
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       40810.0 * si::calorie/(80.088 * si::gram), // Perry
    /*latent_heat_of_fusion*/             28.16*si::kilojoule/(90.442*si::gram),
    /*triple_point_pressure*/             30.0 * si::pascal, // wikipedia data page
    /*triple_point_temperature*/          1074.0 * si::kelvin, // wikipedia data page
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 800.7*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  1413.0 * si::celcius, // Perry
    
    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      missing(),
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ state_invariant(15.9 * si::calorie/(80.088 * si::gram*si::kelvin)), // Perry, 1073-1205K
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      
            get_interpolated_temperature_function
                (si::kelvin, si::millipascal*si::second,
                 std::vector<double>{1073.15,  1270.15  }, 
                 std::vector<double>{1.57,     0.708    }), // Dortmund data bank
        /*density*/                1549.0 * si::kilogram/si::meter3,  // wikipedia data page
        /*vapor_pressure*/         
            get_interpolated_temperature_function
                (si::celcius, si::pascal,
                  std::vector<double>{835.0,     987.0,     1461.0}, 
                 std::vector<double>{100.0 ,     1e3,       100e3}),
        /*surface_tension*/        
            get_interpolated_temperature_function
                (si::celcius, si::dyne/si::centimeter,
                 std::vector<double>{ 1080.0, 1250.0 },
                 std::vector<double>{  112.5,  102.5 }), // Sato (1990)
        /*refractive_index*/       missing(),
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   // 50.0 * si::joule / (90.442 * si::gram * si::kelvin), // wikipedia data page
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(80.088 * si::gram*si::kelvin), 
                     10.79, 0.00420, 0.0, 0.0,
                      273.0, 1074.0),
            /*thermal_conductivity*/              state_invariant(5.55 * si::watt / ( si::meter * si::kelvin )), // Wilkens (2011)
            /*dynamic_viscosity*/                 state_invariant(1e17 * si::poise), // various sources, Carey (1953) cites this number from Weinberg (1927), and Mukherjee (2010), provides a literature review and findings from salt diapirs. Science is weird.
            /*density*/                           state_invariant(2170.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{865.0,  1017.0, 1220.0, 1460.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/      
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{  0.22,   0.24,   0.26,   0.28,  23.81,  26.32,  27.78,  29.41,  31.25,  33.33,  38.46,  41.67,  45.45,  50.00,  55.56,  62.50,  71.43,  83.33, 100.00, 125.00, 166.67},
                     std::vector<double>{1.66, 1.71, 1.68, 1.64, 1.30, 1.23, 1.17, 1.09, 1.00, 0.87, 0.48, 0.50, 0.27, 0.19, 0.43, 6.29, 3.66, 3.05, 2.76, 2.58, 2.49}),
                    // Querry (1987)
            /*extinction_coefficient*/ 
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.22,  26.32,  27.78,  31.25,  33.33,  35.71,  38.46,  41.67,  45.45,  50.00,  55.56,  62.50,  71.43,  83.33, 100.00, 166.67},
                     std::vector<double>{0.000, 0.014, 0.011, 0.049, 0.102, 0.172, 0.617, 0.792, 1.147, 1.970, 3.582, 1.948, 0.226, 0.095, 0.077, 0.056}),
                    // Querry (1987)
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for halite
                    (si::micrometer,  162 *si::micrometer, 
                     std::vector<double>{  0.2183,  0.2295,  0.2439,  0.2615,  0.2743,  0.2903,  0.3207,  0.4455,  0.5656,  0.7928,  1.0617,  1.3802,  1.4523,  1.5531,  1.8588,  1.9292,  1.9900,  2.0605,  2.1309,  2.2349,  2.3549,  2.6558,  2.7247,  2.7855,  2.8479,  2.8847,  2.9119,  2.9439,  2.9807,  8.1565,  9.3855, 10.7373, 12.2121, 12.7037, 18.1112, 27.6972, 30.1552, 32.4903, 35.1940, 37.1604, 38.3893, 40.2328, 42.4450, 43.7969, 45.7632, 48.5899, 50.0646, 51.1707, 55.1034, 56.4553, 58.1759, 60.0194, 65.5498, 67.7619, 71.4489, 73.7839, 78.5769, 86.4424, 93.0789, 98.7322,109.9159,114.4631,127.7360,146.2936,153.9133,167.1862 },
                     std::vector<double>{ 0.59984, 0.63333, 0.64910, 0.64975, 0.54138, 0.67077, 0.71872, 0.81461, 0.86059, 0.89146, 0.90263, 0.90328, 0.89343, 0.90657, 0.90394, 0.87176, 0.87241, 0.90066, 0.90985, 0.90394, 0.90197, 0.86256, 0.78112, 0.55057, 0.45271, 0.44154, 0.41921, 0.40805, 0.41724, 0.87214, 0.82290, 0.83550, 0.78511, 0.75534, 0.60191, 0.04198, 0.00420, 0.00305, 0.03740, 0.17023, 0.22405, 0.14504, 0.22519, 0.23435, 0.36260, 0.44275, 0.50573, 0.55840, 0.66489, 0.67405, 0.66718, 0.62137, 0.24466, 0.18969, 0.16450, 0.14389, 0.12557, 0.13130, 0.11412, 0.11756, 0.10611, 0.10382, 0.12214, 0.11641, 0.11756, 0.13244 }),

            /*bulk_modulus*/                      state_invariant(24.8 * si::gigapascal), // Mavko (2009)
            /*tensile_modulus*/                   missing(),                              // state_invariant(0.4947e11*si::pascal), // values from Perry produce negative bulk modulus so they are commented out
            /*shear_modulus*/                     state_invariant(14.9 * si::gigapascal), // state_invariant(0.1287e11*si::pascal), // values from Perry produce negative bulk modulus so they are commented out
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.25), // Mavko (2009)

            /*compressive_fracture_strength*/     state_invariant(20.0 * si::megapascal), // Bauer (2019)
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        state_invariant(15.0 * si::megapascal), // Bauer (2019)
            /*tensile_yield_strength*/            state_invariant(1.65 * si::megapascal), // https://material-properties.org/strength-of-materials-tensile-yield/
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);

// corundum, Al2O3, aluminum sequioxide, aluminum oxide, alumina, ruby, sapphire, beryl
// representative of precious stones, excluding diamond
PartlyKnownCompound  corundum (
    /*molar_mass*/                        101.96 * si::gram/si::mole, 
    /*atoms_per_molecule*/                5u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             900.0*si::kilojoule / si::kilogram,
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,     // Perry
    /*freezing_point_sample_temperature*/ 2000.0*si::celcius, // Perry
    /*boiling_point_sample_pressure*/     si::atmosphere,     // Perry
    /*boiling_point_sample_temperature*/  2210.0*si::celcius, // Perry

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      missing(),
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   missing(),
        // .dynamic_viscosity = 0.035 * si::pascal * si::second, // Blomquist (1978)
        /*dynamic_viscosity*/      
            field::StateFunction<si::dynamic_viscosity>([](const si::pressure p, const si::temperature T){ 
                return exp((11448.0/std::clamp(T/si::kelvin, 2400.0, 2750.0) - 8.2734))*si::pascal*si::second; 
            }), // Blomquist (1978)
        /*density*/                missing(),
        /*vapor_pressure*/         missing(),
        /*surface_tension*/        missing(),
        /*refractive_index*/       missing(),
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   // 750.0*si::joule/(si::kilogram* si::kelvin)
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(101.96 * si::gram*si::kelvin), 
                     22.08, 0.008971, -522500.0, 0.0,
                     273.0, 1973.0), 
            /*thermal_conductivity*/              // field::StateSample<si::thermal_conductivity>(37.0*si::watt/(si::meter * si::kelvin), si::atmosphere, 20.0*si::celcius),//azom.com/article.aspx?ArticleId=1948
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{10.0, 60.0,  300.0, 400.0, 1400.0}, 
                     std::vector<double>{7.0,  174.0,  36.0,  26.0,    6.0}), // Perry
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(3970.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  7.94,  8.33,  8.55,  9.26, 10.64, 10.75, 10.99, 11.11, 11.24, 11.36, 14.49, 14.71, 14.93, 15.15, 15.38, 15.62, 15.87, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 33.33, 34.48, 37.04, 55.56},
                     std::vector<double>{1.831, 1.345, 1.294, 1.243, 1.113, 0.592, 0.514, 0.279, 0.182, 0.124, 0.098, 0.117, 0.135, 0.172, 0.243, 0.369, 0.664, 1.672, 0.735, 0.774, 1.062, 1.523, 2.382, 5.146, 8.559, 6.585, 5.308, 4.497, 3.797, 3.232, 2.725, 1.951, 0.629, 0.283, 0.512, 1.472, 7.250, 9.484, 6.881, 5.718, 5.020, 3.879, 5.677, 4.903, 4.560, 4.279, 4.122, 4.000, 3.893, 3.723, 3.667, 3.617, 3.326}),
                    // Querry (1985)
            /*extinction_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  7.87,  8.62,  9.09,  9.52, 10.75, 10.87, 10.99, 11.11, 11.36, 13.89, 14.08, 14.29, 14.49, 14.71, 14.93, 15.15, 15.38, 15.62, 15.87, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 30.30, 31.25, 32.26, 33.33, 34.48, 41.67, 55.56},
                     std::vector<double>{-0.052, 0.044, 0.068, 0.080, 0.089, 0.139, 0.166, 0.239, 0.355, 0.608, 2.178, 2.311, 2.466, 2.645, 2.838, 3.049, 3.326, 3.683, 4.250, 3.460, 3.626, 4.260, 4.919, 5.835, 6.996, 8.491, 3.839, 1.299, 0.642, 0.420, 0.480, 0.662, 0.591, 0.573, 1.220, 2.824, 4.730, 7.373,11.173, 1.231, 0.312, 0.108, 0.066, 0.606, 1.312, 0.108,-0.069,-0.089, 0.010, 0.046, 0.019, 0.013, 0.077, 0.013, 0.011}),
                    // Querry (1985)
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for corundum
                    (si::micrometer,  162 *si::micrometer, 
                     std::vector<double>{ 0.22736, 0.27200, 0.29592, 0.31824, 0.34216, 0.36129, 0.36926, 0.37883, 0.39318, 0.39636, 0.41071, 0.43144, 0.45217, 0.47449, 0.47608, 0.48884, 0.55261, 0.57334, 0.63552, 0.69133, 0.72162, 0.84917, 0.85236, 0.87468, 0.90019, 0.93846, 1.08036, 1.14573, 1.30676, 1.38489, 1.41199, 1.44866, 1.50925, 1.54273, 1.77232, 1.81218, 1.84885, 1.89509, 1.93973, 1.96524, 2.00510, 2.05134, 2.15179, 2.30485, 2.38297, 2.43399, 2.59343, 2.65402, 2.75128, 2.78795, 2.85013, 2.94420, 2.97449 },
                     std::vector<double>{  0.0529,  0.0582,  0.0794,  0.2286,  0.2561,  0.4741,  0.4878,  0.4455,  0.4561,  0.5175,  0.5767,  0.5968,  0.5407,  0.6159,  0.6339,  0.6508,  0.5788,  0.5778,  0.6106,  0.6233,  0.6360,  0.7249,  0.7365,  0.7418,  0.7566,  0.7672,  0.7767,  0.7926,  0.8593,  0.8677,  0.8550,  0.8614,  0.8392,  0.8127,  0.5302,  0.5153,  0.5164,  0.5323,  0.5365,  0.5249,  0.4963,  0.4878,  0.4910,  0.4772,  0.4561,  0.4265,  0.3873,  0.3820,  0.2476,  0.2233,  0.1577,  0.1016,  0.0984 }),

            /*bulk_modulus*/                      252.9 * si::gigapascal, // Mavko (2009)
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     162.1 * si::gigapascal, // Mavko (2009)
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     0.24, // Mavko (2009)

            /*compressive_fracture_strength*/     state_invariant(2265.0*si::megapascal), //azom.com/article.aspx?ArticleId=1948
            /*tensile_fracture_strength*/         state_invariant(325.0*si::megapascal), //azom.com/article.aspx?ArticleId=1948
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  false
        }
    }
);


// apatite, XCa5(PO4)3
// theoretical biomineral deposits, analogous to limestone, that could occur on alien planets
PartlyKnownCompound  apatite (
    /*molar_mass*/                        509.1 * si::gram/si::mole, 
    /*atoms_per_molecule*/                21u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    missing(),
    /*freezing_point_sample_temperature*/ missing(),
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/                 missing(),
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()
    },



    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilojoule / (si::kilogram*si::kelvin),
                     std::vector<double>{0.0, 300.0}, 
                     std::vector<double>{0.0,   0.7}), // Schön (2015)
            /*thermal_conductivity*/              state_invariant(1.38 * si::watt / (si::meter*si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(3180.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  1.645,
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   state_invariant(1.667e11*si::pascal),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.26), // Schön (2015)

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// carbon, C
// for diamonds, graphite, high-grade anthracite coal, 
// and theoretical exobiominerals deposits analogous to limestone  // TODO: custom polymorphic class, with complex phase diagram
PartlyKnownCompound carbon (
    /*molar_mass*/                        12.011 * si::gram/si::mole, 
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           2200.0 * si::atmosphere, // Leider (1973)
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        6810.0 * si::kelvin,     // Leider (1973)
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       355.8*si::kilojoule/(12.011*si::gram),
    /*latent_heat_of_fusion*/             117.4*si::kilojoule/(12.011*si::gram),
    /*triple_point_pressure*/             107.0 * si::atmosphere,  // Haaland (1976)
    /*triple_point_temperature*/          4765.0 * si::kelvin,       // Leider (1973)
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 3500.0 *si::celcius, // Perry, lower bound, for both forms
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  4200.0 *si::celcius, // Perry, lower bound, for both forms

    /*phase*/ // estimated from phase diagram
    field::StateFunction<int>([]
            (const si::pressure pressure, 
             const si::temperature temperature)
            {
                const double supercritical = -3;
                const double vapor = -2;
                const double liquid = -1;
                const double graphite = 0;
                const double diamond = 1;
                const double p = pressure / si::pascal;
                const double t = temperature / si::kelvin;
                const     double Ru(8.3144598);           // universal gas constant
                const     double M (0.012);               // molar mass, kg/mol
                const     double Lv(355.8*si::kilojoule/(12.011*si::gram) /(si::joule/si::kilogram)); // specific latent heat of vaporization (J/kg)
                const     double p0(107e5);               // triple point pressure (Pa)
                const     double t0(4765);                // triple point temperature (K)
                constexpr double R (Ru/M);                // individual gas constant
                constexpr double mv(-R/Lv);               // slope of clapeyron equation for vaporus
                constexpr double bv(1/t0 - mv * std::log(p0)); // intercept for clapeyron equation for vaporus
                const     double ml(1e-3);                // slope of clapeyron equation repurposed for liquidus, estimated from phase diagram
                const     double bl(-3.411e-5);           // intercept for clapeyron equation repurposed for liquidus, estimted from phase diagram
                if ( t > 1/(mv*std::log(p)+bv) )
                {
                    if (t > 6810 && p > 2.2e8)
                    {
                        return supercritical; // based on Leider (1973)
                    }
                    return vapor;
                }
                else if ( t < t0 && p < 1e10 )
                {
                    return graphite;
                }
                else if ( t > 1/(ml*std::log(p)+bl) )
                {
                    return liquid;
                }
                else
                {
                    return diamond;
                }
            }
        ),

    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ state_invariant(26.0 * si::joule / (12.011 * si::gram * si::kelvin)), // Steinbeck (1990)
            // NOTE: the following equation is reported, but fails dimensionality check
            // field::StateFunction<si::isobaric_specific_heat_capacity>([](const si::pressure p, const si::temperature T){
            //     const si::energy fermi_energy = 16.9 * si::electronvolt;
            //     const double pi = 3.1415926;
            //     return 3.0*si::universal_gas_constant + pi*pi*si::universal_gas_constant*si::boltzmann_constant*T/(2.0*fermi_energy);
            // }), // Steinbeck (1990)
        /*thermal_conductivity*/   // 2.9 * si::watt / (si::centimeter*si::kelvin), // Steinbeck (1990)
            field::StateFunction<si::thermal_conductivity>([](const si::pressure p, const si::temperature T){
                const double pi = 3.1415926;
                const auto rho1 = 39.5*si::microohm*si::centimeter;
                return pi*pi*si::boltzmann_constant*si::boltzmann_constant*T  /
                    (3.0*si::elementary_charge_constant*si::elementary_charge_constant*rho1);
            }), // Steinbeck (1990)
        /*dynamic_viscosity*/      missing(),
        /*density*/                state_invariant(1.6*si::gram/si::centimeter3), // Bundy, referenced in Steinbeck (1990)
        /*vapor_pressure*/         
                get_interpolated_temperature_function
                    (si::celcius, si::pascal,
                     std::vector<double>{2566.0,     3016.0,     3635.0}, 
                     std::vector<double>{1.0 ,       1e3,        100e3}),
                                                                         // TOOD: autocomplete vapor pressure for solids/liquids if function is present for other phase
        /*surface_tension*/        missing(),
        /*refractive_index*/       missing(),
        /*extinction_coefficient*/ missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid { // graphite
            /*isobaric_specific_heat_capacity*/   // 0.710 * si::joule / (si::gram * si::kelvin), // wikipedia, Diamond is 0.5091
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     1e-5, 1.0/970e3, 10.0,
                     si::calorie/(12.011 * si::gram*si::kelvin),
                     2.673, 0.002617, -116900.0, 0.0,
                     273.0, 1373.0), // Johnson (1960) and Perry
            /*thermal_conductivity*/              // 247.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia (middle range value)
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{  7.0, 10.0, 100.0,  300.0, 1400.0}, 
                     std::vector<double>{ 15.0, 81.0,4980.0, 2000.0,  370.0}), // Johnson (1960) and Perry, perpendicular to basal plane
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2260.0 * si::kilogram/si::meter3), 
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::pascal,
                     std::vector<double>{2566.0,     3016.0,     3635.0}, 
                     std::vector<double>{10.0 ,      1e3,        100e3}),
            /*refractive_index*/      
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.2, 0.22, 0.23, 0.24, 0.26, 0.27, 0.28, 0.29, 3.9526, 4.2017, 4.8544, 4.902, 5, 5.0505, 6.25, 6.2893, 6.3291, 6.4516, 8.0645, 9.009, 10.2041, 10.3093, 11.3636, 11.4943, 11.6279, 12.3457, 14.4928, 14.9254, 15.3846, 15.625, 15.873, 16.129, 20, 20.4082, 20.8333, 21.2766, 22.2222, 23.8095, 25.641, 27.027, 27.7778, 28.5714, 29.4118, 30.303, 31.25, 32.2581, 33.3333, 34.4828, 35.7143, 37.037, 38.4615, 40, 41.6667, 43.4783, 45.4545, 47.619, 50, 52.6316, 55.5556},
                     std::vector<double>{0.84, 0.962, 1.045, 1.162, 1.49, 1.591, 1.646, 1.679, 3.747, 3.844, 4.141, 4.173, 4.192, 4.237, 4.693, 4.713, 4.764, 4.78, 5.113, 5.26, 5.458, 5.458, 5.572, 5.571, 5.616, 5.677, 5.877, 5.935, 5.971, 5.972, 5.996, 6.042, 6.438, 6.46, 6.511, 6.582, 6.686, 6.95, 7.247, 7.469, 7.602, 7.709, 7.83, 8.035, 8.224, 8.425, 8.682, 8.969, 9.225, 9.586, 9.984, 10.214, 10.813, 10.952, 11.266, 11.898, 12.39, 13.618, 14.474}),
                    // Querry (1985)
            /*extinction_coefficient*/ 
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.2, 0.24, 0.25, 0.26, 0.28, 0.29, 2.7322, 3.7736, 3.7879, 4.1841, 4.878, 5, 5.0251, 5.7143, 6.25, 6.2893, 6.3291, 6.3694, 6.4103, 8.1301, 8.9286, 10.101, 10.2041, 11.236, 11.3636, 11.4943, 11.6279, 11.7647, 12.5, 12.6582, 14.7059, 15.625, 15.873, 19.2308, 20, 20.4082, 20.8333, 21.7391, 22.7273, 23.2558, 24.3902, 25, 27.7778, 28.5714, 30.303, 31.25, 33.3333, 34.4828, 35.7143, 37.037, 38.4615, 41.6667, 43.4783, 45.4545, 47.619, 50, 52.6316, 55.5556},
                     std::vector<double>{0.723, 1.162, 1.221, 1.195, 1.061, 1.009, 2.395, 2.762, 2.785, 2.91, 3.124, 3.154, 3.179, 3.254, 3.312, 3.347, 3.343, 3.323, 3.316, 3.489, 3.598, 3.774, 3.776, 3.927, 3.934, 3.979, 4.009, 4.015, 4.153, 4.168, 4.541, 4.705, 4.784, 5.434, 5.606, 5.702, 5.823, 6.02, 6.281, 6.393, 6.645, 6.755, 7.307, 7.462, 7.891, 8.052, 8.482, 8.664, 8.868, 9.1, 9.171, 9.559, 9.451, 9.974, 10.13, 10.524, 10.803, 10.332}),
                    // Querry (1985)
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for carbon
                    (si::micrometer,  0.17 *si::micrometer, 
                     std::vector<double>{ 0.22365, 0.23614, 0.25332, 0.27518, 0.28767, 0.32670, 0.32670, 0.33607, 0.35012, 0.35636, 0.36885, 0.44692, 0.65301, 0.77635, 0.82162, 0.82943, 0.84348, 0.85129, 0.86066, 0.87158, 0.88407, 0.89344, 0.91218, 0.93247, 0.94965, 1.44926, 1.53044, 1.55386, 1.57884, 1.96916, 1.98322, 2.02849, 2.04411, 2.06909, 2.08314, 2.18462, 2.19243, 2.28142, 2.30172, 2.31577, 2.32514, 2.36417, 2.37510, 2.41569, 2.46409, 2.49532, 2.52654, 2.58743, 2.62334, 2.66081, 2.68735, 2.75293, 2.81538, 2.84660, 2.87939, 2.90906 },
                     std::vector<double>{ 0.00272, 0.01331, 0.01585, 0.01762, 0.01675, 0.01778, 0.01778, 0.01685, 0.01710, 0.01630, 0.01646, 0.01572, 0.01534, 0.01556, 0.01463, 0.01303, 0.01303, 0.01540, 0.01547, 0.01672, 0.01649, 0.01598, 0.01643, 0.01736, 0.01636, 0.01351, 0.01409, 0.01315, 0.01354, 0.01238, 0.01091, 0.01290, 0.01075, 0.01219, 0.01081, 0.01235, 0.01139, 0.00924, 0.00503, 0.01884, 0.01184, 0.01165, 0.00998, 0.01181, 0.01014, 0.01071, 0.00988, 0.00921, 0.01049, 0.01068, 0.01059, 0.02173, 0.00317, 0.01152, 0.00359, 0.02734 }),


            /*bulk_modulus*/                      missing(), 
            /*tensile_modulus*/                   missing(), 
            /*shear_modulus*/                     missing(), 
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            state_invariant(14.0 * si::megapascal), // https://material-properties.org/strength-of-materials-tensile-yield/
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        },
        phase::PartlyKnownSolid { // diamond
            /*isobaric_specific_heat_capacity*/   // 0.5091* si::joule / (si::gram * si::kelvin), // wikipedia 
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     3e-6, 1.0/67e6, 30.0,
                     si::calorie/(12.011 * si::gram*si::kelvin),
                     2.162, 0.003059, -130300.0, 0.0, 
                     273.0, 1313.0), // Johnson (1960) and Perry
            /*thermal_conductivity*/              state_invariant(2200.0 * si::watt / ( si::meter * si::kelvin )), //wikipedia 
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(3513.0  * si::kilogram/si::meter3), 
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/      
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.035,  0.058,  0.058,  0.059,  0.076,  0.083,  0.084,  0.091,  0.093,  0.094,  0.095,  0.097,  0.098,  0.100,  0.102,  0.103,  0.105,  0.107,  0.109,  0.111,  0.113,  0.117,  0.122,  0.124,  0.129,  0.135,  0.148,  0.155,  0.159,  0.172,  0.177,  0.182,  0.188,  0.200,  0.230, 10.000},
                     std::vector<double>{ 0.58,   0.46,   0.46,   0.45,   0.78,   0.94,   0.97,   1.09,   1.10,   1.14,   1.21,   1.34,   1.68,   2.08,   2.55,   2.85,   3.06,   3.28,   3.44,   3.51,   3.49,   3.50,   3.45,   3.42,   3.32,   3.26,   3.22,   3.25,   3.28,   3.42,   3.28,   3.11,   2.99,   2.86,   2.69, 2.38  }),
                    // Philip (1964)
            /*extinction_coefficient*/ 
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.035, 0.056, 0.057, 0.058, 0.058, 0.060, 0.063, 0.087, 0.090, 0.091, 0.093, 0.094, 0.095, 0.097, 0.098, 0.100, 0.102, 0.103, 0.105, 0.107, 0.109, 0.111, 0.113, 0.115, 0.117, 0.119, 0.129, 0.163, 0.168, 0.172, 0.177, 0.182,10.000},
                     std::vector<double>{0.242, 0.858, 0.956, 1.023, 1.046, 1.120, 1.241, 2.111, 2.257, 2.354, 2.466, 2.608, 2.777, 3.017, 3.372, 3.511, 3.516, 3.220, 2.970, 2.675, 2.398, 2.174, 1.919, 1.749, 1.607, 1.449, 1.071, 0.614, 0.507, 0.386, 0.088, 0.029, 0.000}),
                    // Philip (1964)
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      state_invariant(443.0 * si::gigapascal),  // wikipedia, for diamond
            /*tensile_modulus*/                   state_invariant(1050.0 * si::gigapascal), // wikipedia, for diamond
            /*shear_modulus*/                     state_invariant(478.0 * si::gigapascal),  // wikipedia, from McSkimin (1972), for diamond
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// calcite, CaCO3, calcium carbonate
// for biomineral deposits like limestone  // TODO: custom polymorphic class, with complex phase diagram
PartlyKnownCompound  calcite (
    /*molar_mass*/                        100.087 * si::gram/si::mole, 
    /*atoms_per_molecule*/                4u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 823.0 * si::celcius, // for calcite, 1339 for aragonite
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/                 missing(),
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()
    },

    

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid { // calcite

            /*isobaric_specific_heat_capacity*/   // 0.793 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988, room temperature, 0.79 for aragonite
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(100.087 * si::gram*si::kelvin), 
                     19.68, 0.01189, -307600.0, 0.0, 
                     273.0, 1033.0), 
            /*thermal_conductivity*/              state_invariant(3.59 * si::watt / (si::meter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2710.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  
            // // TODO: reimplement this
            // field::SpectralFunction<double>([](
            //     const si::wavenumber nlo, 
            //     const si::wavenumber nhi, 
            //     const si::pressure p, 
            //     const si::temperature T
            // ) {
            //     double l = (2.0 / (nhi+nlo) / si::micrometer);
            //     return sqrt(
            //         1.0
            //         + 0.73358749f 
            //         + 0.96464345 * l*l / (l*l - 1.94325203f)
            //         + 1.82831454 * l*l / (l*l - 120.0)
            //     );
            // }),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for calcite
                    (si::micrometer,  410 *si::micrometer, 
                    std::vector<double>{  0.246,  0.271,  0.279,  0.327,  0.358,  0.450,  0.501,  0.818,  0.844,  0.920,  1.034,  1.451,  1.492,  1.584,  1.606,  1.709,  1.749,  1.777,  1.815,  1.855,  1.876,  1.898,  1.915,  1.947,  1.963,  1.983,  1.996,  2.026,  2.053,  2.090,  2.109,  2.136,  2.143,  2.162,  2.186,  2.226,  2.258,  2.296,  2.307,  2.335,  2.380,  2.389,  2.419,  2.500,  2.529,  2.560,  2.592,  2.627,  2.659,  2.690,  2.757,  2.785,  2.814,  2.879,  2.914,  2.946,  2.979,  1.354,  2.709,  3.694,  4.433,  5.295,  7.266,  8.497, 12.931, 15.270, 16.871, 17.610, 19.088, 20.812, 23.522, 26.477, 30.665, 33.251, 34.482, 37.438, 38.546, 41.748, 43.472, 46.305, 48.275, 49.137, 50.000, 51.724, 52.955, 54.433, 55.911, 56.773, 60.591, 61.453, 62.807, 64.162, 66.379, 69.088, 70.197, 72.044, 73.891, 77.463, 79.926, 82.142, 85.344, 87.807, 97.536, 99.753,101.477,103.571,107.512,112.561,117.487,123.522,129.433,132.758,136.330,140.147,143.842,148.522,152.586,156.896,178.940,191.871,199.630,207.019,216.1330},
                    std::vector<double>{  0.709,  0.715,  0.743,  0.794,  0.797,  0.919,  0.936,  0.955,  0.960,  0.953,  0.964,  0.958,  0.962,  0.957,  0.960,  0.947,  0.937,  0.951,  0.940,  0.910,  0.864,  0.925,  0.929,  0.921,  0.900,  0.887,  0.865,  0.929,  0.940,  0.936,  0.921,  0.908,  0.877,  0.868,  0.893,  0.859,  0.789,  0.656,  0.648,  0.573,  0.810,  0.829,  0.812,  0.552,  0.490,  0.601,  0.775,  0.841,  0.844,  0.800,  0.602,  0.496,  0.609,  0.709,  0.677,  0.682,  0.660,  0.580,  0.563,  0.071,  0.459,  0.024,  0.055,  0.245,  0.239,  0.199,  0.089,  0.084,  0.038,  0.019,  0.010,  0.078,  0.099,  0.128,  0.117,  0.133,  0.122,  0.033,  0.066,  0.102,  0.158,  0.157,  0.174,  0.183,  0.172,  0.176,  0.163,  0.183,  0.160,  0.165,  0.158,  0.183,  0.100,  0.092,  0.066,  0.066,  0.038,  0.039,  0.021,  0.041,  0.053,  0.071,  0.116,  0.103,  0.099,  0.104,  0.086,  0.111,  0.108,  0.147,  0.134,  0.141,  0.166,  0.178,  0.146,  0.157,  0.161,  0.177,  0.209,  0.171,  0.215,  0.355,  0.4454}),

            /*bulk_modulus*/                      state_invariant(73.0 * si::gigapascal), // Schön (2015)
            /*tensile_modulus*/                   missing(),                                              
            /*shear_modulus*/                     state_invariant(32.0 * si::gigapascal), // Schön (2015) 
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.32), // Schön (2015)

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        },
        phase::PartlyKnownSolid { // aragonite

            /*isobaric_specific_heat_capacity*/   // 0.793 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988, room temperature, 0.79 for aragonite
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(100.087 * si::gram*si::kelvin), 
                     19.68, 0.01189, -307600.0, 0.0, 
                     273.0, 1033.0), 
            /*thermal_conductivity*/              state_invariant(2.24 * si::watt / (si::meter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2930.0 * si::kilogram/si::meter3), // Perry
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      state_invariant(47.0 * si::gigapascal), // Schön (2015)
            /*tensile_modulus*/                   missing(),                                              
            /*shear_modulus*/                     state_invariant(38.5 * si::gigapascal), // Schön (2015) 
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.18), // Schön (2015)

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// quartz, SiO2, silicon dioxide, silica, glass
// representative of felsic rocks, namely sand, and biomineral deposits like diatomaceous earth  // TODO: custom polymorphic class, with complex phase diagram
PartlyKnownCompound  quartz (
    /*molar_mass*/                        60.08 * si::gram/si::mole, 
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           1.7e8 * si::pascal,
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        5300.0* si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       11770e3 * si::joule / si::kilogram,
    /*latent_heat_of_fusion*/             9.76*si::kilojoule/(60.08*si::gram), // cristobalite
    /*triple_point_pressure*/             0.0003 * si::pascal,
    /*triple_point_temperature*/          1983.0 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 1425.0 * si::celcius, // Perry
    /*boiling_point_sample_pressure*/     si::atmosphere, 
    /*boiling_point_sample_temperature*/  2230.0 * si::celcius, // Perry

    /*phase*/
    field::StateFunction<int>([]
            (const si::pressure p, 
             const si::temperature T)
            {
                // const int supercritical = -3;
                // const int gas = -2;
                // const int liquid = -1;
                const int beta = 0;
                const int alpha = 1;
                const int cristobalite_alpha = 2;
                const int cristobalite_beta = 3;
                const int coesite = 4;
                const int stishovite = 5; // unused

                // triple point between solid, liquid, and gas
                const si::pressure        p0 = 0.0003 * si::pascal;
                const si::temperature     T0 = 1983.0 * si::kelvin;
                // triple point between alpha, beta, and coesite
                const si::pressure        pabc = 3.1 * si::gigapascal;
                const si::temperature     Tabc = 1190.0 * si::celcius;
                // sample point between solid and liquid
                const si::pressure        pf = si::atmosphere;
                const si::temperature     Tf = 1425.0 * si::celcius;
                // sample point between stishovite and coesite
                const si::pressure        psc0 = 7.75 * si::gigapascal;
                const si::temperature     Tsc0 = 0.0 * si::celcius;
                // sample point between stishovite and coesite
                const si::pressure        psc1 = 11.0 * si::gigapascal;
                const si::temperature     Tsc1 = 1400.0 * si::celcius;
                // sample point between alpha and coesite
                const si::pressure        pac = 1.8 * si::gigapascal;
                const si::temperature     Tac = 0.0 * si::celcius;
                // sample point between beta and coesite
                const si::pressure        pbc = 4.3 * si::gigapascal;
                const si::temperature     Tbc = 2200.0 * si::celcius;
                // sample point between alpha and beta
                const si::pressure        pab = 0.0 * si::gigapascal;
                const si::temperature     Tab = 573.0 * si::celcius;
                // sample point between beta and cristobalite alpha
                const si::pressure        pbca = 0.0 * si::gigapascal;
                const si::temperature     Tbca = 870.0 * si::celcius;
                // sample point between beta and cristobalite beta
                const si::pressure        pbcb = 1.0 * si::gigapascal;
                const si::temperature     Tbcb = 1705.0 * si::celcius;
                // sample point between cristobalite alpha and cristobalite beta
                const si::temperature     Tcacb = 1470.0 * si::celcius;
                // critical point
                const si::pressure        pc = 1.7e8 * si::pascal;
                const si::temperature     Tc = 5300.0* si::kelvin;
                // latent heat of vaporization and molar mass
                const si::specific_energy L  = 11770e3 * si::joule / si::kilogram;
                const si::molar_mass      M  = 60.08 * si::gram/si::mole;

                const int basic_phase = property::get_basic_phase(p,T,p0,T0,pc,Tc,pf,Tf,L,M);
                if (basic_phase < 0)
                {
                    return basic_phase;
                } 
                else if ( p > (psc1-psc0)/(Tsc1-Tsc0) * (T-Tsc0) + psc0 )
                {
                    return stishovite;
                }
                else if ( p > (pbc-pac)/(Tbc-Tac) * (T-Tac) + pac )
                {
                    return coesite;
                }
                else if ( p > (pabc-pab)/(Tabc-Tab) * (T-Tab) + pab )
                {
                    return alpha;
                }
                else if ( p > (pbcb-pbca)/(Tbcb-Tbca) * (T-Tbca) + pbca )
                {
                    return beta;
                }
                else if ( T < Tcacb )
                {
                    return cristobalite_alpha;
                }
                else
                {
                    return cristobalite_beta;
                }
            }
        ),

    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               state_invariant(exp(10.0) * si::poise), // Doremus (2002), at 1400 C
        /*density*/                         state_invariant(2180.0 * si::kilogram/si::meter3), // from Murase and McBirney (1973), for rhyolitic magma
        /*vapor_pressure*/         
                get_interpolated_temperature_function
                    (si::celcius, si::pascal,
                     std::vector<double>{1966.0,     2149.0,     2368.0}, 
                     std::vector<double>{1.0 ,       10.0,       100.0  }),
        /*surface_tension*/                 state_invariant(312.0 * si::dyne / si::centimeter), // 1400C, Shartsis (1951)
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()
    },

    

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid { // alpha
            /*isobaric_specific_heat_capacity*/            // 0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/1.4e6, 10.0,
                     si::calorie/(60.08 * si::gram*si::kelvin), 
                     10.87, 0.008712, 241200.0, 0.0, 
                     273.0, 848.0), // Johnson (1960) and Perry
            /*thermal_conductivity*/              
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{10.0,    20.0,  80.0, 273.15}, 
                     std::vector<double>{1200.0, 480.0,  30.0, 7.69}), // Perry, with Schön (2015) for standard temperature 
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2650.0 *  si::kilogram/si::meter3), // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::kelvin, si::millimeter_mercury,
                     std::vector<double>{1732.0, 1969.0, 2227.0 }, 
                     std::vector<double>{10.0,   100.0,  760.0  }), // Perry, nonspecific solid phase
            /*refractive_index*/      
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 1.5,  8.0,  8.0,  8.6,  8.8,  8.9,  8.9,  8.9,  9.0,  9.0,  9.0,  9.1,  9.1,  9.1,  9.2,  9.2,  9.2,  9.3,  9.3,  9.4,  9.4,  9.5,  9.5,  9.5,  9.6,  9.6,  9.7,  9.7,  9.7,  9.8,  9.8,  9.8,  9.9,  9.9, 14.3},
                     std::vector<double>{1.44, 0.52, 0.51, 0.56, 0.47, 0.47, 0.47, 0.49, 0.53, 0.58, 0.65, 0.74, 0.84, 0.95, 1.07, 1.20, 1.33, 1.47, 1.78, 1.94, 2.22, 2.36, 2.47, 2.58, 2.68, 2.77, 2.83, 2.87, 2.91, 2.93, 2.94, 2.94, 2.92, 2.89, 1.75}),
                    // Kischkat (2012)
            /*extinction_coefficient*/ 
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 1.5,  7.9,  7.9,  8.6,  8.7,  8.7,  8.7,  8.7,  8.8,  8.8,  8.8,  8.9,  8.9,  8.9,  9.0,  9.1,  9.1,  9.1,  9.2,  9.2,  9.2,  9.3,  9.3,  9.3,  9.4,  9.4,  9.4,  9.5,  9.5,  9.5,  9.6,  9.6,  9.8,  9.9,  9.9, 10.0, 10.0, 10.0, 10.1, 10.1, 10.2, 10.2, 10.2, 10.3, 10.3, 10.4, 14.3},
                     std::vector<double>{0.00002, 0.12770, 0.15315, 0.91466, 0.94547, 0.98260, 1.02616, 1.07603, 1.13242, 1.19597, 1.26754, 1.34784, 1.43703, 1.53432, 1.84907, 1.94684, 2.03267, 2.10448, 2.16419, 2.21577, 2.26057, 2.29387, 2.30736, 2.29703, 2.26706, 2.22350, 2.16431, 2.08460, 1.99475, 1.89944, 1.78965, 1.66503, 0.92326, 0.80887, 0.70568, 0.61411, 0.53217, 0.45759, 0.38927, 0.32737, 0.27266, 0.22569, 0.18645, 0.15438, 0.12857, 0.10802, 0.06946}),
                    // Kischkat (2012)
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for quartz
                    (si::micrometer,  37.5 *si::micrometer, 
                     std::vector<double>{ 0.221, 0.239, 0.263, 0.280, 0.348, 0.416, 0.531, 0.784, 1.862, 1.918, 2.152, 2.193, 2.309, 2.317, 2.475, 2.593, 2.662, 2.692, 2.720, 2.784, 2.848, 2.886, 2.915, 2.985, 3.452, 3.667, 3.721, 3.963, 4.084, 4.192, 4.340, 4.475, 4.596, 4.905, 7.395, 7.624, 7.893, 8.216, 8.526, 8.620, 8.714, 9.239, 9.495, 9.724,10.114,10.531,11.958,12.281,12.402,12.483,12.644,12.806,12.994,13.156,13.600,14.313,14.448,14.757,17.476,17.839,17.920,18.176,18.257,18.445,18.903,19.091,19.333,19.831,20.141,20.572,20.935,21.729,22.025,22.483,22.900,23.331,23.586,23.869,24.057,24.380,24.582,24.703 },
                     std::vector<double>{ 0.447, 0.465, 0.546, 0.550, 0.710, 0.761, 0.803, 0.831, 0.865, 0.861, 0.879, 0.867, 0.871, 0.876, 0.874, 0.861, 0.867, 0.847, 0.816, 0.720, 0.710, 0.728, 0.696, 0.729, 0.389, 0.352, 0.291, 0.303, 0.271, 0.186, 0.168, 0.071, 0.175, 0.034, 0.009, 0.031, 0.206, 0.791, 0.678, 0.406, 0.597, 0.905, 0.330, 0.210, 0.132, 0.102, 0.049, 0.022, 0.165, 0.198, 0.152, 0.192, 0.125, 0.096, 0.075, 0.053, 0.087, 0.066, 0.028, 0.039, 0.077, 0.195, 0.207, 0.188, 0.099, 0.097, 0.157, 0.684, 0.791, 0.829, 0.802, 0.584, 0.546, 0.309, 0.213, 0.175, 0.167, 0.131, 0.136, 0.104, 0.063, 0.065 }),

            /*bulk_modulus*/                      state_invariant(37.0 * si::gigapascal), // Schön (2015)
            /*tensile_modulus*/                   state_invariant(0.8680e11*si::pascal), 
            /*shear_modulus*/                     state_invariant(0.5820e11*si::pascal), 
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.08), // Schön (2015)

            /*compressive_fracture_strength*/     state_invariant(1.1 * si::gigapascal), // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
            /*tensile_fracture_strength*/         state_invariant(48.0 * si::megapascal), // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            state_invariant(48.0 * si::megapascal), // https://material-properties.org/strength-of-materials-tensile-yield/
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        },
        phase::PartlyKnownSolid { // beta
            /*isobaric_specific_heat_capacity*/   // 0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/1.4e6, 10.0,
                     si::calorie/(60.08 * si::gram*si::kelvin), 
                     10.95, 0.00550, 0.0, 0.0, 
                     848.0, 1873.0), // Johnson (1960) and Perry
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2533.0 *  si::kilogram/si::meter3), // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::kelvin, si::millimeter_mercury,
                     std::vector<double>{1732.0, 1969.0, 2227.0 }, 
                     std::vector<double>{10.0,   100.0,  760.0  }), // Perry, nonspecific solid phase
            /*refractive_index*/                  missing(),// 1.4585,  // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
            /*extinction_coefficient*/            missing(),
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for quartz
                    (si::micrometer,  37.5 *si::micrometer, 
                    std::vector<double>{ 0.221, 0.239, 0.263, 0.280, 0.348, 0.416, 0.531, 0.784, 1.862, 1.918, 2.152, 2.193, 2.309, 2.317, 2.475, 2.593, 2.662, 2.692, 2.720, 2.784, 2.848, 2.886, 2.915, 2.985, 3.452, 3.667, 3.721, 3.963, 4.084, 4.192, 4.340, 4.475, 4.596, 4.905, 7.395, 7.624, 7.893, 8.216, 8.526, 8.620, 8.714, 9.239, 9.495, 9.724,10.114,10.531,11.958,12.281,12.402,12.483,12.644,12.806,12.994,13.156,13.600,14.313,14.448,14.757,17.476,17.839,17.920,18.176,18.257,18.445,18.903,19.091,19.333,19.831,20.141,20.572,20.935,21.729,22.025,22.483,22.900,23.331,23.586,23.869,24.057,24.380,24.582,24.703 },
                    std::vector<double>{ 0.447, 0.465, 0.546, 0.550, 0.710, 0.761, 0.803, 0.831, 0.865, 0.861, 0.879, 0.867, 0.871, 0.876, 0.874, 0.861, 0.867, 0.847, 0.816, 0.720, 0.710, 0.728, 0.696, 0.729, 0.389, 0.352, 0.291, 0.303, 0.271, 0.186, 0.168, 0.071, 0.175, 0.034, 0.009, 0.031, 0.206, 0.791, 0.678, 0.406, 0.597, 0.905, 0.330, 0.210, 0.132, 0.102, 0.049, 0.022, 0.165, 0.198, 0.152, 0.192, 0.125, 0.096, 0.075, 0.053, 0.087, 0.066, 0.028, 0.039, 0.077, 0.195, 0.207, 0.188, 0.099, 0.097, 0.157, 0.684, 0.791, 0.829, 0.802, 0.584, 0.546, 0.309, 0.213, 0.175, 0.167, 0.131, 0.136, 0.104, 0.063, 0.065 }),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        },
        phase::PartlyKnownSolid { // crystobalite alpha
            /*isobaric_specific_heat_capacity*/   // 0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/1.4e6, 10.0,
                     si::calorie/(60.08 * si::gram*si::kelvin), 
                     3.65, 0.0240, 0.0, 0.0,
                     273.0, 523.0), // Johnson (1960) and Perry
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2334.0 *  si::kilogram/si::meter3), // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::kelvin, si::millimeter_mercury,
                     std::vector<double>{1732.0, 1969.0, 2227.0 }, 
                     std::vector<double>{10.0,   100.0,  760.0  }), // Perry, nonspecific solid phase
            /*refractive_index*/                  missing(),// 1.4585,  // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        },
        phase::PartlyKnownSolid { // crystobalite beta
            /*isobaric_specific_heat_capacity*/   // 0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/1.4e6, 10.0,
                     si::calorie/(60.08 * si::gram*si::kelvin), 
                     17.09, 0.000454, -897200.0, 0.0,
                     523.0, 1973.0), // Johnson (1960) and Perry
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2334.0 *  si::kilogram/si::meter3), // 2650 alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::kelvin, si::millimeter_mercury,
                     std::vector<double>{1732.0, 1969.0, 2227.0 }, 
                     std::vector<double>{10.0,   100.0,  760.0  }), // Perry, nonspecific solid phase
            /*refractive_index*/                  missing(),// 1.4585,  // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// orthoclase, KAlSi3O8, 
// representative of felsic rocks
PartlyKnownCompound  orthoclase (
    /*molar_mass*/                        278.33 * si::gram/si::mole, 
    /*atoms_per_molecule*/                13u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             100.0 * si::calorie/si::gram, // Perry
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,     // Perry
    /*freezing_point_sample_temperature*/ 1170.0*si::celcius, // Perry
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               state_invariant(1e8 * si::poise), // observed by Bowen (1934) for molten Orthoclase, seems large compared to other silicates, but continental crust has been observed to have higher viscosity than oceanic (Itô 1979, "Rheology of the crust...", I can only get a hold of the abstract)
        /*density*/                         state_invariant(2180.0 * si::kilogram/si::meter3), // from Murase and McBirney (1973), for rhyolitic magma
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/                 missing(),
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*isobaric_specific_heat_capacity*/   // 0.61 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(278.33 * si::gram*si::kelvin), 
                     69.26, 0.00821, -2331000.0, 0.0, 
                     273.0, 1373.0),
            /*thermal_conductivity*/              state_invariant(2.31 * si::watt/(si::meter*si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2560.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  1.527,
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for orthoclase
                    (si::micrometer,  37.5 *si::micrometer, 
                    std::vector<double>{ 0.256, 0.281, 0.321, 0.351, 0.354, 0.367, 0.376, 0.390, 0.471, 0.542, 0.893, 1.234, 1.359, 1.626, 2.115, 2.122, 2.195, 2.301, 2.329, 2.600, 2.661, 2.721, 2.785, 2.821, 2.853, 2.883, 2.914, 2.982, 3.094, 3.334, 3.454, 3.560, 3.840, 3.987, 4.227, 4.666, 4.920, 5.013, 5.293, 5.652, 5.959, 6.265, 6.558, 6.945, 7.558, 7.784, 8.104, 8.344, 8.464, 8.704, 8.917, 9.290, 9.463,10.036,10.249,10.769,12.101,12.248,12.541,13.447,15.179,16.232,16.752,18.151,19.350,20.029,20.962,23.014,23.281,24.960 },
                    std::vector<double>{ 0.069, 0.146, 0.482, 0.551, 0.602, 0.616, 0.605, 0.654, 0.716, 0.743, 0.788, 0.767, 0.769, 0.794, 0.809, 0.816, 0.803, 0.803, 0.811, 0.801, 0.807, 0.729, 0.596, 0.598, 0.561, 0.590, 0.569, 0.586, 0.661, 0.711, 0.658, 0.745, 0.757, 0.743, 0.730, 0.510, 0.481, 0.481, 0.295, 0.178, 0.180, 0.140, 0.148, 0.117, 0.023, 0.009, 0.018, 0.082, 0.103, 0.111, 0.096, 0.106, 0.129, 0.117, 0.103, 0.112, 0.144, 0.134, 0.102, 0.121, 0.140, 0.171, 0.201, 0.223, 0.273, 0.282, 0.333, 0.376, 0.374, 0.403 }),

            /*bulk_modulus*/                      state_invariant(46.8 * si::gigapascal), // Schön (2015)
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     state_invariant(27.3 * si::gigapascal), // Schön (2015)
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// andesine, (Ca,Na)(Al,Si)4O8
// representative of plagioclase as a common plagioclase mineral in andesite and diorite,
// representative of intermediate rocks in general
PartlyKnownCompound andesine (
    /*molar_mass*/                        268.6 * si::gram/si::mole, 
    /*atoms_per_molecule*/                13u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             100.0 * si::calorie/si::gram, // Perry
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 1551.0 * si::celcius, // Perry, for anothite
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               state_invariant(1.38e2 * si::poise), // 1.36-1.19 poises, observed by Kani for andesitic basaltic magma at 1400C, referenced by Bowen (1934)
        // .dynamic_viscosity = exp(5.0) * si::poise, // Doremus (2002) for Albite, at 1400 C
        // .dynamic_viscosity = 4e4 * si::poise, // observed by Bowen (1934) for Albite at 1400C
        // .dynamic_viscosity = 3.8e1 * si::poise, // observed by McCaffery for molten anorthite at 1550C, referenced by Bowen (1934)
        // .dynamic_viscosity = 1.38e2 * si::poise, // 1.36-1.19 poises, observed by Kani for andesitic basaltic magma at 1400C, referenced by Bowen (1934)
        // .dynamic_viscosity = 3.0e11 * si::pascal * si::second, // Melosh (2011), from Hiesinger (2007), for andesite lava flow, middle value on log scale
        /*density*/                         state_invariant(2180.0 * si::kilogram/si::meter3), // from Murase and McBirney (1973), for rhyolitic magma
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/        
            get_interpolated_temperature_function
                (si::celcius, si::dyne/si::centimeter,
                 std::vector<double>{ 1300.0, 1600.0 },
                 std::vector<double>{  400.0,  300.0 }), // from Taniguchi (1988), for Anorthite
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*isobaric_specific_heat_capacity*/   // 66.0 * si::joule / (268.6 * si::gram * si::kelvin), // Richet (1984)
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(268.6 * si::gram*si::kelvin), 
                     63.13, 0.01500, -1537000.0, 0.0, 
                     273.0, 1673.0), // for anorthite
            /*thermal_conductivity*/              state_invariant(1.69 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015), for anorthite
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2670.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  1.553,
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for andesine
                    (si::micrometer,  285 *si::micrometer, 
                    std::vector<double>{  0.253,  0.278,  0.348,  0.378,  0.563,  0.615,  1.405,  1.441,  1.460,  1.838,  1.897,  1.908,  2.075,  2.156,  2.242,  2.308,  2.327,  2.626,  2.659,  2.786,  2.821,  2.848,  2.881,  2.916,  2.973,  4.139,  5.614,  7.704,  8.688, 10.409, 10.532, 12.377, 12.991, 15.204, 19.385, 20.000, 23.811, 24.795, 25.901, 27.008, 29.344, 30.327, 37.704, 42.745, 55.409, 64.754, 66.598, 68.196, 82.704, 85.286, 93.401, 99.549,107.172,112.827,133.606,136.680,140.245,148.237,152.418,167.909,172.950,185.368,199.385,207.254,215.983 },
                    std::vector<double>{  0.097,  0.124,  0.243,  0.254,  0.472,  0.493,  0.579,  0.581,  0.595,  0.662,  0.653,  0.628,  0.682,  0.690,  0.672,  0.669,  0.680,  0.614,  0.594,  0.228,  0.226,  0.216,  0.228,  0.214,  0.207,  0.515,  0.043,  0.010,  0.224,  0.153,  0.086,  0.035,  0.057,  0.036,  0.075,  0.119,  0.135,  0.087,  0.094,  0.140,  0.072,  0.099,  0.076,  0.098,  0.115,  0.094,  0.137,  0.104,  0.102,  0.129,  0.119,  0.159,  0.159,  0.145,  0.166,  0.150,  0.153,  0.232,  0.187,  0.230,  0.220,  0.222,  0.266,  0.269,  0.216 }),

            /*bulk_modulus*/                      state_invariant(84.0 * si::gigapascal), // Schön (2015), for anorthite
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     state_invariant(40.0 * si::gigapascal), // Schön (2015), for anorthite
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.29), // Schön (2015), for anorthite

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// augite, (Ca,Na)(Mg,Fe,Al,Ti)(Si,Al)2O6
// representative of pyroxenes as the most common pyroxene mineral
// representative of mafic rocks in general
PartlyKnownCompound augite (
    /*molar_mass*/                        236.4 * si::gram/si::mole, 
    /*atoms_per_molecule*/                10u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 984.0*si::celcius, // for Basalt, http://www.minsocam.org/msa/collectors_corner/arc/tempmagmas.htm
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ state_invariant(1.08 * si::kilojoule / (si::kilogram*si::kelvin)), // Schön (2015), for basaltic lava
        /*thermal_conductivity*/            state_invariant(2.5 * si::watt / (si::meter*si::kelvin)), // Schön (2015), for basaltic lava
        /*dynamic_viscosity*/               missing(),
        /*density*/                         state_invariant(2800.0 * si::kilogram/si::meter3), // from Murase and McBirney (1973), for basaltic  magma
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/        
            get_interpolated_temperature_function
                (si::celcius, si::dyne/si::centimeter,
                 std::vector<double>{ 1228.0, 1438.0 },
                 std::vector<double>{  388.5,  364.4 }), // from Walker (1981), for Basalt
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()
    },



    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            // .dynamic_viscosity = 1e4 * si::pascal * si::second, // Melosh (2011), from Hiesinger (2007), for basaltic lava flow, order of magnitude estimate

            /*isobaric_specific_heat_capacity*/   // 0.7 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988), representative of pyroxenes
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(236.4 * si::gram*si::kelvin), 
                     23.35, 0.008062, -558800.0, 0.0, 
                     273.0, 773.0), // for maganese pyroxenes 
            /*thermal_conductivity*/              state_invariant(4.66 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(3380.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  1.707,
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for augite
                    (si::micrometer,  400 *si::micrometer, 
                    std::vector<double>{  0.221,  0.282,  0.365,  0.508,  0.586,  0.748,  0.797,  0.872,  1.029,  1.225,  2.055,  2.497,  2.625,  2.660,  2.692,  2.750,  2.791,  2.852,  2.916,  2.950,  2.977,  1.391,  2.741,  3.846,  8.510, 10.842, 11.947, 13.543, 14.893, 16.857, 18.085, 20.540, 21.276, 23.854, 25.327, 27.291, 28.764, 29.746, 32.446, 34.042, 35.147, 38.952, 40.671, 41.039, 42.880, 49.140, 54.787, 58.960, 62.397, 66.816, 73.199,114.934,117.757,123.649,126.841,129.418,140.220,143.535,152.127,162.315,172.381,178.273,191.530,199.018,207.119,215.589 },
                    std::vector<double>{  0.026,  0.029,  0.095,  0.168,  0.179,  0.130,  0.127,  0.145,  0.141,  0.150,  0.354,  0.442,  0.468,  0.473,  0.466,  0.337,  0.333,  0.299,  0.325,  0.326,  0.360,  0.238,  0.474,  0.496,  0.037,  0.144,  0.099,  0.093,  0.032,  0.038,  0.181,  0.137,  0.148,  0.077,  0.095,  0.074,  0.070,  0.081,  0.062,  0.093,  0.095,  0.080,  0.095,  0.111,  0.122,  0.095,  0.094,  0.081,  0.089,  0.084,  0.103,  0.114,  0.106,  0.122,  0.118,  0.118,  0.132,  0.141,  0.127,  0.137,  0.131,  0.141,  0.051,  0.048,  0.100,  0.133 }),

            /*bulk_modulus*/                      state_invariant(94.1 * si::gigapascal), // Schön (2015)
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     state_invariant(57.0*si::gigapascal), // Schön (2015)
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.25), // Schön (2015)

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// forsterite, MgSiO 
// representative of Olivine ((Mg,Fe)SiO) as its much more common Mg-rich end member (Smyth 2006)
// representative of ultramafic rocks in general
// also appears most common on the surface of Mercury (Namur 2016)
PartlyKnownCompound forsterite (
    /*molar_mass*/                        153.31 * si::gram/si::mole, 
    /*atoms_per_molecule*/                7u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             130.0 * si::calorie/si::gram, // Perry, for Olivine (85 for fayalite)
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    missing(),
    /*freezing_point_sample_temperature*/ missing(),
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      missing(),
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               state_invariant(1.27e2 * si::poise), // 1.36-1.19 poises, observed by Kani for olivine basaltic magma at 1400C, referenced by Bowen (1934)
        // .dynamic_viscosity = exp(1.5) * si::poise, // Doremus (2002) for Olivine, at 1400 C
        /*density*/                         missing(),
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/        
            get_interpolated_temperature_function
                (si::celcius, si::dyne/si::centimeter,
                 std::vector<double>{ 1246.0, 1450.0 },
                 std::vector<double>{  380.0,  350.0 }), // from Walker (1981), for Limburgite
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*isobaric_specific_heat_capacity*/   // 0.79 * si::joule / (si::gram * si::kelvin), // Cermak (1988), for fayalite/forsterite mix
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(153.31 * si::gram*si::kelvin), 
                     33.57, 0.01907, -879700.0, 0.0, 
                     273.0, 1161.0), // for fayalite
            /*thermal_conductivity*/              state_invariant(7.69 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(3810.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  1.651,
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for olivine
                    (si::micrometer,  162.0 *si::micrometer, 
                    std::vector<double>{  0.221,  0.232,  0.256,  0.303,  0.371,  0.405,  0.411,  0.463,  0.464,  0.557,  0.584,  0.636,  0.666,  0.696,  0.837,  0.853,  1.013,  1.045,  1.085,  1.149,  1.258,  1.335,  1.562,  1.639,  1.780,  1.842,  1.903,  1.956,  2.154,  2.170,  2.353,  2.364,  2.563,  2.627,  2.657,  2.716,  2.758,  2.788,  2.822,  2.886,  2.915,  2.945,  2.977,  3.524,  4.754,  5.614,  6.721,  8.688, 10.655, 12.254, 13.114, 14.959, 15.942, 17.172, 18.893, 20.737, 23.811, 24.795, 26.762, 27.991, 32.049, 34.139, 35.000, 35.737, 40.655, 57.008, 68.565, 72.131, 80.122, 82.459, 85.040, 87.991, 89.344, 92.172,105.942,129.549,136.188,152.172,157.213,168.155,191.639,199.508,207.131,215.614 },
                    std::vector<double>{  0.039,  0.056,  0.057,  0.088,  0.321,  0.362,  0.382,  0.404,  0.418,  0.476,  0.472,  0.448,  0.455,  0.445,  0.299,  0.296,  0.165,  0.155,  0.159,  0.191,  0.218,  0.254,  0.428,  0.448,  0.457,  0.454,  0.455,  0.447,  0.465,  0.460,  0.444,  0.454,  0.439,  0.446,  0.437,  0.164,  0.213,  0.237,  0.249,  0.303,  0.299,  0.316,  0.319,  0.419,  0.267,  0.046,  0.143,  0.014,  0.234,  0.057,  0.032,  0.014,  0.062,  0.030,  0.169,  0.051,  0.180,  0.146,  0.092,  0.129,  0.058,  0.137,  0.135,  0.118,  0.085,  0.070,  0.081,  0.078,  0.105,  0.103,  0.128,  0.128,  0.114,  0.134,  0.142,  0.199,  0.187,  0.239,  0.242,  0.232,  0.260,  0.303,  0.359,  0.380 }),

            /*bulk_modulus*/                      state_invariant(129.6 * si::gigapascal), // Schön (2015)
            /*tensile_modulus*/                   state_invariant(3.2848e11*si::pascal),
            /*shear_modulus*/                     state_invariant(0.6515e11*si::pascal),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.24),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// Goethite, FeO(OH)
// for surface of mars, representative of iron oxides and red soils in general, and siderophile ores
PartlyKnownCompound  goethite (
    /*molar_mass*/                        88.85 * si::gram/si::mole, 
    /*atoms_per_molecule*/                4u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    missing(),
    /*freezing_point_sample_temperature*/ missing(),
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/                 missing(),
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              state_invariant(2.91 * si::watt / (si::meter * si::kelvin)), // Cermak (1988)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(4300.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  2.401,
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for goethite
                    (si::micrometer,  125.0 *si::micrometer, 
                    std::vector<double>{   0.22471,   0.43278,   0.44558,   0.48880,   0.52561,   0.58003,   0.60403,   0.65205,   0.68246,   0.74808,   0.77529,   0.85531,   0.90333,   0.94814,   1.03777,   1.07778,   1.21223,   1.27465,   1.57234,   1.66677,   1.88284,   1.93406,   2.00288,   2.12132,   2.17734,   2.32778,   2.39981,   2.44782,   2.59187,   2.66229,   2.72311,   2.78233,   2.88316,   2.91357,   2.95198,   2.98079,   3.27869,   4.50820,   6.10656,   6.84426,   7.95082,   8.81148,  10.40984,  12.37705,  13.97541,  14.83607,  17.04918,  21.47541,  27.50000,  32.17213,  35.24590,  38.19672,  40.65574,  46.43443,  49.87705,  56.27049,  77.04918,  82.58197,  90.81967,  94.38525, 130.04098, 133.11475, 140.36885, 148.11475, 156.96721, 167.78689, 178.85246, 192.13115, 207.86885, 215.61475 },
                    std::vector<double>{  0.0113,  0.0225,  0.0317,  0.0352,  0.0725,  0.1824,  0.2007,  0.2049,  0.2176,  0.2782,  0.2824,  0.2275,  0.2190,  0.2218,  0.2627,  0.2979,  0.4803,  0.5282,  0.5549,  0.5331,  0.5373,  0.5254,  0.5282,  0.5662,  0.5599,  0.4979,  0.4380,  0.4296,  0.4359,  0.3958,  0.3007,  0.1570,  0.0732,  0.0761,  0.0563,  0.0627,  0.0348,  0.2310,  0.1034,  0.2304,  0.1926,  0.0458,  0.0163,  0.0293,  0.0218,  0.0122,  0.0190,  0.0753,  0.0753,  0.0348,  0.0828,  0.0870,  0.0794,  0.0918,  0.0815,  0.1151,  0.1336,  0.1295,  0.1604,  0.1501,  0.1988,  0.1933,  0.2214,  0.2125,  0.2516,  0.2654,  0.2516,  0.2928,  0.3031,  0.2935 }),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// pyrite, FeS2
// for surfaces of the heaviest planets, maybe venus and mercury, and representative of siderophile ores
PartlyKnownCompound  pyrite (
    /*molar_mass*/                        119.98 * si::gram/si::mole, 
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 1180.0 * si::celcius, // new world encyclopedia
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/                 missing(),
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*isobaric_specific_heat_capacity*/   // 0.5 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988, room temperature
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(119.98 * si::gram*si::kelvin), 
                     10.7, 0.01336, 0.0, 0.0, 
                     273.0, 773.0),
            /*thermal_conductivity*/              state_invariant(19.21 * si::watt / (si::meter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(5020.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  1.78, //gemologyproject.com
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for pyrite
                    (si::micrometer,  162.0 *si::micrometer, 
                    std::vector<double>{  0.23550,  0.24665,  0.31676,  0.34226,  0.37731,  0.46176,  0.50000,  0.58764,  0.62588,  0.69280,  0.73263,  0.84098,  0.90153,  1.02900,  1.17559,  1.36998,  1.60739,  1.66953,  1.70459,  2.05672,  2.32760,  2.49490,  2.66061,  2.68929,  2.72275,  2.75621,  2.78649,  2.88050,  2.91396,  2.94583,  3.03656,  3.56466,  3.87610,  4.03859,  4.93230,  5.14895,  5.41977,  6.04265,  6.44888,  6.69262,  6.93636,  7.07177,  7.65403,  7.83006,  8.10088,  8.16858,  8.43940,  8.58835,  8.73731,  8.89980,  9.02167,  9.36019,  9.59039,  9.71225,  9.84766,  9.92891, 10.09140, 11.35071, 12.20379, 12.65064, 12.81313, 14.08599, 14.65471, 14.83074, 14.99323, 15.20988, 15.39946, 15.88693, 16.15775, 16.87542, 17.03791, 17.29519, 17.95870, 18.13473, 19.24509, 21.39810, 22.27827, 22.72512, 22.86053, 23.11781, 24.22817, 24.81043, 24.93230 },
                    std::vector<double>{ 0.00702, 0.02027, 0.03712, 0.03753, 0.05559, 0.07873, 0.09264, 0.10428, 0.10428, 0.10936, 0.10896, 0.09612, 0.09425, 0.08421, 0.08047, 0.07900, 0.08060, 0.07953, 0.08033, 0.07405, 0.07766, 0.08087, 0.08528, 0.08475, 0.08876, 0.07833, 0.07659, 0.04890, 0.04582, 0.03418, 0.02283, 0.05607, 0.06561, 0.07341, 0.07890, 0.08584, 0.08237, 0.05549, 0.08064, 0.06676, 0.08121, 0.08410, 0.08988, 0.09509, 0.10896, 0.10405, 0.09913, 0.08844, 0.08844, 0.07717, 0.07601, 0.07890, 0.09827, 0.07399, 0.07225, 0.06301, 0.06012, 0.06532, 0.05289, 0.04971, 0.05202, 0.05665, 0.06474, 0.06965, 0.06734, 0.05780, 0.05694, 0.06590, 0.06156, 0.07572, 0.07457, 0.06879, 0.06647, 0.06850, 0.06734, 0.05723, 0.04364, 0.02601, 0.03064, 0.08497, 0.20318, 0.24451, 0.23988 }),

            /*bulk_modulus*/                      state_invariant(143.0 * si::gigapascal), // Schön (2015)
            /*tensile_modulus*/                   state_invariant(3.818e11*si::pascal),
            /*shear_modulus*/                     state_invariant(1.094e11*si::pascal),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.16), // Schön (2015)

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// hematite, Fe2O3 
// representative of iron oxides and red soils in general, surfaces of early earth, and siderophile ores
// TODO: pick one, goethite or hematite, we can't afford them both
PartlyKnownCompound hematite (
    /*molar_mass*/                        159.69 * si::gram/si::mole, 
    /*atoms_per_molecule*/                5u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 1560.0 * si::celcius, // Perry
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/                 missing(),
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*isobaric_specific_heat_capacity*/   // 0.61 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(159.69 * si::gram*si::kelvin), 
                     24.72, 0.01604, -423400.0, 0.0, 
                     273.0, 1097.0),
            /*thermal_conductivity*/              state_invariant(11.28 * si::watt / (si::meter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(5250.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  0.22,  0.23,  0.24,  0.27,  0.28,  0.29,  0.32,  0.33,  0.34,  0.35,  0.36,  0.37,  0.38,  0.43,  0.44,  0.45,  0.46,  0.49,  0.50,  0.52,  0.53,  0.54,  0.56,  0.57,  0.58, 14.29, 14.49, 14.71, 15.15, 15.38, 15.62, 16.13, 16.39, 16.67, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 45.45, 47.62, 50.00, 52.63, 55.56, 58.82, 62.50, 66.67, 71.43, 90.91},
                     std::vector<double>{ 1.20,  1.47,  1.66,  1.79,  2.11,  2.20,  2.26,  2.42,  2.45,  2.45,  2.43,  2.43,  2.47,  2.54,  3.05,  3.13,  3.18,  3.22,  3.28,  3.28,  3.26,  3.27,  3.29,  3.35,  3.36,  3.35,  1.15,  1.04,  0.91,  0.60,  0.48,  0.40,  0.33,  0.31,  0.30,  0.34,  0.40,  0.52,  0.79,  1.26,  2.01,  2.82,  2.67,  1.76,  1.10,  0.90,  1.01,  1.27,  2.23,  4.99,  7.43,  6.23,  4.97,  4.10,  3.26,  2.38,  1.55,  1.03,  0.81,  0.85,  1.00,  1.40,  2.68,  6.49, 12.59, 11.93,  9.43,  8.31,  7.29,  6.65,  7.02,  6.79,  6.36,  6.09,  5.84,  5.66,  5.51,  5.39,  5.29,  5.20,  5.00}),
                    // Querry (1985)
            /*extinction_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  0.22,  0.23,  0.26,  0.27,  0.29,  0.30,  0.31,  0.32,  0.34,  0.35,  0.36,  0.38,  0.39,  0.40,  0.41,  0.42,  0.43,  0.50,  0.51,  0.52,  0.54,  0.55,  0.56,  0.59,  0.60, 14.49, 14.71, 14.93, 15.15, 15.38, 16.13, 16.39, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 45.45, 47.62, 50.00, 52.63, 90.91},
                     std::vector<double>{ 1.207,  1.291,  1.309,  1.317,  1.300,  1.233,  1.208,  1.195,  1.169,  1.085,  1.093,  1.140,  1.258,  1.291,  1.294,  1.271,  1.231,  1.173,  0.675,  0.622,  0.587,  0.538,  0.498,  0.437,  0.202,  0.149,  0.297,  0.350,  0.440,  0.586,  0.779,  1.406,  1.624,  2.096,  2.358,  2.669,  3.035,  3.436,  3.791,  3.911,  3.368,  2.292,  2.054,  2.553,  3.328,  4.068,  4.993,  6.303,  6.991,  4.077,  1.587,  0.982,  0.718,  0.656,  0.803,  1.390,  2.254,  3.245,  4.284,  5.382,  6.922,  9.062, 11.145,  8.306,  2.150,  0.782,  0.389,  0.132,  0.601,  0.668,  0.030, -0.074, -0.117, -0.148, -0.076}),
                    // Querry (1985)
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for hematite
                    (si::micrometer,  233.0*si::micrometer, 
                    std::vector<double>{  0.231,  0.243,  0.281,  0.310,  0.319,  0.334,  0.358,  0.412,  0.440,  0.468,  0.519,  0.552,  0.574,  0.614,  0.673,  0.723,  0.771,  0.846,  0.852,  0.870,  0.884,  0.897,  0.925,  1.179,  1.246,  1.347,  1.591,  1.712,  1.755,  1.951,  2.441,  2.468,  2.597,  2.656,  2.685,  2.720,  2.754,  2.784,  2.818,  2.848,  2.883,  2.913,  2.948,  2.980,  4.139,  8.442, 14.221, 15.450, 18.155, 20.122, 21.475, 23.073, 24.303, 25.532, 26.885, 28.729, 33.032, 35.245, 36.844, 42.131, 42.868, 50.122, 67.581,101.639,108.032,112.827,117.868,132.868,136.188,140.122,143.442,148.360,152.172,156.967,167.418,173.073,185.122,191.762,199.262 },
                    std::vector<double>{ 0.1100, 0.0969, 0.0950, 0.0843, 0.0856, 0.0766, 0.0741, 0.0884, 0.0878, 0.0820, 0.0781, 0.0802, 0.0792, 0.0850, 0.0865, 0.0926, 0.0918, 0.0850, 0.0893, 0.0875, 0.0902, 0.0858, 0.0857, 0.1266, 0.1279, 0.1235, 0.1217, 0.1254, 0.1250, 0.1274, 0.1450, 0.1476, 0.1503, 0.1485, 0.1380, 0.1342, 0.0908, 0.1087, 0.0900, 0.0828, 0.0979, 0.0836, 0.0890, 0.1104, 0.1633, 0.0549, 0.0067, 0.1623, 0.2621, 0.0837, 0.2109, 0.1704, 0.0939, 0.1131, 0.0625, 0.2890, 0.4106, 0.2601, 0.2180, 0.1689, 0.1881, 0.1562, 0.1390, 0.1253, 0.1476, 0.1192, 0.1562, 0.1476, 0.1668, 0.1430, 0.1456, 0.1897, 0.1947, 0.1461, 0.1435, 0.1861, 0.3650, 0.2317, 0.2023 }),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   state_invariant(2.4243e11*si::pascal), 
            /*shear_modulus*/                     state_invariant(0.8569e11*si::pascal), 
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);

// native gold, Au
// for precious metals
PartlyKnownCompound  gold (
    /*molar_mass*/                        196.967 * si::gram/si::mole, 
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                2.0 * 174.0 * si::picometer, // www.webelements.com, from calculated radius
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           510e3 * si::kilopascal,
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        7250.0 * si::kelvin, // wow!
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       324.0 * si::kilojoule/(196.967*si::gram),
    /*latent_heat_of_fusion*/             12.55*si::kilojoule/(196.967*si::gram),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 1064.180*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  3243.0 * si::kelvin, // wikipedia

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ state_invariant(7.0 * si::calorie/(196.967 * si::gram*si::kelvin)), // Perry, 1336-1573K
        /*thermal_conductivity*/            state_invariant(105.0 * si::watt / (si::meter * si::kelvin)), // Mills (1996)
        /*dynamic_viscosity*/      
                get_interpolated_inverse_temperature_function
                    (si::kelvin, si::centipoise,
                     std::vector<double>{0.6, 0.725}, 
                     std::vector<double>{4.0, 5.3  }), // Chapman (1966)
        /*density*/                17310.0 * si::kilogram/si::meter3, 
        /*vapor_pressure*/         
                get_interpolated_temperature_function
                    (si::celcius, si::pascal,
                     std::vector<double>{1373.0,     2008.0,     2805.0}, 
                     std::vector<double>{1.0 ,       1e3,        100e3}),
        /*surface_tension*/        
                get_linear_liquid_surface_tension_temperature_function
                    (si::kelvin, si::newton/si::meter, 
                     1338.0, 1.162, -1.8e-4), // Egry(2010)
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()
    },



    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   // 0.129 * si::joule / (si::gram * si::kelvin), // wikipedia, room temperature
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     3.75e-6, 9.86/(165.0*165.0*165.0), 15.0,
                     si::calorie/(196.967 * si::gram*si::kelvin), 
                     5.61, 0.00144, 0.0, 0.0, 
                     273.0, 1336.0), // Johnson (1960) and Perry
            /*thermal_conductivity*/              // 314.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{   4.0, 10.0,    60.0,  300.0, 1200.0}, 
                     std::vector<double>{1600.0, 2800.0, 380.0,  315.0,  262.0}), // Johnson (1960) and Perry
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(19300.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{1869.0, 2154.0, 2521.0, 2966.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.003542,0.005636,0.008266,0.008856,0.009537,0.01127, 0.01378, 0.01459, 0.01631, 0.01722, 0.01771, 0.01879, 0.02,    0.02138, 0.02214, 0.02296, 0.02339, 0.02384, 0.0248,  0.0253,  0.02818, 0.03263, 0.03542, 0.03875, 0.03999, 0.04133, 0.04592, 0.04679, 0.04862, 0.05061, 0.05391, 0.0551,  0.05636, 0.05767, 0.05904, 0.06199, 0.06525, 0.07293, 0.07749, 0.08266, 0.08551, 0.08856, 0.1127,  0.124,   0.155,   0.1653,  0.2066,  0.261,   0.2755,  0.2952,  0.3024,  0.31,    0.3179,  0.3263, 0.3351, 0.3444, 0.3542, 0.3815, 0.4133, 0.4959, 0.6199, 0.8266, 1.24,  4.133, 6.199, 12.4,  24.8, 49.59, 124.0, 248.0},
                     std::vector<double>{1.001,   0.987,   0.963,   0.955,   0.943,   0.902,   0.849,   0.846,   0.853,   0.851,   0.859,   0.883,   0.881,   0.868,   0.871,   0.885,   0.89,    0.889,   0.884,   0.885,   0.899,   0.892,   0.865,   0.921,   0.932,   0.93,    0.886,   0.889,   0.913,   0.989,   1.175,   1.262,   1.338,   1.368,   1.346,   1.29,    1.266,   1.33,    1.421,   1.66,    1.644,   1.72,    1.545,   1.462,   1.16,    1.142,   1.339,   1.612,   1.694,   1.752,   1.729,   1.615,   1.295,   0.727,  0.3,    0.23,   0.211,  0.229,  0.226,  0.237,  0.269,  0.271,  0.278, 1.411, 2.842, 9.936, 36.7, 118.0, 309.0, 531.0}),
                // Hagemann (1974)
            /*extinction_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{8.266E-06,1.494E-05,1.55E-05, 2.48E-05, 4.133E-05,6.199E-05,8.266E-05,8.856E-05,9.537E-05,0.000124, 0.000248, 0.0004275,0.0004959,0.0006199,0.00124,  0.00155,  0.002066, 0.00248,  0.003542, 0.004133, 0.005166, 0.005636, 0.007749, 0.008856, 0.009184, 0.01033,  0.01127,  0.01181,  0.01393,  0.01409,  0.01459,  0.01467,  0.01476,  0.01485,  0.01494,  0.01512,  0.01722,  0.02,     0.02214,  0.02384,  0.02638,  0.03024,  0.03444,  0.03542,  0.03757,  0.03999,  0.04065,  0.04133,  0.04275,  0.04428,  0.04592,  0.04959,  0.05166,  0.0551,   0.05636,  0.05904,  0.06199,  0.06888,  0.07749,  0.08266,  0.09537,  0.1078,   0.1127,   0.1378,   0.155,    0.1653,   0.1771,   0.2066,   0.2254,   0.248,    0.2818,   0.2952,   0.3263,   0.3444,   0.3757,   0.3936,   0.4428,   0.4769,   0.5166,   0.6199,   0.8266,   1.24,     2.48,     12.4,     24.8,     124.0,    248.0},
                     std::vector<double>{2.56E-09, 1.95E-08, 5.22E-09, 3.05E-08, 1.85E-07, 8.83E-07, 2.28E-06, 1.94E-06, 2.56E-06, 2.53E-06, 2.87E-05, 0.000164, 0.000211, 0.00012,  0.00096,  0.00163,  0.0034,   0.00471,  0.00731,  0.00895,  0.0113,   0.0118,   0.0101,   0.0109,   0.0118,   0.0178,   0.0283,   0.0353,   0.0807,   0.0838,   0.0956,   0.0962,   0.0938,   0.0943,   0.0963,   0.101,    0.14,     0.211,    0.23,     0.255,    0.315,    0.415,    0.54,     0.573,    0.672,    0.788,    0.799,    0.804,    0.802,    0.814,    0.855,    1,        1.12,     1.28,     1.29,     1.21,     1.07,     0.865,    0.898,    0.934,    0.899,    0.92,     0.945,    1.14,     1.22,     1.15,     1.13,     1.3,      1.45,     1.65,     1.87,     1.89,     1.86,     1.81,     1.88,     1.89,     1.81,     1.71,     2.01,     3.31,     5.13,     8.36,     18.1,     80.7,     124.0,    256.0,    379.0}),
                // Hagemann (1974)
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   state_invariant((76.0+81.0)/2.0*si::gigapascal), // https://www.azom.com/properties.aspx?ArticleID=598 // state_invariant(1.9244e11*si::pascal), // values from Perry produce negative bulk modulus so they are commented out
            /*shear_modulus*/                     state_invariant((26.0+30.0)/2.0*si::gigapascal), // https://www.azom.com/properties.aspx?ArticleID=598 state_invariant(0.4200e11*si::pascal),    // values from Perry produce negative bulk modulus so they are commented out
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.43), // wikipedia

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            state_invariant(300.0 * si::megapascal), // https://material-properties.org/strength-of-materials-tensile-yield/
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// native silver, Ag
// for precious metals
PartlyKnownCompound  silver (
    /*molar_mass*/                        107.868 * si::gram/si::mole, 
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                2.0*165.0 * si::picometer, // www.webelements.com, from calculated radius
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        6410.0 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       60720.0 *  si::calorie/(107.868 * si::gram), // Perry
    /*latent_heat_of_fusion*/             11.3*si::kilojoule/(107.868*si::gram),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 961.78*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  2435.0 * si::kelvin, // wikipedia

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      missing(),
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ state_invariant(8.2 * si::calorie/(107.868 * si::gram*si::kelvin)), // Perry, 1234-1573K
        /*thermal_conductivity*/   state_invariant(180.0 * si::watt / (si::meter * si::kelvin)), // Mills (1996)
        /*dynamic_viscosity*/      
                get_interpolated_inverse_temperature_function
                    (si::kelvin, si::centipoise,
                     std::vector<double>{0.6, 0.8}, 
                     std::vector<double>{2.4, 4.0}), // Chapman (1966)
        /*density*/                9320.0 * si::kilogram/si::meter3, 
        /*vapor_pressure*/         
                get_interpolated_temperature_function
                    (si::celcius, si::pascal,
                     std::vector<double>{1010.0,     1509.0,     2160.0}, 
                     std::vector<double>{1.0 ,       1e3,        100e3}),
        /*surface_tension*/        
                get_linear_liquid_surface_tension_temperature_function
                    (si::kelvin, si::newton/si::meter, 
                     1234.0, 0.914, -1.5e-4), // Egry(2010)
        /*refractive_index*/       missing(),
        /*extinction_coefficient*/ missing()
    },



    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   // 0.233 * si::joule / (si::gram * si::kelvin), // wikipedia
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/600e3, 30.0,
                     si::calorie/(107.868 * si::gram*si::kelvin), 
                     5.6, 0.00150, 0.0, 0.0, 
                     273.0, 1336.0), // Johnson (1960) and Perry
            /*thermal_conductivity*/              // 427.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{10.0,     60.0,  300.0, 1200.0}, 
                     std::vector<double>{16500.0, 630.0,  424.0,  358.0}), // Perry
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(10500.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{1357.0, 1575.0, 1865.0, 2212.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.003542,0.005636,0.008266,0.008856,0.009537,0.01127, 0.01378, 0.01459, 0.01631, 0.01722, 0.01771, 0.01879, 0.02,    0.02138, 0.02214, 0.02296, 0.02339, 0.02384, 0.0248,  0.0253,  0.02818, 0.03263, 0.03542, 0.03875, 0.03999, 0.04133, 0.04592, 0.04679, 0.04862, 0.05061, 0.05391, 0.0551,  0.05636, 0.05767, 0.05904, 0.06199, 0.06525, 0.07293, 0.07749, 0.08266, 0.08551, 0.08856, 0.1127,  0.124,   0.155,   0.1653,  0.2066,  0.261,   0.2755,  0.2952,  0.3024,  0.31,    0.3179,  0.3263, 0.3351, 0.3444, 0.3542, 0.3815, 0.4133, 0.4959, 0.6199, 0.8266, 1.24,  4.133, 6.199, 12.4,  24.8, 49.59, 124.0, 248.0},
                     std::vector<double>{1.001,   0.987,   0.963,   0.955,   0.943,   0.902,   0.849,   0.846,   0.853,   0.851,   0.859,   0.883,   0.881,   0.868,   0.871,   0.885,   0.89,    0.889,   0.884,   0.885,   0.899,   0.892,   0.865,   0.921,   0.932,   0.93,    0.886,   0.889,   0.913,   0.989,   1.175,   1.262,   1.338,   1.368,   1.346,   1.29,    1.266,   1.33,    1.421,   1.66,    1.644,   1.72,    1.545,   1.462,   1.16,    1.142,   1.339,   1.612,   1.694,   1.752,   1.729,   1.615,   1.295,   0.727,  0.3,    0.23,   0.211,  0.229,  0.226,  0.237,  0.269,  0.271,  0.278, 1.411, 2.842, 9.936, 36.7, 118.0, 309.0, 531.0}),
                // Hagemann (1974)
            /*extinction_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{2.48e-06, 6.199e-06,1.24e-05, 1.653e-05,2.48e-05, 3.1e-05, 4.133e-05,4.769e-05,4.959e-05,6.199e-05,0.000124, 0.000155, 0.0002066,0.000248, 0.0006199,0.0008266,0.00155,0.001675, 0.002066, 0.00248,  0.0031, 0.003351, 0.003542,0.005166, 0.005636, 0.006199, 0.006888, 0.007749, 0.008266, 0.01033,0.01127, 0.01305, 0.0155, 0.01675, 0.01823, 0.01879,  0.02, 0.02296, 0.02384, 0.02638, 0.02695,  0.02952,  0.031, 0.03351,  0.03444,  0.03647,  0.03757, 0.04428,  0.04509, 0.05166,0.05276,  0.05391,  0.0551, 0.05904, 0.06199,0.07749,  0.08266, 0.08551, 0.08856,0.09537,  0.1127,   0.1378,   0.1653,  0.1907,   0.2254,   0.261,   0.2755,   0.2883,   0.2952,   0.31,   0.3179,   0.3289, 0.4133,  0.4959,  0.6199, 0.8266,   1.24,    2.48,     3.1,   4.133,   6.199,    12.4,     24.8,    49.59,    124.0,    248.0    },
                     std::vector<double>{3.98e-12, 1.15e-10, 1.49e-09, 4.33e-09, 1.92e-08, 4.64e-08,1.32e-07, 2.11e-07,  3.63e-08, 9.57e-08,1.29e-06, 2.92e-06,  8.36e-06,1.52e-05, 4.72e-05,  0.000121,0.00108, 0.00142,  0.00269, 0.00457, 0.00882,  0.00823,  0.00168,  0.0035,  0.00345,  0.00377,  0.00431,  0.00438,  0.00407, 0.00622, 0.0172,  0.0565,  0.139,   0.166,   0.208,   0.211, 0.211,   0.274,   0.276,   0.317,   0.323,    0.349,  0.368,     0.4,    0.418,    0.489,    0.514,    0.59,    0.616,   0.904,   0.93,    0.943,   0.935,   0.745,   0.714,   0.91,    0.918,   0.875,   0.783,   0.64,   0.553,    0.562,    0.911,    1.18,     1.34,    1.34,     1.28,     1.13,     1.06,    0.599,   0.36,    0.399,   2.28,    3.09,    4.18,    5.8,   9.02,    18.8,    22.9,    30.5,    45.7,    90.2,    173.0,    306.0,    506.0,    689.0    }),
                // Hagemann (1974)
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   state_invariant(1.2399e11*si::pascal),
            /*shear_modulus*/                     state_invariant(0.4612e11*si::pascal),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// native copper, Cu
// for precious metals
PartlyKnownCompound  copper (
    /*molar_mass*/                        63.546 * si::gram/si::mole, 
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                2.0 * 145.0 * si::picometer, // www.webelements.com, from calculated radius
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       204.8 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             13.26*si::kilojoule/(63.546*si::gram),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 1084.62*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  2835 * si::kelvin, // wikipedia

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      missing(),
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ 
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::calorie/(63.546 * si::gram*si::kelvin), 
                 5.44, 0.001462, 0.0, 0.0, 0.0,
                 1357.0, 1573.0),
        /*thermal_conductivity*/   state_invariant(160.0 * si::watt / (si::meter * si::kelvin)), // Mills (1996)
        /*dynamic_viscosity*/      
                get_interpolated_inverse_temperature_function
                    (si::kelvin, si::centipoise,
                     std::vector<double>{0.6, 0.725}, 
                     std::vector<double>{2.4, 5.0  }), // Chapman (1966)
        /*density*/                8020.0 * si::kilogram/si::meter3, 
        /*vapor_pressure*/         
                get_interpolated_temperature_function
                    (si::celcius, si::pascal,
                     std::vector<double>{1236.0,     1816.0,     2563.0}, 
                     std::vector<double>{1.0 ,       1e3,        100e3}),
        /*surface_tension*/        
                get_linear_liquid_surface_tension_temperature_function
                    (si::kelvin, si::newton/si::meter, 
                     1357.0, 1.339, -1.8e-4), // Egry(2010)
        /*refractive_index*/       missing(),
        /*extinction_coefficient*/ missing()
    },



    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/            // 0.385 * si::joule / (si::gram * si::kelvin), // wikipedia
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     10.8e-6, 30.6/(344.5*344.5*344.5), 10.0,
                     si::calorie/(63.546 * si::gram*si::kelvin), 
                     5.44, 0.001462, 0.0, 0.0, 
                     273.0, 1357.0), // Johnson (1960) and Perry
            /*thermal_conductivity*/              // 401.0 * si::watt / (si::meter * si::kelvin), // wikipedia
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{   4.0, 10.0,     60.0,  300.0, 1200.0}, 
                     std::vector<double>{7000.0, 19000.0, 850.0,  398.0,  342.0}), // Johnson (1960)  and Perry
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           8960.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{1628.0, 1879.0, 2207.0, 2595.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{2.5e-05, 2.5e-03, 4.1e-03, 8.3e-03, 1.0e-02, 3.9e-02, 4.0e-02, 4.1e-02, 4.3e-02, 4.4e-02, 4.6e-02, 5.0e-02, 5.2e-02, 5.4e-02, 5.6e-02, 5.9e-02, 6.2e-02, 6.5e-02, 6.9e-02, 7.3e-02, 7.7e-02, 8.6e-02, 8.9e-02, 9.5e-02, 1.0e-01, 1.1e-01, 1.2e-01, 1.3e-01, 1.4e-01, 1.5e-01, 1.7e-01, 1.8e-01, 1.9e-01, 2.1e-01, 2.1e-01, 2.2e-01, 2.3e-01, 2.4e-01, 2.5e-01, 2.6e-01, 2.7e-01, 2.8e-01, 3.0e-01, 3.1e-01, 3.3e-01, 3.4e-01, 3.9e-01, 4.1e-01, 4.8e-01, 5.2e-01, 5.4e-01, 5.6e-01, 5.9e-01, 6.2e-01, 6.5e-01, 7.1e-01, 7.3e-01, 8.3e-01, 1.2e+00, 2.5e+00, 1.2e+01, 2.5e+01, 1.2e+02, 2.5e+02},
                     std::vector<double>{  1.00,   1.00,   0.99,   0.97,   0.96,   0.89,   0.88,   0.86,   0.85,   0.86,   0.88,   0.96,   0.96,   0.94,   0.92,   0.90,   0.88,   0.88,   0.89,   0.91,   0.95,   1.03,   1.05,   1.08,   1.09,   1.07,   1.04,   1.03,   1.03,   1.03,   1.00,   0.97,   0.96,   1.04,   1.10,   1.18,   1.28,   1.38,   1.47,   1.53,   1.52,   1.49,   1.42,   1.34,   1.34,   1.31,   1.23,   1.18,   1.15,   1.12,   1.04,   0.83,   0.47,   0.27,   0.21,   0.21,   0.22,   0.26,   0.43,   1.69,  29.10,  62.80, 186.00, 270.00}),
                // Hagemann (1974)
            /*extinction_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{2.5e-05, 4.1e-03, 6.2e-03, 8.3e-03, 1.0e-02, 3.9e-02, 4.0e-02, 4.1e-02, 4.3e-02, 4.4e-02, 4.6e-02, 4.8e-02, 5.0e-02, 5.2e-02, 5.4e-02, 5.6e-02, 5.9e-02, 6.2e-02, 6.9e-02, 7.3e-02, 7.7e-02, 8.0e-02, 8.9e-02, 9.5e-02, 1.0e-01, 1.1e-01, 1.2e-01, 1.3e-01, 1.5e-01, 1.5e-01, 1.7e-01, 1.8e-01, 1.9e-01, 2.1e-01, 2.1e-01, 2.2e-01, 2.3e-01, 2.4e-01, 2.5e-01, 2.6e-01, 2.7e-01, 2.8e-01, 3.0e-01, 3.1e-01, 3.3e-01, 3.4e-01, 3.6e-01, 3.9e-01, 4.1e-01, 4.4e-01, 4.8e-01, 5.2e-01, 5.4e-01, 5.6e-01, 5.9e-01, 6.5e-01, 7.3e-01, 8.3e-01, 1.2e+00, 2.5e+00, 1.2e+01, 2.5e+01, 1.2e+02, 2.5e+02},
                     std::vector<double>{4.5e-09, 5.1e-03, 1.6e-02, 3.1e-02, 5.4e-02, 2.2e-01, 2.4e-01, 2.6e-01, 3.0e-01, 3.5e-01, 3.8e-01, 4.0e-01, 4.0e-01, 3.7e-01, 3.7e-01, 3.8e-01, 4.1e-01, 4.6e-01, 5.6e-01, 6.2e-01, 6.7e-01, 6.9e-01, 7.2e-01, 7.2e-01, 7.1e-01, 7.5e-01, 8.2e-01, 8.7e-01, 9.8e-01, 1.0e+00, 1.1e+00, 1.2e+00, 1.4e+00, 1.6e+00, 1.7e+00, 1.7e+00, 1.8e+00, 1.8e+00, 1.8e+00, 1.7e+00, 1.7e+00, 1.6e+00, 1.6e+00, 1.7e+00, 1.8e+00, 1.9e+00, 1.9e+00, 2.1e+00, 2.2e+00, 2.4e+00, 2.5e+00, 2.6e+00, 2.6e+00, 2.6e+00, 2.8e+00, 3.7e+00, 4.4e+00, 5.3e+00, 8.5e+00, 1.8e+01, 7.1e+01, 1.0e+02, 2.1e+02, 2.9e+02}),
                // Hagemann (1974)
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   state_invariant(1.683e11*si::pascal),
            /*shear_modulus*/                     state_invariant(0.757e11*si::pascal),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.33),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         state_invariant(220.0 * si::megapascal), // engineering toolbox
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            state_invariant(70.0 * si::megapascal), // engineering toolbox
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// magnetite, Fe3O4
// representative of siderophile ores, and for surfaces of mars and maybe venus
PartlyKnownCompound  magnetite (
    /*molar_mass*/                        231.53 * si::gram/si::mole,
    /*atoms_per_molecule*/                7u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 1538.0 * si::celcius, // Perry
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/                 missing(),
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()
    },



    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*isobaric_specific_heat_capacity*/   // 0.6 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(231.53 * si::gram*si::kelvin), 
                     41.17, 0.01882, -979500.0, 0.0, 
                     273.0, 1065.0),
            /*thermal_conductivity*/              state_invariant(5.1 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(), // 3e8 * si::pascal * si::second, // Melosh (2011), from Hiesinger (2007), for venusian lava flows, middle of range on log scale
            /*density*/                           state_invariant(5170.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  0.22,  4.18,  4.24,  4.20, 15.62, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 47.62, 50.00, 52.63, 55.56},
                     std::vector<double>{2.3, 2.3, 3.3, 3.3, 3.3, 3.5, 3.5, 3.5, 3.5, 3.5, 3.8, 4.4, 5.1, 5.3, 5.1, 4.9, 4.7, 4.6, 4.5, 4.5, 4.4, 4.4, 4.4, 4.4, 4.5, 4.7, 5.2, 6.3, 7.4, 7.3, 6.9, 6.6, 6.4, 6.3, 6.3, 6.4, 6.4, 6.5, 6.7, 6.7, 7.1, 7.3, 7.7, 7.9}),
                    // Querry (1985)
            /*extinction_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{ 0.21,  0.22,  2.83,  2.96,  9.90, 15.38, 15.62, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 45.45, 47.62, 50.00, 52.63, 55.56},
                     std::vector<double>{0.085, 0.109, 1.194, 1.180, 1.464, 2.348, 2.427, 2.626, 2.761, 2.963, 3.249, 3.642, 3.907, 3.604, 2.966, 2.540, 2.383, 2.348, 2.365, 2.402, 2.460, 2.959, 3.078, 3.224, 3.413, 3.655, 3.958, 4.392, 4.940, 5.210, 4.290, 3.268, 2.855, 2.776, 2.807, 2.858, 2.950, 3.017, 3.058, 3.185, 3.226, 3.253, 3.357, 3.331, 3.400, 3.351, 3.209}),
                    // Querry (1985)
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for magnetite
                    (si::micrometer,  233.0 *si::micrometer, 
                    std::vector<double>{  0.242,  0.257,  0.273,  0.301,  0.333,  0.392,  0.469,  0.555,  0.600,  0.703,  1.009,  1.111,  1.271,  1.755,  1.887,  1.914,  2.000,  2.019,  2.138,  2.442,  2.465,  2.496,  2.561,  2.595,  2.652,  2.720,  2.755,  2.779,  2.846,  2.882,  2.912,  2.980,  4.870, 13.431, 15.719, 16.678, 17.785, 18.745, 20.000, 22.361, 24.059, 28.929, 30.996, 32.915, 33.800, 38.671, 39.704, 40.811, 42.878, 44.428, 46.346, 55.424, 57.490, 62.287, 66.346, 70.258, 72.988, 84.575, 89.889,101.107,109.594,119.926,126.568 },
                    std::vector<double>{  0.052,  0.051,  0.058,  0.056,  0.059,  0.061,  0.057,  0.057,  0.058,  0.057,  0.044,  0.044,  0.048,  0.071,  0.075,  0.073,  0.079,  0.077,  0.081,  0.086,  0.084,  0.087,  0.086,  0.089,  0.087,  0.095,  0.081,  0.088,  0.090,  0.090,  0.102,  0.080,  0.110,  0.141,  0.164,  0.182,  0.222,  0.180,  0.175,  0.179,  0.186,  0.305,  0.270,  0.268,  0.277,  0.303,  0.300,  0.303,  0.319,  0.310,  0.324,  0.321,  0.315,  0.329,  0.315,  0.330,  0.321,  0.323,  0.340,  0.324,  0.332,  0.331,  0.324 }),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   state_invariant(2.730e11*si::pascal),
            /*shear_modulus*/                     state_invariant(0.971e11*si::pascal),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// chalcocite, copper sulfide, Cu2S, 
// representative of chalcophile ores, and for surfaces of maybe venus and mercury
PartlyKnownCompound chalcocite (
    /*molar_mass*/                        159.16 * si::gram/si::mole, 
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ 1130.0 * si::celcius, // wikipedia
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      missing(),
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   missing(),
        /*dynamic_viscosity*/      missing(),
        /*density*/                missing(),
        /*vapor_pressure*/         missing(),
        /*surface_tension*/        missing(),
        /*refractive_index*/       missing(),
        /*extinction_coefficient*/ missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid { // alpha

            /*isobaric_specific_heat_capacity*/            
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin), 
                     0.0, 0.0, 0.0,
                     si::calorie/(159.16 * si::gram*si::kelvin), 
                     9.38, 0.0312, 0.0, 0.0, 
                     273.0, 376.0),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(5600.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        },
        phase::PartlyKnownSolid { // beta
            /*isobaric_specific_heat_capacity*/     
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (159.16 * si::gram * si::kelvin),
                      std::vector<double>{0.0, 273.15},
                      std::vector<double>{0.0,   20.9}), // Perry
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(5600.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      state_invariant(242.88 * si::gigapascal), // de Jong (2015)
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     state_invariant(91.89 * si::gigapascal), // de Jong (2015)
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);
// chalcopyrite, CuFeS, 
// representative of chalcophile ores, and for surfaces of maybe venus and mercury
PartlyKnownCompound  chalcopyrite (
    /*molar_mass*/                        183.5 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   missing(),

    /*critical_point_pressure*/           missing(),
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        missing(),
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       missing(),
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             missing(),
    /*triple_point_temperature*/          missing(),
    /*freezing_point_sample_pressure*/    missing(),
    /*freezing_point_sample_temperature*/ missing(),
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*refractive_index*/                missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/            missing(),
        /*dynamic_viscosity*/               missing(),
        /*density*/                         missing(),
        /*vapor_pressure*/                  missing(),
        /*surface_tension*/                 missing(),
        /*refractive_index*/                missing(),
        /*extinction_coefficient*/          missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilojoule / (si::kilogram*si::kelvin),
                     std::vector<double>{0.0, 300.0}, 
                     std::vector<double>{0.0,  0.54}), // Cermak (1988), for chalcopyrite
            /*thermal_conductivity*/              state_invariant(8.19 * si::watt / (si::centimeter * si::kelvin)), // Cermak (1988), for chalcopyrite
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(4200.0 *  si::kilogram/si::meter3), //wikipedia
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                compound::relation::get_absorption_coefficient_function_from_reflectance_at_wavelengths // for chalcopyrite
                    (si::micrometer,  162 *si::micrometer, 
                    std::vector<double>{ 0.235, 0.246, 0.315, 0.342, 0.382, 0.406, 0.428, 0.546, 0.585, 0.700, 0.824, 0.840, 0.902, 0.951, 1.147, 1.459, 1.618, 1.656, 1.941, 2.423, 2.465, 2.527, 2.589, 2.750, 2.786, 2.812, 2.842, 2.883, 2.910, 2.942, 2.979, 3.512, 3.806, 5.509, 6.126, 6.233, 6.501, 6.675, 6.863, 7.627, 7.949, 8.190, 8.565, 8.686, 9.410, 9.651, 9.772,10.415,10.831,13.176,13.458,14.383,15.335,15.911,16.099,18.069,18.860,19.195,19.705,19.919,20.093,20.482,21.179,21.434,22.050,22.144,23.391,23.592,24.008,24.262,24.450,24.691 },
                    std::vector<double>{ 0.020, 0.031, 0.033, 0.027, 0.024, 0.026, 0.031, 0.084, 0.092, 0.099, 0.096, 0.094, 0.097, 0.098, 0.107, 0.109, 0.106, 0.107, 0.101, 0.103, 0.102, 0.104, 0.102, 0.103, 0.093, 0.103, 0.102, 0.114, 0.109, 0.110, 0.101, 0.111, 0.113, 0.119, 0.120, 0.121, 0.122, 0.122, 0.124, 0.126, 0.128, 0.124, 0.125, 0.127, 0.120, 0.120, 0.122, 0.122, 0.123, 0.123, 0.122, 0.123, 0.121, 0.121, 0.122, 0.121, 0.119, 0.117, 0.121, 0.119, 0.122, 0.118, 0.117, 0.118, 0.113, 0.114, 0.109, 0.107, 0.109, 0.107, 0.107, 0.105 }),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     missing(),
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     missing(),

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);




} // end namespace published
} // end namespace compound