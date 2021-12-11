
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "PartlyKnownSolid.hpp"
#include "PartlyKnownSolid_operators.hpp"

#include "PartlyKnownSolid_test_utils.hpp"

TEST_CASE( "PartlyKnownSolid infer() purity", "[phase]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::phase::infer(unknown_solid) == compound::phase::infer(unknown_solid));
        CHECK(compound::phase::infer(ice) == compound::phase::infer(ice));
        CHECK(compound::phase::infer(copper) == compound::phase::infer(copper));
    	CHECK(compound::phase::infer(dummy_solid) == compound::phase::infer(dummy_solid));
    }
}
TEST_CASE( "PartlyKnownSolid infer() identity", "[phase]" ) {
	SECTION("There exists a value that when applied to a function returns the original value")
	{
        CHECK(compound::phase::infer(unknown_solid) == unknown_solid);
        CHECK(compound::phase::infer(ice) == ice);CHECK(compound::phase::infer(ice).isobaric_specific_heat_capacity == ice.isobaric_specific_heat_capacity );
    }
}

TEST_CASE( "PartlyKnownSolid infer() increasing", "[phase]" ) {
    SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
    {

        CHECK(PartlyKnownSolid_attribute_known_count(compound::phase::infer(unknown_solid)) >= PartlyKnownSolid_attribute_known_count(unknown_solid));
        CHECK(PartlyKnownSolid_attribute_known_count(compound::phase::infer(ice)) >= PartlyKnownSolid_attribute_known_count(ice));
        CHECK(PartlyKnownSolid_attribute_known_count(compound::phase::infer(copper)) >= PartlyKnownSolid_attribute_known_count(copper));
        CHECK(PartlyKnownSolid_attribute_known_count(compound::phase::infer(dummy_solid)) >= PartlyKnownSolid_attribute_known_count(dummy_solid));

    }
}
