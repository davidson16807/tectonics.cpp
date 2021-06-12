
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <units/si_test_utils.hpp>

#include "OptionalStateField.hpp"
#include "OptionalStateField_to_string.hpp"

TEST_CASE( "OptionalStateField to_string() purity", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> unit  = 1.0;
	compound::field::OptionalStateField<double> unit_nudged  = 1.01;
	compound::field::OptionalStateField<si::mass> unit_mass  = 1.0 * si::kilogram;
	compound::field::OptionalStateField<si::length> unit_length  = 1.0 * si::meter;
	compound::field::OptionalStateField<double> sample  = compound::field::StateSample<double>(2.0, si::standard_pressure, si::standard_temperature);
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::field::to_string(unknown ) 
    	   == compound::field::to_string(unknown ));
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

TEST_CASE( "OptionalStateField to_string() appreciable difference preservation", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> unit  = 1.0;
	compound::field::OptionalStateField<double> unit_nudged  = 1.01;
	compound::field::OptionalStateField<si::mass> unit_mass  = 1.0 * si::kilogram;
	compound::field::OptionalStateField<si::length> unit_length  = 1.0 * si::meter;

	SECTION("Appreciable differences in input are preserved within output")
	{
    	CHECK(compound::field::to_string(unknown)   != compound::field::to_string(unit));
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_nudged));
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_mass));
    	CHECK(compound::field::to_string(unit_mass) != compound::field::to_string(unit_length));
    }
}
