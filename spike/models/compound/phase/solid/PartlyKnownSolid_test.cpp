
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
    	CHECK(unknown_solid.value_or(unknown_solid) == unknown_solid.value_or(unknown_solid));
        CHECK(ice.value_or(ice) == ice.value_or(ice));
        CHECK(copper.value_or(copper) == copper.value_or(copper));
    	CHECK(dummy_solid.value_or(dummy_solid) == dummy_solid.value_or(dummy_solid));
    }
}
TEST_CASE( "PartlyKnownSolid value_or() identity", "[phase]" ) {
	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(unknown_solid.value_or(unknown_solid) == unknown_solid);
        CHECK(ice.value_or(unknown_solid) == ice);
        CHECK(copper.value_or(unknown_solid) == copper);
    	CHECK(dummy_solid.value_or(unknown_solid) == dummy_solid);
    }
}

TEST_CASE( "PartlyKnownSolid value_or() associativity", "[phase]" ) {
	SECTION("Functions can be applied in any order and still produce the same results")
	{
        CHECK(ice.value_or(copper.value_or(dummy_solid)) == 
              ice.value_or(copper).value_or(dummy_solid));

        CHECK(ice.value_or(dummy_solid.value_or(copper)) == 
              ice.value_or(dummy_solid).value_or(copper));

        
        CHECK(copper.value_or(ice.value_or(dummy_solid)) == 
              copper.value_or(ice).value_or(dummy_solid));

        CHECK(copper.value_or(dummy_solid.value_or(ice)) == 
              copper.value_or(dummy_solid).value_or(ice));

        
        CHECK(dummy_solid.value_or(ice.value_or(dummy_solid)) == 
              dummy_solid.value_or(ice).value_or(dummy_solid));

        CHECK(dummy_solid.value_or(dummy_solid.value_or(ice)) == 
              dummy_solid.value_or(dummy_solid).value_or(ice));
    }
}

TEST_CASE( "PartlyKnownSolid value_or() increasing", "[phase]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(PartlyKnownSolid_attribute_index_sum(unknown_solid.value_or(unknown_solid)) >= PartlyKnownSolid_attribute_index_sum(unknown_solid));
        CHECK(PartlyKnownSolid_attribute_index_sum(unknown_solid.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(unknown_solid));
        CHECK(PartlyKnownSolid_attribute_index_sum(unknown_solid.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(unknown_solid));
        CHECK(PartlyKnownSolid_attribute_index_sum(unknown_solid.value_or(dummy_solid)) >= PartlyKnownSolid_attribute_index_sum(unknown_solid));


        CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(unknown_solid)) >= PartlyKnownSolid_attribute_index_sum(ice));
        CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(ice));
        CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(ice));
        CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(dummy_solid)) >= PartlyKnownSolid_attribute_index_sum(ice));


        CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(unknown_solid)) >= PartlyKnownSolid_attribute_index_sum(copper));
        CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(copper));
        CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(copper));
        CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(dummy_solid)) >= PartlyKnownSolid_attribute_index_sum(copper));


        CHECK(PartlyKnownSolid_attribute_index_sum(dummy_solid.value_or(unknown_solid)) >= PartlyKnownSolid_attribute_index_sum(dummy_solid));
        CHECK(PartlyKnownSolid_attribute_index_sum(dummy_solid.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(dummy_solid));
        CHECK(PartlyKnownSolid_attribute_index_sum(dummy_solid.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(dummy_solid));
        CHECK(PartlyKnownSolid_attribute_index_sum(dummy_solid.value_or(dummy_solid)) >= PartlyKnownSolid_attribute_index_sum(dummy_solid));

    }
}

