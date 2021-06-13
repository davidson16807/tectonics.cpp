
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "CompletedLiquid_to_string.hpp"
#include "CompletedLiquid_test_utils.hpp"

TEST_CASE( "CompletedLiquid to_string() purity", "[field]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
		CHECK(compound::phase::to_string(known_liquid_nitrogen)
		   == compound::phase::to_string(known_liquid_nitrogen)); 
		CHECK(compound::phase::to_string(known_dummy_liquid)
		   == compound::phase::to_string(known_dummy_liquid));  
    }
}

TEST_CASE( "CompletedLiquid to_string() appreciable difference preservation", "[field]" ) {
	compound::phase::CompletedLiquid nudge_specific_heat_capacity = known_dummy_liquid; nudge_specific_heat_capacity .specific_heat_capacity = 1.01 * nudge_specific_heat_capacity .specific_heat_capacity (compound::field::StateParameters());
	compound::phase::CompletedLiquid nudge_thermal_conductivity   = known_dummy_liquid; nudge_thermal_conductivity   .thermal_conductivity   = 1.01 * nudge_thermal_conductivity   .thermal_conductivity   (compound::field::StateParameters());
	compound::phase::CompletedLiquid nudge_dynamic_viscosity      = known_dummy_liquid; nudge_dynamic_viscosity      .dynamic_viscosity      = 1.01 * nudge_dynamic_viscosity      .dynamic_viscosity      (compound::field::StateParameters());
	compound::phase::CompletedLiquid nudge_density                = known_dummy_liquid; nudge_density                .density                = 1.01 * nudge_density                .density                (compound::field::StateParameters());
	compound::phase::CompletedLiquid nudge_vapor_pressure         = known_dummy_liquid; nudge_vapor_pressure         .vapor_pressure         = 1.01 * nudge_vapor_pressure         .vapor_pressure         (compound::field::StateParameters());
	compound::phase::CompletedLiquid nudge_surface_tension        = known_dummy_liquid; nudge_surface_tension        .surface_tension        = 1.01 * nudge_surface_tension        .surface_tension        (compound::field::StateParameters());
	compound::phase::CompletedLiquid nudge_refractive_index       = known_dummy_liquid; nudge_refractive_index       .refractive_index       = 1.01 * nudge_refractive_index       .refractive_index       (compound::field::SpectralParameters());
	
	SECTION("Appreciable differences in input are preserved within output")
	{
		CHECK(compound::phase::to_string(nudge_specific_heat_capacity) != compound::phase::to_string(known_dummy_liquid));
		CHECK(compound::phase::to_string(nudge_thermal_conductivity  ) != compound::phase::to_string(known_dummy_liquid));
		CHECK(compound::phase::to_string(nudge_dynamic_viscosity     ) != compound::phase::to_string(known_dummy_liquid));
		CHECK(compound::phase::to_string(nudge_density               ) != compound::phase::to_string(known_dummy_liquid));
		CHECK(compound::phase::to_string(nudge_vapor_pressure        ) != compound::phase::to_string(known_dummy_liquid));
		CHECK(compound::phase::to_string(nudge_surface_tension       ) != compound::phase::to_string(known_dummy_liquid));
		CHECK(compound::phase::to_string(nudge_refractive_index      ) != compound::phase::to_string(known_dummy_liquid));
    }
}
