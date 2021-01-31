
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <si.hpp>
#include <si_test_utils.hpp>

#include "OptionalStateField.hpp"
#include "OptionalStateField_to_CompletedStateField.hpp"

double test_ideal_gas_law_optional2(const si::pressure p, const si::temperature T)
{
	return si::unitless(si::mole*si::universal_gas_constant*T/p/si::liter);
}

TEST_CASE( "OptionalStateField complete() purity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
    compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
    compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional2(p,T); });

    compound::field::CompletedStateField<double> known_constant  = 1.0;
    compound::field::CompletedStateField<double> known_sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
    compound::field::CompletedStateField<double> known_relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional2(p,T); });

	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::field::complete(unknown,known_constant )(p, T) == compound::field::complete(unknown,known_constant )(p, T));
    	CHECK(compound::field::complete(constant,known_constant)(p, T) == compound::field::complete(constant,known_constant)(p, T));
    	CHECK(compound::field::complete(sample,known_sample  )(p, T) == compound::field::complete(sample,known_sample  )(p, T));
    	CHECK(compound::field::complete(relation,known_relation)(p, T) == compound::field::complete(relation,known_relation)(p, T));
    }
}


TEST_CASE( "OptionalStateField complete() associativity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);

    compound::field::CompletedStateField<double> known_constant  = 1.0;
    compound::field::CompletedStateField<double> known_sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
    compound::field::CompletedStateField<double> known_relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional2(p,T); });

	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Functions can be applied in any order and still produce the same results")
	{
    	CHECK(compound::field::complete(unknown, compound::field::complete(constant,known_sample))(p, T) == 
    		  compound::field::complete(unknown,known_constant)(p, T));

    	CHECK(compound::field::complete(unknown, compound::field::complete(sample,known_constant))(p, T) == 
    		  compound::field::complete(unknown,known_sample)(p, T));


    	CHECK(compound::field::complete(unknown, compound::field::complete(constant,known_sample))(p, T) == 
    		  compound::field::complete(unknown,known_constant)(p, T));

    	CHECK(compound::field::complete(unknown, compound::field::complete(sample,known_constant))(p, T) == 
    		  compound::field::complete(unknown,known_sample)(p, T));



    	CHECK(compound::field::complete(constant, compound::field::complete(sample,known_constant))(p, T) == 
    		  compound::field::complete(constant,known_sample)(p, T));

    	CHECK(compound::field::complete(constant, compound::field::complete(unknown,known_sample))(p, T) == 
    		  compound::field::complete(constant,known_constant)(p, T));


    	CHECK(compound::field::complete(constant, compound::field::complete(sample,known_constant))(p, T) == 
    		  compound::field::complete(constant,known_sample)(p, T));

    	CHECK(compound::field::complete(constant, compound::field::complete(unknown,known_sample))(p, T) == 
    		  compound::field::complete(constant,known_constant)(p, T));

    }
}


TEST_CASE( "OptionalStateField complete() increasing", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional2(p,T); });

    compound::field::CompletedStateField<double> known_constant  = 1.0;
    compound::field::CompletedStateField<double> known_sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
    compound::field::CompletedStateField<double> known_relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional2(p,T); });

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






