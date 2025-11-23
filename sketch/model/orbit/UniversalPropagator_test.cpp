
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

#include "ElementsAndState.hpp"
#include "UniversalPropagator.hpp"

#include <test/properties.hpp>

namespace orbit {

	template<typename scalar>
	struct Adapter{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;
		using Propagator = orbit::UniversalPropagator<double>;
		using State = orbit::State<double>;

	    static constexpr scalar s0 = scalar(0);

		Propagator propagator;
	    scalar threshold;

	    Adapter(const scalar threshold):
	        threshold(threshold)
	    {}

	    bool equal(const Universals<scalar>& a, const Universals<scalar>& b) const {
	    	State a0(propagator.state(a,s0));
	    	State b0(propagator.state(b,s0));
	    	return 
	    		glm::distance(a0.position, b0.position) < threshold &&
	    		glm::distance(a0.velocity, b0.velocity) < threshold;
	    }

	    std::string print(const Universals<scalar>& orbit) const {
	        return std::to_string(orbit.gravitational_parameter) + " " +
	        	std::to_string(orbit.time_offset) + " " +
	        	glm::to_string(orbit.initial_position) + " " +
	        	glm::to_string(orbit.initial_velocity) + " ";
	    }

	};

	struct EscapeVelocity {
	    double mass;
	    double radius;
	    double velocity;
	};

}

