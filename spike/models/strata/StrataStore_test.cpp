
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "StrataStore.hpp"
#include "Strata_test_utils.hpp"

using namespace strata;

TEST_CASE( "StrataStore compress/decompress invertibility", "[strata]" ) {
  	std::mt19937 generator(2);
  	const int L = 16;
  	const int M = 15;

	Strata original = get_random_strata(generator);

	StrataStore<L,M> strata_store;
	strata_store.compress(original);

	Strata reconstructed;
	strata_store.decompress(reconstructed);

    SECTION("compressing a Strata object then decompressing it must reproduce the original object to within acceptable tolerances"){
		STRATA_EQUAL(original, reconstructed);
	}
}