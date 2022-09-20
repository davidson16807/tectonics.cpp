
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "isobaric_specific_heat_capacity.hpp"

TEST_CASE( "isobaric_specific_heat_capacity order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
        {
            for (int i = 0; i<compound::tables::ids::count; i++)
            {
                if (compound::tables::isobaric_specific_heat_capacity_as_solid.count(i) > 0) {
                    auto x = compound::tables::isobaric_specific_heat_capacity_as_solid[i](T);
                    CHECK(x / (si::joule/(si::kilogram * si::kelvin)) < 30000.0); /*based on hydrogen*/ \
                    CHECK(x / (si::joule/(si::kilogram * si::kelvin)) >= 0.0); /*based on predicted behavior at 0K*/ \
                }
            }
        }
    }
}
