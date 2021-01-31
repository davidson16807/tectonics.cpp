
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <si.hpp>
#include <si_test_utils.hpp>

#include "OptionalStateRecord.hpp"
#include "OptionalStateRecord_to_CompletedStateRecord.hpp"

double test_ideal_gas_law_optional2(const si::pressure p, const si::temperature T)
{
	return si::unitless(si::mole*si::universal_gas_constant*T/p/si::liter);
}

TEST_CASE( "OptionalStateRecord complete() purity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
    compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
    compound::record::OptionalStateRecord<double> relation  = compound::record::AuthoritativeStateRecord<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional2(p,T); });

    compound::record::CompletedStateRecord<double> known_constant  = 1.0;
    compound::record::CompletedStateRecord<double> known_sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
    compound::record::CompletedStateRecord<double> known_relation  = compound::record::AuthoritativeStateRecord<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional2(p,T); });

	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::record::complete(unknown,known_constant )(p, T) == compound::record::complete(unknown,known_constant )(p, T));
    	CHECK(compound::record::complete(constant,known_constant)(p, T) == compound::record::complete(constant,known_constant)(p, T));
    	CHECK(compound::record::complete(sample,known_sample  )(p, T) == compound::record::complete(sample,known_sample  )(p, T));
    	CHECK(compound::record::complete(relation,known_relation)(p, T) == compound::record::complete(relation,known_relation)(p, T));
    }
}


TEST_CASE( "OptionalStateRecord complete() associativity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);

    compound::record::CompletedStateRecord<double> known_constant  = 1.0;
    compound::record::CompletedStateRecord<double> known_sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
    compound::record::CompletedStateRecord<double> known_relation  = compound::record::AuthoritativeStateRecord<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional2(p,T); });

	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Functions can be applied in any order and still produce the same results")
	{
    	CHECK(compound::record::complete(unknown, compound::record::complete(constant,known_sample))(p, T) == 
    		  compound::record::complete(unknown,known_constant)(p, T));

    	CHECK(compound::record::complete(unknown, compound::record::complete(sample,known_constant))(p, T) == 
    		  compound::record::complete(unknown,known_sample)(p, T));


    	CHECK(compound::record::complete(unknown, compound::record::complete(constant,known_sample))(p, T) == 
    		  compound::record::complete(unknown,known_constant)(p, T));

    	CHECK(compound::record::complete(unknown, compound::record::complete(sample,known_constant))(p, T) == 
    		  compound::record::complete(unknown,known_sample)(p, T));



    	CHECK(compound::record::complete(constant, compound::record::complete(sample,known_constant))(p, T) == 
    		  compound::record::complete(constant,known_sample)(p, T));

    	CHECK(compound::record::complete(constant, compound::record::complete(unknown,known_sample))(p, T) == 
    		  compound::record::complete(constant,known_constant)(p, T));


    	CHECK(compound::record::complete(constant, compound::record::complete(sample,known_constant))(p, T) == 
    		  compound::record::complete(constant,known_sample)(p, T));

    	CHECK(compound::record::complete(constant, compound::record::complete(unknown,known_sample))(p, T) == 
    		  compound::record::complete(constant,known_constant)(p, T));

    }
}


TEST_CASE( "OptionalStateRecord complete() increasing", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = compound::record::AuthoritativeStateRecord<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional2(p,T); });

    compound::record::CompletedStateRecord<double> known_constant  = 1.0;
    compound::record::CompletedStateRecord<double> known_sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
    compound::record::CompletedStateRecord<double> known_relation  = compound::record::AuthoritativeStateRecord<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional2(p,T); });

	SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
	{
    	CHECK(compound::record::complete(unknown,known_constant).index()+1 >= unknown.index());
    	CHECK(compound::record::complete(unknown,known_sample).index()+1 >= unknown.index());
    	CHECK(compound::record::complete(unknown,known_relation).index()+1 >= unknown.index());


    	CHECK(compound::record::complete(constant,known_constant).index()+1 >= constant.index());
    	CHECK(compound::record::complete(constant,known_sample).index()+1 >= constant.index());
    	CHECK(compound::record::complete(constant,known_relation).index()+1 >= constant.index());


    	CHECK(compound::record::complete(sample,known_constant).index()+1 >= sample.index());
    	CHECK(compound::record::complete(sample,known_sample).index()+1 >= sample.index());
    	CHECK(compound::record::complete(sample,known_relation).index()+1 >= sample.index());


    	CHECK(compound::record::complete(relation,known_constant).index()+1 >= relation.index());
    	CHECK(compound::record::complete(relation,known_sample).index()+1 >= relation.index());
    	CHECK(compound::record::complete(relation,known_relation).index()+1 >= relation.index());
    }
}






