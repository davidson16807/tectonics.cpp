
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
#include <store/glm/each.hpp>  
#include <store/whole.hpp>  
#include <store/series/Range.hpp>
#include <store/series/Get.hpp>

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>  

#include "UnitVectorInterleave.hpp"

TEST_CASE( "UnitVector3Interleave()", "[series]" ) {
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::GlmAdapter<int,double> adapter(1e-7);

    REQUIRE(test::determinism(adapter,
        "UnitVector3Interleave(…)", 
        TEST_INDEX(
            series::UnitVector3Interleave(
                series::UnitIntervalNoise<double>())),
        indices
    ));

    auto vec3s = series::get(
        series::UnitVector3Interleave(
            series::UnitIntervalNoise<double>()), 
        series::Range(1000));
    std::vector<double> lengths(vec3s.size());
    each::length(vec3s, lengths);
    CHECK(whole::max(lengths) <= 1.0);
    CHECK(whole::max(lengths) >  1.0-1e-7);
    CHECK(whole::min(lengths) <= 1.0);
    CHECK(whole::min(lengths) >  1.0-1e-7);
}


TEST_CASE( "UnitVector2Interleave()", "[series]" ) {
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::GlmAdapter<int,double> adapter(1e-7);

    REQUIRE(test::determinism(adapter,
        "UnitVector2Interleave(…)", 
        TEST_INDEX(
            series::UnitVector2Interleave(
                series::UnitIntervalNoise<double>())),
        indices
    ));

    auto vec2s = series::get(
        series::UnitVector2Interleave(
            series::UnitIntervalNoise<double>()), 
        series::Range(1000));
    std::vector<double> lengths(vec2s.size());
    each::length(vec2s, lengths);
    CHECK(whole::max(lengths) <= 1.0);
    CHECK(whole::max(lengths) >  1.0-1e-7);
    CHECK(whole::min(lengths) <= 1.0);
    CHECK(whole::min(lengths) >  1.0-1e-7);
}

