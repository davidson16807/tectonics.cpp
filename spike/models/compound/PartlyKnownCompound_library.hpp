#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "field/missing.hpp"
#include "PartlyKnownCompound.hpp"

namespace compound {

/* 
Hi, and welcome to Crazy Eddie's Chemical Properties Emporium, where WE'VE GONE CRAZY!

Unless otherwise noted, all data should be assumed to be from the 91st Handbook Of Chemistry And Physics. 
Molecular absorption samples are taken from the MPI-Mainz UV/VIS Spectral Atlas of Gaseous Molecules of Atmospheric Interest.
Refraction index functions are taken from refractionindex.info.

NOTE:
Specific heat always denotes specific heat at constant pressure, c_p.
This is the only convention that's not enforced by the type system. 

NOTE:
Some compounds, such as water or nitrogen, occur consistently enough
that it is straight forward to collect data on their properties 
and represent their mass pools within a model.
Other compounds, such as orthoclases or pyroxenes, form into groups,
and the compounds within a group are so numerous that we have no hope to collect data on all of them, 
let alone store mass pools for each compound in a group.
So some mass pools in our model represent classes of compound rather than any individual compound.
However that still leaves us with the problem of collecting data on their properties.
Deciding a representative value for a particular class of compound 
requires studying the properties of several compounds in order to pick the best one;
it requires ample documentation in case we need to revisit those values if a problem is discovered with them;
and due to the added work, it is also highly error prone.
So to avoid these complications we simply pick a single member from each class to represent the entire class.
We then only have to find and document the chemical properties for that single compound.
The compound we choose to represent a class is typically the member of the class that occurs most frequently,
or is most noteworthy if none stand out.
Next to each compound we document in comments what that compound is supposed to represent within the model,
and why the compound was chosen as the representative for its class.
If a compound occurs frequently enough to be represented by its own mass pool,
we simply document why that compound was chosen for use within the model.
Along with this documentation, we include in comments multiple names for the compound where present, 
including its chemical formula, and any notes about the quality of data, especially concerning absorption cross sections

SIDE NOTE:
There are likely some among you who think this is excessive. That is all.
*/   

// "GOLD STANDARD" COMPOUNDS:
// Compounds for which almost all properties have published values
// These can serve as representative "fallback" values for other compounds
// whose properties are not published.
// See PartlyKnownCompound for implementation details concerning this.


// water, H2O
// for the oceans and ice caps of earth, and the surface and mantle of europa, and the surface of pluto
PartlyKnownCompound water {
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
    /*latent_heat_of_fusion*/             6.01*si::kilojoule/(molar_mass*si::gram),
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
        /*specific_heat_capacity*/ 2.080 * si::joule / (si::gram * si::kelvin),                     // wikipedia
        /*thermal_conductivity*/   // 0.016 * si::watt / (si::meter * si::kelvin),                     // wikipedia
            field::get_exponential_temperature_function<si::thermal_conductivity>( 300.0*si::kelvin,                           400.0*si::kelvin,                            600.0*si::kelvin,
                                                                                    18.7*si::milliwatt/(si::meter*si::kelvin),  27.1*si::milliwatt/(si::meter*si::kelvin),   47.1*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 1.24e-5 * si::pascal * si::second,                               // engineering toolbox, at 100 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   300.0*si::kelvin,                  400.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                   10.0*si::micropascal*si::second,   13.3*si::micropascal*si::second,   21.4*si::micropascal*si::second),
        /*density*/                0.6* si::kilogram/si::meter3,
        /*refractive_index*/       1.000261                                                         // engineering toolbox
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 4.1813 * si::joule / (si::gram * si::kelvin),                    // wikipedia
        /*thermal_conductivity*/   // 0.6062 * si::watt / (si::meter * si::kelvin), 
            field::get_exponential_temperature_function<si::thermal_conductivity>(     0.0*si::celcius,                       25.0*si::celcius,                        100.0*si::celcius,
                                                                                    0.5562*si::watt/(si::meter*si::kelvin), 0.6062*si::watt/(si::meter*si::kelvin),   0.6729*si::watt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   0.0  *si::celcius,                 25.0  *si::celcius,                 75.0  *si::celcius,              
                                                                                  1.793*si::millipascal*si::second,  0.890 *si::millipascal*si::second,  0.378 *si::millipascal*si::second),
        /*density*/                1022.0 * si::kilogram/si::meter3,                                // NOTE: we use the density of seawater, because do not model the effect of solute concentration on density, otherwise 997.0
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
            /*specific_heat_capacity*/            2.05 * si::joule / (si::gram * si::kelvin),       // wikipedia
            /*thermal_conductivity*/              2.09 * si::watt / (si::meter * si::kelvin),       // wikipedia
            /*dynamic_viscosity*/                 1e13 * si::poise,                                 // reference by Carey (1953)
            /*density*/                           0916.9 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    //138.268 * si::megapascal,
                field::get_exponential_temperature_function<si::pressure>(190.0*si::kelvin, 240.0*si::kelvin, 270.0*si::kelvin, 
                                                                          0.032*si::pascal, 27.28*si::pascal, 470.1*si::pascal),
            /*refractive_index*/                  1.3098,
            /*spectral_reflectance*/              0.9,

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



// nitrogen, N2
// for the atmosphere of Earth, and the surfaces of pluto or triton
PartlyKnownCompound nitrogen {
    /*molar_mass*/                        28.013  * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                357.8 * si::picometer,                                    // Mehio (2014)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.040,

    /*critical_point_pressure*/           3.39 * si::megapascal,
    /*critical_point_volume*/             90.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        126.21 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       198.8 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             0.71*si::kilojoule/(molar_mass*si::gram),
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
        /*specific_heat_capacity*/ 1.341 * si::joule / (si::gram * si::kelvin),
        /*thermal_conductivity*/   // 0.0234 * si::watt / (si::meter * si::kelvin),                    // wikipedia
            field::get_exponential_temperature_function<si::thermal_conductivity>( 100.0*si::kelvin,                           300.0*si::kelvin,                            600.0*si::kelvin,
                                                                                     9.8*si::milliwatt/(si::meter*si::kelvin),  26.0*si::milliwatt/(si::meter*si::kelvin),   44.0*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 1.76e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   200.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                   12.9*si::micropascal*si::second,   17.9*si::micropascal*si::second,   29.6*si::micropascal*si::second),
        /*density*/                4.622 * si::gram/si::liter,
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
        /*specific_heat_capacity*/ 2.042 * si::joule / (si::gram * si::kelvin),
        /*thermal_conductivity*/   0.1396 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
        /*dynamic_viscosity*/      157.9 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
        /*density*/                0.807 * si::gram/si::milliliter,
        /*vapor_pressure*/         
            field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                return (3.720822 - 293.94358*si::kelvin/T + 10.31993/si::kelvin*T) * si::standard_pressure;
            }), // Solomon (1950)
        /*refractive_index*/       1.19876
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
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>( -236.0*si::celcius, -226.8*si::celcius, -211.1*si::celcius, 
                                                                              1.0*si::pascal,   100.0*si::pascal,    10.0*si::kilopascal),
            /*refractive_index*/                  1.25,                                             // wikipedia
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
    /*molecular_diameter*/                334.0 * si::picometer,                                    // Mehio (2014)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.022,

    /*critical_point_pressure*/           5.043 * si::megapascal,
    /*critical_point_volume*/             73.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        154.59 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       213.1 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             0.44*si::kilojoule/(molar_mass*si::gram),
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
        /*specific_heat_capacity*/ 0.980 * si::joule / (si::gram * si::kelvin),                     
        /*thermal_conductivity*/   // 0.0238 * si::watt / (si::meter * si::kelvin),                    // wikipedia
            field::get_exponential_temperature_function<si::thermal_conductivity>( 100.0*si::kelvin,                           300.0*si::kelvin,                            600.0*si::kelvin,
                                                                                     9.3*si::milliwatt/(si::meter*si::kelvin),  26.3*si::milliwatt/(si::meter*si::kelvin),   48.1*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 2.04e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   100.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                    7.5*si::micropascal*si::second,   20.8*si::micropascal*si::second,   35.1*si::micropascal*si::second),
        /*density*/                4.467* si::gram/si::liter,
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
        /*specific_heat_capacity*/ 1.699 * si::kilojoule / (si::gram * si::kelvin),             
        /*thermal_conductivity*/   0.1514 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
        /*dynamic_viscosity*/      188.0 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
        /*density*/                1.141 * si::gram/si::milliliter,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-211.9*si::celcius, -200.5*si::celcius,    -183.1*si::celcius, 
                                                                         1.0*si::kilopascal,    10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.2243
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            11.06 * si::calorie / (31.9988*si::gram * si::kelvin), // Johnson (1960), 10.73 for solid II, 4.4 for solid III
            /*thermal_conductivity*/              0.17 * si::watt / (si::centimeter * si::kelvin),  // Jezowski (1993)
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
    /*molecular_diameter*/                346.9 * si::picometer,                                    // Mehio (2014)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.228,

    /*critical_point_pressure*/           7.375 * si::megapascal,
    /*critical_point_volume*/             94.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        304.13 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       205.0 * si::joule / si::gram,
    /*latent_heat_of_fusion*/             9.02*si::kilojoule/(molar_mass*si::gram),
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
        /*specific_heat_capacity*/ 36.61 * si::joule / (44.01 * si::gram * si::kelvin),             // wikipedia
        /*thermal_conductivity*/   // 0.01662 * si::watt / ( si::meter * si::kelvin ),                 // wikipedia
            field::get_exponential_temperature_function<si::thermal_conductivity>( 200.0*si::kelvin,                           300.0*si::kelvin,                            600.0*si::kelvin,
                                                                                     9.6*si::milliwatt/(si::meter*si::kelvin),  16.8*si::milliwatt/(si::meter*si::kelvin),   41.6*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 1.47e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   200.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                   10.0*si::micropascal*si::second,   15.0*si::micropascal*si::second,   28.0*si::micropascal*si::second),
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
        /*thermal_conductivity*/   0.087 * si::watt / (si::meter * si::kelvin),                     // wikipedia
        /*dynamic_viscosity*/      0.0712 * si::millipascal*si::second,                             // wikipedia data page
        /*density*/                1101.0 * si::kilogram/si::meter3,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-159.1*si::celcius, -121.6*si::celcius,     -78.6*si::celcius, 
                                                                            1.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.6630
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            47.11 * si::joule / (44.01 * si::gram * si::kelvin), // wikipedia
            // .thermal_conductivity = 720.0 * si::watt / (si::meter * si::kelvin),                 // Sumarakov (2003), peak conductivity, unusual for its variance
            /*thermal_conductivity*/              0.6 * si::watt / (si::meter * si::kelvin),        // Melosh (2011)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           1562.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>(130.0*si::kelvin,     170.0*si::kelvin,     205.0*si::kelvin,     
                                                                          0.032*si::kilopascal,  9.98*si::kilopascal, 227.1*si::kilopascal),
            /*refractive_index*/                  1.4,                                              // Warren (1986)
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
    /*molecular_diameter*/                404.6 * si::picometer,                                    // Mehio (2014)
    /*molecular_degrees_of_freedom*/      5.1,
    /*acentric_factor*/                   0.011,

