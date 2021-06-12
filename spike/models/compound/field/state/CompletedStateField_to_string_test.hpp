
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <units/si_test_utils.hpp>

#include "CompletedStateField.hpp"
#include "CompletedStateField_to_string.hpp"

TEST_CASE( "CompletedStateField to_string() purity", "[field]" ) {
	compound::field::CompletedStateField<double> unit  = 1.0;
	compound::field::CompletedStateField<double> unit_nudged  = 1.01;
	compound::field::CompletedStateField<si::mass> unit_mass  = 1.0 * si::kilogram;
	compound::field::CompletedStateField<si::length> unit_length  = 1.0 * si::meter;
	compound::field::CompletedStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_completed(p,T); });

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::field::to_string(unit) 
    	   == compound::field::to_string(unit));
    	CHECK(compound::field::to_string(unit_nudged) 
    	   == compound::field::to_string(unit_nudged));
    	CHECK(compound::field::to_string(unit_mass) 
    	   == compound::field::to_string(unit_mass));
    	CHECK(compound::field::to_string(unit_length) 
    	   == compound::field::to_string(unit_length));
    	CHECK(compound::field::to_string(sample  ) 
    	   == compound::field::to_string(sample  ));
    	CHECK(compound::field::to_string(relation) 
    	   == compound::field::to_string(relation));
    }
}

TEST_CASE( "CompletedStateField to_string() appreciable difference preservation", "[field]" ) {
	compound::field::CompletedStateField<double> unit  = 1.0;
	compound::field::CompletedStateField<double> unit_nudged  = 1.01;
	compound::field::CompletedStateField<si::mass> unit_mass  = 1.0 * si::kilogram;
	compound::field::CompletedStateField<si::length> unit_length  = 1.0 * si::meter;

	SECTION("Appreciable differences in input are preserved within output")
	{
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_nudged));
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_mass));
    	CHECK(compound::field::to_string(unit_mass) != compound::field::to_string(unit_length));
    }
}
