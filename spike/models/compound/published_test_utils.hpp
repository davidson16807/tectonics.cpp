#pragma once

#define PARTLY_KNOWN_COMPOUND_VALID(compound) \
    CHECK(compound.molar_mass > 1.0 * si::gram/si::mole); \
    CHECK(compound.molar_mass < 1000.0 * si::gram/si::mole); \
    CHECK(compound.atoms_per_molecule < 100u); \
    CHECK(compound.atoms_per_molecule > 0u); \
    CHECK(compound.molecular_diameter > 100.0 * si::picometer); \
    CHECK(compound.molecular_diameter < 1000.0 * si::picometer); \
    CHECK(compound.molecular_degrees_of_freedom <= 7.0); \
    CHECK(compound.molecular_degrees_of_freedom >= 3.0); \
    CHECK(compound.acentric_factor > -1.0); \
    CHECK(compound.acentric_factor <  1.0); \
    CHECK(compound.critical_point_pressure > 0.1 * si::megapascal); \
    CHECK(compound.critical_point_pressure < 100.0 * si::megapascal); \
    \ // compound.critical_point_volume
    CHECK(compound.critical_point_temperature > 0.0 * si::kelvin); \
    CHECK(compound.critical_point_temperature < 1000.0 * si::kelvin); \
    \ // compound.critical_point_compressibility
    CHECK(compound.latent_heat_of_vaporization < 1e8 * si::joule / si::kilogram); \
    CHECK(compound.latent_heat_of_vaporization > 1e4 * si::joule / si::kilogram); \
    CHECK(compound.latent_heat_of_fusion); \
    compound.triple_point_pressure
    compound.triple_point_temperature
    compound.freezing_point_sample_pressure
    compound.freezing_point_sample_temperature
    compound.boiling_point_sample_pressure
    compound.boiling_point_sample_temperature
    compound.simon_glatzel_slope
    compound.simon_glatzel_exponent
    compound.molecular_absorption_cross_section
    compound.gas.specific_heat_capacity
    compound.gas.thermal_conductivity
    compound.gas.dynamic_viscosity
    compound.gas.density
    compound.gas.refractive_index
    compound.liquid.specific_heat_capacity
    compound.liquid.thermal_conductivity
    compound.liquid.dynamic_viscosity
    compound.liquid.density
    compound.liquid.vapor_pressure
    compound.liquid.surface_tension
    compound.liquid.refractive_index
    compound.solids[i].specific_heat_capacity
    compound.solids[i].thermal_conductivity
    compound.solids[i].dynamic_viscosity
    compound.solids[i].density
    compound.solids[i].vapor_pressure
    compound.solids[i].refractive_index
    compound.solids[i].spectral_reflectance
    compound.solids[i].bulk_modulus
    compound.solids[i].tensile_modulus
    compound.solids[i].shear_modulus
    compound.solids[i].pwave_modulus
    compound.solids[i].lame_parameter
    compound.solids[i].poisson_ratio
    compound.solids[i].compressive_fracture_strength
    compound.solids[i].tensile_fracture_strength
    compound.solids[i].shear_fracture_strength
    compound.solids[i].compressive_yield_strength
    compound.solids[i].tensile_yield_strength
    compound.solids[i].shear_yield_strength
    compound.solids[i].chemical_susceptibility_estimate
    CHECK(crust1.size() != 0);\
    for (std::size_t strata_i = 0; strata_i < crust1.size(); ++strata_i) \
    {                                                         \
        strata::Strata<L,M> strata1; crust1[strata_i].unpack(strata1);\
        STRATA_VALID(strata1) \
    }