
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "OptionalSpectralField.hpp"
#include "OptionalSpectralField_to_CompletedSpectralField.hpp"

double test_OptionalSpectralField2(const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, si::pressure<double> p, const si::temperature<double> T)
{
	return (si::mole*si::universal_gas_constant*T/p/si::liter);
}

TEST_CASE( "OptionalSpectralField complete() purity", "[field]" ) {
    si::wavenumber<double> nlo = 14286.0/si::centimeter;
    si::wavenumber<double> nhi = 25000.0/si::centimeter;
    si::pressure<double> p = si::standard_pressure;
    si::temperature<double> T = si::standard_temperature;

	compound::field::OptionalSpectralField<double> unknown  = std::monostate();
	compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
    compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);
    compound::field::OptionalSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T){ return test_OptionalSpectralField2(nlo,nhi,p,T); });

    compound::field::CompletedSpectralField<double> known_constant  = compound::field::completed_spectral_invariant_test(1.0);
    compound::field::CompletedSpectralField<double> known_sample  = compound::field::completed_spectral_invariant_test(2.0);
    compound::field::CompletedSpectralField<double> known_relation  = compound::field::SpectralFunction<double>([](const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T){ return test_OptionalSpectralField2(nlo,nhi,p,T); });


	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::field::complete(unknown,known_constant )(nlo, nhi, p, T) == compound::field::complete(unknown,known_constant )(nlo, nhi, p, T));
    	CHECK(compound::field::complete(constant,known_constant)(nlo, nhi, p, T) == compound::field::complete(constant,known_constant)(nlo, nhi, p, T));
    	CHECK(compound::field::complete(sample,known_sample  )(nlo, nhi, p, T) == compound::field::complete(sample,known_sample  )(nlo, nhi, p, T));
    	CHECK(compound::field::complete(relation,known_relation)(nlo, nhi, p, T) == compound::field::complete(relation,known_relation)(nlo, nhi, p, T));
    }
}


TEST_CASE( "OptionalSpectralField complete() associativity", "[field]" ) {
    si::wavenumber<double> nlo = 14286.0/si::centimeter;
    si::wavenumber<double> nhi = 25000.0/si::centimeter;
    si::pressure<double> p = si::standard_pressure;
    si::temperature<double> T = si::standard_temperature;
    
	compound::field::OptionalSpectralField<double> unknown  = std::monostate();
	compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
	compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);

    compound::field::CompletedSpectralField<double> known_constant  = compound::field::completed_spectral_invariant_test(1.0);
    compound::field::CompletedSpectralField<double> known_sample  = compound::field::completed_spectral_invariant_test(2.0);
    compound::field::CompletedSpectralField<double> known_relation  = compound::field::SpectralFunction<double>([](const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T){ return test_OptionalSpectralField2(nlo,nhi,p,T); });

	SECTION("Functions can be applied in any order and still produce the same results")
	{
    	CHECK(compound::field::complete(unknown, compound::field::complete(constant,known_sample))(nlo, nhi, p, T) == 
    		  compound::field::complete(unknown,known_constant)(nlo, nhi, p, T));

    	CHECK(compound::field::complete(unknown, compound::field::complete(sample,known_constant))(nlo, nhi, p, T) == 
    		  compound::field::complete(unknown,known_sample)(nlo, nhi, p, T));


    	CHECK(compound::field::complete(unknown, compound::field::complete(constant,known_sample))(nlo, nhi, p, T) == 
    		  compound::field::complete(unknown,known_constant)(nlo, nhi, p, T));

    	CHECK(compound::field::complete(unknown, compound::field::complete(sample,known_constant))(nlo, nhi, p, T) == 
    		  compound::field::complete(unknown,known_sample)(nlo, nhi, p, T));



    	CHECK(compound::field::complete(constant, compound::field::complete(sample,known_constant))(nlo, nhi, p, T) == 
    		  compound::field::complete(constant,known_sample)(nlo, nhi, p, T));

    	CHECK(compound::field::complete(constant, compound::field::complete(unknown,known_sample))(nlo, nhi, p, T) == 
    		  compound::field::complete(constant,known_constant)(nlo, nhi, p, T));


    	CHECK(compound::field::complete(constant, compound::field::complete(sample,known_constant))(nlo, nhi, p, T) == 
    		  compound::field::complete(constant,known_sample)(nlo, nhi, p, T));

    	CHECK(compound::field::complete(constant, compound::field::complete(unknown,known_sample))(nlo, nhi, p, T) == 
    		  compound::field::complete(constant,known_constant)(nlo, nhi, p, T));

    }
}


TEST_CASE( "OptionalSpectralField complete() increasing", "[field]" ) {
	compound::field::OptionalSpectralField<double> unknown  = std::monostate();
	compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
	compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);
	compound::field::OptionalSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T){ return test_OptionalSpectralField2(nlo,nhi,p,T); });

    compound::field::CompletedSpectralField<double> known_constant  = compound::field::completed_spectral_invariant_test(1.0);
    compound::field::CompletedSpectralField<double> known_sample  = compound::field::completed_spectral_invariant_test(2.0);
    compound::field::CompletedSpectralField<double> known_relation  = compound::field::SpectralFunction<double>([](const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T){ return test_OptionalSpectralField2(nlo,nhi,p,T); });

	SECTION("An attribute of a function's return entry either increases or remains the same when compared to the same attribute of the input entry")
	{
    	CHECK(compound::field::complete(unknown,known_constant).index()+1 >= unknown.index());
    	CHECK(compound::field::complete(unknown,known_sample).index()+1 >= unknown.index());
    	CHECK(compound::field::complete(unknown,known_relation).index()+1 >= unknown.index());


    	CHECK(compound::field::complete(constant,known_constant).index()+1 >= constant.index());
    	CHECK(compound::field::complete(constant,known_sample).index()+1 >= constant.index());
    	CHECK(compound::field::complete(constant,known_relation).index()+1 >= constant.index());


    	CHECK(compound::field::complete(sample,known_constant).index()+1 >= sample.index());
    	CHECK(compound::field::complete(sample,known_sample).index()+1 >= sample.index());
    	CHECK(compound::field::complete(sample,known_relation).index()+1 >= sample.index());


    	CHECK(compound::field::complete(relation,known_constant).index()+1 >= relation.index());
    	CHECK(compound::field::complete(relation,known_sample).index()+1 >= relation.index());
    	CHECK(compound::field::complete(relation,known_relation).index()+1 >= relation.index());
    }
}






