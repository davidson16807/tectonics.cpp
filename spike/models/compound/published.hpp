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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  9.73e1, 6.05e2, 7.37e3, 1.65e4, 2.98e4, 6.50e4, 9.73e4, 1.38e5, 1.62e5, 2.63e5, 3.35e5, 4.39e5, 5.15e5, 5.89e5, 6.93e5, 9.82e5, 1.25e6, 1.64e6, 2.08e6, 2.38e6, 2.41e6, 2.44e6, 2.47e6, 2.53e6, 2.59e6, 2.63e6, 2.73e6, 2.78e6, 2.93e6, 2.98e6, 3.05e6, 3.08e6, 5.11e6, 5.63e6, 6.04e6, 6.45e6, 6.86e6, 8.04e6, 9.68e6, 1.08e7, 1.24e7, 1.37e7, 2.37e7, 3.94e7, 6.98e7, 1.69e8 },
          std::vector<double>{  -24.98, -24.44, -23.93, -23.46, -23.46, -22.97, -23.70, -23.77, -23.11, -24.44, -22.46, -25.14, -24.47, -25.68, -25.10, -27.10, -28.15, -29.10, -30.25, -29.38, -29.28, -29.28, -29.47, -29.22, -29.47, -29.55, -29.28, -29.21, -29.27, -28.95, -28.71, -28.69, -25.41, -21.62, -21.41, -21.51, -21.76, -21.09, -20.98, -20.74, -20.82, -20.75, -20.83, -21.08, -21.54, -22.44 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 2.080 * si::joule / (si::gram * si::kelvin),                     // wikipedia
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.01766, 0.80539, 0.00707, 0.69586, 1.42782,
                 300.0, 1273.2,  0.0, 10.0), 
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
                 300.0, 1273.2,  0.0, 10.0), 
                // water, mean error: 1.2%, max error: 3.5%, range: 300-1273.2K, 0-10MPa, stp estimate: 8.765
        /*density*/                missing(),
        /*refractive_index*/       spectral_invariant(1.000261)                                        // engineering toolbox
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 4.1813 * si::joule / (si::gram * si::kelvin),                    // wikipedia
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (18.01528 * si::kilogram * si::kelvin), 
                276370.0, -2090.1, 8.125, -0.014116, 9.3701e-6,
                273.16*si::kelvin, 533.15*si::kelvin), 
        /*thermal_conductivity*/   // 0.6062 * si::watt / (si::meter * si::kelvin), 
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 -0.432, 0.0057255, -0.000008078, 1.861e-9, 0.0,
                  273.15, 633.15), 
        /*dynamic_viscosity*/      
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -52.843, 3703.6, 5.866, -5.879e-29, 10.0,
                  273.16, 646.15), // 273.16-646.15K
        /*density*/                // 997.0 * si::kilogram/si::meter3,                                
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
            // get_dippr_liquid_vapor_pressure_temperature_function
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
            get_linear_interpolated_refractive_index_function
                (si::micrometer, 
                 std::vector<double>{0.2,   0.3,   0.425,  0.55, 0.675,   0.8, 0.925,   1.4,   2.4,   2.8,  3.05,  3.3,   5.6,   6.6,   7.6,  13.0,  15.5,  18.0,  21.0,  26.0,  32.0,  42.0,  60.0, 110.0, 160.0,200.0},
                 std::vector<double>{1.396, 1.349, 1.338, 1.333, 1.331, 1.329, 1.328, 1.321, 1.279, 1.142, 1.426, 1.45, 1.289, 1.334, 1.302, 1.146, 1.297, 1.423, 1.487, 1.539, 1.546, 1.522, 1.703, 1.966, 2.081, 2.13}),
                // Hale (1973)
        /*extinction_coefficient*/ 
            get_linear_interpolated_refractive_index_function
                (si::micrometer, 
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
            /*vapor_pressure*/                    //138.268 * si::megapascal,
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{-17.3,  11.2,   51.6,   100.0  }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/       //1.33336,
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{0.0443, 0.0528, 0.067,  0.0886, 0.1181, 0.137,  0.1468, 0.158,  0.177,  0.197,  0.41,   0.51,   0.81,   1.515,  1.76,   1.923,  2.29,   2.52,   2.817,  2.985,  3.175,  3.39,   3.775,  4.56,   5.714,  6.329,  7.143,  8.333,  10.31,  11.9,   14.08,  17.24,  22.22,  34.48,  46.15,  55,     62.5,   76.29,  210, 1300.0},
                     std::vector<double>{0.8228, 0.8505, 0.9679, 1.3807, 1.3783, 1.2849, 1.5729, 1.6309, 1.4462, 1.3974, 1.3185, 1.3126, 1.3047, 1.2914, 1.2843, 1.2771, 1.2545, 1.2232, 1.0657, 1.018, 1.6336, 1.5086, 1.385, 1.3482, 1.2933, 1.3178, 1.3236, 1.2964, 1.1501, 1.2546, 1.5701, 1.5303, 1.4575, 1.253, 1.2543, 1.7955, 1.6372, 1.8504, 1.8114, 1.7868}),
                    // Warren (2008)
            /*extinction_coefficient*/ 
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
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
        /*isobaric_specific_heat_capacity*/ // 1.341 * si::joule / (si::gram * si::kelvin),
            get_sigmoid_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.00115, 0.62179, 0.00000, 0.00000, 2.54371, 2268.64874, -2637.49785, -0.99334), 
                // nitrogen, mean error: 0.8%, max error: 5.4%, range: 163.15-2013.2K, 0-3MPa, stp estimate: 1.013
        /*thermal_conductivity*/   // 0.0234 * si::watt / (si::meter * si::kelvin),                    // wikipedia
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00026, 1.21142, 0.00037, 0.75287, -0.00165,
                 113.15, 2013.2,  0.0, 3.0), 
                // nitrogen, mean error: 0.8%, max error: 5.0%, range: 113.15-2013.2K, 0-3MPa, stp estimate: 0.024
        /*dynamic_viscosity*/      // 1.76e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.04908, 2.30670, 0.50611, 0.64605, -2.56297,
                 113.15, 2013.2,  0.0, 3.0), 
                // nitrogen, mean error: 1.5%, max error: 5.1%, range: 113.15-2013.2K, 0-3MPa, stp estimate: 16.416
        /*density*/                missing(),
        /*refractive_index*/       
            field::SpectralFunction<double>([](
                const si::wavenumber nlo, 
                const si::wavenumber nhi, 
                const si::pressure p, 
                const si::temperature T
            ) {
                double l = (2.0 / (nhi+nlo) / si::micrometer);
                double invl2 = 1.0/(l*l);
                return 1.0 + 6.497378e-5 + 3.0738649e-2 / (144.0 - invl2);
            }) // Peck (1966)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 2.042 * si::joule / (si::gram * si::kelvin),
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (28.0134 * si::kilogram * si::kelvin), 
                281970.0, -12281.0, 248.0, -2.2182, 0.0074902,
                63.15*si::kelvin, 112.0*si::kelvin), 
        /*thermal_conductivity*/                  
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.2654, -0.001677,  0.0,  0.0,  0.0,
                  63.15, 124), 
        /*dynamic_viscosity*/      // 157.9 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 16.004, -181.61, -5.1551, 0.0, 0.0,
                  63.15, 124), // 63.15-124K
        /*density*/                // state_invariant(0.4314*si::gram/si::centimeter3), // Johnson (1960), @ si::atmosphere, 125.01*si::kelvin
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 28.013  * si::gram/si::decimeter3,
                 3.2091, 0.2861, 126.2, 0.2966,
                 63.15, 126.2), // 63.15-126.2K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
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
            /*refractive_index*/                  spectral_invariant(1.25), // wikipedia
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
            /*refractive_index*/                  spectral_invariant(1.25),                                             // wikipedia
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  5.58e6, 5.72e6, 6.36e6, 6.93e6, 7.26e6, 7.40e6, 7.50e6, 7.65e6, 7.74e6, 7.86e6, 8.05e6, 8.24e6, 8.36e6, 8.43e6, 8.52e6, 8.66e6, 8.80e6, 8.88e6, 8.99e6, 9.06e6, 9.20e6, 9.28e6, 9.75e6, 1.05e7, 1.13e7, 1.22e7, 1.36e7, 1.46e7, 1.83e7, 2.14e7, 3.50e7, 6.28e7, 1.11e8, 4.23e8  },
          std::vector<double>{  -24.99, -22.55, -21.15, -20.81, -20.86, -21.15, -21.70, -22.61, -22.16, -23.10, -21.82, -24.11, -22.09, -24.11, -21.91, -23.78, -21.50, -23.63, -23.80, -22.07, -22.14, -22.21, -21.36, -21.04, -21.01, -20.68, -20.60, -20.65, -20.59, -20.70, -20.81, -21.28, -21.79, -23.05  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/        // 0.980 * si::joule / (si::gram * si::kelvin),              
            get_sigmoid_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                -0.00022, 0.00063, 0.00000, 0.00000, 1.11067, 1268.80242, -836.03510, 0.17582), 
                // oxygen, mean error: 0.7%, max error: 3.3%, range: 204.36-1004.4K, 0-3MPa, stp estimate: 0.907
        /*thermal_conductivity*/   // 0.0238 * si::watt / (si::meter * si::kelvin),                    // wikipedia
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00023, 1.29450, 0.00018, 0.88149, -0.00130,
                 104.36, 1004.4,  0.0, 3.0), 
                // oxygen, mean error: 0.8%, max error: 4.2%, range: 104.36-1004.4K, 0-3MPa, stp estimate: 0.025
        /*dynamic_viscosity*/      // 2.04e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                -0.08909, 0.00000, 0.39475, 0.70840, -2.08620,
                 104.36, 1004.4,  0.0, 3.0), 
                // oxygen, mean error: 2.1%, max error: 8.1%, range: 104.36-1004.4K, 0-3MPa, stp estimate: 18.827
        /*density*/                missing(),
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
        /*isobaric_specific_heat_capacity*/ // 1.699 * si::joule / (si::gram * si::kelvin),             
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (31.9988 * si::kilogram * si::kelvin), 
                175430.0, -6152.3, 113.92, -0.92382, 0.0027963,
                4.36*si::kelvin, 142.0*si::kelvin), 
            // get_interpolated_temperature_function
            //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
            //       std::vector<double>{54.4,100.0,120.0,140.0,150.0},
            //       std::vector<double>{2.00,1.738,1.927,2.691,6.625}),
        /*thermal_conductivity*/   // 0.1514 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.2741, -0.00138, 0.0,  0.0,  0.0,
                  60, 150), 
            // get_interpolated_pressure_temperature_function
            //     (si::kelvin, si::milliwatt/(si::centimeter*si::kelvin),
            //                            std::vector<double>{73.16,     133.16,   173.16,   313.16},
            //        1.0*si::atmosphere, std::vector<double>{0.0651,    0.1209,   0.14607,  0.1582},
            //      100.0*si::atmosphere, std::vector<double>{1.744,     1.0118,   0.4617,   0.3349}), // Johnson (1960)
        /*dynamic_viscosity*/      // 188.0 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -4.1476, 94.04, -1.207, 0.0, 0.0,
                  54.36, 150), // 54.36-150K
        /*density*/                // 1.141 * si::gram/si::milliliter,
            // field::StateSample<si::density>(0.6779*si::gram/si::centimeter3, si::atmosphere, 149.8*si::kelvin), // Johnson (1960)
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 31.9988 * si::gram/si::decimeter3,
                 3.9143, 0.28772, 154.58, 0.2924,
                 54.35, 154.58), // 54.35-154.58K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 51.245, -1200.2, -6.4361, 0.028405, 1.0,
                  54.36, 154.58), // 54.36-154.58K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 154.59, 0.03843, 1.225, 0.0, 0.0, 0.0, 0.0,
                 55.7, 152.08), // Mulero (2012)
        /*refractive_index*/       spectral_invariant(1.2243),
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{ 8.58e2,  8.58e3,  2.75e4,  3.26e4,  3.78e4,  6.70e4,  8.58e4,  9.53e4,  1.00e5,  1.07e5,  1.11e5,  1.18e5,  1.27e5,  1.32e5,  1.36e5,  1.45e5,  1.53e5,  1.56e5,  1.60e5,  1.66e5,  1.69e5,  1.73e5,  1.87e5,  1.92e5,  2.00e5,  2.23e5,  2.32e5,  2.37e5,  2.40e5,  2.47e5,  2.53e5,  2.60e5,  2.69e5,  2.76e5,  2.88e5,  3.10e5,  3.41e5,  3.59e5,  3.72e5,  3.90e5,  4.30e5,  5.01e5,  5.74e5,  6.20e5,  6.57e5,  6.99e5,  8.13e5,  9.54e5,  1.07e6,  1.29e6,  1.32e6,  2.50e6,  4.95e6,  5.00e6,  6.14e6,  6.73e6,  7.17e6,  7.62e6,  8.39e6,  8.90e6,  9.21e6,  1.11e7,  1.21e7,  1.29e7,  1.52e7,  2.91e7,  4.65e7,  1.26e8,  1.89e8,  2.41e8,  4.42e8 },
          std::vector<double>{ -33.32,  -35.77,  -38.01,  -37.15,  -32.27,  -22.83,  -28.57,  -26.72,  -28.50,  -26.46,  -30.22,  -31.21,  -28.04,  -30.02,  -28.11,  -32.47,  -32.60,  -33.79,  -36.49,  -37.22,  -35.11,  -32.07,  -28.17,  -26.66,  -28.64,  -25.03,  -22.21,  -21.30,  -26.74,  -27.46,  -30.28,  -28.25,  -31.00,  -29.30,  -32.57,  -29.89,  -28.05,  -24.31,  -23.66,  -28.38,  -32.11,  -24.84,  -31.52,  -26.54,  -30.21,  -26.02,  -31.13,  -28.70,  -37.36,  -29.75,  -35.98,  -35.98,  -29.16,  -27.60,  -22.96,  -22.26,  -22.30,  -22.01,  -23.52,  -20.24,  -21.06,  -20.04,  -20.86,  -20.36,  -20.45,  -20.56,  -20.83,  -21.74,  -22.21,  -21.69,  -21.96 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 36.61 * si::joule / (44.01 * si::gram * si::kelvin),             // wikipedia
            get_sigmoid_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.00034, 3.89562, 0.00000, 0.00000, 3.24445, 600.78481, -562.26128, -1.80573), 
                // carbon dioxide, mean error: 0.8%, max error: 2.8%, range: 266.59-1116.6K, 0-3MPa, stp estimate: 0.828
        /*thermal_conductivity*/   // 0.01662 * si::watt / ( si::meter * si::kelvin ),                 // wikipedia
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00032, 1.14102, 0.00027, 0.83072, -0.01434,
                 266.59, 1116.6,  0.0, 3.0), 
                // carbon dioxide, mean error: 0.8%, max error: 2.7%, range: 266.59-1116.6K, 0-3MPa, stp estimate: 0.015
        /*dynamic_viscosity*/      // 1.47e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.05967, 1.75501, 0.18576, 0.79011, -1.65245,
                 266.59, 1116.6,  0.0, 3.0), 
                // carbon dioxide, mean error: 1.5%, max error: 3.2%, range: 266.59-1116.6K, 0-3MPa, stp estimate: 13.980
        /*density*/                missing(),
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
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (44.0095 * si::kilogram * si::kelvin), 
                -8304300.0, 104370.0, -433.33, 0.60052, 0.0,
                220*si::kelvin, 290*si::kelvin), 
            // get_interpolated_temperature_function
            //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
            //       std::vector<double>{216.6,240.0,260.0,280.0,290.0,300.0},
            //       std::vector<double>{1.953,2.051,2.255,2.814,3.676,8.698}),
        /*thermal_conductivity*/   // 0.087 * si::watt / (si::meter * si::kelvin),                     // wikipedia
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.4406, -0.0012175, 0.0,  0.0,  0.0,
                  216.58, 300), 
        /*dynamic_viscosity*/      // 0.0712 * si::millipascal*si::second,                             // wikipedia data page
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 18.775, -402.92, -4.6854, -6.9171e-26, 10.0,
                  216.58, 303.15), // 216.58-303.15K
        /*density*/                // 1101.0 * si::kilogram/si::meter3,
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 44.01 * si::gram/si::decimeter3,
                 2.768, 0.26212, 304.21, 0.2908,
                 216.58, 304.21), // 216.58-304.21K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 47.0169, -2839.0, -3.86388, 2.81e-16, 6.0,
                  216.58, 304.21), // 216.58-304.21K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 304.13, 0.07863, 1.254, 0.0, 0.0, 0.0, 0.0,
                 216.55, 304.11), // Mulero (2012)
        /*refractive_index*/       spectral_invariant(1.6630),
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
                get_linear_interpolated_refractive_index_function
                    (si::micrometer,
                     std::vector<double>{0.033, 0.051, 0.055, 0.060, 0.063, 0.064, 0.068, 0.078, 0.086, 0.090, 0.098, 0.103, 0.109, 0.115, 0.120, 0.128, 0.142, 0.168, 0.238, 0.264, 0.272, 0.437, 0.984, 0.988, 1.169, 3.140, 3.235, 3.389, 3.838, 4.094, 4.189, 4.220, 4.269, 4.301, 4.384, 4.606, 4.284, 4.724, 4.836, 4.942, 5.321, 5.527, 5.596, 6.687, 7.137, 8.121, 8.249, 8.278, 9.627, 9.886 }, 
                     std::vector<double>{1.448, 0.505, 0.505, 0.793, 0.884, 1.567, 0.876, 0.703, 0.925, 0.793, 1.032, 0.934, 1.057, 0.793, 3.793, 2.318, 1.815, 1.601, 1.469, 1.483, 1.413, 1.403, 1.379, 1.410, 1.384, 1.363, 1.397, 1.434, 1.364, 1.291, 0.981, 3.015, 0.452, 1.806, 1.542, 1.456, 1.258, 3.207, 0.385, 1.413, 1.423, 1.567, 1.342, 1.328, 1.462, 1.448, 1.483, 1.427, 1.406, 1.403 }), // Warren (1986)
            /*extinction_coefficient*/
                get_linear_interpolated_refractive_index_function
                    (si::micrometer,
                     std::vector<double>{0.051,    0.053,    0.057,    0.066,    0.071,    0.081,    0.086,    0.092,    0.097,    0.104,    0.109,    0.116,    0.122,    0.125,    0.351,    0.481,    0.992,    1.003,    2.442,    2.555,    2.653,    2.684,    2.714,    2.755,    2.755,    2.861,    3.028,    3.085,    3.252,    3.302,    3.327,    3.587,    3.753,    4.017,    4.109,    4.140,    4.218,    4.299,    4.315,    4.464,    4.481,    4.813,    4.979,    5.074,    5.132,    5.289,    5.660,    6.173,    6.483,    6.681,    6.912,    7.096,    7.259,    7.481,    7.594,    7.946,    8.127,    8.408,    8.536,    8.797,    9.100,    9.450,    9.703,    9.924,    0.075,    0.620,    1.536,    1.979,    2.345,    2.818,    3.820,    4.350,    4.900,    5.355,    5.588,    6.308,    9.100,    2.202,    3.937,    4.855,    9.194,    7.332,    0.000,    7.168,    6.779,    0.650   }, 
                     std::vector<double>{1.17e-01, 3.14e-01, 4.96e-01, 3.39e-01, 3.39e-01, 6.98e-01, 5.35e-01, 8.45e-01, 7.25e-01, 1.02e+00, 1.10e+00, 2.74e+00, 2.15e-01, 9.89e-03, 6.58e-07, 7.10e-07, 2.22e-06, 2.17e-06, 7.66e-06, 1.22e-05, 2.45e-02, 8.01e-04, 1.50e-03, 2.37e-02, 1.40e-03, 3.77e-05, 8.37e-05, 8.65e-05, 3.30e-05, 5.62e-05, 2.21e-05, 2.39e-06, 2.83e-06, 1.64e-02, 7.66e-03, 1.34e-02, 2.31e+00, 8.10e-02, 1.30e-02, 3.69e-03, 1.08e-03, 8.37e-05, 3.65e-05, 1.68e-04, 3.90e-05, 1.48e-05, 4.97e-06, 2.92e-06, 4.97e-06, 7.58e-05, 1.12e-04, 4.03e-05, 6.21e-05, 1.16e-04, 6.86e-05, 2.37e-05, 3.65e-05, 2.21e-05, 1.03e-05, 3.69e-06, 4.35e-06, 1.14e-05, 7.17e-06, 7.17e-06, 1.06e-05, 2.15e-05, 7.53e-03, 4.57e-03, 4.88e-03, 1.16e-02, 2.27e-03, 7.05e-03, 1.95e+00, 9.46e-02, 2.11e-02, 4.27e-03, 3.89e-04, 1.48e-04, 1.25e-04, 1.58e-04, 3.06e-03, 1.23e-01, 2.19e-03, 3.74e-03, 1.90e-01, 3.50e-03}), // Warren (1986)
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  9.47e5, 9.92e5, 1.02e6, 1.05e6, 1.07e6, 1.11e6, 1.13e6, 1.14e6, 1.16e6, 1.17e6, 1.19e6, 1.21e6, 1.25e6, 1.28e6, 1.33e6, 1.38e6, 1.40e6, 1.43e6, 1.44e6, 1.48e6, 1.50e6, 1.57e6, 1.61e6, 1.70e6, 1.73e6, 1.80e6, 1.84e6, 1.91e6, 1.96e6, 2.03e6, 2.05e6, 2.10e6, 2.15e6, 2.17e6, 2.25e6, 2.28e6, 2.30e6, 6.56e6, 7.00e6, 7.66e6, 8.65e6, 1.08e7, 2.83e7, 4.90e7, 1.82e8, 2.23e8, 2.38e8, 2.57e8, 3.12e8, 3.24e8, 3.61e8, 3.94e8, 4.45e8, 4.89e8, 5.15e8, 5.95e8, 6.36e8, 7.38e8, 7.91e8, 8.66e8, 9.40e8  },
          std::vector<double>{  -28.89, -27.28, -27.50, -29.05, -29.13, -27.31, -27.03, -28.16, -27.76, -28.91, -28.60, -30.96, -28.43, -28.67, -29.96, -27.88, -29.30, -28.99, -29.93, -29.91, -29.27, -30.48, -28.66, -31.16, -29.91, -31.27, -29.36, -31.86, -30.34, -32.07, -29.75, -31.26, -31.40, -30.69, -32.07, -30.48, -31.86, -27.37, -22.93, -20.76, -20.71, -20.22, -20.83, -21.53, -22.69, -29.75, -23.23, -21.93, -25.62, -29.16, -30.28, -28.11, -26.80, -30.93, -22.60, -27.39, -31.20, -27.26, -33.23, -27.72, -33.23  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 2.218 * si::joule / (si::gram * si::kelvin),                     
            get_sigmoid_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.02624, 1.04690, 0.00000, 0.00000, 1.04105, 264.87732, 492.16481, 2.86235), 
                // methane, mean error: 0.7%, max error: 2.1%, range: 140.69-640.69K, 0-3MPa, stp estimate: 2.201
        /*thermal_conductivity*/   // 34.4 * si::milliwatt / ( si::meter * si::kelvin ),               // Huber & Harvey         
            get_sigmoid_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00081, 1.07534, 0.00000, 0.00000, 0.28162, 818.25156, 1044.83566, 0.22399), 
                // methane, mean error: 1.1%, max error: 3.2%, range: 140.69-640.69K, 0-3MPa, stp estimate: 0.031
        /*dynamic_viscosity*/      // 1.10e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.12223, 1.53060, 0.09521, 0.83600, -0.21904,
                 140.69, 640.69,  0.0, 3.0), 
                // methane, mean error: 1.9%, max error: 5.2%, range: 140.69-640.69K, 0-3MPa, stp estimate: 10.148
        /*density*/                missing(),
        /*refractive_index*/       // 1.000444,
            get_interpolated_refractive_index_function
                (si::micrometer, 
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
            // get_interpolated_temperature_function
            //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
            //       std::vector<double>{90.7,150.0,170.0,180.0,190.0},
            //       std::vector<double>{3.36,4.047,5.187,7.292,94.01}),
        /*thermal_conductivity*/   // 0.1931 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.41768, -0.0024528, 3.5588e-6,  0.0,  0.0,
                  90.69, 180), 
        /*dynamic_viscosity*/      // 118.6 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -6.1572, 178.15, -0.95239, -9.0606e-24, 10.0,
                  90.69, 188), // 90.69-188K
        /*density*/                
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 16.043 * si::gram/si::decimeter3,
                 2.9214, 0.28976, 190.56, 0.28881,
                 90.69, 190.56), // 90.69-190.56K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 39.205, -1324.4, -3.4366, 0.000031019, 2.0,
                  90.69, 190.56), // 90.69-190.56K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 190.56, 0.03825, 1.191, -0.006024, 5.422, -0.0007065, 0.6161,
                 90.67, 188.84), // Mulero (2012)
        /*refractive_index*/       spectral_invariant(1.2730), 
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
                get_linear_interpolated_refractive_index_function_of_wavenumbers
                    (1.0/si::centimeter, 
                     std::vector<double>{ 4017.56,  3126.23,  3042.81,  3012.07,  2994.51,  2959.38,  1365.53,  1321.62,  1299.67,  1282.10, 1251.37 },
                     std::vector<double>{   1.503,   1.503,   1.528,   1.672,   1.538,   1.503,   1.506,   1.522,   2.095,   1.519,   1.503,   1.509 }),
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_log_at_wavenumbers
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
                get_linear_interpolated_refractive_index_function_of_wavenumbers
                    (1.0/si::centimeter, 
                     std::vector<double>{ 4017.56,  3126.23,  3042.81,  3012.07,  2994.51,  2959.38,  1365.53,  1321.62,  1299.67,  1282.10, 1251.37 },
                     std::vector<double>{   1.503,   1.503,   1.528,   1.672,   1.538,   1.503,   1.506,   1.522,   2.095,   1.519,   1.503,   1.509 }),
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_log_at_wavenumbers
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
            get_interpolated_pressure_temperature_function
                ( si::kelvin, si::kilojoule/(si::kilogram*si::kelvin),
                                       std::vector<double>{90.0,  120.0, 200.0, 1000.0},
                   0.1*si::megapascal, std::vector<double>{0.5654,0.5347,0.5236,0.5210},
                   1.0*si::megapascal, std::vector<double>{0.3795,0.3682,0.5556,0.3124},
                  10.0*si::megapascal, std::vector<double>{1.085, 1.163, 1.215, 0.5271}),
        /*thermal_conductivity*/   // 0.016 * si::watt / ( si::meter * si::kelvin ),                   // wikipedia
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00034, 1.11244, 0.00029, 0.73962, -0.00193,
                 133.81, 683.81,  0.0, 3.0), 
                // argon, mean error: 1.2%, max error: 2.7%, range: 133.81-683.81K, 0-3MPa, stp estimate: 0.016
        /*dynamic_viscosity*/      // 2.23e-5 * si::pascal * si::second,                               // engineering toolbox, at 20 C
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.41072, 0.79782, 0.22045, 0.81080, -0.33331,
                 133.81, 683.81,  0.0, 3.0), 
                // argon, mean error: 1.7%, max error: 4.2%, range: 133.81-683.81K, 0-3MPa, stp estimate: 20.565
        /*density*/                missing(),
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
        /*isobaric_specific_heat_capacity*/ //1.078 * si::joule / (si::gram * si::kelvin),              
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (39.948 * si::kilogram * si::kelvin), 
                134390.0, -1989.4, 11.043, 0.0, 0.0,
                83.78*si::kelvin, 135.0*si::kelvin), 
            // get_interpolated_temperature_function
            //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
            //       std::vector<double>{83.8,100.0,120.0,140.0,150.0},
            //       std::vector<double>{1.12,1.154,1.332,2.225,23.60}),
        /*thermal_conductivity*/   // 0.1232 * si::watt / (si::meter * si::kelvin),                    // Timmerhaus (1989)
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.1819, -0.0003176, -0.00000411, 0.0, 0.0,
                  83.78, 150), 
        /*dynamic_viscosity*/      // 252.1 * si::kilogram / (si::meter * 1e6*si::second),             // Timmerhaus (1989)
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal * si::second, 
                 -8.8685, 204.29, -0.38305, -1.294e-22, 10.0,
                  83.78, 150.0), // 83.78-150.0K
        /*density*/                
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 39.948 * si::gram/si::decimeter3,
                 3.8469, 0.2881, 150.86, 0.29783,
                 3.78, 150.86), // 83.78, 150.86K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 42.127, -1093.1, -4.1425, 0.000057254, 2.0,
                  83.78, 150.86), // 83.78-150.86K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 150.87, 0.037, 1.25, 0.0, 0.0, 0.0, 0.0,
                 83.82, 148.15), // Mulero (2012)
        /*refractive_index*/       spectral_invariant(1.23),
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
            /*refractive_index*/                  spectral_invariant(1.2703),
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

    /*molecular_absorption_cross_section*/ spectral_invariant(1e-35* si::meter2),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 9.78 * si::joule / (si::gram * si::kelvin), 
            get_interpolated_pressure_temperature_function
                ( si::kelvin, si::kilojoule/(si::kilogram*si::kelvin),
                                       std::vector<double>{20.0,  40.0,  100.0, 1000.0},
                   0.1*si::megapascal, std::vector<double>{5.250, 5.206, 5.194, 5.193 },
                   1.0*si::megapascal, std::vector<double>{5.728, 5.317, 5.206, 5.193 },
                  10.0*si::megapascal, std::vector<double>{5.413, 5.721, 5.303, 5.188 }),
        /*thermal_conductivity*/   // 155.7 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                -0.15319, 1.77283, 0.00284, 0.70030, 0.00337,
                 52.177, 1502.2,  0.0, 0.1), 
                // helium, mean error: 0.8%, max error: 4.1%, range: 52.177-1502.2K, 0-0.1MPa, stp estimate: 0.145
        /*dynamic_viscosity*/      // 1.96e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                -0.19093, 1.50359, 0.28815, 0.73057, 1.34631,
                 52.177, 1502.2,  0.0, 0.1), 
                // helium, mean error: 0.1%, max error: 0.7%, range: 52.177-1502.2K, 0-0.1MPa, stp estimate: 18.702
        /*density*/                missing(),
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
                    + 0.01470091/(423.98  - invl2);
            }) // Mansfield (1969)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 4.545 * si::joule / (si::gram * si::kelvin), 
            get_interpolated_temperature_function
                ( si::kelvin, si::joule/(si::gram * si::kelvin),
                  std::vector<double>{3.0,3.5, 4.0, 4.5, 5.0  },
                  std::vector<double>{2.6,3.41,4.52,6.78,20.24}),
        /*thermal_conductivity*/   
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 -0.013833, 0.022913, -0.0054872, 0.0004585, 0.0,
                  2.2, 4.8), 
        /*dynamic_viscosity*/      // 3.57 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -9.6312, -3.841, -1.458, -1.065e-8, 10.0,
                  2.2, 5.1), // 2.2-5.1K
        /*density*/                
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 4.0026 * si::gram/si::decimeter3,
                 7.2475, 0.41865, 5.2, 0.24096,
                 2.2, 5.2), // 2.2-5.2K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 11.533, -8.99, 0.6724, 0.2743, 1.0,
                  1.76, 5.2), // 1.76-5.2K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 5.19, 0.0004656, 1.040, 0.001889, 2.468, -0.002006, 2.661,
                 0.0, 5.11), // Mulero (2012)
        /*refractive_index*/       // 1.02451,
            get_dewaele2003_pressure_function
                (si::kilobar, 1.0, 0.8034, 0.20256, 1.0, 0.12763, 0.8, 115.0), // Dewaele (2003)
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
            /*refractive_index*/                  
                get_dewaele2003_pressure_function
                    (si::kilobar, 1.0, -0.1033, 1.0, 1.0, 0.052, 117.0, 202.0), // Dewaele (2003)
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{   5e6, 8.06e6, 8.48e6, 9.97e6, 1.05e7, 1.13e7, 1.41e7, 2.66e7, 5.74e7  },
          std::vector<double>{ -29.5, -23.05, -22.54, -20.41, -20.44, -20.79, -20.97, -21.66, -22.68  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 12.24 * si::joule / (si::gram * si::kelvin),       
            get_interpolated_temperature_function
                ( si::kelvin, si::joule/(si::gram * si::kelvin),
                  std::vector<double>{13.8,16.0,20.0,24.0,28.0,32.0},
                  std::vector<double>{10.5,10.9,11.9,14.4,21.5,92.4}),
            // NOTE: this entry produces absurd high values at STP, we need to reimplement this
            // get_sigmoid_exponent_pressure_temperature_function
            //     (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
            //     0.02971, 21.52834, 1116.18449, 51.63246, 3.23568, 107.08994, 116.34534, 11.50257), 
            //     // hydrogen, mean error: 0.6%, max error: 2.0%, range: 63.957-1014K, 0-1MPa, stp estimate: 14.153
        /*thermal_conductivity*/   // 186.6 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00197, 1.33064, 0.00154, 0.84231, -0.00399,
                 63.957, 1014,  0.0, 1.0), 
                // hydrogen, mean error: 1.3%, max error: 2.5%, range: 63.957-1014K, 0-1MPa, stp estimate: 0.169
        /*dynamic_viscosity*/      // 0.88e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.05907, 1.95272, 0.20949, 0.66373, -0.28287,
                 63.957, 1014,  0.0, 1.0), 
                // hydrogen, mean error: 0.2%, max error: 0.9%, range: 63.957-1014K, 0-1MPa, stp estimate: 8.393
        /*density*/                missing(),
        /*refractive_index*/       // 1.0001392,
            field::SpectralFunction<double>([](
                const si::wavenumber nlo, 
                const si::wavenumber nhi, 
                const si::pressure p, 
                const si::temperature T
            ) {
                double l = (2.0 / (nhi+nlo) / si::micrometer);
                return 1.0 + 0.0148956/(180.7-1.0/(l*l)) + 0.0049037/(92.0-1.0/(l*l));
            }) // Peck & Hung (1977)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 9.668 * si::joule / (si::gram * si::kelvin), 
            // NOTE: this estimate produces bogus results outside its range, we may need to clamp this
            get_dippr_liquid_heat_capacity_temperature_function_114
                (32.97 * si::kelvin, si::joule / (2.01588 * si::kilogram * si::kelvin), 
                66.653, 6765.9, -123.63, 478.27, 0.0,
                13.95*si::kelvin, 32.0*si::kelvin), 
            // get_interpolated_temperature_function
            //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
            //       std::vector<double>{13.8,16.0,20.0,24.0,28.0,32.0},
            //       std::vector<double>{6.92,7.66,9.57,12.5,18.5,68.2}),
        /*thermal_conductivity*/   
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 -0.0917, 0.017678, -0.000382, -3.3324e-6, 1.0266e-7,
                  13.95, 31), 
        /*dynamic_viscosity*/      // 13.06 * si::kilogram / (si::meter * 1e6*si::second), // Timmerhaus (1989)
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -11.661, 24.7, -0.261, -4.10e-16, 10.0,
                  13.95, 33), // 13.95-33K
        /*density*/                // 0.0708 * si::gram/si::milliliter,
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 2.016 * si::gram/si::decimeter3,
                 5.414, 0.34893, 33.19, 0.2706,
                 13.95, 33.19), // 13.95-33.19K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 12.69, -94.896, 1.1125, 0.00032915, 2.0,
                  13.95, 33.19), // 13.95-33.19K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 32.97, -1.4165, 0.63882, 0.746383, 0.659804, 0.675625, 0.619149,
                 13.8, 31.0), // Mulero (2012)
        /*refractive_index*/       // 1.1096,
            get_dewaele2003_pressure_function
                (si::kilobar, 1.0, 0.949, 0.06829, 11.8, 0.2853, 3.0, 49.0), // Dewaele (2003)
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
            /*refractive_index*/                  
                field::SpectralFunction<double>([](
                    const si::wavenumber nlo, 
                    const si::wavenumber nhi, 
                    const si::pressure p, 
                    const si::temperature T
                ) {
                    return 0.95+0.1144*std::pow(std::clamp(p/si::kilobar, 52.0, 350.0), 0.3368);
                }), // Peck & Hung (1977)
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  4.35e6, 4.84e6, 5.26e6, 6.14e6, 6.61e6, 7.28e6, 8.03e6, 9.32e6, 1.06e7, 1.23e7, 2.52e7, 5.80e7, 1.93e8f  },
          std::vector<double>{  -26.43, -21.51, -21.17, -21.60, -21.58, -21.00, -20.92, -20.56, -20.76, -20.44, -20.78, -21.60, -22.61f  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 35.06 * si::joule / (17.031 * si::gram * si::kelvin), // wikipedia data page
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.04652, 0.81959, 0.02155, 0.67889, 1.04112,
                 245.5, 695.5,  0.0, 10.0), 
                // ammonia, mean error: 1.2%, max error: 4.3%, range: 245.5-695.5K, 0-10MPa, stp estimate: 2.020
        /*thermal_conductivity*/   // 25.1 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            get_sigmoid_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00076, 1.10036, 0.00000, 0.00000, 0.07387, 491.21103, 544.10090, 0.05855), 
                // ammonia, mean error: 2.5%, max error: 6.9%, range: 245.5-695.5K, 0-10MPa, stp estimate: 0.023
        /*dynamic_viscosity*/      // 0.99e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.00986, 1.88109, 0.02443, 1.06310, -0.30191,
                 245.5, 695.5,  0.0, 10.0), 
                // ammonia, mean error: 0.7%, max error: 3.5%, range: 245.5-695.5K, 0-10MPa, stp estimate: 9.204
        /*density*/                missing(),
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
        /*isobaric_specific_heat_capacity*/ // 4.700 * si::joule / (si::gram * si::kelvin), // wikipedia
            get_dippr_liquid_heat_capacity_temperature_function_114
                (405.56 * si::kelvin, si::joule / (17.031 * si::kilogram * si::kelvin), 
                61.289, 80925.0, 799.4, -2651.0, 0.0,
                203.15*si::kelvin, 401.15*si::kelvin),
            // get_interpolated_temperature_function
            //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
            //       std::vector<double>{195.5,260.0,300.0,340.0,380.0,400.0},
            //       std::vector<double>{4.202,4.548,4.800,5.385,7.818,22.73}),
        /*thermal_conductivity*/   
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 1.169, -0.002314, 0.0, 0.0,  0.0,
                  195.41, 400.05), 
        /*dynamic_viscosity*/      
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -6.743,598.3,-0.7341,-3.690e-27,10.0,
                  195.41, 393.15), // 195.41-393.15K
        /*density*/                // 681.97 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 17.031 * si::gram/si::decimeter3,
                 3.5383, 0.25443, 405.65, 0.2888,
                 195.41, 405.65), // 195.41-405.65K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
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
                get_linear_interpolated_refractive_index_function_of_wavenumbers
                    (1.0/si::centimeter, 
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{   0.0,  2e5,  7e5,  9e5,1.6e6,  2e6,2.5e6,2.8e6,  3e6,3.5e6,4.6e6,  6e6,7.7e6,1.2e7f },
          std::vector<double>{ -28.0,-26.0,-31.0,-28.0,-24.0,-25.0,-27.0,-24.5,-23.0,-21.0,-22.5,-22.0,-21.0,-21.0  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   
            get_dippr_gas_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 0.0043147, 0.47999, 700.09, 0.0,
                 161.85, 1000.0), // 161.85-1000K
        /*dynamic_viscosity*/      
            get_dippr_gas_viscosity_temperature_function
                (si::kelvin, si::pascal * si::second,
                1.196e-7, 0.84797, 0.0, 0.0,
                80.15, 1000.0), // 80.15-1000K
        /*density*/                missing(),
        /*refractive_index*/       spectral_invariant(1.00052)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (47.9982 * si::kilogram * si::kelvin), 
                60046.0, 281.16, 0.0, 0.0, 0.0,
                90.0*si::kelvin, 150.0*si::kelvin), 
        /*thermal_conductivity*/   
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.17483, 0.00075288, -2.5228e-6, 0.0, 0.0,
                  77.35, 161.85), 
        /*dynamic_viscosity*/      
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -10.94, 415.96, 0.0, 0.0, 0.0,
                  75.55, 208.8), // 75.55-208.8K
        /*density*/                // 1349.0 * si::kilogram / si::meter3, //encyclopedia.airliquide.com
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 47.998 * si::gram/si::decimeter3,
                 3.3592, 0.29884, 261.0, 0.28523,
                 80.15, 261.0), // 80.15-261.0K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 40.067, -2204.8, -2.9351, 7.75e-16, 6.0,
                  80.15, 261), // 80.15-261K
        /*surface_tension*/        state_invariant(38.1 * si::dyne/si::centimeter), // -182.7C, Jasper (1972)
        /*refractive_index*/       spectral_invariant(1.2226),
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  3.16e6, 3.29e6, 3.85e6, 4.15e6, 5.10e6, 5.62e6, 5.95e6, 6.29e6, 6.33e6, 6.66e6, 6.91e6, 7.25e6, 7.31e6, 7.73e6, 8.00e6, 9.68e6, 1.07e7, 1.32e7, 1.54e7, 2.82e7, 4.30e7, 7.11e7, 2.34e8  },
          std::vector<double>{  -28.02, -27.33, -27.38, -26.88, -23.17, -22.83, -23.13, -23.43, -23.43, -21.58, -21.14, -20.67, -22.14, -20.17, -20.53, -20.62, -20.75, -20.37, -20.40, -20.57, -20.85, -21.29, -22.43  }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   // 17.4 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
            get_dippr_gas_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 0.001096, 0.667, 540.0, 0.0,
                 182.3, 1000.0), // 182.3-1000K
        /*dynamic_viscosity*/      // 1.47e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            get_dippr_gas_viscosity_temperature_function
                (si::kelvin, si::pascal * si::second,
                2.1150e-6, 0.4642, 305.7, 0.0,
                182.3, 1000.0), // 182.3-1000 K
        /*density*/                missing(),
        /*refractive_index*/       spectral_invariant(1.000516)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (44.0128 * si::kilogram * si::kelvin), 
                67556.0, 54.373, 0.0, 0.0, 0.0,
                182.3*si::kelvin, 200.0*si::kelvin), 
        /*thermal_conductivity*/   
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.10112, 0.0, 0.0, 0.0,  0.0,
                  277.59, 277.59), 
        /*dynamic_viscosity*/      
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 19.329, -381.68, -4.8618, 0.0, 0.0,
                  210, 283.09), // 210-283.09K
        /*density*/                // 1230.458 * si::kilogram / si::meter3, 
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 44.012 * si::gram/si::decimeter3,
                 2.781, 0.27244, 309.57, 0.2882,
                 182.3, 309.57), // 182.3-309.57K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 96.512, -4045, -12.277, 0.00002886, 2.0,
                  182.3, 309.57),// 182.3-309.57K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 309.52, 0.07087, 1.204, 0.0, 0.0, 0.0, 0.0,
                 182.50, 293.15), // Mulero (2012)
        /*refractive_index*/       spectral_invariant(1.238),
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
            /*refractive_index*/                  spectral_invariant(1.317), // Hudson (2020)
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{2.47e6,2.69e6,2.91e6,3.12e6,3.41e6,3.76e6,4.25e6,4.37e6,4.95e6,5.45e6,6.01e6,6.63e6,7.36e6,7.95e6,8.51e6,8.80e6,9.07e6,9.35e6},
          std::vector<double>{-28.29,-25.86,-25.90,-23.18,-22.04,-22.33,-23.69,-22.55,-20.88,-21.43,-22.48,-21.25,-21.45,-19.92,-21.12,-20.35,-20.88,-20.68}),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ 
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.03124, 1.38702, 0.00214, 0.79655, 0.42765,
                 247.7, 547.7,  0.0, 1.0), 
                // sulfur dioxide, mean error: 0.7%, max error: 2.0%, range: 247.7-547.7K, 0-1MPa, stp estimate: 0.615
        /*thermal_conductivity*/   // 9.6 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            get_dippr_gas_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 10.527, -0.7732, -1333.0, 1506400.0,
                 250.0, 900.0), // 250-900K
        /*dynamic_viscosity*/      // 1.26e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            get_dippr_gas_viscosity_temperature_function
                (si::kelvin, si::pascal * si::second,
                6.8630e-7, 0.6112, 217.0, 0.0,
                197.67, 1000.0), // 197.67-1000K
        /*density*/                missing(),
        /*refractive_index*/       spectral_invariant(1.000686)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (64.0638 * si::kilogram * si::kelvin), 
                85743.0, 5.7443, 0.0, 0.0, 0.0,
                197.67*si::kelvin, 350.0*si::kelvin), 
            // get_interpolated_temperature_function
            //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
            //       std::vector<double>{197.70,280.0,300.0,380.0,400.0,420.0},
            //       std::vector<double>{0.8776,0.811,0.799,0.783,0.792,3.867}),
        /*thermal_conductivity*/   
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.38218, -0.0006254, 0.0, 0.0, 0.0,
                  197.67, 400), 
        /*dynamic_viscosity*/      // 0.368 * si::millipascal*si::second, // pubchem
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 46.223, -1378.0, -8.7475, 0.0, 0.0,
                  225, 400), // 225-400K
        /*density*/                // 389.06 * si::kilogram / si::meter3, // encyclopedia.airliquide.com
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 64.064 * si::gram/si::decimeter3,
                 2.106, 0.25842, 430.75, 0.2895,
                 197.67, 430.75), // 197.67-430.75K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 47.365, -4084.5, -3.6469, 1.80e-17, 6.0,
                 197.67, 430.75),//197.67-430.75K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 430.64, 0.0803, 0.928, 0.0139, 1.570, -0.0114, 0.364,
                 189.15, 373.15), // Mulero (2012)
        /*refractive_index*/        spectral_invariant(1.3396),
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
                get_linear_interpolated_refractive_index_function_of_wavenumbers
                    (1.0/si::centimeter, 
                     std::vector<double>{ 4032.02, 1483.46, 1381.00, 1361.79, 1336.18, 1323.37, 1321.24, 1299.89, 1171.82, 1163.29, 1150.48, 1135.54,  569.90,  552.83,  537.89,  525.08,  522.95,  508.00,  405.55,  405.55 },
                     std::vector<double>{   2.598,   2.598,   2.617,   2.653,   3.233,   3.190,   2.724,   2.604,   2.604,   3.043,   3.052,   2.607,   2.601,   2.623,   3.040,   3.052,   2.613,   2.595,   2.601,   2.601 }),
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_log_at_wavenumbers
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{4.82e6, 5.61e6, 5.83e6, 6.55e6, 7.23e6, 7.65e6, 8.31e6, 9.94e6, 1.11e7, 1.26e7, 1.67e7, 4.05e7, 7.18e7, 1.85e8},
          std::vector<double>{-22.20, -21.43, -21.37, -21.56, -21.49, -21.56, -21.57, -20.71, -20.41, -20.71, -20.60, -20.94, -21.42, -22.55}),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ 
            get_perry_temperature_function(si::kelvin, si::calorie/(30.006 * si::gram*si::kelvin), 8.05, 0.000233, -156300.0, 0.0, 300.0, 5000.0),
        /*thermal_conductivity*/   // 25.9 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
            get_dippr_gas_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 0.0004096, 0.7509, 45.6, 0.0,
                 121.38, 750.0), // 121.38-750K
        /*dynamic_viscosity*/      // 0.0188 * si::millipascal * si::second, //pubchem
            get_dippr_gas_viscosity_temperature_function
                (si::kelvin, si::pascal * si::second,
                1.4670e-6, 0.5123, 125.4, 0.0,
                110.0, 1500.0), // 110-1500K
        /*density*/                missing(),
        /*refractive_index*/       spectral_invariant(1.000297)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (30.0061 * si::kilogram * si::kelvin), 
                -2979600.0, 76602.0, -652.59, 1.8879, 0.0,
                109.5*si::kelvin, 150.0*si::kelvin), 
        /*thermal_conductivity*/   
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.1878, 0.0010293, -0.00000943,  0.0, 0.0,
                  110, 176.4), 
        /*dynamic_viscosity*/      
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -246.65, 3150.3, 49.98, -0.22541, 1.0,
                  109.5, 180.05), // 109.5-180.05K
        /*density*/                // 1230.458 * si::kilogram / si::meter3,  //encyclopedia.airliquide.com
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 30.006 * si::gram/si::decimeter3,
                 5.246, 0.3044, 180.15, 0.242,
                 109.5, 180.15), // 109.5-180.15K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 72.974, -2650.0, -8.261, 9.7e-15, 6.0,
                  109.5, 180.15),// 109.5-180.15K
        /*surface_tension*/        
            get_interpolated_temperature_function
                (si::celcius, si::dyne/si::centimeter,
                 std::vector<double>{-160.0,-156.0,-152.0}, 
                 std::vector<double>{ 26.17, 23.83, 21.49}), // Jasper (1972)
        /*refractive_index*/       spectral_invariant(1.330),
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  4.83e6, 4.85e6, 4.88e6, 5.00e6, 5.02e6, 5.05e6, 5.17e6, 5.19e6, 5.22e6, 5.33e6, 5.36e6, 5.38e6, 5.49e6, 5.51e6, 5.55e6, 5.64e6, 5.67e6, 5.68e6, 5.71e6, 6.02e6, 6.85e6, 7.98e6, 8.42e6, 9.28e6, 1.00e7, 1.05e7, 1.13e7, 1.21e7, 1.38e7, 2.10e7, 4.54e7, 5.15e8 },
          std::vector<double>{  -28.38, -24.93, -28.40, -28.39, -24.91, -28.40, -28.39, -25.16, -28.42, -28.39, -25.52, -28.39, -28.38, -25.72, -28.41, -28.40, -25.96, -23.41, -28.42, -22.47, -20.89, -21.48, -22.01, -20.72, -20.93, -20.48, -20.35, -20.56, -20.56, -20.68, -21.04, -22.55 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 29.0 * si::joule / (28.010 * si::gram * si::kelvin), // wikipedia data page
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.00728, 0.92688, 0.00010, 0.97052, 1.01709,
                 168.16, 518.16,  0.0, 3.0), 
                // carbon monoxide, mean error: 0.3%, max error: 0.8%, range: 168.16-518.16K, 0-3MPa, stp estimate: 1.041
        /*thermal_conductivity*/   // 25.0 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00045, 0.43412, 0.00055, 0.70174, -0.00365,
                 118.16, 518.16,  0.0, 3.0), 
                // carbon monoxide, mean error: 0.8%, max error: 2.5%, range: 118.16-518.16K, 0-3MPa, stp estimate: 0.025
        /*dynamic_viscosity*/      // 1.74e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.26687, 1.09457, 0.33802, 0.70825, -1.67961,
                 118.16, 518.16,  0.0, 3.0), 
                // carbon monoxide, mean error: 1.2%, max error: 2.9%, range: 118.16-518.16K, 0-3MPa, stp estimate: 16.311
        /*density*/                missing(),
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
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.2855, -0.001784, 0.0,  0.0,  0.0,
                  68.15, 125), 
        /*dynamic_viscosity*/      // 0.170 * si::millipascal * si::second, // Johnson (1960)
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -4.9735, 97.67, -1.1088, 0.0, 0.0,
                  68.15, 131.37), // 68.15-131.37K
        /*density*/                
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 28.010 * si::gram/si::decimeter3,
                 2.897, 0.27532, 132.92, 0.2813,
                 68.15, 132.92), // 68.15-132.92K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
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
                get_absorption_coefficient_function_from_log_at_wavenumbers
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
                get_absorption_coefficient_function_from_log_at_wavenumbers
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{ 5.6e6, 6.25e6, 6.73e6, 7.49e6, 8.23e6, 9.26e6, 1.01e7, 1.14e7, 2.42e7, 5.43e7, 1.72e8 },
          std::vector<double>{ -35.0, -25.67, -21.86, -20.50, -20.60, -20.27, -20.25, -20.08, -20.57, -21.44, -22.57 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 52.49 * si::joule / (30.070 * si::gram * si::kelvin), // wikipedia data page
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.05856, 0.85039, 0.00090, 1.21385, 0.86559,
                 140.35, 640.35,  0.0, 3.0), 
                // ethane, mean error: 1.3%, max error: 3.3%, range: 140.35-640.35K, 0-3MPa, stp estimate: 1.687
        /*thermal_conductivity*/   // 21.2 * si::milliwatt / ( si::meter * si::kelvin ),  // Huber & Harvey
            get_sigmoid_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00064, 1.08467, 0.00000, 0.00000, 0.08892, 442.87962, 573.08449, 0.06794), 
                // ethane, mean error: 1.1%, max error: 3.8%, range: 140.35-640.35K, 0-3MPa, stp estimate: 0.018
        /*dynamic_viscosity*/      // 9.4 * si::micropascal*si::second,
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.07538, 2.19443, 0.07385, 0.85870, -0.57044,
                 140.35, 640.35,  0.0, 3.0), 
                // ethane, mean error: 1.2%, max error: 2.9%, range: 140.35-640.35K, 0-3MPa, stp estimate: 8.560
        /*density*/                missing(),
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
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.35758, -0.0011458, 6.1866e-7, 0.0, 0.0,
                  90.35, 300), 
        /*dynamic_viscosity*/      
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -7.0046, 276.38, -0.6087, -3.11e-18, 7.0,
                  90.35, 300), // 90.35-300K
        /*density*/                // 545.0 * si::kilogram/si::meter3,
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 30.070 * si::gram/si::decimeter3,
                 1.9122, 0.27937, 305.32, 0.29187,
                 90.35, 305.32), // 90.35-305.32K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  8.50e6, 8.58e6, 9.19e6, 9.59e6, 9.89e6, 9.99e6, 1.01e7, 1.02e7, 1.03e7, 1.05e7, 1.07e7, 1.09e7, 1.26e7, 1.35e7, 1.41e7, 1.46e7, 1.62e7 },
          std::vector<double>{   -24.2,  -21.8,  -21.5,  -22.1,  -20.3,  -20.6,  -20.3,  -20.7,  -20.2,  -20.2,  -20.6,  -20.3,  -20.5,  -20.3,  -20.4,  -20.4,  -20.5 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   
            get_dippr_gas_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 4.6496e-6, 1.3669, -210.76, 58295.0,
                 273.15, 673.15), // 273.15-673.15K
        /*dynamic_viscosity*/      
            get_dippr_gas_viscosity_temperature_function
                (si::kelvin, si::pascal * si::second,
                1.2780e-8, 1.0631, 340.0, 0.0,
                183.65, 1000.0), // 183.65-1000K
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (27.02534 * si::kilogram * si::kelvin), 
                95398.0, -197.52, 0.3883, 0.0, 0.0,
                259.83*si::kelvin, 298.85*si::kelvin), 
        /*thermal_conductivity*/   
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.43454, -0.0007008,  0.0,  0.0,  0.0,
                  259.83, 298.85), 
        /*dynamic_viscosity*/      
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -21.927, 1266.5, 1.5927, 0.0, 0.0,
                  259.83, 298.85), // 259.83-298.85K
        /*density*/                // 687.6 * si::kilogram/si::meter3,
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 27.026 * si::gram/si::decimeter3,
                 1.3413, 0.18589, 456.65, 0.28206,
                 259.83, 456.65), // 259.83-456.65K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 36.75, -3927.1, -2.1245, 3.89e-17, 6.0,
                 259.83, 456.65),
        /*surface_tension*/        get_jasper_temperature_function(20.62, -0.1155, 7e-5), // Jasper (1972)
        /*refractive_index*/       spectral_invariant(1.2614),
        /*extinction_coefficient*/ spectral_invariant(7.63), // Moore 2010, 95K, 5000cm-1

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
            get_dippr_gas_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 -0.010109, 0.6475, -7332.0, -268e3,
                 293.15, 1000.0), // 293.15-1000K
        /*dynamic_viscosity*/      
            get_dippr_gas_viscosity_temperature_function
                (si::kelvin, si::pascal * si::second,
                1.0613e-7, 0.8066, 52.7, 0.0,
                200.0, 1000.0), // 200-1000K
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 112.4 * si::joule / (46.068*si::gram*si::kelvin), // wikipedia data page
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (46.06844 * si::kilogram * si::kelvin), 
                102640.0, -139.63, -0.030341, 0.0020386, 0.0,
                159*si::kelvin, 390*si::kelvin), 
            // get_interpolated_temperature_function
            //     ( si::kelvin, si::joule/(si::gram * si::kelvin),
            //       std::vector<double>{250.0,280.0,300.0,440.0,480.0,500.0},
            //       std::vector<double>{2.032,2.351,2.597,4.109,5.091,7.255}),
        /*thermal_conductivity*/   // 0.167 * si::watt / ( si::meter * si::kelvin ),
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.2468, -0.000264,  0.0,  0.0,  0.0,
                  159.05, 353.15), 
        /*dynamic_viscosity*/      
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 7.875, 781.98, -3.0418, 0.0, 0.0,
                  200, 440), // 200-440K
        /*density*/                // 0789.3 * si::kilogram/si::meter3,
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 46.068 * si::gram/si::decimeter3,
                 1.6288, 0.27469, 514.0, 0.23178,
                 159.05, 514), // 159.05-514K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 73.304, -7122.3, -7.1424, 2.8853e-6, 2.0,
                 159.05, 514.0), 
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 351.44, 0.05, 0.952, 0.0, 0.0, 0.0, 0.0,
                 180.12, 513.15), // Mulero (2012)
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
        }),
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
            /*refractive_index*/                  spectral_invariant(1.319), // Hudson (2020)
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  2.51e6, 2.67e6, 2.70e6, 2.74e6, 2.83e6, 2.86e6, 2.95e6, 2.98e6, 3.06e6, 3.09e6, 3.18e6, 3.62e6, 4.02e6, 4.44e6 },
          std::vector<double>{   -26.5,  -26.1,  -25.2,  -26.1,  -23.6,  -25.4,  -23.2,  -25.0,  -23.1,  -24.5,  -23.2,  -23.6,  -24.5,  -25.7 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ missing(),
        /*thermal_conductivity*/   
            get_dippr_gas_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 5.2201e-06, 1.417, 0.0, 0.0,
                 253.85, 1000.0), // 253.85-1000K
        /*dynamic_viscosity*/      
            get_dippr_gas_viscosity_temperature_function
                (si::kelvin, si::pascal * si::second,
                1.5948e-5, 0.21516, 1151.1, 0.0,
                167.55, 1000.0), // 167.55-1000K
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (30.02598* si::kilogram * si::kelvin), 
                70077.0, -661.79, 5.9749, -0.01813, 0.00001983,
                155.15*si::kelvin, 253.85*si::kelvin), 
        /*thermal_conductivity*/   
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.336003243, -0.00054, 0.0, 0.0, 0.0,
                  155.15, 253.85), 
        /*dynamic_viscosity*/      // 0.1421 * si::millipascal * si::second, //pubchem
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -7.6591, 603.36, -0.53378, 0.0, 0.0,
                  155.15, 253.85), // 155.15-253.85K
        /*density*/                // 0.8153 * si::kilogram/si::meter3,  // wikipedia
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 30.026 * si::gram/si::decimeter3,
                 3.897011, 0.331636, 420.0, 0.28571,
                 155.15, 420.0), // 155.15-420.0K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 49.3632, -3847.87, -4.09834, 4.64e-17, 6.0,
                  155.15, 420), // 155.15-420K
        /*surface_tension*/        state_invariant(27.3797 * si::dyne/si::centimeter), // 25 °C, PubChem
        /*refractive_index*/       spectral_invariant(1.3714),  // wikipedia
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
            get_dippr_gas_viscosity_temperature_function
                (si::kelvin, si::pascal * si::second,
                5.0702e-8, 0.9114, 0.0, 0.0,
                281.45, 1000.0), // 281.45-1000K
        /*density*/                missing(),
        /*refractive_index*/       missing()
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // 101.3 * si::joule / (46.026*si::gram*si::kelvin), // wikipedia data page
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (46.0257* si::kilogram * si::kelvin), 
                78060.0, 71.54, 0.0, 0.0, 0.0,
                281.45*si::kelvin, 380.0*si::kelvin), 
        /*thermal_conductivity*/   // 0.267 * si::watt / (si::meter * si::kelvin),
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.302, -0.00018, 0.0,  0.0,  0.0,
                  281.45, 373.71), 
        /*dynamic_viscosity*/      // 1.607 * si::millipascal*si::second,
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -48.529, 3394.7, 5.3903, 0.0, 0.0,
                  281.45, 373.71), // 281.45-373.71K
        /*density*/                // 1220.0 * si::kilogram/si::meter3,
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 46.026 * si::gram/si::decimeter3,
                 1.938, 0.24225, 588.0, 0.24435,
                 281.45, 588.0), // 281.45-588.0K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 43.8066, -5131.03, -3.18777, 2.37819e-6, 2.0,
                  281.45, 588), // 281.45-588K
        /*surface_tension*/        
            get_interpolated_temperature_function
                (si::celcius, si::millinewton/si::meter,
                 std::vector<double>{ 25.0,  50.0,  75.0 }, 
                 std::vector<double>{37.13, 34.38, 31.64 }), 
        /*refractive_index*/       spectral_invariant(1.3714),
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
            /*refractive_index*/                  spectral_invariant(1.291), // Hudson (2020)
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 7.99e6, 8.62e6, 9.25e6, 1.00e7, 1.05e7, 1.08e7, 1.13e7, 1.20e7, 1.28e7, 1.33e7, 1.79e7, 2.14e7, 3.37e7, 5.79e7, 1.71e8 },
          std::vector<double>{  -20.19, -20.19, -19.97, -19.91, -19.92, -20.05, -20.10, -20.12, -20.17, -23.26, -23.10, -22.58, -21.35, -21.52, -20.18, -20.24, -21.06, -20.16, -20.43, -20.13, -20.31, -20.33, -20.68, -21.63 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ 
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.00845, 1.30975, 0.00914, 0.70892, 0.16266,
                 148.94, 598.94,  0.0, 3.0), 
                // tetrafluoromethane, mean error: 2.6%, max error: 5.3%, range: 148.94-598.94K, 0-3MPa, stp estimate: 0.651
        /*thermal_conductivity*/   // 16.0 * si::milliwatt/(si::meter*si::kelvin), // Huber & Harvey
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::watt/(si::meter * si::kelvin),
                0.00019, 1.49795, 0.00010, 0.95277, -0.00704,
                 148.94, 598.94,  0.0, 3.0), 
                // tetrafluoromethane, mean error: 3.3%, max error: 8.4%, range: 148.94-598.94K, 0-3MPa, stp estimate: 0.015
        /*dynamic_viscosity*/      
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::micropascal*si::second, 
                0.22721, 1.71531, 0.16433, 0.82188, -0.84952,
                 148.94, 598.94,  0.0, 3.0), 
                // tetrafluoromethane, mean error: 2.0%, max error: 4.9%, range: 148.94-598.94K, 0-3MPa, stp estimate: 15.680
        /*density*/                missing(),
        /*refractive_index*/       spectral_invariant(1.0004823)
    },

    /*liquid*/
    phase::PartlyKnownLiquid {
        /*isobaric_specific_heat_capacity*/ // missing(),
            get_dippr_liquid_heat_capacity_temperature_function_100
                (si::kelvin, si::joule / (88.0043* si::kilogram * si::kelvin), 
                104600.0, -500.6, 2.2851, 0.0, 0.0,
                89.56*si::kelvin, 145.1*si::kelvin), 
        /*thermal_conductivity*/   
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.20771, -0.00078883,  0.0,  0.0,  0.0,
                  89.56, 145.1), 
        /*dynamic_viscosity*/      
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 -9.9212, 300.5, 0.0, 0.0, 0.0,
                  89.56, 145.1), // 89.56-145.1K
        /*density*/                // 1890.0 * si::kilogram/si::meter3, //pubchem
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 88.0  * si::gram/si::decimeter3,
                 1.955, 0.27884, 227.51, 0.28571,
                 89.56, 227.51), // 89.56-227.51K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
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
    get_molecular_absorption_cross_section_function
        ( 1.0/si::meter, si::meter2,
          std::vector<double>{  3.63e6, 3.73e6, 3.75e6, 3.76e6, 3.82e6, 3.86e6, 3.87e6, 3.92e6, 3.95e6, 3.97e6, 4.03e6, 4.04e6, 4.07e6, 4.12e6, 4.14e6, 4.16e6, 4.21e6, 4.23e6, 4.25e6, 4.33e6, 4.52e6, 4.75e6, 4.91e6, 5.03e6, 5.33e6, 5.58e6, 5.85e6, 6.72e6, 7.54e6, 8.11e6, 8.79e6, 9.59e6, 1.03e7, 1.12e7, 1.41e7, 3.33e7, 2.11e8 },
          std::vector<double>{   -25.0,  -23.9,  -23.0,  -23.8,  -23.2,  -21.7,  -23.2,  -22.6,  -21.7,  -23.1,  -22.5,  -21.7,  -23.1,  -22.7,  -21.9,  -23.2,  -22.9,  -22.6,  -23.3,  -23.3,  -24.0,  -23.3,  -20.7,  -20.6,  -20.8,  -19.5,  -20.2,  -21.1,  -20.4,  -20.5,  -20.1,  -20.2,  -20.0,  -20.2,  -19.8,  -20.5,  -22.3 }),

    /*gas*/
    phase::PartlyKnownGas {
        /*isobaric_specific_heat_capacity*/ // 82.44 * si::joule / (79.109 * si::gram * si::kelvin),
            get_exponent_pressure_temperature_function
                (si::kelvin, si::megapascal, si::joule/(si::gram * si::kelvin),
                0.11074, 0.98216, 0.00048, 1.26428, 0.48272,
                 300, 628.7,  0.0, 1.0), 
                // benzene, mean error: 2.6%, max error: 7.1%, range: 300-628.7K, 0-1MPa, stp estimate: 1.069
        /*thermal_conductivity*/   
            get_dippr_gas_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin), 
                 0.00001652, 1.3117, 491, 0.0,
                 339.15, 1000.0), // 339.15-1000K
        /*dynamic_viscosity*/      // 0.75e-5 * si::pascal * si::second, // engineering toolbox, at 20 C
            get_dippr_gas_viscosity_temperature_function
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
            get_dippr_liquid_thermal_conductivity_temperature_function
                (si::kelvin, si::watt / (si::meter * si::kelvin),
                 0.23444, -0.00030572, 0.0,  0.0,  0.0,
                  278.68, 413.1), 
        /*dynamic_viscosity*/      // 0.601 * si::millipascal * si::second, // engineering toolbox, at 300K
            get_dippr_liquid_viscosity_temperature_function
                (si::kelvin, si::pascal* si::second, 
                 7.5117, 294.68, -2.794, 0.0, 0.0,
                  278.68, 545), // 278.68-545K
        /*density*/                // 879.0 * si::kilogram/si::meter3, // Perry
            get_dippr_liquid_density_temperature_function
                (si::kelvin, 79.102 * si::gram/si::decimeter3,
                 1.0259, 0.266666, 562.05, 0.28394,
                 278.68, 562.05), // 278.68-562.05K
        /*vapor_pressure*/         
            get_dippr_liquid_vapor_pressure_temperature_function
                (si::kelvin, si::pascal,
                 83.107, -6486.2, -9.2194, 6.9844e-06, 2.0,
                  278.68, 562.05), // 278.68-562.05K
        /*surface_tension*/        
            get_refprop_liquid_surface_tension_temperature_function
                (si::kelvin, si::newton/si::meter,
                 562.0, 0.07298, 1.232, -0.0007802, 0.8635, -0.0001756, 0.3065,
                 272.95, 553.15), // Mulero (2012)
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
        }),
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(80.088 * si::gram*si::kelvin), 10.79, 0.00420, 0.0, 0.0, 273.0, 1074.0),
            /*thermal_conductivity*/              state_invariant(5.55 * si::watt / ( si::meter * si::kelvin )), // Wilkens (2011)
            /*dynamic_viscosity*/                 state_invariant(1e17 * si::poise), // various sources, Carey (1953) cites this number from Weinberg (1927), and Mukherjee (2010), provides a literature review and findings from salt diapirs. Science is weird.
            /*density*/                           state_invariant(2170.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    
                get_interpolated_temperature_function
                    (si::celcius, si::millimeter_mercury,
                     std::vector<double>{865.0,  1017.0, 1220.0, 1460.0 }, 
                     std::vector<double>{1.0,    10.0,   100.0,  760.0  }), // Perry
            /*refractive_index*/      
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{  0.22,   0.24,   0.26,   0.28,  23.81,  26.32,  27.78,  29.41,  31.25,  33.33,  38.46,  41.67,  45.45,  50.00,  55.56,  62.50,  71.43,  83.33, 100.00, 125.00, 166.67},
                     std::vector<double>{1.66, 1.71, 1.68, 1.64, 1.30, 1.23, 1.17, 1.09, 1.00, 0.87, 0.48, 0.50, 0.27, 0.19, 0.43, 6.29, 3.66, 3.05, 2.76, 2.58, 2.49}),
                    // Querry (1987)
            /*extinction_coefficient*/ 
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{ 0.22,  26.32,  27.78,  31.25,  33.33,  35.71,  38.46,  41.67,  45.45,  50.00,  55.56,  62.50,  71.43,  83.33, 100.00, 166.67},
                     std::vector<double>{0.000, 0.014, 0.011, 0.049, 0.102, 0.172, 0.617, 0.792, 1.147, 1.970, 3.582, 1.948, 0.226, 0.095, 0.077, 0.056}),
                    // Querry (1987)
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for halite
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(101.96 * si::gram*si::kelvin), 22.08, 0.008971, -522500.0, 0.0, 273.0, 1973.0), 
            /*thermal_conductivity*/              // field::StateSample<si::thermal_conductivity>(37.0*si::watt/(si::meter * si::kelvin), si::atmosphere, 20.0*si::celcius),//azom.com/article.aspx?ArticleId=1948
                get_interpolated_temperature_function
                    (si::kelvin, si::watt / (si::meter * si::kelvin),
                     std::vector<double>{10.0, 60.0,  300.0, 400.0, 1400.0}, 
                     std::vector<double>{7.0,  174.0,  36.0,  26.0,    6.0}), // Perry
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(3970.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{ 0.21,  7.94,  8.33,  8.55,  9.26, 10.64, 10.75, 10.99, 11.11, 11.24, 11.36, 14.49, 14.71, 14.93, 15.15, 15.38, 15.62, 15.87, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 33.33, 34.48, 37.04, 55.56},
                     std::vector<double>{1.831, 1.345, 1.294, 1.243, 1.113, 0.592, 0.514, 0.279, 0.182, 0.124, 0.098, 0.117, 0.135, 0.172, 0.243, 0.369, 0.664, 1.672, 0.735, 0.774, 1.062, 1.523, 2.382, 5.146, 8.559, 6.585, 5.308, 4.497, 3.797, 3.232, 2.725, 1.951, 0.629, 0.283, 0.512, 1.472, 7.250, 9.484, 6.881, 5.718, 5.020, 3.879, 5.677, 4.903, 4.560, 4.279, 4.122, 4.000, 3.893, 3.723, 3.667, 3.617, 3.326}),
                    // Querry (1985)
            /*extinction_coefficient*/            
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{ 0.21,  7.87,  8.62,  9.09,  9.52, 10.75, 10.87, 10.99, 11.11, 11.36, 13.89, 14.08, 14.29, 14.49, 14.71, 14.93, 15.15, 15.38, 15.62, 15.87, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 30.30, 31.25, 32.26, 33.33, 34.48, 41.67, 55.56},
                     std::vector<double>{-0.052, 0.044, 0.068, 0.080, 0.089, 0.139, 0.166, 0.239, 0.355, 0.608, 2.178, 2.311, 2.466, 2.645, 2.838, 3.049, 3.326, 3.683, 4.250, 3.460, 3.626, 4.260, 4.919, 5.835, 6.996, 8.491, 3.839, 1.299, 0.642, 0.420, 0.480, 0.662, 0.591, 0.573, 1.220, 2.824, 4.730, 7.373,11.173, 1.231, 0.312, 0.108, 0.066, 0.606, 1.312, 0.108,-0.069,-0.089, 0.010, 0.046, 0.019, 0.013, 0.077, 0.013, 0.011}),
                    // Querry (1985)
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for corundum
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
            /*refractive_index*/                  spectral_invariant(1.645),
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
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{0.2, 0.22, 0.23, 0.24, 0.26, 0.27, 0.28, 0.29, 3.9526, 4.2017, 4.8544, 4.902, 5, 5.0505, 6.25, 6.2893, 6.3291, 6.4516, 8.0645, 9.009, 10.2041, 10.3093, 11.3636, 11.4943, 11.6279, 12.3457, 14.4928, 14.9254, 15.3846, 15.625, 15.873, 16.129, 20, 20.4082, 20.8333, 21.2766, 22.2222, 23.8095, 25.641, 27.027, 27.7778, 28.5714, 29.4118, 30.303, 31.25, 32.2581, 33.3333, 34.4828, 35.7143, 37.037, 38.4615, 40, 41.6667, 43.4783, 45.4545, 47.619, 50, 52.6316, 55.5556},
                     std::vector<double>{0.84, 0.962, 1.045, 1.162, 1.49, 1.591, 1.646, 1.679, 3.747, 3.844, 4.141, 4.173, 4.192, 4.237, 4.693, 4.713, 4.764, 4.78, 5.113, 5.26, 5.458, 5.458, 5.572, 5.571, 5.616, 5.677, 5.877, 5.935, 5.971, 5.972, 5.996, 6.042, 6.438, 6.46, 6.511, 6.582, 6.686, 6.95, 7.247, 7.469, 7.602, 7.709, 7.83, 8.035, 8.224, 8.425, 8.682, 8.969, 9.225, 9.586, 9.984, 10.214, 10.813, 10.952, 11.266, 11.898, 12.39, 13.618, 14.474}),
                    // Querry (1985)
            /*extinction_coefficient*/ 
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{0.2, 0.24, 0.25, 0.26, 0.28, 0.29, 2.7322, 3.7736, 3.7879, 4.1841, 4.878, 5, 5.0251, 5.7143, 6.25, 6.2893, 6.3291, 6.3694, 6.4103, 8.1301, 8.9286, 10.101, 10.2041, 11.236, 11.3636, 11.4943, 11.6279, 11.7647, 12.5, 12.6582, 14.7059, 15.625, 15.873, 19.2308, 20, 20.4082, 20.8333, 21.7391, 22.7273, 23.2558, 24.3902, 25, 27.7778, 28.5714, 30.303, 31.25, 33.3333, 34.4828, 35.7143, 37.037, 38.4615, 41.6667, 43.4783, 45.4545, 47.619, 50, 52.6316, 55.5556},
                     std::vector<double>{0.723, 1.162, 1.221, 1.195, 1.061, 1.009, 2.395, 2.762, 2.785, 2.91, 3.124, 3.154, 3.179, 3.254, 3.312, 3.347, 3.343, 3.323, 3.316, 3.489, 3.598, 3.774, 3.776, 3.927, 3.934, 3.979, 4.009, 4.015, 4.153, 4.168, 4.541, 4.705, 4.784, 5.434, 5.606, 5.702, 5.823, 6.02, 6.281, 6.393, 6.645, 6.755, 7.307, 7.462, 7.891, 8.052, 8.482, 8.664, 8.868, 9.1, 9.171, 9.559, 9.451, 9.974, 10.13, 10.524, 10.803, 10.332}),
                    // Querry (1985)
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for carbon
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
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{0.035,  0.058,  0.058,  0.059,  0.076,  0.083,  0.084,  0.091,  0.093,  0.094,  0.095,  0.097,  0.098,  0.100,  0.102,  0.103,  0.105,  0.107,  0.109,  0.111,  0.113,  0.117,  0.122,  0.124,  0.129,  0.135,  0.148,  0.155,  0.159,  0.172,  0.177,  0.182,  0.188,  0.200,  0.230, 10.000},
                     std::vector<double>{ 0.58,   0.46,   0.46,   0.45,   0.78,   0.94,   0.97,   1.09,   1.10,   1.14,   1.21,   1.34,   1.68,   2.08,   2.55,   2.85,   3.06,   3.28,   3.44,   3.51,   3.49,   3.50,   3.45,   3.42,   3.32,   3.26,   3.22,   3.25,   3.28,   3.42,   3.28,   3.11,   2.99,   2.86,   2.69, 2.38  }),
                    // Philip (1964)
            /*extinction_coefficient*/ 
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(100.087 * si::gram*si::kelvin), 19.68, 0.01189, -307600.0, 0.0, 273.0, 1033.0), 
            /*thermal_conductivity*/              state_invariant(3.59 * si::watt / (si::meter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2710.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
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
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for calcite
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(100.087 * si::gram*si::kelvin), 19.68, 0.01189, -307600.0, 0.0, 273.0, 1033.0), 
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
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{ 1.5,  8.0,  8.0,  8.6,  8.8,  8.9,  8.9,  8.9,  9.0,  9.0,  9.0,  9.1,  9.1,  9.1,  9.2,  9.2,  9.2,  9.3,  9.3,  9.4,  9.4,  9.5,  9.5,  9.5,  9.6,  9.6,  9.7,  9.7,  9.7,  9.8,  9.8,  9.8,  9.9,  9.9, 14.3},
                     std::vector<double>{1.44, 0.52, 0.51, 0.56, 0.47, 0.47, 0.47, 0.49, 0.53, 0.58, 0.65, 0.74, 0.84, 0.95, 1.07, 1.20, 1.33, 1.47, 1.78, 1.94, 2.22, 2.36, 2.47, 2.58, 2.68, 2.77, 2.83, 2.87, 2.91, 2.93, 2.94, 2.94, 2.92, 2.89, 1.75}),
                    // Kischkat (2012)
            /*extinction_coefficient*/ 
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{ 1.5,  7.9,  7.9,  8.6,  8.7,  8.7,  8.7,  8.7,  8.8,  8.8,  8.8,  8.9,  8.9,  8.9,  9.0,  9.1,  9.1,  9.1,  9.2,  9.2,  9.2,  9.3,  9.3,  9.3,  9.4,  9.4,  9.4,  9.5,  9.5,  9.5,  9.6,  9.6,  9.8,  9.9,  9.9, 10.0, 10.0, 10.0, 10.1, 10.1, 10.2, 10.2, 10.2, 10.3, 10.3, 10.4, 14.3},
                     std::vector<double>{0.00002, 0.12770, 0.15315, 0.91466, 0.94547, 0.98260, 1.02616, 1.07603, 1.13242, 1.19597, 1.26754, 1.34784, 1.43703, 1.53432, 1.84907, 1.94684, 2.03267, 2.10448, 2.16419, 2.21577, 2.26057, 2.29387, 2.30736, 2.29703, 2.26706, 2.22350, 2.16431, 2.08460, 1.99475, 1.89944, 1.78965, 1.66503, 0.92326, 0.80887, 0.70568, 0.61411, 0.53217, 0.45759, 0.38927, 0.32737, 0.27266, 0.22569, 0.18645, 0.15438, 0.12857, 0.10802, 0.06946}),
                    // Kischkat (2012)
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for quartz
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
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for quartz
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(278.33 * si::gram*si::kelvin), 69.26, 0.00821, -2331000.0, 0.0, 273.0, 1373.0),
            /*thermal_conductivity*/              state_invariant(2.31 * si::watt/(si::meter*si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2560.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  spectral_invariant(1.527),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for orthoclase
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(268.6 * si::gram*si::kelvin), 63.13, 0.01500, -1537000.0, 0.0, 273.0, 1673.0), // for anorthite
            /*thermal_conductivity*/              state_invariant(1.69 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015), for anorthite
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(2670.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  spectral_invariant(1.553),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for andesine
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(236.4 * si::gram*si::kelvin), 23.35, 0.008062, -558800.0, 0.0, 273.0, 773.0), // for maganese pyroxenes 
            /*thermal_conductivity*/              state_invariant(4.66 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(3380.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  spectral_invariant(1.707),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for augite
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(153.31 * si::gram*si::kelvin), 33.57, 0.01907, -879700.0, 0.0, 273.0, 1161.0), // for fayalite
            /*thermal_conductivity*/              state_invariant(7.69 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(3810.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  spectral_invariant(1.651),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for olivine
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
            /*refractive_index*/                  spectral_invariant(2.401),
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for goethite
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(119.98 * si::gram*si::kelvin), 10.7, 0.01336, 0.0, 0.0, 273.0, 773.0),
            /*thermal_conductivity*/              state_invariant(19.21 * si::watt / (si::meter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(5020.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  spectral_invariant(1.78), //gemologyproject.com
            /*extinction_coefficient*/            missing(),
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for pyrite
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(159.69 * si::gram*si::kelvin), 24.72, 0.01604, -423400.0, 0.0, 273.0, 1097.0),
            /*thermal_conductivity*/              state_invariant(11.28 * si::watt / (si::meter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(),
            /*density*/                           state_invariant(5250.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{ 0.21,  0.22,  0.23,  0.24,  0.27,  0.28,  0.29,  0.32,  0.33,  0.34,  0.35,  0.36,  0.37,  0.38,  0.43,  0.44,  0.45,  0.46,  0.49,  0.50,  0.52,  0.53,  0.54,  0.56,  0.57,  0.58, 14.29, 14.49, 14.71, 15.15, 15.38, 15.62, 16.13, 16.39, 16.67, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 45.45, 47.62, 50.00, 52.63, 55.56, 58.82, 62.50, 66.67, 71.43, 90.91},
                     std::vector<double>{ 1.20,  1.47,  1.66,  1.79,  2.11,  2.20,  2.26,  2.42,  2.45,  2.45,  2.43,  2.43,  2.47,  2.54,  3.05,  3.13,  3.18,  3.22,  3.28,  3.28,  3.26,  3.27,  3.29,  3.35,  3.36,  3.35,  1.15,  1.04,  0.91,  0.60,  0.48,  0.40,  0.33,  0.31,  0.30,  0.34,  0.40,  0.52,  0.79,  1.26,  2.01,  2.82,  2.67,  1.76,  1.10,  0.90,  1.01,  1.27,  2.23,  4.99,  7.43,  6.23,  4.97,  4.10,  3.26,  2.38,  1.55,  1.03,  0.81,  0.85,  1.00,  1.40,  2.68,  6.49, 12.59, 11.93,  9.43,  8.31,  7.29,  6.65,  7.02,  6.79,  6.36,  6.09,  5.84,  5.66,  5.51,  5.39,  5.29,  5.20,  5.00}),
                    // Querry (1985)
            /*extinction_coefficient*/            
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{ 0.21,  0.22,  0.23,  0.26,  0.27,  0.29,  0.30,  0.31,  0.32,  0.34,  0.35,  0.36,  0.38,  0.39,  0.40,  0.41,  0.42,  0.43,  0.50,  0.51,  0.52,  0.54,  0.55,  0.56,  0.59,  0.60, 14.49, 14.71, 14.93, 15.15, 15.38, 16.13, 16.39, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 21.74, 22.22, 22.73, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 45.45, 47.62, 50.00, 52.63, 90.91},
                     std::vector<double>{ 1.207,  1.291,  1.309,  1.317,  1.300,  1.233,  1.208,  1.195,  1.169,  1.085,  1.093,  1.140,  1.258,  1.291,  1.294,  1.271,  1.231,  1.173,  0.675,  0.622,  0.587,  0.538,  0.498,  0.437,  0.202,  0.149,  0.297,  0.350,  0.440,  0.586,  0.779,  1.406,  1.624,  2.096,  2.358,  2.669,  3.035,  3.436,  3.791,  3.911,  3.368,  2.292,  2.054,  2.553,  3.328,  4.068,  4.993,  6.303,  6.991,  4.077,  1.587,  0.982,  0.718,  0.656,  0.803,  1.390,  2.254,  3.245,  4.284,  5.382,  6.922,  9.062, 11.145,  8.306,  2.150,  0.782,  0.389,  0.132,  0.601,  0.668,  0.030, -0.074, -0.117, -0.148, -0.076}),
                    // Querry (1985)
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for hematite
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
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{0.003542,0.005636,0.008266,0.008856,0.009537,0.01127, 0.01378, 0.01459, 0.01631, 0.01722, 0.01771, 0.01879, 0.02,    0.02138, 0.02214, 0.02296, 0.02339, 0.02384, 0.0248,  0.0253,  0.02818, 0.03263, 0.03542, 0.03875, 0.03999, 0.04133, 0.04592, 0.04679, 0.04862, 0.05061, 0.05391, 0.0551,  0.05636, 0.05767, 0.05904, 0.06199, 0.06525, 0.07293, 0.07749, 0.08266, 0.08551, 0.08856, 0.1127,  0.124,   0.155,   0.1653,  0.2066,  0.261,   0.2755,  0.2952,  0.3024,  0.31,    0.3179,  0.3263, 0.3351, 0.3444, 0.3542, 0.3815, 0.4133, 0.4959, 0.6199, 0.8266, 1.24,  4.133, 6.199, 12.4,  24.8, 49.59, 124.0, 248.0},
                     std::vector<double>{1.001,   0.987,   0.963,   0.955,   0.943,   0.902,   0.849,   0.846,   0.853,   0.851,   0.859,   0.883,   0.881,   0.868,   0.871,   0.885,   0.89,    0.889,   0.884,   0.885,   0.899,   0.892,   0.865,   0.921,   0.932,   0.93,    0.886,   0.889,   0.913,   0.989,   1.175,   1.262,   1.338,   1.368,   1.346,   1.29,    1.266,   1.33,    1.421,   1.66,    1.644,   1.72,    1.545,   1.462,   1.16,    1.142,   1.339,   1.612,   1.694,   1.752,   1.729,   1.615,   1.295,   0.727,  0.3,    0.23,   0.211,  0.229,  0.226,  0.237,  0.269,  0.271,  0.278, 1.411, 2.842, 9.936, 36.7, 118.0, 309.0, 531.0}),
                // Hagemann (1974)
            /*extinction_coefficient*/            
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
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
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{0.003542,0.005636,0.008266,0.008856,0.009537,0.01127, 0.01378, 0.01459, 0.01631, 0.01722, 0.01771, 0.01879, 0.02,    0.02138, 0.02214, 0.02296, 0.02339, 0.02384, 0.0248,  0.0253,  0.02818, 0.03263, 0.03542, 0.03875, 0.03999, 0.04133, 0.04592, 0.04679, 0.04862, 0.05061, 0.05391, 0.0551,  0.05636, 0.05767, 0.05904, 0.06199, 0.06525, 0.07293, 0.07749, 0.08266, 0.08551, 0.08856, 0.1127,  0.124,   0.155,   0.1653,  0.2066,  0.261,   0.2755,  0.2952,  0.3024,  0.31,    0.3179,  0.3263, 0.3351, 0.3444, 0.3542, 0.3815, 0.4133, 0.4959, 0.6199, 0.8266, 1.24,  4.133, 6.199, 12.4,  24.8, 49.59, 124.0, 248.0},
                     std::vector<double>{1.001,   0.987,   0.963,   0.955,   0.943,   0.902,   0.849,   0.846,   0.853,   0.851,   0.859,   0.883,   0.881,   0.868,   0.871,   0.885,   0.89,    0.889,   0.884,   0.885,   0.899,   0.892,   0.865,   0.921,   0.932,   0.93,    0.886,   0.889,   0.913,   0.989,   1.175,   1.262,   1.338,   1.368,   1.346,   1.29,    1.266,   1.33,    1.421,   1.66,    1.644,   1.72,    1.545,   1.462,   1.16,    1.142,   1.339,   1.612,   1.694,   1.752,   1.729,   1.615,   1.295,   0.727,  0.3,    0.23,   0.211,  0.229,  0.226,  0.237,  0.269,  0.271,  0.278, 1.411, 2.842, 9.936, 36.7, 118.0, 309.0, 531.0}),
                // Hagemann (1974)
            /*extinction_coefficient*/            
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
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
            get_perry_temperature_function(si::kelvin, si::calorie/(63.546 * si::gram*si::kelvin), 5.44, 0.001462, 0.0, 0.0, 1357.0, 1573.0),
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
                     5.44, 0.001462, 0.0, 0.0, 273.0, 1357.0), // Johnson (1960) and Perry
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
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{2.5e-05, 2.5e-03, 4.1e-03, 8.3e-03, 1.0e-02, 3.9e-02, 4.0e-02, 4.1e-02, 4.3e-02, 4.4e-02, 4.6e-02, 5.0e-02, 5.2e-02, 5.4e-02, 5.6e-02, 5.9e-02, 6.2e-02, 6.5e-02, 6.9e-02, 7.3e-02, 7.7e-02, 8.6e-02, 8.9e-02, 9.5e-02, 1.0e-01, 1.1e-01, 1.2e-01, 1.3e-01, 1.4e-01, 1.5e-01, 1.7e-01, 1.8e-01, 1.9e-01, 2.1e-01, 2.1e-01, 2.2e-01, 2.3e-01, 2.4e-01, 2.5e-01, 2.6e-01, 2.7e-01, 2.8e-01, 3.0e-01, 3.1e-01, 3.3e-01, 3.4e-01, 3.9e-01, 4.1e-01, 4.8e-01, 5.2e-01, 5.4e-01, 5.6e-01, 5.9e-01, 6.2e-01, 6.5e-01, 7.1e-01, 7.3e-01, 8.3e-01, 1.2e+00, 2.5e+00, 1.2e+01, 2.5e+01, 1.2e+02, 2.5e+02},
                     std::vector<double>{  1.00,   1.00,   0.99,   0.97,   0.96,   0.89,   0.88,   0.86,   0.85,   0.86,   0.88,   0.96,   0.96,   0.94,   0.92,   0.90,   0.88,   0.88,   0.89,   0.91,   0.95,   1.03,   1.05,   1.08,   1.09,   1.07,   1.04,   1.03,   1.03,   1.03,   1.00,   0.97,   0.96,   1.04,   1.10,   1.18,   1.28,   1.38,   1.47,   1.53,   1.52,   1.49,   1.42,   1.34,   1.34,   1.31,   1.23,   1.18,   1.15,   1.12,   1.04,   0.83,   0.47,   0.27,   0.21,   0.21,   0.22,   0.26,   0.43,   1.69,  29.10,  62.80, 186.00, 270.00}),
                // Hagemann (1974)
            /*extinction_coefficient*/            
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(231.53 * si::gram*si::kelvin), 41.17, 0.01882, -979500.0, 0.0, 273.0, 1065.0),
            /*thermal_conductivity*/              state_invariant(5.1 * si::watt / (si::centimeter * si::kelvin)), // Schön (2015)
            /*dynamic_viscosity*/                 missing(), // 3e8 * si::pascal * si::second, // Melosh (2011), from Hiesinger (2007), for venusian lava flows, middle of range on log scale
            /*density*/                           state_invariant(5170.0 * si::kilogram/si::meter3),
            /*vapor_pressure*/                    missing(),
            /*refractive_index*/                  
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{ 0.21,  0.22,  4.18,  4.24,  4.20, 15.62, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 20.83, 21.28, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 47.62, 50.00, 52.63, 55.56},
                     std::vector<double>{2.3, 2.3, 3.3, 3.3, 3.3, 3.5, 3.5, 3.5, 3.5, 3.5, 3.8, 4.4, 5.1, 5.3, 5.1, 4.9, 4.7, 4.6, 4.5, 4.5, 4.4, 4.4, 4.4, 4.4, 4.5, 4.7, 5.2, 6.3, 7.4, 7.3, 6.9, 6.6, 6.4, 6.3, 6.3, 6.4, 6.4, 6.5, 6.7, 6.7, 7.1, 7.3, 7.7, 7.9}),
                    // Querry (1985)
            /*extinction_coefficient*/            
                get_linear_interpolated_refractive_index_function
                    (si::micrometer, 
                     std::vector<double>{ 0.21,  0.22,  2.83,  2.96,  9.90, 15.38, 15.62, 16.13, 16.39, 16.67, 16.95, 17.24, 17.54, 17.86, 18.18, 18.52, 18.87, 19.23, 19.61, 20.00, 20.41, 23.26, 23.81, 24.39, 25.00, 25.64, 26.32, 27.03, 27.78, 28.57, 29.41, 30.30, 31.25, 32.26, 33.33, 34.48, 35.71, 37.04, 38.46, 40.00, 41.67, 43.48, 45.45, 47.62, 50.00, 52.63, 55.56},
                     std::vector<double>{0.085, 0.109, 1.194, 1.180, 1.464, 2.348, 2.427, 2.626, 2.761, 2.963, 3.249, 3.642, 3.907, 3.604, 2.966, 2.540, 2.383, 2.348, 2.365, 2.402, 2.460, 2.959, 3.078, 3.224, 3.413, 3.655, 3.958, 4.392, 4.940, 5.210, 4.290, 3.268, 2.855, 2.776, 2.807, 2.858, 2.950, 3.017, 3.058, 3.185, 3.226, 3.253, 3.357, 3.331, 3.400, 3.351, 3.209}),
                    // Querry (1985)
            /*absorption_coefficient*/            
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for magnetite
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
                get_perry_temperature_function_for_specific_heat_capacity_as_solid
                    (si::kelvin, si::calorie/(159.16 * si::gram*si::kelvin), 9.38, 0.0312, 0.0, 0.0, 273.0, 376.0),
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
                get_absorption_coefficient_function_from_reflectance_at_wavelengths // for chalcopyrite
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