
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "latent_heat_of_vaporization.hpp"

TEST_CASE( "published latent_heat_of_vaporization order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::ids::count; i++)
        {
            if (compound::published::latent_heat_of_vaporization.count(i) > 0) {
                auto x = compound::published::latent_heat_of_vaporization[i];
                // std::cout << i << std::endl;
                // std::cout << si::to_string(x) << std::endl;
                CHECK(x / (si::joule/si::kilogram) < 100e6); /*based on quartz*/
                CHECK(x / (si::joule/si::kilogram) > 10.0); /*based on helium*/
            }
        }
    }
}