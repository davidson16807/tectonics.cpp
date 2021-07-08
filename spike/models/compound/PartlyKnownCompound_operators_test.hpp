
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "published.hpp"
#include "PartlyKnownCompound_operators.hpp"

// test utility libraries
#include "PartlyKnownCompound_test_utils.hpp"

TEST_CASE( "PartlyKnownCompound infer() purity", "[compound]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(compound::infer(compound::test_water) == compound::infer(compound::test_water));
        CHECK(compound::infer(compound::published::nitrogen) == compound::infer(compound::published::nitrogen));
        CHECK(compound::infer(compound::published::benzene) == compound::infer(compound::published::benzene));
        CHECK(compound::infer(compound::published::quartz) == compound::infer(compound::published::quartz));
    }
}
TEST_CASE( "PartlyKnownCompound infer() idempotence", "[compound]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(compound::infer(compound::test_water) == compound::infer(compound::infer(compound::test_water)));
        CHECK(compound::infer(compound::published::nitrogen) == compound::infer(compound::infer(compound::published::nitrogen)));
        CHECK(compound::infer(compound::published::benzene) == compound::infer(compound::infer(compound::published::benzene)));
        CHECK(compound::infer(compound::published::quartz) == compound::infer(compound::infer(compound::published::quartz)));
        // CHECK(compound::to_string(compound::speculate(compound::infer(compound::published::quartz))) == compound::to_string(compound::published::quartz));
        // CHECK(compound::to_string(compound::infer(compound::published::nitrogen)) == compound::to_string(compound::published::nitrogen));
    }
}
TEST_CASE( "PartlyKnownCompound infer() increasing", "[compound]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(compound::PartlyKnownCompound_attribute_index_sum(compound::infer(compound::test_water)) >= compound::PartlyKnownCompound_attribute_index_sum(compound::test_water));
        CHECK(compound::PartlyKnownCompound_attribute_index_sum(compound::infer(compound::published::nitrogen)) >= compound::PartlyKnownCompound_attribute_index_sum(compound::published::nitrogen));
        CHECK(compound::PartlyKnownCompound_attribute_index_sum(compound::infer(compound::published::benzene)) >= compound::PartlyKnownCompound_attribute_index_sum(compound::published::benzene));
        CHECK(compound::PartlyKnownCompound_attribute_index_sum(compound::infer(compound::published::quartz)) >= compound::PartlyKnownCompound_attribute_index_sum(compound::published::quartz));
    }
}