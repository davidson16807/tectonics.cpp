


// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated solid tensile_modulus order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::polymorphs::count; i++)
            {
                auto x = compound::estimated::elasticities.tensile_modulus[i](T);
                CHECK(x / si::gigapascal > 0.01); /*based on helium*/ \
                CHECK(x / si::gigapascal < 3000.0); /*based on graphite*/ \
            }
        }
    }
}
