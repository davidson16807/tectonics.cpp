
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <rasters/entities/SpheroidGrid/SpheroidGrid.hpp>
#include <rasters/entities/SpheroidGrid/SpheroidGrid_test_utils.hpp>

#include "Crust_CrustValues_to_Crust.hpp"
#include "Crust_to_CrustValues.hpp"
#include "Crust_test_utils.hpp"

using namespace crust;

TEST_CASE( "CrustValues getter/setter invertibility", "[crust]" ) {
  	std::mt19937 generator(2);
  	const std::size_t L = 16;
  	const std::size_t M = 15;

	auto original = get_random_crust<L,M>(icosahedron_grid, generator);
	auto modified = get_random_crust<L,M>(icosahedron_grid, generator);

	auto pressures = make_CrustValues<float,L>(icosahedron_grid);
	get_max_pressures_received(original, pressures);
	set_max_pressures_received(original, pressures, modified);

    SECTION("updating a Strata object attribute to its original value must produce the original object to within acceptable tolerances"){
		CRUST_EQUAL(original, modified);
	}

	auto temperatures = make_CrustValues<float,L>(icosahedron_grid);
	get_max_temperatures_received(original, temperatures);
	set_max_temperatures_received(original, temperatures, modified);

    SECTION("updating a Strata object attribute to its original value must produce the original object to within acceptable tolerances"){
		CRUST_EQUAL(original, modified);
	}
}