
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "density.hpp"

TEST_CASE( "published liquid density order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::compounds::count; i++)
        {
            for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
            {
                if (compound::published::density_as_liquid.has(i)) {
                    auto x = compound::published::density_as_liquid[i](T);
                    CHECK(x / (si::gram / si::centimeter3) < 20.0); /*based on gold*/ \
                    CHECK(x / (si::gram / si::centimeter3) > 0.01); /*based on mercury*/ \
                }
            }
        }
    }
}
