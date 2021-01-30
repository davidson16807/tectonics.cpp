
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <si.hpp>
#include <si_test_utils.hpp>

#include "OptionalStateRecord.hpp"
#include "OptionalStateRecord_operators.hpp"

double test_ideal_gas_law(const si::pressure p, const si::temperature T)
{
	return si::unitless(si::mole*si::universal_gas_constant*T/p/si::liter);
}

TEST_CASE( "OptionalStateRecord substitute() purity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::record::value(compound::record::substitute(unknown,  unknown ), p, T) 
    	   == compound::record::value(compound::record::substitute(unknown,  unknown ), p, T));
    	CHECK(compound::record::value(compound::record::substitute(constant, constant), p, T) 
    	   == compound::record::value(compound::record::substitute(constant, constant), p, T));
    	CHECK(compound::record::value(compound::record::substitute(sample,   sample  ), p, T) 
    	   == compound::record::value(compound::record::substitute(sample,   sample  ), p, T));
    	CHECK(compound::record::value(compound::record::substitute(relation, relation), p, T) 
    	   == compound::record::value(compound::record::substitute(relation, relation), p, T));
    }
}

TEST_CASE( "OptionalStateRecord substitute() identity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(compound::record::value(compound::record::substitute(unknown,  unknown), p, T) == compound::record::value(unknown,  p, T));
    	CHECK(compound::record::value(compound::record::substitute(constant, unknown), p, T) == compound::record::value(constant, p, T));
    	CHECK(compound::record::value(compound::record::substitute(sample,   unknown), p, T) == compound::record::value(sample,   p, T));
    	CHECK(compound::record::value(compound::record::substitute(relation, unknown), p, T) == compound::record::value(relation, p, T));
    }
}

TEST_CASE( "OptionalStateRecord substitute() associativity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Functions can be applied in any order and still produce the same results")
	{
    	CHECK(compound::record::value(compound::record::substitute(unknown, compound::record::substitute(constant, sample)), p, T) == 
    		  compound::record::value(compound::record::substitute(compound::record::substitute(unknown, constant), sample),  p, T));

    	CHECK(compound::record::value(compound::record::substitute(unknown, compound::record::substitute(sample, constant)), p, T) == 
    		  compound::record::value(compound::record::substitute(compound::record::substitute(unknown, sample), constant),  p, T));


    	CHECK(compound::record::value(compound::record::substitute(unknown, compound::record::substitute(constant, sample)), p, T) == 
    		  compound::record::value(compound::record::substitute(compound::record::substitute(unknown, constant), sample),  p, T));

    	CHECK(compound::record::value(compound::record::substitute(unknown, compound::record::substitute(sample, constant)), p, T) == 
    		  compound::record::value(compound::record::substitute(compound::record::substitute(unknown, sample), constant),  p, T));





    	CHECK(compound::record::value(compound::record::substitute(constant, compound::record::substitute(sample, unknown)), p, T) == 
    		  compound::record::value(compound::record::substitute(compound::record::substitute(constant, sample), unknown),  p, T));

    	CHECK(compound::record::value(compound::record::substitute(constant, compound::record::substitute(unknown, sample)), p, T) == 
    		  compound::record::value(compound::record::substitute(compound::record::substitute(constant, unknown), sample),  p, T));


    	CHECK(compound::record::value(compound::record::substitute(constant, compound::record::substitute(sample, unknown)), p, T) == 
    		  compound::record::value(compound::record::substitute(compound::record::substitute(constant, sample), unknown),  p, T));

    	CHECK(compound::record::value(compound::record::substitute(constant, compound::record::substitute(unknown, sample)), p, T) == 
    		  compound::record::value(compound::record::substitute(compound::record::substitute(constant, unknown), sample),  p, T));

    }
}


TEST_CASE( "OptionalStateRecord substitute() increasing", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };

	SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
	{

    	CHECK(compound::record::substitute(unknown, unknown).index() >= unknown.index());
    	CHECK(compound::record::substitute(unknown, constant).index() >= unknown.index());
    	CHECK(compound::record::substitute(unknown, sample).index() >= unknown.index());
    	CHECK(compound::record::substitute(unknown, relation).index() >= unknown.index());


    	CHECK(compound::record::substitute(constant, unknown).index() >= constant.index());
    	CHECK(compound::record::substitute(constant, constant).index() >= constant.index());
    	CHECK(compound::record::substitute(constant, sample).index() >= constant.index());
    	CHECK(compound::record::substitute(constant, relation).index() >= constant.index());


    	CHECK(compound::record::substitute(sample, unknown).index() >= sample.index());
    	CHECK(compound::record::substitute(sample, constant).index() >= sample.index());
    	CHECK(compound::record::substitute(sample, sample).index() >= sample.index());
    	CHECK(compound::record::substitute(sample, relation).index() >= sample.index());


    	CHECK(compound::record::substitute(relation, unknown).index() >= relation.index());
    	CHECK(compound::record::substitute(relation, constant).index() >= relation.index());
    	CHECK(compound::record::substitute(relation, sample).index() >= relation.index());
    	CHECK(compound::record::substitute(relation, relation).index() >= relation.index());

    }
}








