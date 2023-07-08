
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/handbook/estimated.hpp>

TEST_CASE( "estimated solid lame_parameter order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::ids::count; i++)
            {
                auto x = compound::estimated::elasticities.lame_parameter[i](T);
                CHECK(x / si::gigapascal > 0.001); /*based on helium*/
                CHECK(x / si::gigapascal < 1000.0); /*based on various rocks*/
            }
        }
    }
}
