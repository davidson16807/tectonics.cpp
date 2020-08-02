
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "Stratum.hpp"
#include "Stratum_operators.hpp"
#include "Stratum_test_utils.hpp"

using namespace strata;



TEST_CASE( "Stratum scale() associativity", "[strata]" ) {
  	std::mt19937 generator(2);
	Stratum a = get_random_stratum(generator);
	float b = 2.0f;
	float c = 3.0f;

	Stratum ab;
	Stratum ab_c;
	Stratum a_bc;
	scale(a, b, ab);
	scale(ab, c, ab_c);
	scale(a, b*c, a_bc);

    SECTION("switching order of invocation for scale() must produce the same Stratum object to within acceptable tolerances"){
    	STRATUM_EQUAL(ab_c, a_bc)
    }
}

TEST_CASE( "Stratum scale() closure", "[strata]" ) {
  	std::mt19937 generator(2);
	Stratum a = get_random_stratum(generator);
	float b = 2.0f;

	Stratum ab;
	scale(a, b, ab);

    SECTION("the result of passing a valid Stratum object to scale() must itself produce a valid Stratum"){
    	STRATUM_VALID(ab)
    }
}




TEST_CASE( "Stratum combine() commutativity", "[strata]" ) {
  	std::mt19937 generator(2);
	Stratum a = get_random_stratum(generator);
	Stratum b = get_random_stratum(generator);

	Stratum ab;
	Stratum bc;
	combine(a, b, oxygen_planet_mass_pool_densities, ab);
	combine(b, a, oxygen_planet_mass_pool_densities, bc);

    SECTION("switching arguments for combine() must produce the same Stratum object to within acceptable tolerances"){
    	STRATUM_EQUAL(ab, bc)
    }
}

TEST_CASE( "Stratum combine() associativity", "[strata]" ) {
  	std::mt19937 generator(2);
	Stratum a = get_random_stratum(generator);
	Stratum b = get_random_stratum(generator);
	Stratum c = get_random_stratum(generator);

	Stratum ab;
	Stratum ab_c;
	Stratum bc;
	Stratum a_bc;
	combine(a, b, oxygen_planet_mass_pool_densities, ab);
	combine(ab, c, oxygen_planet_mass_pool_densities, ab_c);
	combine(b, c, oxygen_planet_mass_pool_densities, bc);
	combine(a, bc, oxygen_planet_mass_pool_densities, a_bc);

    SECTION("switching order of invocation for combine() must produce the same Stratum object to within acceptable tolerances"){
    	STRATUM_EQUAL(ab_c, a_bc)
    }
}

TEST_CASE( "Stratum combine() closure", "[strata]" ) {
  	std::mt19937 generator(2);
	Stratum a = get_random_stratum(generator);
	Stratum b = get_random_stratum(generator);

	Stratum ab;
	combine(a, b, oxygen_planet_mass_pool_densities, ab);

    SECTION("the result of passing two valid Stratum objects to combine() must itself produce a valid Stratum"){
    	STRATUM_VALID(ab)
    }
}

/*
NOTE: 
there are no tests for the invertibility of combine(), since it is not expect to have that property.
*/