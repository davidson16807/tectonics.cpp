
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "StratumStore.hpp"

using namespace stratum;

TEST_CASE( "StratumStore compress/decompress invertibility", "[stratum]" ) {
  	std::mt19937 generator(2);
  	const int M = 15;

	Stratum<M> original(generator(), generator(), generator());
	for (int i = 0; i < M; ++i)
	{
		original.minerals[i].mass = generator();
		for (int j = 0; j < int(mineral::GrainType::count); ++j)
		{
			original.minerals[i].grain_type_relative_volume[j] = generator();
		}
	}

	StratumStore<M> stratum_mineral_store;
	stratum_mineral_store.compress(original);

	Stratum<M> reconstructed(generator(), generator(), generator());
	stratum_mineral_store.decompress(reconstructed);

    SECTION("compressing a Stratum object then decompressing it must reproduce the original object's mass to within 4 decimal places"){
		for (int i = 0; i < M; ++i)
		{
    		CHECK(reconstructed.minerals[i].mass == Approx(original.minerals[i].mass).epsilon(1e-4));
		}
	}

    SECTION("compressing a Stratum object then decompressing it must reproduce the original object's fractional grain sizes to within 1%"){
		for (int i = 0; i < M; ++i)
		{
			float original_total_relative_volume(original.minerals[i].grain_type_total_relative_volume());
			float reconstructed_total_relative_volume(reconstructed.minerals[i].grain_type_total_relative_volume());
			for (int j = 0; j < int(mineral::GrainType::count); ++j)
			{
	    		CHECK(reconstructed.minerals[i].grain_type_relative_volume[j] / reconstructed_total_relative_volume == 
	    			Approx(original.minerals[i].grain_type_relative_volume[j] / original_total_relative_volume).margin(0.01));
	    	}
		}
    }
}
TEST_CASE( "StratumStore memory constraints", "[strata]" ) {
    SECTION("a StratumStore must fit within an expected memory footprint for a given number of layers and mass pools"){
		CHECK(sizeof(StratumStore<15>) <= 188); //bytes
	}
}