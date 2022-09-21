
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "compressive_yield_strength.hpp"

TEST_CASE( "solid compressive_yield_strength order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::tables::ids::count; i++)
            {
                if (compound::tables::compressive_yield_strength_as_solid.count(i) > 0) {
                    auto x = compound::tables::compressive_yield_strength_as_solid[i](T);
                    CHECK(x / si::megapascal < 3000.0); /*based on ceramics*/ \
                    CHECK(x / si::megapascal > 1.0); /*based on hydrogen*/ \
                }
            }
        }
    }
}
