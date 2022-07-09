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







// oxygen, O2
// for atmospheres of earth-like planets
PartlyKnownCompound oxygen (
    /*molar_mass*/                        31.9988 * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                334.0 * si::picometer,                                    // Mehio (2014)
    /*molecular_degrees_of_freedom*/      5.06,
    /*acentric_factor*/                   0.022,

    /*critical_point_pressure*/           5.043 * si::megapascal,
    /*critical_point_volume*/             73.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        154.59 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       213.1 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             0.44*si::kilojoule/(31.9988*si::gram),
    /*triple_point_pressure*/             0.14633 * si::kilopascal,
    /*triple_point_temperature*/          54.35 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -218.79*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -183.0 * si::celcius, // Perry

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

                // triple point between solid, liquid, and gas
                const si::pressure        p0 = 0.14633 * si::kilopascal;
                const si::temperature     T0 = 54.35 * si::kelvin;
                // critical point
                const si::pressure        pc = 5.043 * si::megapascal;
                const si::temperature     Tc = 154.59 * si::kelvin;
                // latent heat of vaporization and molar mass
                const si::specific_energy L  = 213.1 * si::joule/si::gram;
                const si::molar_mass      M  = 31.9988 * si::gram/si::mole;
                // Simon-Glatzel constants
                const si::pressure        a  = 2733e5 * si::pascal;
                const si::pressure        b  = 0.0 * si::pascal;
                const double              c  = 1.7425;
                const si::pressure        pab0 = 0.0 * si::kilobar;
                const si::temperature     Tab0 = 24.0 * si::kelvin;
                const si::pressure        pab1 = 9.0 * si::kilobar;
                const si::temperature     Tab1 = 52.0 * si::kelvin;
                const auto                dpdTab = (pab1-pab0) / (Tab1-Tab0);
                const si::pressure        pbg0 = 0.0 * si::kilobar;
                const si::temperature     Tbg0 = 44.0 * si::kelvin;
                const si::pressure        pbg1 = 2.0 * si::kilobar;
                const si::temperature     Tbg1 = 55.0 * si::kelvin;
                const auto                dpdTbg = (pbg1-pbg0) / (Tbg1-Tbg0);

                const int basic_phase = property::get_simon_glatzel_phase(p, T, p0, T0, pc, Tc, L,  M, a, b, c);
                if (basic_phase < 0)
                {
                    return basic_phase;
                } 
                else if ( p > dpdTab * (T-Tab0) + pab0 )
                {
                    return alpha;
                }
                else if ( p > dpdTbg * (T-Tbg0) + pbg0 )
                {
                    return beta;
                }
                else
                {
                    return gamma;
                }
            }
        ),

    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  5.58e6, 5.72e6, 6.36e6, 6.93e6, 7.26e6, 7.40e6, 7.50e6, 7.65e6, 7.74e6, 7.86e6, 8.05e6, 8.24e6, 8.36e6, 8.43e6, 8.52e6, 8.66e6, 8.80e6, 8.88e6, 8.99e6, 9.06e6, 9.20e6, 9.28e6, 9.75e6, 1.05e7, 1.13e7, 1.22e7, 1.36e7, 1.46e7, 1.83e7, 2.14e7, 3.50e7, 6.28e7, 1.11e8, 4.23e8  },
          std::vector<double>{  -24.99, -22.55, -21.15, -20.81, -20.86, -21.15, -21.70, -22.61, -22.16, -23.10, -21.82, -24.11, -22.09, -24.11, -21.91, -23.78, -21.50, -23.63, -23.80, -22.07, -22.14, -22.21, -21.36, -21.04, -21.01, -20.68, -20.60, -20.65, -20.59, -20.70, -20.81, -21.28, -21.79, -23.05  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/        
            relation::get_sigmoid_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                -0.00022, 0.00063, 0.00000, 0.00000, 1.11067, 1268.80242, -836.03510, 0.17582,
                204.36, 1004.4, 0.0, 3.0, 0.033), 
                // oxygen, mean error: 0.7%, max error: 3.3%, range: 204.36-1004.4K, 0-3MPa, stp estimate: 0.907
        /*thermal_conductivity*/   
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00023, 1.29450, 0.00018, 0.88149, -0.00130,
                 104.36, 1004.4,  0.0, 3.0, 0.042), 
                // oxygen, mean error: 0.8%, max error: 4.2%, range: 104.36-1004.4K, 0-3MPa, stp estimate: 0.025
        /*dynamic_viscosity*/      
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                -0.08909, 0.00000, 0.39475, 0.70840, -2.08620,
                 104.36, 1004.4,  0.0, 3.0, 0.081), 
                // oxygen, mean error: 2.1%, max error: 8.1%, range: 104.36-1004.4K, 0-3MPa, stp estimate: 18.827
        /*density*/                missing(),
        /*refractive_index*/       1.0002709,
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     constexpr double n = 1.0002709f;
        //     constexpr double dndl = -0.00027966;
        //     return n + dndl * l;
        // }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 1.699 * si::joule / (si::gram * si::kelvin),             
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (31.9988 * si::kilogram * si::kelvin), 
                175430.0, -6152.3, 113.92, -0.92382, 0.0027963,
                4.36, 142.0), 
        /*thermal_conductivity*/   // 0.1514 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.2741, -0.00138, 0.0,  0.0,  0.0,
                  60, 150), // 60-150K
        /*dynamic_viscosity*/      // 188.0 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -4.1476, 94.04, -1.207, 0.0, 0.0,
                  54.36, 150), // 54.36-150K
        /*density*/                // 1.141 * si::gram/si::milliliter,
            // field::StateSample<si::density>(0.6779*si::gram/si::centimeter3, si::atmosphere, 149.8*si::kelvin), // Johnson (1960)
            get_dippr_temperature_function_105
                (si::kelvin, 31.9988 * si::gram/si::decimeter3,
                 3.9143, 0.28772, 154.58, 0.2924,
                 54.35, 154.58), // 54.35-154.58K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 51.245, -1200.2, -6.4361, 0.028405, 1.0,
                  54.36, 154.58), // 54.36-154.58K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 154.59, 0.03843, 1.225, 0.0, 0.0, 0.0, 0.0,
                 55.7, 152.08), // Mulero (2012)
        /*refractive_index*/       1.2243,
        /*extinction_coefficient*/ missing()
    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        // gamma, a.k.a "Solid I"
        phase::PartlyKnownSolid { 
            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (31.9988*si::gram * si::kelvin),
                      std::vector<double>{45.90, 54.39},
                      std::vector<double>{11.02, 11.06}), // Johnson (1960)
            /*thermal_conductivity*/              state_invariant(0.17 * si::watt / (si::centimeter * si::kelvin)),  // Jezowski (1993)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{1.396, 1.390, 1.383, 1.377, 1.370}), // Prokhvatilov
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-219.1, -210.6, -198.8, -183.1 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{24.76, 24.27, 23.81, 23.33, 22.84}), // Prokhvatilov
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{ 7.55,  7.48,  7.44,  7.37,  7.30}), // Prokhvatilov
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{ 2.61,  2.58,  2.57,  2.55,  2.52}), // Prokhvatilov
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 44.0,  46.0,  48.0,  50.0,  52.0}, 
                     std::vector<double>{0.449, 0.449, 0.448, 0.447, 0.447}), // Prokhvatilov

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           state_invariant(0.31 * si::megapascal), // Bates 1955, @ 1atm, 45.0K
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        },
        // beta, a.k.a. "Solid II"
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (31.9988*si::gram * si::kelvin),
                      std::vector<double>{25.02, 42.21},
                      std::vector<double>{ 5.42, 10.73}), // Johnson (1960)
            /*thermal_conductivity*/              state_invariant(0.17 * si::watt / (si::centimeter * si::kelvin)),  // Jezowski (1993)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{1.521, 1.513, 1.510, 1.505, 1.501, 1.496, 1.490, 1.485, 1.475}), // Prokhvatilov
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-219.1, -210.6, -198.8, -183.1 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{33.67, 32.93, 32.78, 32.42, 31.92, 31.06, 30.70, 29.97, 29.22}), // Prokhvatilov
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{15.84, 18.33, 19.62, 20.85, 21.92, 23.04, 23.79, 24.34, 23.41}), // Prokhvatilov
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{ 5.57,  6.51,  7.00,  7.48,  7.91,  8.37,  8.68,  8.92,  8.56}), // Prokhvatilov
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{0.422, 0.407, 0.400, 0.393, 0.385, 0.376, 0.371, 0.365, 0.366}), // Prokhvatilov

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           
                field::StateFunction<si::pressure>([](si::pressure p, si::temperature T){ 
                    return math::mix(1.4, 2.7, math::linearstep(40.0, 30.0, T/si::kelvin))*si::megapascal;
                }), // Bates 1955
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        },
        // alpha
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (31.9988*si::gram * si::kelvin),
                      std::vector<double>{0.0,          2.0, 12.97, 22.24},
                      std::vector<double>{0.0, 11.7/31.9988,  1.10,  4.40}), // Johnson (1960)
            /*thermal_conductivity*/              state_invariant(0.17 * si::watt / (si::centimeter * si::kelvin)),  // Jezowski (1993)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           //1524.0 * si::kilogram/si::meter3,
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 24.5,  28.0,  30.0,  32.0,  34.0,  36.0,  38.0,  40.0,  43.0}, 
                     std::vector<double>{1.537, 1.536, 1.535, 1.535, 1.534, 1.532, 1.531, 1.528, 1.525}), // Prokhvatilov
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-219.1, -210.6, -198.8, -183.1 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  4.0,   7.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  23.5}, 
                     std::vector<double>{35.75, 35.73, 35.56, 35.47, 35.29, 35.17, 35.01, 34.74, 34.41, 34.19}), // Prokhvatilov
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  4.0,   7.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  23.5}, 
                     std::vector<double>{34.54, 33.47, 32.38, 31.44, 30.50, 29.32, 27.77, 25.66, 21.85, 15.14}), // Prokhvatilov
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  4.0,   7.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  23.5}, 
                     std::vector<double>{ 12.9, 12.45, 12.01, 11.63, 11.25, 10.77, 10.16,  9.32,  7.84,  5.31}), // Prokhvatilov
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{  4.0,   7.0,  10.0,  12.0,  14.0,  16.0,  18.0,  20.0,  22.0,  23.5}, 
                     std::vector<double>{0.345, 0.344, 0.348, 0.352, 0.356, 0.361, 0.368, 0.377, 0.394, 0.426}), // Prokhvatilov

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           
                get_interpolated_temperature_function
                    (si::celcius, si::megapascal,
                     std::vector<double>{30.0, 18.0,  4.0}, 
                     std::vector<double>{2.70, 4.46, 3.49}), // Bates 1955
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);

