


// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <model/compound/estimated/estimated.hpp>

TEST_CASE( "estimated solid shear_fracture_strength order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::polymorphs::count; i++)
            {
                auto x = compound::estimated::strengths.shear_fracture[i](T);
                CHECK(x / si::megapascal < 10000.0); /*based on high performance steels*/
                CHECK(x / si::megapascal > 0.1); /*based on hydrogen*/
            }
        }
    }
}
