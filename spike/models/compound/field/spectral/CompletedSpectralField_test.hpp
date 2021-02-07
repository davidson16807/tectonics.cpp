
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "CompletedSpectralField.hpp"

#include "CompletedSpectralField_test_utils.hpp"










TEST_CASE( "CompletedSpectralField compare() purity", "[field]" ) {
    si::wavenumber nlo = 14286.0/si::centimeter;
    si::wavenumber nhi = 25000.0/si::centimeter;
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

	compound::field::CompletedSpectralField<double> constant  = 1.0;
	compound::field::CompletedSpectralField<double> sample  = compound::field::SpectralSample<double>(2.0, nlo, nhi, p, T);
	compound::field::CompletedSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_CompletedSpectralField(nlo,nhi,p,T); });

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(constant.compare(constant) 
    	   == constant.compare(constant));
    	CHECK(sample.compare(sample  ) 
    	   == sample.compare(sample  ));
    	CHECK(relation.compare(relation) 
    	   == relation.compare(relation));
    }
}




TEST_CASE( "CompletedSpectralField compare() associativity", "[field]" ) {
    si::wavenumber nlo = 14286.0/si::centimeter;
    si::wavenumber nhi = 25000.0/si::centimeter;
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

    compound::field::CompletedSpectralField<double> constant1  = 1.0;
	compound::field::CompletedSpectralField<double> constant2  = 2.0;
	compound::field::CompletedSpectralField<double> sample  = compound::field::SpectralSample<double>(3.0, nlo, nhi, p, T);

	SECTION("Functions can be applied in any order and still produce the same results")
	{

        CHECK(constant1.compare(constant2.compare(sample)) == 
              constant1.compare(constant2).compare(sample));

        CHECK(constant1.compare(sample.compare(constant2)) == 
              constant1.compare(sample).compare(constant2));


        CHECK(constant1.compare(constant2.compare(sample)) == 
              constant1.compare(constant2).compare(sample));

        CHECK(constant1.compare(sample.compare(constant2)) == 
              constant1.compare(sample).compare(constant2));



        CHECK(constant2.compare(sample.compare(constant1)) == 
              constant2.compare(sample).compare(constant1));

        CHECK(constant2.compare(constant1.compare(sample)) == 
              constant2.compare(constant1).compare(sample));


        CHECK(constant2.compare(sample.compare(constant1)) == 
              constant2.compare(sample).compare(constant1));

        CHECK(constant2.compare(constant1.compare(sample)) == 
              constant2.compare(constant1).compare(sample));

    }
}


TEST_CASE( "CompletedSpectralField compare() increasing", "[field]" ) {
    si::wavenumber nlo = 14286.0/si::centimeter;
    si::wavenumber nhi = 25000.0/si::centimeter;
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

	compound::field::CompletedSpectralField<double> constant  = 1.0;
	compound::field::CompletedSpectralField<double> sample  = compound::field::SpectralSample<double>(2.0, nlo, nhi, p, T);
	compound::field::CompletedSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_CompletedSpectralField(nlo,nhi,p,T); });

	SECTION("An attribute of a function's return entry either increases or remains the same when compared to the same attribute of the input entry")
	{

    	CHECK(constant.compare(constant).index() >= constant.index());
    	CHECK(constant.compare(sample).index() >= constant.index());
    	CHECK(constant.compare(relation).index() >= constant.index());


    	CHECK(sample.compare(constant).index() >= sample.index());
    	CHECK(sample.compare(sample).index() >= sample.index());
    	CHECK(sample.compare(relation).index() >= sample.index());


    	CHECK(relation.compare(constant).index() >= relation.index());
    	CHECK(relation.compare(sample).index() >= relation.index());
    	CHECK(relation.compare(relation).index() >= relation.index());

    }
}


TEST_CASE( "CompletedSpectralField best() commutativity", "[field]" ) {
    si::wavenumber nlo = 14286.0/si::centimeter;
    si::wavenumber nhi = 25000.0/si::centimeter;
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

	compound::field::CompletedSpectralField<double> constant  = 1.0;
	compound::field::CompletedSpectralField<double> sample  = compound::field::SpectralSample<double>(2.0, nlo, nhi, p, T);
	compound::field::CompletedSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_CompletedSpectralField(nlo,nhi,p,T); });

	SECTION("Arguments to a function can be swapped and still produce the same results")
	{

    	CHECK(constant.compare(constant) == constant.compare(constant));
    	CHECK(constant.compare(sample) == sample.compare(constant));
    	CHECK(constant.compare(relation) == relation.compare(constant));


    	CHECK(sample.compare(constant) == constant.compare(sample));
    	CHECK(sample.compare(sample) == sample.compare(sample));
    	CHECK(sample.compare(relation) == relation.compare(sample));


    	CHECK(relation.compare(constant) == constant.compare(relation));
    	CHECK(relation.compare(sample) == sample.compare(relation));
    	CHECK(relation.compare(relation) == relation.compare(relation));

    }
}