// carbon dioxide, CO2
// for the atmospheres earth, venus, and mars, as well as the ice caps of mars
// I am super proud of our coverage for absorption cross sections: 
// 2.5nm to 1mm with only one gap in Vis (at the 2.5e6 m^-1 mark),
// which we fill on the assumption it is invisible there.
// Truly only the best for such a common and highly influential gas!
PartlyKnownCompound carbon_dioxide (
    /*molar_mass*/                        44.01 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                346.9 * si::picometer,                                    // Mehio (2014)
    /*molecular_degrees_of_freedom*/      6.96,
    /*acentric_factor*/                   0.228,

    /*critical_point_pressure*/           7.375 * si::megapascal,
    /*critical_point_volume*/             94.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        304.13 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       205.0 * si::joule / si::gram,
    /*latent_heat_of_fusion*/             9.02*si::kilojoule/(44.01*si::gram),
    /*triple_point_pressure*/             517e3 * si::pascal,
    /*triple_point_temperature*/          216.56 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -56.56 * si::celcius,
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             
    get_simon_glatzel_phase_function(
            /*triple_point_pressure*/       517e3 * si::pascal,
            /*triple_point_temperature*/    216.56 * si::kelvin,
            /*critical_point_pressure*/     7.375 * si::megapascal,
            /*critical_point_temperature*/  304.13 * si::kelvin,
            /*latent_heat_of_vaporization*/ 205.0 * si::joule / si::gram,
            /*molar_mass*/                  44.01 * si::gram/si::mole,
            /*simon_glatzel_slope*/         3.3080e-4 * si::bar,
            /*simon_glatzel_intercept*/     -3355.42 * si::bar,
            /*simon_glatzel_exponent*/      3.0
        ),

    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{ 8.58e2,  8.58e3,  2.75e4,  3.26e4,  3.78e4,  6.70e4,  8.58e4,  9.53e4,  1.00e5,  1.07e5,  1.11e5,  1.18e5,  1.27e5,  1.32e5,  1.36e5,  1.45e5,  1.53e5,  1.56e5,  1.60e5,  1.66e5,  1.69e5,  1.73e5,  1.87e5,  1.92e5,  2.00e5,  2.23e5,  2.32e5,  2.37e5,  2.40e5,  2.47e5,  2.53e5,  2.60e5,  2.69e5,  2.76e5,  2.88e5,  3.10e5,  3.41e5,  3.59e5,  3.72e5,  3.90e5,  4.30e5,  5.01e5,  5.74e5,  6.20e5,  6.57e5,  6.99e5,  8.13e5,  9.54e5,  1.07e6,  1.29e6,  1.32e6,  2.50e6,  4.95e6,  5.00e6,  6.14e6,  6.73e6,  7.17e6,  7.62e6,  8.39e6,  8.90e6,  9.21e6,  1.11e7,  1.21e7,  1.29e7,  1.52e7,  2.91e7,  4.65e7,  1.26e8,  1.89e8,  2.41e8,  4.42e8 },
          std::vector<double>{ -33.32,  -35.77,  -38.01,  -37.15,  -32.27,  -22.83,  -28.57,  -26.72,  -28.50,  -26.46,  -30.22,  -31.21,  -28.04,  -30.02,  -28.11,  -32.47,  -32.60,  -33.79,  -36.49,  -37.22,  -35.11,  -32.07,  -28.17,  -26.66,  -28.64,  -25.03,  -22.21,  -21.30,  -26.74,  -27.46,  -30.28,  -28.25,  -31.00,  -29.30,  -32.57,  -29.89,  -28.05,  -24.31,  -23.66,  -28.38,  -32.11,  -24.84,  -31.52,  -26.54,  -30.21,  -26.02,  -31.13,  -28.70,  -37.36,  -29.75,  -35.98,  -35.98,  -29.16,  -27.60,  -22.96,  -22.26,  -22.30,  -22.01,  -23.52,  -20.24,  -21.06,  -20.04,  -20.86,  -20.36,  -20.45,  -20.56,  -20.83,  -21.74,  -22.21,  -21.69,  -21.96 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 36.61 * si::joule / (44.01 * si::gram * si::kelvin),             // wikipedia
            relation::get_sigmoid_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.00034, 3.89562, 0.00000, 0.00000, 3.24445, 600.78481, -562.26128, -1.80573,
                266.59, 1116.6, 0.0, 3.0, 0.028), 
                // carbon dioxide, mean error: 0.8%, max error: 2.8%, range: 266.59-1116.6K, 0-3MPa, stp estimate: 0.828
        /*thermal_conductivity*/   // 0.01662 * si::watt / ( si::meter * si::kelvin ),                 // wikipedia
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00032, 1.14102, 0.00027, 0.83072, -0.01434,
                 266.59, 1116.6,  0.0, 3.0, 0.027), 
                // carbon dioxide, mean error: 0.8%, max error: 2.7%, range: 266.59-1116.6K, 0-3MPa, stp estimate: 0.015
        /*dynamic_viscosity*/      // 1.47e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.05967, 1.75501, 0.18576, 0.79011, -1.65245,
                 266.59, 1116.6,  0.0, 3.0, 0.032), 
                // carbon dioxide, mean error: 1.5%, max error: 3.2%, range: 266.59-1116.6K, 0-3MPa, stp estimate: 13.980
        /*density*/                missing(),
        /*refractive_index*/       1.0004493
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     double invl2 = 1.0/(l*l);
        //     return 1.0
        //         + 6.99100e-2/(166.175   - invl2)
        //         + 1.44720e-3/(79.609    - invl2)
        //         + 6.42941e-5/(56.3064   - invl2)
        //         + 5.21306e-5/(46.0196   - invl2)
        //         + 1.46847e-6/(0.0584738 - invl2);
        // }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (44.0095 * si::kilogram * si::kelvin), 
                -8304300.0, 104370.0, -433.33, 0.60052, 0.0,
                220, 290), 
        /*thermal_conductivity*/   // 0.087 * si::watt / (si::meter * si::kelvin),                     // wikipedia
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.4406, -0.0012175, 0.0,  0.0,  0.0,
                  216.58, 300), 
        /*dynamic_viscosity*/      // 0.0712 * si::millipascal*si::second,                             // wikipedia data page
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 18.775, -402.92, -4.6854, -6.9171e-26, 10.0,
                  216.58, 303.15), // 216.58-303.15K
        /*density*/                // 1101.0 * si::kilogram/si::meter3,
            get_dippr_temperature_function_105
                (si::kelvin, 44.01 * si::gram/si::decimeter3,
                 2.768, 0.26212, 304.21, 0.2908,
                 216.58, 304.21), // 216.58-304.21K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 47.0169, -2839.0, -3.86388, 2.81e-16, 6.0,
                  216.58, 304.21), // 216.58-304.21K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 304.13, 0.07863, 1.254, 0.0, 0.0, 0.0, 0.0,
                 216.55, 304.11), // Mulero (2012)
        /*refractive_index*/       1.6630,
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/            
                get_interpolated_temperature_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                     std::vector<double>{0.0,   3.0,  10.0, 40.0, 70.0, 200.0, 215.0}, 
                     std::vector<double>{0.0,0.0151,0.5883,19.64,33.38, 56.77, 61.90}), // Manzhelii (1997)
            /*thermal_conductivity*/              
                get_interpolated_temperature_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{ 25.0, 40.0, 50.0, 80.0, 120.0, 210.0}, 
                     std::vector<double>{59.17,27.32,20.24,11.68,  7.79,  4.19}), // Manzhelii (1997)
            /*dynamic_viscosity*/                 state_invariant(1e14 * si::pascal*si::second), // Yamashita (1997) @1 bar, 180K
            /*density*/                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{1.7056,1.7036,1.6963,1.6852,1.6718,1.6568,1.6403,1.6223,1.6022,1.5789}), // Prokhvatilov
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-134.3, -119.5, -100.2, -78.2  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer,1.0,
                     std::vector<double>{  0.051,   0.055,   0.060,   0.064,   0.068,   0.078,   0.086,   0.091,   0.098,   0.103,   0.110,   0.116,   0.120,   0.129,   0.142,   0.168,   0.238,   0.437,   0.985,   0.989,   3.235,   3.838,   4.095,   4.189,   4.269,   4.221,   4.302,   4.384,   4.606,   5.322,   9.887,   9.628,  13.141,  14.284,  14.837,  14.724,  15.528,  17.137,  23.390,  70.273,  81.169,  85.596,  90.265,  98.121, 134.942, 146.687, 150.064, 158.249, 180.034, 948.278}, 
                     std::vector<double>{   0.51,    0.51,    0.79,    0.88,    0.88,    0.70,    0.93,    0.79,    1.03,    0.93,    1.06,    0.79,    3.79,    2.32,    1.82,    1.60,    1.47,    1.40,    1.38,    1.41,    1.40,    1.36,    1.29,    0.98,    0.45,    3.02,    1.81,    1.54,    1.46,    1.42,    1.40,    1.41,    1.36,    1.26,    0.39,    3.21,    1.57,    1.46,    1.43,    1.41,    1.38,    1.34,    1.48,    1.45,    1.41,    1.33,    1.57,    1.48,    1.45,    1.43}), // Warren (1986)
            /*extinction_coefficient*/
                compound::relation::get_spectral_linear_interpolation_function_of_wavelength
                    (si::micrometer,1.0,
                     std::vector<double>{  0.051,   0.054,   0.057,   0.067,   0.072,   0.082,   0.086,   0.092,   0.097,   0.105,   0.109,   0.116,   0.123,   0.125,   0.351,   0.482,   0.993,   1.004,   2.443,   2.556,   2.654,   2.684,   2.715,   2.756,   2.756,   2.862,   3.028,   3.086,   3.253,   3.302,   3.327,   3.588,   3.754,   4.017,   4.109,   4.140,   4.219,   4.299,   4.315,   4.464,   4.481,   4.814,   4.980,   5.075,   5.132,   5.289,   5.661,   6.173,   6.483,   6.682,   6.912,   7.097,   7.259,   7.481,   7.595,   7.946,   8.128,   8.408,   8.536,   8.797,   9.101,   9.450,   9.703,   9.925,  10.076,  10.620,  11.537,  11.979,  12.345,  12.819,  13.820,  14.350,  14.901,  15.356,  15.589,  16.309,  19.101,  22.203,  23.938,  24.856,  79.194,  87.333, 100.000, 137.169, 146.780, 160.651}, 
                     std::vector<double>{1.2e-01, 3.1e-01, 5.0e-01, 3.4e-01, 3.4e-01, 7.0e-01, 5.4e-01, 8.5e-01, 7.3e-01, 1.0e+00, 1.1e+00, 2.7e+00, 2.2e-01, 9.9e-03, 6.6e-07, 7.1e-07, 2.2e-06, 2.2e-06, 7.7e-06, 1.2e-05, 2.5e-02, 8.0e-04, 1.5e-03, 2.4e-02, 1.4e-03, 3.8e-05, 8.4e-05, 8.7e-05, 3.3e-05, 5.6e-05, 2.2e-05, 2.4e-06, 2.8e-06, 1.6e-02, 7.7e-03, 1.3e-02, 2.3e+00, 8.1e-02, 1.3e-02, 3.7e-03, 1.1e-03, 8.4e-05, 3.7e-05, 1.7e-04, 3.9e-05, 1.5e-05, 5.0e-06, 2.9e-06, 5.0e-06, 7.6e-05, 1.1e-04, 4.0e-05, 6.2e-05, 1.2e-04, 6.9e-05, 2.4e-05, 3.7e-05, 2.2e-05, 1.0e-05, 3.7e-06, 4.4e-06, 1.1e-05, 7.2e-06, 7.2e-06, 1.1e-05, 2.2e-05, 7.5e-03, 4.6e-03, 4.9e-03, 1.2e-02, 2.3e-03, 7.1e-03, 2.0e+00, 9.5e-02, 2.1e-02, 4.3e-03, 3.9e-04, 1.5e-04, 1.3e-04, 1.6e-04, 3.1e-03, 1.2e-01, 2.2e-03, 3.7e-03, 1.9e-01, 3.5e-03}), // Warren (1986)
            /*absorption_coefficient*/            missing(),
            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{79.73, 79.06, 78.53, 76.16, 72.73, 70.12, 67.67, 63.46, 58.25, 52.45}), // Prokhvatilov
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{111.7,111.45,110.66,109.09,106.22,102.60, 97.72, 91.48, 84.14, 75.37}), // Prokhvatilov
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 15.0,  30.0,  50.0,  70.0,  90.0, 110.0, 130.0, 150.0, 170.0, 190.0}, 
                     std::vector<double>{44.10, 44.05, 43.81, 43.25, 42.26, 40.84, 38.80, 36.31, 33.41, 29.89}), // Prokhvatilov
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

