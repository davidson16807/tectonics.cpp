
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
#include <index/procedural/noise/UnitIntervalNoise.hpp>
#include <index/procedural/Get.hpp>

#include <test/structures/grouplike.hpp>  
#include <test/macros.hpp>  
#include <test/adapter.hpp>  

#include "Get.hpp"
#include "test_tools.cpp"

TEST_CASE( "Get is a monoid where range is identity", "[series]" ) {
    procedural::Adapter adapter(1e-7);
    std::vector<procedural::Range<int>> ranges {
        procedural::Range<int>(),
        procedural::Range<int>(1,100),
        procedural::Range<int>(-50,50)
    };
    test::Monoid monoid(
        "procedural::range", procedural::Range<int>(), 
        "procedural::get",   TEST_BINARY(procedural::get)
    );
    REQUIRE(monoid.valid(adapter, ranges));
}
