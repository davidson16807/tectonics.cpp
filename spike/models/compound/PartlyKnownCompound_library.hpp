#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "field/missing.hpp"
#include "PartlyKnownCompound.hpp"

namespace compound {


// water, H2O
// for the oceans and ice caps of earth, and the surface and mantle of europa, and the surface of pluto
PartlyKnownCompound water {
    /*molar_mass*/                        18.015 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                265.0 * si::picometer, // wikipedia,  Ismail (2015)
    /*molecular_degrees_of_freedom*/      6.8,
    /*acentric_factor*/                   0.345,

    /*critical_point_pressure*/           22.064e6 * si::pascal,
    /*critical_point_volume*/             56.0 *  si::centimeter3/si::mole, // engineering toolbox
    /*critical_point_temperature*/        647.096 * si::kelvin,
    /*critical_point_compressibility*/    0.230, // engineering toolbox

    /*latent_heat_of_vaporization*/       22.6e5 * si::joule/si::kilogram, 
    /*latent_heat_of_fusion*/             0.334 * si::kilojoule / si::kilogram,
    /*triple_point_pressure*/             0.6116e3*si::pascal,
    /*triple_point_temperature*/          273.15 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ si::standard_temperature,
    /*simon_glatzel_slope*/               7070e5,
    /*simon_glatzel_exponent*/            4.46,

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{  9.73e1, 6.05e2, 7.37e3, 1.65e4, 2.98e4, 6.50e4, 9.73e4, 1.38e5, 1.62e5, 2.63e5, 3.35e5, 4.39e5, 5.15e5, 5.89e5, 6.93e5, 9.82e5, 1.25e6, 1.64e6, 2.08e6, 2.38e6, 2.41e6, 2.44e6, 2.47e6, 2.53e6, 2.59e6, 2.63e6, 2.73e6, 2.78e6, 2.93e6, 2.98e6, 3.05e6, 3.08e6, 5.11e6, 5.63e6, 6.04e6, 6.45e6, 6.86e6, 8.04e6, 9.68e6, 1.08e7, 1.24e7, 1.37e7, 2.37e7, 3.94e7, 6.98e7, 1.69e8 },
            std::vector<double>{  -24.98, -24.44, -23.93, -23.46, -23.46, -22.97, -23.70, -23.77, -23.11, -24.44, -22.46, -25.14, -24.47, -25.68, -25.10, -27.10, -28.15, -29.10, -30.25, -29.38, -29.28, -29.28, -29.47, -29.22, -29.47, -29.55, -29.28, -29.21, -29.27, -28.95, -28.71, -28.69, -25.41, -21.62, -21.41, -21.51, -21.76, -21.09, -20.98, -20.74, -20.82, -20.75, -20.83, -21.08, -21.54, -22.44 },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 2.080 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   0.016 * si::watt / (si::meter * si::kelvin), // wikipedia
        /*dynamic_viscosity*/      1.24e-5 * si::pascal * si::second, // engineering toolbox, at 100 C
        /*density*/                0.6* si::kilogram/si::meter3,
        /*refractive_index*/       1.000261 // engineering toolbox
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 4.1813 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   0.6062 * si::watt / (si::meter * si::kelvin), 
        /*dynamic_viscosity*/      0.8949 * si::millipascal * si::second, // pubchem
        /*density*/                1022.0 * si::kilogram/si::meter3, // NOTE: we use the density of seawater, because do not model the effect of solute concentration on density, otherwise 997.0
        /*vapor_pressure*/         
        field::StateFunction<si::pressure>([](const si::pressure p, const si::temperature T) {
            // Buck equation
            double C = T/si::celcius;
            return 0.61121*exp((18.678-C/234.5) * (C/(257.14+C))) * si::kilopascal; 
        }),
        /*refractive_index*/       //1.33336,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            return math::lerp(
                    std::vector<double>{-0.69, -0.53,  0.24,  0.36,  0.41,  0.45,  0.50,  0.56,  0.65,  0.73,  0.77,  0.79,  0.84,  0.97,  1.08,  1.27,  1.33,  1.46,  1.59,  1.68,  1.85,  2.00,  2.05,  2.08,  2.30f},
                    std::vector<double>{1.391, 1.351, 1.315, 1.288, 1.243, 1.148, 1.476, 1.382, 1.337, 1.310, 1.243, 1.346, 1.324, 1.256, 1.117, 1.458, 1.490, 1.548, 1.526, 1.548, 1.841, 1.957, 1.957, 2.002, 2.124f},
                    log10(l)
                );
        }) 
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            2.05 * si::joule / (si::gram * si::kelvin), // wikipedia
            /*thermal_conductivity*/              2.09 * si::watt / (si::meter * si::kelvin), // wikipedia
            /*dynamic_viscosity*/                 1e13 * si::poise, // reference by Carey (1953)
            /*density*/                           0916.9 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    138.268 * si::megapascal,
            /*refractive_index*/                  1.3098,
            /*spectral_reflectance*/              0.9,

            /*bulk_modulus*/                      8.899 * si::gigapascal, // gammon (1983)
            /*tensile_modulus*/                   9.332 * si::gigapascal, // gammon (1983)
            /*shear_modulus*/                     3.521 * si::gigapascal, // gammon (1983)
            /*pwave_modulus*/                     13.59 * si::gigapascal, // gammon (1983)
            /*lame_parameter*/                    6.552 * si::gigapascal, // gammon (1983)
            /*poisson_ratio*/                     0.3252, // gammon (1983)

            /*compressive_fracture_strength*/     6.0 * si::megapascal, //engineering toolbox
            /*tensile_fracture_strength*/         1.0 * si::megapascal, //engineering toolbox
            /*shear_fracture_strength*/           1.1 * si::megapascal, // Frederking (1989)
            /*compressive_yield_strength*/        6.0 * si::megapascal, // brittle, effectively the same as fracture strength
            /*tensile_yield_strength*/            1.0 * si::megapascal, // brittle, effectively the same as fracture strength
            /*shear_yield_strength*/              1.1 * si::megapascal, // brittle, effectively the same as fracture strength

            /*chemical_susceptibility_estimate*/  false
        }
    }
    
};



