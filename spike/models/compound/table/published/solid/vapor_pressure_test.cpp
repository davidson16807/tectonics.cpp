
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "vapor_pressure.hpp"

TEST_CASE( "published solid vapor_pressure order of magnitude", "[table]" ) 
{
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::ids::count; i++)
        {
            if (compound::published::vapor_pressure_as_solid.has(i))
            {
                for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
                {
                    auto x = compound::published::vapor_pressure_as_solid[i](T);
                    // std::cout << i << std::endl;
                    // std::cout << si::to_string(T) << std::endl;
                    // std::cout << si::to_string(x) << std::endl;
                    CHECK(x / si::kilopascal < 300.0); /*based on tetraflourosilane*/ \
                    CHECK(x / si::pascal >= 0.0); /*based on Claypeyron relation near 0K*/ \
                }
            }
        }
    }
}
TEST_CASE( "published solid vapor_pressure monotonically increase", "[table]" )
{
    SECTION("Vapor pressure must monotonically increase with temperature")
    {
        for (int i = 0; i<compound::ids::count; i++)
        {
            if (compound::published::vapor_pressure_as_solid.has(i))
            {
                si::temperature<double> T = 3.0*si::kelvin;
                auto last = compound::published::vapor_pressure_as_solid[i](T);
                for (; T <= si::solar_temperature; T*=1.778)
                {
                    auto next = compound::published::vapor_pressure_as_solid[i](T);
                    CHECK(next / si::pascal >= last / si::pascal);
                    last = next;
                }
            }
        }
    }
}
