
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
#include <store/series/noise/UnitIntervalNoise.hpp>

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>  

#include "UnitVectorNoise.hpp"

TEST_CASE( "UnitVectorNoise<4>()", "[series]" ) {
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::GlmAdapter<int,double> adapter(1e-7);

    REQUIRE(test::determinism(adapter,
        "UnitVectorNoise(…)", 
        TEST_INDEX((series::unit_vector_noise<4,double>())),
        indices
    ));

    auto vec4s = series::get(
        series::unit_vector_noise<4,double>(), 
        series::Range(1000));
    std::vector<double> lengths(vec4s.size());
    each::length(vec4s, lengths);
    CHECK(std::abs(whole::max(lengths)-1.0) < 1e-7);
    CHECK(std::abs(whole::min(lengths)-1.0) < 1e-7);
}

TEST_CASE( "UnitVectorNoise<3>()", "[series]" ) {
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::GlmAdapter<int,double> adapter(1e-7);

    REQUIRE(test::determinism(adapter,
        "UnitVectorNoise(…)", 
        TEST_INDEX((series::unit_vector_noise<3,double>())),
        indices
    ));

    auto vec3s = series::get(
        series::unit_vector_noise<3,double>(), 
        series::Range(1000));
    std::vector<double> lengths(vec3s.size());
    each::length(vec3s, lengths);
    CHECK(std::abs(whole::max(lengths)-1.0) < 1e-7);
    CHECK(std::abs(whole::min(lengths)-1.0) < 1e-7);
}

TEST_CASE( "UnitVectorNoise<2>()", "[series]" ) {
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::GlmAdapter<int,double> adapter(1e-7);

    REQUIRE(test::determinism(adapter,
        "UnitVectorNoise(…)", 
        TEST_INDEX((series::unit_vector_noise<2,double>())),
        indices
    ));

    auto vec2s = series::get(
        series::unit_vector_noise<2,double>(), 
        series::Range(1000));
    std::vector<double> lengths(vec2s.size());
    each::length(vec2s, lengths);
    CHECK(std::abs(whole::max(lengths)-1.0) < 1e-7);
    CHECK(std::abs(whole::min(lengths)-1.0) < 1e-7);
}