// methane, CH4
// for the atmosphere of Titan, and surfaces of pluto or other kuiper belt objects
// The absorption cross section graph for CO2 also included CH4, so CH4 has good coverage as well: 
// 2nm to 1mm with virtually zero gaps or overlaps between sources! Can you believe this? This thing predicts the color of Neptune!
PartlyKnownCompound methane (
    /*molar_mass*/                        16.043 * si::gram/si::mole,
    /*atoms_per_molecule*/                5u,
    /*molecular_diameter*/                404.6 * si::picometer,                                    // Mehio (2014)
    /*molecular_degrees_of_freedom*/      5.1,
    /*acentric_factor*/                   0.011,

    /*critical_point_pressure*/           4.60 * si::megapascal,
    /*critical_point_volume*/             99.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        190.56 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       510.83 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             0.94*si::kilojoule/(16.043*si::gram),
    /*triple_point_pressure*/             11.696 * si::kilopascal,
    /*triple_point_temperature*/          90.694 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -182.46 * si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -161.4 * si::celcius, // Perry
    
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

                // triple point between solid, liquid, and gas
                const si::pressure        p0 = 11.696 * si::kilopascal;
                const si::temperature     T0 = 90.694 * si::kelvin;
                // critical point
                const si::pressure        pc = 4.60 * si::megapascal;
                const si::temperature     Tc = 190.56 * si::kelvin;
                // latent heat of vaporization and molar mass
                const si::specific_energy L  = 510.83 * si::joule/si::gram;
                const si::molar_mass      M  = 16.043 * si::gram/si::mole;
                // Simon-Glatzel constants
                const si::pressure        a  = 2080e5 * si::pascal;
                const si::pressure        b  = 0.0 * si::pascal;
                const double              c  = 1.698;

                const int basic_phase = property::get_simon_glatzel_phase(p, T, p0, T0, pc, Tc, L,  M, a, b, c);
                if (basic_phase < 0)
                {
                    return basic_phase;
                } 
                else if ( T > 20.0 * si::kelvin )
                {
                    return alpha;
                }
                else
                {
                    return beta;
                }
            }
        ),

    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  9.47e5, 9.92e5, 1.02e6, 1.05e6, 1.07e6, 1.11e6, 1.13e6, 1.14e6, 1.16e6, 1.17e6, 1.19e6, 1.21e6, 1.25e6, 1.28e6, 1.33e6, 1.38e6, 1.40e6, 1.43e6, 1.44e6, 1.48e6, 1.50e6, 1.57e6, 1.61e6, 1.70e6, 1.73e6, 1.80e6, 1.84e6, 1.91e6, 1.96e6, 2.03e6, 2.05e6, 2.10e6, 2.15e6, 2.17e6, 2.25e6, 2.28e6, 2.30e6, 6.56e6, 7.00e6, 7.66e6, 8.65e6, 1.08e7, 2.83e7, 4.90e7, 1.82e8, 2.23e8, 2.38e8, 2.57e8, 3.12e8, 3.24e8, 3.61e8, 3.94e8, 4.45e8, 4.89e8, 5.15e8, 5.95e8, 6.36e8, 7.38e8, 7.91e8, 8.66e8, 9.40e8  },
          std::vector<double>{  -28.89, -27.28, -27.50, -29.05, -29.13, -27.31, -27.03, -28.16, -27.76, -28.91, -28.60, -30.96, -28.43, -28.67, -29.96, -27.88, -29.30, -28.99, -29.93, -29.91, -29.27, -30.48, -28.66, -31.16, -29.91, -31.27, -29.36, -31.86, -30.34, -32.07, -29.75, -31.26, -31.40, -30.69, -32.07, -30.48, -31.86, -27.37, -22.93, -20.76, -20.71, -20.22, -20.83, -21.53, -22.69, -29.75, -23.23, -21.93, -25.62, -29.16, -30.28, -28.11, -26.80, -30.93, -22.60, -27.39, -31.20, -27.26, -33.23, -27.72, -33.23  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 2.218 * si::joule / (si::gram * si::kelvin),                     
            relation::get_sigmoid_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.02624, 1.04690, 0.00000, 0.00000, 1.04105, 264.87732, 492.16481, 2.86235,
                140.69, 640.69, 0.0, 3.0, 0.021), 
                // methane, mean error: 0.7%, max error: 2.1%, range: 140.69-640.69K, 0-3MPa, stp estimate: 2.201
        /*thermal_conductivity*/   // 34.4 * si::milliwatt / ( si::meter * si::kelvin ),               // Huber & Harvey         
            relation::get_sigmoid_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00081, 1.07534, 0.00000, 0.00000, 0.28162, 818.25156, 1044.83566, 0.22399,
                140.69, 640.69, 0.0, 3.0, 0.032), 
                // methane, mean error: 1.1%, max error: 3.2%, range: 140.69-640.69K, 0-3MPa, stp estimate: 0.031
        /*dynamic_viscosity*/      // 1.10e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.12223, 1.53060, 0.09521, 0.83600, -0.21904,
                 140.69, 640.69,  0.0, 3.0, 0.052), 
                // methane, mean error: 1.9%, max error: 5.2%, range: 140.69-640.69K, 0-3MPa, stp estimate: 10.148
        /*density*/                missing(),
        /*refractive_index*/       // 1.000444,
            compound::relation::get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_input
                (si::micrometer, 1.0,
                 std::vector<double>{    1.67,     2.70,     3.01,     3.66,     3.79,     4.46,     5.66,     6.51,     7.00,     8.38,     8.95,    10.09,    10.86,    11.54,    11.93,    12.37,    13.22,    13.63,    14.02,    14.83},
                 std::vector<double>{1.000430, 1.000425, 1.000417, 1.000440, 1.000437, 1.000431, 1.000427, 1.000419, 1.000402, 1.000466, 1.000451, 1.000445, 1.000442, 1.000443, 1.000440, 1.000441, 1.000440, 1.000439, 1.000444, 1.000439}),
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 3.481 * si::joule / (si::gram * si::kelvin),              
            get_dippr_liquid_heat_capacity_temperature_function_114
                (190.56 * si::kelvin, si::joule / (16.0425 * si::kilogram * si::kelvin), 
                65.708, 38883.0, -257.95, 614.07, 0.0,
                90.69*si::kelvin, 190.0*si::kelvin), 
        /*thermal_conductivity*/   // 0.1931 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.41768, -0.0024528, 3.5588e-6,  0.0,  0.0,
                  90.69, 180), 
        /*dynamic_viscosity*/      // 118.6 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -6.1572, 178.15, -0.95239, -9.0606e-24, 10.0,
                  90.69, 188), // 90.69-188K
        /*density*/                
            get_dippr_temperature_function_105
                (si::kelvin, 16.043 * si::gram/si::decimeter3,
                 2.9214, 0.28976, 190.56, 0.28881,
                 90.69, 190.56), // 90.69-190.56K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 39.205, -1324.4, -3.4366, 0.000031019, 2.0,
                  90.69, 190.56), // 90.69-190.56K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 190.56, 0.03825, 1.191, -0.006024, 5.422, -0.0007065, 0.6161,
                 90.67, 188.84), // Mulero (2012)
        /*refractive_index*/       1.2730, 
        /*extinction_coefficient*/ missing()
    },



    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid { // alpha

            /*isobaric_specific_heat_capacity*/ 
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (16.043*si::gram * si::kelvin),
                      std::vector<double>{21.35, 61.40, 87.20},
                      std::vector<double>{4.325, 8.673, 10.19}), // Johnson (1960)
            /*thermal_conductivity*/              // 0.010 * si::watt / (si::centimeter * si::kelvin), // Jezowski (1997)
                get_interpolated_temperature_function
                    (si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{18.0, 20.0, 20.4, 21.0},              
                     std::vector<double>{0.7,  2.5,  11.0,  0.7}), // Johnson (1960)
            /*dynamic_viscosity*/                 state_invariant(1e11 * si::pascal*si::second), // Yamashita (1997), @ 0.1*si::megapascal, 77.0*si::kelvin
            /*density*/                           state_invariant(0.5245*si::gram/si::centimeter3), // Prokhvatilov, @ si::atmosphere, 14.4*si::kelvin
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::kelvin, si::kilopascal,
                      std::vector<double>{65.0,     75.0,     85.0},     
                      std::vector<double>{0.1 ,      0.8 ,     4.9 }),
            /*refractive_index*/                  1.3219,
            /*extinction_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber
                    (1.0/si::centimeter, 1.0,
                     std::vector<double>{ 4017.56,  3126.23,  3042.81,  3012.07,  2994.51,  2959.38,  1365.53,  1321.62,  1299.67,  1282.10, 1251.37 },
                     std::vector<double>{   1.503,    1.503,    1.528,    1.672,    1.538,    1.503,    1.506,    1.522,    2.095,    1.519,    1.503 }),
            /*absorption_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    (1.0/si::centimeter, 1.0/si::centimeter,
                     std::vector<double>{ 10653.5, 10534.7, 10445.5, 10297.0, 10217.8, 10108.9, 10059.4, 10029.7,  9960.4,  9871.3,  9792.1,  9752.5,  9673.3,  9623.8,  9554.5,  9435.6,  9376.2,  9148.5,  9039.6,  8990.1,  8901.0,  8851.5,  8802.0,  8703.0,  8594.1,  8455.4,  8396.0,  8346.5,  8306.9,  8257.4,  8227.7,  8128.7,  8089.1,  8069.3,  8039.6,  8009.9,  7950.5,  7861.4,  7811.9,  7623.8,  7495.0,  7415.8,  7356.4,  7277.2,  7237.6,  7217.8,  7148.5,  7128.7,  7059.4,  7009.9,  6960.4,  6871.3,  6792.1,  6732.7,  6703.0,  6643.6,  6584.2,  6524.8,  6287.1,  6059.4,  5980.2,  5940.6,  5881.2,  5782.2,  5742.6,  5673.3,  5554.5,  5514.9,  5465.3,  5376.2,  5277.2,  5168.3,  5108.9,  5039.6,  4811.9,  4594.1,  4524.8,  4475.2,  4356.4,  4316.8,  4247.5,  4207.9,  4148.5,  4079.2,  4039.6,  3920.8,  3851.5,  3772.3,  3475.2,  3188.1,  3029.7,  2940.6,  2841.6,  2752.5,  2623.8,  2505.0 },
                     std::vector<double>{  -2.134,  -1.866,  -1.966,  -0.978,  -1.514,  -0.642,  -1.078,  -0.844,  -1.346,  -0.693,  -1.698,  -1.430,  -1.564,  -1.950,  -2.218,  -2.084,  -2.101,  -1.749,  -1.045,  -1.246,  -0.173,  -0.391,   0.246,  -0.743,   0.547,  -0.994,  -0.190,  -0.860,  -0.542,  -1.263,  -1.749,  -2.050,  -1.447,  -1.749,  -1.581,  -2.117,  -2.335,  -2.067,  -2.134,  -1.179,   0.682,  -0.391,   0.330,   0.296,  -0.089,   0.145,  -0.039,   0.430,   0.313,  -0.609,  -0.927,  -0.609,  -1.279,  -0.324,  -1.849,  -2.034,  -1.782,  -2.168,  -1.028,   0.732,   1.335,   0.229,   0.078,   1.101,  -0.123,   0.061,   0.933,  -0.458,  -1.112,   0.011,  -1.715,  -0.642,  -0.911,  -1.615,  -0.642,   0.547,   1.419,   0.782,   1.419,   2.257,   1.134,   2.374,   0.430,   0.782,  -0.358,   0.464,   1.570,  -1.598,  -0.508,   1.369,   3.715,   1.268,   1.905,   0.196,   1.184,  -1.061 }),


            /*bulk_modulus*/                      state_invariant(19.69 * si::kilobar), // Prokhvatilov, @ si::standard_pressure, 14.4*si::kelvin
            /*tensile_modulus*/                   state_invariant(30.83 * si::gigapascal), // Manzhelii (2003), @ si::standard_pressure, 30.0*si::kelvin
            /*shear_modulus*/                     state_invariant(12.4*si::kilobar), // Prokhvatilov, @ si::standard_pressure, 14.4*si::kelvin
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     state_invariant(0.39), // Prokhvatilov, @ si::standard_pressure, 14.4*si::kelvin

            /*compressive_fracture_strength*/     8.0 * si::megapascal, // Yamashita (2010)
            /*tensile_fracture_strength*/         
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  10.0,  15.0,  20.0,  25.0,  30.0}, 
                     std::vector<double>{180.0, 178.0, 176.0, 174.0, 170.0, 165.0}), // Prokhvatilov
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  10.0,  15.0,  20.0,  25.0,  30.0}, 
                     std::vector<double>{178.0, 159.0, 145.0, 137.0, 135.0, 117.0}), // Prokhvatilov
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        },
        phase::PartlyKnownSolid { // beta
            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (16.043*si::gram * si::kelvin),
                      std::vector<double>{0.0, 10.33, 15.48, 19.94},
                      std::vector<double>{0.0, 0.954, 2.774, 10.91}), // Johnson (1960)
            /*thermal_conductivity*/              // 0.010 * si::watt / (si::centimeter * si::kelvin), // Jezowski (1997)
                get_interpolated_temperature_function
                    (si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{18.0, 20.0, 20.4, 21.0},              
                     std::vector<double>{0.7,  2.5,  11.0,  0.7}), // Johnson (1960)
            /*dynamic_viscosity*/                 state_invariant(1e11 * si::pascal*si::second), // Yamashita (1997), @ 0.1*si::megapascal, 77.0*si::kelvin
            /*density*/                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  25.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{0.5228,0.5278,0.5136,0.5087,0.5031,0.4969}), // Prokhvatilov
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::kelvin, si::kilopascal,
                      std::vector<double>{65.0,     75.0,     85.0},     
                      std::vector<double>{0.1 ,      0.8 ,     4.9}),
            /*refractive_index*/                  1.3219,
            /*extinction_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber
                    (1.0/si::centimeter, 1.0,
                     std::vector<double>{ 4017.56,  3126.23,  3042.81,  3012.07,  2994.51,  2959.38,  1365.53,  1321.62,  1299.67,  1282.10, 1251.37 },
                     std::vector<double>{   1.503,    1.503,    1.528,    1.672,    1.538,    1.503,    1.506,    1.522,    2.095,    1.519,    1.503 }),
            /*absorption_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    (1.0/si::centimeter, 1.0/si::centimeter,
                     std::vector<double>{ 10653.5, 10534.7, 10445.5, 10297.0, 10217.8, 10108.9, 10059.4, 10029.7,  9960.4,  9871.3,  9792.1,  9752.5,  9673.3,  9623.8,  9554.5,  9435.6,  9376.2,  9148.5,  9039.6,  8990.1,  8901.0,  8851.5,  8802.0,  8703.0,  8594.1,  8455.4,  8396.0,  8346.5,  8306.9,  8257.4,  8227.7,  8128.7,  8089.1,  8069.3,  8039.6,  8009.9,  7950.5,  7861.4,  7811.9,  7623.8,  7495.0,  7415.8,  7356.4,  7277.2,  7237.6,  7217.8,  7148.5,  7128.7,  7059.4,  7009.9,  6960.4,  6871.3,  6792.1,  6732.7,  6703.0,  6643.6,  6584.2,  6524.8,  6287.1,  6059.4,  5980.2,  5940.6,  5881.2,  5782.2,  5742.6,  5673.3,  5554.5,  5514.9,  5465.3,  5376.2,  5277.2,  5168.3,  5108.9,  5039.6,  4811.9,  4594.1,  4524.8,  4475.2,  4356.4,  4316.8,  4247.5,  4207.9,  4148.5,  4079.2,  4039.6,  3920.8,  3851.5,  3772.3,  3475.2,  3188.1,  3029.7,  2940.6,  2841.6,  2752.5,  2623.8,  2505.0 },
                     std::vector<double>{  -2.134,  -1.866,  -1.966,  -0.978,  -1.514,  -0.642,  -1.078,  -0.844,  -1.346,  -0.693,  -1.698,  -1.430,  -1.564,  -1.950,  -2.218,  -2.084,  -2.101,  -1.749,  -1.045,  -1.246,  -0.173,  -0.391,   0.246,  -0.743,   0.547,  -0.994,  -0.190,  -0.860,  -0.542,  -1.263,  -1.749,  -2.050,  -1.447,  -1.749,  -1.581,  -2.117,  -2.335,  -2.067,  -2.134,  -1.179,   0.682,  -0.391,   0.330,   0.296,  -0.089,   0.145,  -0.039,   0.430,   0.313,  -0.609,  -0.927,  -0.609,  -1.279,  -0.324,  -1.849,  -2.034,  -1.782,  -2.168,  -1.028,   0.732,   1.335,   0.229,   0.078,   1.101,  -0.123,   0.061,   0.933,  -0.458,  -1.112,   0.011,  -1.715,  -0.642,  -0.911,  -1.615,  -0.642,   0.547,   1.419,   0.782,   1.419,   2.257,   1.134,   2.374,   0.430,   0.782,  -0.358,   0.464,   1.570,  -1.598,  -0.508,   1.369,   3.715,   1.268,   1.905,   0.196,   1.184,  -1.061 }),


            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  25.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{ 20.72, 20.30, 19.66, 18.82, 17.85, 17.11}), // Prokhvatilov
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  25.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{ 25.74, 24.51, 23.31, 22.38, 20.85, 19.39}), // Prokhvatilov
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  25.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{  9.96,  9.44,  8.95,  8.60,  7.99,  7.39}), // Prokhvatilov
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{  25.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{ 0.293, 0.299, 0.302, 0.302, 0.305, 0.311}), // Prokhvatilov

            /*compressive_fracture_strength*/     state_invariant(8.0 * si::megapascal), // Yamashita (2010)
            /*tensile_fracture_strength*/         
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  10.0,  15.0,  20.0,  25.0,  30.0}, 
                     std::vector<double>{180.0, 178.0, 176.0, 174.0, 170.0, 165.0}), // Prokhvatilov
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  10.0,  15.0,  20.0,  25.0,  30.0}, 
                     std::vector<double>{178.0, 159.0, 145.0, 137.0, 135.0, 117.0}), // Prokhvatilov
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);




