#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "PartlyKnownCompound.hpp"
#include "state_functions.hpp"
#include "spectral_functions.hpp"

namespace compound {
namespace published {

/* 
Hi, and welcome to Crazy Eddie's Chemical Properties Emporium, where WE'VE GONE CRAZY!

SOURCES (unless otherwise noted in comments):
* Generic properties (those not mentioned below) are taken from 91st Handbook Of Chemistry And Physics. 
* Custom phase functions are taken from phase diagrams by Young (1975), "Phase Diagrams of the Elements", unless otherwise noted
* Molecular absorption functions are taken from the MPI-Mainz UV/VIS Spectral Atlas of Gaseous Molecules of Atmospheric Interest.
* Refraction index functions are taken from refractionindex.info.
* Reflectance functions are taken from version 7 of the USGS spectral library
* exponential and sigmoid functions for fluids at pressure and temperature were parameterized using data from NIST, https://webbook.nist.gov/chemistry/fluid/
* "Perry" functions for temperature were taken from the 9th edition of Perry's Chemical Engineer's Handbook
* "Dippr" functions for temperature are from the "DIPPR Data Compilation of Pure Chemical Properties", copied from the 9th edition of Perry's Chemical Engineer's Handbook
* "Prokhvatilov" functions for yield and fracture strength of cryogenic solids are taken from "Plasticity and Elasticity of Cryocrystals" by Prokhvatilov (2001)
* "Manzhelii" functions for thermal conductivity and specific heat of cryogenic solids are taken from "Physics of Cryocrystals" by Manzhelii & Freiman (1997)
* "Schön" properties for elastic moduli for minerals are taken from "Physical Properties of Rocks: Fundamentals and Principles of Petrophysics" by Juergen H. Schön (2015)

NOTE:
Some compounds, such as water or nitrogen, are common enough
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


// water, H2O
// for the oceans and ice caps of earth, and the surface and mantle of europa, and the surface of pluto
PartlyKnownCompound water (
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

    /*phase*/
    field::StateFunction<int>([]
            (const si::pressure pressure, 
             const si::temperature temperature)
            {
                const int supercritical = -3;
                const int gas = -2;
                const int liquid = -1;
                const int ice1h = 0;
                const int ice1c = 1;
                const int ice2 = 2;
                const int ice3 = 3;
                // const int ice4 = 4; // unused
                const int ice5 = 5;
                const int ice6 = 6;
                const int ice7 = 7;
                const int ice8 = 8;
                const int ice9 = 9;
                const int ice10 = 10;
                const int ice11h = 11;
                const int ice11o = 12;
                // const int ice12 = 13; // unused
                // const int ice13 = 14; // unused
                // const int ice14 = 15; // unused
                const int ice15 = 16;
                const double p = pressure / si::pascal;
                const double t = temperature / si::kelvin;
                const     double Ru(8.3144598);          // universal gas constant
                const     double M (0.0180153);          // molar mass, kg/mol
                const     double L (22.6e5);             // specific latent heat of vaporization (J/kg)
                const     double p0(0.6116e3);           // triple point pressure (Pa)
                const     double t0(273.15);             // triple point temperature (K)
                constexpr double R (Ru/M);               // individual gas constant
                constexpr double m (-R/L);               // slope of clapeyron equation for vaporus
                constexpr double b (1/t0 - m * std::log(p0)); // intercept for clapeyron equation for vaporus
                const     double tc(647.096);            // critical point temperature (K) 
                const     double pc(22.064e6);           // critical point pressure (Pa)
                const     double a3(7070e5);             // slope of clapeyron equation repurposed for liquidus, estimated from phase diagram
                const     double c3(4.46);               // intercept for clapeyron equation repurposed for liquidus, estimted from phase diagram
                if ( t > 1.0/(m*std::log(p)+b) && p<1e9f)
                {
                    if (t > tc && p > pc)
                    {
                        return supercritical;
                    }
                    else
                    {
                        return gas;
                    }
                }
                else if ( t > t0 * std::pow(std::max((p-p0)/a3 + 1.0, 0.0), 1.0/c3) )
                {
                    if (t > tc && p > pc)
                    {
                        return supercritical;
                    }
                    else
                    {
                        return liquid;
                    }
                }
                else if (p < 209.9e6 && t < 73.15f)
                {
                    return ice11o;
                }
                else if (p < 209.9e6 && t < 173.15f)
                {
                    return ice1c;
                }
                else if (p < 209.9e6f)
                {
                    return ice1h;
                }
                else if (p < 632.4e6 && t < 170.0f)
                {
                    return ice9;
                }
                else if (p < 350.1e6 && t > 238.5f)
                {
                    return ice3;
                }
                else if (350.1e6 < p && p < 632.4e6 && t > 218.0f)
                {
                    return ice5;
                }
                else if (p < 632.4e6f)
                {
                    return ice2;
                }
                else if (p < 2.216e9 && t < 130.0f)
                {
                    return ice15;
                }
                else if (p < 2.216e9f)
                {
                    return ice6;
                }
                else if (p < 62e9 && (t-100.0f)/(278.0f-100.0f) < (p-62e9f)/(2.1e9f-62e9f))
                {
                    return ice8;
                }
                else if (p < 62e9f)
                {
                    return ice7;
                }
                else if (p < 350e9f)
                {
                    return ice10;
                }
                else 
                {
                    return ice11h;
                }
            }
        ),

    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  9.73e1, 6.05e2, 7.37e3, 1.65e4, 2.98e4, 6.50e4, 9.73e4, 1.38e5, 1.62e5, 2.63e5, 3.35e5, 4.39e5, 5.15e5, 5.89e5, 6.93e5, 9.82e5, 1.25e6, 1.64e6, 2.08e6, 2.38e6, 2.41e6, 2.44e6, 2.47e6, 2.53e6, 2.59e6, 2.63e6, 2.73e6, 2.78e6, 2.93e6, 2.98e6, 3.05e6, 3.08e6, 5.11e6, 5.63e6, 6.04e6, 6.45e6, 6.86e6, 8.04e6, 9.68e6, 1.08e7, 1.24e7, 1.37e7, 2.37e7, 3.94e7, 6.98e7, 1.69e8 },
          std::vector<double>{  -24.98, -24.44, -23.93, -23.46, -23.46, -22.97, -23.70, -23.77, -23.11, -24.44, -22.46, -25.14, -24.47, -25.68, -25.10, -27.10, -28.15, -29.10, -30.25, -29.38, -29.28, -29.28, -29.47, -29.22, -29.47, -29.55, -29.28, -29.21, -29.27, -28.95, -28.71, -28.69, -25.41, -21.62, -21.41, -21.51, -21.76, -21.09, -20.98, -20.74, -20.82, -20.75, -20.83, -21.08, -21.54, -22.44 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ 
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.01766, 0.80539, 0.00707, 0.69586, 1.42782,
                 300.0, 1273.2,  0.0, 10.0, 0.034),
                // water, mean error: 0.8%, max error: 3.4%, range: 300-1273.2K, 0-10MPa, stp estimate: 1.781
        /*thermal_conductivity*/   
            relation::get_sigmoid_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00054, 1.09614, 0.00000, 0.00000, 0.09827, 691.90362, 883.95160, 0.08323,
                300.0, 1273.2, 0.0, 10.0, 0.097), 
                // water, mean error: 2.5%, max error: 9.7%, range: 300-1273.2K, 0-10MPa, stp estimate: 0.018
        /*dynamic_viscosity*/      
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.00019, 3.33694, 0.02183, 1.08016, -0.58257,
                 300.0, 1273.2,  0.0, 10.0, 0.035), 
                // water, mean error: 1.2%, max error: 3.5%, range: 300-1273.2K, 0-10MPa, stp estimate: 8.765
        /*density*/                missing(),
        /*refractive_index*/       1.000261                                        
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ 
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (18.01528 * si::kilogram * si::kelvin), 
                276370.0, -2090.1, 8.125, -0.014116, 9.3701e-6,
                273.16, 533.15), 
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 -0.432, 0.0057255, -0.000008078, 1.861e-9, 0.0,
                  273.15, 633.15), 
        /*dynamic_viscosity*/      
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -52.843, 3703.6, 5.866, -5.879e-29, 10.0,
                  273.16, 646.15), // 273.16-646.15K
        /*density*/                
            field::StateFunction<si::density>([](const si::pressure p, const si::temperature T) {
                // Perry equation 119, specialized for water
                // valid for 273.16-647.096K
                double Tc = 647.096;
                double t = std::clamp(T/si::kelvin, 273.16, Tc);
                double tau = 1.0 - (t/Tc);
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
            // get_dippr_temperature_function_101
            //     (si::kelvin, si::pascal,
            //      73.649, -7258.2, -7.3037, 4.1653e-6,
            //      273.16, 647.1),//273.16-647.1K
            field::StateFunction<si::pressure>([](const si::pressure p, const si::temperature T) {
                // Buck equation
                double C = T/si::celcius;
                return 0.61121*exp((18.678-C/234.5) * (C/(257.14+C))) * si::kilopascal; 
            }),
        /*surface_tension*/            
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 647.01, -0.1306, 2.471, 0.2151, 1.233, 0.0, 0.0, 
                 233.22, 646.15), // Mulero (2012)
        /*refractive_index*/       //1.33336,
            compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                (si::micrometer, 1.0,
                 std::vector<double>{0.2,   0.3,   0.425,  0.55, 0.675,   0.8, 0.925,   1.4,   2.4,   2.8,  3.05,  3.3,   5.6,   6.6,   7.6,  13.0,  15.5,  18.0,  21.0,  26.0,  32.0,  42.0,  60.0, 110.0, 160.0,200.0},
                 std::vector<double>{1.396, 1.349, 1.338, 1.333, 1.331, 1.329, 1.328, 1.321, 1.279, 1.142, 1.426, 1.45, 1.289, 1.334, 1.302, 1.146, 1.297, 1.423, 1.487, 1.539, 1.546, 1.522, 1.703, 1.966, 2.081, 2.13}),
                // Hale (1973)
        /*extinction_coefficient*/ 
            compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                (si::micrometer, 1.0,
                 std::vector<double>{0.2,     0.3,     0.425,   0.55,     0.675,    0.8,      0.925,    1.4,      2.8,   3.0,   3.3,    3.6,     4.1,     4.6,    5.6,    6.1,      6.6,    9.2,   10.5,  13.0,  15.5,  18.0,  32.0,  60.0, 200.0},
                 std::vector<double>{1.1e-07, 1.6e-08, 1.3e-09, 1.96e-09, 2.23e-08, 1.25e-07, 1.06e-06, 0.000138, 0.115, 0.272, 0.0368, 0.00515, 0.00562, 0.0147, 0.0142, 0.131, 0.0356, 0.0415, 0.0662, 0.305, 0.414, 0.426, 0.324, 0.587, 0.504})
                // Hale (1973)
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid { // ice1h, regular ice
            /*isobaric_specific_heat_capacity*/   
                get_perry_johnson_temperature_function
                    (si::kelvin, 
                     si::joule/(si::gram*si::kelvin),
                     0.0, 1.0/66e3, 15.0,
                     si::kilojoule / (si::kilogram * si::kelvin), 
                     2.05, 1.0/186.0, 0.0, -1.0/80e3,
                     173.15, 273.15), // Johnson (1960) and Engineering Toolbox, custom fit
            /*thermal_conductivity*/              
                get_interpolated_temperature_function
                    (si::kelvin, si::watt/(si::meter*si::kelvin),
                     std::vector<double>{ 2.0,  4.0,  8.0, 10.0, 30.0, 100.0, 173.0, 193.0, 233.0, 253.0, 273.15 }, 
                     std::vector<double>{21.0, 57.0, 87.0, 81.0, 22.0,   6.0,  3.48,  3.19,  2.63,  2.39,   2.22 }), // Ahmad (1994) and Engineering Toolbox
            /*dynamic_viscosity*/                 
                state_invariant(1e13 * si::poise),                                 // reference by Carey (1953)
            /*density*/                           
                get_quadratic_pressure_function
                   (si::kilobar, si::gram/si::centimeter3,
                    0.9228, 0.00728, 0.00075), // Gagnon (1990)
            /*vapor_pressure*/                   
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-17.3,  11.2,   51.6,   100.0  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/       
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.0443, 0.0528, 0.067,  0.0886, 0.1181, 0.137,  0.1468, 0.158,  0.177,  0.197,  0.41,   0.51,   0.81,   1.515,  1.76,   1.923,  2.29,   2.52,   2.817,  2.985,  3.175,  3.39,   3.775,  4.56,   5.714,  6.329,  7.143,  8.333,  10.31,  11.9,   14.08,  17.24,  22.22,  34.48,  46.15,  55,     62.5,   76.29,  210, 1300.0},
                     std::vector<double>{0.8228, 0.8505, 0.9679, 1.3807, 1.3783, 1.2849, 1.5729, 1.6309, 1.4462, 1.3974, 1.3185, 1.3126, 1.3047, 1.2914, 1.2843, 1.2771, 1.2545, 1.2232, 1.0657, 1.018, 1.6336, 1.5086, 1.385, 1.3482, 1.2933, 1.3178, 1.3236, 1.2964, 1.1501, 1.2546, 1.5701, 1.5303, 1.4575, 1.253, 1.2543, 1.7955, 1.6372, 1.8504, 1.8114, 1.7868}),
                    // Warren (2008)
            /*extinction_coefficient*/ 
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer, 1.0,
                     std::vector<double>{0.0443,   0.0528,   0.067,    0.0886,   0.1181,   0.137,    0.1468,   0.158,    0.177,    0.197,    0.41,     0.51,     0.61,     0.71,     0.81,     0.91,     1.01,     1.11,     1.21,     1.31,     1.41,     1.515,    1.76,     1.923,    2.13,     2.29,     2.52,     2.817,    3.003,    3.175,    3.39,     3.775,    4.56,     5.714,    6.329,    7.143,    8.333,    10.31,    11.9,     14.08,    17.24,    22.22,    34.48,    46.15,    55,       76.29,    210,      140000,   240000,   2000000  },
                     std::vector<double>{0.164,    0.268,    0.492,    0.468,    0.244,    0.271,    0.345,    0.083,    1.061e-05,2.816e-10,2.669e-11,8.036e-10,6.89e-09, 3.44e-08, 1.4e-07,  4.44e-07, 2e-06,    1.76e-06, 8.66e-06, 1.31e-05, 3.442e-05,0.0005143,0.0001522,0.000706, 0.0005255,0.0002538,0.0007685,0.03245,  0.438,    0.372,    0.04286,  0.006966, 0.03007,  0.03256,  0.0617,   0.0494,   0.037,    0.075,    0.409,    0.271,    0.107,    0.029,    0.1615,   0.7684,   0.4214,   0.334,    0.04282,  9.36e-05, 0.0001066,0.0006596}),
                    // Warren (2008)
            /*absorption_coefficient*/            missing(),
            /*bulk_modulus*/                      state_invariant(8.899 * si::gigapascal),                           // gammon (1983)
            /*tensile_modulus*/                   state_invariant(9.332 * si::gigapascal),                           // gammon (1983)
            /*shear_modulus*/                     state_invariant(3.521 * si::gigapascal),                           // gammon (1983)
            /*pwave_modulus*/                     state_invariant(13.59 * si::gigapascal),                           // gammon (1983)
            /*lame_parameter*/                    state_invariant(6.552 * si::gigapascal),                           // gammon (1983)
            /*poisson_ratio*/                     state_invariant(0.3252),                                           // gammon (1983)

            /*compressive_fracture_strength*/     state_invariant(6.0 * si::megapascal),                             //engineering toolbox
            /*tensile_fracture_strength*/         state_invariant(1.0 * si::megapascal),                             //engineering toolbox
            /*shear_fracture_strength*/           state_invariant(1.1 * si::megapascal),                             // Frederking (1989)
            /*compressive_yield_strength*/        state_invariant(6.0 * si::megapascal),                             // brittle, effectively the same as fracture strength
            /*tensile_yield_strength*/            state_invariant(1.0 * si::megapascal),                             // brittle, effectively the same as fracture strength
            /*shear_yield_strength*/              state_invariant(1.1 * si::megapascal),                             // brittle, effectively the same as fracture strength

            /*chemical_susceptibility_estimate*/  false
        },
        phase::PartlyKnownSolid { // ice1c

            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(0.964 * si::gram/si::centimeter3),                    // Tchijov (2008)
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
        phase::PartlyKnownSolid { // ice2

            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_quadratic_pressure_function
                   (si::kilobar, si::gram/si::centimeter3,
                    1.1698, 0.00818, 0.0), // Gagnon (1990)
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      state_invariant(14.39e4 * si::bar),                                   // Gagnon (1990)
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     state_invariant(6.2e4 * si::bar),                                     // Gagnon (1990)
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
        phase::PartlyKnownSolid { // ice3

            /*isobaric_specific_heat_capacity*/   state_invariant(2500.0*si::joule/(si::kilogram*si::kelvin)),          // Tchijov (2008)
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_quadratic_pressure_function
                   (si::kilobar, si::gram/si::centimeter3,
                    1.1321, 0.01206, 0.0), // Gagnon (1990)
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      state_invariant(9.6e4 * si::bar),                                     // Gagnon (1990)
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     state_invariant(4.6e4 * si::bar),                                     // Gagnon (1990)
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
        phase::PartlyKnownSolid { // ice4

            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(1.314 * si::gram/si::centimeter3),                    // Tchijov (2008)
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
        phase::PartlyKnownSolid { // ice5

            /*isobaric_specific_heat_capacity*/   state_invariant(2500.0*si::joule/(si::kilogram*si::kelvin)),          // Tchijov (2008)
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_quadratic_pressure_function
                   (si::kilobar, si::gram/si::centimeter3,
                    1.1974, 0.01963, 0.0), // Gagnon (1990)
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      state_invariant(13.86e4 * si::bar),                                   // Gagnon (1990)
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     state_invariant(6.1e4 * si::bar),                                     // Gagnon (1990)
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
        phase::PartlyKnownSolid { // ice6

            /*isobaric_specific_heat_capacity*/   state_invariant(2600.0*si::joule/(si::kilogram*si::kelvin)),          // Tchijov (2008)
            /*thermal_conductivity*/              
                get_interpolated_pressure_function
                    (si::gigapascal, si::watt/(si::meter*si::kelvin),
                     std::vector<double>{ 0.96, 2.17}, 
                     std::vector<double>{ 1.69, 1.79}), // Carretero (2018)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_quadratic_pressure_function
                   (si::kilobar, si::gram/si::centimeter3,
                    1.1559, 0.04521, -0.002435), // Gagnon (1990)
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      state_invariant(18.48e4 * si::bar),                                   // Gagnon (1990)
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     state_invariant(7.5e4 * si::bar),                                     // Gagnon (1990)
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
        phase::PartlyKnownSolid { // ice7

            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              
                get_interpolated_pressure_function
                    (si::gigapascal, si::watt/(si::meter*si::kelvin),
                     std::vector<double>{7.52,  9.97,  19.75 }, 
                     std::vector<double>{ 4.6,   5.7,   9.85 }), // Carretero (2018)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_interpolated_pressure_function
                    (si::gigapascal, si::gram/si::centimeter3,
                     std::vector<double>{ 7.52,  9.97,  19.75 }, 
                     std::vector<double>{1.688, 1.748,  1.915 }), // Carretero (2018)
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      state_invariant(23.9*si::gigapascal),                                 //Fei (1993)
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
        phase::PartlyKnownSolid { // ice8

            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(1.674 * si::gram/si::centimeter3),                    // Tchijov (2008)
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
        phase::PartlyKnownSolid { // ice9

            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(1.210 * si::gram/si::centimeter3),                    // Tchijov (2008)
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
        phase::PartlyKnownSolid { // ice10

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
        },
        phase::PartlyKnownSolid { // ice11h

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
        },
        phase::PartlyKnownSolid { // ice11o

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
        },
        phase::PartlyKnownSolid { // ice12

            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(1.314 * si::gram/si::centimeter3),                    // Tchijov (2008)
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
        phase::PartlyKnownSolid { // ice13

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
        },
        phase::PartlyKnownSolid { // ice14

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
        },
        phase::PartlyKnownSolid { // ice15

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
        },
        phase::PartlyKnownSolid { // ice16

            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(0.81*si::gram/si::centimeter3), //wikipedia
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


// nitrogen, N2
// for the atmosphere of Earth, and the surfaces of pluto or triton
PartlyKnownCompound nitrogen (
    /*molar_mass*/                        28.013  * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                357.8 * si::picometer,                                    // Mehio (2014)
    /*molecular_degrees_of_freedom*/      5.0,
    /*acentric_factor*/                   0.040,

    /*critical_point_pressure*/           3.39 * si::megapascal,
    /*critical_point_volume*/             90.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        126.21 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       198.8 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             0.71*si::kilojoule/(28.013*si::gram),
    /*triple_point_pressure*/             12.463 * si::kilopascal,
    /*triple_point_temperature*/          63.15 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -210.0*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -195.8 * si::celcius, // Perry

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
                const int gamma = 2;

                // current pressure and temperature
                // triple point between solid, liquid, and gas
                const si::pressure        p0 = 12.463 * si::kilopascal;
                const si::temperature     T0 = 63.15 * si::kelvin;
                // triple point between alpha, beta, and gamma solids
                const si::pressure        pabg = 4650 * si::atmosphere;
                const si::temperature     Tabg = 44.5 * si::kelvin;
                // critical point
                const si::pressure        pc = 3.39 * si::megapascal;
                const si::temperature     Tc = 126.21 * si::kelvin;
                // latent heat of vaporization and molar mass
                const si::specific_energy L  = 198.8 * si::joule/si::gram;
                const si::molar_mass      M  = 28.013  * si::gram/si::mole;
                // Simon-Glatzel constants, from Manzhelii (1997)
                const si::pressure        a  = 0.9598 * si::bar;
                const si::pressure        b  = -1592.0 * si::bar;
                const double              c  = 1.7895;
                const auto                dpdTab = (pabg-p0) / (Tabg-T0);

                const int basic_phase = property::get_simon_glatzel_phase(p, T, p0, T0, pc, Tc, L,  M, a, b, c);
                if (basic_phase < 0)
                {
                    return basic_phase;
                } 
                else if ( p < dpdTab * (T-Tabg) + pabg )
                {
                    return beta;
                }
                else if ( p > pabg )
                {
                    return alpha;
                }
                else
                {
                    return gamma;
                }
            }
        ),

    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_output
        (si::meter, si::meter2,
         std::vector<double>{ 8.22e6, 9.25e6, 9.94e6, 1.03e7, 1.13e7, 1.21e7, 1.89e7, 3.35e7, 3.93e7, 9.31e7, 1.07e9 },
         std::vector<double>{ -26.22, -23.04, -20.64, -20.27, -20.29, -20.62, -20.58, -20.93, -20.96, -21.80, -23.38 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 1.341 * si::joule / (si::gram * si::kelvin),
            relation::get_sigmoid_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.00115, 0.62179, 0.00000, 0.00000, 2.54371, 2268.64874, -2637.49785, -0.99334,
                163.15, 2013.2, 0.0, 3.0, 0.054), 
                // nitrogen, mean error: 0.8%, max error: 5.4%, range: 163.15-2013.2K, 0-3MPa, stp estimate: 1.013
        /*thermal_conductivity*/   // 0.0234 * si::watt / (si::meter * si::kelvin),                    // wikipedia
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00026, 1.21142, 0.00037, 0.75287, -0.00165,
                 113.15, 2013.2,  0.0, 3.0, 0.050), 
                // nitrogen, mean error: 0.8%, max error: 5.0%, range: 113.15-2013.2K, 0-3MPa, stp estimate: 0.024
        /*dynamic_viscosity*/      // 1.76e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.04908, 2.30670, 0.50611, 0.64605, -2.56297,
                 113.15, 2013.2,  0.0, 3.0, 0.051), 
                // nitrogen, mean error: 1.5%, max error: 5.1%, range: 113.15-2013.2K, 0-3MPa, stp estimate: 16.416
        /*density*/                missing(),
        /*refractive_index*/       missing()
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     double invl2 = 1.0/(l*l);
        //     return 1.0 + 6.497378e-5 + 3.0738649e-2 / (144.0 - invl2);
        // }) // Peck (1966)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 2.042 * si::joule / (si::gram * si::kelvin),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (28.0134 * si::kilogram * si::kelvin), 
                281970.0, -12281.0, 248.0, -2.2182, 0.0074902,
                63.15, 112.0), 
        /*thermal_conductivity*/                  
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.2654, -0.001677,  0.0,  0.0,  0.0,
                  63.15, 124), 
        /*dynamic_viscosity*/      // 157.9 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 16.004, -181.61, -5.1551, 0.0, 0.0,
                  63.15, 124), // 63.15-124K
        /*density*/                // state_invariant(0.4314*si::gram/si::centimeter3), // Johnson (1960), @ si::atmosphere, 125.01*si::kelvin
            get_dippr_temperature_function_105
                (si::kelvin, 28.013  * si::gram/si::decimeter3,
                 3.2091, 0.2861, 126.2, 0.2966,
                 63.15, 126.2), // 63.15-126.2K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 58.282, -1084.1, -8.3144, 0.044127, 1.0,
                  63.15, 126.2), // 63.15-126.2K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 126.21, 0.02898, 1.246, 0.0, 0.0, 0.0, 0.0,
                 64.8, 120.24), // Mulero (2012)
        /*refractive_index*/                      1.19876,
        /*extinction_coefficient*/                missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        // beta
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/                   
                get_interpolated_temperature_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                      std::vector<double>{ 39.0, 60.0, 62.5},
                      std::vector<double>{37.39,45.64,46.97}), // Manzhelii (1997)
            /*thermal_conductivity*/              
                get_interpolated_temperature_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{ 45.0, 57.5}, 
                     std::vector<double>{ 2.21, 2.09}), // Manzhelii (1997)
            /*dynamic_viscosity*/                 missing(),
                // field::StateFunction<si::dynamic_viscosity>([](si::pressure p, si::temperature T){ 
                //     return math::mix(2.5e9, 0.6e9, math::linearstep(45.0, 56.0, T/si::kelvin))*si::pascal*si::second;
                // }), // Yamashita 2010
            /*density*/                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{0.9955,0.9899,0.9862,0.9845,0.9748,0.9669,0.9586}), // Prokhvatilov
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-226.1, -219.1, -209.7, -195.8 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  1.25, // wikipedia
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{14.91, 14.54, 14.26, 14.26, 13.26, 12.52, 11.71}), // Prokhvatilov
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{18.18, 17.95, 17.81, 17.66, 17.31, 16.94, 16.54}), // Prokhvatilov
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{7.008, 6.935, 6.893, 6.893, 6.472, 6.645, 6.540}), // Prokhvatilov
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 37.0,  40.0,  42.0,  44.0,  48.0,  52.0,  56.0}, 
                     std::vector<double>{ 0.30,  0.29,  0.29,  0.29,  0.28,  0.28,  0.27}), // Prokhvatilov

            /*compressive_fracture_strength*/     
                field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                    return math::mix(0.24, 6.00, math::linearstep(58.0, 5.0, T/si::kelvin))*si::megapascal;
                }), // wikipedia, and Yamashita (2010)
            /*tensile_fracture_strength*/         
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 22.0,  24.0,  26.0,  28.0,  30.0,  35.0,  40.0}, 
                     std::vector<double>{220.0, 234.0, 350.0, 540.0, 515.0, 446.0, 240.0}), // Prokhvatilov
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 22.0,  24.0,  26.0,  28.0,  30.0,  35.0,  40.0}, 
                     std::vector<double>{175.0, 130.0, 100.0,  80.0,  78.0,  76.0,  70.0}), // Prokhvatilov
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }, 
        // alpha
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/
                get_interpolated_temperature_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                      std::vector<double>{0.0,  2.0,  6.0, 10.0, 30.0, 35.3},
                      std::vector<double>{0.0,0.024,0.907,4.832,34.16,45.53}), // Manzhelli (1997)
            /*thermal_conductivity*/              
                get_interpolated_temperature_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{  1.1,  1.5,  2.0,  3.3,   3.9,   4.5,   5.0,   6.0,   7.0,   8.0,   9.0,  14.0,  17.0,  24.0,  30.0,  34.0 }, 
                     std::vector<double>{17.08,40.27, 84.9,237.1, 238.5, 199.1, 156.3, 87.82, 50.56, 32.55, 22.73,  7.56,  5.26,  3.33,  2.81,  2.66 }), // Manzhelii (1997)
            /*dynamic_viscosity*/                 missing(),
                // field::StateFunction<si::dynamic_viscosity>([](si::pressure p, si::temperature T){ 
                //     return math::mix(2.5e9, 0.6e9, math::linearstep(45.0, 56.0, T/si::kelvin))*si::pascal*si::second;
                // }), // Yamashita 2010
            /*density*/                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0}, 
                     std::vector<double>{1.0296,1.0284,1.0271,1.0255,1.0237,1.0216,1.0192,1.0166,1.0134,1.0095}), // Prokhvatilov
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-226.1, -219.1, -209.7, -195.8 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  1.25,                                             // wikipedia
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0}, 
                     std::vector<double>{21.71, 21.65, 21.54, 21.38, 21.15, 20.81, 20.42, 20.01, 19.42, 18.76}), // Prokhvatilov
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0}, 
                     std::vector<double>{21.48, 21.45, 21.37, 21.29, 21.18, 21.06, 20.90, 20.70, 20.38, 19.98}), // Prokhvatilov
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0}, 
                     std::vector<double>{8.046, 8.036, 8.008, 7.978, 7.946, 7.911, 7.857, 7.783, 7.688, 7.553}), // Prokhvatilov
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 16.0,  18.0,  20.0,  22.0,  24.0,  26.0,  28.0,  30.0,  32.0,  34.0}, 
                     std::vector<double>{ 0.34,  0.34,  0.34,  0.33,  0.33,  0.33,  0.33,  0.33,  0.33,  0.32}), // Prokhvatilov

            /*compressive_fracture_strength*/     
                field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                    return math::mix(0.24, 6.00, math::linearstep(58.0, 5.0, T/si::kelvin))*si::megapascal;
                }), // wikipedia, and Yamashita (2010)
            /*tensile_fracture_strength*/         
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 22.0,  24.0,  26.0,  28.0,  30.0,  35.0,  40.0}, 
                     std::vector<double>{220.0, 234.0, 350.0, 540.0, 515.0, 446.0, 240.0}), // Prokhvatilov
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 22.0,  24.0,  26.0,  28.0,  30.0,  35.0,  40.0}, 
                     std::vector<double>{175.0, 130.0, 100.0,  80.0,  78.0,  76.0,  70.0}), // Prokhvatilov
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        },
        phase::PartlyKnownSolid { // gamma
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







} // end namespace published
} // end namespace compound