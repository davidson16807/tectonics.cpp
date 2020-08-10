
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "MineralStore.hpp"
#include "Mineral_test_utils.hpp"

using namespace mineral;

TEST_CASE( "MineralStore pack/unpack invertibility", "[mineral]" ) {
  	std::mt19937 generator(2);

	Mineral random1 = get_random(generator);
	Mineral random2 = get_random(generator);
	MineralStore random_store;
	random_store.pack(random1);
	random_store.unpack(random2);

	Mineral identity1;
	Mineral identity2 = get_random(generator);
	MineralStore identity_store;
	identity_store.pack(identity1);
	identity_store.unpack(identity2);

    SECTION("packing a Mineral object then unpacking it must reproduce the original object to within acceptable tolerances"){
    	MINERAL_EQUAL(random2, random1);
    	MINERAL_EQUAL(identity2, identity1);
	}
    SECTION("packing a Mineral object then unpacking it must produce a valid Mineral object"){
    	MINERAL_VALID(random2);
    	MINERAL_VALID(identity2);
	}
}
TEST_CASE( "MineralStore memory constraints", "[strata]" ) {
    SECTION("a MineralStore must fit within an expected memory footprint"){
		CHECK(sizeof(MineralStore) <= 12 );
	}
}