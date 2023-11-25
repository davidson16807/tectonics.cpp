


// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <model/compound/estimated/estimated.hpp>

TEST_CASE( "estimated liquid surface_tension order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::compounds::count; i++)
        {
            for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.78)
            {
                    auto x = compound::estimated::thermodynamics.surface_tension_as_liquid[i](T);
                    CHECK(x / (si::millinewton/si::meter) < 3e3);   /*based on copper*/ \
                    CHECK(x / (si::millinewton/si::meter) > 0.0003); /*based on liquid carbon dioxide (Quinn 1927) */ \
            }
        }
    }
}