// argon, Ar
// for the atmospheres of earth like planets
PartlyKnownCompound argon (
    /*molar_mass*/                        39.948 * si::gram/si::mole,
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                340.0 * si::picometer,                                    // wikipedia, Breck (1974)
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   0.001,

    /*critical_point_pressure*/           4.898* si::megapascal,
    /*critical_point_volume*/             75.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        150.87 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       161.0 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             5.66*si::kilojoule/(39.948*si::gram),
    /*triple_point_pressure*/             68.95 * si::kilopascal,
    /*triple_point_temperature*/          83.8058 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -189.36*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -185.7*si::celcius,

    /*phase*/                             
    get_simon_glatzel_phase_function(
            /*triple_point_pressure*/       68.95 * si::kilopascal,
            /*triple_point_temperature*/    83.8058 * si::kelvin,
            /*critical_point_pressure*/     4.898* si::megapascal,
            /*critical_point_temperature*/  150.87 * si::kelvin,
            /*latent_heat_of_vaporization*/ 161.0 * si::joule/si::gram,
            /*molar_mass*/                  39.948 * si::gram/si::mole,
            /*simon_glatzel_slope*/         2114e5 * si::pascal,
            /*simon_glatzel_intercept*/     0.0 * si::pascal,
            /*simon_glatzel_exponent*/      1.593
        ),
    
    /*molecular_absorption_cross_section*/ 1e-35*si::meter2,

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/  // 0.570 * si::joule / (si::gram * si::kelvin),        
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::kilojoule/(si::kilogram*si::kelvin),
                 -1e-8, 0.98, 2.75e-4, 1.02, 5.6e-1,
                 90.0, 1000.0,  0.1, 10.0, 0.0),             
        /*thermal_conductivity*/   // 0.016 * si::watt / ( si::meter * si::kelvin ),                   // wikipedia
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00034, 1.11244, 0.00029, 0.73962, -0.00193,
                 133.81, 683.81,  0.0, 3.0, 0.027), 
                // argon, mean error: 1.2%, max error: 2.7%, range: 133.81-683.81K, 0-3MPa, stp estimate: 0.016
        /*dynamic_viscosity*/      // 2.23e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.41072, 0.79782, 0.22045, 0.81080, -0.33331,
                 133.81, 683.81,  0.0, 3.0, 0.042), 
                // argon, mean error: 1.7%, max error: 4.2%, range: 133.81-683.81K, 0-3MPa, stp estimate: 20.565
        /*density*/                missing(),
        /*refractive_index*/       1.000281
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     double invl2 = 1.0/(l*l);
        //     return 1.0
        //         + 2.50141e-3/(91.012  - invl2)
        //         + 5.00283e-4/(87.892  - invl2)
        //         + 5.22343e-2/(214.02  - invl2);
        // })
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ //1.078 * si::joule / (si::gram * si::kelvin),              
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (39.948 * si::kilogram * si::kelvin), 
                134390.0, -1989.4, 11.043, 0.0, 0.0,
                83.78, 135.0), 
        /*thermal_conductivity*/   // 0.1232 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.1819, -0.0003176, -0.00000411, 0.0, 0.0,
                  83.78, 150), 
        /*dynamic_viscosity*/      // 252.1 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal * si::second, 
                 -8.8685, 204.29, -0.38305, -1.294e-22, 10.0,
                  83.78, 150.0), // 83.78-150.0K
        /*density*/                
            get_dippr_temperature_function_105
                (si::kelvin, 39.948 * si::gram/si::decimeter3,
                 3.8469, 0.2881, 150.86, 0.29783,
                 3.78, 150.86), // 83.78, 150.86K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 42.127, -1093.1, -4.1425, 0.000057254, 2.0,
                  83.78, 150.86), // 83.78-150.86K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 150.87, 0.037, 1.25, 0.0, 0.0, 0.0, 0.0,
                 83.82, 148.15), // Mulero (2012)
        /*refractive_index*/       1.23,
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (si::gram * si::kelvin),
                      std::vector<double>{0.0, 90.68},
                      std::vector<double>{0.0, 0.197}), // Johnson (1960)
            /*thermal_conductivity*/              
                get_interpolated_temperature_function
                    (si::kelvin, si::milliwatt / (si::centimeter * si::kelvin),
                     std::vector<double>{1.0,  8.0, 50.0},              
                     std::vector<double>{4.0, 40.0,  5.0}),  // Johnson (1960)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{1.7705,1.7641,1.7518,1.7349,1.7148,1.6919,1.6657,1.6349}), // Prokhvatilov
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::kelvin, si::kilopascal,
                     std::vector<double>{55.0,     65.0,     75.0}, 
                     std::vector<double>{0.2,      2.8,      18.7}),
            /*refractive_index*/                  1.2703,
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{26.07, 25.98, 24.72, 24.61, 23.24, 20.89, 19.62, 17.38}), // Prokhvatilov
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{39.11, 38.97, 37.07, 35.22, 33.46, 30.08, 27.08, 23.70}), // Prokhvatilov
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{15.64, 15.59, 14.83, 14.06, 13.28, 11.94, 10.66,  9.44}), // Prokhvatilov
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0}, 
                     std::vector<double>{ 0.25,  0.25,  0.25,  0.26,  0.26,  0.27,  0.27,  0.27}), // Prokhvatilov

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  20.0,  30.0,  40.0,  50.0,  60.0,  75.0}, 
                     std::vector<double>{160.0, 159.0, 155.0, 150.0, 130.0,  99.0,  38.0}), // Prokhvatilov
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  5.0,  20.0,  30.0,  40.0,  50.0,  60.0,  75.0}, 
                     std::vector<double>{151.0, 107.0,  82.0,  61.0,  49.0,  43.0,  20.0}), // Prokhvatilov
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);



// helium, He
// for the atmospheres of gas giants
PartlyKnownCompound helium (
    /*molar_mass*/                        4.0026 * si::gram/si::mole,
    /*atoms_per_molecule*/                1u,
    /*molecular_diameter*/                255.7 * si::picometer, // Mehio (2014)
    /*molecular_degrees_of_freedom*/      3.0,
    /*acentric_factor*/                   -0.39,

    /*critical_point_pressure*/           0.227 * si::megapascal,
    /*critical_point_volume*/             57.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        5.19 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       20.7 * si::joule / si::gram,
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             5.048e3 * si::pascal,
    /*triple_point_temperature*/          2.1768 * si::kelvin,
    /*freezing_point_sample_pressure*/    25.0 * si::atmosphere,
    /*freezing_point_sample_temperature*/ 0.92778 * si::kelvin,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -268.9 * si::celcius, // Perry

    /*phase*/
    field::StateFunction<int>([]
            (const si::pressure p, 
             const si::temperature T)
            {
                // const int supercritical = -3;
                // const int gas = -2;
                // const int liquid = -1;
                const int hcp = 0;
                const int fcc = 1;

                // triple point between solid, liquid, and gas
                const si::pressure        p0 = 5.048e3 * si::pascal;
                const si::temperature     T0 = 2.1768 * si::kelvin;
                // critical point
                const si::pressure        pc = 0.227 * si::megapascal;
                const si::temperature     Tc = 5.19 * si::kelvin;
                // latent heat of vaporization and molar mass
                const si::specific_energy L  = 20.7 * si::joule / si::gram;
                const si::molar_mass      M  = 4.0026 * si::gram/si::mole;
                // Simon-Glatzel constants
                const si::pressure        a  = 50.96e5 * si::pascal;
                const si::pressure        b  = 0.0 * si::pascal;
                const double              c  = 1.5602;
                const si::pressure        pab0 = 1.1 * si::kilobar;
                const si::temperature     Tab0 = 15.0 * si::kelvin;
                const si::pressure        pab1 = 3.25 * si::kilobar;
                const si::temperature     Tab1 = 20.0 * si::kelvin;
                const auto                dpdTab = (pab1-pab0) / (Tab1-Tab0);

                const int basic_phase = property::get_simon_glatzel_phase(p, T, p0, T0, pc, Tc, L,  M, a, b, c);
                if (basic_phase < 0)
                {
                    return basic_phase;
                } 
                else if ( p > dpdTab * (T-Tab0) + pab0 )
                {
                    return hcp;
                }
                else
                {
                    return fcc;
                }
            }
        ),

    /*molecular_absorption_cross_section*/ 1e-35* si::meter2,

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 9.78 * si::joule / (si::gram * si::kelvin), 
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::kilojoule/(si::kilogram*si::kelvin),
                 -1e-6, 0.95, 1.86e-2, 1.02, 5.7e-1,
                 20.0, 1000.0,  0.1, 10.0, 0.0),             
        /*thermal_conductivity*/   // 155.7 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                -0.15319, 1.77283, 0.00284, 0.70030, 0.00337,
                 52.177, 1502.2,  0.0, 0.1, 0.041), 
                // helium, mean error: 0.8%, max error: 4.1%, range: 52.177-1502.2K, 0-0.1MPa, stp estimate: 0.145
        /*dynamic_viscosity*/      // 1.96e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                -0.19093, 1.50359, 0.28815, 0.73057, 1.34631,
                 52.177, 1502.2,  0.0, 0.1, 0.007), 
                // helium, mean error: 0.1%, max error: 0.7%, range: 52.177-1502.2K, 0-0.1MPa, stp estimate: 18.702
        /*density*/                missing(),
        /*refractive_index*/       1.000036
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     double invl2 = 1.0/(l*l);
        //     return 1.0
        //         + 0.01470091/(423.98  - invl2);
        // }) // Mansfield (1969)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 4.545 * si::joule / (si::gram * si::kelvin), 
            get_interpolated_temperature_function
                ( si::kelvin, si::joule/(si::gram * si::kelvin),
                  std::vector<double>{3.0,3.5, 4.0, 4.5, 5.0  },
                  std::vector<double>{2.6,3.41,4.52,6.78,20.24}),
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 -0.013833, 0.022913, -0.0054872, 0.0004585, 0.0,
                  2.2, 4.8), 
        /*dynamic_viscosity*/      // 3.57 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -9.6312, -3.841, -1.458, -1.065e-8, 10.0,
                  2.2, 5.1), // 2.2-5.1K
        /*density*/                
            get_dippr_temperature_function_105
                (si::kelvin, 4.0026 * si::gram/si::decimeter3,
                 7.2475, 0.41865, 5.2, 0.24096,
                 2.2, 5.2), // 2.2-5.2K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 11.533, -8.99, 0.6724, 0.2743, 1.0,
                  1.76, 5.2), // 1.76-5.2K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 5.19, 0.0004656, 1.040, 0.001889, 2.468, -0.002006, 2.661,
                 0.0, 5.11), // Mulero (2012)
        /*refractive_index*/       1.02451,
        // get_dewaele2003_pressure_function
        //     (si::kilobar, 1.0, 0.8034, 0.20256, 1.0, 0.12763, 0.8, 115.0), // Dewaele (2003)
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              // 0.1 * si::watt / (si::centimeter * si::kelvin), // Webb (1952)
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::centimeter * si::kelvin),
                     std::vector<double>{1.39, 1.70, 2.12, 4.05}, 
                     std::vector<double>{0.32, 0.63, 0.40, 0.01}), // Johnson (1960)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           // 0187.0 * si::kilogram/si::meter3,
                get_interpolated_temperature_function
                    (si::kelvin, si::gram / si::centimeter3,
                     std::vector<double>{1.0,    4.0,    26.0}, 
                     std::vector<double>{0.1891, 0.2305,  0.3963}), // Johnson (1960)
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-271.7, -271.3, -270.3, -268.6 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  missing(),  
            // // TODO: reimplement this
            // get_dewaele2003_pressure_function
            //     (si::kilobar, 1.0, -0.1033, 1.0, 1.0, 0.052, 117.0, 202.0), // Dewaele (2003)
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      missing(),
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, 1e8 * si::dyne/si::centimeter2,
                     std::vector<double>{ 23.8,  24.06,  24.4}, 
                     std::vector<double>{ 2.43,   2.32,  2.17}), // Prokhvatilov 
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1e8 * si::dyne/si::centimeter2,
                     std::vector<double>{ 23.8,  24.06,  24.4}, 
                     std::vector<double>{ 1.16,   1.05,  1.00}), // Prokhvatilov 
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

