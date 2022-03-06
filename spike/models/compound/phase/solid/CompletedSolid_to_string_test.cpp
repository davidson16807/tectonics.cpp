
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "CompletedSolid_to_string.hpp"
#include "CompletedSolid_test_utils.hpp"

TEST_CASE( "CompletedSolid to_string() purity", "[field]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
		CHECK(compound::phase::to_string(known_ice)
		   == compound::phase::to_string(known_ice));
		CHECK(compound::phase::to_string(known_dummy_solid)
		   == compound::phase::to_string(known_dummy_solid));
    }
}

TEST_CASE( "CompletedSolid to_string() appreciable difference preservation", "[field]" ) {
	compound::phase::CompletedSolid nudge_isobaric_specific_heat_capacity  = known_dummy_solid; nudge_isobaric_specific_heat_capacity  .isobaric_specific_heat_capacity  = 1.01 * nudge_isobaric_specific_heat_capacity  .isobaric_specific_heat_capacity(compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_thermal_conductivity             = known_dummy_solid; nudge_thermal_conductivity             .thermal_conductivity             = 1.01 * nudge_thermal_conductivity             .thermal_conductivity           (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_dynamic_viscosity                = known_dummy_solid; nudge_dynamic_viscosity                .dynamic_viscosity                = 1.01 * nudge_dynamic_viscosity                .dynamic_viscosity              (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_density                          = known_dummy_solid; nudge_density                          .density                          = 1.01 * nudge_density                          .density                        (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_vapor_pressure                   = known_dummy_solid; nudge_vapor_pressure                   .vapor_pressure                   = 1.01 * nudge_vapor_pressure                   .vapor_pressure                 (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_refractive_index                 = known_dummy_solid; nudge_refractive_index                 .refractive_index                 = 1.01 * nudge_refractive_index                 .refractive_index;
	compound::phase::CompletedSolid nudge_extinction_coefficient           = known_dummy_solid; nudge_extinction_coefficient           .extinction_coefficient           = 1.01 * nudge_extinction_coefficient           .extinction_coefficient;
	compound::phase::CompletedSolid nudge_absorption_coefficient           = known_dummy_solid; nudge_absorption_coefficient           .absorption_coefficient           = 1.01 * nudge_absorption_coefficient           .absorption_coefficient;
	compound::phase::CompletedSolid nudge_bulk_modulus                     = known_dummy_solid; nudge_bulk_modulus                     .bulk_modulus                     = 1.01 * nudge_bulk_modulus                     .bulk_modulus                   (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_tensile_modulus                  = known_dummy_solid; nudge_tensile_modulus                  .tensile_modulus                  = 1.01 * nudge_tensile_modulus                  .tensile_modulus                (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_shear_modulus                    = known_dummy_solid; nudge_shear_modulus                    .shear_modulus                    = 1.01 * nudge_shear_modulus                    .shear_modulus                  (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_pwave_modulus                    = known_dummy_solid; nudge_pwave_modulus                    .pwave_modulus                    = 1.01 * nudge_pwave_modulus                    .pwave_modulus                  (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_lame_parameter                   = known_dummy_solid; nudge_lame_parameter                   .lame_parameter                   = 1.01 * nudge_lame_parameter                   .lame_parameter                 (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_poisson_ratio                    = known_dummy_solid; nudge_poisson_ratio                    .poisson_ratio                    = 1.01 * nudge_poisson_ratio                    .poisson_ratio                  (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_compressive_fracture_strength    = known_dummy_solid; nudge_compressive_fracture_strength    .compressive_fracture_strength    = 1.01 * nudge_compressive_fracture_strength    .compressive_fracture_strength  (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_tensile_fracture_strength        = known_dummy_solid; nudge_tensile_fracture_strength        .tensile_fracture_strength        = 1.01 * nudge_tensile_fracture_strength        .tensile_fracture_strength      (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_shear_fracture_strength          = known_dummy_solid; nudge_shear_fracture_strength          .shear_fracture_strength          = 1.01 * nudge_shear_fracture_strength          .shear_fracture_strength        (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_compressive_yield_strength       = known_dummy_solid; nudge_compressive_yield_strength       .compressive_yield_strength       = 1.01 * nudge_compressive_yield_strength       .compressive_yield_strength     (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_tensile_yield_strength           = known_dummy_solid; nudge_tensile_yield_strength           .tensile_yield_strength           = 1.01 * nudge_tensile_yield_strength           .tensile_yield_strength         (compound::field::StateParameters());
	compound::phase::CompletedSolid nudge_shear_yield_strength             = known_dummy_solid; nudge_shear_yield_strength             .shear_yield_strength             = 1.01 * nudge_shear_yield_strength             .shear_yield_strength           (compound::field::StateParameters());
	// compound::phase::CompletedSolid nudge_chemical_susceptibility_estimate = known_dummy_solid; nudge_chemical_susceptibility_estimate .chemical_susceptibility_estimate = std::monostate();
	
	SECTION("Appreciable differences in input are preserved within output")
	{
		CHECK(compound::phase::to_string(nudge_isobaric_specific_heat_capacity ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_thermal_conductivity            ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_dynamic_viscosity               ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_density                         ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_vapor_pressure                  ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_refractive_index                ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_extinction_coefficient          ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_absorption_coefficient          ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_bulk_modulus                    ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_tensile_modulus                 ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_shear_modulus                   ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_pwave_modulus                   ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_lame_parameter                  ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_poisson_ratio                   ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_compressive_fracture_strength   ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_tensile_fracture_strength       ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_shear_fracture_strength         ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_compressive_yield_strength      ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_tensile_yield_strength          ) != compound::phase::to_string(known_dummy_solid));
		CHECK(compound::phase::to_string(nudge_shear_yield_strength            ) != compound::phase::to_string(known_dummy_solid));
		// CHECK(compound::phase::to_string(nudge_chemical_susceptibility_estimate) != compound::phase::to_string(known_dummy_solid));
    }
}
