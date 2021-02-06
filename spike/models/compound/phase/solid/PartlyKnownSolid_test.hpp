
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "PartlyKnownSolid.hpp"

compound::phase::PartlyKnownSolid unknown {
    /*specific_heat_capacity*/            std::monostate(),
    /*thermal_conductivity*/              std::monostate(),
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           std::monostate(),
    /*vapor_pressure*/                    std::monostate(),
    /*refractive_index*/                  std::monostate(),
    /*spectral_reflectance*/              std::monostate(),

    /*bulk_modulus*/                      std::monostate(),
    /*tensile_modulus*/                   std::monostate(),
    /*shear_modulus*/                     std::monostate(),
    /*pwave_modulus*/                     std::monostate(),
    /*lame_parameter*/                    std::monostate(),
    /*poisson_ratio*/                     std::monostate(),

    /*compressive_fracture_strength*/     std::monostate(),
    /*tensile_fracture_strength*/         std::monostate(),
    /*shear_fracture_strength*/           std::monostate(),
    /*compressive_yield_strength*/        std::monostate(),
    /*tensile_yield_strength*/            std::monostate(),
    /*shear_yield_strength*/              std::monostate(),

    /*chemical_susceptibility_estimate*/  std::monostate()
};
compound::phase::PartlyKnownSolid ice {
    /*specific_heat_capacity*/            2.05 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              2.09 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 1e13 * si::poise, // reference by Carey (1953)
    /*density*/                           0916.9 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    138.268 * si::megapascal,
    /*refractive_index*/                  1.3098,
    /*spectral_reflectance*/              0.9,

    /*bulk_modulus*/                      8.899 * si::gigapascal, // gammon (1983)
    /*tensile_modulus*/                   9.332 * si::gigapascal, // gammon (1983)
    /*shear_modulus*/                     3.521 * si::gigapascal, // gammon (1983)
    /*pwave_modulus*/                     13.59 * si::gigapascal, // gammon (1983)
    /*lame_parameter*/                    6.552 * si::gigapascal, // gammon (1983)
    /*poisson_ratio*/                     0.3252, // gammon (1983)

    /*compressive_fracture_strength*/     6.0 * si::megapascal, // engineering toolbox
    /*tensile_fracture_strength*/         1.0 * si::megapascal, // engineering toolbox
    /*shear_fracture_strength*/           1.1 * si::megapascal, // Frederking (1989)
    /*compressive_yield_strength*/        6.0 * si::megapascal, // brittle, effectively the same as fracture strength
    /*tensile_yield_strength*/            1.0 * si::megapascal, // brittle, effectively the same as fracture strength
    /*shear_yield_strength*/              1.1 * si::megapascal, // brittle, effectively the same as fracture strength

    /*chemical_susceptibility_estimate*/  false
};
compound::phase::PartlyKnownSolid copper{

    /*specific_heat_capacity*/            0.385 * si::joule / (si::gram * si::kelvin), // wikipedia
    /*thermal_conductivity*/              401.0 * si::watt / (si::meter * si::kelvin), // wikipedia
    /*dynamic_viscosity*/                 std::monostate(),
    /*density*/                           8960.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    std::monostate(),
    /*refractive_index*/                  
        compound::field::SpectralFunction<double>([](const si::wavenumber nhi, const si::wavenumber nlo, const si::pressure p, const si::temperature T) {
            auto l = 2.0 / (nhi+nlo);
            constexpr double n = 0.059513; 
            constexpr auto dndl = 13.100 / si::micrometer;
            return n + si::unitless(dndl * l);
        }), 
    /*spectral_reflectance*/              std::monostate(),

    /*bulk_modulus*/                      130.0 * si::gigapascal,
    /*tensile_modulus*/                   117.0 * si::gigapascal,
    /*shear_modulus*/                     44.7 * si::gigapascal,
    /*pwave_modulus*/                     161.5 * si::gigapascal,
    /*lame_parameter*/                    72.14 * si::gigapascal,
    /*poisson_ratio*/                     0.33,

    /*compressive_fracture_strength*/     45.0 * si::megapascal,
    /*tensile_fracture_strength*/         220.0 * si::megapascal, // engineering toolbox
    /*shear_fracture_strength*/           172.0 * si::megapascal,
    /*compressive_yield_strength*/        33.3 * si::megapascal,
    /*tensile_yield_strength*/            70.0 * si::megapascal, // engineering toolbox
    /*shear_yield_strength*/              std::monostate(),

    /*chemical_susceptibility_estimate*/  std::monostate(),
}; 
compound::phase::PartlyKnownSolid dummy {
    /*specific_heat_capacity*/            1.0 * si::joule / (si::gram * si::kelvin), 
    /*thermal_conductivity*/              2.0 * si::watt / (si::meter * si::kelvin), 
    /*dynamic_viscosity*/                 3.0 * si::poise, 
    /*density*/                           4.0 * si::kilogram/si::meter3,
    /*vapor_pressure*/                    5.0 * si::megapascal,
    /*refractive_index*/                  1.3,
    /*spectral_reflectance*/              0.5,

    /*bulk_modulus*/                      6.0 * si::gigapascal, 
    /*tensile_modulus*/                   7.0 * si::gigapascal, 
    /*shear_modulus*/                     8.0 * si::gigapascal, 
    /*pwave_modulus*/                     9.0 * si::gigapascal, 
    /*lame_parameter*/                    10.0 * si::gigapascal,
    /*poisson_ratio*/                     0.3, 

    /*compressive_fracture_strength*/     11.0 * si::megapascal,
    /*tensile_fracture_strength*/         12.0 * si::megapascal,
    /*shear_fracture_strength*/           13.1 * si::megapascal,
    /*compressive_yield_strength*/        14.0 * si::megapascal,
    /*tensile_yield_strength*/            15.0 * si::megapascal,
    /*shear_yield_strength*/              16.1 * si::megapascal,

    /*chemical_susceptibility_estimate*/  false
};

