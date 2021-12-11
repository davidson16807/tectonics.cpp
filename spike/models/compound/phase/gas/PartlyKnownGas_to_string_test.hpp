
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "PartlyKnownGas_to_string.hpp"
#include "PartlyKnownGas_test_utils.hpp"

TEST_CASE( "PartlyKnownGas to_string() purity", "[field]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
		CHECK(compound::phase::to_string(unknown_gas)
		   == compound::phase::to_string(unknown_gas));
		CHECK(compound::phase::to_string(steam)
		   == compound::phase::to_string(steam));
		CHECK(compound::phase::to_string(nitrogen_gas)
		   == compound::phase::to_string(nitrogen_gas));
		CHECK(compound::phase::to_string(dummy_gas)
		   == compound::phase::to_string(dummy_gas));
		CHECK(compound::phase::to_string(perflouromethane_gas)
		   == compound::phase::to_string(perflouromethane_gas));
    }
}

TEST_CASE( "PartlyKnownGas to_string() appreciable difference preservation", "[field]" ) {
	compound::phase::PartlyKnownGas nudge_isobaric_specific_heat_capacity = dummy_gas; nudge_isobaric_specific_heat_capacity .isobaric_specific_heat_capacity = std::monostate();
	compound::phase::PartlyKnownGas nudge_thermal_conductivity            = dummy_gas; nudge_thermal_conductivity            .thermal_conductivity            = std::monostate();
	compound::phase::PartlyKnownGas nudge_dynamic_viscosity               = dummy_gas; nudge_dynamic_viscosity               .dynamic_viscosity               = std::monostate();
	compound::phase::PartlyKnownGas nudge_density                         = dummy_gas; nudge_density                         .density                         = std::monostate();
	compound::phase::PartlyKnownGas nudge_refractive_index                = dummy_gas; nudge_refractive_index                .refractive_index                = std::monostate();
	
	SECTION("Appreciable differences in input are preserved within output")
	{
    	CHECK(compound::phase::to_string(nudge_isobaric_specific_heat_capacity) != compound::phase::to_string(dummy_gas));
		CHECK(compound::phase::to_string(nudge_thermal_conductivity  )          != compound::phase::to_string(dummy_gas));
		CHECK(compound::phase::to_string(nudge_dynamic_viscosity     )          != compound::phase::to_string(dummy_gas));
		CHECK(compound::phase::to_string(nudge_density               )          != compound::phase::to_string(dummy_gas));
		CHECK(compound::phase::to_string(nudge_refractive_index      )          != compound::phase::to_string(dummy_gas));
    }
}
