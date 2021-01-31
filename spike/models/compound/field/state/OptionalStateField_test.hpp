
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <si.hpp>
#include <si_test_utils.hpp>

#include "OptionalStateField.hpp"

double test_ideal_gas_law_optional(const si::pressure p, const si::temperature T)
{
	return si::unitless(si::mole*si::universal_gas_constant*T/p/si::liter);
}

TEST_CASE( "OptionalStateField substitute() purity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.substitute(unknown )(p, T) 
    	   == unknown.substitute(unknown )(p, T));
    	CHECK(constant.substitute(constant)(p, T) 
    	   == constant.substitute(constant)(p, T));
    	CHECK(sample.substitute(sample  )(p, T) 
    	   == sample.substitute(sample  )(p, T));
    	CHECK(relation.substitute(relation)(p, T) 
    	   == relation.substitute(relation)(p, T));
    }
}

TEST_CASE( "OptionalStateField substitute() identity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(unknown.substitute(unknown)(p, T) == unknown(p, T));
    	CHECK(constant.substitute(unknown)(p, T) == constant(p, T));
    	CHECK(sample.substitute(unknown)(p, T) == sample(p, T));
    	CHECK(relation.substitute(unknown)(p, T) == relation(p, T));
    }
}

TEST_CASE( "OptionalStateField substitute() associativity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Functions can be applied in any order and still produce the same results")
	{
    	CHECK(unknown.substitute(constant.substitute(sample))(p, T) == 
    		  unknown.substitute(constant).substitute(sample)(p, T));

    	CHECK(unknown.substitute(sample.substitute(constant))(p, T) == 
    		  unknown.substitute(sample).substitute(constant)(p, T));


    	CHECK(unknown.substitute(constant.substitute(sample))(p, T) == 
    		  unknown.substitute(constant).substitute(sample)(p, T));

    	CHECK(unknown.substitute(sample.substitute(constant))(p, T) == 
    		  unknown.substitute(sample).substitute(constant)(p, T));





    	CHECK(constant.substitute(sample.substitute(unknown))(p, T) == 
    		  constant.substitute(sample).substitute(unknown)(p, T));

    	CHECK(constant.substitute(unknown.substitute(sample))(p, T) == 
    		  constant.substitute(unknown).substitute(sample)(p, T));


    	CHECK(constant.substitute(sample.substitute(unknown))(p, T) == 
    		  constant.substitute(sample).substitute(unknown)(p, T));

    	CHECK(constant.substitute(unknown.substitute(sample))(p, T) == 
    		  constant.substitute(unknown).substitute(sample)(p, T));

    }
}


TEST_CASE( "OptionalStateField substitute() increasing", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });

	SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
	{

    	CHECK(unknown.substitute(unknown).index() >= unknown.index());
    	CHECK(unknown.substitute(constant).index() >= unknown.index());
    	CHECK(unknown.substitute(sample).index() >= unknown.index());
    	CHECK(unknown.substitute(relation).index() >= unknown.index());


    	CHECK(constant.substitute(unknown).index() >= constant.index());
    	CHECK(constant.substitute(constant).index() >= constant.index());
    	CHECK(constant.substitute(sample).index() >= constant.index());
    	CHECK(constant.substitute(relation).index() >= constant.index());


    	CHECK(sample.substitute(unknown).index() >= sample.index());
    	CHECK(sample.substitute(constant).index() >= sample.index());
    	CHECK(sample.substitute(sample).index() >= sample.index());
    	CHECK(sample.substitute(relation).index() >= sample.index());


    	CHECK(relation.substitute(unknown).index() >= relation.index());
    	CHECK(relation.substitute(constant).index() >= relation.index());
    	CHECK(relation.substitute(sample).index() >= relation.index());
    	CHECK(relation.substitute(relation).index() >= relation.index());

    }
}








TEST_CASE( "OptionalStateField compare() purity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.compare(unknown )(p, T) 
    	   == unknown.compare(unknown )(p, T));
    	CHECK(constant.compare(constant)(p, T) 
    	   == constant.compare(constant)(p, T));
    	CHECK(sample.compare(sample  )(p, T) 
    	   == sample.compare(sample  )(p, T));
    	CHECK(relation.compare(relation)(p, T) 
    	   == relation.compare(relation)(p, T));
    }
}

TEST_CASE( "OptionalStateField compare() identity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(unknown.compare(unknown)(p, T) == unknown(p, T));
    	CHECK(constant.compare(unknown)(p, T) == constant(p, T));
    	CHECK(sample.compare(unknown)(p, T) == sample(p, T));
    	CHECK(relation.compare(unknown)(p, T) == relation(p, T));
    }
}

TEST_CASE( "OptionalStateField compare() associativity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Functions can be applied in any order and still produce the same results")
	{
    	CHECK(unknown.compare(constant.compare(sample))(p, T) == 
    		  unknown.compare(constant).compare(sample)(p, T));

    	CHECK(unknown.compare(sample.compare(constant))(p, T) == 
    		  unknown.compare(sample).compare(constant)(p, T));


    	CHECK(unknown.compare(constant.compare(sample))(p, T) == 
    		  unknown.compare(constant).compare(sample)(p, T));

    	CHECK(unknown.compare(sample.compare(constant))(p, T) == 
    		  unknown.compare(sample).compare(constant)(p, T));





    	CHECK(constant.compare(sample.compare(unknown))(p, T) == 
    		  constant.compare(sample).compare(unknown)(p, T));

    	CHECK(constant.compare(unknown.compare(sample))(p, T) == 
    		  constant.compare(unknown).compare(sample)(p, T));


    	CHECK(constant.compare(sample.compare(unknown))(p, T) == 
    		  constant.compare(sample).compare(unknown)(p, T));

    	CHECK(constant.compare(unknown.compare(sample))(p, T) == 
    		  constant.compare(unknown).compare(sample)(p, T));

    }
}


