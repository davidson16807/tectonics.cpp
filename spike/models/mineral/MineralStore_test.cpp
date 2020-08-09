
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "MineralStore.hpp"
#include "Mineral_test_utils.hpp"

using namespace mineral;

TEST_CASE( "MineralStore compress/decompress invertibility", "[mineral]" ) {
  	std::mt19937 generator(2);

	Mineral original = get_random_mineral_mineral(generator);
	
	MineralStore mineral_store;
	mineral_store.compress(original);

	Mineral reconstructed;
	mineral_store.decompress(reconstructed);

    SECTION("compressing a Mineral object then decompressing it must reproduce the original object to within acceptable tolerances"){
    	STRATUM_MASS_POOL_EQUAL(reconstructed, original);
	}
}