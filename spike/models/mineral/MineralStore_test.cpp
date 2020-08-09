
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "MineralStore.hpp"
#include "Mineral_test_utils.hpp"

using namespace mineral;

TEST_CASE( "MineralStore pack/unpack invertibility", "[mineral]" ) {
  	std::mt19937 generator(2);

	Mineral original = get_random(generator);
	
	MineralStore mineral_store;
	mineral_store.pack(original);

	Mineral reconstructed;
	mineral_store.unpack(reconstructed);

    SECTION("packing a Mineral object then unpacking it must reproduce the original object to within acceptable tolerances"){
    	MINERAL_EQUAL(reconstructed, original);
	}
}
TEST_CASE( "MineralStore memory constraints", "[strata]" ) {
    SECTION("a MineralStore must fit within an expected memory footprint"){
		CHECK(sizeof(MineralStore) <= 12 );
	}
}