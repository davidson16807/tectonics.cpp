
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "library.hpp"
#include "library_test_utils.hpp"

TEST_CASE( "CompletedCompound order of magnitude", "[field]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        COMPLETED_COMPOUNDS_VALID()
        COMPLETED_COMPOUNDS_STP_VALID()
        for (float p = 1.0; p <= 100000000.0; p*=10.0)
        {
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(3.0*si::kelvin, p*si::pascal) // cosmic background radiation
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(33.0*si::kelvin, p*si::pascal) // temperature of Pluto
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(90.0*si::kelvin, p*si::pascal) // temperature of Titan
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(0.0*si::celcius, p*si::pascal) // standard temperature
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(20.0*si::celcius, p*si::pascal) // room temperature
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(101.0*si::celcius, p*si::pascal) // boiling water
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(380.0*si::celcius, p*si::pascal) // temperature of Venus
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(1085.0*si::celcius, p*si::pascal) // melted copper
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(1510.0*si::celcius, p*si::pascal) // melted stainless steel
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(2001.0*si::celcius, p*si::pascal) // melted corundum
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(3501.0*si::kelvin, p*si::pascal) // melted carbon
            COMPLETED_PHASES_OF_COMPOUNDS_VALID(si::solar_temperature, p*si::pascal) // temperature of the sun
        }
    }
}