// nitrogen, N2
// for the atmosphere of Earth, and the surfaces of pluto or triton
PartlyKnownCompound nitrogen {
    /*molar_mass*/                        28.013  * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                357.8 * si::picometer, // Mehio (2014)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.040,

    /*critical_point_pressure*/           3.390 * si::megapascal,
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        126.20 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       198.8 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             12.463 * si::kilopascal,
    /*triple_point_temperature*/          63.15 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -210.0*si::celcius,
    /*simon_glatzel_slope*/               1607e5,
    /*simon_glatzel_exponent*/            1.7910,

    /*molecular_absorption_cross_section*/ 
        field::SpectralFunction<si::area>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            return std::pow(10.0, math::integral_of_lerp(
                std::vector<double>{ 8.22e6, 9.25e6, 9.94e6, 1.03e7, 1.13e7, 1.21e7, 1.89e7, 3.35e7, 3.93e7, 9.31e7, 1.07e9 },
                std::vector<double>{ -26.22, -23.04, -20.64, -20.27, -20.29, -20.62, -20.58, -20.93, -20.96, -21.80, -23.38 },
                (nlo*si::meter), (nhi*si::meter)
            ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
        }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 1.040 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   0.0234 * si::watt / (si::meter * si::kelvin), // wikipedia
        /*dynamic_viscosity*/      1.76e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
        /*density*/                field::missing(),
        /*refractive_index*/       
            field::SpectralFunction<double>([](
                const si::wavenumber nlo, 
                const si::wavenumber nhi, 
                const si::pressure p, 
                const si::temperature T
            ) {
                double l = (2.0 / (nhi+nlo) / si::micrometer);
                double invl2 = 1.0/(l*l);
                return 1.0 + 6.8552e-5 + 3.243157e-2 / (144.0 - invl2);
            }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 2.04 * si::kilojoule / (si::kilogram * si::kelvin), // Timmerhaus (1989)
        /*thermal_conductivity*/   0.1396 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
        /*dynamic_viscosity*/      157.9 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
        /*density*/                0807.0 * si::kilogram/si::meter3,
        /*vapor_pressure*/         
            field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                return (3.720822 - 293.94358*si::kelvin/T + 10.31993/si::kelvin*T) * si::standard_pressure;
            }), // Solomon (1950)
        /*refractive_index*/       1.19876,
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/                   
                field::StateFunction<si::specific_heat_capacity>([](si::pressure p, si::temperature T){ 
                    return 926.91*exp(0.0093*(T/si::kelvin))*si::joule/(si::kilogram*si::kelvin);
                }), // wikipedia
            /*thermal_conductivity*/              
                field::StateFunction<si::thermal_conductivity>([](si::pressure p, si::temperature T){ 
                    return 180.2*pow((T/si::kelvin), 0.1041)*si::watt / (si::meter * si::kelvin);
                }), // wikipedia
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           0850.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  1.25, // wikipedia
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      
                field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                    return math::mix(2.16, 1.47, math::linearstep(20.0, 44.0, T/si::kelvin))*si::gigapascal;
                }), // wikipedia
            /*tensile_modulus*/                   
                field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                    return math::mix(161.0, 225.0, math::linearstep(58.0, 40.6, T/si::kelvin))*si::megapascal;
                }), // wikipedia
            /*shear_modulus*/                     field::missing(),
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    field::missing(),
            /*poisson_ratio*/                     field::missing(),

            /*compressive_fracture_strength*/     
                field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                    return math::mix(0.24, 0.54, math::linearstep(58.0, 40.6, T/si::kelvin))*si::megapascal;
                }), // wikipedia
            /*tensile_fracture_strength*/         field::missing(),
            /*shear_fracture_strength*/           field::missing(),
            /*compressive_yield_strength*/        field::missing(),
            /*tensile_yield_strength*/            field::missing(),
            /*shear_yield_strength*/              field::missing(),

            /*chemical_susceptibility_estimate*/  field::missing()
        }
    }
};




