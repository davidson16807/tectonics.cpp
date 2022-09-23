
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "extinction_coefficient.hpp"

TEST_CASE( "published liquid extinction_coefficient order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::length<double> l = 10.0*si::nanometer; l <= 3.0*si::millimeter; l*=1.78)
        {
            for (int i = 0; i<compound::ids::count; i++)
            {
                if (compound::published::extinction_coefficient_as_liquid.count(i) > 0) {
                    auto x = compound::published::extinction_coefficient_as_liquid[i](1.0/l);
                    CHECK(x > 1e-9 ); 
                    CHECK(x < 1.0 );  
                }
            }
        }
    }
}
