
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "Mineral.hpp"
#include "Mineral_operators.hpp"
#include "Mineral_test_utils.hpp"

using namespace mineral;

TEST_CASE( "Mineral combine() commutativity", "[mineral]" ) {
  	std::mt19937 generator(2);
	Mineral a = get_random(generator);
	Mineral b = get_random(generator);

	Mineral ab;
	Mineral bc;
	combine(a, b, ab);
	combine(b, a, bc);

    SECTION("switching arguments for combine() must produce the same Mineral object to within acceptable tolerances"){
    	MINERAL_EQUAL(ab, bc)
    }
}

TEST_CASE( "Mineral combine() associativity", "[mineral]" ) {
  	std::mt19937 generator(2);
	Mineral a = get_random(generator);
	Mineral b = get_random(generator);
	Mineral c = get_random(generator);

	Mineral ab;
	Mineral ab_c;
	Mineral bc;
	Mineral a_bc;
	combine(a, b, ab);
	combine(ab, c, ab_c);
	combine(b, c, bc);
	combine(a, bc, a_bc);

    SECTION("switching order of invocation for combine() must produce the same Mineral object to within acceptable tolerances"){
    	MINERAL_EQUAL(ab_c, a_bc)
    }
}

TEST_CASE( "Mineral combine() closure", "[mineral]" ) {
  	std::mt19937 generator(2);
	Mineral a = get_random(generator);
	Mineral b = get_random(generator);

	Mineral ab;
	combine(a, b, ab);

    SECTION("the result of passing two valid Mineral objects to combine() must itself produce a valid Mineral"){
    	MINERAL_VALID(ab)
    }
}
TEST_CASE( "Mineral combine() identity", "[mineral]" ) {
  	std::mt19937 generator(2);
	Mineral a = get_random(generator);
	Mineral b;

	Mineral ab;
	combine(a, b, ab);

    SECTION("there is a value that can be passed to combine() that produces the original Mineral"){
    	MINERAL_VALID(ab)
    }
}
TEST_CASE( "Mineral combine() mass conservation", "[mineral]" ) {
  	std::mt19937 generator(2);
	Mineral a = get_random(generator);
	Mineral b = get_random(generator);

	Mineral ab;
	combine(a, b, ab);

    SECTION("the result of passing two valid Mineral objects to combine() must produce a new Mineral object with equal mass"){
    	CHECK(ab.mass == a.mass + b.mass);
    }
}

/*
NOTE: 
there are no tests for the invertibility of combine(), since it is not expect to have that property.
*/