    /*critical_point_pressure*/           4.60 * si::megapascal,
    /*critical_point_volume*/             99.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        190.56 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       510.83 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             0.94*si::kilojoule/(molar_mass*si::gram),
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
        /*specific_heat_capacity*/ 2.218 * si::joule / (si::gram * si::kelvin),                     
        /*thermal_conductivity*/   // 34.4 * si::milliwatt / ( si::meter * si::kelvin ),               // Huber & Harvey
            field::get_exponential_temperature_function<si::thermal_conductivity>( 200.0*si::kelvin,                           300.0*si::kelvin,                            600.0*si::kelvin,
                                                                                    22.5*si::milliwatt/(si::meter*si::kelvin),  34.1*si::milliwatt/(si::meter*si::kelvin),   84.1*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 1.10e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   200.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                    7.7*si::micropascal*si::second,   11.2*si::micropascal*si::second,   19.4*si::micropascal*si::second),
        /*density*/                1.816* si::gram/si::liter,
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
        /*specific_heat_capacity*/ 3.481 * si::kilojoule / (si::gram * si::kelvin),              
        /*thermal_conductivity*/   0.1931 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
        /*dynamic_viscosity*/      118.6 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
        /*density*/                0.4224 * si::gram/si::milliliter,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-197.0*si::celcius, -183.6*si::celcius,    -161.7*si::celcius, 
                                                                         1.0*si::kilopascal,    10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.2730, 
    },


    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            5.193 * si::calorie / (16.043*si::gram * si::kelvin), // Johnson (1960)
            /*thermal_conductivity*/              0.010 * si::watt / (si::centimeter * si::kelvin), // Jezowski (1997)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           0522.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>( 65.0*si::kelvin,      75.0*si::kelvin,      85.0*si::kelvin,     
                                                                           0.1 *si::kilopascal,  0.8 *si::kilopascal,  4.9 *si::kilopascal),
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

            /*chemical_susceptibility_estimate*/  field::missing()
        }
    }
};




// argon, Ar
// for the atmospheres of earth like planets
PartlyKnownCompound argon {
    /*molar_mass*/                        39.948 * si::gram/si::mole,
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                340.0 * si::picometer,                                    // wikipedia, Breck (1974)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.0,

    /*critical_point_pressure*/           4.898* si::megapascal,
    /*critical_point_volume*/             75.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        150.87 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       161.0 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             5.66*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             68.95 * si::kilopascal,
    /*triple_point_temperature*/          83.8058 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -189.36*si::celcius,
    /*simon_glatzel_slope*/               2114e5,
    /*simon_glatzel_exponent*/            1.593,
    
    /*molecular_absorption_cross_section*/ 1e-35*si::meter2,

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 0.570 * si::joule / (si::gram * si::kelvin),                    
        /*thermal_conductivity*/   // 0.016 * si::watt / ( si::meter * si::kelvin ),                   // wikipedia
            field::get_exponential_temperature_function<si::thermal_conductivity>( 100.0*si::kelvin,                           300.0*si::kelvin,                            600.0*si::kelvin,
                                                                                     6.2*si::milliwatt/(si::meter*si::kelvin),  17.9*si::milliwatt/(si::meter*si::kelvin),   30.6*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 2.23e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   100.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                    8.0*si::micropascal*si::second,   22.9*si::micropascal*si::second,   39.0*si::micropascal*si::second),
        /*density*/                5.79 * si::gram/si::liter,
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
        /*specific_heat_capacity*/ 1.078 * si::kilojoule / (si::gram * si::kelvin),              
        /*thermal_conductivity*/   0.1232 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
        /*dynamic_viscosity*/      252.1 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
        /*density*/                1.396 * si::gram/si::milliliter,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-212.4*si::celcius, -201.7*si::celcius,    -186.0*si::celcius, 
                                                                         1000.0 *si::pascal,    10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.23
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            0.197 * si::calorie / (si::gram * si::kelvin),    // Johnson (1960)
            /*thermal_conductivity*/              0.045 * si::watt / (si::centimeter * si::kelvin), // White (1956)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           1680.7 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>(55.0*si::kelvin,    65.0*si::kelvin,    75.0*si::kelvin, 
                                                                          0.2*si::kilopascal, 2.8*si::kilopascal, 18.7*si::kilopascal),
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



// helium, He
// for the atmospheres of gas giants
PartlyKnownCompound helium {
    /*molar_mass*/                        4.0026 * si::gram/si::mole,
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                255.7 * si::picometer, // Mehio (2014)
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   -0.39,

    /*critical_point_pressure*/           0.227 * si::megapascal,
    /*critical_point_volume*/             57.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        5.19 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       20.7 * si::joule / si::gram,
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             5.048e3 * si::pascal,
    /*triple_point_temperature*/          2.1768 * si::kelvin,
    /*freezing_point_sample_pressure*/    25.0 * si::standard_pressure,
    /*freezing_point_sample_temperature*/ 0.92778 * si::kelvin,
    /*simon_glatzel_slope*/               50.96e5,
    /*simon_glatzel_exponent*/            1.5602,

    /*molecular_absorption_cross_section*/ 1e-35* si::meter2,

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 9.78 * si::joule / (si::gram * si::kelvin), 
        /*thermal_conductivity*/   // 155.7 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            field::get_exponential_temperature_function<si::thermal_conductivity>( 100.0*si::kelvin,                           300.0*si::kelvin,                            600.0*si::kelvin,
                                                                                    75.5*si::milliwatt/(si::meter*si::kelvin), 156.7*si::milliwatt/(si::meter*si::kelvin),  252.4*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 1.96e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   100.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                    9.7*si::micropascal*si::second,   20.0*si::micropascal*si::second,   32.3*si::micropascal*si::second),
        /*density*/                16.89 * si::gram/si::liter,
        /*refractive_index*/       //1.000036,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            double invl2 = 1.0/(l*l);
            return 1.0
                + 0.014755297f/(426.29740f  - invl2);
        }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 4.545 * si::kilojoule / (si::gram * si::kelvin), 
        /*thermal_conductivity*/   0.0272 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
        /*dynamic_viscosity*/      3.57 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
        /*density*/                0.124901* si::gram/si::milliliter,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(    2.2*si::kelvin,     3.6*si::kelvin,        5.0*si::kelvin,  
                                                                          5.3*si::kilopascal,    52.9*si::kilopascal,  195.4*si::kilopascal),
        /*refractive_index*/       1.02451
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              0.1 * si::watt / (si::centimeter * si::kelvin), // Webb (1952)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           0187.0 * si::kilogram/si::meter3,
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

// hydrogen, H2
// for the atmospheres of gas giants
PartlyKnownCompound hydrogen {
    /*molar_mass*/                        2.016   * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                287.7 * si::picometer, // wikipedia,  Ismail (2015)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   -0.215,

    /*critical_point_pressure*/           1.293 *  si::megapascal,
    /*critical_point_volume*/             65.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        32.97 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       445.0 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             0.12*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             7.042 * si::kilopascal,
    /*triple_point_temperature*/          13.8 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -259.198*si::celcius,
    /*simon_glatzel_slope*/               274.22e5,
    /*simon_glatzel_exponent*/            1.74407,

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{   5e6, 8.06e6, 8.48e6, 9.97e6, 1.05e7, 1.13e7, 1.41e7, 2.66e7, 5.74e7  },
            std::vector<double>{ -29.5, -23.05, -22.54, -20.41, -20.44, -20.79, -20.97, -21.66, -22.68  },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 12.24 * si::joule / (si::gram * si::kelvin), 
        /*thermal_conductivity*/   // 186.6 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            field::get_exponential_temperature_function<si::thermal_conductivity>( 100.0*si::kelvin,                           300.0*si::kelvin,                            400.0*si::kelvin,
                                                                                    68.6*si::milliwatt/(si::meter*si::kelvin), 186.9*si::milliwatt/(si::meter*si::kelvin),  230.4*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 0.88e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   100.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                    4.2*si::micropascal*si::second,    9.0*si::micropascal*si::second,   14.4*si::micropascal*si::second),
        /*density*/                1.3390 * si::gram/si::liter,
        /*refractive_index*/       // 1.0001392,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            constexpr double n = 1.00014930f;
            constexpr double dndl = -0.000082645f;
            return n + dndl * l;
        }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 9.668 * si::kilojoule / (si::gram * si::kelvin), 
        /*thermal_conductivity*/   0.1185 * si::watt / (si::meter * si::kelvin), // Timmerhaus (1989)
        /*dynamic_viscosity*/      13.06 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
        /*density*/                0.0708 * si::gram/si::milliliter,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(   14.0*si::kelvin,    23.0*si::kelvin,       32.0*si::kelvin,  
                                                                          7.9*si::kilopascal,   209.0*si::kilopascal, 1119.0*si::kilopascal),
        /*refractive_index*/       1.1096
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            0.2550 * si::calorie / (si::gram * si::kelvin), // Johnson (1960)
            /*thermal_conductivity*/              1.819 * si::watt / ( si::meter * si::kelvin ), // wikipedia
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           0086.0 * si::kilogram/si::meter3, // https://en.wikipedia.org/wiki/Solid_hydrogen
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

