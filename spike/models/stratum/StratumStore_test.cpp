
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "StratumStore.hpp"
#include "Stratum_test_utils.hpp"

using namespace stratum;

TEST_CASE( "StratumStore pack/unpack invertibility", "[stratum]" ) {
  	std::mt19937 generator(2);
  	const int M = 15;

	Stratum<M> original = stratum::get_random<M>(generator);

	StratumStore<M> stratum_mineral_store;
	stratum_mineral_store.pack(original);

	Stratum<M> reconstructed = stratum::get_random<M>(generator);
	stratum_mineral_store.unpack(reconstructed);

    SECTION("packing a Stratum object then unpacking it must reproduce the original object to within defined tolerances"){
		STRATUM_EQUAL(original, reconstructed)
	}
}
TEST_CASE( "StratumStore memory constraints", "[strata]" ) {
    SECTION("a StratumStore must fit within an expected memory footprint for a given number of layers and mass pools"){
		CHECK(sizeof(StratumStore<15>) <= 188); //bytes
	}
}