TEST_CASE( "OptionalStateRecord best() purity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::record::value(compound::record::best(unknown,  unknown ), p, T) 
    	   == compound::record::value(compound::record::best(unknown,  unknown ), p, T));
    	CHECK(compound::record::value(compound::record::best(constant, constant), p, T) 
    	   == compound::record::value(compound::record::best(constant, constant), p, T));
    	CHECK(compound::record::value(compound::record::best(sample,   sample  ), p, T) 
    	   == compound::record::value(compound::record::best(sample,   sample  ), p, T));
    	CHECK(compound::record::value(compound::record::best(relation, relation), p, T) 
    	   == compound::record::value(compound::record::best(relation, relation), p, T));
    }
}

TEST_CASE( "OptionalStateRecord best() identity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(compound::record::value(compound::record::best(unknown,  unknown), p, T) == compound::record::value(unknown,  p, T));
    	CHECK(compound::record::value(compound::record::best(constant, unknown), p, T) == compound::record::value(constant, p, T));
    	CHECK(compound::record::value(compound::record::best(sample,   unknown), p, T) == compound::record::value(sample,   p, T));
    	CHECK(compound::record::value(compound::record::best(relation, unknown), p, T) == compound::record::value(relation, p, T));
    }
}

TEST_CASE( "OptionalStateRecord best() associativity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Functions can be applied in any order and still produce the same results")
	{
    	CHECK(compound::record::value(compound::record::best(unknown, compound::record::best(constant, sample)), p, T) == 
    		  compound::record::value(compound::record::best(compound::record::best(unknown, constant), sample),  p, T));

    	CHECK(compound::record::value(compound::record::best(unknown, compound::record::best(sample, constant)), p, T) == 
    		  compound::record::value(compound::record::best(compound::record::best(unknown, sample), constant),  p, T));


    	CHECK(compound::record::value(compound::record::best(unknown, compound::record::best(constant, sample)), p, T) == 
    		  compound::record::value(compound::record::best(compound::record::best(unknown, constant), sample),  p, T));

    	CHECK(compound::record::value(compound::record::best(unknown, compound::record::best(sample, constant)), p, T) == 
    		  compound::record::value(compound::record::best(compound::record::best(unknown, sample), constant),  p, T));





    	CHECK(compound::record::value(compound::record::best(constant, compound::record::best(sample, unknown)), p, T) == 
    		  compound::record::value(compound::record::best(compound::record::best(constant, sample), unknown),  p, T));

    	CHECK(compound::record::value(compound::record::best(constant, compound::record::best(unknown, sample)), p, T) == 
    		  compound::record::value(compound::record::best(compound::record::best(constant, unknown), sample),  p, T));


    	CHECK(compound::record::value(compound::record::best(constant, compound::record::best(sample, unknown)), p, T) == 
    		  compound::record::value(compound::record::best(compound::record::best(constant, sample), unknown),  p, T));

    	CHECK(compound::record::value(compound::record::best(constant, compound::record::best(unknown, sample)), p, T) == 
    		  compound::record::value(compound::record::best(compound::record::best(constant, unknown), sample),  p, T));

    }
}

TEST_CASE( "OptionalStateRecord best() commutativity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Arguments to a function can be swapped and still produce the same results")
	{

    	CHECK(compound::record::value(compound::record::best(unknown, unknown),  p, T) == compound::record::value(compound::record::best(unknown, unknown),   p, T));
    	CHECK(compound::record::value(compound::record::best(unknown, constant), p, T) == compound::record::value(compound::record::best(constant, unknown),  p, T));
    	CHECK(compound::record::value(compound::record::best(unknown, sample),   p, T) == compound::record::value(compound::record::best(sample, unknown),    p, T));
    	CHECK(compound::record::value(compound::record::best(unknown, relation), p, T) == compound::record::value(compound::record::best(relation, unknown),  p, T));


    	CHECK(compound::record::value(compound::record::best(constant, unknown), p, T) == compound::record::value(compound::record::best(unknown, constant),  p, T));
    	CHECK(compound::record::value(compound::record::best(constant, constant),p, T) == compound::record::value(compound::record::best(constant, constant), p, T));
    	CHECK(compound::record::value(compound::record::best(constant, sample),  p, T) == compound::record::value(compound::record::best(sample, constant),   p, T));
    	CHECK(compound::record::value(compound::record::best(constant, relation),p, T) == compound::record::value(compound::record::best(relation, constant), p, T));


    	CHECK(compound::record::value(compound::record::best(sample, unknown),   p, T) == compound::record::value(compound::record::best(unknown, sample),    p, T));
    	CHECK(compound::record::value(compound::record::best(sample, constant),  p, T) == compound::record::value(compound::record::best(constant, sample),   p, T));
    	CHECK(compound::record::value(compound::record::best(sample, sample),    p, T) == compound::record::value(compound::record::best(sample, sample),     p, T));
    	CHECK(compound::record::value(compound::record::best(sample, relation),  p, T) == compound::record::value(compound::record::best(relation, sample),   p, T));


    	CHECK(compound::record::value(compound::record::best(relation, unknown), p, T) == compound::record::value(compound::record::best(unknown, relation),  p, T));
    	CHECK(compound::record::value(compound::record::best(relation, constant),p, T) == compound::record::value(compound::record::best(constant, relation), p, T));
    	CHECK(compound::record::value(compound::record::best(relation, sample),  p, T) == compound::record::value(compound::record::best(sample, relation),   p, T));
    	CHECK(compound::record::value(compound::record::best(relation, relation),p, T) == compound::record::value(compound::record::best(relation, relation), p, T));

    }
}

