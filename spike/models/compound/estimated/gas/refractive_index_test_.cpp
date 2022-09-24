// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated gaseous refractive_index order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::length<double> l = 10.0*si::nanometer; l <= 3.0*si::millimeter; l*=1.78)
        {
            for (int i = 0; i<compound::ids::count; i++)
            {
                if (compound::estimated::refractive_index_as_gas.has(i)) {
                    auto x = compound::estimated::refractive_index_as_gas[i](1.0/l);
                    CHECK(x > 1.00003 ); // for helium
                    CHECK(x < 1.001 );   // for acetone
                }
            }
        }
    }
}
