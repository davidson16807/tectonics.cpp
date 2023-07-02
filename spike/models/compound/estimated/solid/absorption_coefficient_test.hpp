
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated solid absorption_coefficient order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::ids::count; i++)
        {
            for (si::length<double> l = 10.0*si::nanometer; l <= 3.0*si::millimeter; l*=1.78)
            {
                auto x = compound::estimated::spectrums.absorption_coefficient[i](1.0/l);
                CHECK(x * si::centimeter < 300000000.0); /*based on water*/ 
                // CHECK(x * si::centimeter < 1000000.0); /*based on water*/ 
                CHECK(x * si::centimeter >= 0.0); /*based on nitrogen*/ 
            }
        }
    }
}
