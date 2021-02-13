
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <math/lerp.hpp>

#include <units/si.hpp>
#include <units/si_test_utils.hpp>

#include "PartlyKnownCompound.hpp"

#include "field/missing.hpp"

#include "field/state/OptionalStateField_test_utils.hpp"
#include "field/spectral/OptionalSpectralField_test_utils.hpp"

#include "phase/gas/PartlyKnownGas_test_utils.hpp"
#include "phase/liquid/PartlyKnownLiquid_test_utils.hpp"
#include "phase/solid/PartlyKnownSolid_test_utils.hpp"


compound::PartlyKnownCompound unknown {
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
    /*simon_glatzel_slope*/               compound::field::missing(),
    /*simon_glatzel_exponent*/            compound::field::missing(),

    /*molecular_absorption_cross_section*/ compound::field::missing(),

    /*gas*/
    compound::phase::PartlyKnownGas {
        /*specific_heat_capacity*/ compound::field::missing(),
        /*thermal_conductivity*/   compound::field::missing(),
        /*dynamic_viscosity*/      compound::field::missing(),
        /*density*/                compound::field::missing(),
        /*refractive_index*/       compound::field::missing(),
    },

    /*liquid*/
    compound::phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ compound::field::missing(),
        /*thermal_conductivity*/   compound::field::missing(),
        /*dynamic_viscosity*/      compound::field::missing(),
        /*density*/                compound::field::missing(),
        /*vapor_pressure*/         compound::field::missing(),
        /*refractive_index*/       compound::field::missing(),
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

compound::PartlyKnownCompound water {
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
    compound::field::SpectralFunction<si::area>([](
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
    compound::phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 2.080 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   0.016 * si::watt / (si::meter * si::kelvin), // wikipedia
        /*dynamic_viscosity*/      1.24e-5 * si::pascal * si::second, // engineering toolbox, at 100 C
        /*density*/                0.6* si::kilogram/si::meter3,
        /*refractive_index*/       1.000261 // engineering toolbox
    },

    /*liquid*/
    compound::phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 4.1813 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   0.6062 * si::watt / (si::meter * si::kelvin), 
        /*dynamic_viscosity*/      0.8949 * si::millipascal * si::second, // pubchem
        /*density*/                1022.0 * si::kilogram/si::meter3, // NOTE: we use the density of seawater, because do not model the effect of solute concentration on density, otherwise 997.0
        /*vapor_pressure*/         
        compound::field::StateFunction<si::pressure>([](const si::pressure p, const si::temperature T) {
            // Buck equation
            double C = T/si::celcius;
            return 0.61121*exp((18.678-C/234.5) * (C/(257.14+C))) * si::kilopascal; 
        }),
        /*refractive_index*/       //1.33336,
        compound::field::SpectralFunction<double>([](
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
    std::vector<compound::phase::PartlyKnownSolid>{
        compound::phase::PartlyKnownSolid {
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



compound::PartlyKnownCompound nitrogen {
    /*molar_mass*/                        28.013  * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                357.8 * si::picometer, // Mehio (2014)
    /*molecular_degrees_of_freedom*/      compound::field::missing(),
    /*acentric_factor*/                   0.040,

    /*critical_point_pressure*/           3.390 * si::megapascal,
    /*critical_point_volume*/             compound::field::missing(),
    /*critical_point_temperature*/        126.20 * si::kelvin,
    /*critical_point_compressibility*/    compound::field::missing(),

    /*latent_heat_of_vaporization*/       198.8 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             compound::field::missing(),
    /*triple_point_pressure*/             12.463 * si::kilopascal,
    /*triple_point_temperature*/          63.15 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -210.0*si::celcius,
    /*simon_glatzel_slope*/               1607e5,
    /*simon_glatzel_exponent*/            1.7910,

    /*molecular_absorption_cross_section*/ 
        compound::field::SpectralFunction<si::area>([](
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
    compound::phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 1.040 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   0.0234 * si::watt / (si::meter * si::kelvin), // wikipedia
        /*dynamic_viscosity*/      1.76e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
        /*density*/                compound::field::missing(),
        /*refractive_index*/       
            compound::field::SpectralFunction<double>([](
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
    compound::phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 2.04 * si::kilojoule / (si::kilogram * si::kelvin), // Timmerhaus (1989)
        /*thermal_conductivity*/   0.1396 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
        /*dynamic_viscosity*/      157.9 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
        /*density*/                0807.0 * si::kilogram/si::meter3,
        /*vapor_pressure*/         
            compound::field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                return (3.720822 - 293.94358*si::kelvin/T + 10.31993/si::kelvin*T) * si::standard_pressure;
            }), // Solomon (1950)
        /*refractive_index*/       1.19876,
    },

    /*solid*/ 
    std::vector<compound::phase::PartlyKnownSolid>{
        compound::phase::PartlyKnownSolid {
            /*specific_heat_capacity*/                   
                compound::field::StateFunction<si::specific_heat_capacity>([](si::pressure p, si::temperature T){ 
                    return 926.91*exp(0.0093*(T/si::kelvin))*si::joule/(si::kilogram*si::kelvin);
                }), // wikipedia
            /*thermal_conductivity*/              
                compound::field::StateFunction<si::thermal_conductivity>([](si::pressure p, si::temperature T){ 
                    return 180.2*pow((T/si::kelvin), 0.1041)*si::watt / (si::meter * si::kelvin);
                }), // wikipedia
            /*dynamic_viscosity*/                 compound::field::missing(),
            /*density*/                           0850.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    compound::field::missing(),
            /*refractive_index*/                  1.25, // wikipedia
            /*spectral_reflectance*/              compound::field::missing(),

            /*bulk_modulus*/                      
                compound::field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                    return math::mix(2.16, 1.47, math::linearstep(20.0, 44.0, T/si::kelvin))*si::gigapascal;
                }), // wikipedia
            /*tensile_modulus*/                   
                compound::field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                    return math::mix(161.0, 225.0, math::linearstep(58.0, 40.6, T/si::kelvin))*si::megapascal;
                }), // wikipedia
            /*shear_modulus*/                     compound::field::missing(),
            /*pwave_modulus*/                     compound::field::missing(),
            /*lame_parameter*/                    compound::field::missing(),
            /*poisson_ratio*/                     compound::field::missing(),

            /*compressive_fracture_strength*/     
                compound::field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                    return math::mix(0.24, 0.54, math::linearstep(58.0, 40.6, T/si::kelvin))*si::megapascal;
                }), // wikipedia
            /*tensile_fracture_strength*/         compound::field::missing(),
            /*shear_fracture_strength*/           compound::field::missing(),
            /*compressive_yield_strength*/        compound::field::missing(),
            /*tensile_yield_strength*/            compound::field::missing(),
            /*shear_yield_strength*/              compound::field::missing(),

            /*chemical_susceptibility_estimate*/  compound::field::missing()
        }
    }
};




// oxygen, O2
// for atmospheres of earth like planets
compound::PartlyKnownCompound oxygen {
    /*molar_mass*/                        31.9988 * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                334.0 * si::picometer, // Mehio (2014)
    /*molecular_degrees_of_freedom*/      compound::field::missing(),
    /*acentric_factor*/                   0.022,

    /*critical_point_pressure*/           5.043 * si::megapascal,
    /*critical_point_volume*/             compound::field::missing(),
    /*critical_point_temperature*/        154.581 * si::kelvin,
    /*critical_point_compressibility*/    compound::field::missing(),

    /*latent_heat_of_vaporization*/       213.1 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             compound::field::missing(),
    /*triple_point_pressure*/             0.14633 * si::kilopascal,
    /*triple_point_temperature*/          54.35 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -218.79*si::celcius,
    /*simon_glatzel_slope*/               2733e5,
    /*simon_glatzel_exponent*/            1.7425,

    /*molecular_absorption_cross_section*/ 
    compound::field::SpectralFunction<si::area>([](
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
    compound::phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 0.918 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   0.0238 * si::watt / (si::meter * si::kelvin), // wikipedia
        /*dynamic_viscosity*/      2.04e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
        /*density*/                compound::field::missing(),
        /*refractive_index*/       // 1.0002709,
        compound::field::SpectralFunction<double>([](
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
    compound::phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 1.70 * si::kilojoule / (si::kilogram * si::kelvin), // Timmerhaus (1989)
        /*thermal_conductivity*/   0.1514 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
        /*dynamic_viscosity*/      188.0 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
        /*density*/                1141.0 * si::kilogram/si::meter3,
        /*vapor_pressure*/         compound::field::missing(),
        /*refractive_index*/       1.2243,
    },

    /*solid*/ 
    std::vector<compound::phase::PartlyKnownSolid>{
        compound::phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            11.06 * si::calorie / (31.9988*si::gram * si::kelvin), // Johnson (1960), 10.73 for solid II, 4.4 for solid III
            /*thermal_conductivity*/              0.17 * si::watt / (si::centimeter * si::kelvin), // Jezowski (1993)
            /*dynamic_viscosity*/                 compound::field::missing(),
            /*density*/                           1524.0 * si::kilogram/si::meter3,
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

TEST_CASE( "PartlyKnownCompound value_or() purity", "[compound]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(water.value_or(water) == water.value_or(water));
        CHECK(nitrogen.value_or(nitrogen) == nitrogen.value_or(nitrogen));
        CHECK(nitrogen.value_or(water) == nitrogen.value_or(water));
        CHECK(water.value_or(nitrogen) == water.value_or(nitrogen));
    }
}
TEST_CASE( "PartlyKnownCompound value_or() left identity", "[compound]" ) {
    SECTION("There exists a value that when applied as the first operand to a function returns the original value")
    {
        CHECK(water.value_or(water) == water);
        CHECK(water.value_or(nitrogen) == water);
    }
}
TEST_CASE( "PartlyKnownCompound value_or() right identity", "[compound]" ) {
    SECTION("There exists a value that when applied as the second operand to a function returns the original value")
    {
        CHECK(water.value_or(unknown) == water);
        CHECK(nitrogen.value_or(unknown) == nitrogen);
    }
}
TEST_CASE( "PartlyKnownCompound value_or() associativity", "[compound]" ) {
    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(nitrogen.value_or(water.value_or(oxygen)) == 
              nitrogen.value_or(water).value_or(oxygen));

        CHECK(nitrogen.value_or(oxygen.value_or(water)) == 
              nitrogen.value_or(oxygen).value_or(water));

        
        CHECK(water.value_or(nitrogen.value_or(oxygen)) == 
              water.value_or(nitrogen).value_or(oxygen));

        CHECK(water.value_or(oxygen.value_or(nitrogen)) == 
              water.value_or(oxygen).value_or(nitrogen));

        
        CHECK(oxygen.value_or(nitrogen.value_or(oxygen)) == 
              oxygen.value_or(nitrogen).value_or(oxygen));

        CHECK(oxygen.value_or(oxygen.value_or(nitrogen)) == 
              oxygen.value_or(oxygen).value_or(nitrogen));
    }
}

/*
TEST_CASE( "PartlyKnownCompound value_or() increasing", "[compound]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(PartlyKnownCompound_attribute_index_sum(water.value_or(water)) >= PartlyKnownCompound_attribute_index_sum(water));
        CHECK(PartlyKnownCompound_attribute_index_sum(water.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(water));
        CHECK(PartlyKnownCompound_attribute_index_sum(water.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(water));
        CHECK(PartlyKnownCompound_attribute_index_sum(water.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(water));


        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(water)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));


        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(water)) >= PartlyKnownCompound_attribute_index_sum(known_steam));
        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(known_steam));
        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(known_steam));
        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(known_steam));


        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(water)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));

    }
}
*/
