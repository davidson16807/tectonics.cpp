


// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <model/compound/estimated/estimated.hpp>

TEST_CASE( "estimated liquid thermal_conductivity order of magnitude", "[table]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        for (int i = 0; i<compound::compounds::count; i++)
        {
            for (si::temperature<double> T = 3.0*si::kelvin; T <= si::solar_temperature; T*=3.0)
            {
                auto x = compound::estimated::thermodynamics.thermal_conductivity_as_liquid[i](T);
                // std::cout << compound::estimated::name[i] << std::endl;
                // std::cout << T << std::endl;
                // std::cout << x << std::endl;
                CHECK(x / (si::watt / (si::meter * si::kelvin)) < 1000.0); /*based on carbon*/ \
                CHECK(x / (si::watt / (si::meter * si::kelvin)) > 0.01); /*based on helium*/ \
            }
        }
    }
}
