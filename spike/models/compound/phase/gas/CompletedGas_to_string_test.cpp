
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "CompletedGas_to_string.hpp"
#include "CompletedGas_test_utils.hpp"

TEST_CASE( "CompletedGas to_string() purity", "[field]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
		CHECK(compound::phase::to_string(known_nitrogen)
		   == compound::phase::to_string(known_nitrogen));
		CHECK(compound::phase::to_string(known_steam)
		   == compound::phase::to_string(known_steam));
		CHECK(compound::phase::to_string(known_dummy_gas)
		   == compound::phase::to_string(known_dummy_gas));
    }
}

TEST_CASE( "CompletedGas to_string() appreciable difference preservation", "[field]" ) {
	compound::phase::CompletedGas isobaric_specific_heat_capacity       = known_dummy_gas; isobaric_specific_heat_capacity       .isobaric_specific_heat_capacity = 1.01 * isobaric_specific_heat_capacity       .isobaric_specific_heat_capacity (compound::field::StateParameters());
	compound::phase::CompletedGas nudge_thermal_conductivity            = known_dummy_gas; nudge_thermal_conductivity            .thermal_conductivity            = 1.01 * nudge_thermal_conductivity            .thermal_conductivity            (compound::field::StateParameters());
	compound::phase::CompletedGas nudge_dynamic_viscosity               = known_dummy_gas; nudge_dynamic_viscosity               .dynamic_viscosity               = 1.01 * nudge_dynamic_viscosity               .dynamic_viscosity               (compound::field::StateParameters());
	compound::phase::CompletedGas nudge_density                         = known_dummy_gas; nudge_density                         .density                         = 1.01 * nudge_density                         .density                         (compound::field::StateParameters());
	compound::phase::CompletedGas nudge_refractive_index                = known_dummy_gas; nudge_refractive_index                .refractive_index                = 1.01 * nudge_refractive_index                .refractive_index                (compound::field::SpectralParameters());
	
	SECTION("Appreciable differences in input are preserved within output")
	{
    	CHECK(compound::phase::to_string(isobaric_specific_heat_capacity)       != compound::phase::to_string(known_dummy_gas));
		CHECK(compound::phase::to_string(nudge_thermal_conductivity     )       != compound::phase::to_string(known_dummy_gas));
		CHECK(compound::phase::to_string(nudge_dynamic_viscosity        )       != compound::phase::to_string(known_dummy_gas));
		CHECK(compound::phase::to_string(nudge_density                  )       != compound::phase::to_string(known_dummy_gas));
		CHECK(compound::phase::to_string(nudge_refractive_index         )       != compound::phase::to_string(known_dummy_gas));
    }
}
