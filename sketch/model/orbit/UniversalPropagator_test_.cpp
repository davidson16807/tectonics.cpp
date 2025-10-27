
// std libraries
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>     // *vec3
#include <glm/geometric.hpp>// glm::distance
#define GLM_ENABLE_EXPERIMENTAL // needed for glm::to_string()
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries
#include <unit/si.hpp>

#include "Orbit.hpp"

#include <test/properties.hpp>

namespace orbit {

	template<typename scalar>
	struct Adapter{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;
		using Propagator = body::OrbitPropagator<double>;
		using State = body::State<double>;

	    static constexpr scalar s0 = scalar(0);

		Propagator propagator;
	    scalar threshold;

	    Adapter(const scalar threshold):
	        threshold(threshold)
	    {}

	    bool equal(const Orbit<scalar>& a, const Orbit<scalar>& b) const {
	    	State a0(propagator.state(a,s0));
	    	State b0(propagator.state(b,s0));
	    	return 
	    		glm::distance(a0.position, b0.position) < threshold &&
	    		glm::distance(a0.velocity, b0.velocity) < threshold;
	    }

	    std::string print(const Orbit<scalar>& orbit) const {
	        return std::to_string(orbit.gravitational_parameter) + " " +
	        	std::to_string(orbit.time_offset) + " " +
	        	glm::to_string(orbit.initial_position) + " " +
	        	glm::to_string(orbit.initial_velocity) + " ";
	    }

	};

}

TEST_CASE("Orbit", "[body]") {
    SECTION("tare") {

    	body::Adapter<double> adapter(1e-5);

		using Orbit = body::Orbit<double>;
		using Propagator = body::OrbitPropagator<double>;
		using vec3 = glm::vec<3,double,glm::defaultp>;
		Propagator propagator;
		// `Orbit` is compatible with any unit system as long as it is used consistently
		// here, we defined everything in mks base units.
		Orbit earth(1.32712440018e20, 
			vec3(si::astronomical_unit/si::meter,0.0,0.0), 
			vec3(0.0,29.7*si::kilometer/si::meter,0.0));
		Orbit moon(3.986004418e12, 
			vec3(384399e3,0.0,0.0),
			vec3(0.0,1.002*si::kilometer/si::meter,0.0));

		std::vector<Orbit> orbits{earth, moon};

    	REQUIRE(
    		test::unary_idempotence(adapter, 
    			"orbit.tare()", [=](const Orbit& orbit){return propagator.tare(orbit);},
    			orbits
    	));

	}

}

