
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "refractive_index.hpp"

TEST_CASE( "published liquid refractive_index order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::compounds::count; i++)
        {
            for (si::length<double> l = 10.0*si::nanometer; l <= 1.0*si::micrometer; l*=1.78)
            {
                if (compound::published::refractive_index_as_liquid.has(i)) {
                    auto x = compound::published::refractive_index_as_liquid[i](1.0/l);
                    CHECK(x > 1.02 ); /*based on liquid helium*/ \
                    CHECK(x < 1.8 ); /*based on Methylene iodine*/ \
                }
            }
            for (si::length<double> l = 1.0*si::micrometer; l <= 3.0*si::millimeter; l*=1.78)
            {
                if (compound::published::refractive_index_as_liquid.has(i)) {
                    auto x = compound::published::refractive_index_as_liquid[i](1.0/l);
                    CHECK(x > 1.02 ); /*based on liquid helium*/ \
                    CHECK(x < 2.25 ); /*based on water*/ \
                }
            }
        }
    }
}
