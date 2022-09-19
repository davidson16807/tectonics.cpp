
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "shear_yield_strength.hpp"

TEST_CASE( "shear_yield_strength order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::pressure<double> p = 1.0*si::pascal; p <= 100000000.0*si::pascal; p*=10.0)
        {
            for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
            {
                for (int i = 0; i<compound::tables::ids::count; i++)
                {
                    if (compound::tables::shear_yield_strength_as_solid.count(i) > 0) {
                        auto x = compound::tables::shear_yield_strength_as_solid[i](p,T);
                        CHECK(x / si::megapascal < 10000.0); /*based on high performance steels*/ \
                        CHECK(x / si::megapascal > 0.003); /*based on hydrogen*/ \
                    }
                }
            }
        }
    }
}
