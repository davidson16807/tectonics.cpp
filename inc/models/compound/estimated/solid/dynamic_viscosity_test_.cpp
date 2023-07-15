


// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated solid dynamic_viscosity order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::polymorphs::count; i++)
            {
                    auto x = compound::estimated::thermodynamics.dynamic_viscosity_as_solid[i](T);
                    CHECK(x / (si::pascal * si::second) <= 1e24); /*based on granite*/ \
                    CHECK(x / (si::pascal * si::second) > 1e10); /*based on ice*/ \
            }
        }
    }
}