// hydrogen, H2
// for the atmospheres of gas giants
PartlyKnownCompound hydrogen (
    /*molar_mass*/                        2.016   * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                287.7 * si::picometer, // wikipedia,  Ismail (2015)
    /*molecular_degrees_of_freedom*/      4.96,
    /*acentric_factor*/                   -0.215,

    /*critical_point_pressure*/           1.293 *  si::megapascal,
    /*critical_point_volume*/             65.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        32.97 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       445.0 * si::joule/si::gram,
    /*latent_heat_of_fusion*/             0.12*si::kilojoule/(2.016*si::gram),
    /*triple_point_pressure*/             7.042 * si::kilopascal,
    /*triple_point_temperature*/          13.8 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -259.198*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -252.7 * si::celcius, // Perry
    
    /*phase*/                             
    get_simon_glatzel_phase_function(
            /*triple_point_pressure*/       7.042 * si::kilopascal,
            /*triple_point_temperature*/    13.8 * si::kelvin,
            /*critical_point_pressure*/     1.293 *  si::megapascal,
            /*critical_point_temperature*/  32.97 * si::kelvin,
            /*latent_heat_of_vaporization*/ 445.0 * si::joule/si::gram,
            /*molar_mass*/                  2.016   * si::gram/si::mole,
            /*simon_glatzel_slope*/         274.22e5 * si::pascal,
            /*simon_glatzel_intercept*/     0.0 * si::pascal,
            /*simon_glatzel_exponent*/      1.74407
        ),
    
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{   5e6, 8.06e6, 8.48e6, 9.97e6, 1.05e7, 1.13e7, 1.41e7, 2.66e7, 5.74e7  },
          std::vector<double>{ -29.5, -23.05, -22.54, -20.41, -20.44, -20.79, -20.97, -21.66, -22.68  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ 
            relation::get_sigmoid_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                 0.03602, 3.74271, 0.00000, 1.70014, 2.72519, 120.41496, 97.80116, 11.80351,
                 63.957, 1014.0, 0.0, 1.0, 0.08), 
                 // hydrogen, mean error: 0.3%, max error: 1.4%, range: 63.957-1014K, 0-1MPa, stp estimate: 14.133
        /*thermal_conductivity*/   // 186.6 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00197, 1.33064, 0.00154, 0.84231, -0.00399,
                 63.957, 1014,  0.0, 1.0, 0.025), 
                // hydrogen, mean error: 1.3%, max error: 2.5%, range: 63.957-1014K, 0-1MPa, stp estimate: 0.169
        /*dynamic_viscosity*/      // 0.88e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.05907, 1.95272, 0.20949, 0.66373, -0.28287,
                 63.957, 1014,  0.0, 1.0, 0.009), 
                // hydrogen, mean error: 0.2%, max error: 0.9%, range: 63.957-1014K, 0-1MPa, stp estimate: 8.393
        /*density*/                missing(),
        /*refractive_index*/       1.0001392
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     return 1.0 + 0.0148956/(180.7-1.0/(l*l)) + 0.0049037/(92.0-1.0/(l*l));
        // }) // Peck & Hung (1977)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 9.668 * si::joule / (si::gram * si::kelvin), 
            // NOTE: this estimate produces bogus results outside its range, we may need to clamp this
            get_dippr_liquid_heat_capacity_temperature_function_114
                (32.97 * si::kelvin, si::joule / (2.01588 * si::kilogram * si::kelvin), 
                66.653, 6765.9, -123.63, 478.27, 0.0,
                13.95*si::kelvin, 32.0*si::kelvin), 
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 -0.0917, 0.017678, -0.000382, -3.3324e-6, 1.0266e-7,
                  13.95, 31), 
        /*dynamic_viscosity*/      // 13.06 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -11.661, 24.7, -0.261, -4.10e-16, 10.0,
                  13.95, 33), // 13.95-33K
        /*density*/                // 0.0708 * si::gram/si::milliliter,
            get_dippr_temperature_function_105
                (si::kelvin, 2.016 * si::gram/si::decimeter3,
                 5.414, 0.34893, 33.19, 0.2706,
                 13.95, 33.19), // 13.95-33.19K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 12.69, -94.896, 1.1125, 0.00032915, 2.0,
                  13.95, 33.19), // 13.95-33.19K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 32.97, -1.4165, 0.63882, 0.746383, 0.659804, 0.675625, 0.619149,
                 13.8, 31.0), // Mulero (2012)
        /*refractive_index*/       1.1096,
        // TODO: reimplement this
        // get_dewaele2003_pressure_function
        //     (si::kilobar, 1.0, 0.949, 0.06829, 11.8, 0.2853, 3.0, 49.0), // Dewaele (2003)
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   
                get_interpolated_temperature_function
                    ( si::kelvin, si::calorie / (si::gram * si::kelvin),
                      std::vector<double>{0.0,    3.04,    5.95,   9.87},
                      std::vector<double>{0.0, 0.02584, 0.06349, 0.2763}), // Johnson (1960)
            /*thermal_conductivity*/              state_invariant(1.819 * si::watt / ( si::meter * si::kelvin )), // wikipedia
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           // 86.0 * si::kilogram/si::meter3, // https://en.wikipedia.org/wiki/Solid_hydrogen
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{   2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{.08740,.08740,.08738,.08732,.08718,.08706,.08656}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-263.3, -261.3, -257.9, -252.5 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  missing(),
            // // TODO: reimplement this
            // field::SpectralFunction<double>([](
            //     const si::wavenumber nlo, 
            //     const si::wavenumber nhi, 
            //     const si::pressure p, 
            //     const si::temperature T
            // ) {
            //     return 0.95+0.1144*std::pow(std::clamp(p/si::kilobar, 52.0, 350.0), 0.3368);
            // }), // Peck & Hung (1977)
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{1.861, 1.849, 1.834, 1.815, 1.790, 1.761, 1.737}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{3.135, 3.115, 3.089, 3.056, 3.015, 2.966, 2.924}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{  2.0,   4.0,   6.0,   8.0,  10.0,  12.0,  13.5}, 
                     std::vector<double>{1.285, 1.278, 1.267, 1.253, 1.237, 1.216, 1.199}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{  2.0,  13.5}, 
                     std::vector<double>{0.219, 0.219}), // Prokhvatilov, for parahydrogen (more common at low temperatures)

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  1.4,   2.0,   3.0,   4.2,   6.0,   8.0,  10.0,  12.0,  13.0}, 
                     std::vector<double>{ 50.8,  50.4,  50.0,  49.2,  44.5,  37.2,  28.6,  17.1,  10.0}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{  1.4,   2.0,   3.0,   4.2,   6.0,   8.0,  10.0,  12.0,  13.0}, 
                     std::vector<double>{  8.4,   7.6,   7.0,   6.6,   5.5,   4.3,   3.8,   3.0,   1.6}), // Prokhvatilov, for parahydrogen (more common at low temperatures)
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);

// ammonia, NH3
// for the atmosphere of Titan
PartlyKnownCompound ammonia (

    /*molar_mass*/                        17.031 * si::gram/si::mole,
    /*atoms_per_molecule*/                4u,
    /*molecular_diameter*/                260.0 * si::picometer, // Breck (1974)
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.252,

    /*critical_point_pressure*/           11.357*si::megapascal,
    /*critical_point_volume*/             69.8*si::centimeter3/si::mole,
    /*critical_point_temperature*/        405.56 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       23.33*si::kilojoule/(17.031*si::gram),
    /*latent_heat_of_fusion*/             5.66*si::kilojoule/(17.031*si::gram),
    /*triple_point_pressure*/             6.060e3 * si::pascal,
    /*triple_point_temperature*/          195.30 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -77.65*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -33.4*si::celcius, // Perry
    
    /*phase*/                             
    get_simon_glatzel_phase_function(
            /*triple_point_pressure*/       6.060e3 * si::pascal,
            /*triple_point_temperature*/    195.30 * si::kelvin,
            /*critical_point_pressure*/     11.357*si::megapascal,
            /*critical_point_temperature*/  405.56 * si::kelvin,
            /*latent_heat_of_vaporization*/ 23.33*si::kilojoule/(17.031*si::gram),
            /*molar_mass*/                  17.031 * si::gram/si::mole,
            /*simon_glatzel_slope*/         5270e5 * si::pascal,
            /*simon_glatzel_intercept*/     0.0 * si::pascal,
            /*simon_glatzel_exponent*/      4.3
        ),
    
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  4.35e6, 4.84e6, 5.26e6, 6.14e6, 6.61e6, 7.28e6, 8.03e6, 9.32e6, 1.06e7, 1.23e7, 2.52e7, 5.80e7, 1.93e8  },
          std::vector<double>{  -26.43, -21.51, -21.17, -21.60, -21.58, -21.00, -20.92, -20.56, -20.76, -20.44, -20.78, -21.60, -22.61  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 35.06 * si::joule / (17.031 * si::gram * si::kelvin), // wikipedia data page
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.04652, 0.81959, 0.02155, 0.67889, 1.04112,
                 245.5, 695.5,  0.0, 10.0, 0.043), 
                // ammonia, mean error: 1.2%, max error: 4.3%, range: 245.5-695.5K, 0-10MPa, stp estimate: 2.020
        /*thermal_conductivity*/   // 25.1 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            relation::get_sigmoid_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00076, 1.10036, 0.00000, 0.00000, 0.07387, 491.21103, 544.10090, 0.05855,
                245.5, 695.5, 0.0, 10.0, 0.069), 
                // ammonia, mean error: 2.5%, max error: 6.9%, range: 245.5-695.5K, 0-10MPa, stp estimate: 0.023
        /*dynamic_viscosity*/      // 0.99e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.00986, 1.88109, 0.02443, 1.06310, -0.30191,
                 245.5, 695.5,  0.0, 10.0, 0.035), 
                // ammonia, mean error: 0.7%, max error: 3.5%, range: 245.5-695.5K, 0-10MPa, stp estimate: 9.204
        /*density*/                missing(),
        /*refractive_index*/       1.000376,
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     double invl2 = 1.0/(l*l);
        //     return 1.0
        //         + 0.032953f/(90.392f  - invl2);
        // }) 
    },
    
    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 4.700 * si::joule / (si::gram * si::kelvin), // wikipedia
            get_dippr_liquid_heat_capacity_temperature_function_114
                (405.56 * si::kelvin, si::joule / (17.031 * si::kilogram * si::kelvin), 
                61.289, 80925.0, 799.4, -2651.0, 0.0,
                203.15*si::kelvin, 401.15*si::kelvin),
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 1.169, -0.002314, 0.0, 0.0,  0.0,
                  195.41, 400.05), 
        /*dynamic_viscosity*/      
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -6.743,598.3,-0.7341,-3.690e-27,10.0,
                  195.41, 393.15), // 195.41-393.15K
        /*density*/                // 681.97 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
            get_dippr_temperature_function_105
                (si::kelvin, 17.031 * si::gram/si::decimeter3,
                 3.5383, 0.25443, 405.65, 0.2888,
                 195.41, 405.65), // 195.41-405.65K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 90.483, -4669.7, -11.607, 0.017194, 1.0,
                  195.41, 405.65), // 195.41-405.65K
        /*surface_tension*/        // 0.021 * si::newton/si::meter, // 25C, engineering toolbox
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 405.56, 0.1028, 1.211, -0.09453, 5.585, 0.0, 0.0,
                 197.85, 403.15), // Mulero (2012)
        /*refractive_index*/        1.3944,
            // get_interpolated_temperature_function
            //     (si::celcius, 1.0,
            //      std::vector<double>{ -20.0,  40.0}, 
            //      std::vector<double>{1.3475, 1.321}), // Francis (1960)
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{0.855, 0.853, 0.848, 0.841, 0.834, 0.826, 0.821}), // Prokhvatilov
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::kelvin, si::kilopascal,
                     std::vector<double>{160.0,     180.0,     190.0},     
                     std::vector<double>{0.1  ,     1.2  ,     3.5  }),
            /*refractive_index*/                  1.5,
            /*extinction_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber
                    (1.0/si::centimeter, 1.0,
                     std::vector<double>{ 4017.54, 3412.28, 3381.57, 3364.03, 3342.10, 3197.36, 3144.73, 1697.36, 1627.19, 1583.33, 1188.59, 1140.35, 1078.94, 1021.93,  969.29,  631.57,  561.40,  473.68,  403.50,  144.73,   83.333 },
                     std::vector<double>{   1.100,   1.103,   1.285,   1.282,   1.160,   1.141,   1.097,   1.103,   1.166,   1.129,   1.113,   1.216,   1.530,   1.132,   1.110,   1.125,   1.169,   1.342,   1.395,   1.226,   1.219 }),
            /*absorption_coefficient*/            missing(),

            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{61.24, 61.02, 59.16, 56.23, 53.20, 49.64, 47.83}), // Prokhvatilov
            /*tensile_modulus*/                   missing(),
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{43.28, 43.14, 42.70, 41.67, 40.18, 49.64, 47.83}), // Prokhvatilov
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 90.0, 100.0, 120.0, 140.0, 160.0, 180.0, 190.0}, 
                     std::vector<double>{0.214, 0.214, 0.209, 0.203, 0.198, 0.195, 0.196}), // Prokhvatilov

            /*compressive_fracture_strength*/     
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 77.0, 100.0, 120.0, 130.0, 140.0, 150.0, 160.0}, 
                     std::vector<double>{580.0, 620.0, 540.0, 450.0, 360.0, 270.0, 190.0}), // Prokhvatilov
            /*tensile_fracture_strength*/         
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 77.0, 120.0, 130.0, 140.0, 150.0, 160.0}, 
                     std::vector<double>{120.0, 140.0, 160.0, 180.0,  90.0,  50.0}), // Prokhvatilov
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram/si::millimeter2,
                     std::vector<double>{ 77.0, 100.0, 120.0, 130.0, 140.0, 150.0}, 
                     std::vector<double>{580.0, 480.0, 230.0, 150.0, 110.0, 200.0}), // Prokhvatilov
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);

