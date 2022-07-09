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
    /*molecular_absorption_cross_section*/missing(),
    // compound::relation::get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output
    //     ( 1.0/si::meter, si::meter2,
    //       std::vector<double>{  1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 1.28e5, 7.99e6, 8.62e6, 9.25e6, 1.00e7, 1.05e7, 1.08e7, 1.13e7, 1.20e7, 1.28e7, 1.33e7, 1.79e7, 2.14e7, 3.37e7, 5.79e7, 1.71e8 },
    //       std::vector<double>{  -20.19, -20.19, -19.97, -19.91, -19.92, -20.05, -20.10, -20.12, -20.17, -23.26, -23.10, -22.58, -21.35, -21.52, -20.18, -20.24, -21.06, -20.16, -20.43, -20.13, -20.31, -20.33, -20.68, -21.63 }),

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


} // end namespace published
} // end namespace compound