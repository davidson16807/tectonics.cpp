
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "acentric_factor.hpp"

TEST_CASE( "estimated acentric_factor order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::ids::count; i++)
        {
            if (compound::estimated::acentric_factor.has(i)) {
                auto x = compound::estimated::acentric_factor[i];
                // std::cout << i << std::endl;
                // std::cout << si::to_string(x) << std::endl;
                CHECK(x / (si::joule/si::kilogram) <  1.0); /*based on quartz*/
                CHECK(x / (si::joule/si::kilogram) > -1.0); /*based on helium*/
            }
        }
    }
}
