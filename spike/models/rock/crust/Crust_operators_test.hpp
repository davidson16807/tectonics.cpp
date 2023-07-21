#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <series/types.hpp>
#include <series/statistic.hpp>

#include <rasters/entities/SpheroidGrid/SpheroidGrid.hpp>
#include <rasters/entities/SpheroidGrid/SpheroidGrid_test_utils.hpp>

#include "Crust_operators.hpp"
#include "Crust_to_floats.hpp"
#include "Crust_test_utils.hpp"

using namespace crust;


TEST_CASE( "Crust get_sediment() closure", "[crust]" ) {
    std::mt19937 generator(2);
    const std::size_t L = 1;
    const std::size_t L2 = 16;
    const std::size_t M = 15;
    auto a = get_random_crust<L2,M>(icosahedron_grid, generator);

    auto fa = make_Crust<L,M>(icosahedron_grid);
    get_sediment(a, fa);

    SECTION("the result of passing a valid Crust object to get_sediment() must itself produce a valid Crust"){
      CRUST_VALID(fa)
    }
}

TEST_CASE( "Crust overlap() associativity", "[crust]" ) {
    std::mt19937 generator(2);
    const std::size_t L = 16;
    const std::size_t M = 15;
    auto a = get_random_crust<L,M>(icosahedron_grid, generator);
    auto b = get_random_crust<L,M>(icosahedron_grid, generator);
    auto c = get_random_crust<L,M>(icosahedron_grid, generator);

    auto ab = make_Crust<L,M>(icosahedron_grid);
    auto ab_c = make_Crust<L,M>(icosahedron_grid);
    auto bc = make_Crust<L,M>(icosahedron_grid);
    auto a_bc = make_Crust<L,M>(icosahedron_grid);
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
  	const std::size_t L = 16;
  	const std::size_t M = 15;
  	auto a = get_random_crust<L,M>(icosahedron_grid, generator);
  	auto b = get_random_crust<L,M>(icosahedron_grid, generator);

  	auto ab = make_Crust<L,M>(icosahedron_grid);
  	overlap(a, b, ab);

    SECTION("the result of passing two valid Crust<L,M> objects to overlap() must itself produce a valid Crust<L,M>"){
    	CRUST_VALID(ab)
    }
}

TEST_CASE( "Crust overlap() identity", "[strata]" ) {
    std::mt19937 generator(2);
    const std::size_t L = 16;
    const std::size_t M = 15;
    auto a = get_random_crust<L,M>(icosahedron_grid, generator);
    auto b = make_Crust<L,M>(icosahedron_grid);

    auto ab = make_Crust<L,M>(icosahedron_grid);
    overlap(a, b, ab);

    SECTION("there is a value that can be passed to overlap() that produces the original Crust"){
      CRUST_EQUAL(ab, a)
    }
}

TEST_CASE( "Crust overlap() mass conservation", "[crust]" ) {
    std::mt19937 generator(2);
    const std::size_t L = 16;
    const std::size_t M = 15;
    auto a = get_random_crust<L,M>(icosahedron_grid, generator);
    auto b = get_random_crust<L,M>(icosahedron_grid, generator);

    auto ab = make_Crust<L,M>(icosahedron_grid);
    overlap(a, b, ab);

    auto ab_mass = rasters::make_Raster<float>(icosahedron_grid); get_mass(ab, ab_mass);
    auto a_mass = rasters::make_Raster<float>(icosahedron_grid); get_mass(a, a_mass);
    auto b_mass = rasters::make_Raster<float>(icosahedron_grid); get_mass(b, b_mass);

    SECTION("the result of passing two valid auto objects = make_Crust<L,M>(icosahedron_grid) to overlap() must produce a Crust<L,M> of equivalent mass"){
      CHECK(series::sum(ab_mass) == Approx(series::sum(a_mass) + series::sum(b_mass)).epsilon(1e-4));
    }
}


/*
NOTE: 
there are no tests for the invertibility or commutativity of overlap(), since it is not expect to have that property.
*/



TEST_CASE( "Crust simplify() closure", "[crust]" ) {
    std::mt19937 generator(2);
    const std::size_t L = 16;
    const std::size_t M = 15;
    auto a = get_random_crust<L,M>(icosahedron_grid, generator);
    auto fa = get_random_crust<L,M>(icosahedron_grid, generator);

    simplify(a, fa);

    SECTION("the result of passing a valid Crust object to simplify() must itself produce a valid Crust"){
      CRUST_VALID(fa)
    }
}

TEST_CASE( "Crust simplify() mass conservation", "[crust]" ) {
    std::mt19937 generator(2);
    const std::size_t L = 16;
    const std::size_t M = 15;
    auto a = get_random_crust<L,M>(icosahedron_grid, generator);
    auto fa = get_random_crust<L,M>(icosahedron_grid, generator);

    simplify(a, fa);

    auto fa_mass = rasters::make_Raster<float>(icosahedron_grid); get_mass(fa, fa_mass);
    auto a_mass = rasters::make_Raster<float>(icosahedron_grid);  get_mass(a,  a_mass );

    SECTION("the result of passing a valid Crust object to simplify() must itself produce a Crust object of equivalent mass"){
      CHECK(series::sum(fa_mass) == Approx(series::sum(a_mass)).epsilon(1e-4));
    }
}