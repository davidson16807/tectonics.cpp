
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "CompletedStateField.hpp"
#include "CompletedStateField_test_utils.hpp"
















TEST_CASE( "CompletedStateField map() purity", "[field]" ) {
	compound::field::CompletedStateField<double> constant  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return 1.0; });
	compound::field::CompletedStateField<double> sample  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return 2.0; });
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
	compound::field::CompletedStateField<double> constant  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return 1.0; });
	compound::field::CompletedStateField<double> sample  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return 2.0; });
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
	compound::field::CompletedStateField<double> constant  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return 1.0; });
	compound::field::CompletedStateField<double> sample  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return 2.0; });
	compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });
	std::function<double(const double, const si::pressure, const si::temperature)> f  = 
		[](const double entry, const si::pressure p, const si::temperature T){ return 1.0 - 2.0*entry; };
	si::pressure p = si::standard_pressure;
	si::temperature T = si::standard_temperature;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(constant(p,T) == constant(p,T));
    	CHECK(sample(p,T) == sample(p,T));
    	CHECK(relation(p,T) == relation(p,T));
    }
}

TEST_CASE( "CompletedStateField map_to_constant() identity", "[field]" ) {
    compound::field::CompletedStateField<double> constant  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return 1.0; });
    compound::field::CompletedStateField<double> sample  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return 2.0; });
    compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });
    std::function<double(const double, const si::pressure, const si::temperature)> I  = 
        [](const double entry, const si::pressure p, const si::temperature T){ return entry; };
    si::pressure p = si::standard_pressure;
    si::temperature T = si::standard_temperature;

    SECTION("There exists a entry that when applied to a function returns the original entry")
    {
        CHECK(constant(p,T)  == constant(p,T) );
        CHECK(sample(p,T)  == sample(p,T) );
        CHECK(relation(p,T)  == relation(p,T) );
    }
}

TEST_CASE( "CompletedStateField function() consistency", "[field]" ) {
    compound::field::CompletedStateField<double> constant  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return 1.0; });
    compound::field::CompletedStateField<double> sample  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return 2.0; });
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