TEST_CASE( "OptionalStateField compare() increasing", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });

	SECTION("An attribute of a function's return value either increases or remains the same when compared to the same attribute of the input value")
	{

    	CHECK(unknown.compare(unknown).index() >= unknown.index());
    	CHECK(unknown.compare(constant).index() >= unknown.index());
    	CHECK(unknown.compare(sample).index() >= unknown.index());
    	CHECK(unknown.compare(relation).index() >= unknown.index());


    	CHECK(constant.compare(unknown).index() >= constant.index());
    	CHECK(constant.compare(constant).index() >= constant.index());
    	CHECK(constant.compare(sample).index() >= constant.index());
    	CHECK(constant.compare(relation).index() >= constant.index());


    	CHECK(sample.compare(unknown).index() >= sample.index());
    	CHECK(sample.compare(constant).index() >= sample.index());
    	CHECK(sample.compare(sample).index() >= sample.index());
    	CHECK(sample.compare(relation).index() >= sample.index());


    	CHECK(relation.compare(unknown).index() >= relation.index());
    	CHECK(relation.compare(constant).index() >= relation.index());
    	CHECK(relation.compare(sample).index() >= relation.index());
    	CHECK(relation.compare(relation).index() >= relation.index());

    }
}

TEST_CASE( "OptionalStateField best() commutativity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Arguments to a function can be swapped and still produce the same results")
	{

    	CHECK(unknown.compare(unknown)(p, T) == unknown.compare(unknown)(p, T));
    	CHECK(unknown.compare(constant)(p, T) == constant.compare(unknown)(p, T));
    	CHECK(unknown.compare(sample)(p, T) == sample.compare(unknown)(p, T));
    	CHECK(unknown.compare(relation)(p, T) == relation.compare(unknown)(p, T));


    	CHECK(constant.compare(unknown)(p, T) == unknown.compare(constant)(p, T));
    	CHECK(constant.compare(constant)(p, T) == constant.compare(constant)(p, T));
    	CHECK(constant.compare(sample)(p, T) == sample.compare(constant)(p, T));
    	CHECK(constant.compare(relation)(p, T) == relation.compare(constant)(p, T));


    	CHECK(sample.compare(unknown)(p, T) == unknown.compare(sample)(p, T));
    	CHECK(sample.compare(constant)(p, T) == constant.compare(sample)(p, T));
    	CHECK(sample.compare(sample)(p, T) == sample.compare(sample)(p, T));
    	CHECK(sample.compare(relation)(p, T) == relation.compare(sample)(p, T));


    	CHECK(relation.compare(unknown)(p, T) == unknown.compare(relation)(p, T));
    	CHECK(relation.compare(constant)(p, T) == constant.compare(relation)(p, T));
    	CHECK(relation.compare(sample)(p, T) == sample.compare(relation)(p, T));
    	CHECK(relation.compare(relation)(p, T) == relation.compare(relation)(p, T));

    }
}









TEST_CASE( "OptionalStateField map() purity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	std::function<double(const double)> f  = [](const double value){ return 1.0 - 2.0*value; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.map(f)(p, T) == unknown.map(f)(p, T));
    	CHECK(constant.map(f)(p, T) == constant.map(f)(p, T));
    	CHECK(sample.map(f)(p, T) == sample.map(f)(p, T));
    	CHECK(relation.map(f)(p, T) == relation.map(f)(p, T));
    }
}

TEST_CASE( "OptionalStateField map() identity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	std::function<double(const double)> I  = [](const double value){ return value; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(unknown.map(I)(p, T) == unknown(p, T));
    	CHECK(constant.map(I)(p, T) == constant(p, T));
    	CHECK(sample.map(I)(p, T) == sample(p, T));
    	CHECK(relation.map(I)(p, T) == relation(p, T));
    }
}









TEST_CASE( "OptionalStateField map_to_constant() purity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	std::function<double(const double, const si::pressure, const si::temperature)> f  = 
		[](const double value, const si::pressure p, const si::temperature T){ return 1.0 - 2.0*value; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.map_to_constant(p,T,f) == unknown.map_to_constant(p,T,f));
    	CHECK(constant.map_to_constant(p,T,f) == constant.map_to_constant(p,T,f));
    	CHECK(sample.map_to_constant(p,T,f) == sample.map_to_constant(p,T,f));
    	CHECK(relation.map_to_constant(p,T,f) == relation.map_to_constant(p,T,f));
    }
}

TEST_CASE( "OptionalStateField map_to_constant() identity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	std::function<double(const double, const si::pressure, const si::temperature)> I  = 
		[](const double value, const si::pressure p, const si::temperature T){ return value; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a value that when applied to a function returns the original value")
	{
    	CHECK(unknown.map_to_constant(p,T,I)  == unknown(p,T) );
    	CHECK(constant.map_to_constant(p,T,I)  == constant(p,T) );
    	CHECK(sample.map_to_constant(p,T,I)  == sample(p,T) );
    	CHECK(relation.map_to_constant(p,T,I)  == relation(p,T) );
    }
}