// oxygen, O2
// for atmospheres of earth like planets
PartlyKnownCompound oxygen {
    /*molar_mass*/                        31.9988 * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                334.0 * si::picometer, // Mehio (2014)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.022,

    /*critical_point_pressure*/           5.043 * si::megapascal,
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        154.581 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       213.1 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             0.14633 * si::kilopascal,
    /*triple_point_temperature*/          54.35 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -218.79*si::celcius,
    /*simon_glatzel_slope*/               2733e5,
    /*simon_glatzel_exponent*/            1.7425,

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{  5.58e6, 5.72e6, 6.36e6, 6.93e6, 7.26e6, 7.40e6, 7.50e6, 7.65e6, 7.74e6, 7.86e6, 8.05e6, 8.24e6, 8.36e6, 8.43e6, 8.52e6, 8.66e6, 8.80e6, 8.88e6, 8.99e6, 9.06e6, 9.20e6, 9.28e6, 9.75e6, 1.05e7, 1.13e7, 1.22e7, 1.36e7, 1.46e7, 1.83e7, 2.14e7, 3.50e7, 6.28e7, 1.11e8, 4.23e8  },
            std::vector<double>{  -24.99, -22.55, -21.15, -20.81, -20.86, -21.15, -21.70, -22.61, -22.16, -23.10, -21.82, -24.11, -22.09, -24.11, -21.91, -23.78, -21.50, -23.63, -23.80, -22.07, -22.14, -22.21, -21.36, -21.04, -21.01, -20.68, -20.60, -20.65, -20.59, -20.70, -20.81, -21.28, -21.79, -23.05  },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 0.918 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   0.0238 * si::watt / (si::meter * si::kelvin), // wikipedia
        /*dynamic_viscosity*/      2.04e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
        /*density*/                field::missing(),
        /*refractive_index*/       // 1.0002709,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            constexpr double n = 1.0002709f;
            constexpr double dndl = -0.00027966;
            return n + dndl * l;
        }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 1.70 * si::kilojoule / (si::kilogram * si::kelvin), // Timmerhaus (1989)
        /*thermal_conductivity*/   0.1514 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
        /*dynamic_viscosity*/      188.0 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
        /*density*/                1141.0 * si::kilogram/si::meter3,
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       1.2243,
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            11.06 * si::calorie / (31.9988*si::gram * si::kelvin), // Johnson (1960), 10.73 for solid II, 4.4 for solid III
            /*thermal_conductivity*/              0.17 * si::watt / (si::centimeter * si::kelvin), // Jezowski (1993)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           1524.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  field::missing(),
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   field::missing(),
            /*shear_modulus*/                     field::missing(),
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    field::missing(),
            /*poisson_ratio*/                     field::missing(),

            /*compressive_fracture_strength*/     field::missing(),
            /*tensile_fracture_strength*/         field::missing(),
            /*shear_fracture_strength*/           field::missing(),
            /*compressive_yield_strength*/        field::missing(),
            /*tensile_yield_strength*/            field::missing(),
            /*shear_yield_strength*/              field::missing(),

            /*chemical_susceptibility_estimate*/  field::missing()
        }
    }
};

