#pragma once

#define COMPLETED_COMPOUND_VALID(compound) \
    CHECK(compound.molar_mass / (si::gram/si::mole) > 1.0); \
    CHECK(compound.molar_mass / (si::gram/si::mole) < 1000.0); \
    CHECK(compound.atoms_per_molecule < 100u); \
    CHECK(compound.atoms_per_molecule > 0u); \
    CHECK(compound.molecular_diameter / (si::picometer) > 100.0); \
    CHECK(compound.molecular_diameter / (si::picometer) < 1000.0); \
    CHECK(compound.molecular_degrees_of_freedom <= 8.0); \
    CHECK(compound.molecular_degrees_of_freedom >= 3.0); \
    CHECK(compound.acentric_factor > -1.0); \
    CHECK(compound.acentric_factor <  1.0); \
    CHECK(compound.critical_point_pressure / (si::megapascal) > 0.1); \
    CHECK(compound.critical_point_pressure / (si::megapascal) < 1000.0); /*based on gold*/ \
    CHECK(compound.critical_point_temperature / (si::kelvin) > 0.0); \
    CHECK(compound.critical_point_temperature / (si::kelvin) < 10000.0); /*based on aluminum*/ \
    CHECK(compound.latent_heat_of_vaporization / (si::joule / si::kilogram) < 1e8); /*based on aluminum*/\
    CHECK(compound.latent_heat_of_vaporization / (si::joule / si::kilogram) > 1e4); /*based on helium*/\
    CHECK(compound.latent_heat_of_fusion / (si::joule / si::kilogram) < 1e7); /*based on graphite*/\
    CHECK(compound.latent_heat_of_fusion / (si::joule / si::kilogram) > 3e3); /*based on perflouromethane*/\
    CHECK(compound.triple_point_pressure / (si::pascal) > 0.0001); \
    CHECK(compound.triple_point_pressure / (si::atmosphere) < 10.0); /*based on carbon dioxide*/ \
    CHECK(compound.triple_point_temperature / (si::kelvin) > 0.0); \
    CHECK(compound.triple_point_temperature / (si::kelvin) < 10000.0); \
    CHECK(compound.freezing_point_sample_pressure / (si::pascal) > 0.0001); \
    CHECK(compound.freezing_point_sample_pressure / (si::megapascal) < 10.0); \
    CHECK(compound.freezing_point_sample_temperature / (si::kelvin) > 0.0); \
    CHECK(compound.freezing_point_sample_temperature / (si::kelvin) < 10000.0); \
    CHECK(compound.boiling_point_sample_pressure / (si::pascal) > 0.0001); \
    CHECK(compound.boiling_point_sample_pressure / (si::megapascal) < 1000.0); /*same as for critical point*/ \
    CHECK(compound.boiling_point_sample_temperature / (si::kelvin) > 0.0); \
    CHECK(compound.boiling_point_sample_temperature / (si::kelvin) < 10000.0); \
    CHECK(compound.molecular_absorption_cross_section(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), si::standard_pressure, si::standard_temperature) / (si::meter2) > 1e-36); \
    CHECK(compound.molecular_absorption_cross_section(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), si::standard_pressure, si::standard_temperature) / (si::meter2) < 1e-15); 
    // compound.critical_point_volume
    // compound.critical_point_compressibility
    // compound.simon_glatzel_slope
    // compound.simon_glatzel_exponent
    /*
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
    */