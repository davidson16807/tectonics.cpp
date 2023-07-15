#pragma once

// in-house libraries
#include <models/compound/published/published.hpp>
#include <models/compound/handbook/BasicHandbook.hpp>
#include <models/compound/handbook/CriticalPropertiesHandbook.hpp>
#include <models/compound/handbook/SamplePointHandbook.hpp>
#include <models/compound/handbook/MolecularPropertiesHandbook.hpp>
#include <models/compound/handbook/ThermodynamicsHandbook.hpp>
#include <models/compound/handbook/SpectralHandbook.hpp>
#include <models/compound/handbook/StrengthsHandbook.hpp>
#include <models/compound/handbook/ElasticitiesHandbook.hpp>

namespace compound{
namespace estimated{

    // Refer to property/published.png to see the category that we are traversing here.
    // Critical point properties and molecular diameter are guaranteed to be inferrable, so start there.
    // After critical point properties, we want to get to acentric factor ("omega") as soon as possible, 
    // since that lets us calculate so many other things.
    // We want to make sure we have at least some route to estimate acentric factor,
    // So we first try to estimate all the things that correlate with acentric factor but are derived by other means.
    // This includes latent_heat_of_vaporization, vapor_pressure_as_liquid, 
    // dynamic_viscosity_as_liquid, and molar_volume_as_liquid.
    // Some of these properties can be found using triple point properties, 
    // Some of these properties can be found using the pressure/temperature at which measurements were collected,
    // and one property ("molar_volume_as_liquid") can be found using liquid or solid density.
    // 
    // So to summarize, the order we use to derive is: 
    //   properties that are already table::completed by published values
    //   critical point properties
    //   pressure/temperature samples
    //   triple_point_temperature and densities
    //   latent_heat_of_vaporization, vapor_pressure_as_liquid, dynamic_viscosity_as_liquid, molar_volume_as_liquid
    //   acentric factor
    //   properties derived by acentric factor
    //   miscellaneous properties

    BasicHandbook basic(
        published::name,
        published::formula,
        published::atoms_per_molecule,
        published::molar_mass
    );

    CriticalPropertiesHandbook critical(
        published::critical_point_pressure,
        published::critical_point_volume,
        published::critical_point_temperature,
        published::critical_point_compressibility,
        published::atoms_per_molecule,
        published::molar_mass
    );

    SamplePointHandbook sample(
        published::freezing_sample_point,
        published::boiling_sample_point,
        critical,
        basic
    );

    MolecularPropertiesHandbook molecular(
        published::molecular_diameter,
        published::molecular_degrees_of_freedom,
        published::acentric_factor,
        published::dynamic_viscosity_as_liquid,
        published::latent_heat_of_vaporization,
        published::vapor_pressure_as_liquid,
        critical,
        sample,
        basic
    );

    ThermodynamicsHandbook thermodynamics(
        published::triple_point,
        published::latent_heat_of_sublimation,
        published::latent_heat_of_fusion,
        published::latent_heat_of_vaporization,
        published::density_as_solid,
        published::density_as_liquid,
        published::isobaric_specific_heat_capacity_as_solid,
        published::isobaric_specific_heat_capacity_as_liquid,
        published::isobaric_specific_heat_capacity_as_gas,
        published::dynamic_viscosity_as_solid,
        published::dynamic_viscosity_as_liquid,
        published::dynamic_viscosity_as_gas,
        published::vapor_pressure_as_solid,
        published::vapor_pressure_as_liquid,
        published::thermal_conductivity_as_solid,
        published::thermal_conductivity_as_liquid,
        published::thermal_conductivity_as_gas,
        published::surface_tension_as_liquid,
        molecular,
        critical,
        sample,
        basic,
        canonical_polymorph,
        polymorph_compound,
        compound_similarity,
        polymorph_similarity,
        compounds::water
    );

    SpectralHandbook spectra_as_solid(
        published::refractive_index_as_solid,
        published::extinction_coefficient_as_solid,
        published::absorption_coefficient_as_solid,
        polymorph_similarity,
        compounds::water
    );

    StrengthsHandbook strengths(
        published::shear_yield_strength_as_solid,
        published::tensile_yield_strength_as_solid,
        published::compressive_yield_strength_as_solid,
        published::shear_fracture_strength_as_solid,
        published::tensile_fracture_strength_as_solid,
        published::compressive_fracture_strength_as_solid,
        polymorph_similarity,
        compounds::water
    );

    ElasticitiesHandbook elasticities(
        published::bulk_modulus_as_solid,
        published::tensile_modulus_as_solid,
        published::lame_parameter_as_solid,
        published::shear_modulus_as_solid,
        published::poisson_ratio_as_solid,
        published::pwave_modulus_as_solid,
        polymorph_similarity,
        compounds::water
    );

}}

