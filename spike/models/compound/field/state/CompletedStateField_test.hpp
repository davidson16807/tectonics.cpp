
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <si.hpp>
#include <si_test_utils.hpp>

#include "CompletedStateField.hpp"

double test_ideal_gas_law_completed(const si::pressure p, const si::temperature T)
{
	return si::unitless(si::mole*si::universal_gas_constant*T/p/si::liter);
}









TEST_CASE( "CompletedStateField compare() purity", "[field]" ) {
	compound::field::CompletedStateField<double> constant  = 1.0;
	compound::field::CompletedStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(constant.compare(constant)(p, T) 
    	   == constant.compare(constant)(p, T));
    	CHECK(sample.compare(sample  )(p, T) 
    	   == sample.compare(sample  )(p, T));
    	CHECK(relation.compare(relation)(p, T) 
    	   == relation.compare(relation)(p, T));
    }
}


TEST_CASE( "CompletedStateField compare() associativity", "[field]" ) {
    compound::field::CompletedStateField<double> constant1  = 1.0;
	compound::field::CompletedStateField<double> constant2  = 2.0;
	compound::field::CompletedStateField<double> sample  = compound::field::StateSample<double>(3.0, si::standard_pressure, si::standard_temperature);
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Functions can be applied in any order and still produce the same results")
	{

        CHECK(constant1.compare(constant2.compare(sample))(p, T) == 
              constant1.compare(constant2).compare(sample)(p, T));

        CHECK(constant1.compare(sample.compare(constant2))(p, T) == 
              constant1.compare(sample).compare(constant2)(p, T));


        CHECK(constant1.compare(constant2.compare(sample))(p, T) == 
              constant1.compare(constant2).compare(sample)(p, T));

        CHECK(constant1.compare(sample.compare(constant2))(p, T) == 
              constant1.compare(sample).compare(constant2)(p, T));



        CHECK(constant2.compare(sample.compare(constant1))(p, T) == 
              constant2.compare(sample).compare(constant1)(p, T));

        CHECK(constant2.compare(constant1.compare(sample))(p, T) == 
              constant2.compare(constant1).compare(sample)(p, T));


        CHECK(constant2.compare(sample.compare(constant1))(p, T) == 
              constant2.compare(sample).compare(constant1)(p, T));

        CHECK(constant2.compare(constant1.compare(sample))(p, T) == 
              constant2.compare(constant1).compare(sample)(p, T));

    }
}


TEST_CASE( "CompletedStateField compare() increasing", "[field]" ) {
	compound::field::CompletedStateField<double> constant  = 1.0;
	compound::field::CompletedStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });

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

TEST_CASE( "CompletedStateField best() commutativity", "[field]" ) {
	compound::field::CompletedStateField<double> constant  = 1.0;
	compound::field::CompletedStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Arguments to a function can be swapped and still produce the same results")
	{



    	CHECK(constant.compare(constant)(p, T) == constant.compare(constant)(p, T));
    	CHECK(constant.compare(sample)(p, T) == sample.compare(constant)(p, T));
    	CHECK(constant.compare(relation)(p, T) == relation.compare(constant)(p, T));


    	CHECK(sample.compare(constant)(p, T) == constant.compare(sample)(p, T));
    	CHECK(sample.compare(sample)(p, T) == sample.compare(sample)(p, T));
    	CHECK(sample.compare(relation)(p, T) == relation.compare(sample)(p, T));


    	CHECK(relation.compare(constant)(p, T) == constant.compare(relation)(p, T));
    	CHECK(relation.compare(sample)(p, T) == sample.compare(relation)(p, T));
    	CHECK(relation.compare(relation)(p, T) == relation.compare(relation)(p, T));

    }
}









TEST_CASE( "CompletedStateField map() purity", "[field]" ) {
	compound::field::CompletedStateField<double> constant  = 1.0;
	compound::field::CompletedStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });
	std::function<double(const double)> f  = [](const double entry){ return 1.0 - 2.0*entry; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(constant.map(f)(p, T) == constant.map(f)(p, T));
    	CHECK(sample.map(f)(p, T) == sample.map(f)(p, T));
    	CHECK(relation.map(f)(p, T) == relation.map(f)(p, T));
    }
}

TEST_CASE( "CompletedStateField map() identity", "[field]" ) {
	compound::field::CompletedStateField<double> constant  = 1.0;
	compound::field::CompletedStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });
	std::function<double(const double)> I  = [](const double entry){ return entry; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("There exists a entry that when applied to a function returns the original entry")
	{
    	CHECK(constant.map(I)(p, T) == constant(p, T));
    	CHECK(sample.map(I)(p, T) == sample(p, T));
    	CHECK(relation.map(I)(p, T) == relation(p, T));
    }
}









TEST_CASE( "CompletedStateField map_to_constant() purity", "[field]" ) {
	compound::field::CompletedStateField<double> constant  = 1.0;
	compound::field::CompletedStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });
	std::function<double(const double, const si::pressure, const si::temperature)> f  = 
		[](const double entry, const si::pressure p, const si::temperature T){ return 1.0 - 2.0*entry; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(constant.map_to_constant(p,T,f) == constant.map_to_constant(p,T,f));
    	CHECK(sample.map_to_constant(p,T,f) == sample.map_to_constant(p,T,f));
    	CHECK(relation.map_to_constant(p,T,f) == relation.map_to_constant(p,T,f));
    }
}

TEST_CASE( "CompletedStateField map_to_constant() identity", "[field]" ) {
    compound::field::CompletedStateField<double> constant  = 1.0;
    compound::field::CompletedStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
    compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });
    std::function<double(const double, const si::pressure, const si::temperature)> I  = 
        [](const double entry, const si::pressure p, const si::temperature T){ return entry; };
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

    SECTION("There exists a entry that when applied to a function returns the original entry")
    {
        CHECK(constant.map_to_constant(p,T,I)  == constant(p,T) );
        CHECK(sample.map_to_constant(p,T,I)  == sample(p,T) );
        CHECK(relation.map_to_constant(p,T,I)  == relation(p,T) );
    }
}

TEST_CASE( "CompletedStateField function() consistency", "[field]" ) {
    compound::field::CompletedStateField<double> constant  = 1.0;
    compound::field::CompletedStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
    compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

    SECTION("field.function()(p,T) should return the same result as field(p,T)")
    {
        CHECK(constant.function()(p,T)  == constant(p,T) );
        CHECK(sample.function()(p,T)  == sample(p,T) );
        CHECK(relation.function()(p,T)  == relation(p,T) );
    }
}