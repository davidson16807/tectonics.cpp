
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "density.hpp"

TEST_CASE( "density order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::pressure<double> p = 1.0*si::pascal; p <= 100000000.0*si::pascal; p*=10.0)
        {
            for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
            {
                for (int i = 0; i<compound::tables::ids::count; i++)
                {
                    if (compound::tables::density_as_liquid.count(i) > 0) {
                        auto x = compound::tables::density_as_liquid[i](p,T);
                        CHECK(x / (si::gram / si::centimeter3) < 20.0); /*based on gold*/ \
                        CHECK(x / (si::gram / si::centimeter3) > 0.01); /*based on mercury*/ \
                    }
                }
            }
        }
    }
}
