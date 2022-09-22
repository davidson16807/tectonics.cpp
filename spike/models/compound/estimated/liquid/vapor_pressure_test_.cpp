


// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated liquid vapor_pressure order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::ids::count; i++)
        {
            if (compound::estimated::vapor_pressure_as_liquid.count(i) > 0) 
            {
                for (si::temperature<double> T = 3.0*si::kelvin; T <= 100.0*si::kelvin; T*=3.0)
                {
                    auto x = compound::estimated::vapor_pressure_as_liquid[i](T);
                    CHECK(x / si::pascal < 3e6); /*based on high temperature acetaldehyde*/ \
                    CHECK(x / si::pascal > 0.03); /*based on low temperature ethylene glycol*/ \

                }
                for (si::temperature<double> T = 300.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
                {
                    auto x = compound::estimated::vapor_pressure_as_liquid[i](T);
                    CHECK(x / si::pascal < 3e7); /*based on high temperature acetaldehyde*/ \
                    CHECK(x / si::pascal > 0.03); /*based on low temperature ethylene glycol*/ \
                }
            }
        }
    }
}
