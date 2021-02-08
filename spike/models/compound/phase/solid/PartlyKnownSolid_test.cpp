
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "PartlyKnownSolid.hpp"

#include "PartlyKnownSolid_test_utils.hpp"

TEST_CASE( "PartlyKnownSolid value_or() purity", "[phase]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.value_or(unknown) == unknown.value_or(unknown));
        CHECK(ice.value_or(ice) == ice.value_or(ice));
        CHECK(copper.value_or(copper) == copper.value_or(copper));
    	CHECK(dummy.value_or(dummy) == dummy.value_or(dummy));
    }
}
TEST_CASE( "PartlyKnownSolid value_or() identity", "[phase]" ) {
	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(unknown.value_or(unknown) == unknown);
        CHECK(ice.value_or(unknown) == ice);
        CHECK(copper.value_or(unknown) == copper);
    	CHECK(dummy.value_or(unknown) == dummy);
    }
}

TEST_CASE( "PartlyKnownSolid value_or() associativity", "[phase]" ) {
	SECTION("Functions can be applied in any order and still produce the same results")
	{
        CHECK(ice.value_or(copper.value_or(dummy)) == 
              ice.value_or(copper).value_or(dummy));

        CHECK(ice.value_or(dummy.value_or(copper)) == 
              ice.value_or(dummy).value_or(copper));

        
        CHECK(copper.value_or(ice.value_or(dummy)) == 
              copper.value_or(ice).value_or(dummy));

        CHECK(copper.value_or(dummy.value_or(ice)) == 
              copper.value_or(dummy).value_or(ice));

        
        CHECK(dummy.value_or(ice.value_or(dummy)) == 
              dummy.value_or(ice).value_or(dummy));

        CHECK(dummy.value_or(dummy.value_or(ice)) == 
              dummy.value_or(dummy).value_or(ice));
    }
}

/*
TEST_CASE( "CompletedSolid value_or() increasing", "[phase]" ) {
	SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
	{

    	CHECK(CompletedSolid_attribute_index_sum(unknown.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(unknown));
    	CHECK(CompletedSolid_attribute_index_sum(unknown.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(unknown));
    	CHECK(CompletedSolid_attribute_index_sum(unknown.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(unknown));
    	CHECK(CompletedSolid_attribute_index_sum(unknown.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(unknown));


    	CHECK(CompletedSolid_attribute_index_sum(ice.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(ice));
    	CHECK(CompletedSolid_attribute_index_sum(ice.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(ice));
    	CHECK(CompletedSolid_attribute_index_sum(ice.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(ice));
    	CHECK(CompletedSolid_attribute_index_sum(ice.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(ice));


    	CHECK(CompletedSolid_attribute_index_sum(copper.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(copper));
    	CHECK(CompletedSolid_attribute_index_sum(copper.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(copper));
    	CHECK(CompletedSolid_attribute_index_sum(copper.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(copper));
    	CHECK(CompletedSolid_attribute_index_sum(copper.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(copper));


    	CHECK(CompletedSolid_attribute_index_sum(dummy.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(dummy));
    	CHECK(CompletedSolid_attribute_index_sum(dummy.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(dummy));
    	CHECK(CompletedSolid_attribute_index_sum(dummy.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(dummy));
    	CHECK(CompletedSolid_attribute_index_sum(dummy.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(dummy));

    }
}
*/
