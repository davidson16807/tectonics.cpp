
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated solid poisson_ratio order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=1.778)
        {
            for (int i = 0; i<compound::ids::count; i++)
            {
                if (compound::estimated::poisson_ratio_as_solid.count(i) > 0) {
                    auto x = compound::estimated::poisson_ratio_as_solid[i](T);
                    CHECK(x < 0.5); /*based on rubber*/ \
                    CHECK(x > 0.0); /*based on cork*/ \
                }
            }
        }
    }
}