TEST_CASE( "OptionalStateRecord best() increasing", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };

	SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
	{

    	CHECK(compound::record::best(unknown, unknown).index() >= unknown.index());
    	CHECK(compound::record::best(unknown, constant).index() >= unknown.index());
    	CHECK(compound::record::best(unknown, sample).index() >= unknown.index());
    	CHECK(compound::record::best(unknown, relation).index() >= unknown.index());


    	CHECK(compound::record::best(constant, unknown).index() >= constant.index());
    	CHECK(compound::record::best(constant, constant).index() >= constant.index());
    	CHECK(compound::record::best(constant, sample).index() >= constant.index());
    	CHECK(compound::record::best(constant, relation).index() >= constant.index());


    	CHECK(compound::record::best(sample, unknown).index() >= sample.index());
    	CHECK(compound::record::best(sample, constant).index() >= sample.index());
    	CHECK(compound::record::best(sample, sample).index() >= sample.index());
    	CHECK(compound::record::best(sample, relation).index() >= sample.index());


    	CHECK(compound::record::best(relation, unknown).index() >= relation.index());
    	CHECK(compound::record::best(relation, constant).index() >= relation.index());
    	CHECK(compound::record::best(relation, sample).index() >= relation.index());
    	CHECK(compound::record::best(relation, relation).index() >= relation.index());

    }
}









TEST_CASE( "OptionalStateRecord map() purity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };
	std::function<double(const double)> f  = [](const double value){ return 1.0 - 2.0*value; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::record::value(compound::record::map(unknown,  f), p, T) 
    	   == compound::record::value(compound::record::map(unknown,  f), p, T));
    	CHECK(compound::record::value(compound::record::map(constant, f), p, T) 
    	   == compound::record::value(compound::record::map(constant, f), p, T));
    	CHECK(compound::record::value(compound::record::map(sample,   f), p, T) 
    	   == compound::record::value(compound::record::map(sample,   f), p, T));
    	CHECK(compound::record::value(compound::record::map(relation, f), p, T) 
    	   == compound::record::value(compound::record::map(relation, f), p, T));
    }
}

TEST_CASE( "OptionalStateRecord map() identity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };
	std::function<double(const double)> I  = [](const double value){ return value; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(compound::record::value(compound::record::map(unknown,  I), p, T) == compound::record::value(unknown,  p, T));
    	CHECK(compound::record::value(compound::record::map(constant, I), p, T) == compound::record::value(constant, p, T));
    	CHECK(compound::record::value(compound::record::map(sample,   I), p, T) == compound::record::value(sample,   p, T));
    	CHECK(compound::record::value(compound::record::map(relation, I), p, T) == compound::record::value(relation, p, T));
    }
}









TEST_CASE( "OptionalStateRecord map_to_constant() purity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };
	std::function<double(const double, const si::pressure, const si::temperature)> f  = 
		[](const double value, const si::pressure p, const si::temperature T){ return 1.0 - 2.0*value; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::record::map_to_constant(unknown,  p, T, f) 
    	   == compound::record::map_to_constant(unknown,  p, T, f));
    	CHECK(compound::record::map_to_constant(constant, p, T, f) 
    	   == compound::record::map_to_constant(constant, p, T, f));
    	CHECK(compound::record::map_to_constant(sample,   p, T, f) 
    	   == compound::record::map_to_constant(sample,   p, T, f));
    	CHECK(compound::record::map_to_constant(relation, p, T, f) 
    	   == compound::record::map_to_constant(relation, p, T, f));
    }
}

TEST_CASE( "OptionalStateRecord map_to_constant() identity", "[record]" ) {
	compound::record::OptionalStateRecord<double> unknown  = std::monostate();
	compound::record::OptionalStateRecord<double> constant  = 1.0;
	compound::record::OptionalStateRecord<double> sample  = compound::record::StateSampleRecord<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::record::OptionalStateRecord<double> relation  = [](const si::pressure p, const si::temperature T){ return test_ideal_gas_law(p,T); };
	std::function<double(const double, const si::pressure, const si::temperature)> I  = 
		[](const double value, const si::pressure p, const si::temperature T){ return value; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(compound::record::map_to_constant(unknown,  p, T, I)  == compound::record::value(unknown,  p,T) );
    	CHECK(compound::record::map_to_constant(constant, p, T, I)  == compound::record::value(constant, p,T) );
    	CHECK(compound::record::map_to_constant(sample,   p, T, I)  == compound::record::value(sample,   p,T) );
    	CHECK(compound::record::map_to_constant(relation, p, T, I)  == compound::record::value(relation, p,T) );
    }
}