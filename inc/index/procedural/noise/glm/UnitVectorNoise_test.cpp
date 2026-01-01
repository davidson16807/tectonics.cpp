
#include <iostream>

// std libraries
#include <limits>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <glm/vec2.hpp>     // *vec3
#include <glm/vec3.hpp>     // *vec3

// in-house libraries
#include <index/whole.hpp>  
#include <index/procedural/Range.hpp>
#include <index/procedural/Get.hpp>
#include <index/procedural/noise/UnitIntervalNoise.hpp>
#include <index/iterated/Metric.hpp>
#include <index/adapted/glm/GlmMetric.hpp>

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
    iterated::Metric metric{adapted::GlmMetric{}};

    REQUIRE(test::determinism(adapter,
        "UnitVectorNoise(…)", 
        TEST_INDEX((procedural::unit_vector_noise<4,double>())),
        indices
    ));

    auto vec4s = procedural::get(
        procedural::unit_vector_noise<4,double>(), 
        procedural::Range(1000));
    std::vector<double> lengths(vec4s.size());
    metric.length(vec4s, lengths);
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
    iterated::Metric metric{adapted::GlmMetric{}};

    REQUIRE(test::determinism(adapter,
        "UnitVectorNoise(…)", 
        TEST_INDEX((procedural::unit_vector_noise<3,double>())),
        indices
    ));

    auto vec3s = procedural::get(
        procedural::unit_vector_noise<3,double>(), 
        procedural::Range(1000));
    std::vector<double> lengths(vec3s.size());
    metric.length(vec3s, lengths);
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
    iterated::Metric metric{adapted::GlmMetric{}};

    REQUIRE(test::determinism(adapter,
        "UnitVectorNoise(…)", 
        TEST_INDEX((procedural::unit_vector_noise<2,double>())),
        indices
    ));

    auto vec2s = procedural::get(
        procedural::unit_vector_noise<2,double>(), 
        procedural::Range(1000));
    std::vector<double> lengths(vec2s.size());
    metric.length(vec2s, lengths);
    CHECK(std::abs(whole::max(lengths)-1.0) < 1e-7);
    CHECK(std::abs(whole::min(lengths)-1.0) < 1e-7);
}

