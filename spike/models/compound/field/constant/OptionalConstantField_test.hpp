
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "OptionalConstantField.hpp"

double test_ideal_gas_law_optional(const si::pressure p, const si::temperature T)
{
	return si::unitless(si::mole*si::universal_gas_constant*T/p/si::liter);
}

TEST_CASE( "OptionalConstantField value_or() purity", "[field]" ) {
	compound::field::OptionalConstantField<double> unknown  = std::monostate();
	compound::field::OptionalConstantField<double> constant  = 1.0;

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(unknown.value_or(unknown ) 
    	   == unknown.value_or(unknown ));
    	CHECK(constant.value_or(constant) 
    	   == constant.value_or(constant));
    }
}

TEST_CASE( "OptionalConstantField value_or() identity", "[field]" ) {
	compound::field::OptionalConstantField<double> unknown  = std::monostate();
	compound::field::OptionalConstantField<double> constant  = 1.0;

	SECTION("There exists a entry that when applied to a function returns the original entry")
	{
    	CHECK(unknown.value_or(unknown) == unknown);
    	CHECK(constant.value_or(unknown) == constant);
    }
}

TEST_CASE( "OptionalConstantField value_or() associativity", "[field]" ) {
    compound::field::OptionalConstantField<double> constant1  = 1.0;
    compound::field::OptionalConstantField<double> constant2  = 2.0;
    compound::field::OptionalConstantField<double> constant3  = 3.0;

	SECTION("Functions can be applied in any order and still produce the same results")
	{
        CHECK(constant1.value_or(constant2.value_or(constant3)) == 
              constant1.value_or(constant2).value_or(constant3));

        CHECK(constant1.value_or(constant3.value_or(constant2)) == 
              constant1.value_or(constant3).value_or(constant2));



        CHECK(constant2.value_or(constant1.value_or(constant3)) == 
              constant2.value_or(constant1).value_or(constant3));

        CHECK(constant2.value_or(constant3.value_or(constant1)) == 
              constant2.value_or(constant3).value_or(constant1));



        CHECK(constant3.value_or(constant1.value_or(constant2)) == 
              constant3.value_or(constant1).value_or(constant2));

        CHECK(constant3.value_or(constant2.value_or(constant1)) == 
              constant3.value_or(constant2).value_or(constant1));

    }
}


TEST_CASE( "OptionalConstantField value_or() increasing", "[field]" ) {
	compound::field::OptionalConstantField<double> unknown  = std::monostate();
	compound::field::OptionalConstantField<double> constant  = 1.0;

	SECTION("An attribute of a function's return entry either increases or remains the same when compared to the same attribute of the input entry")
	{
    	CHECK(unknown.value_or(unknown).index() >= unknown.index());
    	CHECK(unknown.value_or(constant).index() >= unknown.index());

    	CHECK(constant.value_or(unknown).index() >= constant.index());
    	CHECK(constant.value_or(constant).index() >= constant.index());
    }
}

