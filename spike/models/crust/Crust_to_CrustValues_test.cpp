
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Crust_CrustValues_to_Crust.hpp"
#include "Crust_to_CrustValues.hpp"
#include "Crust_test_utils.hpp"

using namespace crust;

TEST_CASE( "CrustValues getter/setter invertibility", "[crust]" ) {
  	std::mt19937 generator(2);
  	const int L = 16;
  	const int M = 15;
  	const int N = 10;

	Crust<L,M> original = get_random_crust<L,M>(N, generator);
	Crust<L,M> modified = get_random_crust<L,M>(N, generator);

	CrustValues<float, L> pressures(N);
	get_max_pressures_received(original, pressures);
	set_max_pressures_received(original, pressures, modified);

    SECTION("packing a Crust object then unpacking it must reproduce the original object to within acceptable tolerances"){
		CRUST_EQUAL(original, modified);
	}

	CrustValues<float, L> temperatures(N);
	get_max_temperatures_received(original, temperatures);
	set_max_temperatures_received(original, temperatures, modified);

    SECTION("packing a Crust object then unpacking it must reproduce the original object to within acceptable tolerances"){
		CRUST_EQUAL(original, modified);
	}
}