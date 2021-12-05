#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "state_functions.hpp"

#include "CompletedCompound.hpp"
#include "PartlyKnownCompound.hpp"
#include "PartlyKnownCompound_operators.hpp"
#include "PartlyKnownCompound_to_CompletedCompound.hpp"

#include "published.hpp"

namespace compound {
    namespace fallback {
        // water, H2O
        // for the oceans and ice caps of earth, and the surface and mantle of europa, and the surface of pluto
        CompletedCompound water (
            /*molar_mass*/                        18.015 * si::gram/si::mole,
            /*atoms_per_molecule*/                3u,
            /*molecular_diameter*/                265.0 * si::picometer,                                    // wikipedia,  Ismail (2015)
            /*molecular_degrees_of_freedom*/      6.8,
            /*acentric_factor*/                   0.345,

            /*critical_point_pressure*/           22.06 *  si::megapascal,
            /*critical_point_volume*/             56.0 *  si::centimeter3/si::mole,                         
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
            get_completed_molecular_absorption_cross_section_function
                ( 1.0/si::meter, si::meter2,
                  std::vector<double>{  9.73e1, 6.05e2, 7.37e3, 1.65e4, 2.98e4, 6.50e4, 9.73e4, 1.38e5, 1.62e5, 2.63e5, 3.35e5, 4.39e5, 5.15e5, 5.89e5, 6.93e5, 9.82e5, 1.25e6, 1.64e6, 2.08e6, 2.38e6, 2.41e6, 2.44e6, 2.47e6, 2.53e6, 2.59e6, 2.63e6, 2.73e6, 2.78e6, 2.93e6, 2.98e6, 3.05e6, 3.08e6, 5.11e6, 5.63e6, 6.04e6, 6.45e6, 6.86e6, 8.04e6, 9.68e6, 1.08e7, 1.24e7, 1.37e7, 2.37e7, 3.94e7, 6.98e7, 1.69e8 },
                  std::vector<double>{  -24.98, -24.44, -23.93, -23.46, -23.46, -22.97, -23.70, -23.77, -23.11, -24.44, -22.46, -25.14, -24.47, -25.68, -25.10, -27.10, -28.15, -29.10, -30.25, -29.38, -29.28, -29.28, -29.47, -29.22, -29.47, -29.55, -29.28, -29.21, -29.27, -28.95, -28.71, -28.69, -25.41, -21.62, -21.41, -21.51, -21.76, -21.09, -20.98, -20.74, -20.82, -20.75, -20.83, -21.08, -21.54, -22.44 }),

            /*gas*/
            phase::CompletedGas {
                /*specific_heat_capacity*/ // 2.080 * si::joule / (si::gram * si::kelvin),                     // wikipedia
                    get_completed_exponent_pressure_temperature_function
                        (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                        0.01766, 0.80539, 0.00707, 0.69586, 1.42782), 
                        // water, mean error: 0.8%, max error: 3.4%, range: 300-1273.2K, 0-10MPa, stp estimate: 1.781
                /*thermal_conductivity*/   // 0.016 * si::watt / (si::meter * si::kelvin),                     // wikipedia
                    get_completed_sigmoid_exponent_pressure_temperature_function
                        (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                        0.00054, 1.09614, 0.00000, 0.00000, 0.09827, 691.90362, 883.95160, 0.08323), 
                        // water, mean error: 2.5%, max error: 9.7%, range: 300-1273.2K, 0-10MPa, stp estimate: 0.018
                /*dynamic_viscosity*/      // 1.24e-5 * si::pascal * si::second,                               // engineering toolbox, at 100 C
                    get_completed_exponent_pressure_temperature_function
                        (si::kelvin, si::megapascal, si::micropascal*si::second, 
                        0.00019, 3.33694, 0.02183, 1.08016, -0.58257), 
                        // water, mean error: 1.2%, max error: 3.5%, range: 300-1273.2K, 0-10MPa, stp estimate: 8.765
                /*density*/                0.6* si::kilogram/si::meter3,
                /*refractive_index*/       1.000261                                                         // engineering toolbox
            },

            /*liquid*/
            phase::CompletedLiquid {
                /*specific_heat_capacity*/ // 4.1813 * si::joule / (si::gram * si::kelvin),                    // wikipedia
                    get_completed_dippr_liquid_heat_capacity_temperature_function_100
                        (si::kelvin, si::joule / (18.01528 * si::kilogram * si::kelvin), 
                        276370.0, -2090.1, 8.125, -0.014116, 9.3701e-6,
                        273.16*si::kelvin, 533.15*si::kelvin), 
                /*thermal_conductivity*/   // 0.6062 * si::watt / (si::meter * si::kelvin), 
                    get_completed_dippr_liquid_thermal_conductivity_temperature_function
                        (si::kelvin, si::watt / (si::meter * si::kelvin),
                         -0.432, 0.0057255, -0.000008078, 1.861e-9, 0.0), // 273.15-633.15K
                /*dynamic_viscosity*/      
                    get_completed_dippr_liquid_viscosity_temperature_function
                        (si::kelvin, si::pascal* si::second, 
                         -52.843, 3703.6, 5.866, -5.879e-29, 10.0), // 273.16-646.15
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
                    // get_completed_antoine_vapor_pressure_function(
                    //     si::celcius, si::millimeter_mercury, 
                    //     7.94917, 1657.462, 1474.68, 213.69), // Physical and Chemical Equilibrium for Chemical Engineers, Second Edition. 
                    // get_completed_dippr_liquid_vapor_pressure_temperature_function
                    //     (si::kelvin, si::pascal,
                    //      73.649, -7258.2, -7.3037, 4.1653e-6),//273.16-647.1K
                    field::StateFunction<si::pressure>([](const si::pressure p, const si::temperature T) {
                        // Buck equation
                        double C = T/si::celcius;
                        return 0.61121*exp((18.678-C/234.5) * (C/(257.14+C))) * si::kilopascal; 
                    }),
                /*surface_tension*/            
                    get_completed_refprop_liquid_surface_tension_temperature_function
                        (647.01 * si::kelvin, si::millinewton/si::meter,
                         -0.1306, 2.471, 0.2151, 1.233, 0.0, 0.0), // Mulero (2012)
                /*refractive_index*/       //1.33336,
                    get_completed_linear_interpolated_refractive_index_function
                        (si::micrometer, 
                         std::vector<double>{0.2,   0.3,   0.425,  0.55, 0.675,   0.8, 0.925,   1.4,   2.4,   2.8,  3.05,  3.3,   5.6,   6.6,   7.6,  13.0,  15.5,  18.0,  21.0,  26.0,  32.0,  42.0,  60.0, 110.0, 160.0,200.0},
                         std::vector<double>{1.396, 1.349, 1.338, 1.333, 1.331, 1.329, 1.328, 1.321, 1.279, 1.142, 1.426, 1.45, 1.289, 1.334, 1.302, 1.146, 1.297, 1.423, 1.487, 1.539, 1.546, 1.522, 1.703, 1.966, 2.081, 2.13}),
                        // Hale (1973)
                /*extinction_coefficient*/ 
                    get_completed_linear_interpolated_refractive_index_function
                        (si::micrometer, 
                         std::vector<double>{0.2,     0.3,     0.425,   0.55,     0.675,    0.8,      0.925,    1.4,      2.8,   3.0,   3.3,    3.6,     4.1,     4.6,    5.6,    6.1,      6.6,    9.2,   10.5,  13.0,  15.5,  18.0,  32.0,  60.0, 200.0},
                         std::vector<double>{1.1e-07, 1.6e-08, 1.3e-09, 1.96e-09, 2.23e-08, 1.25e-07, 1.06e-06, 0.000138, 0.115, 0.272, 0.0368, 0.00515, 0.00562, 0.0147, 0.0142, 0.131, 0.0356, 0.0415, 0.0662, 0.305, 0.414, 0.426, 0.324, 0.587, 0.504})
                        // Hale (1973)
            },

            /*solid*/ 
            std::vector<phase::CompletedSolid>{
                phase::CompletedSolid {
                    /*specific_heat_capacity*/            2.05 * si::joule / (si::gram * si::kelvin),       // wikipedia
                    /*thermal_conductivity*/              2.09 * si::watt / (si::meter * si::kelvin),       // wikipedia
                    /*dynamic_viscosity*/                 1e13 * si::poise,                                 // reference by Carey (1953)
                    /*density*/                           0916.9 * si::kilogram/si::meter3,
                    /*vapor_pressure*/                    //138.268 * si::megapascal,
                        get_completed_interpolated_temperature_function
                            (si::celcius, si::millimeter_mercury,
                             std::vector<double>{-17.3,  11.2,   51.6,   100.0  }, 
                             std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
                    /*refractive_index*/       //1.33336,
                        get_completed_linear_interpolated_refractive_index_function
                            (si::micrometer, 
                             std::vector<double>{0.0443, 0.0528, 0.067,  0.0886, 0.1181, 0.137,  0.1468, 0.158,  0.177,  0.197,  0.41,   0.51,   0.81,   1.515,  1.76,   1.923,  2.29,   2.52,   2.817,  2.985,  3.175,  3.39,   3.775,  4.56,   5.714,  6.329,  7.143,  8.333,  10.31,  11.9,   14.08,  17.24,  22.22,  34.48,  46.15,  55,     62.5,   76.29,  210, 1300.0},
                             std::vector<double>{0.8228, 0.8505, 0.9679, 1.3807, 1.3783, 1.2849, 1.5729, 1.6309, 1.4462, 1.3974, 1.3185, 1.3126, 1.3047, 1.2914, 1.2843, 1.2771, 1.2545, 1.2232, 1.0657, 1.018, 1.6336, 1.5086, 1.385, 1.3482, 1.2933, 1.3178, 1.3236, 1.2964, 1.1501, 1.2546, 1.5701, 1.5303, 1.4575, 1.253, 1.2543, 1.7955, 1.6372, 1.8504, 1.8114, 1.7868}),
                            // Warren (2008)
                    /*extinction_coefficient*/ 
                        get_completed_linear_interpolated_refractive_index_function
                            (si::micrometer, 
                             std::vector<double>{0.0443,   0.0528,   0.067,    0.0886,   0.1181,   0.137,    0.1468,   0.158,    0.177,    0.197,    0.41,     0.51,     0.61,     0.71,     0.81,     0.91,     1.01,     1.11,     1.21,     1.31,     1.41,     1.515,    1.76,     1.923,    2.13,     2.29,     2.52,     2.817,    3.003,    3.175,    3.39,     3.775,    4.56,     5.714,    6.329,    7.143,    8.333,    10.31,    11.9,     14.08,    17.24,    22.22,    34.48,    46.15,    55,       76.29,    210,      140000,   240000,   2000000  },
                             std::vector<double>{0.164,    0.268,    0.492,    0.468,    0.244,    0.271,    0.345,    0.083,    1.061e-05,2.816e-10,2.669e-11,8.036e-10,6.89e-09, 3.44e-08, 1.4e-07,  4.44e-07, 2e-06,    1.76e-06, 8.66e-06, 1.31e-05, 3.442e-05,0.0005143,0.0001522,0.000706, 0.0005255,0.0002538,0.0007685,0.03245,  0.438,    0.372,    0.04286,  0.006966, 0.03007,  0.03256,  0.0617,   0.0494,   0.037,    0.075,    0.409,    0.271,    0.107,    0.029,    0.1615,   0.7684,   0.4214,   0.334,    0.04282,  9.36e-05, 0.0001066,0.0006596}),
                            // Warren (2008)
                    /*absorption_coefficient*/            // 0.9,
                        get_completed_absorption_coefficient_function_at_wavenumbers
                           (1.0/si::centimeter, 1.0/si::centimeter, 
                            std::vector<double>{ 10257.4, 10118.8,  9930.7,  9722.8,  9445.5,  9306.9,  9227.7,  9069.3,  8802.0,  8574.3,  8346.5,  7920.8,  7752.5,  7653.5,  7485.1,  7198.0,  7019.8,  6752.5,  6663.4,  6564.4,  6386.1,  6178.2,  6059.4,  5990.1,  5762.4,  5425.7,  5346.5,  5198.0,  5039.6,  4841.6,  4782.2,  4683.2,  4465.3,  4257.4,  4118.8,  3980.2,  3821.8,  3752.5,  3604.0,  2940.6,  2861.4,  2762.4,  2574.3,  2366.3,  2287.1 },
                            std::vector<double>{  -0.982,  -0.375,   0.089,   0.321,   0.393,   0.036,  -0.304,  -0.429,  -0.446,  -0.107,   0.554,   1.107,   1.125,   1.179,   0.946,   1.000,   1.268,   2.482,   2.679,   2.625,   2.589,   2.179,   2.482,   2.161,   1.625,   1.625,   1.732,   2.500,   2.964,   2.946,   2.768,   2.339,   1.661,   2.357,   2.393,   2.482,   2.393,   2.571,   3.589,   3.571,   3.268,   3.286,   3.571,   3.661,   3.589 }),


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
        );
    } 


    namespace library{
        /*
        water

         hydrogen
          helium
         
         nitrogen
          oxygen
          argon
          ammonia
          ozone
          nitrous_oxide
          sulfur_dioxide
          hydrogen_cyanide
          nitric_oxide
         

         carbon_dioxide
          carbon_monoxide
          ethane
           ethanol
          formaldehyde
           formic_acid
           benzene
            pyrimidine
          methane
           perflouromethane
          quartz
           corundum
           carbon
           orthoclase
           andesine
           augite
           forsterite
           hematite
           goethite
           magnetite
           pyrite
            chalcocite
            chalcopyrite
            copper
             silver
             gold
   
         halite
          calcite
          apatite

        */

        CompletedCompound water = compound::complete( speculate(infer(published::water)), fallback::water );

        CompletedCompound hydrogen = compound::complete( speculate(infer(published::hydrogen)), fallback::water );
        CompletedCompound helium = compound::complete( speculate(infer(published::helium)), hydrogen );
        CompletedCompound argon = compound::complete( speculate(infer(published::argon)), helium );

        CompletedCompound nitrogen = compound::complete( speculate(infer(published::nitrogen)), fallback::water );
        CompletedCompound oxygen = compound::complete( speculate(infer(published::oxygen)), nitrogen );
        CompletedCompound ammonia = compound::complete( speculate(infer(published::ammonia)), nitrogen );
        CompletedCompound ozone = compound::complete( speculate(infer(published::ozone)), oxygen );
        CompletedCompound sulfur_dioxide = compound::complete( speculate(infer(published::sulfur_dioxide)), nitrogen );
        CompletedCompound nitrous_oxide = compound::complete( speculate(infer(published::nitrous_oxide)), nitrogen );
        CompletedCompound nitric_oxide = compound::complete( speculate(infer(published::nitric_oxide)), nitrogen );

        CompletedCompound carbon_dioxide = compound::complete( speculate(infer(published::carbon_dioxide)), fallback::water );
        CompletedCompound carbon_monoxide = compound::complete( speculate(infer(published::carbon_monoxide)), carbon_dioxide );
        CompletedCompound hydrogen_cyanide = compound::complete( speculate(infer(published::hydrogen_cyanide)), carbon_dioxide );
        CompletedCompound methane = compound::complete( speculate(infer(published::methane)), carbon_dioxide );
        CompletedCompound perflouromethane = compound::complete( speculate(infer(published::perflouromethane)), methane );
        CompletedCompound formaldehyde = compound::complete( speculate(infer(published::formaldehyde)), methane );
        CompletedCompound formic_acid = compound::complete( speculate(infer(published::formic_acid)), formaldehyde );
        CompletedCompound benzene = compound::complete( speculate(infer(published::benzene)), formaldehyde );
        CompletedCompound pyrimidine = compound::complete( speculate(infer(published::pyrimidine)), benzene );
        CompletedCompound ethane = compound::complete( speculate(infer(published::ethane)), methane );
        CompletedCompound ethanol = compound::complete( speculate(infer(published::ethanol)), ethane );

        CompletedCompound quartz = compound::complete( speculate(infer(published::quartz)), carbon_dioxide);
        CompletedCompound corundum = compound::complete( speculate(infer(published::corundum)), quartz);
        CompletedCompound carbon = compound::complete( speculate(infer(published::carbon)), quartz);
        CompletedCompound orthoclase = compound::complete( speculate(infer(published::orthoclase)), quartz);
        CompletedCompound andesine = compound::complete( speculate(infer(published::andesine)), quartz);
        CompletedCompound augite = compound::complete( speculate(infer(published::augite)), quartz);
        CompletedCompound forsterite = compound::complete( speculate(infer(published::forsterite)), quartz);
        CompletedCompound hematite = compound::complete( speculate(infer(published::hematite)), quartz);
        CompletedCompound goethite = compound::complete( speculate(infer(published::goethite)), quartz);
        CompletedCompound magnetite = compound::complete( speculate(infer(published::magnetite)), quartz);

        CompletedCompound pyrite = compound::complete( speculate(infer(published::pyrite)), quartz);
        CompletedCompound chalcocite = compound::complete( speculate(infer(published::chalcocite)), pyrite);
        CompletedCompound chalcopyrite = compound::complete( speculate(infer(published::chalcopyrite)), pyrite);
        CompletedCompound copper = compound::complete( speculate(infer(published::copper)), chalcopyrite);
        CompletedCompound silver = compound::complete( speculate(infer(published::silver)), copper);
        CompletedCompound gold = compound::complete( speculate(infer(published::gold)), copper);

        CompletedCompound halite = compound::complete( speculate(infer(published::halite)), fallback::water);
        CompletedCompound calcite = compound::complete( speculate(infer(published::calcite)), halite);
        CompletedCompound apatite = compound::complete( speculate(infer(published::apatite)), halite);

    }

} // end namespace compound
