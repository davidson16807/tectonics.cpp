
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "refractive_index.hpp"

TEST_CASE( "published solid refractive_index order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::compounds::count; i++)
        {
            for (si::length<double> l = 10.0*si::nanometer; l <= 3.0*si::micrometer; l*=1.778)
            {
                if (compound::published::refractive_index_as_solid.has(i)) {
                    auto x = compound::published::refractive_index_as_solid[i](1.0/l);
                    CHECK(x < 4.1 ); /*based on germanium */\
                    CHECK(x > 0.2); /*based on silver*/\
                }
            }
            for (si::length<double> l = 3.0*si::micrometer; l <= 3.0*si::millimeter; l*=1.778)
            {
                if (compound::published::refractive_index_as_solid.has(i)) {
                    auto x = compound::published::refractive_index_as_solid[i](1.0/l);
                    CHECK(x < 1000.0 ); /*based on copper at long wavelengths */\
                    CHECK(x > 0.2);     /*based on silver*/\
                }
            }
        }
    }
}
