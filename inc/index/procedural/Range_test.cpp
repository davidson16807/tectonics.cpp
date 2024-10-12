
#include <iostream>

// std libraries
#include <limits>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec2.hpp>     // *vec3
#include <glm/vec3.hpp>     // *vec3

// in-house libraries
#include <index/whole.hpp>  
#include <index/procedural/Range.hpp>
#include <index/procedural/Get.hpp>

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/adapter.hpp>  

#include "Range.hpp"

TEST_CASE( "Range()", "[series]" ) {
    procedural::Range noise(0.1);
    test::OperatorAdapter exact;
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };

    REQUIRE(test::determinism(exact,
        "Range(…)", TEST_INDEX(procedural::Range(0.1f)), 
        indices
    ));

    procedural::Range range(100);
    CHECK(whole::max(range) == 99);
    CHECK(whole::min(range) == 0);
}

