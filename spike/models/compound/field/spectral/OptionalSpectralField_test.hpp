
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "OptionalSpectralField.hpp"

#include "OptionalSpectralField_test_utils.hpp"



TEST_CASE( "OptionalSpectralField value_or() purity", "[field]" ) {
	compound::field::OptionalSpectralField<double> unknown  = std::monostate();
	compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
	compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);
	compound::field::OptionalSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_OptionalSpectralField(nlo,nhi,p,T); });


	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.value_or(unknown ) 
    	   == unknown.value_or(unknown ));
    	CHECK(constant.value_or(constant) 
    	   == constant.value_or(constant));
    	CHECK(sample.value_or(sample  ) 
    	   == sample.value_or(sample  ));
    	CHECK(relation.value_or(relation) 
    	   == relation.value_or(relation));
    }
}

TEST_CASE( "OptionalSpectralField value_or() identity", "[field]" ) {
	compound::field::OptionalSpectralField<double> unknown  = std::monostate();
	compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
	compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);
	compound::field::OptionalSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_OptionalSpectralField(nlo,nhi,p,T); });


	SECTION("There exists a entry that when applied to a function returns the original entry")
	{
    	CHECK(unknown.value_or(unknown) == unknown);
    	CHECK(constant.value_or(unknown) == constant);
    	CHECK(sample.value_or(unknown) == sample);
    	CHECK(relation.value_or(unknown) == relation);
    }
}

TEST_CASE( "OptionalSpectralField value_or() associativity", "[field]" ) {
	compound::field::OptionalSpectralField<double> unknown  = std::monostate();
	compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
	compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);

	SECTION("Functions can be applied in any order and still produce the same results")
	{
    	CHECK(unknown.value_or(constant.value_or(sample)) == 
    		  unknown.value_or(constant).value_or(sample));

    	CHECK(unknown.value_or(sample.value_or(constant)) == 
    		  unknown.value_or(sample).value_or(constant));


    	CHECK(unknown.value_or(constant.value_or(sample)) == 
    		  unknown.value_or(constant).value_or(sample));

    	CHECK(unknown.value_or(sample.value_or(constant)) == 
    		  unknown.value_or(sample).value_or(constant));





    	CHECK(constant.value_or(sample.value_or(unknown)) == 
    		  constant.value_or(sample).value_or(unknown));

    	CHECK(constant.value_or(unknown.value_or(sample)) == 
    		  constant.value_or(unknown).value_or(sample));


    	CHECK(constant.value_or(sample.value_or(unknown)) == 
    		  constant.value_or(sample).value_or(unknown));

    	CHECK(constant.value_or(unknown.value_or(sample)) == 
    		  constant.value_or(unknown).value_or(sample));

    }
}


TEST_CASE( "OptionalSpectralField value_or() increasing", "[field]" ) {
	compound::field::OptionalSpectralField<double> unknown  = std::monostate();
	compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
	compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);
	compound::field::OptionalSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_OptionalSpectralField(nlo,nhi,p,T); });

	SECTION("An attribute of a function's return entry either increases or remains the same when compared to the same attribute of the input entry")
	{

    	CHECK(unknown.value_or(unknown).index() >= unknown.index());
    	CHECK(unknown.value_or(constant).index() >= unknown.index());
    	CHECK(unknown.value_or(sample).index() >= unknown.index());
    	CHECK(unknown.value_or(relation).index() >= unknown.index());


    	CHECK(constant.value_or(unknown).index() >= constant.index());
    	CHECK(constant.value_or(constant).index() >= constant.index());
    	CHECK(constant.value_or(sample).index() >= constant.index());
    	CHECK(constant.value_or(relation).index() >= constant.index());


    	CHECK(sample.value_or(unknown).index() >= sample.index());
    	CHECK(sample.value_or(constant).index() >= sample.index());
    	CHECK(sample.value_or(sample).index() >= sample.index());
    	CHECK(sample.value_or(relation).index() >= sample.index());


    	CHECK(relation.value_or(unknown).index() >= relation.index());
    	CHECK(relation.value_or(constant).index() >= relation.index());
    	CHECK(relation.value_or(sample).index() >= relation.index());
    	CHECK(relation.value_or(relation).index() >= relation.index());

    }
}
