// ammonia, NH3
// for the atmosphere of Titan
PartlyKnownCompound ammonia {

    /*molar_mass*/                        17.031 * si::gram/si::mole,
    /*atoms_per_molecule*/                4u,
    /*molecular_diameter*/                260.0 * si::picometer, // Breck (1974)
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.252,

    /*critical_point_pressure*/           11.357*si::megapascal,
    /*critical_point_volume*/             69.8*si::centimeter3/si::mole,
    /*critical_point_temperature*/        405.56 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       23.33*si::kilojoule/(17.031*si::gram),
    /*latent_heat_of_fusion*/             5.66*si::kilojoule/(17.031*si::gram),
    /*triple_point_pressure*/             6.060e3 * si::pascal,
    /*triple_point_temperature*/          195.30 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 77.65*si::celcius,
    /*simon_glatzel_slope*/               5270e5,
    /*simon_glatzel_exponent*/            4.3,

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{  4.35e6, 4.84e6, 5.26e6, 6.14e6, 6.61e6, 7.28e6, 8.03e6, 9.32e6, 1.06e7, 1.23e7, 2.52e7, 5.80e7, 1.93e8f  },
            std::vector<double>{  -26.43, -21.51, -21.17, -21.60, -21.58, -21.00, -20.92, -20.56, -20.76, -20.44, -20.78, -21.60, -22.61f  },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 35.06 * si::joule / (17.031 * si::gram * si::kelvin), // wikipedia data page
        /*thermal_conductivity*/   // 25.1 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            field::get_exponential_temperature_function<si::thermal_conductivity>( 300.0*si::kelvin,                           400.0*si::kelvin,                            600.0*si::kelvin,
                                                                                    24.4*si::milliwatt/(si::meter*si::kelvin),  37.4*si::milliwatt/(si::meter*si::kelvin),   66.8*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      0.99e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
        /*density*/                field::missing(),
        /*refractive_index*/       //1.000376,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            double invl2 = 1.0/(l*l);
            return 1.0
                + 0.032953f/(90.392f  - invl2);
        }) 
    },
    
    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 4.700 * si::joule / (si::gram * si::kelvin), // wikipedia
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                681.97 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-139.0*si::celcius,  -71.3*si::celcius,     -36.6*si::celcius, 
                                                                          1.0*si::kilopascal,   10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.3944,
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>(160.0*si::kelvin,     180.0*si::kelvin,     190.0*si::kelvin,     
                                                                          0.1  *si::kilopascal, 1.2  *si::kilopascal, 3.5  *si::kilopascal),
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

// ozone, O3
// for modeling the ozone layer of earth like planets
PartlyKnownCompound ozone {
    /*molar_mass*/                        47.998 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                335.0 * si::picometer, // Streng (1961)
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.227,

    /*critical_point_pressure*/           5.57 *  si::megapascal,
    /*critical_point_volume*/             89.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        261.1 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       288.49 * si::kilojoule / si::kilogram,
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             7.346e-6 * si::bar, //encyclopedia.airliquide.com
    /*triple_point_temperature*/          -193.0 * si::celcius, //encyclopedia.airliquide.com
    /*freezing_point_sample_pressure*/    si::standard_pressure, 
    /*freezing_point_sample_temperature*/ -193.0*si::celcius,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{   0.0,  2e5,  7e5,  9e5,1.6e6,  2e6,2.5e6,2.8e6,  3e6,3.5e6,4.6e6,  6e6,7.7e6,1.2e7f },
            std::vector<double>{ -28.0,-26.0,-31.0,-28.0,-24.0,-25.0,-27.0,-24.5,-23.0,-21.0,-22.5,-22.0,-21.0,-21.0  },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       1.00052
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                1349.0 * si::kilogram / si::meter3, //encyclopedia.airliquide.com
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-189.0*si::celcius, -158.0*si::celcius,    -111.5*si::celcius, 
                                                                            1.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.2226
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              5.21e-4 * si::calorie / (si::second*si::centimeter2*si::kelvin/si::centimeter), // Streng (1961)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           1354.0  * si::kilogram / si::meter3, //https://www.aqua-calc.com/page/density-table/substance/solid-blank-ozone
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

// LESS CHARACTERIZED COMPOUNDS:
// nitrous oxide, N2O
// representative of industrial emissions
PartlyKnownCompound nitrous_oxide {
    /*molar_mass*/                        44.012 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                330.0 * si::picometer, // wikipedia, Matteucci
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.142,

    /*critical_point_pressure*/           7.245 *  si::megapascal,
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        309.52 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       374.286 * si::kilojoule / si::kilogram, //encyclopedia.airliquide.com
    /*latent_heat_of_fusion*/             6.54*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             8.785e-1 * si::bar, //encyclopedia.airliquide.com
    /*triple_point_temperature*/          -90.82 * si::celcius, //encyclopedia.airliquide.com
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -90.8*si::celcius,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{  3.16e6, 3.29e6, 3.85e6, 4.15e6, 5.10e6, 5.62e6, 5.95e6, 6.29e6, 6.33e6, 6.66e6, 6.91e6, 7.25e6, 7.31e6, 7.73e6, 8.00e6, 9.68e6, 1.07e7, 1.32e7, 1.54e7, 2.82e7, 4.30e7, 7.11e7, 2.34e8  },
            std::vector<double>{  -28.02, -27.33, -27.38, -26.88, -23.17, -22.83, -23.13, -23.43, -23.43, -21.58, -21.14, -20.67, -22.14, -20.17, -20.53, -20.62, -20.75, -20.37, -20.40, -20.57, -20.85, -21.29, -22.43  },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   // 17.4 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
            field::get_exponential_temperature_function<si::thermal_conductivity>( 200.0*si::kelvin,                           300.0*si::kelvin,                            600.0*si::kelvin,
                                                                                     9.8*si::milliwatt/(si::meter*si::kelvin),  17.4*si::milliwatt/(si::meter*si::kelvin),   41.8*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 1.47e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   200.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                   10.0*si::micropascal*si::second,   15.0*si::micropascal*si::second,   27.4*si::micropascal*si::second),
        /*density*/                field::missing(),
        /*refractive_index*/       1.000516
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                1230.458 * si::kilogram / si::meter3, 
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-131.1*si::celcius, -112.9*si::celcius,     -88.7*si::celcius, 
                                                                         1.0*si::kilopascal,    10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.238
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
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


// sulfur dioxide, SO2
// representative of industrial emissions
PartlyKnownCompound  sulfur_dioxide {
    /*molar_mass*/                        64.064 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                360.0 * si::picometer, // wikipedia, Breck (1974)
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.245,

    /*critical_point_pressure*/           7.884 * si::megapascal,
    /*critical_point_volume*/             122.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        430.64 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       24.94 *si::kilojoule/(64.064*si::gram),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             1.67e3 * si::pascal,
    /*triple_point_temperature*/          197.69 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -75.45 * si::celcius,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{2.47e6,2.69e6,2.91e6,3.12e6,3.41e6,3.76e6,4.25e6,4.37e6,4.95e6,5.45e6,6.01e6,6.63e6,7.36e6,7.95e6,8.51e6,8.80e6,9.07e6,9.35e6},
            std::vector<double>{-28.29,-25.86,-25.90,-23.18,-22.04,-22.33,-23.69,-22.55,-20.88,-21.43,-22.48,-21.25,-21.45,-19.92,-21.12,-20.35,-20.88,-20.68},
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   // 9.6 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            field::get_exponential_temperature_function<si::thermal_conductivity>( 300.0*si::kelvin,                           400.0*si::kelvin,                            600.0*si::kelvin,
                                                                                     9.6*si::milliwatt/(si::meter*si::kelvin),  14.3*si::milliwatt/(si::meter*si::kelvin),   25.6*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 1.26e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   200.0*si::kelvin,                  300.0*si::kelvin,                  500.0*si::kelvin,              
                                                                                    8.6*si::micropascal*si::second,   12.9*si::micropascal*si::second,   21.7*si::micropascal*si::second),
        /*density*/                field::missing(),
        /*refractive_index*/       1.000686
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      0.368 * si::millipascal*si::second, // pubchem
        /*density*/                389.06 * si::kilogram / si::meter3, // encyclopedia.airliquide.com
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>( -80.0*si::celcius,  -52.0*si::celcius,     -10.3*si::celcius, 
                                                                         1.0*si::kilopascal,    10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.3396
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
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


// nitric oxide, NO
// representative of industrial emissions
    PartlyKnownCompound nitric_oxide {
    /*molar_mass*/                        30.006 * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                317.0 * si::picometer, // wikipedia, Matteucci
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.585,

    /*critical_point_pressure*/           6.48 *  si::megapascal,
    /*critical_point_volume*/             58.0 *  si::centimeter3/si::mole,
    /*critical_point_temperature*/        180.0 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       13.83 * si::kilojoule/(30.006*si::gram),
    /*latent_heat_of_fusion*/             2.3*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             87.85e3 * si::pascal,
    /*triple_point_temperature*/          182.34 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -163.6*si::celcius,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{4.82e6, 5.61e6, 5.83e6, 6.55e6, 7.23e6, 7.65e6, 8.31e6, 9.94e6, 1.11e7, 1.26e7, 1.67e7, 4.05e7, 7.18e7, 1.85e8},
            std::vector<double>{-22.20, -21.43, -21.37, -21.56, -21.49, -21.56, -21.57, -20.71, -20.41, -20.71, -20.60, -20.94, -21.42, -22.55}, 
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   // 25.9 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
            field::get_exponential_temperature_function<si::thermal_conductivity>( 200.0*si::kelvin,                           300.0*si::kelvin,                            600.0*si::kelvin,
                                                                                    17.8*si::milliwatt/(si::meter*si::kelvin),  25.9*si::milliwatt/(si::meter*si::kelvin),   46.2*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 0.0188 * si::millipascal * si::second, //pubchem
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   200.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                   13.8*si::micropascal*si::second,   19.2*si::micropascal*si::second,   31.9*si::micropascal*si::second),
        /*density*/                field::missing(),
        /*refractive_index*/       1.000297
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                1230.458 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-179.3*si::celcius, -168.1*si::celcius,    -159.9*si::celcius, 
                                                                          1.0*si::kilopascal,   10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.330
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>( 85.0*si::kelvin,      95.0*si::kelvin,     105.0*si::kelvin,     
                                                                           0.1 *si::kilopascal,  1.3 *si::kilopascal, 10.0 *si::kilopascal),
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

// carbon monoxide, CO
// for the surface of pluto
PartlyKnownCompound carbon_monoxide {
    /*molar_mass*/                        28.010 * si::gram/si::mole, 
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                357.0 * si::picometer, // Mehio (2014)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.066,

    /*critical_point_pressure*/           3.494 *  si::megapascal,
    /*critical_point_volume*/             93.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        132.86 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       6.04 * si::kilojoule/(28.010*si::gram),
    /*latent_heat_of_fusion*/             0.833*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             1.53e-1 * si::bar, //encyclopedia.airliquide.com
    /*triple_point_temperature*/          -204.99*si::celcius,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -205.02*si::celcius,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{  4.83e6, 4.85e6, 4.88e6, 5.00e6, 5.02e6, 5.05e6, 5.17e6, 5.19e6, 5.22e6, 5.33e6, 5.36e6, 5.38e6, 5.49e6, 5.51e6, 5.55e6, 5.64e6, 5.67e6, 5.68e6, 5.71e6, 6.02e6, 6.85e6, 7.98e6, 8.42e6, 9.28e6, 1.00e7, 1.05e7, 1.13e7, 1.21e7, 1.38e7, 2.10e7, 4.54e7, 5.15e8 },
            std::vector<double>{  -28.38, -24.93, -28.40, -28.39, -24.91, -28.40, -28.39, -25.16, -28.42, -28.39, -25.52, -28.39, -28.38, -25.72, -28.41, -28.40, -25.96, -23.41, -28.42, -22.47, -20.89, -21.48, -22.01, -20.72, -20.93, -20.48, -20.35, -20.56, -20.56, -20.68, -21.04, -22.55 },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 29.0 * si::joule / (28.010 * si::gram * si::kelvin), // wikipedia data page
        /*thermal_conductivity*/   // 25.0 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            field::get_exponential_temperature_function<si::thermal_conductivity>( 300.0*si::kelvin,                           400.0*si::kelvin,                            600.0*si::kelvin,
                                                                                    25.0*si::milliwatt/(si::meter*si::kelvin),  32.3*si::milliwatt/(si::meter*si::kelvin),   45.7*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 1.74e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   100.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                    6.7*si::micropascal*si::second,   17.8*si::micropascal*si::second,   29.1*si::micropascal*si::second),
        /*density*/                field::missing(),
        /*refractive_index*/       // 1.00036320, //https://refractiveindex.info
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            constexpr double n = 1.00036350f;
            constexpr double dndl = -0.00027275f;
            return n + dndl * l;
        }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 60.351 * si::joule / (28.010 * si::gram * si::kelvin), // pubchem
        /*thermal_conductivity*/   0.1428 * si::watt / (si::meter * si::kelvin), // pubchem
        /*dynamic_viscosity*/      0.170 * si::millipascal * si::second, // Johnson (1960)
        /*density*/                793.2 * si::kilogram / si::meter3,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-223.0*si::celcius, -216.5*si::celcius,    -191.7*si::celcius, 
                                                                          100.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       //1.33336f,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            return math::lerp(
                    std::vector<double>{},
                    std::vector<double>{},
                    log10(l)
                );
        }) 
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            12.29 * si::calorie / (28.010 * si::gram * si::kelvin), // Johnson (1960)
            /*thermal_conductivity*/              30.0 * si::milliwatt / (si::centimeter * si::kelvin), // Stachowiak (1998)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           1.0288 * si::kilogram / si::meter3, // Johnson (1960), for alpha, 0.929 for beta
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>( 50.0*si::kelvin,      55.0*si::kelvin,      65.0*si::kelvin,     
                                                                           0.1 *si::kilopascal,  0.6 *si::kilopascal,  8.2 *si::kilopascal),
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


