
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "PartlyKnownGas_to_CompletedGas.hpp"

#include "PartlyKnownGas_test_utils.hpp"
#include "CompletedGas_test_utils.hpp"


TEST_CASE( "PartlyKnownGas complete() purity", "[phase]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(compound::phase::complete(unknown_gas,known_nitrogen ) == compound::phase::complete(unknown_gas,known_nitrogen ));
        CHECK(compound::phase::complete(known_steam,known_nitrogen) == compound::phase::complete(known_steam,known_nitrogen));
        CHECK(compound::phase::complete(perflouromethane,known_nitrogen) == compound::phase::complete(perflouromethane,known_nitrogen));

        CHECK(compound::phase::complete(unknown_gas,known_dummy_gas ) == compound::phase::complete(unknown_gas,known_dummy_gas ));
        CHECK(compound::phase::complete(known_steam,known_dummy_gas) == compound::phase::complete(known_steam,known_dummy_gas));
        CHECK(compound::phase::complete(perflouromethane,known_dummy_gas) == compound::phase::complete(perflouromethane,known_dummy_gas));
    }
}

TEST_CASE( "PartlyKnownGas complete() left identity", "[phase]" ) {
    SECTION("There exists a value that when applied as the left argument of the function returns the original value")
    {
        CHECK(compound::phase::complete(unknown_gas,known_nitrogen ) == known_nitrogen);
        CHECK(compound::phase::complete(unknown_gas,known_dummy_gas ) == known_dummy_gas);
    }
}

TEST_CASE( "PartlyKnownGas complete() associativity", "[phase]" ) {
    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(compound::phase::complete(unknown_gas, compound::phase::complete(known_steam,known_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(unknown_gas,known_steam),known_nitrogen));

        CHECK(compound::phase::complete(unknown_gas, compound::phase::complete(known_steam,known_dummy_gas)) == 
              compound::phase::complete(compound::phase::complete(unknown_gas,known_steam),known_dummy_gas));

        CHECK(compound::phase::complete(unknown_gas, compound::phase::complete(perflouromethane,known_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(unknown_gas,perflouromethane),known_nitrogen));

        CHECK(compound::phase::complete(unknown_gas, compound::phase::complete(perflouromethane,known_dummy_gas)) == 
              compound::phase::complete(compound::phase::complete(unknown_gas,perflouromethane),known_dummy_gas));

        CHECK(compound::phase::complete(perflouromethane, compound::phase::complete(known_steam,known_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(perflouromethane,known_steam),known_nitrogen));

        CHECK(compound::phase::complete(perflouromethane, compound::phase::complete(known_steam,known_dummy_gas)) == 
              compound::phase::complete(compound::phase::complete(perflouromethane,known_steam),known_dummy_gas));

        CHECK(compound::phase::complete(known_steam, compound::phase::complete(perflouromethane,known_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(known_steam,perflouromethane),known_nitrogen));

        CHECK(compound::phase::complete(known_steam, compound::phase::complete(perflouromethane,known_dummy_gas)) == 
              compound::phase::complete(compound::phase::complete(known_steam,perflouromethane),known_dummy_gas));

    }
}
