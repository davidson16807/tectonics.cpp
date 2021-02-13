
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
        CHECK(compound::water.value_or(compound::water) == compound::water.value_or(compound::water));
        CHECK(compound::nitrogen.value_or(compound::nitrogen) == compound::nitrogen.value_or(compound::nitrogen));
        CHECK(compound::nitrogen.value_or(compound::water) == compound::nitrogen.value_or(compound::water));
        CHECK(compound::water.value_or(compound::nitrogen) == compound::water.value_or(compound::nitrogen));
    }
}
TEST_CASE( "PartlyKnownCompound value_or() left identity", "[compound]" ) {
    SECTION("There exists a value that when applied as the first operand to a function returns the original value")
    {
        CHECK(compound::water.value_or(compound::water) == compound::water);
        CHECK(compound::water.value_or(compound::nitrogen) == compound::water);
    }
}
TEST_CASE( "PartlyKnownCompound value_or() right identity", "[compound]" ) {
    SECTION("There exists a value that when applied as the second operand to a function returns the original value")
    {
        CHECK(compound::water.value_or(unknown_hydrogen) == compound::water);
        CHECK(compound::nitrogen.value_or(unknown_hydrogen) == compound::nitrogen);
    }
}
TEST_CASE( "PartlyKnownCompound value_or() associativity", "[compound]" ) {
    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(compound::nitrogen.value_or(compound::water.value_or(compound::oxygen)) == 
              compound::nitrogen.value_or(compound::water).value_or(compound::oxygen));

        CHECK(compound::nitrogen.value_or(compound::oxygen.value_or(compound::water)) == 
              compound::nitrogen.value_or(compound::oxygen).value_or(compound::water));

        
        CHECK(compound::water.value_or(compound::nitrogen.value_or(compound::oxygen)) == 
              compound::water.value_or(compound::nitrogen).value_or(compound::oxygen));

        CHECK(compound::water.value_or(compound::oxygen.value_or(compound::nitrogen)) == 
              compound::water.value_or(compound::oxygen).value_or(compound::nitrogen));

        
        CHECK(compound::oxygen.value_or(compound::nitrogen.value_or(compound::oxygen)) == 
              compound::oxygen.value_or(compound::nitrogen).value_or(compound::oxygen));

        CHECK(compound::oxygen.value_or(compound::oxygen.value_or(compound::nitrogen)) == 
              compound::oxygen.value_or(compound::oxygen).value_or(compound::nitrogen));
    }
}

/*
TEST_CASE( "PartlyKnownCompound value_or() increasing", "[compound]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(PartlyKnownCompound_attribute_index_sum(compound::water.value_or(compound::water)) >= PartlyKnownCompound_attribute_index_sum(compound::water));
        CHECK(PartlyKnownCompound_attribute_index_sum(compound::water.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(compound::water));
        CHECK(PartlyKnownCompound_attribute_index_sum(compound::water.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(compound::water));
        CHECK(PartlyKnownCompound_attribute_index_sum(compound::water.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(compound::water));


        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(compound::water)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));
        CHECK(PartlyKnownCompound_attribute_index_sum(perflouromethane.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(perflouromethane));


        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(compound::water)) >= PartlyKnownCompound_attribute_index_sum(known_steam));
        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(known_steam));
        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(known_steam));
        CHECK(PartlyKnownCompound_attribute_index_sum(known_steam.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(known_steam));


        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(compound::water)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(perflouromethane)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(known_steam)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));
        CHECK(PartlyKnownCompound_attribute_index_sum(dummy_gas.value_or(dummy_gas)) >= PartlyKnownCompound_attribute_index_sum(dummy_gas));

    }
}
*/