// carbon dioxide, CO2
// for the atmospheres earth, venus, and mars, as well as the ice caps of mars
// I am super proud of our coverage for absorption cross sections: 
// 2.5nm to 1mm with only one gap in Vis (at the 2.5e6 m^-1 mark),
// which we fill on the assumption it is invisible there.
// Truly only the best for such a common and highly influential gas!
PartlyKnownCompound carbon_dioxide {
    /*molar_mass*/                        44.01 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                346.9 * si::picometer, // Mehio (2014)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.228,

    /*critical_point_pressure*/           7380e3 * si::pascal,
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        304.19 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       205.0 * si::joule / si::gram,
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             517e3 * si::pascal,
    /*triple_point_temperature*/          216.56 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -56.56 * si::celcius,
    /*simon_glatzel_slope*/               4000e5,
    /*simon_glatzel_exponent*/            2.60,

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{ 8.58e2,  8.58e3,  2.75e4,  3.26e4,  3.78e4,  6.70e4,  8.58e4,  9.53e4,  1.00e5,  1.07e5,  1.11e5,  1.18e5,  1.27e5,  1.32e5,  1.36e5,  1.45e5,  1.53e5,  1.56e5,  1.60e5,  1.66e5,  1.69e5,  1.73e5,  1.87e5,  1.92e5,  2.00e5,  2.23e5,  2.32e5,  2.37e5,  2.40e5,  2.47e5,  2.53e5,  2.60e5,  2.69e5,  2.76e5,  2.88e5,  3.10e5,  3.41e5,  3.59e5,  3.72e5,  3.90e5,  4.30e5,  5.01e5,  5.74e5,  6.20e5,  6.57e5,  6.99e5,  8.13e5,  9.54e5,  1.07e6,  1.29e6,  1.32e6,  2.50e6,  4.95e6,  5.00e6,  6.14e6,  6.73e6,  7.17e6,  7.62e6,  8.39e6,  8.90e6,  9.21e6,  1.11e7,  1.21e7,  1.29e7,  1.52e7,  2.91e7,  4.65e7,  1.26e8,  1.89e8,  2.41e8,  4.42e8 },
            std::vector<double>{ -33.32,  -35.77,  -38.01,  -37.15,  -32.27,  -22.83,  -28.57,  -26.72,  -28.50,  -26.46,  -30.22,  -31.21,  -28.04,  -30.02,  -28.11,  -32.47,  -32.60,  -33.79,  -36.49,  -37.22,  -35.11,  -32.07,  -28.17,  -26.66,  -28.64,  -25.03,  -22.21,  -21.30,  -26.74,  -27.46,  -30.28,  -28.25,  -31.00,  -29.30,  -32.57,  -29.89,  -28.05,  -24.31,  -23.66,  -28.38,  -32.11,  -24.84,  -31.52,  -26.54,  -30.21,  -26.02,  -31.13,  -28.70,  -37.36,  -29.75,  -35.98,  -35.98,  -29.16,  -27.60,  -22.96,  -22.26,  -22.30,  -22.01,  -23.52,  -20.24,  -21.06,  -20.04,  -20.86,  -20.36,  -20.45,  -20.56,  -20.83,  -21.74,  -22.21,  -21.69,  -21.96 },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 36.61 * si::joule / (44.01 * si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   0.01662 * si::watt / ( si::meter * si::kelvin ), // wikipedia
        /*dynamic_viscosity*/      1.47e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
        /*density*/                field::missing(),
        /*refractive_index*/       // 1.0004493,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            double invl2 = 1.0/(l*l);
            return 1.0
                + 6.99100e-2/(166.175   - invl2)
                + 1.44720e-3/(79.609    - invl2)
                + 6.42941e-5/(56.3064   - invl2)
                + 5.21306e-5/(46.0196   - invl2)
                + 1.46847e-6/(0.0584738 - invl2);
        }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   0.087 * si::watt / (si::meter * si::kelvin), // wikipedia
        /*dynamic_viscosity*/      0.0712 * si::millipascal*si::second, // wikipedia data page
        /*density*/                1101.0 * si::kilogram/si::meter3,
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       1.6630,
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            47.11 * si::joule / (44.01 * si::gram * si::kelvin), // wikipedia
            // .thermal_conductivity = 720.0 * si::watt / (si::meter * si::kelvin), // Sumarakov (2003), peak conductivity, unusual for its variance
            /*thermal_conductivity*/              0.6 * si::watt / (si::meter * si::kelvin), // Melosh (2011)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           1562.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  1.4, // Warren (1986)
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   field::missing(),
            /*shear_modulus*/                     field::missing(),
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    field::missing(),
            /*poisson_ratio*/                     field::missing(),

            /*compressive_fracture_strength*/     field::missing(),
            /*tensile_fracture_strength*/         field::missing(),
            /*shear_fracture_strength*/           field::missing(),
            /*compressive_yield_strength*/        field::missing(),
            /*tensile_yield_strength*/            field::missing(),
            /*shear_yield_strength*/              field::missing(),

            /*chemical_susceptibility_estimate*/  field::missing()
        }
    }
};

