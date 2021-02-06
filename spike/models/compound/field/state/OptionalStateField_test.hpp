
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <si_test_utils.hpp>

#include "OptionalStateField.hpp"

double test_ideal_gas_law_optional(const si::pressure p, const si::temperature T)
{
	return si::unitless(si::mole*si::universal_gas_constant*T/p/si::liter);
}

TEST_CASE( "OptionalStateField value_or() purity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.value_or(unknown )(p, T) 
    	   == unknown.value_or(unknown )(p, T));
    	CHECK(constant.value_or(constant)(p, T) 
    	   == constant.value_or(constant)(p, T));
    	CHECK(sample.value_or(sample  )(p, T) 
    	   == sample.value_or(sample  )(p, T));
    	CHECK(relation.value_or(relation)(p, T) 
    	   == relation.value_or(relation)(p, T));
    }
}

TEST_CASE( "OptionalStateField value_or() identity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a entry that when applied to a function returns the original entry")
	{
    	CHECK(unknown.value_or(unknown)(p, T) == unknown(p, T));
    	CHECK(constant.value_or(unknown)(p, T) == constant(p, T));
    	CHECK(sample.value_or(unknown)(p, T) == sample(p, T));
    	CHECK(relation.value_or(unknown)(p, T) == relation(p, T));
    }
}

TEST_CASE( "OptionalStateField value_or() associativity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Functions can be applied in any order and still produce the same results")
	{
    	CHECK(unknown.value_or(constant.value_or(sample))(p, T) == 
    		  unknown.value_or(constant).value_or(sample)(p, T));

    	CHECK(unknown.value_or(sample.value_or(constant))(p, T) == 
    		  unknown.value_or(sample).value_or(constant)(p, T));


    	CHECK(unknown.value_or(constant.value_or(sample))(p, T) == 
    		  unknown.value_or(constant).value_or(sample)(p, T));

    	CHECK(unknown.value_or(sample.value_or(constant))(p, T) == 
    		  unknown.value_or(sample).value_or(constant)(p, T));





    	CHECK(constant.value_or(sample.value_or(unknown))(p, T) == 
    		  constant.value_or(sample).value_or(unknown)(p, T));

    	CHECK(constant.value_or(unknown.value_or(sample))(p, T) == 
    		  constant.value_or(unknown).value_or(sample)(p, T));


    	CHECK(constant.value_or(sample.value_or(unknown))(p, T) == 
    		  constant.value_or(sample).value_or(unknown)(p, T));

    	CHECK(constant.value_or(unknown.value_or(sample))(p, T) == 
    		  constant.value_or(unknown).value_or(sample)(p, T));

    }
}


TEST_CASE( "OptionalStateField value_or() increasing", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> constant  = 1.0;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });

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

	SECTION("There exists a entry that when applied to a function returns the original entry")
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

	SECTION("An attribute of a function's return entry either increases or remains the same when compared to the same attribute of the input entry")
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
	std::function<double(const double)> f  = [](const double entry){ return 1.0 - 2.0*entry; };
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
	std::function<double(const double)> I  = [](const double entry){ return entry; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a entry that when applied to a function returns the original entry")
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
		[](const double entry, const si::pressure p, const si::temperature T){ return 1.0 - 2.0*entry; };
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
		[](const double entry, const si::pressure p, const si::temperature T){ return entry; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a entry that when applied to a function returns the original entry")
	{
    	CHECK(unknown.map_to_constant(p,T,I)  == unknown(p,T) );
    	CHECK(constant.map_to_constant(p,T,I)  == constant(p,T) );
    	CHECK(sample.map_to_constant(p,T,I)  == sample(p,T) );
    	CHECK(relation.map_to_constant(p,T,I)  == relation(p,T) );
    }
}