// ethane, C2H6
// for the lakes of Titan
PartlyKnownCompound ethane {
    /*molar_mass*/                        30.070 * si::gram/si::mole,
    /*atoms_per_molecule*/                8u,
    /*molecular_diameter*/                443.0 * si::picometer,  // Aguado (2012)
    /*molecular_degrees_of_freedom*/      6.8,
    /*acentric_factor*/                   0.099,

    /*critical_point_pressure*/           4.88 * si::megapascal,
    /*critical_point_volume*/             146.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        305.36 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       14.69 * si::kilojoule/(30.070*si::gram),
    /*latent_heat_of_fusion*/             2.72*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             1.4 * si::pascal,
    /*triple_point_temperature*/          90.35 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -183.79 * si::celcius, 
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{ 5.6e6, 6.25e6, 6.73e6, 7.49e6, 8.23e6, 9.26e6, 1.01e7, 1.14e7, 2.42e7, 5.43e7, 1.72e8 },
            std::vector<double>{ -35.0, -25.67, -21.86, -20.50, -20.60, -20.27, -20.25, -20.08, -20.57, -21.44, -22.57 },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 52.49 * si::joule / (30.070 * si::gram * si::kelvin), // wikipedia data page
        /*thermal_conductivity*/   // 21.2 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            field::get_exponential_temperature_function<si::thermal_conductivity>( 200.0*si::kelvin,                           300.0*si::kelvin,                            600.0*si::kelvin,
                                                                                    11.0*si::milliwatt/(si::meter*si::kelvin),  21.3*si::milliwatt/(si::meter*si::kelvin),   70.5*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 9.4 * si::micropascal*si::second,
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   200.0*si::kelvin,                  300.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                    6.4*si::micropascal*si::second,    9.5*si::micropascal*si::second,   17.3*si::micropascal*si::second),
        /*density*/                field::missing(),
        /*refractive_index*/       // 1.0377,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            double invl2 = 1.0/(l*l);
            return 1.0007330f
                + 6.822764e-6 * invl2;
        }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 68.5 * si::joule / (30.070 * si::gram * si::kelvin), // wikipedia data page
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                545.0 * si::kilogram/si::meter3,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-183.3*si::celcius, -145.3*si::celcius,     -88.8*si::celcius, 
                                                                            1.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       //1.33336f,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            return math::lerp(
                    std::vector<double>{},
                    std::vector<double>{},
                    log10(l)
                );
        }) 
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
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


