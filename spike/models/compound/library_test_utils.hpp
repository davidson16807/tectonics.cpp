#pragma once

#define COMPLETED_COMPOUND_STP_VALID(compound) \
    CHECK(compound.gas.specific_heat_capacity(si::standard_pressure, si::standard_temperature) / (si::joule / (si::kilogram * si::kelvin)) > 3e2 ); /*based on argon*/ \
    CHECK(compound.gas.specific_heat_capacity(si::standard_pressure, si::standard_temperature) / (si::joule / (si::kilogram * si::kelvin)) < 1e5 ); /*based on hydrogen*/ \
    CHECK(compound.gas.density(si::standard_pressure, si::standard_temperature) / (si::kilogram/si::meter3) < 30.0); /*based on theoretical estimate for rocks*/ \
    CHECK(compound.gas.density(si::standard_pressure, si::standard_temperature) / (si::kilogram/si::meter3) > 0.08); /*based on hydrogen*/ \
    CHECK(compound.liquid.density(si::standard_pressure, si::standard_temperature) / (si::gram / si::centimeter3) < 20.0); /*based on gold*/ \
    CHECK(compound.liquid.density(si::standard_pressure, si::standard_temperature) / (si::gram / si::centimeter3) > 0.01); /*based on mercury*/

#define COMPLETED_COMPOUNDS_STP_VALID() \
    COMPLETED_COMPOUND_STP_VALID(compound::library::water           ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::hydrogen        ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::helium          ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::argon           ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::nitrogen        ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::oxygen          ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::ammonia         ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::ozone           ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::sulfur_dioxide  ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::nitrous_oxide   ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::nitric_oxide    ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::carbon_dioxide  ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::carbon_monoxide ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::hydrogen_cyanide); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::methane         ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::perflouromethane); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::formaldehyde    ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::formic_acid     ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::benzene         ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::pyrimidine      ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::ethane          ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::ethanol         ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::quartz          ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::corundum        ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::carbon          ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::orthoclase      ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::andesine        ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::augite          ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::forsterite      ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::hematite        ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::goethite        ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::magnetite       ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::pyrite          ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::chalcocite      ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::chalcopyrite    ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::copper          ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::silver          ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::gold            ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::halite          ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::calcite         ); \
    COMPLETED_COMPOUND_STP_VALID(compound::library::apatite         ); 


