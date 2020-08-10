#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Crust_operators.hpp"
#include "Crust_test_utils.hpp"

using namespace crust;

TEST_CASE( "Crust overlap() associativity", "[crust]" ) {
    std::mt19937 generator(2);
    const int L = 16;
    const int M = 15;
    const int N = 10;
    Crust<L,M> a = get_random_crust<L,M>(N, generator);
    Crust<L,M> b = get_random_crust<L,M>(N, generator);
    Crust<L,M> c = get_random_crust<L,M>(N, generator);

    Crust<L,M> ab(N);
    Crust<L,M> ab_c(N);
    Crust<L,M> bc(N);
    Crust<L,M> a_bc(N);
    overlap(a, b, ab);
    overlap(ab, c, ab_c);
    overlap(b, c, bc);
    overlap(a, bc, a_bc);

    SECTION("switching order of invocation for overlap() must produce the same Crust<L,M> object to within acceptable tolerances"){
      CRUST_EQUAL(ab_c, a_bc)
    }
}

TEST_CASE( "Crust overlap() closure", "[crust]" ) {
  	std::mt19937 generator(2);
  	const int L = 16;
  	const int M = 15;
  	const int N = 10;
  	Crust<L,M> a = get_random_crust<L,M>(N, generator);
  	Crust<L,M> b = get_random_crust<L,M>(N, generator);

  	Crust<L,M> ab(N);
  	overlap(a, b, ab);

    SECTION("the result of passing two valid Crust<L,M> objects to overlap() must itself produce a valid Crust<L,M>"){
    	CRUST_VALID(ab)
    }
}

TEST_CASE( "Crust overlap() identity", "[strata]" ) {
  	std::mt19937 generator(2);
  	const int L = 16;
  	const int M = 15;
  	const int N = 10;
  	Crust<L,M> a = get_random_crust<L,M>(N, generator);
  	Crust<L,M> b(a.size());

  	Crust<L,M> ab(N);
  	overlap(a, b, ab);

    SECTION("there is a value that can be passed to overlap() that produces the original Crust"){
    	CRUST_EQUAL(ab, a)
    }
}

/*
NOTE: 
there are no tests for the invertibility or commutativity of overlap(), since it is not expect to have that property.
*/