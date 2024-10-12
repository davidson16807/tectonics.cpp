
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
#include <index/procedural/noise/UnitIntervalNoise.hpp>
#include <index/iterated/Metric.hpp>
#include <index/adapted/glm/GlmMetric.hpp>

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/glm/adapter.hpp>  

#include "VectorInterleave.hpp"

TEST_CASE( "VectorInterleave<4>()", "[series]" ) {
    const double tolerance(0.11);
    const int N(4);
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::GlmAdapter<int,double> adapter(1e-7);
    iterated::Metric metric{adapted::GlmMetric{}};

    REQUIRE(test::determinism(adapter,
        "VectorInterleave(…)", 
        TEST_INDEX(
            procedural::vector_interleave<N>(
                procedural::UnitIntervalNoise<double>())),
        indices
    ));

    auto vec4s = procedural::get(
        procedural::vector_interleave<N>(
            procedural::UnitIntervalNoise<double>()), 
        procedural::Range(6000));
    std::vector<double> lengths(vec4s.size());
    metric.length(vec4s, lengths);
    CHECK(whole::max(lengths) <= sqrt(N));
    CHECK(whole::max(lengths) >  sqrt(N)-tolerance);
}

TEST_CASE( "VectorInterleave<3>()", "[series]" ) {
    const double tolerance(1e-1);
    const int N(3);
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::GlmAdapter<int,double> adapter(1e-7);
    iterated::Metric metric{adapted::GlmMetric{}};

    REQUIRE(test::determinism(adapter,
        "VectorInterleave(…)", 
        TEST_INDEX(
            procedural::vector_interleave<N>(
                procedural::UnitIntervalNoise<double>())),
        indices
    ));

    auto vec3s = procedural::get(
        procedural::vector_interleave<N>(
            procedural::UnitIntervalNoise<double>()), 
        procedural::Range(6000));
    std::vector<double> lengths(vec3s.size());
    metric.length(vec3s, lengths);
    CHECK(whole::max(lengths) <= sqrt(N));
    CHECK(whole::max(lengths) >  sqrt(N)-tolerance);
}

TEST_CASE( "VectorInterleave<2>()", "[series]" ) {
    const double tolerance(1e-1);
    const int N(2);
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::GlmAdapter<int,double> adapter(1e-7);
    iterated::Metric metric{adapted::GlmMetric{}};

    REQUIRE(test::determinism(adapter,
        "VectorInterleave(…)", 
        TEST_INDEX(
            procedural::vector_interleave<N>(
                procedural::UnitIntervalNoise<double>())),
        indices
    ));

    auto vec2s = procedural::get(
        procedural::vector_interleave<N>(
            procedural::UnitIntervalNoise<double>()), 
        procedural::Range(6000));
    std::vector<double> lengths(vec2s.size());
    metric.length(vec2s, lengths);
    CHECK(whole::max(lengths) <= sqrt(N));
    CHECK(whole::max(lengths) >  sqrt(N)-tolerance);
}

