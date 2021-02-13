
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "PartlyKnownCompound_library.hpp"

// test utility libraries
#include "PartlyKnownCompound_test_utils.hpp"

TEST_CASE( "PartlyKnownCompound value_or() purity", "[compound]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(water.value_or(water) == water.value_or(water));
        CHECK(nitrogen.value_or(nitrogen) == nitrogen.value_or(nitrogen));
        CHECK(nitrogen.value_or(water) == nitrogen.value_or(water));
        CHECK(water.value_or(nitrogen) == water.value_or(nitrogen));
    }
}
TEST_CASE( "PartlyKnownCompound value_or() left identity", "[compound]" ) {
    SECTION("There exists a value that when applied as the first operand to a function returns the original value")
    {
        CHECK(water.value_or(water) == water);
        CHECK(water.value_or(nitrogen) == water);
    }
}
TEST_CASE( "PartlyKnownCompound value_or() right identity", "[compound]" ) {
    SECTION("There exists a value that when applied as the second operand to a function returns the original value")
    {
        CHECK(water.value_or(unknown_hydrogen) == water);
        CHECK(nitrogen.value_or(unknown_hydrogen) == nitrogen);
    }
}
TEST_CASE( "PartlyKnownCompound value_or() associativity", "[compound]" ) {
    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(nitrogen.value_or(water.value_or(oxygen)) == 
              nitrogen.value_or(water).value_or(oxygen));

        CHECK(nitrogen.value_or(oxygen.value_or(water)) == 
              nitrogen.value_or(oxygen).value_or(water));

        
        CHECK(water.value_or(nitrogen.value_or(oxygen)) == 
              water.value_or(nitrogen).value_or(oxygen));

        CHECK(water.value_or(oxygen.value_or(nitrogen)) == 
              water.value_or(oxygen).value_or(nitrogen));

        
        CHECK(oxygen.value_or(nitrogen.value_or(oxygen)) == 
              oxygen.value_or(nitrogen).value_or(oxygen));

        CHECK(oxygen.value_or(oxygen.value_or(nitrogen)) == 
              oxygen.value_or(oxygen).value_or(nitrogen));
    }
}

/*
TEST_CASE( "PartlyKnownCompound value_or() increasing", "[compound]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(PartlyKnownCompound_attribute_index_sum(water.value_or(water)) >= PartlyKnownCompound_attribute_index_sum(water));
        CHECK(PartlyKnownCompound_attribute_index_sum(water.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(water));
        CHECK(PartlyKnownCompound_attribute_index_sum(water.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(water));
        CHECK(PartlyKnownCompound_attribute_index_sum(water.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(water));


        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(water)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));


        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(water)) >= PartlyKnownCompound_attribute_index_sum(known_steam));
        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(known_steam));
        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(known_steam));
        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(known_steam));


        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(water)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));

    }
}
*/