// hydrogen cyanide, HCN
// for small bodies in the outer solar system and interstellar space
// one of the most abundant compounds following from elemental abundances in the universe
PartlyKnownCompound hydrogen_cyanide {
    /*molar_mass*/                        27.026 * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                376.0 * si::picometer, // wikipedia, Matteucci
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.407,

    /*critical_point_pressure*/           5400e3 * si::pascal,
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        456.65 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       25.2 * si::kilojoule/(27.026*si::gram), // pubchem
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             0.153e3 * si::pascal,
    /*triple_point_temperature*/          259.7 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -13.29 * si::celcius,
    /*simon_glatzel_slope*/               3080e5,
    /*simon_glatzel_exponent*/            3.6,

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{  8.50e6, 8.58e6, 9.19e6, 9.59e6, 9.89e6, 9.99e6, 1.01e7, 1.02e7, 1.03e7, 1.05e7, 1.07e7, 1.09e7, 1.26e7, 1.35e7, 1.41e7, 1.46e7, 1.62e7 },
            std::vector<double>{   -24.2,  -21.8,  -21.5,  -22.1,  -20.3,  -20.6,  -20.3,  -20.7,  -20.2,  -20.2,  -20.6,  -20.3,  -20.5,  -20.3,  -20.4,  -20.4,  -20.5 },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      
            field::get_linear_temperature_function<si::dynamic_viscosity>(   0.0  *si::celcius,                 25.0  *si::celcius,              
                                                                             0.235*si::millipascal*si::second,  0.183 *si::millipascal*si::second),
        /*density*/                687.6 * si::kilogram/si::meter3,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>( -52.6*si::celcius,  -22.7*si::celcius,      25.4*si::celcius, 
                                                                         1.0*si::kilopascal,    10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.2614
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>(200.0*si::kelvin,     230.0*si::kelvin,     250.0*si::kelvin,     
                                                                          0.2  *si::kilopascal, 2.2  *si::kilopascal, 9.7  *si::kilopascal),
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


// ethanol, C2H6O
// for small bodies in the outer solar system and interstellar space
// one of the most abundant compounds following from elemental abundances in the universe
PartlyKnownCompound ethanol {
    /*molar_mass*/                        46.068 * si::gram/si::mole,
    /*atoms_per_molecule*/                9u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.637,

    /*critical_point_pressure*/           6.25 * si::megapascal,
    /*critical_point_volume*/             169.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        351.44 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       42.32 * si::kilojoule/(46.068*si::gram), 
    /*latent_heat_of_fusion*/             4.931*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             0.00043 * si::pascal,  // wikipedia data page
    /*triple_point_temperature*/          150.0 * si::kelvin,  // wikipedia data page
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -114.14 * si::celcius,
    /*simon_glatzel_slope*/               10600e5,
    /*simon_glatzel_exponent*/            1.61,

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 78.28 * si::joule / (46.068*si::gram*si::kelvin), // wikipedia data page
        /*thermal_conductivity*/   // 14.4 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            field::get_exponential_temperature_function<si::thermal_conductivity>( 300.0*si::kelvin,                           300.0*si::kelvin,                            600.0*si::kelvin,
                                                                                    14.4*si::milliwatt/(si::meter*si::kelvin),  25.8*si::milliwatt/(si::meter*si::kelvin),   53.2*si::milliwatt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      
            field::get_exponential_temperature_function<si::dynamic_viscosity>(   400.0*si::kelvin,                  500.0*si::kelvin,                  600.0*si::kelvin,              
                                                                                   11.6*si::micropascal*si::second,   14.5*si::micropascal*si::second,   17.0*si::micropascal*si::second),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 112.4 * si::joule / (46.068*si::gram*si::kelvin), // wikipedia data page
        /*thermal_conductivity*/   // 0.167 * si::watt / ( si::meter * si::kelvin ),
            field::get_exponential_temperature_function<si::thermal_conductivity>(   -25.0*si::celcius,                        0.0*si::celcius,                        100.0*si::celcius,
                                                                                    0.181 *si::watt/(si::meter*si::kelvin), 0.1742*si::watt/(si::meter*si::kelvin),   0.148 *si::watt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      
            field::get_exponential_temperature_function<si::dynamic_viscosity>( -25.0  *si::celcius,                 25.0  *si::celcius,                 75.0  *si::celcius,              
                                                                                  3.262*si::millipascal*si::second,  1.074 *si::millipascal*si::second,  0.476 *si::millipascal*si::second),
        /*density*/                0789.3 * si::kilogram/si::meter3,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>( -73.0*si::celcius,   -7.0*si::celcius,      78.0*si::celcius, 
                                                                            1.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       //1.361,  // wikipedia data page
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            double invl2 = 1.0/(l*l);
            return 1.35265f
                + 0.00306 * invl2
                + 0.00002 * invl2*invl2;
        })
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid> {
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            111.46 * si::joule / (46.068*si::gram*si::kelvin), // wikipedia data page
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
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

// formaldehyde, CH2O
// for small bodies in the outer solar system and interstellar space
// one of the most abundant compounds following from elemental abundances in the universe
PartlyKnownCompound formaldehyde {
    /*molar_mass*/                        30.026 * si::gram/si::mole, // wikipedia
    /*atoms_per_molecule*/                4u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.282,

    /*critical_point_pressure*/           6788e3 * si::pascal,
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        410.3 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       23.3 * si::kilojoule/(30.026*si::gram),//pubchem
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             71549032.0 * si::pascal,
    /*triple_point_temperature*/          155.10 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 181.0 * si::kelvin,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{  2.51e6, 2.67e6, 2.70e6, 2.74e6, 2.83e6, 2.86e6, 2.95e6, 2.98e6, 3.06e6, 3.09e6, 3.18e6, 3.62e6, 4.02e6, 4.44e6 },
            std::vector<double>{   -26.5,  -26.1,  -25.2,  -26.1,  -23.6,  -25.4,  -23.2,  -25.0,  -23.1,  -24.5,  -23.2,  -23.6,  -24.5,  -25.7 },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      0.1421 * si::millipascal * si::second, //pubchem
        /*density*/                0.8153 * si::kilogram/si::meter3,  // wikipedia
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>( -91.0*si::celcius,  -61.7*si::celcius,     -19.3*si::celcius, 
                                                                         1.0*si::kilopascal,    10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.3714  // wikipedia
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
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


// formic acid, CH2O2
// for small bodies in the outer solar system and interstellar space
// one of the most abundant compounds following from elemental abundances in the universe
PartlyKnownCompound formic_acid {
    /*molar_mass*/                        46.026 * si::gram/si::mole,
    /*atoms_per_molecule*/                5u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.473,

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             115.9*9*si::centimeter3/si::mole,
    /*critical_point_temperature*/        588.0 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       20.10 * si::kilojoule/(46.026*si::gram),
    /*latent_heat_of_fusion*/             12.68*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             2.2 * si::kilopascal,
    /*triple_point_temperature*/          281.4 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 8.3 * si::celcius,
    /*simon_glatzel_slope*/               4100e5,
    /*simon_glatzel_exponent*/            5.2,

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 45.68 * si::joule / (46.026*si::gram*si::kelvin), // wikipedia data page
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 101.3 * si::joule / (46.026*si::gram*si::kelvin), // wikipedia data page
        /*thermal_conductivity*/   // 0.267 * si::watt / (si::meter * si::kelvin),
            field::get_exponential_temperature_function<si::thermal_conductivity>(    25.0*si::celcius,                       50.0*si::celcius,                        100.0*si::celcius,
                                                                                    0.267 *si::watt/(si::meter*si::kelvin), 0.2652*si::watt/(si::meter*si::kelvin),   0.261 *si::watt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      1.607 * si::millipascal*si::second,
        /*density*/                1220.0 * si::kilogram/si::meter3,
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(  -0.8*si::celcius,   37.0*si::celcius,     100.2*si::celcius, 
                                                                         1.0*si::kilopascal,    10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       1.3714 
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            74.5 * si::joule / (46.026*si::gram*si::kelvin), // wikipedia data page
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>( -56.0*si::celcius,  -40.4*si::celcius,   -0.8*si::celcius, 
                                                                            1.0 *si::pascal,   100.0*si::pascal,     1.0*si::kilopascal),
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

// POORLY CHARACTERIZED COMPOUNDS:
// perflouromethane, tetraflouromethane, CF4
// for modeling industrial emissions and the terraforming of Mars as suggested by Zubrin (1996) 
// We went out of our way searching for an IR graph since 
// we use CF4 in the model to study pollution and Martian terraformation
PartlyKnownCompound perflouromethane{
    /*molar_mass*/                        88.0  * si::gram/si::mole,
    /*atoms_per_molecule*/                5u,
    /*molecular_diameter*/                470.0 * si::picometer, // Motkuri (2014)
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   0.186,

    /*critical_point_pressure*/           3.73 * si::megapascal,
    /*critical_point_volume*/             140.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        227.54 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       137000.0 * si::joule / si::kilogram,
    /*latent_heat_of_fusion*/             0.704*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             0.1012e3 * si::pascal,
    /*triple_point_temperature*/          89.54 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ -183.60*si::celcius,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{  1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 7.99e6, 8.62e6, 9.25e6, 1.00e7, 1.05e7, 1.08e7, 1.13e7, 1.20e7, 1.28e7, 1.33e7, 1.79e7, 2.14e7, 3.37e7, 5.79e7, 1.71e8 },
            std::vector<double>{  -20.19, -20.19, -19.97, -19.91, -19.92, -20.05, -20.10, -20.12, -20.17, -23.26, -23.10, -22.58, -21.35, -21.52, -20.18, -20.24, -21.06, -20.16, -20.43, -20.13, -20.31, -20.33, -20.68, -21.63 },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   16.0 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
        /*dynamic_viscosity*/      
            field::get_exponential_temperature_function<si::thermal_conductivity>( 300.0*si::kelvin,                           400.0*si::kelvin,                            600.0*si::kelvin,
                                                                                    16.0*si::milliwatt/(si::meter*si::kelvin),  24.1*si::milliwatt/(si::meter*si::kelvin),   39.9*si::milliwatt/(si::meter*si::kelvin)),
        /*density*/                field::missing(),
        /*refractive_index*/       1.0004823
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                1890.0 * si::kilogram/si::meter3, //pubchem
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(-171.6*si::celcius, -153.9*si::celcius,    -128.3*si::celcius, 
                                                                         1.0*si::kilopascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       //1.33336f,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            return math::lerp(
                    std::vector<double>{},
                    std::vector<double>{},
                    log10(l)
                );
        }) 
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           1980.0 * si::kilogram/si::meter3, // pubchem
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>(-199.9*si::celcius, -193.0*si::celcius, -183.9*si::celcius, 
                                                                            1.0 *si::pascal,    10.0*si::pascal,   100.0*si::pascal  ),
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

// benzene, C6H6
// representative of aromatic compounds, predominantly low-grade coal,
// and serves as a template for compounds in prebiotic chemistry such as nucleic acids or tholins
PartlyKnownCompound benzene {
    /*molar_mass*/                        79.102 * si::gram/si::mole, // wikipedia
    /*atoms_per_molecule*/                12u, // wikipedia
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0, // wikipedia
    /*acentric_factor*/                   0.211,

    /*critical_point_pressure*/           4.9 * si::megapascal,
    /*critical_point_volume*/             257.0*si::centimeter3/si::mole,
    /*critical_point_temperature*/        562.0 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       33.83 * si::kilojoule / (79.109 * si::gram), 
    /*latent_heat_of_fusion*/             9.87*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             4.83 * si::kilopascal, // wikipedia data page
    /*triple_point_temperature*/          278.5 * si::kelvin, // wikipedia data page
    /*freezing_point_sample_pressure*/    si::standard_pressure, 
    /*freezing_point_sample_temperature*/ 5.49 * si::celcius, 
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ 
    field::SpectralFunction<si::area>([](
        const si::wavenumber nlo, 
        const si::wavenumber nhi, 
        const si::pressure p, 
        const si::temperature T
    ) {
        return std::pow(10.0, math::integral_of_lerp(
            std::vector<double>{  3.63e6, 3.73e6, 3.75e6, 3.76e6, 3.82e6, 3.86e6, 3.87e6, 3.92e6, 3.95e6, 3.97e6, 4.03e6, 4.04e6, 4.07e6, 4.12e6, 4.14e6, 4.16e6, 4.21e6, 4.23e6, 4.25e6, 4.33e6, 4.52e6, 4.75e6, 4.91e6, 5.03e6, 5.33e6, 5.58e6, 5.85e6, 6.72e6, 7.54e6, 8.11e6, 8.79e6, 9.59e6, 1.03e7, 1.12e7, 1.41e7, 3.33e7, 2.11e8 },
            std::vector<double>{   -25.0,  -23.9,  -23.0,  -23.8,  -23.2,  -21.7,  -23.2,  -22.6,  -21.7,  -23.1,  -22.5,  -21.7,  -23.1,  -22.7,  -21.9,  -23.2,  -22.9,  -22.6,  -23.3,  -23.3,  -24.0,  -23.3,  -20.7,  -20.6,  -20.8,  -19.5,  -20.2,  -21.1,  -20.4,  -20.5,  -20.1,  -20.2,  -20.0,  -20.2,  -19.8,  -20.5,  -22.3 },
            (nlo*si::meter), (nhi*si::meter)
        ) / (nhi*si::meter - nlo*si::meter)) * si::meter2;
    }),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ 82.44 * si::joule / (79.109 * si::gram * si::kelvin),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      0.75e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ 134.8 * si::joule / (79.109 * si::gram * si::kelvin),
        /*thermal_conductivity*/   // 0.1411 * si::watt / ( si::meter * si::kelvin ),
            field::get_exponential_temperature_function<si::thermal_conductivity>(    25.0*si::celcius,                       50.0*si::celcius,                         75.0*si::celcius,
                                                                                    0.1411*si::watt/(si::meter*si::kelvin), 0.1329*si::watt/(si::meter*si::kelvin),   0.1247*si::watt/(si::meter*si::kelvin)),
        /*dynamic_viscosity*/      // 0.601 * si::millipascal * si::second, // engineering toolbox, at 300K
            field::get_exponential_temperature_function<si::dynamic_viscosity>(  25.0  *si::celcius,                 50.0  *si::celcius,                 75.0  *si::celcius,              
                                                                                  0.604*si::millipascal*si::second,  0.436 *si::millipascal*si::second,  0.335 *si::millipascal*si::second),
        /*density*/                0.8765 * si::gram/si::centimeter3, // wikipedia
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>( -15.1*si::celcius,   20.0*si::celcius,      79.7*si::celcius, 
                                                                         1.0*si::kilopascal,    10.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       //1.5011,
        field::SpectralFunction<double>([](
            const si::wavenumber nlo, 
            const si::wavenumber nhi, 
            const si::pressure p, 
            const si::temperature T
        ) {
            double l = (2.0 / (nhi+nlo) / si::micrometer);
            double l2 = l*l;
            double invl2 = 1.0/(l*l);
            return sqrt(
                2.170184597f
                + 0.00059399 * l2
                + 0.02303464 * invl2
                - 0.000499485 * invl2*invl2
                + 0.000178796 * invl2*invl2*invl2
            );
        }) 
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            118.4 * si::joule / (79.109 * si::gram * si::kelvin),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>( -40.0*si::celcius,  -15.1*si::celcius,      20.0*si::celcius, 
                                                                          100.0 *si::pascal,     1.0*si::kilopascal,   10.0*si::kilopascal),
            /*refractive_index*/                  field::missing(),
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   0.0614e11*si::pascal,
            /*shear_modulus*/                     0.0197e11*si::pascal,
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

// pyramidine, C4H4N2
// representative of prebiotic chemistry and tholins,
// since it's been observed from Kawai (2019) that tholins may contain them among other nucleotides
PartlyKnownCompound pyrimidine {
    /*molar_mass*/                        80.088 * si::gram/si::mole, // wikipedia
    /*atoms_per_molecule*/                10u, // wikipedia
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0, // wikipedia
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       49.79*79 * si::kilojoule / (80.088*si::gram),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    si::standard_pressure, 
    /*freezing_point_sample_temperature*/ -22.6 * si::celcius, // wikipedia
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),


    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                1.016 * si::gram/si::centimeter3, // wikipedia
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           field::missing(),
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

// halite, NaCl, sodium chloride, salt
// for salt bed flats and potentially modeling ocean salinity
PartlyKnownCompound  halite {
    /*molar_mass*/                        90.442*si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      field::missing(),
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           26e6 * si::pascal, // wikipedia data page
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        3900.0 * si::kelvin, // wikipedia data page
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             28.16*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             30.0 * si::pascal, // wikipedia data page
    /*triple_point_temperature*/          1074.0 * si::kelvin, // wikipedia data page
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 800.7*si::celcius,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),
    
    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      1270.15 * si::millipascal * si::second, // Dortmund data bank, 1270.15C
        /*density*/                1549.0 * si::kilogram/si::meter3,  // wikipedia data page
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>( 835.0*si::celcius,  987.0*si::celcius,    1461.0*si::celcius, 
                                                                          100.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       field::missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            50.0 * si::joule / (90.442 * si::gram * si::kelvin), // wikipedia data page
            /*thermal_conductivity*/              6.5 * si::watt / ( si::meter * si::kelvin ), // wikipedia data page
            /*dynamic_viscosity*/                 1e17 * si::poise, // various sources, Carey (1953) cites this number from Weinberg (1927), and Mukherjee (2010), provides a literature review and findings from salt diapirs. Science is weird.
            /*density*/                           2170.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>(  653.0*si::kelvin,   733.0*si::kelvin,   835.0*si::kelvin,  
                                                                              1.0*si::pascal,    10.0*si::pascal,   100.0*si::pascal ),
            /*refractive_index*/                  
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    double l = (2.0 / (nhi+nlo) / si::micrometer);
                    double l2 = l*l;
                    return sqrt(
                        1.0
                        + 0.00055f
                        + 0.19800 * l2 / (l2 - pow(0.050, 2.0))
                        + 0.48398 * l2 / (l2 - pow(0.100, 2.0))
                        + 0.38696 * l2 / (l2 - pow(0.128, 2.0))
                        + 0.25998 * l2 / (l2 - pow(0.158, 2.0))
                        + 0.08796 * l2 / (l2 - pow(40.50, 2.0))
                        + 3.17064 * l2 / (l2 - pow(60.98, 2.0))
                        + 0.30038 * l2 / (l2 - pow(120.34, 2.0))
                    );
                }), 
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   0.4947e11*si::pascal, 
            /*shear_modulus*/                     0.1287e11*si::pascal, 
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

// corundum, Al2O3, aluminum sequioxide, alumina, ruby, sapphire, beryl
// representative of precious stones, excluding diamond
PartlyKnownCompound  corundum {
    /*molar_mass*/                        101.96 * si::gram/si::mole, 
    /*atoms_per_molecule*/                5u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             900.0*si::kilojoule / si::kilogram,
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    field::missing(),
    /*freezing_point_sample_temperature*/ field::missing(),
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        // .dynamic_viscosity = 0.035 * si::pascal * si::second, // Blomquist (1978)
        /*dynamic_viscosity*/      field::StateFunction<si::dynamic_viscosity>([](const si::pressure p, const si::temperature T){ return exp((11448.0*si::kelvin/T - 8.2734))*si::pascal*si::second; }), // Blomquist (1978)
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            field::StateSample<si::specific_heat_capacity>(750.0*si::joule/(si::kilogram* si::kelvin), si::standard_pressure, 25.0*si::celcius),//azom.com/article.aspx?ArticleId=1948
            /*thermal_conductivity*/              field::StateSample<si::thermal_conductivity>(37.0*si::watt/(si::meter * si::kelvin), si::standard_pressure, 20.0*si::celcius),//azom.com/article.aspx?ArticleId=1948
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           3970.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    double l = (2.0 / (nhi+nlo) / si::micrometer);
                    double l2 = l*l;
                    return sqrt(
                        1.0
                        + 1.4313493 * l2 / (l2 - pow(0.1193242, 2.0))
                        + 0.65054713 * l2 / (l2 - pow(0.0726631, 2.0))
                        + 5.3414021 * l2 / (l2 - pow(18.028251, 2.0))
                    );
                }),
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      305.0 * si::gigapascal, //azom.com/article.aspx?ArticleId=1948
            /*tensile_modulus*/                   4.9735e11*si::pascal,
            /*shear_modulus*/                     1.4739e11*si::pascal,
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    field::missing(),
            /*poisson_ratio*/                     0.28,  //azom.com/article.aspx?ArticleId=1948

            /*compressive_fracture_strength*/     2265.0*si::megapascal, //azom.com/article.aspx?ArticleId=1948
            /*tensile_fracture_strength*/         325.0*si::megapascal, //azom.com/article.aspx?ArticleId=1948
            /*shear_fracture_strength*/           field::missing(),
            /*compressive_yield_strength*/        field::missing(),
            /*tensile_yield_strength*/            field::missing(),
            /*shear_yield_strength*/              field::missing(),

            /*chemical_susceptibility_estimate*/  false
        }
    }
};


// apatite, XCa5(PO4)3
// theoretical biomineral deposits, analogous to limestone, that could occur on alien planets
PartlyKnownCompound  apatite {
    /*molar_mass*/                        509.1 * si::gram/si::mole, 
    /*atoms_per_molecule*/                21u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    field::missing(),
    /*freezing_point_sample_temperature*/ field::missing(),
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },


    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              1.37 * si::watt / (si::meter*si::kelvin), // Cermak (1988)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           3180.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  1.645,
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   1.667e11*si::pascal,
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
// carbon, C
// for diamonds, graphite, high-grade anthracite coal, 
// and theoretical exobiominerals deposits analogous to limestone  // TODO: custom polymorphic class, with complex phase diagram
PartlyKnownCompound carbon {
    /*molar_mass*/                        12.011 * si::gram/si::mole, 
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             117.4*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    field::missing(),
    /*freezing_point_sample_temperature*/ field::missing(),
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(2566.0*si::celcius, 3016.0*si::celcius,    3635.0*si::celcius, 
                                                                            1.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
                                                                         // TOOD: autocomplete vapor pressure for solids/liquids if function is present for other phase
        /*refractive_index*/       field::missing()
    },


    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.710 * si::joule / (si::gram * si::kelvin), // wikipedia, Diamond is 0.5091
            /*thermal_conductivity*/              247.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia (middle range value), Diamond is 2200.0
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           2260.0 * si::kilogram/si::meter3, // 3513.0 for diamond
            /*vapor_pressure*/                    
                field::get_exponential_temperature_function<si::pressure>(2566.0*si::celcius, 3016.0*si::celcius,    3635.0*si::celcius, 
                                                                           10.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
            /*refractive_index*/                  
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    double l = (2.0 / (nhi+nlo) / si::micrometer);
                    constexpr double n = 2.4202f;
                    constexpr double dndl = -0.16730f;
                    return n + dndl * l;
                }),
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      443.0 * si::gigapascal, // wikipedia, for diamond
            /*tensile_modulus*/                   1050.0 * si::gigapascal, // wikipedia, for diamond
            /*shear_modulus*/                     478.0 * si::gigapascal, // wikipedia, from McSkimin (1972), for diamond
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
// calcite, CaCO3, calcium carbonate
// for biomineral deposits like limestone  // TODO: custom polymorphic class, with complex phase diagram
PartlyKnownCompound  calcite {
    /*molar_mass*/                        100.087 * si::gram/si::mole, 
    /*atoms_per_molecule*/                4u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 1612.0 * si::kelvin, // 1098.0 for aragonite
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },
    

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.793 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988, room temperature, 0.79 for aragonite
            /*thermal_conductivity*/              3.57 * si::watt / (si::meter * si::kelvin), // Cermak (1988), wikipedia, 2.23 for aragonite
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           2710.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    double l = (2.0 / (nhi+nlo) / si::micrometer);
                    return sqrt(
                        1.0
                        + 0.73358749f 
                        + 0.96464345 * l*l / (l*l - 1.94325203f)
                        + 1.82831454 * l*l / (l*l - 120.0)
                    );
                }),
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   1.4806e11*si::pascal, // for calcite // 1.5958e11*si::pascal, // for aragonite
            /*shear_modulus*/                     0.3269e11*si::pascal, // for calcite // 0.4132e11*si::pascal, // for aragonite
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    56.0 *  si::gigapascal, // https://www.subsurfwiki.org/wiki/1st_Lam%C3%A9_parameter,
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
// quartz, SiO2, silicon dioxide, silica, glass
// representative of felsic rocks, namely sand, and biomineral deposits like diatomaceous earth  // TODO: custom polymorphic class, with complex phase diagram
PartlyKnownCompound  quartz {
    /*molar_mass*/                        60.08 * si::gram/si::mole, 
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           1.7e8 * si::pascal,
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        5300.0* si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       11770e3 * si::joule / si::kilogram,
    /*latent_heat_of_fusion*/             9.76*si::kilojoule/(molar_mass*si::gram), // cristobalite
    /*triple_point_pressure*/             0.0003 * si::pascal,
    /*triple_point_temperature*/          1983.0 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 1722.0 * si::celcius, // cristobalite
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      exp(10.0) * si::poise, // Doremus (2002), at 1400 C
        /*density*/                2180.0 * si::kilogram/si::meter3, // from Murase and McBirney (1973), for rhyolitic magma
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(1966.0*si::celcius, 2149.0*si::celcius, 2368.0*si::celcius, 
                                                                            1.0 *si::pascal,    10.0*si::pascal,   100.0*si::pascal  ),
        /*refractive_index*/       field::missing()
    },
    

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*specific_heat_capacity*/            0.703 * si::joule / (si::gram * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
            /*thermal_conductivity*/              1.36 * si::watt / (si::centimeter * si::kelvin), // Cermak (1988), wikipedia, for vitreous silica
            /*dynamic_viscosity*/                 std::monostate(),
            /*density*/                           2650.0 *  si::kilogram/si::meter3, // alpha, 2533 beta, 2265 tridymite, 2334 cristobalite, 2196 vitreous
            /*vapor_pressure*/                    std::monostate(),
            /*refractive_index*/                  1.4585,  // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
            /*spectral_reflectance*/              
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    double l = (2.0 / (nhi+nlo) / si::micrometer);
                    return sqrt(
                        1.0
                        + 0.6961663 * l*l / (l*l - pow(0.0684043, 2.0))
                        + 0.4079426 * l*l / (l*l - pow(0.1162414, 2.0))
                        + 0.8974794 * l*l / (l*l - pow(9.896161,  2.0))
                    );
                }),

            /*bulk_modulus*/                      37.0 * si::gigapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
            /*tensile_modulus*/                   0.8680e11*si::pascal, 
            /*shear_modulus*/                     0.5820e11*si::pascal, 
            /*pwave_modulus*/                     std::monostate(),
            /*lame_parameter*/                    8.0 * si::gigapascal, // https://www.subsurfwiki.org/wiki/1st_Lam%C3%A9_parameter,
            /*poisson_ratio*/                     0.17, // https://www.momentive.com/en-us/categories/quartz/mechanical-properties

            /*compressive_fracture_strength*/     1.1 * si::gigapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
            /*tensile_fracture_strength*/         48.0 * si::megapascal, // https://www.qsiquartz.com/mechanical-properties-of-fused-quartz/
            /*shear_fracture_strength*/           std::monostate(),
            /*compressive_yield_strength*/        std::monostate(),
            /*tensile_yield_strength*/            std::monostate(),
            /*shear_yield_strength*/              std::monostate(),

            /*chemical_susceptibility_estimate*/  std::monostate()
        }
    }
};
// orthoclase, KAlSi3O8, 
// representative of felsic rocks
PartlyKnownCompound  orthoclase {
    /*molar_mass*/                        278.33 * si::gram/si::mole, 
    /*atoms_per_molecule*/                13u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 1170.0*si::celcius, // http://www.minsocam.org/msa/collectors_corner/arc/tempmagmas.htm
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      1e8 * si::poise, // observed by Bowen (1934) for molten Orthoclase, seems large compared to other silicates, but continental crust has been observed to have higher viscosity than oceanic (Itô 1979, "Rheology of the crust...", I can only get a hold of the abstract)
        /*density*/                2180.0 * si::kilogram/si::meter3, // from Murase and McBirney (1973), for rhyolitic magma
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.61 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           2560.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  1.527,
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      50.0 * si::gigapascal, // for Granite, wikipedia
            /*tensile_modulus*/                   52.0 * si::gigapascal, // for Granite, engineering toolbox
            /*shear_modulus*/                     24.0*si::gigapascal, // for Granite, wikipedia
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
// andesine, (Ca,Na)(Al,Si)4O8
// representative of plagioclase as a common plagioclase mineral in andesite and diorite,
// representative of intermediate rocks in general
PartlyKnownCompound andesine {
    /*molar_mass*/                        268.6 * si::gram/si::mole, 
    /*atoms_per_molecule*/                13u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 1100.0 * si::celcius, // for Albite, http://www.minsocam.org/msa/collectors_corner/arc/tempmagmas.htm
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      1.38e2 * si::poise, // 1.36-1.19 poises, observed by Kani for andesitic basaltic magma at 1400C, referenced by Bowen (1934)
        // .dynamic_viscosity = exp(5.0) * si::poise, // Doremus (2002) for Albite, at 1400 C
        // .dynamic_viscosity = 4e4 * si::poise, // observed by Bowen (1934) for Albite at 1400C
        // .dynamic_viscosity = 3.8e1 * si::poise, // observed by McCaffery for molten anorthite at 1550C, referenced by Bowen (1934)
        // .dynamic_viscosity = 1.38e2 * si::poise, // 1.36-1.19 poises, observed by Kani for andesitic basaltic magma at 1400C, referenced by Bowen (1934)
        // .dynamic_viscosity = 3.0e11 * si::pascal * si::second, // Melosh (2011), from Hiesinger (2007), for andesite lava flow, middle value on log scale
        /*density*/                2180.0 * si::kilogram/si::meter3, // from Murase and McBirney (1973), for rhyolitic magma
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            66.0 * si::joule / (268.6 * si::gram * si::kelvin), // Richet (1984)
            /*thermal_conductivity*/              1.68 * si::watt / (si::centimeter * si::kelvin), // Cermak (1988), for anorthite
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           2670.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  1.553,
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   field::missing(),
            /*shear_modulus*/                     field::missing(),
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    58.23*si::gigapascal, // for Gabbro, Shaocheng (2010)
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
// augite, (Ca,Na)(Mg,Fe,Al,Ti)(Si,Al)2O6
// representative of pyroxenes as the most common pyroxene mineral
// representative of mafic rocks in general
PartlyKnownCompound augite {
    /*molar_mass*/                        236.4 * si::gram/si::mole, 
    /*atoms_per_molecule*/                10u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 984.0*si::celcius, // for Basalt, http://www.minsocam.org/msa/collectors_corner/arc/tempmagmas.htm
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                2800.0 * si::kilogram/si::meter3, // from Murase and McBirney (1973), for basaltic  magma
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },


    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            // .dynamic_viscosity = 1e4 * si::pascal * si::second, // Melosh (2011), from Hiesinger (2007), for basaltic lava flow, order of magnitude estimate

            /*specific_heat_capacity*/            0.7 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988), representative of pyroxenes
            /*thermal_conductivity*/              3.82 * si::watt / (si::centimeter * si::kelvin), // Cermak (1988)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           3380.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  1.707,
            /*spectral_reflectance*/              field::missing(),
            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   field::missing(),
            /*shear_modulus*/                     69.0*si::gigapascal, // for Pyroxenite, Shaocheng (2010)
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
// forsterite, MgSiO 
// representative of Olivine ((Mg,Fe)SiO) as its much more common Mg-rich end member (Smyth 2006)
// representative of ultramafic rocks in general
// also appears most common on the surface of Mercury (Namur 2016)
PartlyKnownCompound forsterite {
    /*molar_mass*/                        153.31 * si::gram/si::mole, 
    /*atoms_per_molecule*/                7u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    field::missing(),
    /*freezing_point_sample_temperature*/ field::missing(),
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      1.27e2 * si::poise, // 1.36-1.19 poises, observed by Kani for olivine basaltic magma at 1400C, referenced by Bowen (1934)
        // .dynamic_viscosity = exp(1.5) * si::poise, // Doremus (2002) for Olivine, at 1400 C
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.79 * si::joule / (si::gram * si::kelvin), // Cermak (1988), for fayalite/forsterite mix
            /*thermal_conductivity*/              5.06 * si::watt / (si::centimeter * si::kelvin), // Cermak (1988)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           3810.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  1.651,
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   3.2848e11*si::pascal,
            /*shear_modulus*/                     0.6515e11*si::pascal,
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    73.5*si::gigapascal, // for Peridotite, Shaocheng (2010)
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
// Goethite, FeO(OH)
// for surface of mars, representative of iron oxides and red soils in general, and siderophile ores
PartlyKnownCompound  goethite {
    /*molar_mass*/                        88.85 * si::gram/si::mole, 
    /*atoms_per_molecule*/                4u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    field::missing(),
    /*freezing_point_sample_temperature*/ field::missing(),
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.701 * si::joule / (88.85 * si::gram * si::kelvin), // Majzlan (2003)
            /*thermal_conductivity*/              2.91 * si::watt / (si::meter * si::kelvin), // Cermak (1988)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           4300.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  2.401,
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
// pyrite, FeS2
// for surfaces of the heaviest planets, maybe venus and mercury, and representative of siderophile ores
PartlyKnownCompound  pyrite {
    /*molar_mass*/                        119.98 * si::gram/si::mole, 
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 1180.0 * si::celcius, // new world encyclopedia
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.5 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988, room temperature
            /*thermal_conductivity*/              19.2 * si::watt / (si::meter * si::kelvin), // Cermak (1988)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           5020.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  1.78, //gemologyproject.com
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   3.818e11*si::pascal,
            /*shear_modulus*/                     1.094e11*si::pascal,
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    59.0 * si::gigapascal, // https://www.subsurfwiki.org/wiki/1st_Lam%C3%A9_parameter
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
// hematite, Fe2O3 
// representative of iron oxides and red soils in general, surfaces of early earth, and siderophile ores
// TODO: pick one, goethite or hematite, we can't afford them both
PartlyKnownCompound hematite {
    /*molar_mass*/                        159.69 * si::gram/si::mole, 
    /*atoms_per_molecule*/                5u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    field::missing(),
    /*freezing_point_sample_temperature*/ field::missing(),
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.61 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
            /*thermal_conductivity*/              11.3 * si::watt / (si::meter * si::kelvin), // Cermak (1988)
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           5250.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    double l = (2.0 / (nhi+nlo) / si::micrometer);
                    return math::lerp(
                            std::vector<double>{-0.67, -0.61, -0.48, -0.44, -0.34, -0.23, -0.11,  0.68,  0.99,  1.12,  1.20,  1.26,  1.29,  1.33,  1.37,  1.46,  1.55,  1.63,  1.65,  1.73,  1.96},
                            std::vector<double>{ 1.32,  1.87,  2.49,  2.49,  3.28,  3.43,  2.93,  2.69,  2.31,  1.73,  0.49,  0.46,  2.84,  1.02,  7.39,  0.93, 12.55,  6.71,  7.07,  5.80,  5.10},
                            log10(l)
                        );
                }),
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   2.4243e11*si::pascal, 
            /*shear_modulus*/                     0.8569e11*si::pascal, 
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

// native gold, Au
// for precious metals
PartlyKnownCompound  gold {
    /*molar_mass*/                        196.967 * si::gram/si::mole, 
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           510e3 * si::kilopascal,
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        7250.0 * si::kelvin, // wow!
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       324.0 * si::kilojoule/(196.967*si::gram),
    /*latent_heat_of_fusion*/             12.55*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 1064.180*si::celcius,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                17310.0 * si::kilogram/si::meter3, 
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(1373.0*si::celcius, 2008.0*si::celcius,    2805.0*si::celcius, 
                                                                            1.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       field::missing()
    },


    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.129 * si::joule / (si::gram * si::kelvin), // wikipedia, room temperature
            /*thermal_conductivity*/              314.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           19300.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    double l = (2.0 / (nhi+nlo) / si::micrometer);
                    constexpr double n = 0.35018f; 
                    constexpr double dndl = -4.1791f;
                    return n + dndl * l;
                }),
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   1.9244e11*si::pascal,
            /*shear_modulus*/                     0.4200e11*si::pascal,
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    field::missing(),
            /*poisson_ratio*/                     0.43, // wikipedia

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
// native silver, Ag
// for precious metals
PartlyKnownCompound  silver {
    /*molar_mass*/                        107.868 * si::gram/si::mole, 
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        6410.0 * si::kelvin,
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             11.3*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 961.78*si::celcius,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                9320.0 * si::kilogram/si::meter3, 
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(1010.0*si::celcius, 1509.0*si::celcius,    2160.0*si::celcius, 
                                                                            1.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       field::missing()
    },


    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.233 * si::joule / (si::gram * si::kelvin), // wikipedia
            /*thermal_conductivity*/              427.0 * si::watt / ( si::meter * si::kelvin ), // wikipedia
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           10500.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    double l = (2.0 / (nhi+nlo) / si::micrometer);
                    constexpr double n = 0.051585f; 
                    constexpr double dndl = 0.28818f;
                    return n + dndl * l;
                }),
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   1.2399e11*si::pascal,
            /*shear_modulus*/                     0.4612e11*si::pascal,
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
// native copper, Cu
// for precious metals
PartlyKnownCompound  copper {
    /*molar_mass*/                        63.546 * si::gram/si::mole, 
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       204.8 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             13.26*si::kilojoule/(molar_mass*si::gram),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 1084.62*si::celcius,
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                8020.0 * si::kilogram/si::meter3, 
        /*vapor_pressure*/         
                field::get_exponential_temperature_function<si::pressure>(1236.0*si::celcius, 1816.0*si::celcius,    2563.0*si::celcius, 
                                                                            1.0 *si::pascal,     1.0*si::kilopascal,  100.0*si::kilopascal),
        /*refractive_index*/       field::missing()
    },


    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.385 * si::joule / (si::gram * si::kelvin), // wikipedia
            /*thermal_conductivity*/              401.0 * si::watt / (si::meter * si::kelvin), // wikipedia
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           8960.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    double l = (2.0 / (nhi+nlo) / si::micrometer);
                    constexpr double n = 0.059513f; 
                    constexpr double dndl = 13.100f;
                    return n + dndl * l;
                }),
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   1.683e11*si::pascal,
            /*shear_modulus*/                     0.757e11*si::pascal,
            /*pwave_modulus*/                     field::missing(),
            /*lame_parameter*/                    field::missing(),
            /*poisson_ratio*/                     0.33,

            /*compressive_fracture_strength*/     field::missing(),
            /*tensile_fracture_strength*/         220.0 * si::megapascal, // engineering toolbox
            /*shear_fracture_strength*/           field::missing(),
            /*compressive_yield_strength*/        field::missing(),
            /*tensile_yield_strength*/            70.0 * si::megapascal, // engineering toolbox
            /*shear_yield_strength*/              field::missing(),

            /*chemical_susceptibility_estimate*/  field::missing()
        }
    }
};
// magnetite, Fe3O4
// representative of siderophile ores, and for surfaces of mars and maybe venus
PartlyKnownCompound  magnetite {
    /*molar_mass*/                        231.53 * si::gram/si::mole,
    /*atoms_per_molecule*/                7u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    field::missing(),
    /*freezing_point_sample_temperature*/ field::missing(),
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },


    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.6 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988)
            /*thermal_conductivity*/              5.1 * si::watt / (si::centimeter * si::kelvin), // Cermak (1988), wikipedia, for vitrous silica
            /*dynamic_viscosity*/                 3e8 * si::pascal * si::second, // Melosh (2011), from Hiesinger (2007), for venusian lava flows, middle of range on log scale
            /*density*/                           5170.0 * si::kilogram/si::meter3,
            /*vapor_pressure*/                    field::missing(),
            /*refractive_index*/                  
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    double l = (2.0 / (nhi+nlo) / si::micrometer);
                    return math::lerp(
                            std::vector<double>{-0.68, -0.59, -0.49, -0.40, -0.32, -0.15, 0.00, 0.10, 0.50, 0.88, 1.06, 1.12, 1.23, 1.26, 1.30, 1.40, 1.47, 1.52, 1.55, 1.61, 1.74},
                            std::vector<double>{ 2.26,  2.43,  2.43,  2.49,  2.39,  2.39, 2.14, 2.13, 3.06, 3.59, 3.62, 3.52, 3.46, 5.29, 4.45, 4.39, 7.35, 6.48, 6.33, 6.55, 7.90},
                            log10(l)
                        );
                }),
            /*spectral_reflectance*/              field::missing(),

            /*bulk_modulus*/                      field::missing(),
            /*tensile_modulus*/                   2.730e11*si::pascal,
            /*shear_modulus*/                     0.971e11*si::pascal,
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
// chalcocite, Cu2S, 
// representative of chalcophile ores, and for surfaces of maybe venus and mercury
PartlyKnownCompound chalcocite {
    /*molar_mass*/                        159.16 * si::gram/si::mole, 
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    si::standard_pressure,
    /*freezing_point_sample_temperature*/ 1130.0 * si::celcius, // wikipedia
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },


    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            field::missing(),
            /*thermal_conductivity*/              field::missing(),
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           5600.0 * si::kilogram/si::meter3,
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
// chalcopyrite, Cu2S, 
// representative of chalcophile ores, and for surfaces of maybe venus and mercury
PartlyKnownCompound  chalcopyrite {
    /*molar_mass*/                        183.5 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                field::missing(),
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   field::missing(),

    /*critical_point_pressure*/           field::missing(),
    /*critical_point_volume*/             field::missing(),
    /*critical_point_temperature*/        field::missing(),
    /*critical_point_compressibility*/    field::missing(),

    /*latent_heat_of_vaporization*/       field::missing(),
    /*latent_heat_of_fusion*/             field::missing(),
    /*triple_point_pressure*/             field::missing(),
    /*triple_point_temperature*/          field::missing(),
    /*freezing_point_sample_pressure*/    field::missing(),
    /*freezing_point_sample_temperature*/ field::missing(),
    /*simon_glatzel_slope*/               field::missing(),
    /*simon_glatzel_exponent*/            field::missing(),

    /*molecular_absorption_cross_section*/ field::missing(),

    /*gas*/
    phase::PartlyKnownGas {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*specific_heat_capacity*/ field::missing(),
        /*thermal_conductivity*/   field::missing(),
        /*dynamic_viscosity*/      field::missing(),
        /*density*/                field::missing(),
        /*vapor_pressure*/         field::missing(),
        /*refractive_index*/       field::missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {

            /*specific_heat_capacity*/            0.54 * si::kilojoule / (si::kilogram * si::kelvin), // Cermak (1988), for chalcopyrite
            /*thermal_conductivity*/              8.19 * si::watt / (si::centimeter * si::kelvin), // Cermak (1988), for chalcopyrite
            /*dynamic_viscosity*/                 field::missing(),
            /*density*/                           4200.0 *  si::kilogram/si::meter3, //wikipedia
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




} // end namespace compound