
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "StratumStore.hpp"

using namespace strata;

TEST_CASE( "StratumStore compress/decompress invertibility", "[strata]" ) {
  	std::mt19937 generator(2);
  	const int M = 15;

	Stratum<M> original(generator(), generator(), generator());
	for (int i = 0; i < M; ++i)
	{
		original.mass_pools[i].mass = generator();
		for (int j = 0; j < int(GrainType::count); ++j)
		{
			original.mass_pools[i].grain_type_relative_volume[j] = generator();
		}
	}

	StratumStore<M> stratum_mass_pool_store;
	stratum_mass_pool_store.compress(original);

	Stratum<M> reconstructed(generator(), generator(), generator());
	stratum_mass_pool_store.decompress(reconstructed);

    SECTION("compressing a Stratum object then decompressing it must reproduce the original object's mass to within 4 decimal places"){
		for (int i = 0; i < M; ++i)
		{
    		CHECK(reconstructed.mass_pools[i].mass == Approx(original.mass_pools[i].mass).epsilon(1e-4));
		}
	}

    SECTION("compressing a Stratum object then decompressing it must reproduce the original object's fractional grain sizes to within 1%"){
		for (int i = 0; i < M; ++i)
		{
			float original_total_relative_volume(original.mass_pools[i].grain_type_total_relative_volume());
			float reconstructed_total_relative_volume(reconstructed.mass_pools[i].grain_type_total_relative_volume());
			for (int j = 0; j < int(GrainType::count); ++j)
			{
	    		CHECK(reconstructed.mass_pools[i].grain_type_relative_volume[j] / reconstructed_total_relative_volume == 
	    			Approx(original.mass_pools[i].grain_type_relative_volume[j] / original_total_relative_volume).margin(0.01));
	    	}
		}
    }
}