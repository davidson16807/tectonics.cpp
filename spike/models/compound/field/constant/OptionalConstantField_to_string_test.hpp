
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <units/si_test_utils.hpp>

#include "OptionalConstantField.hpp"
#include "OptionalConstantField_to_string.hpp"

TEST_CASE( "OptionalConstantField to_string() purity", "[field]" ) {
	compound::field::OptionalConstantField<double> unknown  = std::monostate();
	compound::field::OptionalConstantField<double> unit  = 1.0;
	compound::field::OptionalConstantField<double> unit_nudged  = 1.01;
	compound::field::OptionalConstantField<si::mass<double>> unit_mass  = 1.0 * si::kilogram;
	compound::field::OptionalConstantField<si::length<double>> unit_length  = 1.0 * si::meter;
	
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
    }
}

TEST_CASE( "OptionalConstantField to_string() appreciable difference preservation", "[field]" ) {
	compound::field::OptionalConstantField<double> unknown  = std::monostate();
	compound::field::OptionalConstantField<double> unit  = 1.0;
	compound::field::OptionalConstantField<double> unit_nudged  = 1.01;
	compound::field::OptionalConstantField<si::mass<double>> unit_mass  = 1.0 * si::kilogram;
	compound::field::OptionalConstantField<si::length<double>> unit_length  = 1.0 * si::meter;

	SECTION("Appreciable differences in input are preserved within output")
	{
    	CHECK(compound::field::to_string(unknown)   != compound::field::to_string(unit));
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_nudged));
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_mass));
    	CHECK(compound::field::to_string(unit_mass) != compound::field::to_string(unit_length));
    }
}
