#pragma once

#include <catch/catch.hpp>

#include "Mineral.hpp"
#include "GrainType.hpp"

namespace mineral
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
	mineral::Mineral get_random(Tgenerator generator)
	{
		mineral::Mineral output;
		output.mass = generator() * si::kilogram;
		output.phase_id = generator() % 16;
		for (int j = 0; j < int(mineral::GrainType::count); ++j)
		{
			output.grain_type_relative_volume[j] = generator();
		}
		return output;
	}
	
	#define MINERAL_EQUAL(a, b)                                                              \
    	CHECK(a.mass/si::kilogram == Approx(b.mass/si::kilogram).epsilon(1e-4));                                                 \
    	CHECK(a.phase_id == b.phase_id);                                                 \
		for (int grain_i = 0; grain_i < int(mineral::GrainType::count); ++grain_i)                                   \
		{                                                                                              \
			float a_total_relative_volume(a.grain_type_total_relative_volume());                           \
			float b_total_relative_volume(b.grain_type_total_relative_volume());                           \
    		CHECK(b.grain_type_relative_volume[grain_i] / b_total_relative_volume ==                   \
    			Approx(a.grain_type_relative_volume[grain_i] / a_total_relative_volume).margin(0.01)); \
    	}    

    #define MINERAL_VALID(a)                                                    \
    	CHECK(a.mass/si::kilogram > -1e-4);                                                  \
    	CHECK(0 <= a.phase_id);                              \
    	CHECK(a.phase_id < 16);                              \
		for (int grain_i = 0; grain_i < int(mineral::GrainType::count); ++grain_i)       \
		{                                                                       \
    		CHECK(a.grain_type_relative_volume[grain_i] > -1e-4);               \
    	}                                                                       
}

