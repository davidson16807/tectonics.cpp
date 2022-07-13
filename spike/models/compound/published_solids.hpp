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
            (const si::pressure<double> p, 
             const si::temperature<double> T)
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
                const si::pressure<double>        p0 = 0.0003 * si::pascal;
                const si::temperature<double>     T0 = 1983.0 * si::kelvin;
                // triple point between alpha, beta, and coesite
                const si::pressure<double>        pabc = 3.1 * si::gigapascal;
                const si::temperature<double>     Tabc = 1190.0 * si::celcius;
                // sample point between solid and liquid
                const si::pressure<double>        pf = si::atmosphere;
                const si::temperature<double>     Tf = 1425.0 * si::celcius;
                // sample point between stishovite and coesite
                const si::pressure<double>        psc0 = 7.75 * si::gigapascal;
                const si::temperature<double>     Tsc0 = 0.0 * si::celcius;
                // sample point between stishovite and coesite
                const si::pressure<double>        psc1 = 11.0 * si::gigapascal;
                const si::temperature<double>     Tsc1 = 1400.0 * si::celcius;
                // sample point between alpha and coesite
                const si::pressure<double>        pac = 1.8 * si::gigapascal;
                const si::temperature<double>     Tac = 0.0 * si::celcius;
                // sample point between beta and coesite
                const si::pressure<double>        pbc = 4.3 * si::gigapascal;
                const si::temperature<double>     Tbc = 2200.0 * si::celcius;
                // sample point between alpha and beta
                const si::pressure<double>        pab = 0.0 * si::gigapascal;
                const si::temperature<double>     Tab = 573.0 * si::celcius;
                // sample point between beta and cristobalite alpha
                const si::pressure<double>        pbca = 0.0 * si::gigapascal;
                const si::temperature<double>     Tbca = 870.0 * si::celcius;
                // sample point between beta and cristobalite beta
                const si::pressure<double>        pbcb = 1.0 * si::gigapascal;
                const si::temperature<double>     Tbcb = 1705.0 * si::celcius;
                // sample point between cristobalite alpha and cristobalite beta
                const si::temperature<double>     Tcacb = 1470.0 * si::celcius;
                // critical point
                const si::pressure<double>        pc = 1.7e8 * si::pascal;
                const si::temperature<double>     Tc = 5300.0* si::kelvin;
                // latent heat of vaporization and molar mass
                const si::specific_energy<double> L  = 11770e3 * si::joule / si::kilogram;
                const si::molar_mass<double>      M  = 60.08 * si::gram/si::mole;

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

} // end namespace published
} // end namespace compound