// ozone, O3
// for modeling the ozone layer of earth like planets
PartlyKnownCompound ozone (
    /*molar_mass*/                        47.998 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                335.0 * si::picometer, // Streng (1961)
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.227,

    /*critical_point_pressure*/           5.57 *  si::megapascal,
    /*critical_point_volume*/             89.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        261.1 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       288.49 * si::kilojoule / si::kilogram,
    /*latent_heat_of_fusion*/             missing(),
    /*triple_point_pressure*/             7.346e-6 * si::bar, //encyclopedia.airliquide.com
    /*triple_point_temperature*/          -193.0 * si::celcius, //encyclopedia.airliquide.com
    /*freezing_point_sample_pressure*/    si::atmosphere, 
    /*freezing_point_sample_temperature*/ -193.0*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -112.0 * si::celcius, // Perry

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{   0.0,  2e5,  7e5,  9e5,1.6e6,  2e6,2.5e6,2.8e6,  3e6,3.5e6,4.6e6,  6e6,7.7e6,1.2e7f },
          std::vector<double>{ -28.0,-26.0,-31.0,-28.0,-24.0,-25.0,-27.0,-24.5,-23.0,-21.0,-22.5,-22.0,-21.0,-21.0  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 0.0043147, 0.47999, 700.09, 0.0,
                 161.85, 1000.0), // 161.85-1000K
        /*dynamic_viscosity*/      
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::pascal * si::second,
                1.196e-7, 0.84797, 0.0, 0.0,
                80.15, 1000.0), // 80.15-1000K
        /*density*/                missing(),
        /*refractive_index*/       1.00052
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (47.9982 * si::kilogram * si::kelvin), 
                60046.0, 281.16, 0.0, 0.0, 0.0,
                90.0, 150.0), 
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.17483, 0.00075288, -2.5228e-6, 0.0, 0.0,
                  77.35, 161.85), 
        /*dynamic_viscosity*/      
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -10.94, 415.96, 0.0, 0.0, 0.0,
                  75.55, 208.8), // 75.55-208.8K
        /*density*/                // 1349.0 * si::kilogram / si::meter3, //encyclopedia.airliquide.com
            get_dippr_temperature_function_105
                (si::kelvin, 47.998 * si::gram/si::decimeter3,
                 3.3592, 0.29884, 261.0, 0.28523,
                 80.15, 261.0), // 80.15-261.0K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 40.067, -2204.8, -2.9351, 7.75e-16, 6.0,
                  80.15, 261), // 80.15-261K
        /*surface_tension*/        state_invariant(38.1 * si::dyne/si::centimeter), // -182.7C, Jasper (1972)
        /*refractive_index*/       1.2226,
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/   missing(),
            /*thermal_conductivity*/              state_invariant(5.21e-4 * si::calorie / (si::second*si::centimeter2*si::kelvin/si::centimeter)), // Streng (1961)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(1354.0  * si::kilogram / si::meter3), //https://www.aqua-calc.com/page/density-table/substance/solid-blank-ozone
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-180.4, -163.2, -141.0, -111.1 }, 
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

// LESS CHARACTERIZED COMPOUNDS:
// nitrous oxide, N2O
// representative of industrial emissions
PartlyKnownCompound nitrous_oxide (
    /*molar_mass*/                        44.012 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                330.0 * si::picometer, // wikipedia, Matteucci
    /*molecular_degrees_of_freedom*/      7.32,
    /*acentric_factor*/                   0.142,

    /*critical_point_pressure*/           7.245 *  si::megapascal,
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        309.52 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       374.286 * si::kilojoule / si::kilogram, //encyclopedia.airliquide.com
    /*latent_heat_of_fusion*/             6.54*si::kilojoule/(44.012*si::gram),
    /*triple_point_pressure*/             87.85 * si::kilopascal, // wikipedia
    /*triple_point_temperature*/          -90.82 * si::celcius,   // wikipedia
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -102.3*si::celcius, // Perry
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -90.7 * si::celcius,// Perry

    /*phase*/                              
    get_simon_glatzel_phase_function(
            /*triple_point_pressure*/       87.85 * si::kilopascal,// wikipedia
            /*triple_point_temperature*/    -90.82 * si::celcius,  // wikipedia
            /*critical_point_pressure*/     7.245 * si::megapascal,
            /*critical_point_temperature*/  309.52 * si::kelvin,
            /*latent_heat_of_vaporization*/ 374.286 * si::kilojoule / si::kilogram,
            /*molar_mass*/                  44.012 * si::gram/si::mole,
            /*simon_glatzel_slope*/         3.5674e-3 * si::bar,   // Manzhelii (1997)
            /*simon_glatzel_intercept*/     -4052.1 * si::bar,     // Manzhelii (1997)
            /*simon_glatzel_exponent*/      2.6786                 // Manzhelii (1997)
        ),
    
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  3.16e6, 3.29e6, 3.85e6, 4.15e6, 5.10e6, 5.62e6, 5.95e6, 6.29e6, 6.33e6, 6.66e6, 6.91e6, 7.25e6, 7.31e6, 7.73e6, 8.00e6, 9.68e6, 1.07e7, 1.32e7, 1.54e7, 2.82e7, 4.30e7, 7.11e7, 2.34e8  },
          std::vector<double>{  -28.02, -27.33, -27.38, -26.88, -23.17, -22.83, -23.13, -23.43, -23.43, -21.58, -21.14, -20.67, -22.14, -20.17, -20.53, -20.62, -20.75, -20.37, -20.40, -20.57, -20.85, -21.29, -22.43  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   // 17.4 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 0.001096, 0.667, 540.0, 0.0,
                 182.3, 1000.0), // 182.3-1000K
        /*dynamic_viscosity*/      // 1.47e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::pascal * si::second,
                2.1150e-6, 0.4642, 305.7, 0.0,
                182.3, 1000.0), // 182.3-1000 K
        /*density*/                missing(),
        /*refractive_index*/       1.000516
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (44.0128 * si::kilogram * si::kelvin), 
                67556.0, 54.373, 0.0, 0.0, 0.0,
                182.3, 200.0), 
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.10112, 0.0, 0.0, 0.0,  0.0,
                  277.59, 277.59), 
        /*dynamic_viscosity*/      
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 19.329, -381.68, -4.8618, 0.0, 0.0,
                  210, 283.09), // 210-283.09K
        /*density*/                // 1230.458 * si::kilogram / si::meter3, 
            get_dippr_temperature_function_105
                (si::kelvin, 44.012 * si::gram/si::decimeter3,
                 2.781, 0.27244, 309.57, 0.2882,
                 182.3, 309.57), // 182.3-309.57K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 96.512, -4045, -12.277, 0.00002886, 2.0,
                  182.3, 309.57),// 182.3-309.57K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 309.52, 0.07087, 1.204, 0.0, 0.0, 0.0, 0.0,
                 182.50, 293.15), // Mulero (2012)
        /*refractive_index*/       1.238,
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid {
            /*isobaric_specific_heat_capacity*/            
                get_interpolated_temperature_function
                    ( si::kelvin, si::joule/(28.013*si::gram * si::kelvin),
                     std::vector<double>{0.0,   3.0,  10.0, 40.0, 60.0, 150.0, 180.0}, 
                     std::vector<double>{0.0,0.0189, 0.822,21.65,31.47, 50.70, 58.28}), // Manzhelii (1997)
            /*thermal_conductivity*/              
                get_interpolated_temperature_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{ 23.0, 30.0, 40.0, 60.0, 120.0, 180.0}, 
                     std::vector<double>{17.08,40.27, 10.8, 7.89,  4.96,  3.38}), // Manzhelii (1997)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(1.263 * si::gram/si::centimeter3), // Hudson (2020)
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-143.4, -128.7, -110.3, -85.5  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  1.317, // Hudson (2020)
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


