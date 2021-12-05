
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <units/si_test_utils.hpp>

#include "OptionalSpectralField.hpp"
#include "OptionalSpectralField_to_string.hpp"

TEST_CASE( "OptionalSpectralField to_string() purity", "[field]" ) {
	compound::field::OptionalSpectralField<double> unknown  = std::monostate();
	compound::field::OptionalSpectralField<double> unit = compound::field::optional_spectral_invariant_test(1.0);
	compound::field::OptionalSpectralField<double> unit_nudged  = compound::field::optional_spectral_invariant_test(1.01);
	compound::field::OptionalSpectralField<si::mass> unit_mass  = compound::field::optional_spectral_invariant_test(1.0 * si::kilogram);
	compound::field::OptionalSpectralField<si::length> unit_length  = compound::field::optional_spectral_invariant_test(1.0 * si::meter);
	compound::field::OptionalSpectralField<double> sample = compound::field::optional_spectral_invariant_test(2.0);
	compound::field::OptionalSpectralField<double> relation = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_OptionalSpectralField(nlo,nhi,p,T); });

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::field::to_string(unknown ) 
    	   == compound::field::to_string(unknown ));
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

TEST_CASE( "OptionalSpectralField to_string() appreciable difference preservation", "[field]" ) {
	compound::field::OptionalSpectralField<double> unknown  = std::monostate();
	compound::field::OptionalSpectralField<double> unit = compound::field::optional_spectral_invariant_test(1.0);
	compound::field::OptionalSpectralField<double> unit_nudged  = compound::field::optional_spectral_invariant_test(1.01);
	compound::field::OptionalSpectralField<si::mass> unit_mass  = compound::field::optional_spectral_invariant_test(1.0 * si::kilogram);
	compound::field::OptionalSpectralField<si::length> unit_length  = compound::field::optional_spectral_invariant_test(1.0 * si::meter);

	SECTION("Appreciable differences in input are preserved within output")
	{
    	CHECK(compound::field::to_string(unknown)   != compound::field::to_string(unit));
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_nudged));
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_mass));
    	CHECK(compound::field::to_string(unit_mass) != compound::field::to_string(unit_length));
    }
}
