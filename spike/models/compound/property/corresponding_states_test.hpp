
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units.hpp>

#include "corresponding_states.hpp"

TEST_CASE( "Klincewicz method purity", "[properties]" ) {
	// properties of acetone
	float M = 58.080f*units::dalton;
	float Tb = 329.20f*units::kelvin;
	int A = 10;
    SECTION("Calling a function twice with the same arguments must produce the same results (tested using properties of water)"){
    	CHECK(compound::property::approx_critical_temperature_from_klincewicz(M, Tb) == 
    		Approx(compound::property::approx_critical_temperature_from_klincewicz(M, Tb)).epsilon(1e-4f));
    	CHECK(compound::property::approx_critical_pressure_from_klincewicz(M, A) == 
    		Approx(compound::property::approx_critical_pressure_from_klincewicz(M, A)).epsilon(1e-4f));
    	CHECK(compound::property::approx_critical_molecular_volume_from_klincewicz(M, A) == 
    		Approx(compound::property::approx_critical_molecular_volume_from_klincewicz(M, A)).epsilon(1e-4f));
    }
}
TEST_CASE( "Klincewicz method sanity test", "[properties]" ) {
	// properties of acetone
	float M = 58.080f*units::dalton;
	float Tb = 329.20f*units::kelvin;
	int A = 10;
	float pc = 47.0f*units::bar;
	float Tc = 508.1*units::kelvin;
	float Vc = 209.0f * units::centimeter3/units::mole;

    SECTION("Klincewicz method must reconstruct properties of a known compound to within 30%"){
    	CHECK(compound::property::approx_critical_temperature_from_klincewicz(M, Tb) == Approx(Tc).epsilon(0.3));
    	CHECK(compound::property::approx_critical_pressure_from_klincewicz(M, A) == Approx(pc).epsilon(0.3));
    	CHECK(compound::property::approx_critical_molecular_volume_from_klincewicz(M, A) == Approx(Vc).epsilon(0.3));
    }
}
TEST_CASE( "Ihmels method purity", "[properties]" ) {
	// properties of acetone
	float pc = 47.0f*units::bar;
	float Tc = 508.1*units::kelvin;
	float Vc = 209.0f * units::centimeter3/units::mole;

    SECTION("Calling a function twice with the same arguments must produce the same results"){
    	CHECK(compound::property::approx_critical_temperature_from_ihmels(pc, Vc) == Approx(compound::property::approx_critical_temperature_from_ihmels(pc, Vc)).epsilon(1e-4f));
    	CHECK(compound::property::approx_critical_pressure_from_ihmels(Tc, Vc) == Approx(compound::property::approx_critical_pressure_from_ihmels(Tc, Vc)).epsilon(1e-4f));
    	CHECK(compound::property::approx_critical_molecular_volume_from_ihmels(Tc, pc) == Approx(compound::property::approx_critical_molecular_volume_from_ihmels(Tc, pc)).epsilon(1e-4f));
    }
}
TEST_CASE( "Ihmels method sanity test", "[properties]" ) {
	// properties of acetone
	float pc = 47.0f*units::bar;
	float Tc = 508.1*units::kelvin;
	float Vc = 209.0f * units::centimeter3/units::mole;

    SECTION("Ihmels method must reconstruct properties of a known compound to within 30%"){
    	CHECK(compound::property::approx_critical_temperature_from_ihmels(pc, Vc) == Approx(Tc).epsilon(0.3));
    	CHECK(compound::property::approx_critical_pressure_from_ihmels(Tc, Vc) == Approx(pc).epsilon(0.3));
    	CHECK(compound::property::approx_critical_molecular_volume_from_ihmels(Tc, pc) == Approx(Vc).epsilon(0.3));
    }
}
// TEST_CASE( "Klincewicz-Ihmels consistency", "[properties]" ) {
// 	// properties of oxygen
// 	float pc = 5.01f*units::meganewton/units::meter2;
// 	float Tc = (273.15f-118.6f)*units::kelvin;
// 	float Vc = 0.0025f*units::meter3/units::kilogram;

//     SECTION("Ihmels method must produce results that are consistent with the Klincewicz method"){
//     }
// }
// invertibility
// validation