#define COMPLETED_COMPOUND_VALID(compound, temperature, pressure) \
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
    CHECK(compound.molecular_absorption_cross_section(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), pressure, temperature) / (si::meter2) > 1e-36); \
    CHECK(compound.molecular_absorption_cross_section(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), pressure, temperature) / (si::meter2) < 1e-15); \
    if(compound.phase(pressure, temperature) == -2)\
    {\
        CHECK(compound.gas.thermal_conductivity(pressure, temperature) / (si::watt / (si::meter * si::kelvin)) > 0.003); /*based on sulfur dioxide*/ \
        CHECK(compound.gas.thermal_conductivity(pressure, temperature) / (si::watt / (si::meter * si::kelvin)) < 0.3); /*based on hydrogen*/ \
        CHECK(compound.gas.dynamic_viscosity(pressure, temperature) / (si::pascal * si::second) < 3e-5); /*based on argon*/ \
        CHECK(compound.gas.dynamic_viscosity(pressure, temperature) / (si::pascal * si::second) > 1e-6); /*based on steam*/ \
        CHECK(compound.gas.refractive_index(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), pressure, temperature) > 1.00003 ); /*based on helium*/ \
        CHECK(compound.gas.refractive_index(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), pressure, temperature) < 1.001 ); /*based on air*/ \
    }\
    if(compound.phase(pressure, temperature) == -1)\
    {\
        CHECK(compound.liquid.specific_heat_capacity(pressure, temperature) / (si::joule/(si::gram*si::kelvin)) < 100.0); /*based on hydrogen*/ \
        CHECK(compound.liquid.specific_heat_capacity(pressure, temperature) / (si::joule/(si::gram*si::kelvin)) > 0.1); /*based on gold*/ \
        CHECK(compound.liquid.thermal_conductivity(pressure, temperature) / (si::watt / (si::meter * si::kelvin)) < 200.0); /*based on silver*/ \
        CHECK(compound.liquid.thermal_conductivity(pressure, temperature) / (si::watt / (si::meter * si::kelvin)) > 0.01); /*based on helium*/ \
        CHECK(compound.liquid.dynamic_viscosity(pressure, temperature) / (si::pascal * si::second) < 1e9); /*based on pitch*/ \
        CHECK(compound.liquid.dynamic_viscosity(pressure, temperature) / (si::pascal * si::second) >= 1e-6); /*based on helium*/ \
        CHECK(compound.liquid.vapor_pressure(pressure, temperature) / si::pascal < 1e7); /*based on high temperature acetaldehyde*/ \
        CHECK(compound.liquid.vapor_pressure(pressure, temperature) / si::pascal > 0.1); /*based on low temperature ethylene glycol*/ \
        CHECK(compound.liquid.surface_tension(pressure, temperature) / (si::millinewton/si::meter) < 3e3); /*based on molten copper */ \
        CHECK(compound.liquid.surface_tension(pressure, temperature) / (si::millinewton/si::meter) > 0.001); /*based on liquid helium */ \
        CHECK(compound.liquid.refractive_index(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), pressure, temperature) > 1.02 ); /*based on liquid helium*/ \
        CHECK(compound.liquid.refractive_index(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), pressure, temperature) < 1.7 ); /*based on carbon disulfide*/ \
    }\
    CHECK(compound.solids.size() != 0); \
    if(compound.phase(pressure, temperature) >= 0)\
    {\
        for (std::size_t i = 0; i < compound.solids.size(); ++i) \
        {                                                        \
            CHECK(compound.solids[i].specific_heat_capacity(pressure, temperature) / (si::joule/(si::kilogram * si::kelvin)) < 30000.0); /*based on hydrogen*/ \
            CHECK(compound.solids[i].specific_heat_capacity(pressure, temperature) / (si::joule/(si::kilogram * si::kelvin)) > 116.0); /*based on uranium*/ \
            CHECK(compound.solids[i].thermal_conductivity(pressure, temperature) / (si::watt/(si::meter * si::kelvin)) < 3000.0); /*based on boron arsenide*/ \
            CHECK(compound.solids[i].thermal_conductivity(pressure, temperature) / (si::watt/(si::meter * si::kelvin)) > 0.01); /*based on aerogel*/ \
            CHECK(compound.solids[i].dynamic_viscosity(pressure, temperature) / (si::pascal * si::second) <= 1e24); /*based on granite*/ \
            CHECK(compound.solids[i].dynamic_viscosity(pressure, temperature) / (si::pascal * si::second) > 1e10); /*based on ice*/ \
            CHECK(compound.solids[i].density(pressure, temperature) / (si::kilogram / si::meter3) < 30000.0); /*based on iridium*/ \
            CHECK(compound.solids[i].density(pressure, temperature) / (si::kilogram / si::meter3) > 1.5); /*based on aerogel*/ \
            CHECK(compound.solids[i].vapor_pressure(pressure, temperature) / si::kilopascal < 300.0); /*based on tetraflourosilane*/ \
            CHECK(compound.solids[i].vapor_pressure(pressure, temperature) / si::pascal > 0.001); /*based on phenazine*/ \
            CHECK(compound.solids[i].refractive_index(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), pressure, temperature) < 4.1 ); /*based on germanium */\
            CHECK(compound.solids[i].refractive_index(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), pressure, temperature) > 0.2); /*based on silver*/\
            CHECK(compound.solids[i].absorption_coefficient(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), pressure, temperature) * si::centimeter < 1000000.0); /*based on water*/ \
            CHECK(compound.solids[i].absorption_coefficient(1.0/(600.0*si::nanometer), 1.0/(400.0*si::nanometer), pressure, temperature) * si::centimeter >= 0.0); /*based on nitrogen*/ \
            CHECK(compound.solids[i].bulk_modulus(pressure, temperature) / si::gigapascal < 1000.0); /*based on diamond*/ \
            CHECK(compound.solids[i].bulk_modulus(pressure, temperature) / si::gigapascal > 0.003); /*based on helium*/ \
            CHECK(compound.solids[i].tensile_modulus(pressure, temperature) / si::gigapascal > 0.01); /*based on helium*/ \
            CHECK(compound.solids[i].tensile_modulus(pressure, temperature) / si::gigapascal < 3000.0); /*based on graphite*/ \
            CHECK(compound.solids[i].shear_modulus(pressure, temperature) / si::gigapascal > 0.01); /*based on helium*/ \
            CHECK(compound.solids[i].shear_modulus(pressure, temperature) / si::gigapascal < 1000.0); /*based on tungsten*/ \
            CHECK(compound.solids[i].lame_parameter(pressure, temperature) / si::gigapascal > 0.001); /*based on helium*/ \
            CHECK(compound.solids[i].lame_parameter(pressure, temperature) / si::gigapascal < 1000.0); /*based on various rocks*/ \
            CHECK(compound.solids[i].poisson_ratio(pressure, temperature) < 0.5); /*based on rubber*/ \
            CHECK(compound.solids[i].poisson_ratio(pressure, temperature) > 0.0); /*based on cork*/ \
            CHECK(compound.solids[i].tensile_yield_strength(pressure, temperature) / si::megapascal < 10000.0); /*based on carbon nanotube*/ \
            CHECK(compound.solids[i].tensile_yield_strength(pressure, temperature) / si::megapascal > 0.01); /*based on hydrogen*/ \
            CHECK(compound.solids[i].compressive_yield_strength(pressure, temperature) / si::megapascal < 3000.0); /*based on ceramics*/ \
            CHECK(compound.solids[i].compressive_yield_strength(pressure, temperature) / si::megapascal > 1.0); /*based on hydrogen*/ \
            CHECK(compound.solids[i].shear_yield_strength(pressure, temperature) / si::megapascal < 10000.0); /*based on high performance steels*/ \
            CHECK(compound.solids[i].shear_yield_strength(pressure, temperature) / si::megapascal > 0.003); /*based on hydrogen*/ \
            CHECK(compound.solids[i].tensile_fracture_strength(pressure, temperature) / si::megapascal < 3000.0); /*based on titanium*/ \
            CHECK(compound.solids[i].tensile_fracture_strength(pressure, temperature) / si::megapascal > 0.03); /*based on hydrogen*/ \
            CHECK(compound.solids[i].compressive_fracture_strength(pressure, temperature) / si::megapascal < 10000.0); /*based on ceramics*/ \
            CHECK(compound.solids[i].compressive_fracture_strength(pressure, temperature) / si::megapascal > 0.1); /*based on nitrogen*/ \
            CHECK(compound.solids[i].shear_fracture_strength(pressure, temperature) / si::megapascal < 10000.0); /*based on high performance steels*/ \
            CHECK(compound.solids[i].shear_fracture_strength(pressure, temperature) / si::megapascal > 0.1); /*based on hydrogen*/ \
        }\
    }

    // compound.critical_point_volume
    // compound.critical_point_compressibility
    // compound.simon_glatzel_slope
    // compound.simon_glatzel_exponent
    /*
    */

#define COMPLETED_COMPOUNDS_VALID(temperature, pressure) \
    COMPLETED_COMPOUND_VALID(compound::library::water,            temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::hydrogen,         temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::helium,           temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::argon,            temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::nitrogen,         temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::oxygen,           temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::ammonia,          temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::ozone,            temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::sulfur_dioxide,   temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::nitrous_oxide,    temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::nitric_oxide,     temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::carbon_dioxide,   temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::carbon_monoxide,  temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::hydrogen_cyanide, temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::methane,          temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::perflouromethane, temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::formaldehyde,     temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::formic_acid,      temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::benzene,          temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::pyrimidine,       temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::ethane,           temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::ethanol,          temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::quartz,           temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::corundum,         temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::carbon,           temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::orthoclase,       temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::andesine,         temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::augite,           temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::forsterite,       temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::hematite,         temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::goethite,         temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::magnetite,        temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::pyrite,           temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::chalcocite,       temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::chalcopyrite,     temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::copper,           temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::silver,           temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::gold,             temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::halite,           temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::calcite,          temperature, pressure); \
    COMPLETED_COMPOUND_VALID(compound::library::apatite,          temperature, pressure); 

