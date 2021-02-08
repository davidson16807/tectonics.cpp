
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "PartlyKnownLiquid_to_CompletedLiquid.hpp"

#include "PartlyKnownLiquid_test_utils.hpp"
#include "CompletedLiquid_test_utils.hpp"


TEST_CASE( "PartlyKnownLiquid complete() purity", "[phase]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(compound::phase::complete(unknown,known_liquid_nitrogen ) == compound::phase::complete(unknown,known_liquid_nitrogen ));
        CHECK(compound::phase::complete(liquid_ammonia,known_liquid_nitrogen) == compound::phase::complete(liquid_ammonia,known_liquid_nitrogen));
        CHECK(compound::phase::complete(molten_silica,known_liquid_nitrogen) == compound::phase::complete(molten_silica,known_liquid_nitrogen));

        CHECK(compound::phase::complete(unknown,known_dummy ) == compound::phase::complete(unknown,known_dummy ));
        CHECK(compound::phase::complete(liquid_ammonia,known_dummy) == compound::phase::complete(liquid_ammonia,known_dummy));
        CHECK(compound::phase::complete(molten_silica,known_dummy) == compound::phase::complete(molten_silica,known_dummy));
    }
}

TEST_CASE( "PartlyKnownLiquid complete() left identity", "[phase]" ) {
    SECTION("There exists a value that when applied as the left argument of the function returns the original value")
    {
        CHECK(compound::phase::complete(unknown,known_liquid_nitrogen ) == known_liquid_nitrogen);
        CHECK(compound::phase::complete(unknown,known_dummy ) == known_dummy);
    }
}

TEST_CASE( "PartlyKnownLiquid complete() associativity", "[phase]" ) {
    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(compound::phase::complete(unknown, compound::phase::complete(liquid_ammonia,known_liquid_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(unknown,liquid_ammonia),known_liquid_nitrogen));

        CHECK(compound::phase::complete(unknown, compound::phase::complete(liquid_ammonia,known_dummy)) == 
              compound::phase::complete(compound::phase::complete(unknown,liquid_ammonia),known_dummy));

        CHECK(compound::phase::complete(unknown, compound::phase::complete(molten_silica,known_liquid_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(unknown,molten_silica),known_liquid_nitrogen));

        CHECK(compound::phase::complete(unknown, compound::phase::complete(molten_silica,known_dummy)) == 
              compound::phase::complete(compound::phase::complete(unknown,molten_silica),known_dummy));

        CHECK(compound::phase::complete(molten_silica, compound::phase::complete(liquid_ammonia,known_liquid_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(molten_silica,liquid_ammonia),known_liquid_nitrogen));

        CHECK(compound::phase::complete(molten_silica, compound::phase::complete(liquid_ammonia,known_dummy)) == 
              compound::phase::complete(compound::phase::complete(molten_silica,liquid_ammonia),known_dummy));

        CHECK(compound::phase::complete(liquid_ammonia, compound::phase::complete(molten_silica,known_liquid_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(liquid_ammonia,molten_silica),known_liquid_nitrogen));

        CHECK(compound::phase::complete(liquid_ammonia, compound::phase::complete(molten_silica,known_dummy)) == 
              compound::phase::complete(compound::phase::complete(liquid_ammonia,molten_silica),known_dummy));

    }
}

TEST_CASE( "PartlyKnownLiquid value_or() increasing", "[phase]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(PartlyKnownLiquid_attribute_index_sum(unknown.value_or(unknown)) >= PartlyKnownLiquid_attribute_index_sum(unknown));
        CHECK(PartlyKnownLiquid_attribute_index_sum(unknown.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(unknown));
        CHECK(PartlyKnownLiquid_attribute_index_sum(unknown.value_or(liquid_ammonia)) >= PartlyKnownLiquid_attribute_index_sum(unknown));
        CHECK(PartlyKnownLiquid_attribute_index_sum(unknown.value_or(dummy)) >= PartlyKnownLiquid_attribute_index_sum(unknown));


        CHECK(PartlyKnownLiquid_attribute_index_sum(molten_silica.value_or(unknown)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));
        CHECK(PartlyKnownLiquid_attribute_index_sum(molten_silica.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));
        CHECK(PartlyKnownLiquid_attribute_index_sum(molten_silica.value_or(liquid_ammonia)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));
        CHECK(PartlyKnownLiquid_attribute_index_sum(molten_silica.value_or(dummy)) >= PartlyKnownLiquid_attribute_index_sum(molten_silica));


        CHECK(PartlyKnownLiquid_attribute_index_sum(liquid_ammonia.value_or(unknown)) >= PartlyKnownLiquid_attribute_index_sum(liquid_ammonia));
        CHECK(PartlyKnownLiquid_attribute_index_sum(liquid_ammonia.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(liquid_ammonia));
        CHECK(PartlyKnownLiquid_attribute_index_sum(liquid_ammonia.value_or(liquid_ammonia)) >= PartlyKnownLiquid_attribute_index_sum(liquid_ammonia));
        CHECK(PartlyKnownLiquid_attribute_index_sum(liquid_ammonia.value_or(dummy)) >= PartlyKnownLiquid_attribute_index_sum(liquid_ammonia));


        CHECK(PartlyKnownLiquid_attribute_index_sum(dummy.value_or(unknown)) >= PartlyKnownLiquid_attribute_index_sum(dummy));
        CHECK(PartlyKnownLiquid_attribute_index_sum(dummy.value_or(molten_silica)) >= PartlyKnownLiquid_attribute_index_sum(dummy));
        CHECK(PartlyKnownLiquid_attribute_index_sum(dummy.value_or(liquid_ammonia)) >= PartlyKnownLiquid_attribute_index_sum(dummy));
        CHECK(PartlyKnownLiquid_attribute_index_sum(dummy.value_or(dummy)) >= PartlyKnownLiquid_attribute_index_sum(dummy));

    }
}

