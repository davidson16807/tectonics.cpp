
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <units/si_test_utils.hpp>

#include "PartlyKnownGas.hpp"

#include "PartlyKnownGas_test_utils.hpp"

TEST_CASE( "PartlyKnownGas value_or() purity", "[phase]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.value_or(unknown) == unknown.value_or(unknown));
        CHECK(nitrogen.value_or(nitrogen) == nitrogen.value_or(nitrogen));
        CHECK(steam.value_or(steam) == steam.value_or(steam));
    	CHECK(dummy.value_or(dummy) == dummy.value_or(dummy));
    }
}
TEST_CASE( "PartlyKnownGas value_or() identity", "[phase]" ) {
	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(unknown.value_or(unknown) == unknown);
        CHECK(nitrogen.value_or(unknown) == nitrogen);
        CHECK(steam.value_or(unknown) == steam);
    	CHECK(dummy.value_or(unknown) == dummy);
    }
}

TEST_CASE( "PartlyKnownGas value_or() associativity", "[phase]" ) {
	SECTION("Functions can be applied in any order and still produce the same results")
	{
        CHECK(nitrogen.value_or(steam.value_or(dummy)) == 
              nitrogen.value_or(steam).value_or(dummy));

        CHECK(nitrogen.value_or(dummy.value_or(steam)) == 
              nitrogen.value_or(dummy).value_or(steam));

        
        CHECK(steam.value_or(nitrogen.value_or(dummy)) == 
              steam.value_or(nitrogen).value_or(dummy));

        CHECK(steam.value_or(dummy.value_or(nitrogen)) == 
              steam.value_or(dummy).value_or(nitrogen));

        
        CHECK(dummy.value_or(nitrogen.value_or(dummy)) == 
              dummy.value_or(nitrogen).value_or(dummy));

        CHECK(dummy.value_or(dummy.value_or(nitrogen)) == 
              dummy.value_or(dummy).value_or(nitrogen));
    }
}

TEST_CASE( "PartlyKnownGas value_or() increasing", "[phase]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(PartlyKnownGas_attribute_index_sum(unknown.value_or(unknown)) >= PartlyKnownGas_attribute_index_sum(unknown));
        CHECK(PartlyKnownGas_attribute_index_sum(unknown.value_or(perflouromethane)) >= PartlyKnownGas_attribute_index_sum(unknown));
        CHECK(PartlyKnownGas_attribute_index_sum(unknown.value_or(known_steam)) >= PartlyKnownGas_attribute_index_sum(unknown));
        CHECK(PartlyKnownGas_attribute_index_sum(unknown.value_or(dummy)) >= PartlyKnownGas_attribute_index_sum(unknown));


        CHECK(PartlyKnownGas_attribute_index_sum(perflouromethane.value_or(unknown)) >= PartlyKnownGas_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownGas_attribute_index_sum(perflouromethane.value_or(perflouromethane)) >= PartlyKnownGas_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownGas_attribute_index_sum(perflouromethane.value_or(known_steam)) >= PartlyKnownGas_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownGas_attribute_index_sum(perflouromethane.value_or(dummy)) >= PartlyKnownGas_attribute_index_sum(perflouromethane));


        CHECK(PartlyKnownGas_attribute_index_sum(known_steam.value_or(unknown)) >= PartlyKnownGas_attribute_index_sum(known_steam));
        CHECK(PartlyKnownGas_attribute_index_sum(known_steam.value_or(perflouromethane)) >= PartlyKnownGas_attribute_index_sum(known_steam));
        CHECK(PartlyKnownGas_attribute_index_sum(known_steam.value_or(known_steam)) >= PartlyKnownGas_attribute_index_sum(known_steam));
        CHECK(PartlyKnownGas_attribute_index_sum(known_steam.value_or(dummy)) >= PartlyKnownGas_attribute_index_sum(known_steam));


        CHECK(PartlyKnownGas_attribute_index_sum(dummy.value_or(unknown)) >= PartlyKnownGas_attribute_index_sum(dummy));
        CHECK(PartlyKnownGas_attribute_index_sum(dummy.value_or(perflouromethane)) >= PartlyKnownGas_attribute_index_sum(dummy));
        CHECK(PartlyKnownGas_attribute_index_sum(dummy.value_or(known_steam)) >= PartlyKnownGas_attribute_index_sum(dummy));
        CHECK(PartlyKnownGas_attribute_index_sum(dummy.value_or(dummy)) >= PartlyKnownGas_attribute_index_sum(dummy));

    }
}

