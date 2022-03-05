
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <units/si_test_utils.hpp>


#include "PartlyKnownGas.hpp"

#include "PartlyKnownGas_test_utils.hpp"
#include "PartlyKnownGas_to_string.hpp"

TEST_CASE( "PartlyKnownGas value_or() purity", "[phase]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    const float float_precision(1e-7);
  	CHECK(compound::phase::distance(unknown_gas.value_or(unknown_gas), unknown_gas.value_or(unknown_gas))
            < float_precision);
    CHECK(compound::phase::distance(nitrogen_gas.value_or(nitrogen_gas), nitrogen_gas.value_or(nitrogen_gas))
            < float_precision);
    CHECK(compound::phase::distance(steam.value_or(steam), steam.value_or(steam))
            < float_precision);
  	CHECK(compound::phase::distance(dummy_gas.value_or(dummy_gas), dummy_gas.value_or(dummy_gas))
            < float_precision);
  }
}
TEST_CASE( "PartlyKnownGas value_or() identity", "[phase]" ) {
	SECTION("There exists a value that when applied to a function returns the original value")
	{
    const float float_precision(1e-7);
  	CHECK(compound::phase::distance(unknown_gas.value_or(unknown_gas), unknown_gas)
            < float_precision);
    CHECK(compound::phase::distance(nitrogen_gas.value_or(unknown_gas), nitrogen_gas)
            < float_precision);
    CHECK(compound::phase::distance(steam.value_or(unknown_gas), steam)
            < float_precision);
  	CHECK(compound::phase::distance(dummy_gas.value_or(unknown_gas), dummy_gas)
            < float_precision);
  }
}

TEST_CASE( "PartlyKnownGas value_or() associativity", "[phase]" ) {
	SECTION("Functions can be applied in any order and still produce the same results")
	{
    const float float_precision(1e-7);
    CHECK(compound::phase::distance(nitrogen_gas.value_or(steam.value_or(dummy_gas)), 
                                        nitrogen_gas.value_or(steam).value_or(dummy_gas))
            < float_precision);

    CHECK(compound::phase::distance(nitrogen_gas.value_or(dummy_gas.value_or(steam)), 
                                        nitrogen_gas.value_or(dummy_gas).value_or(steam))
            < float_precision);

    
    CHECK(compound::phase::distance(steam.value_or(nitrogen_gas.value_or(dummy_gas)), 
                                        steam.value_or(nitrogen_gas).value_or(dummy_gas))
            < float_precision);

    CHECK(compound::phase::distance(steam.value_or(dummy_gas.value_or(nitrogen_gas)), 
                                        steam.value_or(dummy_gas).value_or(nitrogen_gas))
            < float_precision);

    
    CHECK(compound::phase::distance(dummy_gas.value_or(nitrogen_gas.value_or(dummy_gas)), 
                                        dummy_gas.value_or(nitrogen_gas).value_or(dummy_gas))
            < float_precision);

    CHECK(compound::phase::distance(dummy_gas.value_or(dummy_gas.value_or(nitrogen_gas)), 
                                          dummy_gas.value_or(dummy_gas).value_or(nitrogen_gas))
            < float_precision);
  }
}
