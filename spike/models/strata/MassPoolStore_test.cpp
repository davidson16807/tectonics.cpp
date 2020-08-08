
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "MassPoolStore.hpp"
#include "MassPool_test_utils.hpp"

using namespace strata;

TEST_CASE( "MassPoolStore compress/decompress invertibility", "[strata]" ) {
  	std::mt19937 generator(2);

	MassPool original = get_random_stratum_mass_pool(generator);
	
	MassPoolStore stratum_mass_pool_store;
	stratum_mass_pool_store.compress(original);

	MassPool reconstructed;
	stratum_mass_pool_store.decompress(reconstructed);

    SECTION("compressing a MassPool object then decompressing it must reproduce the original object to within acceptable tolerances"){
    	STRATUM_MASS_POOL_EQUAL(reconstructed, original);
	}
}