
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "PartlyKnownLiquid_to_string.hpp"
#include "PartlyKnownLiquid_test_utils.hpp"

TEST_CASE( "PartlyKnownLiquid to_string() purity", "[field]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
		CHECK(compound::phase::to_string(unknown_liquid)
		   == compound::phase::to_string(unknown_liquid)); 
		CHECK(compound::phase::to_string(molten_silica)
		   == compound::phase::to_string(molten_silica));  
		CHECK(compound::phase::to_string(liquid_nitrogen)
		   == compound::phase::to_string(liquid_nitrogen));
		CHECK(compound::phase::to_string(dummy_liquid)
		   == compound::phase::to_string(dummy_liquid));   
		CHECK(compound::phase::to_string(liquid_ammonia)
		   == compound::phase::to_string(liquid_ammonia));
    }
}

TEST_CASE( "PartlyKnownLiquid to_string() appreciable difference preservation", "[field]" ) {
	compound::phase::PartlyKnownLiquid nudge_specific_heat_capacity = dummy_liquid; nudge_specific_heat_capacity .specific_heat_capacity = std::monostate();
	compound::phase::PartlyKnownLiquid nudge_thermal_conductivity   = dummy_liquid; nudge_thermal_conductivity   .thermal_conductivity   = std::monostate();
	compound::phase::PartlyKnownLiquid nudge_dynamic_viscosity      = dummy_liquid; nudge_dynamic_viscosity      .dynamic_viscosity      = std::monostate();
	compound::phase::PartlyKnownLiquid nudge_density                = dummy_liquid; nudge_density                .density                = std::monostate();
	compound::phase::PartlyKnownLiquid nudge_vapor_pressure         = dummy_liquid; nudge_vapor_pressure         .vapor_pressure         = std::monostate();
	compound::phase::PartlyKnownLiquid nudge_surface_tension        = dummy_liquid; nudge_surface_tension        .surface_tension        = std::monostate();
	compound::phase::PartlyKnownLiquid nudge_refractive_index       = dummy_liquid; nudge_refractive_index       .refractive_index       = std::monostate();
	compound::phase::PartlyKnownLiquid nudge_extinction_coefficient = dummy_liquid; nudge_extinction_coefficient .refractive_index       = std::monostate();
	
	SECTION("Appreciable differences in input are preserved within output")
	{
		CHECK(compound::phase::to_string(nudge_specific_heat_capacity) != compound::phase::to_string(dummy_liquid));
		CHECK(compound::phase::to_string(nudge_thermal_conductivity  ) != compound::phase::to_string(dummy_liquid));
		CHECK(compound::phase::to_string(nudge_dynamic_viscosity     ) != compound::phase::to_string(dummy_liquid));
		CHECK(compound::phase::to_string(nudge_density               ) != compound::phase::to_string(dummy_liquid));
		CHECK(compound::phase::to_string(nudge_vapor_pressure        ) != compound::phase::to_string(dummy_liquid));
		CHECK(compound::phase::to_string(nudge_surface_tension       ) != compound::phase::to_string(dummy_liquid));
		CHECK(compound::phase::to_string(nudge_refractive_index      ) != compound::phase::to_string(dummy_liquid));
		CHECK(compound::phase::to_string(nudge_extinction_coefficient) != compound::phase::to_string(dummy_liquid));
    }
}