// sulfur dioxide, SO2
// representative of industrial emissions
PartlyKnownCompound  sulfur_dioxide (
    /*molar_mass*/                        64.064 * si::gram/si::mole,
    /*atoms_per_molecule*/                3u,
    /*molecular_diameter*/                360.0 * si::picometer, // wikipedia, Breck (1974)
    /*molecular_degrees_of_freedom*/      6.0,
    /*acentric_factor*/                   0.245,

    /*critical_point_pressure*/           7.884 * si::megapascal,
    /*critical_point_volume*/             122.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        430.64 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       24.94 *si::kilojoule/(64.064*si::gram),
    /*latent_heat_of_fusion*/             1769.0 * si::calorie/(64.064 * si::gram), // Perry
    /*triple_point_pressure*/             1.67e3 * si::pascal,
    /*triple_point_temperature*/          197.69 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -75.45 * si::celcius, // Perry
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -10.02*si::celcius,

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{2.47e6,2.69e6,2.91e6,3.12e6,3.41e6,3.76e6,4.25e6,4.37e6,4.95e6,5.45e6,6.01e6,6.63e6,7.36e6,7.95e6,8.51e6,8.80e6,9.07e6,9.35e6},
          std::vector<double>{-28.29,-25.86,-25.90,-23.18,-22.04,-22.33,-23.69,-22.55,-20.88,-21.43,-22.48,-21.25,-21.45,-19.92,-21.12,-20.35,-20.88,-20.68}),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ 
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.03124, 1.38702, 0.00214, 0.79655, 0.42765,
                 247.7, 547.7,  0.0, 1.0, 0.020), 
                // sulfur dioxide, mean error: 0.7%, max error: 2.0%, range: 247.7-547.7K, 0-1MPa, stp estimate: 0.615
        /*thermal_conductivity*/   // 9.6 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 10.527, -0.7732, -1333.0, 1506400.0,
                 250.0, 900.0), // 250-900K
        /*dynamic_viscosity*/      // 1.26e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::pascal * si::second,
                6.8630e-7, 0.6112, 217.0, 0.0,
                197.67, 1000.0), // 197.67-1000K
        /*density*/                missing(),
        /*refractive_index*/       1.000686
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (64.0638 * si::kilogram * si::kelvin), 
                85743.0, 5.7443, 0.0, 0.0, 0.0,
                197.67, 350.0), 
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.38218, -0.0006254, 0.0, 0.0, 0.0,
                  197.67, 400), 
        /*dynamic_viscosity*/      // 0.368 * si::millipascal*si::second, // pubchem
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 46.223, -1378.0, -8.7475, 0.0, 0.0,
                  225, 400), // 225-400K
        /*density*/                // 389.06 * si::kilogram / si::meter3, // encyclopedia.airliquide.com
            get_dippr_temperature_function_105
                (si::kelvin, 64.064 * si::gram/si::decimeter3,
                 2.106, 0.25842, 430.75, 0.2895,
                 197.67, 430.75), // 197.67-430.75K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 47.365, -4084.5, -3.6469, 1.80e-17, 6.0,
                 197.67, 430.75),//197.67-430.75K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 430.64, 0.0803, 0.928, 0.0139, 1.570, -0.0114, 0.364,
                 189.15, 373.15), // Mulero (2012)
        /*refractive_index*/        1.3396,
            // get_interpolated_temperature_function
            //     (si::celcius, 1.0,
            //      std::vector<double>{-20.0, 40.0}, 
            //      std::vector<double>{ 1.37, 1.33}), // Francis (1960)
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
                     std::vector<double>{-95.5,  -76.8,  -46.9,  -10.0  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber
                    (1.0/si::centimeter, 1.0,
                     std::vector<double>{ 4032.02, 1483.46, 1381.00, 1361.79, 1336.18, 1323.37, 1321.24, 1299.89, 1171.82, 1163.29, 1150.48, 1135.54,  569.90,  552.83,  537.89,  525.08,  522.95,  508.00,  405.55,  405.55 },
                     std::vector<double>{   2.598,   2.598,   2.617,   2.653,   3.233,   3.190,   2.724,   2.604,   2.604,   3.043,   3.052,   2.607,   2.601,   2.623,   3.040,   3.052,   2.613,   2.595,   2.601,   2.601 }),
            /*absorption_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    (1.0/si::centimeter, 1.0/si::centimeter,
                     std::vector<double>{  5097.2,  5028.0,  5008.2,  4968.7,  4948.9,  4731.5,  4691.9,  4672.2,  4553.5,  4533.8,  4514.0,  4108.7,  4089.0,  4059.3,  4019.8,  3990.1,  3920.9,  3911.0,  3881.4,  3871.5,  3841.8,  3812.2,  3752.9,  3743.0,  3713.3,  3644.2,  3614.5,  3575.0,  3525.5,  3495.9,  3456.3,  3416.8,  3397.0,  3357.5,  3327.8,  3308.1,  3041.2,  2981.9,  2942.3,  2843.5,  2823.7,  2774.3,  2734.8,  2665.6,  2616.1,  2566.7,  2537.1,  2477.8,  2448.1,  2359.1,  2309.7,  2280.1,  2240.5 },
                     std::vector<double>{  -2.262,  -0.619,  -2.227,  -1.878,  -2.227,  -2.210,  -0.759,  -2.210,  -2.227,  -1.843,  -2.227,  -2.245,  -1.493,  -2.157,  -1.895,  -1.196,   0.343,  -1.161,  -0.462,  -2.000,  -2.192,  -2.192,  -0.794,  -2.210,  -1.790,  -1.493,  -0.584,   1.199,  -0.741,  -1.528,  -1.790,   0.150,  -2.087,  -2.157,  -1.476,  -2.192,  -2.227,  -0.479,  -2.227,  -2.227,   0.010,  -0.899,  -0.706,   0.675,  -0.339,   0.517,   0.430,   2.685,   1.304,   0.045,   2.108,   0.395,   0.255 }),

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


// nitric oxide, NO
// representative of industrial emissions
    PartlyKnownCompound nitric_oxide (
    /*molar_mass*/                        30.006 * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                317.0 * si::picometer, // wikipedia, Matteucci
    /*molecular_degrees_of_freedom*/      missing(),
    /*acentric_factor*/                   0.585,

    /*critical_point_pressure*/           6.48 *  si::megapascal,
    /*critical_point_volume*/             58.0 *  si::centimeter3/si::mole,
    /*critical_point_temperature*/        180.0 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       13.83 * si::kilojoule/(30.006*si::gram),
    /*latent_heat_of_fusion*/             2.3*si::kilojoule/(30.006*si::gram),
    /*triple_point_pressure*/             87.85e3 * si::pascal,
    /*triple_point_temperature*/          182.34 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -163.6*si::celcius,
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{4.82e6, 5.61e6, 5.83e6, 6.55e6, 7.23e6, 7.65e6, 8.31e6, 9.94e6, 1.11e7, 1.26e7, 1.67e7, 4.05e7, 7.18e7, 1.85e8},
          std::vector<double>{-22.20, -21.43, -21.37, -21.56, -21.49, -21.56, -21.57, -20.71, -20.41, -20.71, -20.60, -20.94, -21.42, -22.55}),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ 
            relation::get_perry_temperature_gas_relation
                (si::kelvin, si::calorie/(30.006 * si::gram*si::kelvin), 
                  8.05, 0.000233, -156300.0, 0.0,
                  300.0, 5000.0),
        /*thermal_conductivity*/   // 25.9 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 0.0004096, 0.7509, 45.6, 0.0,
                 121.38, 750.0), // 121.38-750K
        /*dynamic_viscosity*/      // 0.0188 * si::millipascal * si::second, //pubchem
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::pascal * si::second,
                1.4670e-6, 0.5123, 125.4, 0.0,
                110.0, 1500.0), // 110-1500K
        /*density*/                missing(),
        /*refractive_index*/       1.000297
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (30.0061 * si::kilogram * si::kelvin), 
                -2979600.0, 76602.0, -652.59, 1.8879, 0.0,
                109.5, 150.0), 
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.1878, 0.0010293, -0.00000943,  0.0, 0.0,
                  110, 176.4), 
        /*dynamic_viscosity*/      
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -246.65, 3150.3, 49.98, -0.22541, 1.0,
                  109.5, 180.05), // 109.5-180.05K
        /*density*/                // 1230.458 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
            get_dippr_temperature_function_105
                (si::kelvin, 30.006 * si::gram/si::decimeter3,
                 5.246, 0.3044, 180.15, 0.242,
                 109.5, 180.15), // 109.5-180.15K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 72.974, -2650.0, -8.261, 9.7e-15, 6.0,
                  109.5, 180.15),// 109.5-180.15K
        /*surface_tension*/        
            get_interpolated_temperature_function
                (si::celcius, si::dyne/si::centimeter,
                 std::vector<double>{-160.0,-156.0,-152.0}, 
                 std::vector<double>{ 26.17, 23.83, 21.49}), // Jasper (1972)
        /*refractive_index*/       1.330,
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
                     std::vector<double>{-184.5, -178.2, -166.0, -151.7 }, 
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

// carbon monoxide, CO
// for the surface of pluto
PartlyKnownCompound carbon_monoxide (
    /*molar_mass*/                        28.010 * si::gram/si::mole, 
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                357.0 * si::picometer, // Mehio (2014)
    /*molecular_degrees_of_freedom*/      5.0,
    /*acentric_factor*/                   0.066,

    /*critical_point_pressure*/           3.494 *  si::megapascal,
    /*critical_point_volume*/             93.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        132.86 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       6.04 * si::kilojoule/(28.010*si::gram),
    /*latent_heat_of_fusion*/             0.833*si::kilojoule/(28.010*si::gram),
    /*triple_point_pressure*/             15.35 * si::kilopascal, // PubChem
    /*triple_point_temperature*/          68.15 * si::kelvin,     // PubChem
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -205.02*si::celcius,
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -192.0 * si::celcius,// Perry

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

                // triple point between solid, liquid, and gas
                const si::pressure        p0 = 15.35 * si::kilopascal; // PubChem
                const si::temperature     T0 = 68.15 * si::kelvin;     // PubChem
                // sample point along alpha and beta line
                const si::pressure        pab0 = 1.0 * si::atmosphere;
                const si::temperature     Tab0 = 62.0 * si::kelvin;
                // sample point along alpha and beta line
                const si::pressure        pab1 = 3.5 * si::gigapascal;
                const si::temperature     Tab1 = 150.0 * si::kelvin;
                // critical point
                const si::pressure        pc = 3.494 *  si::megapascal;
                const si::temperature     Tc = 132.86 * si::kelvin;
                // latent heat of vaporization and molar mass
                const si::specific_energy L  = 6.04 * si::kilojoule/(28.010*si::gram);
                const si::molar_mass      M  = 28.010 * si::gram/si::mole;
                // Simon-Glatzel constants, from Manzhelii (1997)
                const si::pressure        a  = 1.4198e-2 * si::bar;// Manzhelii (1997)
                const si::pressure        b  = -1240.0 * si::bar;  // Manzhelii (1997)
                const double              c  = 2.695;              // Manzhelii (1997)
                const auto                dpdTab = (pab1-pab0) / (Tab1-Tab0);

                const int basic_phase = property::get_simon_glatzel_phase(p, T, p0, T0, pc, Tc, L, M, a, b, c);
                if (basic_phase < 0)
                {
                    return basic_phase;
                } 
                else if ( p < dpdTab * (T-Tab0) + pab0 )
                {
                    return beta;
                }
                else
                {
                    return alpha;
                }
            }
        ),


    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  4.83e6, 4.85e6, 4.88e6, 5.00e6, 5.02e6, 5.05e6, 5.17e6, 5.19e6, 5.22e6, 5.33e6, 5.36e6, 5.38e6, 5.49e6, 5.51e6, 5.55e6, 5.64e6, 5.67e6, 5.68e6, 5.71e6, 6.02e6, 6.85e6, 7.98e6, 8.42e6, 9.28e6, 1.00e7, 1.05e7, 1.13e7, 1.21e7, 1.38e7, 2.10e7, 4.54e7, 5.15e8 },
          std::vector<double>{  -28.38, -24.93, -28.40, -28.39, -24.91, -28.40, -28.39, -25.16, -28.42, -28.39, -25.52, -28.39, -28.38, -25.72, -28.41, -28.40, -25.96, -23.41, -28.42, -22.47, -20.89, -21.48, -22.01, -20.72, -20.93, -20.48, -20.35, -20.56, -20.56, -20.68, -21.04, -22.55 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 29.0 * si::joule / (28.010 * si::gram * si::kelvin), // wikipedia data page
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.00728, 0.92688, 0.00010, 0.97052, 1.01709,
                 168.16, 518.16,  0.0, 3.0, 0.008), 
                // carbon monoxide, mean error: 0.3%, max error: 0.8%, range: 168.16-518.16K, 0-3MPa, stp estimate: 1.041
        /*thermal_conductivity*/   // 25.0 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00045, 0.43412, 0.00055, 0.70174, -0.00365,
                 118.16, 518.16,  0.0, 3.0, 0.025), 
                // carbon monoxide, mean error: 0.8%, max error: 2.5%, range: 118.16-518.16K, 0-3MPa, stp estimate: 0.025
        /*dynamic_viscosity*/      // 1.74e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.26687, 1.09457, 0.33802, 0.70825, -1.67961,
                 118.16, 518.16,  0.0, 3.0, 0.029), 
                // carbon monoxide, mean error: 1.2%, max error: 2.9%, range: 118.16-518.16K, 0-3MPa, stp estimate: 16.311
        /*density*/                missing(),
        /*refractive_index*/       1.00036320, //https://refractiveindex.info
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     constexpr double n = 1.00036350f;
        //     constexpr double dndl = -0.00027275f;
        //     return n + dndl * l;
        // }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 60.351 * si::joule / (28.010 * si::gram * si::kelvin), // pubchem
            // NOTE: this function is not producing physically sensible values, we may need to remove it if it can't be fixed
            // get_dippr_liquid_heat_capacity_temperature_function_114
            //     (132.86 * si::kelvin, si::joule / (28.0101* si::kilogram * si::kelvin), 
            //     65.429, 28723.0, -847.39, 1959.6, 0.0,
            //     68.15*si::kelvin, 132.0*si::kelvin
            get_interpolated_temperature_function
                ( si::kelvin, si::joule/(si::gram * si::kelvin),
                  std::vector<double>{68.16, 80.0,100.0,110.0,120.0,130.0},
                  std::vector<double>{2.157,2.142,2.306,2.558,3.202,8.070}),
        /*thermal_conductivity*/   // 0.1428 * si::watt / (si::meter * si::kelvin), // pubchem
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.2855, -0.001784, 0.0,  0.0,  0.0,
                  68.15, 125), 
        /*dynamic_viscosity*/      // 0.170 * si::millipascal * si::second, // Johnson (1960)
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -4.9735, 97.67, -1.1088, 0.0, 0.0,
                  68.15, 131.37), // 68.15-131.37K
        /*density*/                
            get_dippr_temperature_function_105
                (si::kelvin, 28.010 * si::gram/si::decimeter3,
                 2.897, 0.27532, 132.92, 0.2813,
                 68.15, 132.92), // 68.15-132.92K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 45.698, -1076.6, -4.8814, 0.000075673, 2.0,
                  68.15, 132.92), // 68.15-132.92K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 132.86, 0.02843, 1.148, 0.0, 0.0, 0.0, 0.0,
                 70.0, 91.89), // Mulero (2012)
        /*refractive_index*/       missing(),
        /*extinction_coefficient*/ missing()

    },

    /*solid*/ 
    std::vector<phase::PartlyKnownSolid>{
        phase::PartlyKnownSolid { // beta: warmer form, transitions to alpha at 61.5K
            /*isobaric_specific_heat_capacity*/            
                get_interpolated_temperature_function
                    (si::kelvin, si::joule/(28.010*si::gram*si::kelvin),
                     std::vector<double>{ 63.0,  64.0,  65.0,  66.0,  67.0}, 
                     std::vector<double>{50.10, 50.58, 51.08, 51.58, 52.29}), // Manzhelii (1997)
            /*thermal_conductivity*/              
                get_interpolated_temperature_function
                    ( si::kelvin, si::milliwatt/(si::centimeter * si::kelvin),
                     std::vector<double>{22.5, 25.0, 35.0, 45.0, 58.0}, 
                     std::vector<double>{6.37, 5.69, 4.16, 3.34, 2.62}), // Manzhelii (1997)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{0.9378,0.9326,0.9269,0.9238}), // Prokhvatilov
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-222.0, -215.0, -205.7, -191.3 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    (1.0/si::centimeter, 1.0/si::centimeter,
                     std::vector<double>{  6439.7,  6390.1,  6350.4,  6320.7,  6300.8,  6211.6,  6191.7,  6152.1,  4486.0,  4466.1,  4366.9,  4327.3,  4287.6,  4257.9,  4238.0,  4208.3,  4178.5,  4158.7,  4138.8,  4079.3,  4059.5,  4029.8 },
                     std::vector<double>{  -2.678,  -1.542,  -2.661,   0.407,  -2.678,  -2.695,  -1.576,  -2.695,  -2.661,  -1.814,  -0.424,   0.373,   0.017,   2.780,  -0.983,  -0.576,  -1.576,   0.797,  -2.678,  -2.695,  -1.797,  -2.678 }),

            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{15.65, 15.23, 14.80, 14.55}), // Prokhvatilov
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{11.27, 10.88, 10.49, 10.33}), // Prokhvatilov
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{ 4.08,  3.94,  3.80,  3.74}), // Prokhvatilov
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 62.0,  64.0,  66.0,  67.0}, 
                     std::vector<double>{0.380, 0.381, 0.382, 0.382}), // Prokhvatilov

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         missing(),
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }, 
        phase::PartlyKnownSolid { //alpha
            /*isobaric_specific_heat_capacity*/            
                get_interpolated_temperature_function
                    (si::kelvin, si::joule/(28.010*si::gram*si::kelvin),
                     std::vector<double>{  0.0,   6.0,  12.0,  18.0,  24.0,  30.0,  36.0,  42.0,  48.0,  54.0,  61.0}, 
                     std::vector<double>{  0.0, 0.467, 4.375, 11.77, 18.92, 25.04, 30.73, 35.92, 41.45, 48.53, 67.83}), // Manzhelii (1997)
            /*thermal_conductivity*/              missing(),
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           
                get_interpolated_temperature_function
                    (si::kelvin, si::gram/si::centimeter3,
                     std::vector<double>{  20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  44.0,  52.0,  56.0,  60.0}, 
                     std::vector<double>{1.0293,1.0273,1.0232,1.0202,1.0159,1.0110,1.0054,0.9909,0.9812,0.9689}), // Prokhvatilov
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  missing(),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
                    (1.0/si::centimeter, 1.0/si::centimeter,
                     std::vector<double>{  6439.7,  6390.1,  6350.4,  6320.7,  6300.8,  6211.6,  6191.7,  6152.1,  4486.0,  4466.1,  4366.9,  4327.3,  4287.6,  4257.9,  4238.0,  4208.3,  4178.5,  4158.7,  4138.8,  4079.3,  4059.5,  4029.8 },
                     std::vector<double>{  -2.678,  -1.542,  -2.661,   0.407,  -2.678,  -2.695,  -1.576,  -2.695,  -2.661,  -1.814,  -0.424,   0.373,   0.017,   2.780,  -0.983,  -0.576,  -1.576,   0.797,  -2.678,  -2.695,  -1.797,  -2.678 }),

            /*bulk_modulus*/                      
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  44.0,  52.0,  56.0,  60.0}, 
                     std::vector<double>{22.29, 22.04, 21.70, 21.34, 20.91, 20.24, 19.24, 16.52, 15.07, 13.56}), // Prokhvatilov
            /*tensile_modulus*/                   
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  44.0,  52.0,  56.0,  60.0}, 
                     std::vector<double>{37.69, 37.30, 36.83, 36.33, 35.73, 34.85, 33.71, 30.15, 27.68, 24.73}), // Prokhvatilov
            /*shear_modulus*/                     
                get_interpolated_temperature_function
                    (si::kelvin, si::kilobar,
                     std::vector<double>{ 20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  44.0,  52.0,  56.0,  60.0}, 
                     std::vector<double>{15.47, 15.32, 15.13, 14.94, 14.70, 14.36, 13.95, 12.60, 11.59, 10.34}), // Prokhvatilov
            /*pwave_modulus*/                     missing(),
            /*lame_parameter*/                    missing(),
            /*poisson_ratio*/                     
                get_interpolated_temperature_function
                    (si::kelvin, 1.0,
                     std::vector<double>{ 20.0,  24.0,  28.0,  32.0,  36.0,  40.0,  44.0,  52.0,  56.0,  60.0}, 
                     std::vector<double>{0.218, 0.218, 0.217, 0.216, 0.215, 0.213, 0.208, 0.196, 0.194, 0.196}), // Prokhvatilov

            /*compressive_fracture_strength*/     missing(),
            /*tensile_fracture_strength*/         
                get_interpolated_temperature_function
                    (si::kelvin, si::standard_gravity * si::gram / si::millimeter2,
                     std::vector<double>{ 53.0,  55.0,  56.0,  57.0,  59.0,  63.5}, 
                     std::vector<double>{ 70.0,  85.0,  96.0, 110.0,  65.0,  24.0}), // Prokhvatilov
            /*shear_fracture_strength*/           missing(),
            /*compressive_yield_strength*/        missing(),
            /*tensile_yield_strength*/            missing(),
            /*shear_yield_strength*/              missing(),

            /*chemical_susceptibility_estimate*/  missing()
        }
    }
);


