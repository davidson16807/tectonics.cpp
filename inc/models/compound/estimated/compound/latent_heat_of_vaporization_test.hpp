
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated latent_heat_of_vaporization order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::compounds::count; i++)
        {
                for (si::temperature<double> T = 3.0*si::kelvin; T <= si::standard_temperature; T*=1.778)
                {
                    si::specific_energy<double> x = compound::estimated::thermodynamics.latent_heat_of_vaporization[i](T);
                    // std::cout << compound::estimated::name[i] << std::endl;
                    // std::cout << T << std::endl;
                    // std::cout << x << std::endl;
                    CHECK(x / (si::joule/si::kilogram) < 100e6); /*based on quartz*/
                    CHECK(x / (si::joule/si::kilogram) > 10.0); /*based on helium*/
                }
                for (si::temperature<double> T = si::standard_temperature; T <= si::solar_temperature; T*=1.778)
                {
                    si::specific_energy<double> x = compound::estimated::thermodynamics.latent_heat_of_vaporization[i](T);
                    // std::cout << compound::estimated::name[i] << std::endl;
                    // std::cout << T << std::endl;
                    // std::cout << x << std::endl;
                    CHECK(x / (si::joule/si::kilogram) < 100e6); /*based on quartz*/
                    CHECK(x / (si::joule/si::kilogram) >= 0.0); /*based on helium*/
                }
        }
    }
}
