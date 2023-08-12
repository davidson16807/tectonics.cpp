
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
#include <store/each.hpp>  
#include <store/whole.hpp>  
#include <store/series/Range.hpp>
#include <store/series/noise/UnitIntervalNoise.hpp>
#include <store/series/Get.hpp>

#include <test/structures.hpp>  
#include <test/macros.hpp>  
#include <test/adapter.hpp>  

#include "Get.hpp"
#include "test_tools.cpp"

TEST_CASE( "Get is a monoid", "[series]" ) {
    series::SeriesAdapter adapter(1e-7);
    std::vector<series::UnitIntervalNoise<double>> noises {
        series::UnitIntervalNoise(10.0, 1.0e4),
        series::UnitIntervalNoise(11.0, 1.1e4),
        series::UnitIntervalNoise(12.0, 1.2e4)
    };
    test::Monoid monoid(
        "series::range", series::range(), 
        "series::get",   TEST_BINARY(series::get)
    );
    REQUIRE(monoid.valid(adapter, noises));
}