TEST_CASE("Orbit nth period congruence modulo n", "[body]") {

	using vec3 = glm::vec<3,double,glm::defaultp>;

	const auto meter3 = si::meter3;
	const auto kilogram = si::kilogram;
	const auto second = si::second;

	using mass = si::mass<double>;

	using State = orbit::State<double>;
	using Elements = orbit::Elements<double>;
	using Universals = orbit::Universals<double>;
	using ElementsAndState = orbit::ElementsAndState<double>;
	using Properties = orbit::Properties<double>;
	using EscapeVelocity = orbit::EscapeVelocity;

	constexpr auto G = si::gravitational_constant;
	constexpr auto au = si::astronomical_unit;
	const auto mass_of_didymos_dimorphos = 5.4e11 * si::kilogram;      
	const auto mass_of_earth_moon       = 6.0457e24 * si::kilogram;    
	const auto mass_of_jupiter          = si::jupiter_mass;     
	const auto mass_of_saturn           = 5.683e26 * si::kilogram;     
	const auto mass_of_uranus           = 8.681e25 * si::kilogram;     
	const auto mass_of_neptune          = 1.024e26 * si::kilogram;     
	const auto mass_of_pluto_charon     = (1.3e22 + 1.5e21) * si::kilogram; 
	const auto mass_of_sun              = si::solar_mass; 
	const auto mass_of_galaxy           = 1.262e41 * si::kilogram; // back calculated to achieve period of 250 million years

	ElementsAndState converter(Properties(vec3(1,0,0), vec3(0,0,1), G/(meter3/(kilogram*second*second))));

	// parent mass (kg), radius (m), escape velocity (m/s)
	std::vector<EscapeVelocity> escape_velocities = {
	    // {mass_of_earth_moon,  12756.2 / 2.0,    11.186e3},
	    // {mass_of_saturn,      120536  / 2.0,    36.09e3},
	    // {mass_of_jupiter,     142984  / 2.0,    60.20e3},
	    // {mass_of_sun,         432300  / 2.0,   617.5e3},
	    // {mass_of_galaxy,      24000 * 9.4e15, 594e3},
	};

	// (parent_mass, Elements)
	std::vector<std::pair<mass, Elements>> elliptic_periapses = {
	    // TODO: add didymos/dimorphos
	    { mass_of_didymos_dimorphos, Elements(1.144e3, 0.0247, 169.3 * si::degree, 0.0, 0.0, 0.0) }, // Didymos, post-impact
	    { mass_of_earth_moon, Elements(384e6, 0.0549, 0.0   * si::degree, 0.0, 0.0, 0.0) }, // Moon
	    { mass_of_saturn, Elements(186e6,     0.0196, 1.53  * si::degree, 0.0, 0.0, 0.0) }, // Mimas
	    { mass_of_saturn, Elements(238.4e6,   0.0047, 0.02  * si::degree, 0.0, 0.0, 0.0) }, // Enceladus
	    { mass_of_saturn, Elements(295e6,     0.0001, 1.09  * si::degree, 0.0, 0.0, 0.0) }, // Tethys
	    { mass_of_saturn, Elements(377.7e6,   0.0022, 0.02  * si::degree, 0.0, 0.0, 0.0) }, // Dione
	    { mass_of_saturn, Elements(527.2e6,   0.001,  0.35  * si::degree, 0.0, 0.0, 0.0) }, // Rhea
	    { mass_of_saturn, Elements(1221.9e6,  0.0288, 0.33  * si::degree, 0.0, 0.0, 0.0) }, // Titan
	    { mass_of_saturn, Elements(1481.5e6,  0.0274, 0.43  * si::degree, 0.0, 0.0, 0.0) }, // Hyperion
	    { mass_of_saturn, Elements(3561.7e6,  0.0283, 157.0 * si::degree, 0.0, 0.0, 0.0) }, // Iapetus
	    { mass_of_jupiter, Elements(421.8e6,  0.0041, 0.050 * si::degree, 0.0, 0.0, 0.0) }, // Io
	    { mass_of_jupiter, Elements(671.1e6,  0.0090, 0.470 * si::degree, 0.0, 0.0, 0.0) }, // Europa
	    { mass_of_jupiter, Elements(1070.4e6, 0.0013, 0.200 * si::degree, 0.0, 0.0, 0.0) }, // Ganymede
	    { mass_of_jupiter, Elements(1882.7e6, 0.0074, 0.192 * si::degree, 0.0, 0.0, 0.0) }, // Callisto
	    { mass_of_sun, Elements(5.790905e10,  0.2056, 7.005  * si::degree, 0.0, 0.0, 0.0) }, // Mercury
	    { mass_of_sun, Elements(1.082080e11,  0.0068, 3.3947 * si::degree, 0.0, 0.0, 0.0) }, // Venus
	    { mass_of_sun, Elements(1.49598023e11,0.0167, 0.0    * si::degree, 0.0, 0.0, 0.0) }, // Earth
	    { mass_of_sun, Elements(2.279392e11,  0.0934, 1.851  * si::degree, 0.0, 0.0, 0.0) }, // Mars
	    { mass_of_sun, Elements(7.78570e11,   0.0489, 1.305  * si::degree, 0.0, 0.0, 0.0) }, // Jupiter
	    { mass_of_sun, Elements(1.43353e12,   0.0565, 2.484  * si::degree, 0.0, 0.0, 0.0) }, // Saturn
	    { mass_of_sun, Elements(2.87246e12,   0.046,  0.770  * si::degree, 0.0, 0.0, 0.0) }, // Uranus
	    { mass_of_sun, Elements(4.49506e12,   0.009,  1.769  * si::degree, 0.0, 0.0, 0.0) }, // Neptune
	    { mass_of_sun, Elements(39.482,  0.2488, 17.14  * si::degree, 0.0, 0.0, 0.0) }, // Pluto
	    // { mass_of_galaxy, Elements(...)} // sun around galaxy
	};

	// (mass, Universals)
	std::vector<std::pair<mass, Universals>> all_periapses;

    // Elliptics
    for (const auto& me : elliptic_periapses) {
        mass m = me.first;
        const Elements& elements = me.second;
        State state = converter.get_state_from_elements(elements, m/kilogram);
        Universals u = Universals(m/kilogram, state);
        all_periapses.emplace_back(m, u);
    }

	// Parabolics
    for (const auto& mrv : escape_velocities) {
        all_periapses.emplace_back(mrv.mass, 
        	Universals(
	            mrv.mass,
	            0.0,
	            glm::vec3(mrv.radius, 0.0f, 0.0f),
	            glm::vec3(0.0f, mrv.velocity, 0.0f)
	        ));
    }

	// Hyperbolics
    for (const auto& mrv : escape_velocities) {
        all_periapses.emplace_back(mrv.mass, 
        	Universals(
	            mrv.mass,
	            0.0,
	            glm::vec3(mrv.radius, 0.0f, 0.0f),
	            glm::vec3(0.0f, mrv.velocity * 1.5, 0.0f)
	        ));
    }

    SECTION("tare") {

    	orbit::Adapter<double> adapter(1e-5);

		using vec3 = glm::vec<3,double,glm::defaultp>;

		using Universals = orbit::Universals<double>;
		using Propagator = orbit::UniversalPropagator<double>;

		Propagator propagator;
		// `Orbit` is compatible with any unit system as long as it is used consistently
		// here, we defined everything in mks base units.
		Universals earth(1.32712440018e20, 
			vec3(si::astronomical_unit/si::meter,0.0,0.0), 
			vec3(0.0,29.7*si::kilometer/si::meter,0.0));
		Universals moon(3.986004418e12, 
			vec3(384399e3,0.0,0.0),
			vec3(0.0,1.002*si::kilometer/si::meter,0.0));

		std::vector<Universals> orbits{earth, moon};

    	REQUIRE(
    		test::unary_idempotence(adapter, 
    			"orbit.tare()", [=](const Universals& orbit){return propagator.tare(orbit);},
    			orbits
    	));

	}

}

