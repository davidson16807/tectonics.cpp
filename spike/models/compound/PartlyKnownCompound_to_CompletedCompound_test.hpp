
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "PartlyKnownCompound_to_CompletedCompound.hpp"

#include "PartlyKnownCompound_test_utils.hpp"
#include "CompletedCompound_test_utils.hpp"

#include "library.hpp"

#include "PartlyKnownCompound_operators.hpp"
#include "PartlyKnownCompound_to_string.hpp"
#include "CompletedCompound_to_string.hpp"

TEST_CASE( "PartlyKnownCompound complete() purity", "[compound]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(compound::to_string(compound::complete(compound::unknown_hydrogen, compound::fallback::water )) 
           == compound::to_string(compound::complete(compound::unknown_hydrogen, compound::fallback::water )));
    }
}

/*
// NOTE: left identity does not apply to compounds, unlike for gas/liquid/solid objects,
// since some attributes like molecular_mass are not read from the right argument
TEST_CASE( "PartlyKnownCompound complete() left identity", "[compound]" ) {
    SECTION("There exists a value that when applied as the left argument of the function returns the original value")
    {
        CHECK(compound::complete(compound::unknown_hydrogen, compound::fallback::water ) ==  compound::fallback::water);
    }
}

// NOTE: associativity does not apply to compound completion,
// since this would require converting the first parameter back to a PartlyKnownCompound
TEST_CASE( "PartlyKnownCompound complete() associativity", "[compound]" ) {
    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(compound::complete(compound::unknown_hydrogen, compound::complete(compound::partly_known_dummy_compound,compound::fallback::water)) == 
              compound::complete(compound::complete(compound::unknown_hydrogen,compound::known_dummy_compound),compound::fallback::water));
    }
}
*/