TEST_CASE( "OptionalSpectralField value_or(f,a) purity", "[field]" ) {
    compound::field::OptionalSpectralField<double> unknown  = std::monostate();
    compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
    compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);
    compound::field::OptionalSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_OptionalSpectralField(nlo,nhi,p,T); });
    std::function<double(compound::field::SpectralParameters, const double)> f  = [](const compound::field::SpectralParameters params, const double entry){ return 1.0 - 2.0*entry; };

    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(unknown.value_or(f,unknown) == unknown.value_or(f,unknown));
        CHECK(unknown.value_or(f,constant) == unknown.value_or(f,constant));
        CHECK(unknown.value_or(f,sample) == unknown.value_or(f,sample));
        CHECK(unknown.value_or(f,relation) == unknown.value_or(f,relation));

        CHECK(constant.value_or(f,unknown) == constant.value_or(f,unknown));
        CHECK(constant.value_or(f,constant) == constant.value_or(f,constant));
        CHECK(constant.value_or(f,sample) == constant.value_or(f,sample));
        CHECK(constant.value_or(f,relation) == constant.value_or(f,relation));

        CHECK(sample.value_or(f,unknown) == sample.value_or(f,unknown));
        CHECK(sample.value_or(f,constant) == sample.value_or(f,constant));
        CHECK(sample.value_or(f,sample) == sample.value_or(f,sample));
        CHECK(sample.value_or(f,relation) == sample.value_or(f,relation));
    }
}

TEST_CASE( "OptionalSpectralField value_or(f,a) identity", "[field]" ) {
    compound::field::OptionalSpectralField<double> unknown  = std::monostate();
    compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
    compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);
    compound::field::OptionalSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_OptionalSpectralField(nlo,nhi,p,T); });
    std::function<double(compound::field::SpectralParameters, const double)> I  = [](const compound::field::SpectralParameters params, const double entry){ return entry; };

    SECTION("There exists a entry that when applied to a function returns the original entry")
    {
        CHECK(unknown.value_or(I,unknown) == unknown);
        CHECK(unknown.value_or(I,constant) == constant);
        CHECK(unknown.value_or(I,sample) == sample);
        CHECK(unknown.value_or(I,relation) == relation);
    }
}










