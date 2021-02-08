
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "PartlyKnownSolid_to_CompletedSolid.hpp"

#include "PartlyKnownSolid_test_utils.hpp"
#include "CompletedSolid_test_utils.hpp"



TEST_CASE( "PartlyKnownSolid complete() purity", "[phase]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(compound::phase::complete(unknown,known_ice ) == compound::phase::complete(unknown,known_ice ));
        CHECK(compound::phase::complete(copper,known_ice) == compound::phase::complete(copper,known_ice));
        CHECK(compound::phase::complete(quartz,known_ice) == compound::phase::complete(quartz,known_ice));

        CHECK(compound::phase::complete(unknown,known_dummy ) == compound::phase::complete(unknown,known_dummy ));
        CHECK(compound::phase::complete(copper,known_dummy) == compound::phase::complete(copper,known_dummy));
        CHECK(compound::phase::complete(quartz,known_dummy) == compound::phase::complete(quartz,known_dummy));
    }
}

TEST_CASE( "PartlyKnownSolid complete() left identity", "[phase]" ) {
    SECTION("There exists a value that when applied as the left argument of the function returns the original value")
    {
        CHECK(compound::phase::complete(unknown,known_ice ) == known_ice);
        CHECK(compound::phase::complete(unknown,known_dummy ) == known_dummy);
    }
}

TEST_CASE( "PartlyKnownSolid complete() associativity", "[phase]" ) {
    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(compound::phase::complete(unknown, compound::phase::complete(copper,known_ice)) == 
              compound::phase::complete(compound::phase::complete(unknown,copper),known_ice));

        CHECK(compound::phase::complete(unknown, compound::phase::complete(copper,known_dummy)) == 
              compound::phase::complete(compound::phase::complete(unknown,copper),known_dummy));

        CHECK(compound::phase::complete(unknown, compound::phase::complete(quartz,known_ice)) == 
              compound::phase::complete(compound::phase::complete(unknown,quartz),known_ice));

        CHECK(compound::phase::complete(unknown, compound::phase::complete(quartz,known_dummy)) == 
              compound::phase::complete(compound::phase::complete(unknown,quartz),known_dummy));

        CHECK(compound::phase::complete(quartz, compound::phase::complete(copper,known_ice)) == 
              compound::phase::complete(compound::phase::complete(quartz,copper),known_ice));

        CHECK(compound::phase::complete(quartz, compound::phase::complete(copper,known_dummy)) == 
              compound::phase::complete(compound::phase::complete(quartz,copper),known_dummy));

        CHECK(compound::phase::complete(copper, compound::phase::complete(quartz,known_ice)) == 
              compound::phase::complete(compound::phase::complete(copper,quartz),known_ice));

        CHECK(compound::phase::complete(copper, compound::phase::complete(quartz,known_dummy)) == 
              compound::phase::complete(compound::phase::complete(copper,quartz),known_dummy));

    }
}

TEST_CASE( "PartlyKnownSolid value_or() increasing", "[phase]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(PartlyKnownSolid_attribute_index_sum(unknown.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(unknown));
        CHECK(PartlyKnownSolid_attribute_index_sum(unknown.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(unknown));
        CHECK(PartlyKnownSolid_attribute_index_sum(unknown.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(unknown));
        CHECK(PartlyKnownSolid_attribute_index_sum(unknown.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(unknown));


        CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(ice));
        CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(ice));
        CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(ice));
        CHECK(PartlyKnownSolid_attribute_index_sum(ice.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(ice));


        CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(copper));
        CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(copper));
        CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(copper));
        CHECK(PartlyKnownSolid_attribute_index_sum(copper.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(copper));


        CHECK(PartlyKnownSolid_attribute_index_sum(dummy.value_or(unknown)) >= PartlyKnownSolid_attribute_index_sum(dummy));
        CHECK(PartlyKnownSolid_attribute_index_sum(dummy.value_or(ice)) >= PartlyKnownSolid_attribute_index_sum(dummy));
        CHECK(PartlyKnownSolid_attribute_index_sum(dummy.value_or(copper)) >= PartlyKnownSolid_attribute_index_sum(dummy));
        CHECK(PartlyKnownSolid_attribute_index_sum(dummy.value_or(dummy)) >= PartlyKnownSolid_attribute_index_sum(dummy));

    }
}

