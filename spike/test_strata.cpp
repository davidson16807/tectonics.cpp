

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include "models/strata.hpp"

using namespace tectonics;

TEST_CASE( "stratum_mass_pool getters and setters must have the inverse property within the valid range", "[stratum_mass_pool]" ) {

	stratum_mass_pool test = stratum_mass_pool();

	SECTION("stratum_mass_pool.max_pressure_received() must have the inverse property within the valid range"){
		test.max_pressure_received(1e9);
		REQUIRE(test.max_pressure_received() == Approx(1e9).epsilon(0.12));
	}
	SECTION("stratum_mass_pool.max_pressure_received() must clamp to the max value when needed"){
		test.max_pressure_received(1e13);
		REQUIRE(test.max_pressure_received() == Approx(exp2(255./6.)).epsilon(0.12));
	}
	SECTION("stratum_mass_pool.max_pressure_received() must clamp to the min value when needed"){
		test.max_pressure_received(0.5);
		REQUIRE(test.max_pressure_received() == Approx(exp2(0./6.)).epsilon(0.12));
	}

	SECTION("stratum_mass_pool.max_temperature_received() must have the inverse property within the valid range"){
		test.max_temperature_received(6000);
		REQUIRE(test.max_temperature_received() == Approx(6000).epsilon(0.12));
	}
	SECTION("stratum_mass_pool.max_temperature_received() must clamp to the max value when needed"){
		test.max_temperature_received(1e13);
		REQUIRE(test.max_temperature_received() == Approx(exp2(255./17.)).epsilon(0.12));
	}
	SECTION("stratum_mass_pool.max_temperature_received() must clamp to the min value when needed"){
		test.max_temperature_received(0.5);
		REQUIRE(test.max_temperature_received() == Approx(exp2(0./17.)).epsilon(0.12));
	}

	SECTION("stratum_mass_pool.grain_size() must have the inverse property within the valid range"){
		test.grain_size(0.1);
		REQUIRE(test.grain_size() == Approx(0.1).epsilon(0.12));
	}
	SECTION("stratum_mass_pool.grain_size() must clamp to the max value when needed"){
		test.grain_size(2);
		REQUIRE(test.grain_size() == Approx(exp2(-0/13.)).epsilon(0.12));
	}
	SECTION("stratum_mass_pool.grain_size() must clamp to the min value when needed"){
		test.grain_size(1e-6);
		REQUIRE(test.grain_size() == Approx(exp2(-255./13.)).epsilon(0.12));
	}
}
