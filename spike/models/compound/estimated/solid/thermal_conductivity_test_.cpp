


// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated solid thermal_conductivity order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
        {
            for (int i = 0; i<compound::ids::count; i++)
            {
                if (compound::estimated::thermal_conductivity_as_solid.count(i) > 0) {
                    auto x = compound::estimated::thermal_conductivity_as_solid[i](T);
                    CHECK(x / (si::watt/(si::meter * si::kelvin)) < 30000.0); /*based on silver at 10K*/ \
                    CHECK(x / (si::watt/(si::meter * si::kelvin)) > 0.01); /*based on aerogel*/ \
                }
            }
        }
    }
}