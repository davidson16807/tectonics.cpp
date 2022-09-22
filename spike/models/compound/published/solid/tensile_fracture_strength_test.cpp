
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "tensile_fracture_strength.hpp"

TEST_CASE( "published solid tensile_fracture_strength order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::ids::count; i++)
            {
                if (compound::published::tensile_fracture_strength_as_solid.count(i) > 0) {
                    auto x = compound::published::tensile_fracture_strength_as_solid[i](T);
                    CHECK(x / si::megapascal < 3000.0); /*based on titanium*/ \
                    CHECK(x / si::megapascal > 0.03); /*based on hydrogen*/ \
                }
            }
        }
    }
}
