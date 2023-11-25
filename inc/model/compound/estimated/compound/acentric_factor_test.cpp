
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <model/compound/estimated/estimated.hpp>

TEST_CASE( "estimated acentric_factor order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::compounds::count; i++)
        {
                auto x = compound::estimated::molecular.acentric_factor[i];
                // std::cout << compound::estimated::basic.name[i] << std::endl;
                // std::cout << x << std::endl;
                CHECK(x <=  1.0); /*based on quartz*/
                CHECK(x >= -1.0); /*based on helium*/
        }
    }
}
