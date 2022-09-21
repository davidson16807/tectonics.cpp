
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "dynamic_viscosity.hpp"

TEST_CASE( "solid dynamic_viscosity order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::tables::ids::count; i++)
            {
                if (compound::tables::dynamic_viscosity_as_solid.count(i) > 0) {
                    auto x = compound::tables::dynamic_viscosity_as_solid[i](T);
                    CHECK(x / (si::pascal * si::second) <= 1e24); /*based on granite*/ \
                    CHECK(x / (si::pascal * si::second) > 1e10); /*based on ice*/ \
                }
            }
        }
    }
}
