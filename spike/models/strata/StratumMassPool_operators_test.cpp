
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "StratumMassPool.hpp"
#include "StratumMassPool_operators.hpp"
#include "StratumMassPool_test_utils.hpp"

using namespace strata;

TEST_CASE( "StratumMassPool combine() commutativity", "[strata]" ) {
  	std::mt19937 generator(2);
	StratumMassPool a = get_random_stratum_mass_pool(generator);
	StratumMassPool b = get_random_stratum_mass_pool(generator);

	StratumMassPool ab;
	StratumMassPool bc;
	combine(a, b, ab);
	combine(b, a, bc);

    SECTION("switching arguments for combine() must produce the same StratumMassPool object to within acceptable tolerances"){
    	STRATUM_MASS_POOL_EQUAL(ab, bc)
    }
}

TEST_CASE( "StratumMassPool combine() associativity", "[strata]" ) {
  	std::mt19937 generator(2);
	StratumMassPool a = get_random_stratum_mass_pool(generator);
	StratumMassPool b = get_random_stratum_mass_pool(generator);
	StratumMassPool c = get_random_stratum_mass_pool(generator);

	StratumMassPool ab;
	StratumMassPool ab_c;
	StratumMassPool bc;
	StratumMassPool a_bc;
	combine(a, b, ab);
	combine(ab, c, ab_c);
	combine(b, c, bc);
	combine(a, bc, a_bc);

    SECTION("switching order of invocation for combine() must produce the same StratumMassPool object to within acceptable tolerances"){
    	STRATUM_MASS_POOL_EQUAL(ab_c, a_bc)
    }
}

TEST_CASE( "StratumMassPool combine() closure", "[strata]" ) {
  	std::mt19937 generator(2);
	StratumMassPool a = get_random_stratum_mass_pool(generator);
	StratumMassPool b = get_random_stratum_mass_pool(generator);

	StratumMassPool ab;
	combine(a, b, ab);

    SECTION("the result of passing two valid StratumMassPool objects to combine() must itself produce a valid StratumMassPool"){
    	STRATUM_MASS_POOL_VALID(ab)
    }
}

/*
NOTE: 
there are no tests for the invertibility of combine(), since it is not expect to have that property.
*/