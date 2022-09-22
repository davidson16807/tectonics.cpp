
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "isobaric_specific_heat_capacity.hpp"

TEST_CASE( "published liquid isobaric_specific_heat_capacity order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::ids::count; i++)
        {
            if (compound::published::isobaric_specific_heat_capacity_as_liquid.count(i) > 0) 
            {
                for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
                {
                    auto x = compound::published::isobaric_specific_heat_capacity_as_liquid[i](T);
                    CHECK(x / (si::joule/(si::gram*si::kelvin)) < 100.0); /*based on hydrogen*/ \
                    CHECK(x / (si::joule/(si::gram*si::kelvin)) > 0.1); /*based on gold*/ \
                }
            }
        }
    }
}