int PartlyKnownSolid_attribute_index_sum(const compound::phase::PartlyKnownSolid& solid)
{
    return
        solid.specific_heat_capacity          .index() +
        solid.thermal_conductivity            .index() +
        solid.dynamic_viscosity               .index() +
        solid.density                         .index() +
        solid.vapor_pressure                  .index() +
        solid.refractive_index                .index() +
        solid.spectral_reflectance            .index() +

        solid.bulk_modulus                    .index() +
        solid.tensile_modulus                 .index() +
        solid.shear_modulus                   .index() +
        solid.pwave_modulus                   .index() +
        solid.lame_parameter                  .index() +
        solid.poisson_ratio                   .index() +

        solid.compressive_fracture_strength   .index() +
        solid.tensile_fracture_strength       .index() +
        solid.shear_fracture_strength         .index() +
        solid.compressive_yield_strength      .index() +
        solid.tensile_yield_strength          .index() +
        solid.shear_yield_strength            .index() +

        solid.chemical_susceptibility_estimate.index();
}
TEST_CASE( "PartlyKnownSolid value_or() purity", "[field]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.value_or(unknown) == unknown.value_or(unknown));
        CHECK(ice.value_or(ice) == ice.value_or(ice));
        CHECK(copper.value_or(copper) == copper.value_or(copper));
    	CHECK(dummy.value_or(dummy) == dummy.value_or(dummy));
    }
}
TEST_CASE( "PartlyKnownSolid value_or() identity", "[field]" ) {
	SECTION("There exists a entry that when applied to a function returns the original entry")
	{
    	CHECK(unknown.value_or(unknown) == unknown);
        CHECK(ice.value_or(unknown) == ice);
        CHECK(copper.value_or(unknown) == copper);
    	CHECK(dummy.value_or(unknown) == dummy);
    }
}

TEST_CASE( "PartlyKnownSolid value_or() associativity", "[field]" ) {
	SECTION("Functions can be applied in any order and still produce the same results")
	{
        CHECK(ice.value_or(copper.value_or(dummy)) == 
              ice.value_or(copper).value_or(dummy));

        CHECK(ice.value_or(dummy.value_or(copper)) == 
              ice.value_or(dummy).value_or(copper));

        
        CHECK(copper.value_or(ice.value_or(dummy)) == 
              copper.value_or(ice).value_or(dummy));

        CHECK(copper.value_or(dummy.value_or(ice)) == 
              copper.value_or(dummy).value_or(ice));

        
        CHECK(dummy.value_or(ice.value_or(dummy)) == 
              dummy.value_or(ice).value_or(dummy));

        CHECK(dummy.value_or(dummy.value_or(ice)) == 
              dummy.value_or(dummy).value_or(ice));
    }
}

TEST_CASE( "PartlyKnownSolid value_or() increasing", "[field]" ) {
	SECTION("An attribute of a function's return entry either increases or remains the same when compared to the same attribute of the input entry")
	{

    	CHECK(PartlyKnownSolid_attribute_index_sum(unknown.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(unknown));
    	CHECK(PartlyKnownSolid_attribute_index_sum(unknown.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(unknown));
    	CHECK(PartlyKnownSolid_attribute_index_sum(unknown.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(unknown));
    	CHECK(PartlyKnownSolid_attribute_index_sum(unknown.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(unknown));


    	CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(ice));
    	CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(ice));
    	CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(ice));
    	CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(ice));


    	CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(copper));
    	CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(copper));
    	CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(copper));
    	CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(copper));


    	CHECK(PartlyKnownSolid_attribute_index_sum(dummy.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(dummy));
    	CHECK(PartlyKnownSolid_attribute_index_sum(dummy.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(dummy));
    	CHECK(PartlyKnownSolid_attribute_index_sum(dummy.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(dummy));
    	CHECK(PartlyKnownSolid_attribute_index_sum(dummy.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(dummy));

    }
}