TEST_CASE( "CompletedSpectralField map() purity", "[field]" ) {
    si::wavenumber nlo = 14286.0/si::centimeter;
    si::wavenumber nhi = 25000.0/si::centimeter;
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

	compound::field::CompletedSpectralField<double> constant  = 1.0;
	compound::field::CompletedSpectralField<double> sample  = compound::field::SpectralSample<double>(2.0, nlo, nhi, p, T);
	compound::field::CompletedSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_CompletedSpectralField(nlo,nhi,p,T); });
	std::function<double(const double)> f  = [](const double entry){ return 1.0 - 2.0*entry; };

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(constant.map(f) == constant.map(f));
    	CHECK(sample.map(f) == sample.map(f));
    	CHECK(relation.map(f) == relation.map(f));
    }
}

TEST_CASE( "CompletedSpectralField map() identity", "[field]" ) {
    si::wavenumber nlo = 14286.0/si::centimeter;
    si::wavenumber nhi = 25000.0/si::centimeter;
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

	compound::field::CompletedSpectralField<double> constant  = 1.0;
	compound::field::CompletedSpectralField<double> sample  = compound::field::SpectralSample<double>(2.0, nlo, nhi, p, T);
	compound::field::CompletedSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_CompletedSpectralField(nlo,nhi,p,T); });
	std::function<double(const double)> I  = [](const double entry){ return entry; };

	SECTION("There exists a entry that when applied to a function returns the original entry")
	{
    	CHECK(constant.map(I) == constant);
    	CHECK(sample.map(I) == sample);
    	CHECK(relation.map(I) == relation);
    }
}









TEST_CASE( "CompletedSpectralField map_to_constant() purity", "[field]" ) {
    si::wavenumber nlo = 14286.0/si::centimeter;
    si::wavenumber nhi = 25000.0/si::centimeter;
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

    compound::field::CompletedSpectralField<double> constant  = 1.0;
	compound::field::CompletedSpectralField<double> sample  = compound::field::SpectralSample<double>(2.0, nlo, nhi, p, T);
	compound::field::CompletedSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_CompletedSpectralField(nlo,nhi,p,T); });
	std::function<double(const double, const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure, const si::temperature)> f  = 
		[](const double entry, const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return 1.0 - 2.0*entry; };

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(constant.map_to_constant(nlo,nhi,p,T,f) == constant.map_to_constant(nlo,nhi,p,T,f));
    	CHECK(sample.map_to_constant(nlo,nhi,p,T,f) == sample.map_to_constant(nlo,nhi,p,T,f));
    	CHECK(relation.map_to_constant(nlo,nhi,p,T,f) == relation.map_to_constant(nlo,nhi,p,T,f));
    }
}

TEST_CASE( "CompletedSpectralField map_to_constant() identity", "[field]" ) {
    si::wavenumber nlo = 14286.0/si::centimeter;
    si::wavenumber nhi = 25000.0/si::centimeter;
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

    compound::field::CompletedSpectralField<double> constant  = 1.0;
    compound::field::CompletedSpectralField<double> sample  = compound::field::SpectralSample<double>(2.0, nlo, nhi, p, T);
    compound::field::CompletedSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_CompletedSpectralField(nlo,nhi,p,T); });
    std::function<double(const double, const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure, const si::temperature)> I  = 
        [](const double entry, const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return entry; };

    SECTION("There exists a entry that when applied to a function returns the original entry")
    {
        CHECK(constant.map_to_constant(nlo, nhi, p,T,I)  == constant(nlo,nhi,p,T) );
        CHECK(sample.map_to_constant(nlo, nhi, p,T,I)  == sample(nlo,nhi,p,T) );
        CHECK(relation.map_to_constant(nlo, nhi, p,T,I)  == relation(nlo,nhi,p,T) );
    }
}

TEST_CASE( "CompletedSpectralField function() consistency", "[field]" ) {
    si::wavenumber nlo = 14286.0/si::centimeter;
    si::wavenumber nhi = 25000.0/si::centimeter;
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

    compound::field::CompletedSpectralField<double> constant  = 1.0;
    compound::field::CompletedSpectralField<double> sample  = compound::field::SpectralSample<double>(2.0, nlo, nhi, p, T);
    compound::field::CompletedSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, const si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_CompletedSpectralField(nlo,nhi,p,T); });

    SECTION("field.function()(nlo,nhi,p,T) should return the same result as field(nlo,nhi,p,T)")
    {
        CHECK(constant.function()(nlo,nhi,p,T)  == constant(nlo,nhi,p,T) );
        CHECK(sample.function()(nlo,nhi,p,T)  == sample(nlo,nhi,p,T) );
        CHECK(relation.function()(nlo,nhi,p,T)  == relation(nlo,nhi,p,T) );
    }
}