TEST_CASE( "OptionalSpectralField value_or(f,a,b) purity", "[field]" ) {
    compound::field::OptionalSpectralField<double> unknown  = std::monostate();
    compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
    compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);
    compound::field::OptionalSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_OptionalSpectralField(nlo,nhi,p,T); });
    std::function<double(compound::field::SpectralParameters, const double, const double)> f  = [](const compound::field::SpectralParameters params, const double first, const double second){ return 1.0 - 2.0*first + 3.0*second; };

    compound::field::OptionalSpectralField<int> unknown_i  = std::monostate();
    compound::field::OptionalSpectralField<int> constant_i  =compound::field::optional_spectral_invariant_test(1);
    compound::field::OptionalSpectralField<int> sample_i  = compound::field::optional_spectral_invariant_test(2);
    compound::field::OptionalSpectralField<int> relation_i  = compound::field::SpectralFunction<int>([](const si::wavenumber nlo, si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_OptionalSpectralField(nlo,nhi,p,T); });
    std::function<double(compound::field::SpectralParameters, const int, const double)> f_id  = [](const compound::field::SpectralParameters params, const int first, const double second){ return 1.0 - 2.0*first + 3.0*second; };
    std::function<double(compound::field::SpectralParameters, const double, const int)> f_di  = [](const compound::field::SpectralParameters params, const double first, const int second){ return 1.0 - 2.0*first + 3.0*second; };
    std::function<double(compound::field::SpectralParameters, const int, const int)> f_ii  = [](const compound::field::SpectralParameters params, const int first, const int second){ return 1.0 - 2.0*first + 3.0*second; };

    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(unknown.value_or(f, unknown,  unknown) == unknown.value_or(f, unknown,  unknown));
        CHECK(unknown.value_or(f, constant, unknown) == unknown.value_or(f, constant, unknown));
        CHECK(unknown.value_or(f, sample,   unknown) == unknown.value_or(f, sample,   unknown));
        CHECK(unknown.value_or(f, relation, unknown) == unknown.value_or(f, relation, unknown));

        CHECK(unknown.value_or(f, unknown,  constant) == unknown.value_or(f, unknown,  constant));
        CHECK(unknown.value_or(f, constant, constant) == unknown.value_or(f, constant, constant));
        CHECK(unknown.value_or(f, sample,   constant) == unknown.value_or(f, sample,   constant));
        CHECK(unknown.value_or(f, relation, constant) == unknown.value_or(f, relation, constant));

        CHECK(unknown.value_or(f, unknown,  sample) == unknown.value_or(f, unknown,  sample));
        CHECK(unknown.value_or(f, constant, sample) == unknown.value_or(f, constant, sample));
        CHECK(unknown.value_or(f, sample,   sample) == unknown.value_or(f, sample,   sample));
        CHECK(unknown.value_or(f, relation, sample) == unknown.value_or(f, relation, sample));

        CHECK(unknown.value_or(f, unknown,  relation) == unknown.value_or(f, unknown,  relation));
        CHECK(unknown.value_or(f, constant, relation) == unknown.value_or(f, constant, relation));
        CHECK(unknown.value_or(f, sample,   relation) == unknown.value_or(f, sample,   relation));
        CHECK(unknown.value_or(f, relation, relation) == unknown.value_or(f, relation, relation));

        CHECK(constant.value_or(f, constant, constant) == constant.value_or(f, constant, constant));
        CHECK(sample  .value_or(f, constant, constant) == sample  .value_or(f, constant, constant));
        CHECK(relation.value_or(f, constant, constant) == relation.value_or(f, constant, constant));

        CHECK(constant.value_or(f, sample, sample) == constant.value_or(f, sample, sample));
        CHECK(sample  .value_or(f, sample, sample) == sample  .value_or(f, sample, sample));
        CHECK(relation.value_or(f, sample, sample) == relation.value_or(f, sample, sample));

        CHECK(constant.value_or(f, relation, relation) == constant.value_or(f, relation, relation));
        CHECK(sample  .value_or(f, relation, relation) == sample  .value_or(f, relation, relation));
        CHECK(relation.value_or(f, relation, relation) == relation.value_or(f, relation, relation));



        CHECK(unknown.value_or(f_ii, unknown_i,  unknown_i) == unknown.value_or(f_ii, unknown_i,  unknown_i));
        CHECK(unknown.value_or(f_ii, constant_i, unknown_i) == unknown.value_or(f_ii, constant_i, unknown_i));
        CHECK(unknown.value_or(f_ii, sample_i,   unknown_i) == unknown.value_or(f_ii, sample_i,   unknown_i));
        CHECK(unknown.value_or(f_ii, relation_i, unknown_i) == unknown.value_or(f_ii, relation_i, unknown_i));

        CHECK(unknown.value_or(f_ii, unknown_i,  constant_i) == unknown.value_or(f_ii, unknown_i,  constant_i));
        CHECK(unknown.value_or(f_ii, constant_i, constant_i) == unknown.value_or(f_ii, constant_i, constant_i));
        CHECK(unknown.value_or(f_ii, sample_i,   constant_i) == unknown.value_or(f_ii, sample_i,   constant_i));
        CHECK(unknown.value_or(f_ii, relation_i, constant_i) == unknown.value_or(f_ii, relation_i, constant_i));

        CHECK(unknown.value_or(f_ii, unknown_i,  sample_i) == unknown.value_or(f_ii, unknown_i,  sample_i));
        CHECK(unknown.value_or(f_ii, constant_i, sample_i) == unknown.value_or(f_ii, constant_i, sample_i));
        CHECK(unknown.value_or(f_ii, sample_i,   sample_i) == unknown.value_or(f_ii, sample_i,   sample_i));
        CHECK(unknown.value_or(f_ii, relation_i, sample_i) == unknown.value_or(f_ii, relation_i, sample_i));

        CHECK(unknown.value_or(f_ii, unknown_i,  relation_i) == unknown.value_or(f_ii, unknown_i,  relation_i));
        CHECK(unknown.value_or(f_ii, constant_i, relation_i) == unknown.value_or(f_ii, constant_i, relation_i));
        CHECK(unknown.value_or(f_ii, sample_i,   relation_i) == unknown.value_or(f_ii, sample_i,   relation_i));
        CHECK(unknown.value_or(f_ii, relation_i, relation_i) == unknown.value_or(f_ii, relation_i, relation_i));

        CHECK(constant.value_or(f_ii, constant_i, constant_i) == constant.value_or(f_ii, constant_i, constant_i));
        CHECK(sample  .value_or(f_ii, constant_i, constant_i) == sample  .value_or(f_ii, constant_i, constant_i));
        CHECK(relation.value_or(f_ii, constant_i, constant_i) == relation.value_or(f_ii, constant_i, constant_i));

        CHECK(constant.value_or(f_ii, sample_i, sample_i) == constant.value_or(f_ii, sample_i, sample_i));
        CHECK(sample  .value_or(f_ii, sample_i, sample_i) == sample  .value_or(f_ii, sample_i, sample_i));
        CHECK(relation.value_or(f_ii, sample_i, sample_i) == relation.value_or(f_ii, sample_i, sample_i));

        CHECK(constant.value_or(f_ii, relation_i, relation_i) == constant.value_or(f_ii, relation_i, relation_i));
        CHECK(sample  .value_or(f_ii, relation_i, relation_i) == sample  .value_or(f_ii, relation_i, relation_i));
        CHECK(relation.value_or(f_ii, relation_i, relation_i) == relation.value_or(f_ii, relation_i, relation_i));



        CHECK(unknown.value_or(f_id, unknown_i,  unknown) == unknown.value_or(f_id, unknown_i,  unknown));
        CHECK(unknown.value_or(f_id, constant_i, unknown) == unknown.value_or(f_id, constant_i, unknown));
        CHECK(unknown.value_or(f_id, sample_i,   unknown) == unknown.value_or(f_id, sample_i,   unknown));
        // CHECK(unknown.value_or(f_id, relation_i, unknown) == unknown.value_or(f_id, relation_i, unknown));

        CHECK(unknown.value_or(f_id, unknown_i,  constant) == unknown.value_or(f_id, unknown_i,  constant));
        CHECK(unknown.value_or(f_id, constant_i, constant) == unknown.value_or(f_id, constant_i, constant));
        CHECK(unknown.value_or(f_id, sample_i,   constant) == unknown.value_or(f_id, sample_i,   constant));
        // CHECK(unknown.value_or(f_id, relation_i, constant) == unknown.value_or(f_id, relation_i, constant));

        CHECK(unknown.value_or(f_id, unknown_i,  sample) == unknown.value_or(f_id, unknown_i,  sample));
        CHECK(unknown.value_or(f_id, constant_i, sample) == unknown.value_or(f_id, constant_i, sample));
        CHECK(unknown.value_or(f_id, sample_i,   sample) == unknown.value_or(f_id, sample_i,   sample));
        // CHECK(unknown.value_or(f_id, relation_i, sample) == unknown.value_or(f_id, relation_i, sample));

        // CHECK(unknown.value_or(f_id, unknown_i,  relation) == unknown.value_or(f_id, unknown_i,  relation));
        // CHECK(unknown.value_or(f_id, constant_i, relation) == unknown.value_or(f_id, constant_i, relation));
        // CHECK(unknown.value_or(f_id, sample_i,   relation) == unknown.value_or(f_id, sample_i,   relation));
        // CHECK(unknown.value_or(f_id, relation_i, relation) == unknown.value_or(f_id, relation_i, relation));

        CHECK(constant.value_or(f_id, constant_i, constant) == constant.value_or(f_id, constant_i, constant));
        CHECK(sample  .value_or(f_id, constant_i, constant) == sample  .value_or(f_id, constant_i, constant));
        // CHECK(relation.value_or(f_id, constant_i, constant) == relation.value_or(f_id, constant_i, constant));

        CHECK(constant.value_or(f_id, sample_i, sample) == constant.value_or(f_id, sample_i, sample));
        CHECK(sample  .value_or(f_id, sample_i, sample) == sample  .value_or(f_id, sample_i, sample));
        // CHECK(relation.value_or(f_id, sample_i, sample) == relation.value_or(f_id, sample_i, sample));

        // CHECK(constant.value_or(f_id, relation_i, relation) == constant.value_or(f_id, relation_i, relation));
        // CHECK(sample  .value_or(f_id, relation_i, relation) == sample  .value_or(f_id, relation_i, relation));
        // CHECK(relation.value_or(f_id, relation_i, relation) == relation.value_or(f_id, relation_i, relation));



        CHECK(unknown.value_or(f_di, unknown,  unknown_i) == unknown.value_or(f_di, unknown,  unknown_i));
        CHECK(unknown.value_or(f_di, constant, unknown_i) == unknown.value_or(f_di, constant, unknown_i));
        CHECK(unknown.value_or(f_di, sample,   unknown_i) == unknown.value_or(f_di, sample,   unknown_i));
        // CHECK(unknown.value_or(f_di, relation, unknown_i) == unknown.value_or(f_di, relation, unknown_i));

        CHECK(unknown.value_or(f_di, unknown,  constant_i) == unknown.value_or(f_di, unknown,  constant_i));
        CHECK(unknown.value_or(f_di, constant, constant_i) == unknown.value_or(f_di, constant, constant_i));
        CHECK(unknown.value_or(f_di, sample,   constant_i) == unknown.value_or(f_di, sample,   constant_i));
        // CHECK(unknown.value_or(f_di, relation, constant_i) == unknown.value_or(f_di, relation, constant_i));

        CHECK(unknown.value_or(f_di, unknown,  sample_i) == unknown.value_or(f_di, unknown,  sample_i));
        CHECK(unknown.value_or(f_di, constant, sample_i) == unknown.value_or(f_di, constant, sample_i));
        CHECK(unknown.value_or(f_di, sample,   sample_i) == unknown.value_or(f_di, sample,   sample_i));
        // CHECK(unknown.value_or(f_di, relation, sample_i) == unknown.value_or(f_di, relation, sample_i));

        // CHECK(unknown.value_or(f_di, unknown,  relation_i) == unknown.value_or(f_di, unknown,  relation_i));
        // CHECK(unknown.value_or(f_di, constant, relation_i) == unknown.value_or(f_di, constant, relation_i));
        // CHECK(unknown.value_or(f_di, sample,   relation_i) == unknown.value_or(f_di, sample,   relation_i));
        // CHECK(unknown.value_or(f_di, relation, relation_i) == unknown.value_or(f_di, relation, relation_i));

        CHECK(constant.value_or(f_di, constant, constant_i) == constant.value_or(f_di, constant, constant_i));
        CHECK(sample  .value_or(f_di, constant, constant_i) == sample  .value_or(f_di, constant, constant_i));
        // CHECK(relation.value_or(f_di, constant, constant_i) == relation.value_or(f_di, constant, constant_i));

        CHECK(constant.value_or(f_di, sample, sample_i) == constant.value_or(f_di, sample, sample_i));
        CHECK(sample  .value_or(f_di, sample, sample_i) == sample  .value_or(f_di, sample, sample_i));
        // CHECK(relation.value_or(f_di, sample, sample_i) == relation.value_or(f_di, sample, sample_i));

        // CHECK(constant.value_or(f_di, relation, relation_i) == constant.value_or(f_di, relation, relation_i));
        // CHECK(sample  .value_or(f_di, relation, relation_i) == sample  .value_or(f_di, relation, relation_i));
        // CHECK(relation.value_or(f_di, relation, relation_i) == relation.value_or(f_di, relation, relation_i));
    }
}

