


// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <unit/si.hpp>
#include <model/compound/estimated/estimated.hpp>

TEST_CASE( "estimated solid extinction_coefficient order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::polymorphs::count; i++)
        {
            for (si::length<double> l = 10.0*si::nanometer; l <= 3.0*si::micrometer; l*=1.778)
            {
                auto x = compound::estimated::spectra_as_solid.extinction_coefficient[i](1.0/l);
                CHECK(x > 1e-11 ); // for water
                CHECK(x < 30.0 );  // for silver
            }
            for (si::length<double> l = 3.0*si::micrometer; l <= 3.0*si::millimeter; l*=1.778)
            {
                auto x = compound::estimated::spectra_as_solid.extinction_coefficient[i](1.0/l);
                CHECK(x > 1e-11 ); // for water
                CHECK(x < 1000.0 ); // for silver
            }
        }
    }
}
