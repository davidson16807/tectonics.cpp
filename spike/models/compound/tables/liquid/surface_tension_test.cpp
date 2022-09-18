
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "surface_tension.hpp"

TEST_CASE( "surface_tension order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::pressure<double> p = 1.0*si::pascal; p <= 100000000.0*si::pascal; p*=10.0)
        {
            for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
            {
                for (int i = 0; i<compound::tables::ids::count; i++)
                {
                    if (compound::tables::surface_tension_as_liquid.count(i) > 0) {
                        auto x = compound::tables::surface_tension_as_liquid[i](p,T);
                        CHECK(x / (si::millinewton/si::meter) < 3e3); /*based on molten copper */ \
                        CHECK(x / (si::millinewton/si::meter) > 0.0003); /*based on liquid carbon dioxide (Quinn 1927) */ \
                    }
                }
            }
        }
    }
}
