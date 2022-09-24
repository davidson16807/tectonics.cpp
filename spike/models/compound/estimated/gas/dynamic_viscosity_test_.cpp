
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <models/compound/estimated/estimated.hpp>

TEST_CASE( "estimated gaseous dynamic_viscosity order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (si::pressure<double> p = 1.0*si::pascal; p <= 100000000.0*si::pascal; p*=10.0)
        {
            for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
            {
                for (int i = 0; i<compound::ids::count; i++)
                {
                    if (compound::estimated::dynamic_viscosity_as_gas.has(i)) {
                        auto x = compound::estimated::dynamic_viscosity_as_gas[i](p,T);
                        CHECK(x / (si::pascal * si::second) < 10e-5); /*based on argon at 500°C*/ 
                        CHECK(x / (si::pascal * si::second) > 1e-6); /*based on steam*/ 
                    }
                }
            }
        }
    }
}
