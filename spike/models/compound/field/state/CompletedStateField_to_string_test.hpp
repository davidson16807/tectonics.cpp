
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include <units/si_test_utils.hpp>

#include "CompletedStateField.hpp"
#include "CompletedStateField_to_string.hpp"

TEST_CASE( "CompletedStateField to_string() purity", "[field]" ) {
	compound::field::CompletedStateField<double> unit  = compound::field::StateFunction<double>([](const si::pressure<double> p, const si::temperature<double> T){ return 1.0; });;
	compound::field::CompletedStateField<double> unit_nudged  = compound::field::StateFunction<double>([](const si::pressure<double> p, const si::temperature<double> T){ return 1.01; });;
	compound::field::CompletedStateField<si::mass<double>> unit_mass  = compound::field::StateFunction<si::mass<double>>([](const si::pressure<double> p, const si::temperature<double> T){ return 1.0 * si::kilogram; }); 
	compound::field::CompletedStateField<si::length<double>> unit_length  = compound::field::StateFunction<si::length<double>>([](const si::pressure<double> p, const si::temperature<double> T){ return 1.0 * si::meter; }); 
	compound::field::CompletedStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure<double> p, const si::temperature<double> T){ return test_ideal_gas_law_completed(p,T); });

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
    	CHECK(compound::field::to_string(relation) 
    	   == compound::field::to_string(relation));
    }
}

TEST_CASE( "CompletedStateField to_string() appreciable difference preservation", "[field]" ) {
	compound::field::CompletedStateField<double> unit  = compound::field::StateFunction<double>([](const si::pressure<double> p, const si::temperature<double> T){ return 1.0; });;
	compound::field::CompletedStateField<double> unit_nudged  = compound::field::StateFunction<double>([](const si::pressure<double> p, const si::temperature<double> T){ return 1.01; });;
	compound::field::CompletedStateField<si::mass<double>> unit_mass  = compound::field::StateFunction<si::mass<double>>([](const si::pressure<double> p, const si::temperature<double> T){ return 1.0 * si::kilogram; }); 
	compound::field::CompletedStateField<si::length<double>> unit_length  = compound::field::StateFunction<si::length<double>>([](const si::pressure<double> p, const si::temperature<double> T){ return 1.0 * si::meter; }); 

	SECTION("Appreciable differences in input are preserved within output")
	{
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_nudged));
    	CHECK(compound::field::to_string(unit)      != compound::field::to_string(unit_mass));
    	CHECK(compound::field::to_string(unit_mass) != compound::field::to_string(unit_length));
    }
}
