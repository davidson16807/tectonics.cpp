
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated solid density order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        std::cout << si::to_string(compound::published::density_as_liquid[compound::ids::pyrimidine](si::standard_temperature)) << std::endl;
        std::cout << si::to_string(compound::property::guess_density_as_solid_from_density_as_liquid(compound::published::density_as_liquid[compound::ids::pyrimidine](si::standard_temperature))) << std::endl;
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::ids::count; i++)
            {
                auto x = compound::estimated::density_as_solid[i](T);
                // std::cout << i << std::endl;
                // std::cout << si::to_string(T) << std::endl;
                // std::cout << si::to_string(x) << std::endl;
                CHECK(x / (si::kilogram / si::meter3) < 30000.0); /*based on iridium*/ \
                CHECK(x / (si::kilogram / si::meter3) > 1.5); /*based on aerogel*/ \
            }
        }
    }
}
