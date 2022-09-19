
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "pwave_modulus.hpp"

TEST_CASE( "pwave_modulus order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
        {
            for (int i = 0; i<compound::tables::ids::count; i++)
            {
                if (compound::tables::pwave_modulus_as_solid.count(i) > 0) {
                    auto x = compound::tables::pwave_modulus_as_solid[i](T);
                    CHECK(x / si::gigapascal < 30.0); /*based on water*/ \
                    CHECK(x / si::gigapascal > 10.0); /*based on water*/ \
                }
            }
        }
    }
}