// methane, CH4
// for the atmosphere of Titan, and surfaces of pluto or other kuiper belt objects
// The absorption cross section graph for CO2 also included CH4, so CH4 has good coverage as well: 
// 2nm to 1mm with virtually zero gaps or overlaps between sources! Can you believe this? This thing predicts the color of Neptune!
PartlyKnownCompound methane {
    /*molar_mass*/                        16.043 * si::gram/si::mole,
    /*atoms_per_molecule*/                5u,
    /*molecular_diameter*/                404.6 * si::picometer, // Mehio (2014)
    /*molecular_degrees_of_freedom*/      5.1,
    /*acentric_factor*/                   0.011,

    /*critical_point_pressure*/           4.592 * si::megapascal,
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        190.56 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       510.83 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             11.696 * si::kilopascal,
    /*triple_point_temperature*/          90.694 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -182.46 * si::celcius,
    /*simon_glatzel_slope*/               2080e5,
    /*simon_glatzel_exponent*/            1.698,

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{  9.47e5, 9.92e5, 1.02e6, 1.05e6, 1.07e6, 1.11e6, 1.13e6, 1.14e6, 1.16e6, 1.17e6, 1.19e6, 1.21e6, 1.25e6, 1.28e6, 1.33e6, 1.38e6, 1.40e6, 1.43e6, 1.44e6, 1.48e6, 1.50e6, 1.57e6, 1.61e6, 1.70e6, 1.73e6, 1.80e6, 1.84e6, 1.91e6, 1.96e6, 2.03e6, 2.05e6, 2.10e6, 2.15e6, 2.17e6, 2.25e6, 2.28e6, 2.30e6, 6.56e6, 7.00e6, 7.66e6, 8.65e6, 1.08e7, 2.83e7, 4.90e7, 1.82e8, 2.23e8, 2.38e8, 2.57e8, 3.12e8, 3.24e8, 3.61e8, 3.94e8, 4.45e8, 4.89e8, 5.15e8, 5.95e8, 6.36e8, 7.38e8, 7.91e8, 8.66e8, 9.40e8  },
            std::vector<double>{  -28.89, -27.28, -27.50, -29.05, -29.13, -27.31, -27.03, -28.16, -27.76, -28.91, -28.60, -30.96, -28.43, -28.67, -29.96, -27.88, -29.30, -28.99, -29.93, -29.91, -29.27, -30.48, -28.66, -31.16, -29.91, -31.27, -29.36, -31.86, -30.34, -32.07, -29.75, -31.26, -31.40, -30.69, -32.07, -30.48, -31.86, -27.37, -22.93, -20.76, -20.71, -20.22, -20.83, -21.53, -22.69, -29.75, -23.23, -21.93, -25.62, -29.16, -30.28, -28.11, -26.80, -30.93, -22.60, -27.39, -31.20, -27.26, -33.23, -27.72, -33.23  },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 2.191 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   34.4 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
        /*dynamic_viscosity*/      1.10e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
        /*density*/                field::missing(),
        /*refractive_index*/       // 1.000444,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            return math::lerp(
                    std::vector<double>{    1.67,     2.70,     3.01,     3.66,     3.79,     4.46,     5.66,     6.51,     7.00,     8.38,     8.95,    10.09,    10.86,    11.54,    11.93,    12.37,    13.22,    13.63,    14.02,    14.83},
                    std::vector<double>{1.000430, 1.000425, 1.000417, 1.000440, 1.000437, 1.000431, 1.000427, 1.000419, 1.000402, 1.000466, 1.000451, 1.000445, 1.000442, 1.000443, 1.000440, 1.000441, 1.000440, 1.000439, 1.000444, 1.000439},
                    log10(l)
                );
        }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 3.45 * si::kilojoule / (si::kilogram * si::kelvin), // Timmerhaus (1989)
        /*thermal_conductivity*/   0.1931 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
        /*dynamic_viscosity*/      118.6 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
        /*density*/                0422.4 * si::kilogram/si::meter3, 
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       1.2730, 
    },


    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            5.193 * si::calorie / (16.043*si::gram * si::kelvin), // Johnson (1960)
            /*thermal_conductivity*/              0.010 * si::watt / (si::centimeter * si::kelvin), // Jezowski (1997)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           0522.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  1.3219,
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   field::missing(),
            /*shear_modulus*/                     field::missing(),
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    field::missing(),
            /*poisson_ratio*/                     field::missing(),

            /*compressive_fracture_strength*/     field::missing(),
            /*tensile_fracture_strength*/         field::missing(),
            /*shear_fracture_strength*/           field::missing(),
            /*compressive_yield_strength*/        field::missing(),
            /*tensile_yield_strength*/            field::missing(),
            /*shear_yield_strength*/              field::missing(),

            /*chemical_susceptibility_estimate*/  field::missing(),
        }
    }
};




