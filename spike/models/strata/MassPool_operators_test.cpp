
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "MassPool.hpp"
#include "MassPool_operators.hpp"
#include "MassPool_test_utils.hpp"

using namespace strata;

TEST_CASE( "MassPool combine() commutativity", "[strata]" ) {
  	std::mt19937 generator(2);
	MassPool a = get_random_stratum_mass_pool(generator);
	MassPool b = get_random_stratum_mass_pool(generator);

	MassPool ab;
	MassPool bc;
	combine(a, b, ab);
	combine(b, a, bc);

    SECTION("switching arguments for combine() must produce the same MassPool object to within acceptable tolerances"){
    	STRATUM_MASS_POOL_EQUAL(ab, bc)
    }
}

TEST_CASE( "MassPool combine() associativity", "[strata]" ) {
  	std::mt19937 generator(2);
	MassPool a = get_random_stratum_mass_pool(generator);
	MassPool b = get_random_stratum_mass_pool(generator);
	MassPool c = get_random_stratum_mass_pool(generator);

	MassPool ab;
	MassPool ab_c;
	MassPool bc;
	MassPool a_bc;
	combine(a, b, ab);
	combine(ab, c, ab_c);
	combine(b, c, bc);
	combine(a, bc, a_bc);

    SECTION("switching order of invocation for combine() must produce the same MassPool object to within acceptable tolerances"){
    	STRATUM_MASS_POOL_EQUAL(ab_c, a_bc)
    }
}

TEST_CASE( "MassPool combine() closure", "[strata]" ) {
  	std::mt19937 generator(2);
	MassPool a = get_random_stratum_mass_pool(generator);
	MassPool b = get_random_stratum_mass_pool(generator);

	MassPool ab;
	combine(a, b, ab);

    SECTION("the result of passing two valid MassPool objects to combine() must itself produce a valid MassPool"){
    	STRATUM_MASS_POOL_VALID(ab)
    }
}

/*
NOTE: 
there are no tests for the invertibility of combine(), since it is not expect to have that property.
*/