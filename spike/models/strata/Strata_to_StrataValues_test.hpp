
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Strata_StrataValues_to_Strata.hpp"
#include "Strata_to_StrataValues.hpp"
#include "Strata_test_utils.hpp"

using namespace strata;

TEST_CASE( "StrataValues getter/setter invertibility", "[strata]" ) {
  	std::mt19937 generator(2);
  	const std::size_t L = 16;
  	const std::size_t M = 15;

	Strata<L,M> original = get_random<L,M>(generator);
	Strata<L,M> modified;

	StrataValues<si::pressure<double>, L> pressures;
	get_max_pressures_received(original, pressures);
	set_max_pressures_received(original, pressures, modified);

    SECTION("updating a Strata object attribute to its original value must produce the original object to within acceptable tolerances"){
		STRATA_EQUAL(original, modified);
	}

	StrataValues<si::pressure<double>, L> temperatures;
	get_max_temperatures_received(original, temperatures);
	set_max_temperatures_received(original, temperatures, modified);

    SECTION("updating a Strata object attribute to its original value must produce the original object to within acceptable tolerances"){
		STRATA_EQUAL(original, modified);
	}
}