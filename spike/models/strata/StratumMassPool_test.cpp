
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "StratumMassPoolStore.hpp"

using namespace strata;

TEST_CASE( "StratumStore compress/decompress invertibility", "[strata]" ) {
  	std::mt19937 generator(2);

	StratumMassPool original;
	original.mass = generator();
	for (int j = 0; j < GrainType::count; ++j)
	{
		original.grain_type_bin_fractional_volume[j] = generator();
	}
	float total_fractional_volume(0);
	for (int j = 0; j < GrainType::count; ++j)
	{
		total_fractional_volume += original.grain_type_bin_fractional_volume[j];
	}
	for (int j = 0; j < GrainType::count; ++j)
	{
		original.grain_type_bin_fractional_volume[j] /= total_fractional_volume;
	}

	StratumMassPoolStore stratum_mass_pool_store;
	stratum_mass_pool_store.compress(original);

	StratumMassPool reconstructed;
	stratum_mass_pool_store.decompress(reconstructed);

    SECTION("compressing a StratumMassPool object then decompressing it must reproduce the original object's mass to within 4 decimal places"){
    	CHECK(reconstructed.mass == Approx(original.mass).epsilon(1e-4));
	}

    SECTION("compressing a StratumMassPool object then decompressing it must reproduce the original object's fractional grain sizes to within 1%"){
		for (int j = 0; j < GrainType::count; ++j)
		{
    		CHECK(reconstructed.grain_type_bin_fractional_volume[j] == Approx(original.grain_type_bin_fractional_volume[j]).margin(0.01));
    	}
    }
}