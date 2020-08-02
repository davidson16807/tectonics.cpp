
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "StratumMassPoolStore.hpp"
#include "StratumMassPool_test_utils.hpp"

using namespace strata;

TEST_CASE( "StratumMassPoolStore compress/decompress invertibility", "[strata]" ) {
  	std::mt19937 generator(2);

	StratumMassPool original = get_random_stratum_mass_pool(generator);
	
	StratumMassPoolStore stratum_mass_pool_store;
	stratum_mass_pool_store.compress(original);

	StratumMassPool reconstructed;
	stratum_mass_pool_store.decompress(reconstructed);

    SECTION("compressing a StratumMassPool object then decompressing it must reproduce the original object to within acceptable tolerances"){
    	APPROXIMATES(reconstructed, original);
	}
}