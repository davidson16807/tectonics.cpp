


// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <models/compound/estimated/estimated.hpp>
#include <models/compound/relation/PolynomialRailyardRelation_to_string.hpp>

TEST_CASE( "estimated liquid extinction_coefficient order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::length<double> l = 10.0*si::nanometer; l <= 3.0*si::millimeter; l*=1.78)
        {
            for (int i = 0; i<compound::ids::count; i++)
            {
                    auto x = compound::estimated::extinction_coefficient_as_liquid[i](1.0/l);
                    CHECK(x > 1e-9 ); 
                    CHECK(x < 1.0 );  
            }
        }
    }
}
