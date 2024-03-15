
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
#include <index/each.hpp>  
#include <index/whole.hpp>  
#include <index/series/Range.hpp>
#include <index/series/noise/UnitIntervalNoise.hpp>
#include <index/series/Get.hpp>

#include <test/structures/grouplike.hpp>  
#include <test/macros.hpp>  
#include <test/adapter.hpp>  

#include "Get.hpp"
#include "test_tools.cpp"

TEST_CASE( "Get is a monoid where range is identity", "[series]" ) {
    series::Adapter adapter(1e-7);
    std::vector<series::Range<int>> ranges {
        series::Range<int>(),
        series::Range<int>(1,100),
        series::Range<int>(-50,50)
    };
    test::Monoid monoid(
        "series::range", series::Range<int>(), 
        "series::get",   TEST_BINARY(series::get)
    );
    REQUIRE(monoid.valid(adapter, ranges));
}