// argon, Ar
// for the atmospheres of earth like planets
PartlyKnownCompound argon {
    /*molar_mass*/                        39.948 * si::gram/si::mole,
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                340.0 * si::picometer, // wikipedia, Breck (1974)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.0,

    /*critical_point_pressure*/           4.860 * si::megapascal,
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        150.663 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       161.0 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             68.95 * si::kilopascal,
    /*triple_point_temperature*/          83.8058 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -189.36*si::celcius,
    /*simon_glatzel_slope*/               2114e5,
    /*simon_glatzel_exponent*/            1.593,
    
    /*molecular_absorption_cross_section*/ 1e-35*si::meter2,

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 0.5203 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   0.016 * si::watt / ( si::meter * si::kelvin ),  // wikipedia
        /*dynamic_viscosity*/      2.23e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
        /*density*/                field::missing(),
        /*refractive_index*/       // 1.000281,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            double invl2 = 1.0/(l*l);
            return 1.0
                + 2.50141e-3/(91.012  - invl2)
                + 5.00283e-4/(87.892  - invl2)
                + 5.22343e-2/(214.02  - invl2);
        }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 1.14 * si::kilojoule / (si::kilogram * si::kelvin), // Timmerhaus (1989)
        /*thermal_conductivity*/   0.1232 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
        /*dynamic_viscosity*/      252.1 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
        /*density*/                1401.0 * si::kilogram/si::meter3,
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       1.23,
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            0.197 * si::calorie / (si::gram * si::kelvin), // Johnson (1960)
            /*thermal_conductivity*/              0.045 * si::watt / (si::centimeter * si::kelvin), // White (1956)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           1680.7 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  1.2703,
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   0.0529e11*si::pascal,
            /*shear_modulus*/                     0.0159e11*si::pascal,
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    field::missing(),
            /*poisson_ratio*/                     field::missing(),

            /*compressive_fracture_strength*/     field::missing(),
            /*tensile_fracture_strength*/         field::missing(),
            /*shear_fracture_strength*/           field::missing(),
            /*compressive_yield_strength*/        field::missing(),
            /*tensile_yield_strength*/            field::missing(),
            /*shear_yield_strength*/              field::missing(),

            /*chemical_susceptibility_estimate*/  field::missing()
        }
    }
};

} // end namespace compound