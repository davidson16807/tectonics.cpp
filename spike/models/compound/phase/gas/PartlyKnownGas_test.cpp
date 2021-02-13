
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
    	CHECK(unknown_gas.value_or(unknown_gas) == unknown_gas.value_or(unknown_gas));
        CHECK(nitrogen_gas.value_or(nitrogen_gas) == nitrogen_gas.value_or(nitrogen_gas));
        CHECK(steam.value_or(steam) == steam.value_or(steam));
    	CHECK(dummy_gas.value_or(dummy_gas) == dummy_gas.value_or(dummy_gas));
    }
}
TEST_CASE( "PartlyKnownGas value_or() identity", "[phase]" ) {
	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(unknown_gas.value_or(unknown_gas) == unknown_gas);
        CHECK(nitrogen_gas.value_or(unknown_gas) == nitrogen_gas);
        CHECK(steam.value_or(unknown_gas) == steam);
    	CHECK(dummy_gas.value_or(unknown_gas) == dummy_gas);
    }
}

TEST_CASE( "PartlyKnownGas value_or() associativity", "[phase]" ) {
	SECTION("Functions can be applied in any order and still produce the same results")
	{
        CHECK(nitrogen_gas.value_or(steam.value_or(dummy_gas)) == 
              nitrogen_gas.value_or(steam).value_or(dummy_gas));

        CHECK(nitrogen_gas.value_or(dummy_gas.value_or(steam)) == 
              nitrogen_gas.value_or(dummy_gas).value_or(steam));

        
        CHECK(steam.value_or(nitrogen_gas.value_or(dummy_gas)) == 
              steam.value_or(nitrogen_gas).value_or(dummy_gas));

        CHECK(steam.value_or(dummy_gas.value_or(nitrogen_gas)) == 
              steam.value_or(dummy_gas).value_or(nitrogen_gas));

        
        CHECK(dummy_gas.value_or(nitrogen_gas.value_or(dummy_gas)) == 
              dummy_gas.value_or(nitrogen_gas).value_or(dummy_gas));

        CHECK(dummy_gas.value_or(dummy_gas.value_or(nitrogen_gas)) == 
              dummy_gas.value_or(dummy_gas).value_or(nitrogen_gas));
    }
}

TEST_CASE( "PartlyKnownGas value_or() increasing", "[phase]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(PartlyKnownGas_attribute_index_sum(unknown_gas.value_or(unknown_gas)) >= PartlyKnownGas_attribute_index_sum(unknown_gas));
        CHECK(PartlyKnownGas_attribute_index_sum(unknown_gas.value_or(perflouromethane)) >= PartlyKnownGas_attribute_index_sum(unknown_gas));
        CHECK(PartlyKnownGas_attribute_index_sum(unknown_gas.value_or(known_steam)) >= PartlyKnownGas_attribute_index_sum(unknown_gas));
        CHECK(PartlyKnownGas_attribute_index_sum(unknown_gas.value_or(dummy_gas)) >= PartlyKnownGas_attribute_index_sum(unknown_gas));


        CHECK(PartlyKnownGas_attribute_index_sum(perflouromethane.value_or(unknown_gas)) >= PartlyKnownGas_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownGas_attribute_index_sum(perflouromethane.value_or(perflouromethane)) >= PartlyKnownGas_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownGas_attribute_index_sum(perflouromethane.value_or(known_steam)) >= PartlyKnownGas_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownGas_attribute_index_sum(perflouromethane.value_or(dummy_gas)) >= PartlyKnownGas_attribute_index_sum(perflouromethane));


        CHECK(PartlyKnownGas_attribute_index_sum(known_steam.value_or(unknown_gas)) >= PartlyKnownGas_attribute_index_sum(known_steam));
        CHECK(PartlyKnownGas_attribute_index_sum(known_steam.value_or(perflouromethane)) >= PartlyKnownGas_attribute_index_sum(known_steam));
        CHECK(PartlyKnownGas_attribute_index_sum(known_steam.value_or(known_steam)) >= PartlyKnownGas_attribute_index_sum(known_steam));
        CHECK(PartlyKnownGas_attribute_index_sum(known_steam.value_or(dummy_gas)) >= PartlyKnownGas_attribute_index_sum(known_steam));


        CHECK(PartlyKnownGas_attribute_index_sum(dummy_gas.value_or(unknown_gas)) >= PartlyKnownGas_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownGas_attribute_index_sum(dummy_gas.value_or(perflouromethane)) >= PartlyKnownGas_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownGas_attribute_index_sum(dummy_gas.value_or(known_steam)) >= PartlyKnownGas_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownGas_attribute_index_sum(dummy_gas.value_or(dummy_gas)) >= PartlyKnownGas_attribute_index_sum(dummy_gas));

    }
}

