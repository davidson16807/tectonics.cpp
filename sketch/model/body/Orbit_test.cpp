
// std libraries
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>     // *vec3
#include <glm/geometric.hpp>

// in-house libraries
#include <unit/si.hpp>

#include "Orbit.hpp"

#include <test/properties.hpp>

TEST_CASE("Orbit", "[body]") {
    SECTION("tare") {

    	body::Adapter strict(1e-5);

		using Orbit = body::Orbit<double>;
		using Propagator = body::OrbitPropagator<double>;
		using vec3 = glm::vec<3,double,glm::defaultp>;
		Propagator propagator;
		// everything is in mks base units
		Orbit earth(1.32712440018, 
			vec3(si::astronomical_unit,0.0,0.0), 
			vec3(0.0,29.7*si::kilometer/si::meter,0.0));
		Orbit moon(3.986004418, 
			vec3(384399e3,0.0,0.0),
			vec3(0.0,1.002*si::kilometer/si::meter,0.0));

		std::vector<Orbit> orbits{earth, moon};

    	REQUIRE(
    		test::unary_idempotence(adapter, 
    			"orbit.tare()", [](const Orbit& orbit){return propagator.tare(orbit);},
    			orbits
    	));

	}

}

