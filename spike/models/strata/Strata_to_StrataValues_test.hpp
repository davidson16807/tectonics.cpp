
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Strata_to_StrataValues.hpp"
#include "StrataValues_to_Strata.hpp"
#include "Strata_test_utils.hpp"

using namespace strata;

TEST_CASE( "StrataValues getter/setter invertibility", "[strata]" ) {
  	std::mt19937 generator(2);
  	const int L = 16;
  	const int M = 15;

	Strata<L,M> original = get_random_strata<L,M>(generator);
	Strata<L,M> modified;

	StrataValues<float, L> pressures;
	get_max_pressures_received_by_strata(original, pressures);
	set_max_pressures_received_by_strata(original, pressures, modified);

    SECTION("compressing a Strata object then decompressing it must reproduce the original object to within acceptable tolerances"){
		STRATA_EQUAL(original, modified);
	}

	StrataValues<float, L> temperatures;
	get_max_temperatures_received_by_strata(original, temperatures);
	set_max_temperatures_received_by_strata(original, temperatures, modified);

    SECTION("compressing a Strata object then decompressing it must reproduce the original object to within acceptable tolerances"){
		STRATA_EQUAL(original, modified);
	}
}