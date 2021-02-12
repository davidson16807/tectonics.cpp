
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
    	CHECK(unknown_liquid.value_or(unknown_liquid) == unknown_liquid.value_or(unknown_liquid));
        CHECK(liquid_nitrogen.value_or(liquid_nitrogen) == liquid_nitrogen.value_or(liquid_nitrogen));
        CHECK(molten_silica.value_or(molten_silica) == molten_silica.value_or(molten_silica));
    	CHECK(dummy_liquid.value_or(dummy_liquid) == dummy_liquid.value_or(dummy_liquid));
    }
}
TEST_CASE( "PartlyKnownLiquid value_or() identity", "[phase]" ) {
	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(unknown_liquid.value_or(unknown_liquid) == unknown_liquid);
        CHECK(liquid_nitrogen.value_or(unknown_liquid) == liquid_nitrogen);
        CHECK(molten_silica.value_or(unknown_liquid) == molten_silica);
    	CHECK(dummy_liquid.value_or(unknown_liquid) == dummy_liquid);
    }
}

TEST_CASE( "PartlyKnownLiquid value_or() associativity", "[phase]" ) {
	SECTION("Functions can be applied in any order and still produce the same results")
	{
        CHECK(liquid_nitrogen.value_or(molten_silica.value_or(dummy_liquid)) == 
              liquid_nitrogen.value_or(molten_silica).value_or(dummy_liquid));

        CHECK(liquid_nitrogen.value_or(dummy_liquid.value_or(molten_silica)) == 
              liquid_nitrogen.value_or(dummy_liquid).value_or(molten_silica));

        
        CHECK(molten_silica.value_or(liquid_nitrogen.value_or(dummy_liquid)) == 
              molten_silica.value_or(liquid_nitrogen).value_or(dummy_liquid));

        CHECK(molten_silica.value_or(dummy_liquid.value_or(liquid_nitrogen)) == 
              molten_silica.value_or(dummy_liquid).value_or(liquid_nitrogen));

        
        CHECK(dummy_liquid.value_or(liquid_nitrogen.value_or(dummy_liquid)) == 
              dummy_liquid.value_or(liquid_nitrogen).value_or(dummy_liquid));

        CHECK(dummy_liquid.value_or(dummy_liquid.value_or(liquid_nitrogen)) == 
              dummy_liquid.value_or(dummy_liquid).value_or(liquid_nitrogen));
    }
}

TEST_CASE( "PartlyKnownLiquid value_or() increasing", "[phase]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(PartlyKnownLiquid_attribute_index_sum(unknown_liquid.value_or(unknown_liquid)) >= PartlyKnownLiquid_attribute_index_sum(unknown_liquid));
        CHECK(PartlyKnownLiquid_attribute_index_sum(unknown_liquid.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(unknown_liquid));
        CHECK(PartlyKnownLiquid_attribute_index_sum(unknown_liquid.value_or(liquid_ammonia)) >= PartlyKnownLiquid_attribute_index_sum(unknown_liquid));
        CHECK(PartlyKnownLiquid_attribute_index_sum(unknown_liquid.value_or(dummy_liquid)) >= PartlyKnownLiquid_attribute_index_sum(unknown_liquid));


        CHECK(PartlyKnownLiquid_attribute_index_sum(molten_silica.value_or(unknown_liquid)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));
        CHECK(PartlyKnownLiquid_attribute_index_sum(molten_silica.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));
        CHECK(PartlyKnownLiquid_attribute_index_sum(molten_silica.value_or(liquid_ammonia)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));
        CHECK(PartlyKnownLiquid_attribute_index_sum(molten_silica.value_or(dummy_liquid)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));


        CHECK(PartlyKnownLiquid_attribute_index_sum(liquid_ammonia.value_or(unknown_liquid)) >= PartlyKnownLiquid_attribute_index_sum(liquid_ammonia));
        CHECK(PartlyKnownLiquid_attribute_index_sum(liquid_ammonia.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(liquid_ammonia));
        CHECK(PartlyKnownLiquid_attribute_index_sum(liquid_ammonia.value_or(liquid_ammonia)) >= PartlyKnownLiquid_attribute_index_sum(liquid_ammonia));
        CHECK(PartlyKnownLiquid_attribute_index_sum(liquid_ammonia.value_or(dummy_liquid)) >= PartlyKnownLiquid_attribute_index_sum(liquid_ammonia));


        CHECK(PartlyKnownLiquid_attribute_index_sum(dummy_liquid.value_or(unknown_liquid)) >= PartlyKnownLiquid_attribute_index_sum(dummy_liquid));
        CHECK(PartlyKnownLiquid_attribute_index_sum(dummy_liquid.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(dummy_liquid));
        CHECK(PartlyKnownLiquid_attribute_index_sum(dummy_liquid.value_or(liquid_ammonia)) >= PartlyKnownLiquid_attribute_index_sum(dummy_liquid));
        CHECK(PartlyKnownLiquid_attribute_index_sum(dummy_liquid.value_or(dummy_liquid)) >= PartlyKnownLiquid_attribute_index_sum(dummy_liquid));

    }
}

