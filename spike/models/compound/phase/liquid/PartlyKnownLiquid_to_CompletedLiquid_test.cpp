
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
        CHECK(compound::phase::complete(unknown_liquid,known_liquid_nitrogen ) == compound::phase::complete(unknown_liquid,known_liquid_nitrogen ));
        CHECK(compound::phase::complete(liquid_ammonia,known_liquid_nitrogen) == compound::phase::complete(liquid_ammonia,known_liquid_nitrogen));
        CHECK(compound::phase::complete(molten_silica,known_liquid_nitrogen) == compound::phase::complete(molten_silica,known_liquid_nitrogen));

        CHECK(compound::phase::complete(unknown_liquid,known_dummy_liquid ) == compound::phase::complete(unknown_liquid,known_dummy_liquid ));
        CHECK(compound::phase::complete(liquid_ammonia,known_dummy_liquid) == compound::phase::complete(liquid_ammonia,known_dummy_liquid));
        CHECK(compound::phase::complete(molten_silica,known_dummy_liquid) == compound::phase::complete(molten_silica,known_dummy_liquid));
    }
}

TEST_CASE( "PartlyKnownLiquid complete() left identity", "[phase]" ) {
    SECTION("There exists a value that when applied as the left argument of the function returns the original value")
    {
        CHECK(compound::phase::complete(unknown_liquid,known_liquid_nitrogen ) == known_liquid_nitrogen);
        CHECK(compound::phase::complete(unknown_liquid,known_dummy_liquid ) == known_dummy_liquid);
    }
}

TEST_CASE( "PartlyKnownLiquid complete() associativity", "[phase]" ) {
    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(compound::phase::complete(unknown_liquid, compound::phase::complete(liquid_ammonia,known_liquid_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(unknown_liquid,liquid_ammonia),known_liquid_nitrogen));

        CHECK(compound::phase::complete(unknown_liquid, compound::phase::complete(liquid_ammonia,known_dummy_liquid)) == 
              compound::phase::complete(compound::phase::complete(unknown_liquid,liquid_ammonia),known_dummy_liquid));

        CHECK(compound::phase::complete(unknown_liquid, compound::phase::complete(molten_silica,known_liquid_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(unknown_liquid,molten_silica),known_liquid_nitrogen));

        CHECK(compound::phase::complete(unknown_liquid, compound::phase::complete(molten_silica,known_dummy_liquid)) == 
              compound::phase::complete(compound::phase::complete(unknown_liquid,molten_silica),known_dummy_liquid));

        CHECK(compound::phase::complete(molten_silica, compound::phase::complete(liquid_ammonia,known_liquid_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(molten_silica,liquid_ammonia),known_liquid_nitrogen));

        CHECK(compound::phase::complete(molten_silica, compound::phase::complete(liquid_ammonia,known_dummy_liquid)) == 
              compound::phase::complete(compound::phase::complete(molten_silica,liquid_ammonia),known_dummy_liquid));

        CHECK(compound::phase::complete(liquid_ammonia, compound::phase::complete(molten_silica,known_liquid_nitrogen)) == 
              compound::phase::complete(compound::phase::complete(liquid_ammonia,molten_silica),known_liquid_nitrogen));

        CHECK(compound::phase::complete(liquid_ammonia, compound::phase::complete(molten_silica,known_dummy_liquid)) == 
              compound::phase::complete(compound::phase::complete(liquid_ammonia,molten_silica),known_dummy_liquid));

    }
}