TEST_CASE( "OptionalSpectralField value_or(f,a,b) free theorem identity", "[field]" ) {
    compound::field::OptionalSpectralField<double> unknown  = std::monostate();
    compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
    compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);
    compound::field::OptionalSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_OptionalSpectralField(nlo,nhi,p,T); });
    std::function<double(compound::field::SpectralParameters, const double, const double)> f  = [](const compound::field::SpectralParameters params, const double first, const double second){ return first + second; };
    compound::field::OptionalSpectralField<double> I  = 0.0;

    SECTION("A function will have the identity property if it is passed a function that itself has the identity property (this presumes for our case that the function is invoked)")
    {
        CHECK(unknown.value_or(f, unknown,  I) == unknown );
        CHECK(unknown.value_or(f, constant, I) == constant);
        CHECK(unknown.value_or(f, sample,   I) == sample  );
        CHECK(unknown.value_or(f, relation, I) == relation);
    }
}

TEST_CASE( "OptionalSpectralField value_or(f,a,b) free theorem commutativity", "[field]" ) {
    compound::field::OptionalSpectralField<double> unknown  = std::monostate();
    compound::field::OptionalSpectralField<double> constant  = compound::field::optional_spectral_invariant_test(1.0);
    compound::field::OptionalSpectralField<double> sample  = compound::field::optional_spectral_invariant_test(2.0);
    compound::field::OptionalSpectralField<double> relation  = compound::field::SpectralFunction<double>([](const si::wavenumber nlo, si::wavenumber nhi, const si::pressure p, const si::temperature T){ return test_OptionalSpectralField(nlo,nhi,p,T); });
    std::function<double(compound::field::SpectralParameters, const double, const double)> f  = [](const compound::field::SpectralParameters params, const double first, const double second){ return first + second; };

    SECTION("A function will have the commutativity property if it is passed a function that itself has the commutativity property (this presumes for our case that the function is invoked)")
    {
        CHECK(unknown.value_or(f, unknown,  unknown) == unknown.value_or(f, unknown, unknown  ));
        CHECK(unknown.value_or(f, constant, unknown) == unknown.value_or(f, unknown, constant ));
        CHECK(unknown.value_or(f, sample,   unknown) == unknown.value_or(f, unknown, sample   ));
        CHECK(unknown.value_or(f, relation, unknown) == unknown.value_or(f, unknown, relation ));

        CHECK(unknown.value_or(f, unknown,  constant) == unknown.value_or(f, constant, unknown  ));
        CHECK(unknown.value_or(f, constant, constant) == unknown.value_or(f, constant, constant ));
        CHECK(unknown.value_or(f, sample,   constant) == unknown.value_or(f, constant, sample   ));
        CHECK(unknown.value_or(f, relation, constant) == unknown.value_or(f, constant, relation ));

        CHECK(unknown.value_or(f, unknown,  sample) == unknown.value_or(f, sample, unknown  ));
        CHECK(unknown.value_or(f, constant, sample) == unknown.value_or(f, sample, constant ));
        CHECK(unknown.value_or(f, sample,   sample) == unknown.value_or(f, sample, sample   ));
        CHECK(unknown.value_or(f, relation, sample) == unknown.value_or(f, sample, relation ));

        CHECK(unknown.value_or(f, unknown,  relation) == unknown.value_or(f, relation, unknown  ));
        CHECK(unknown.value_or(f, constant, relation) == unknown.value_or(f, relation, constant ));
        CHECK(unknown.value_or(f, sample,   relation) == unknown.value_or(f, relation, sample   ));
        CHECK(unknown.value_or(f, relation, relation) == unknown.value_or(f, relation, relation ));
    }
}










