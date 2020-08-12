#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <many/types.hpp>
#include <many/statistic.hpp>
#include "Crust_operators.hpp"
#include "Crust_to_floats.hpp"
#include "Crust_test_utils.hpp"

using namespace crust;


TEST_CASE( "Crust get_sediment() closure", "[crust]" ) {
    std::mt19937 generator(2);
    const int L = 1;
    const int L2 = 16;
    const int M = 15;
    const int N = 10;
    Crust<L2,M> a = get_random_crust<L2,M>(N, generator);

    Crust<L,M> fa(N);
    get_sediment(a, fa);

    SECTION("the result of passing a valid Crust object to get_sediment() must itself produce a valid Crust"){
      CRUST_VALID(fa)
    }
}

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

TEST_CASE( "Crust overlap() mass conservation", "[crust]" ) {
    std::mt19937 generator(2);
    const int L = 16;
    const int M = 15;
    const int N = 10;
    Crust<L,M> a = get_random_crust<L,M>(N, generator);
    Crust<L,M> b = get_random_crust<L,M>(N, generator);

    Crust<L,M> ab(N);
    overlap(a, b, ab);

    many::floats ab_mass(N); get_mass(ab, ab_mass);
    many::floats a_mass(N); get_mass(a, a_mass);
    many::floats b_mass(N); get_mass(b, b_mass);

    SECTION("the result of passing two valid Crust<L,M> objects to overlap() must produce a Crust<L,M> of equivalent mass"){
      CHECK(many::sum(ab_mass) == Approx(many::sum(a_mass) + many::sum(b_mass)).epsilon(1e-4));
    }
}


/*
NOTE: 
there are no tests for the invertibility or commutativity of overlap(), since it is not expect to have that property.
*/



TEST_CASE( "Crust simplify() closure", "[crust]" ) {
    std::mt19937 generator(2);
    const int L = 16;
    const int M = 15;
    const int N = 10;
    Crust<L,M> a = get_random_crust<L,M>(N, generator);
    Crust<L,M> fa = get_random_crust<L,M>(N, generator);

    simplify(a, fa);

    SECTION("the result of passing a valid Crust object to simplify() must itself produce a valid Crust"){
      CRUST_VALID(fa)
    }
}

TEST_CASE( "Crust simplify() mass conservation", "[crust]" ) {
    std::mt19937 generator(2);
    const int L = 16;
    const int M = 15;
    const int N = 10;
    Crust<L,M> a = get_random_crust<L,M>(N, generator);
    Crust<L,M> fa = get_random_crust<L,M>(N, generator);

    simplify(a, fa);

    many::floats fa_mass(N); get_mass(fa, fa_mass);
    many::floats a_mass(N);  get_mass(a,  a_mass );

    SECTION("the result of passing a valid Crust object to simplify() must itself produce a Crust object of equivalent mass"){
      CHECK(many::sum(fa_mass) == Approx(many::sum(a_mass)).epsilon(1e-4));
    }
}