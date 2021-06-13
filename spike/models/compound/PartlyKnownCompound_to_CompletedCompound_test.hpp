
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "PartlyKnownCompound_to_CompletedCompound.hpp"

#include "PartlyKnownCompound_test_utils.hpp"
#include "CompletedCompound_test_utils.hpp"

#include "PartlyKnownCompound_library.hpp"
#include "CompletedCompound_library.hpp"

/*
TEST_CASE( "PartlyKnownCompound complete() purity", "[compound]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(compound::complete(compound::unknown_hydrogen, compound::water ) == compound::complete(compound::unknown_hydrogen, compound::water ));
    }
}

TEST_CASE( "PartlyKnownCompound complete() left identity", "[compound]" ) {
    SECTION("There exists a value that when applied as the left argument of the function returns the original value")
    {
        CHECK(compound::complete(compound::unknown_hydrogen, compound::water ) ==  compound::water);
    }
}
*/

/*
TEST_CASE( "PartlyKnownCompound complete() associativity", "[compound]" ) {
    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(compound::complete(unknown_hydrogen, compound::complete(known_steam,known_nitrogen)) == 
              compound::complete(compound::complete(unknown_hydrogen,known_steam),known_nitrogen));

        CHECK(compound::complete(unknown_hydrogen, compound::complete(known_steam,known_dummy_gas)) == 
              compound::complete(compound::complete(unknown_hydrogen,known_steam),known_dummy_gas));

        CHECK(compound::complete(unknown_hydrogen, compound::complete(perflouromethane_gas,known_nitrogen)) == 
              compound::complete(compound::complete(unknown_hydrogen,perflouromethane_gas),known_nitrogen));

        CHECK(compound::complete(unknown_hydrogen, compound::complete(perflouromethane_gas,known_dummy_gas)) == 
              compound::complete(compound::complete(unknown_hydrogen,perflouromethane_gas),known_dummy_gas));

        CHECK(compound::complete(perflouromethane_gas, compound::complete(known_steam,known_nitrogen)) == 
              compound::complete(compound::complete(perflouromethane_gas,known_steam),known_nitrogen));

        CHECK(compound::complete(perflouromethane_gas, compound::complete(known_steam,known_dummy_gas)) == 
              compound::complete(compound::complete(perflouromethane_gas,known_steam),known_dummy_gas));

        CHECK(compound::complete(known_steam, compound::complete(perflouromethane_gas,known_nitrogen)) == 
              compound::complete(compound::complete(known_steam,perflouromethane_gas),known_nitrogen));

        CHECK(compound::complete(known_steam, compound::complete(perflouromethane_gas,known_dummy_gas)) == 
              compound::complete(compound::complete(known_steam,perflouromethane_gas),known_dummy_gas));

    }
}
*/