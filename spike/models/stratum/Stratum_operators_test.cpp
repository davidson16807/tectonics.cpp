
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "Stratum.hpp"
#include "Stratum_operators.hpp"
#include "Stratum_test_utils.hpp"

using namespace stratum;



TEST_CASE( "Stratum scale() associativity", "[stratum]" ) {
  	std::mt19937 generator(2);
  	const int M = 15;
	Stratum<M> a = get_random_stratum<M>(generator);
	float b = 2.0f;
	float c = 3.0f;

	Stratum<M> ab(0,0,0);
	Stratum<M> ab_c;
	Stratum<M> a_bc;
	scale(a, b, ab);
	scale(ab, c, ab_c);
	scale(a, b*c, a_bc);

    SECTION("switching order of invocation for scale() must produce the same Stratum object to within acceptable tolerances"){
    	STRATUM_EQUAL(ab_c, a_bc)
    }
}

TEST_CASE( "Stratum scale() closure", "[stratum]" ) {
  	std::mt19937 generator(2);
  	const int M = 15;
	Stratum<M> a = get_random_stratum<M>(generator);
	float b = 2.0f;

	Stratum<M> ab;
	scale(a, b, ab);

    SECTION("the result of passing a valid Stratum object to scale() must itself produce a valid Stratum"){
    	STRATUM_VALID(ab)
    }
}
TEST_CASE( "Stratum scale() identity", "[stratum]" ) {
  	std::mt19937 generator(2);
  	const int M = 15;
	Stratum<M> a = get_random_stratum<M>(generator);
	float b = 1.0f;

	Stratum<M> ab;
	scale(a, b, ab);

    SECTION("there is a value that can be passed to scale() that produces the original Stratum"){
    	STRATUM_EQUAL(ab, a)
    }
}




TEST_CASE( "Stratum combine() commutativity", "[stratum]" ) {
  	std::mt19937 generator(2);
  	const int M = 15;
	Stratum<M> a = get_random_stratum<M>(generator);
	Stratum<M> b = get_random_stratum<M>(generator);

	Stratum<M> ab;
	Stratum<M> bc;
	combine(a, b, ab);
	combine(b, a, bc);

    SECTION("switching arguments for combine() must produce the same Stratum object to within acceptable tolerances"){
    	STRATUM_EQUAL(ab, bc)
    }
}

TEST_CASE( "Stratum combine() associativity", "[stratum]" ) {
  	std::mt19937 generator(2);
  	const int M = 15;
	Stratum<M> a = get_random_stratum<M>(generator);
	Stratum<M> b = get_random_stratum<M>(generator);
	Stratum c = get_random_stratum<M>(generator);

	Stratum<M> ab;
	Stratum<M> ab_c;
	Stratum<M> bc;
	Stratum<M> a_bc;
	combine(a, b, ab);
	combine(ab, c, ab_c);
	combine(b, c, bc);
	combine(a, bc, a_bc);

    SECTION("switching order of invocation for combine() must produce the same Stratum object to within acceptable tolerances"){
    	STRATUM_EQUAL(ab_c, a_bc)
    }
}

TEST_CASE( "Stratum combine() closure", "[stratum]" ) {
  	std::mt19937 generator(2);
  	const int M = 15;
	Stratum<M> a = get_random_stratum<M>(generator);
	Stratum<M> b = get_random_stratum<M>(generator);

	Stratum<M> ab;
	combine(a, b, ab);

    SECTION("the result of passing two valid Stratum objects to combine() must itself produce a valid Stratum"){
    	STRATUM_VALID(ab)
    }
}
TEST_CASE( "Stratum combine() identity", "[stratum]" ) {
  	std::mt19937 generator(2);
  	const int M = 15;
	Stratum<M> a = get_random_stratum<M>(generator);
	// default constructor is required to be identity
	Stratum<M> b;
	// test to make sure that any stratum with zero mass is an identity, 
	// regardless of grain type distribution within mass pools 
	b.minerals[0].grain_type_relative_volume[0] = 1000.0f;

	Stratum<M> ab;
	combine(a, b, ab);

    SECTION("there is a value that can be passed to combine() that produces the original Stratum"){
    	STRATUM_EQUAL(ab, a)
    }
}

/*
NOTE: 
there are no tests for the invertibility of combine(), since it is not expect to have that property.
*/