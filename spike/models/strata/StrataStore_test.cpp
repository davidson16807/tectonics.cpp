
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "StrataStore.hpp"
#include "Strata_test_utils.hpp"

using namespace strata;
using namespace stratum;

TEST_CASE( "StrataStore pack/unpack invertibility", "[strata]" ) {
  	std::mt19937 generator(2);
  	const int L = 16;
  	const int M = 15;

	Strata<L,M> original = get_random<L,M>(generator);

	StrataStore<L,M> strata_store;
	strata_store.pack(original);

	Strata<L,M> reconstructed;
	strata_store.unpack(reconstructed);

    SECTION("packing a Strata object then unpacking it must reproduce the original object to within acceptable tolerances"){
		STRATA_EQUAL(original, reconstructed);
	}
}
TEST_CASE( "StrataStore memory constraints", "[strata]" ) {
    SECTION("a StrataStore must fit within an expected memory footprint for a given number of layers and mass pools"){
		CHECK(sizeof(StrataStore<16,15>) <= 3012);
	}
}