// ethane, C2H6
// for the lakes of Titan
PartlyKnownCompound ethane (
    /*molar_mass*/                        30.070 * si::gram/si::mole,
    /*atoms_per_molecule*/                8u,
    /*molecular_diameter*/                443.0 * si::picometer,  // Aguado (2012)
    /*molecular_degrees_of_freedom*/      6.8,
    /*acentric_factor*/                   0.099,

    /*critical_point_pressure*/           4.88 * si::megapascal,
    /*critical_point_volume*/             146.0 * si::centimeter3/si::mole,
    /*critical_point_temperature*/        305.36 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       14.69 * si::kilojoule/(30.070*si::gram),
    /*latent_heat_of_fusion*/             2.72*si::kilojoule/(30.070*si::gram),
    /*triple_point_pressure*/             1.4 * si::pascal,
    /*triple_point_temperature*/          90.35 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -183.79 * si::celcius, 
    /*boiling_point_sample_pressure*/     si::atmosphere,
    /*boiling_point_sample_temperature*/  -88.6 * si::celcius, // Perry

    /*phase*/                             missing(),
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{ 5.6e6, 6.25e6, 6.73e6, 7.49e6, 8.23e6, 9.26e6, 1.01e7, 1.14e7, 2.42e7, 5.43e7, 1.72e8 },
          std::vector<double>{ -35.0, -25.67, -21.86, -20.50, -20.60, -20.27, -20.25, -20.08, -20.57, -21.44, -22.57 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 52.49 * si::joule / (30.070 * si::gram * si::kelvin), // wikipedia data page
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.05856, 0.85039, 0.00090, 1.21385, 0.86559,
                 140.35, 640.35,  0.0, 3.0, 0.033), 
                // ethane, mean error: 1.3%, max error: 3.3%, range: 140.35-640.35K, 0-3MPa, stp estimate: 1.687
        /*thermal_conductivity*/   // 21.2 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            relation::get_sigmoid_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00064, 1.08467, 0.00000, 0.00000, 0.08892, 442.87962, 573.08449, 0.06794,
                140.35, 640.35, 0.0, 3.0, 0.038), 
                // ethane, mean error: 1.1%, max error: 3.8%, range: 140.35-640.35K, 0-3MPa, stp estimate: 0.018
        /*dynamic_viscosity*/      // 9.4 * si::micropascal*si::second,
            relation::get_exponent_pressure_temperature_relation
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.07538, 2.19443, 0.07385, 0.85870, -0.57044,
                 140.35, 640.35,  0.0, 3.0, 0.029), 
                // ethane, mean error: 1.2%, max error: 2.9%, range: 140.35-640.35K, 0-3MPa, stp estimate: 8.560
        /*density*/                missing(),
        /*refractive_index*/       1.00075276,
        // // TODO: reimplement this
        // field::SpectralFunction<double>([](
        //     const si::wavenumber nlo, 
        //     const si::wavenumber nhi, 
        //     const si::pressure p, 
        //     const si::temperature T
        // ) {
        //     double l = (2.0 / (nhi+nlo) / si::micrometer);
        //     double invl2 = 1.0/(l*l);
        //     return 1.0007330f
        //         + 6.822764e-6 * invl2;
        // }) 
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 68.5 * si::joule / (30.070 * si::gram * si::kelvin), // wikipedia data page
            get_dippr_liquid_heat_capacity_temperature_function_114
                (305.36 * si::kelvin, si::joule / (30.069 * si::kilogram * si::kelvin), 
                44.009, 89718.0, 918.77, -1886.0, 0.0,
                92.0*si::kelvin, 290.0*si::kelvin), 
            // get_interpolated_temperature_function
            //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
            //       std::vector<double>{90.4,120.0,160.0,240.0,280.0,300.0},
            //       std::vector<double>{2.33,2.280,2.357,2.847,3.987,10.02}),
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.35758, -0.0011458, 6.1866e-7, 0.0, 0.0,
                  90.35, 300), 
        /*dynamic_viscosity*/      
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -7.0046, 276.38, -0.6087, -3.11e-18, 7.0,
                  90.35, 300), // 90.35-300K
        /*density*/                // 545.0 * si::kilogram/si::meter3,
            get_dippr_temperature_function_105
                (si::kelvin, 30.070 * si::gram/si::decimeter3,
                 1.9122, 0.27937, 305.32, 0.29187,
                 90.35, 305.32), // 90.35-305.32K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 51.857,-2598.7, -5.1283, 0.000014913, 2.0,
                  90.35, 305.32), // 90.35-305.32K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 305.36, 0.07602, 1.320, -0.02912, 1.676, 0.0, 0.0,
                 89.87, 304.93), // Mulero (2012)
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
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-159.5, -142.9, -119.3, -88.6  }, 
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


// hydrogen cyanide, HCN
// for small bodies in the outer solar system and interstellar space
// one of the most abundant compounds following from elemental abundances in the universe
PartlyKnownCompound hydrogen_cyanide (
    /*molar_mass*/                        27.026 * si::gram/si::mole,
    /*atoms_per_molecule*/                2u,
    /*molecular_diameter*/                376.0 * si::picometer, // wikipedia, Matteucci
    /*molecular_degrees_of_freedom*/      missing(),
    /*acentric_factor*/                   0.407,

    /*critical_point_pressure*/           5.4 * si::megapascal,
    /*critical_point_volume*/             missing(),
    /*critical_point_temperature*/        456.65 * si::kelvin,
    /*critical_point_compressibility*/    missing(),

    /*latent_heat_of_vaporization*/       6027.0*si::calorie/(27.026 * si::gram), // Perry
    /*latent_heat_of_fusion*/             2009.0*si::calorie/(27.026 * si::gram), // Perry
    /*triple_point_pressure*/             0.153e3 * si::pascal,
    /*triple_point_temperature*/          259.7 * si::kelvin,
    /*freezing_point_sample_pressure*/    si::atmosphere,
    /*freezing_point_sample_temperature*/ -13.29 * si::celcius,
    /*boiling_point_sample_pressure*/     missing(),
    /*boiling_point_sample_temperature*/  missing(),
    
    /*phase*/                             
    get_simon_glatzel_phase_function(
            /*triple_point_pressure*/       0.153e3 * si::pascal,
            /*triple_point_temperature*/    259.7 * si::kelvin,
            /*critical_point_pressure*/     5.4 * si::megapascal,
            /*critical_point_temperature*/  456.65 * si::kelvin,
            /*latent_heat_of_vaporization*/ 6027.0*si::calorie/(27.026 * si::gram), // Perry
            /*molar_mass*/                  27.026 * si::gram/si::mole,
            /*simon_glatzel_slope*/         3080e5 * si::pascal,
            /*simon_glatzel_intercept*/     0.0 * si::pascal,
            /*simon_glatzel_exponent*/      3.6
        ),
    
    /*molecular_absorption_cross_section*/ 
    compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  8.50e6, 8.58e6, 9.19e6, 9.59e6, 9.89e6, 9.99e6, 1.01e7, 1.02e7, 1.03e7, 1.05e7, 1.07e7, 1.09e7, 1.26e7, 1.35e7, 1.41e7, 1.46e7, 1.62e7 },
          std::vector<double>{   -24.2,  -21.8,  -21.5,  -22.1,  -20.3,  -20.6,  -20.3,  -20.7,  -20.2,  -20.2,  -20.6,  -20.3,  -20.5,  -20.3,  -20.4,  -20.4,  -20.5 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 4.6496e-6, 1.3669, -210.76, 58295.0,
                 273.15, 673.15), // 273.15-673.15K
        /*dynamic_viscosity*/      
            relation::get_dippr_temperature_relation_102
                (si::kelvin, si::pascal * si::second,
                1.2780e-8, 1.0631, 340.0, 0.0,
                183.65, 1000.0), // 183.65-1000K
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::joule / (27.02534 * si::kilogram * si::kelvin), 
                95398.0, -197.52, 0.3883, 0.0, 0.0,
                259.83, 298.85), 
        /*thermal_conductivity*/   
            get_dippr_quartic_temperature_function_100
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.43454, -0.0007008,  0.0,  0.0,  0.0,
                  259.83, 298.85), 
        /*dynamic_viscosity*/      
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal* si::second, 
                 -21.927, 1266.5, 1.5927, 0.0, 0.0,
                  259.83, 298.85), // 259.83-298.85K
        /*density*/                // 687.6 * si::kilogram/si::meter3,
            get_dippr_temperature_function_105
                (si::kelvin, 27.026 * si::gram/si::decimeter3,
                 1.3413, 0.18589, 456.65, 0.28206,
                 259.83, 456.65), // 259.83-456.65K
        /*vapor_pressure*/         
            get_dippr_temperature_function_101
                (si::kelvin, si::pascal,
                 36.75, -3927.1, -2.1245, 3.89e-17, 6.0,
                 259.83, 456.65),
        /*surface_tension*/        
            get_dippr_quartic_temperature_function_100
                (si::celcius, si::dyne/si::centimeter, 
                20.62, -0.1155, 7e-5, 0.0, 0.0,
                -13.3, 25.0), 
        /*refractive_index*/       1.2614,
        /*extinction_coefficient*/ 7.63, // Moore 2010, 95K, 5000cm-1

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
                     std::vector<double>{-71.0,  -47.7,  -17.8,  25.9   }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/                  1.37,    // Moore 2010, 95K 5000 cm-1
            /*extinction_coefficient*/            1.876e-4, // Moore 2010, 95K 5000 cm-1
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