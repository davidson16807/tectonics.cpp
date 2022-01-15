
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "OptionalStateField.hpp"
#include "OptionalStateField_to_CompletedStateField.hpp"

#include "CompletedStateField_test_utils.hpp"


TEST_CASE( "OptionalStateField complete() purity", "[field]" ) {
	// compound::field::OptionalStateField<double> unknown  = std::monostate();
    compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });

    compound::field::CompletedStateField<double> known_relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });

	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
    	CHECK(compound::field::complete(relation,known_relation) == compound::field::complete(relation,known_relation));
    }
}


TEST_CASE( "OptionalStateField complete() increasing", "[field]" ) {
	compound::field::OptionalStateField<double> unknown  = std::monostate();
	compound::field::OptionalStateField<double> relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });

    compound::field::CompletedStateField<double> known_relation  = compound::field::StateFunction<double>([](const si::pressure p, const si::temperature T){ return test_ideal_gas_law_optional(p,T); });

	SECTION("An attribute of a function's return entry either increases or remains the same when compared to the same attribute of the input entry")
	{
    	CHECK(compound::field::complete(unknown,known_relation).index()+1 >= unknown.index());

    	CHECK(compound::field::complete(relation,known_relation).index()+1 >= relation.index());
    }
}






