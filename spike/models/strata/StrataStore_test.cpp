
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "StrataStore.hpp"
#include "Strata_test_utils.hpp"

using namespace strata;
using namespace stratum;

TEST_CASE( "StrataStore compress/decompress invertibility", "[strata]" ) {
  	std::mt19937 generator(2);
  	const int L = 16;
  	const int M = 15;

	Strata<L,M> original = get_random_strata<L,M>(generator);

	StrataStore<L,M> strata_store;
	strata_store.compress(original);

	Strata<L,M> reconstructed;
	strata_store.decompress(reconstructed);

    SECTION("compressing a Strata object then decompressing it must reproduce the original object to within acceptable tolerances"){
		STRATA_EQUAL(original, reconstructed);
	}
}