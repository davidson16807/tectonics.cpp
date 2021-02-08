
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <units/si_test_utils.hpp>

#include "PartlyKnownLiquid.hpp"

#include "PartlyKnownLiquid_test_utils.hpp"

TEST_CASE( "PartlyKnownLiquid value_or() purity", "[phase]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.value_or(unknown) == unknown.value_or(unknown));
        CHECK(liquid_nitrogen.value_or(liquid_nitrogen) == liquid_nitrogen.value_or(liquid_nitrogen));
        CHECK(molten_silica.value_or(molten_silica) == molten_silica.value_or(molten_silica));
    	CHECK(dummy.value_or(dummy) == dummy.value_or(dummy));
    }
}
TEST_CASE( "PartlyKnownLiquid value_or() identity", "[phase]" ) {
	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(unknown.value_or(unknown) == unknown);
        CHECK(liquid_nitrogen.value_or(unknown) == liquid_nitrogen);
        CHECK(molten_silica.value_or(unknown) == molten_silica);
    	CHECK(dummy.value_or(unknown) == dummy);
    }
}

TEST_CASE( "PartlyKnownLiquid value_or() associativity", "[phase]" ) {
	SECTION("Functions can be applied in any order and still produce the same results")
	{
        CHECK(liquid_nitrogen.value_or(molten_silica.value_or(dummy)) == 
              liquid_nitrogen.value_or(molten_silica).value_or(dummy));

        CHECK(liquid_nitrogen.value_or(dummy.value_or(molten_silica)) == 
              liquid_nitrogen.value_or(dummy).value_or(molten_silica));

        
        CHECK(molten_silica.value_or(liquid_nitrogen.value_or(dummy)) == 
              molten_silica.value_or(liquid_nitrogen).value_or(dummy));

        CHECK(molten_silica.value_or(dummy.value_or(liquid_nitrogen)) == 
              molten_silica.value_or(dummy).value_or(liquid_nitrogen));

        
        CHECK(dummy.value_or(liquid_nitrogen.value_or(dummy)) == 
              dummy.value_or(liquid_nitrogen).value_or(dummy));

        CHECK(dummy.value_or(dummy.value_or(liquid_nitrogen)) == 
              dummy.value_or(dummy).value_or(liquid_nitrogen));
    }
}

/*
TEST_CASE( "CompletedLiquid value_or() increasing", "[phase]" ) {
	SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
	{

    	CHECK(CompletedLiquid_attribute_index_sum(unknown.value_or(unknown)) >= PartlyKnownLiquid_attribute_index_sum(unknown));
    	CHECK(CompletedLiquid_attribute_index_sum(unknown.value_or(liquid_nitrogen)) >= PartlyKnownLiquid_attribute_index_sum(unknown));
    	CHECK(CompletedLiquid_attribute_index_sum(unknown.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(unknown));
    	CHECK(CompletedLiquid_attribute_index_sum(unknown.value_or(dummy)) >= PartlyKnownLiquid_attribute_index_sum(unknown));


    	CHECK(CompletedLiquid_attribute_index_sum(liquid_nitrogen.value_or(unknown)) >= PartlyKnownLiquid_attribute_index_sum(liquid_nitrogen));
    	CHECK(CompletedLiquid_attribute_index_sum(liquid_nitrogen.value_or(liquid_nitrogen)) >= PartlyKnownLiquid_attribute_index_sum(liquid_nitrogen));
    	CHECK(CompletedLiquid_attribute_index_sum(liquid_nitrogen.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(liquid_nitrogen));
    	CHECK(CompletedLiquid_attribute_index_sum(liquid_nitrogen.value_or(dummy)) >= PartlyKnownLiquid_attribute_index_sum(liquid_nitrogen));


    	CHECK(CompletedLiquid_attribute_index_sum(molten_silica.value_or(unknown)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));
    	CHECK(CompletedLiquid_attribute_index_sum(molten_silica.value_or(liquid_nitrogen)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));
    	CHECK(CompletedLiquid_attribute_index_sum(molten_silica.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));
    	CHECK(CompletedLiquid_attribute_index_sum(molten_silica.value_or(dummy)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));


    	CHECK(CompletedLiquid_attribute_index_sum(dummy.value_or(unknown)) >= PartlyKnownLiquid_attribute_index_sum(dummy));
    	CHECK(CompletedLiquid_attribute_index_sum(dummy.value_or(liquid_nitrogen)) >= PartlyKnownLiquid_attribute_index_sum(dummy));
    	CHECK(CompletedLiquid_attribute_index_sum(dummy.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(dummy));
    	CHECK(CompletedLiquid_attribute_index_sum(dummy.value_or(dummy)) >= PartlyKnownLiquid_attribute_index_sum(dummy));

    }
}
*/
