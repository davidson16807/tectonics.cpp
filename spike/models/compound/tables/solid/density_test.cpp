
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "density.hpp"

TEST_CASE( "density order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::tables::ids::count; i++)
            {
                if (compound::tables::density_as_solid.count(i) > 0) {
                    auto x = compound::tables::density_as_solid[i](T);
                    CHECK(x / (si::kilogram / si::meter3) < 30000.0); /*based on iridium*/ \
                    CHECK(x / (si::kilogram / si::meter3) > 1.5); /*based on aerogel*/ \
                }
            }
        }
    }
}
