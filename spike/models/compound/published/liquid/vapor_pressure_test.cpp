
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "vapor_pressure.hpp"

TEST_CASE( "published liquid vapor_pressure order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::ids::count; i++)
        {
            if (compound::published::vapor_pressure_as_liquid.has(i)) 
            {
                for (si::temperature<double> T = 3.0*si::kelvin; T <= 100.0*si::kelvin; T*=1.778)
                {
                    auto x = compound::published::vapor_pressure_as_liquid[i](T);
                    CHECK(x / si::pascal < 3e6); /*based on high temperature acetaldehyde*/ \
                    CHECK(x / si::pascal > 0.03); /*based on low temperature ethylene glycol*/ \

                }
                for (si::temperature<double> T = 300.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
                {
                    auto x = compound::published::vapor_pressure_as_liquid[i](T);
                    CHECK(x / si::pascal < 3e7); /*based on high temperature acetaldehyde*/ \
                    CHECK(x / si::pascal > 0.03); /*based on low temperature ethylene glycol*/ \
                }
            }
        }
    }
}
TEST_CASE( "published liquid vapor_pressure monotonically increasing", "[table]" ) 
{
    SECTION("Vapor pressure must monotonically increase with temperature")
    {
        for (int i = 0; i<compound::ids::count; i++)
        {
            if (compound::published::vapor_pressure_as_liquid.has(i))
            {
                si::temperature<double> T = 3.0*si::kelvin;
                auto last = compound::published::vapor_pressure_as_liquid[i](T);
                for (; T <= si::solar_temperature; T*=3.0)
                {
                    auto next = compound::published::vapor_pressure_as_liquid[i](T);
                    CHECK(next / si::pascal >= last / si::pascal);
                    last = next;
                }
            }
        }
    }
}
