
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
    const float float_precision(1e-7);
    CHECK(compound::phase::distance(compound::phase::complete(unknown_gas,known_nitrogen ), 
                                        compound::phase::complete(unknown_gas,known_nitrogen ))
            < float_precision);
    CHECK(compound::phase::distance(compound::phase::complete(steam,known_nitrogen), 
                                        compound::phase::complete(steam,known_nitrogen))
            < float_precision);
    CHECK(compound::phase::distance(compound::phase::complete(perflouromethane_gas,known_nitrogen), 
                                        compound::phase::complete(perflouromethane_gas,known_nitrogen))
            < float_precision);

    CHECK(compound::phase::distance(compound::phase::complete(unknown_gas,known_dummy_gas ), 
                                        compound::phase::complete(unknown_gas,known_dummy_gas ))
            < float_precision);
    CHECK(compound::phase::distance(compound::phase::complete(steam,known_dummy_gas), 
                                        compound::phase::complete(steam,known_dummy_gas))
            < float_precision);
    CHECK(compound::phase::distance(compound::phase::complete(perflouromethane_gas,known_dummy_gas), 
                                        compound::phase::complete(perflouromethane_gas,known_dummy_gas))
            < float_precision);
  }
}

TEST_CASE( "PartlyKnownGas complete() left identity", "[phase]" ) {
  SECTION("There exists a value that when applied as the left argument of the function returns the original value")
  {
    const float float_precision(1e-7);
    CHECK(compound::phase::distance(compound::phase::complete(unknown_gas,known_nitrogen ), 
                                        known_nitrogen)
            < float_precision);
    CHECK(compound::phase::distance(compound::phase::complete(unknown_gas,known_dummy_gas ), 
                                        known_dummy_gas)
            < float_precision);
  }
}

/*
TEST_CASE( "PartlyKnownGas complete() associativity", "[phase]" ) {
    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(compound::phase::distance(compound::phase::complete(unknown_gas, compound::phase::complete(known_steam,known_nitrogen)),
                                            compound::phase::complete(compound::phase::complete(unknown_gas,known_steam),known_nitrogen))
                < float_precision);

        CHECK(compound::phase::distance(compound::phase::complete(unknown_gas, compound::phase::complete(known_steam,known_dummy_gas)),
                                            compound::phase::complete(compound::phase::complete(unknown_gas,known_steam),known_dummy_gas))
                < float_precision);

        CHECK(compound::phase::distance(compound::phase::complete(unknown_gas, compound::phase::complete(perflouromethane_gas,known_nitrogen)),
                                            compound::phase::complete(compound::phase::complete(unknown_gas,perflouromethane_gas),known_nitrogen))
                < float_precision);

        CHECK(compound::phase::distance(compound::phase::complete(unknown_gas, compound::phase::complete(perflouromethane_gas,known_dummy_gas)),
                                            compound::phase::complete(compound::phase::complete(unknown_gas,perflouromethane_gas),known_dummy_gas))
                < float_precision);

        CHECK(compound::phase::distance(compound::phase::complete(perflouromethane_gas, compound::phase::complete(known_steam,known_nitrogen)),
                                            compound::phase::complete(compound::phase::complete(perflouromethane_gas,known_steam),known_nitrogen))
                < float_precision);

        CHECK(compound::phase::distance(compound::phase::complete(perflouromethane_gas, compound::phase::complete(known_steam,known_dummy_gas)),
                                            compound::phase::complete(compound::phase::complete(perflouromethane_gas,known_steam),known_dummy_gas))
                < float_precision);

        CHECK(compound::phase::distance(compound::phase::complete(known_steam, compound::phase::complete(perflouromethane_gas,known_nitrogen)),
                                            compound::phase::complete(compound::phase::complete(known_steam,perflouromethane_gas),known_nitrogen))
                < float_precision);

        CHECK(compound::phase::distance(compound::phase::complete(known_steam, compound::phase::complete(perflouromethane_gas,known_dummy_gas)),
                                            compound::phase::complete(compound::phase::complete(known_steam,perflouromethane_gas),known_dummy_gas))
                < float_precision);

    }
}
*/