
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
#include <index/procedural/Interleave.hpp>
#include <index/procedural/Get.hpp>

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/adapter.hpp>  

#include "Interleave.hpp"

TEST_CASE( "Interleave()", "[series]" ) {
    test::OperatorAdapter exact;
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };

    REQUIRE(test::determinism(exact,
        "Interleave(…)", TEST_INDEX(procedural::interleave(2, procedural::UnitIntervalNoise(10.0))), 
        indices
    ));

    auto range_interleave = procedural::interleave(10, procedural::range(10));
    CHECK(range_interleave.size() == 100);
    CHECK(whole::max(range_interleave) == 9);
    CHECK(whole::min(range_interleave) == 0);
}

