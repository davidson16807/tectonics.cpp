
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

    SECTION("Klincewicz method must predict properties of a known compound to within 30%"){
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

    SECTION("Ihmels method must predict properties of a known compound to within 30%"){
    	CHECK(compound::property::approx_critical_temperature_from_ihmels(pc, Vc) == Approx(Tc).epsilon(0.3));
    	CHECK(compound::property::approx_critical_pressure_from_ihmels(Tc, Vc) == Approx(pc).epsilon(0.3));
    	CHECK(compound::property::approx_critical_molecular_volume_from_ihmels(Tc, pc) == Approx(Vc).epsilon(0.3));
    }
}
TEST_CASE( "Klincewicz-Ihmels consistency", "[properties]" ) {
	// properties of acetone
	float M = 58.080f*units::dalton;
	float Tb = 329.20f*units::kelvin;
	int A = 10;
	float pc = 47.0f*units::bar;
	float Tc = 508.1*units::kelvin;
	float Vc = 209.0f * units::centimeter3/units::mole;
	float Zc = 0.233f;

    SECTION("Klincewicz method and Ihmels method must agree on predictions to within 30%"){
    	CHECK(compound::property::approx_critical_temperature_from_ihmels(pc, Vc) == Approx(compound::property::approx_critical_temperature_from_klincewicz(M, Tb)).epsilon(0.3));
    	CHECK(compound::property::approx_critical_pressure_from_ihmels(Tc, Vc) == Approx(compound::property::approx_critical_pressure_from_klincewicz(M,A)).epsilon(0.3));
    	CHECK(compound::property::approx_critical_molecular_volume_from_ihmels(Tc,pc) == Approx(compound::property::approx_critical_molecular_volume_from_klincewicz(M,A)).epsilon(0.3));
    }

    SECTION("Combining Klincewicz and Ihmels must predict properties of a known compound to within 30%"){
		float pc2 = compound::property::approx_critical_pressure_from_klincewicz(M, A);
		float Vc2 = compound::property::approx_critical_molecular_volume_from_klincewicz(M, A);
		float Tc2 = compound::property::approx_critical_temperature_from_ihmels(pc2, Vc2);
		float Zc2 = compound::property::get_critical_compressibility(pc2, Tc2, Vc2);
    	CHECK(Tc2 == Approx(Tc).epsilon(0.3));
    	CHECK(Zc2 == Approx(Zc).epsilon(0.3));
    }
}
// invertibility
// validation
