
// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "Strata.hpp"
#include "Strata_operators.hpp"
#include "Strata_test_utils.hpp"

using namespace strata;

TEST_CASE( "Strata overlap() associativity", "[strata]" ) {
  	std::mt19937 generator(2);
  	const int L = 16;
  	const int M = 15;
	Strata<L,M> a = get_random<L,M>(generator);
	Strata<L,M> b = get_random<L,M>(generator);
	Strata<L,M> c = get_random<L,M>(generator);

	Strata<L,M> ab;
	Strata<L,M> ab_c;
	Strata<L,M> bc;
	Strata<L,M> a_bc;
	overlap(a, b, ab);
	overlap(ab, c, ab_c);
	overlap(b, c, bc);
	overlap(a, bc, a_bc);

    SECTION("switching order of invocation for overlap() must produce the same Strata object to within acceptable tolerances"){
    	STRATA_EQUAL(ab_c, a_bc)
    }
}

TEST_CASE( "Strata overlap() closure", "[strata]" ) {
  	std::mt19937 generator(2);
  	const int L = 16;
  	const int M = 15;
	Strata<L,M> a = get_random<L,M>(generator);
	Strata<L,M> b = get_random<L,M>(generator);

	Strata<L,M> ab;
	overlap(a, b, ab);

    SECTION("the result of passing two valid Strata objects to overlap() must itself produce a valid Strata"){
    	STRATA_VALID(ab)
    }
}

TEST_CASE( "Strata overlap() identity", "[strata]" ) {
  	std::mt19937 generator(2);
  	const int L = 16;
  	const int M = 15;
	Strata<L,M> a = get_random<L,M>(generator);
	// default constructor is required to be identity
	Strata<L,M> b;

	Strata<L,M> ab;
	overlap(a, b, ab);

    SECTION("there is a value that can be passed to overlap() that produces the original Strata"){
    	STRATA_EQUAL(ab, a)
    }
}

TEST_CASE( "Strata overlap() mass conservation", "[strata]" ) {
    std::mt19937 generator(2);
    const int L = 16;
    const int M = 15;
    Strata<L,M> a = get_random<L,M>(generator);
    Strata<L,M> b = get_random<L,M>(generator);

    Strata<L,M> ab;
    overlap(a, b, ab);

    SECTION("the result of passing two valid Strata objects to overlap() must itself produce a Strata of equivalent mass"){
      CHECK(ab.mass() == Approx(a.mass() + b.mass()).epsilon(1e-4));  
    }
}

/*
NOTE: 
there are no tests for the invertibility of overlap(), since it is not expect to have that property.
*/


TEST_CASE( "Strata simplify() closure", "[strata]" ) {
    std::mt19937 generator(2);
    const int L = 16;
    const int M = 15;
  Strata<L,M> a = get_random<L,M>(generator);

  Strata<L,M> fa;
  simplify(a, fa);

    SECTION("the result of passing a valid Strata object to simplify() must itself produce a valid Strata"){
      STRATA_VALID(fa)
    }
}

TEST_CASE( "Strata simplify() mass conservation", "[strata]" ) {
    std::mt19937 generator(2);
    const int L = 16;
    const int M = 15;
  Strata<L,M> a = get_random<L,M>(generator);

  Strata<L,M> fa;
  simplify(a, fa);

    SECTION("the result of passing a valid Strata object to simplify() must itself produce a Strata object of equivalent mass"){
      CHECK(fa.mass() == Approx(a.mass()).epsilon(1e-4));
    }
}
/*
NOTE: 
there are no tests beside closure for simplify(), 
since it is not a binary operator (nonassociative, nonunary),
and it is deliberately lossy (noninvertible)
*/