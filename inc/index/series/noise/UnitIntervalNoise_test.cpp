
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
#include <index/series/Get.hpp>

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/adapter.hpp>  

#include "UnitIntervalNoise.hpp"

TEST_CASE( "UnitIntervalNoise()", "[series]" ) {
    series::UnitIntervalNoise noise(0.1);
    test::OperatorAdapter exact;
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };

    REQUIRE(test::determinism(exact,
        "UnitIntervalNoise(…)", TEST_INDEX(series::UnitIntervalNoise(0.1f)), 
        indices
    ));

    REQUIRE(test::codomain(exact,
        "within expected range", TEST_RANGE(0.0, 1.0),
        "UnitIntervalNoise(…)",  TEST_INDEX(noise), 
        indices
    ));

    auto out = series::get(noise, series::Range(1000));
    CHECK(whole::max(out) <= 1.0);
    CHECK(whole::max(out) >  0.95);
    CHECK(whole::min(out) >= 0.0);
    CHECK(whole::min(out) <  0.05);
    CHECK(std::abs(whole::mean(out)-0.5) < 0.01);
    CHECK(std::abs(whole::standard_deviation(out)-std::sqrt(1.0/12)) < 0.01);
}

