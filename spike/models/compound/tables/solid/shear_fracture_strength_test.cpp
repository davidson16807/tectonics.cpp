
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "shear_fracture_strength.hpp"

TEST_CASE( "solid shear_fracture_strength order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::tables::ids::count; i++)
            {
                if (compound::tables::shear_fracture_strength_as_solid.count(i) > 0) {
                    auto x = compound::tables::shear_fracture_strength_as_solid[i](T);
                    CHECK(x / si::megapascal < 10000.0); /*based on high performance steels*/ \
                    CHECK(x / si::megapascal > 0.1); /*based on hydrogen*/ \
                }
            }
        }
    }
}
