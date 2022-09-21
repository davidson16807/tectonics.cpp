
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "isobaric_specific_heat_capacity.hpp"

TEST_CASE( "gaseous isobaric_specific_heat_capacity order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::pressure<double> p = 1.0*si::pascal; p <= 100000000.0*si::pascal; p*=10.0)
        {
            for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
            {
                for (int i = 0; i<compound::tables::ids::count; i++)
                {
                    if (compound::tables::isobaric_specific_heat_capacity_as_gas.count(i) > 0) {
                        auto x = compound::tables::isobaric_specific_heat_capacity_as_gas[i](p,T);
                        CHECK(x / (si::joule / (si::kilogram * si::kelvin)) > 3e2 ); /*based on argon*/ 
                        CHECK(x / (si::joule / (si::kilogram * si::kelvin)) < 1e5 ); /*based on hydrogen*/ 
                    }
                }
            }
        }
    }
}
