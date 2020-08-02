#pragma once

#include <catch/catch.hpp>

#include "StratumMassPool.hpp"
#include "StratumMassPool_operators.hpp"

namespace strata
{
	/*
	NOTE: 
	This file does not exist within the concept of the mathematical category defined in README.md.
	This is because the file is only relevant within the context of several potential unit test files.
	We differ from the usual style of method signature here since performance does not matter in test code. 
	We also use macros since we require running unit tests on individual attributes for granularity, 
	and our testing framework itself uses macros to run the unit tests.
	Our chief concern here is to simplify writing unit tests and interpreting their output.
	*/

	template<typename Tgenerator>
	StratumMassPool get_random_stratum_mass_pool(Tgenerator generator)
	{
		StratumMassPool output;
		output.mass = generator();
		for (int j = 0; j < grain_type_count; ++j)
		{
			output.grain_type_fractional_volume[j] = generator();
		}
		float total_fractional_volume(0);
		for (int j = 0; j < grain_type_count; ++j)
		{
			total_fractional_volume += output.grain_type_fractional_volume[j];
		}
		for (int j = 0; j < grain_type_count; ++j)
		{
			output.grain_type_fractional_volume[j] /= total_fractional_volume;
		}
		return output;
	}
	
	#define APPROXIMATES(a, b) \
    	CHECK(a.mass == Approx(b.mass).epsilon(1e-4)); \
		for (int j = 0; j < grain_type_count; ++j) \
		{ \
    		CHECK(a.grain_type_fractional_volume[j] == Approx(b.grain_type_fractional_volume[j]).margin(0.01)); \
    	} 
	
	#define VALID(a) \
    	CHECK(a.mass > 0); \
    	float total_fractional_volume(0); \
		for (int j = 0; j < grain_type_count; ++j) \
		{ \
    		CHECK(a.grain_type_fractional_volume[j] > 0); \
    		total_fractional_volume += a.grain_type_fractional_volume[j]; \
    	} \
    	CHECK(total_fractional_volume == Approx(1).epsilon(1e-4));
}

