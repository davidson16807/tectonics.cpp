


// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated liquid dynamic_viscosity order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::ids::count; i++)
        {
            for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.16)
            {
                auto x = compound::estimated::dynamic_viscosity_as_liquid[i](T);
                CHECK(x / (si::pascal * si::second) < 1e9); /*based on pitch*/ \
                CHECK(x / (si::pascal * si::second) >= 1e-6); /*based on helium*/ \
            }
        }
    }
}
