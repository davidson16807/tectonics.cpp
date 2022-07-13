
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <units/si_test_utils.hpp>

#include "CompletedSpectralField.hpp"
#include "CompletedSpectralField_to_string.hpp"

TEST_CASE( "CompletedSpectralField to_string() purity", "[field]" ) {
	compound::field::CompletedSpectralField<double> unit  = compound::field::completed_spectral_invariant_test(1.0);
	compound::field::CompletedSpectralField<double> unit_nudged  = compound::field::completed_spectral_invariant_test(1.01);
	compound::field::CompletedSpectralField<si::mass<double>> unit_mass  = compound::field::completed_spectral_invariant_test(1.0 * si::kilogram);
	compound::field::CompletedSpectralField<si::length<double>> unit_length  = compound::field::completed_spectral_invariant_test(1.0 * si::meter);
	compound::field::CompletedSpectralField<double> sample  = compound::field::completed_spectral_invariant_test(2.0);
	compound::field::CompletedSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T){ return test_CompletedSpectralField(nlo,nhi,p,T); });

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::field::to_string(unit) 
    	   == compound::field::to_string(unit));
    	CHECK(compound::field::to_string(unit_nudged) 
    	   == compound::field::to_string(unit_nudged));
    	CHECK(compound::field::to_string(unit_mass) 
    	   == compound::field::to_string(unit_mass));
    	CHECK(compound::field::to_string(unit_length) 
    	   == compound::field::to_string(unit_length));
    	CHECK(compound::field::to_string(sample  ) 
    	   == compound::field::to_string(sample  ));
    	CHECK(compound::field::to_string(relation) 
    	   == compound::field::to_string(relation));
    }
}

TEST_CASE( "CompletedSpectralField to_string() appreciable difference preservation", "[field]" ) {
	compound::field::CompletedSpectralField<double> unit  = compound::field::completed_spectral_invariant_test(1.0);
	compound::field::CompletedSpectralField<double> unit_nudged  = compound::field::completed_spectral_invariant_test(1.01);
	compound::field::CompletedSpectralField<si::mass<double>> unit_mass  = compound::field::completed_spectral_invariant_test(1.0 * si::kilogram);
	compound::field::CompletedSpectralField<si::length<double>> unit_length  = compound::field::completed_spectral_invariant_test(1.0 * si::meter);

	SECTION("Appreciable differences in input are preserved within output")
	{
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_nudged));
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_mass));
    	CHECK(compound::field::to_string(unit_mass) != compound::field::to_string(unit_length));
    }
}
