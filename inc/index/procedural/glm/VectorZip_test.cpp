
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

#include "VectorZip.hpp"

TEST_CASE( "Vector4Zip()", "[series]" ) {
    const double tolerance(1e-1);
    const int N(4);
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::GlmAdapter<int,double> adapter(1e-7);
    iterated::Metric metric{adapted::GlmMetric{}};

    REQUIRE(test::determinism(adapter,
        "Vector4Zip(…)", 
        TEST_INDEX(
            procedural::vector4_zip(
                procedural::UnitIntervalNoise<double>(10.0, 1e4),
                procedural::UnitIntervalNoise<double>(20.0, 2e4),
                procedural::UnitIntervalNoise<double>(30.0, 3e4),
                procedural::UnitIntervalNoise<double>(40.0, 4e4))),
        indices
    ));

    auto vec4s = procedural::get(
        procedural::vector4_zip(
            procedural::UnitIntervalNoise<double>(10.0, 1e4),
            procedural::UnitIntervalNoise<double>(20.0, 2e4),
            procedural::UnitIntervalNoise<double>(30.0, 3e4),
            procedural::UnitIntervalNoise<double>(40.0, 4e4)), 
        procedural::Range(6000));
    std::vector<double> lengths(vec4s.size());
    metric.length(vec4s, lengths);
    CHECK(whole::max(lengths) <= sqrt(N));
    CHECK(whole::max(lengths) >  sqrt(N)-tolerance);
}

TEST_CASE( "Vector3Zip()", "[series]" ) {
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
        "Vector3Zip(…)", 
        TEST_INDEX(
            procedural::vector3_zip(
                procedural::UnitIntervalNoise<double>(10.0, 1e4),
                procedural::UnitIntervalNoise<double>(20.0, 2e4),
                procedural::UnitIntervalNoise<double>(30.0, 3e4))),
        indices
    ));

    auto vec3s = procedural::get(
        procedural::vector3_zip(
                procedural::UnitIntervalNoise<double>(10.0, 1e4),
                procedural::UnitIntervalNoise<double>(20.0, 2e4),
                procedural::UnitIntervalNoise<double>(30.0, 3e4)), 
        procedural::Range(6000));
    std::vector<double> lengths(vec3s.size());
    metric.length(vec3s, lengths);
    CHECK(whole::max(lengths) <= sqrt(N));
    CHECK(whole::max(lengths) >  sqrt(N)-tolerance);
}

TEST_CASE( "Vector2Zip()", "[series]" ) {
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
        "Vector2Zip(…)", 
        TEST_INDEX(
            procedural::vector2_zip(
                procedural::UnitIntervalNoise<double>(10.0, 1e4),
                procedural::UnitIntervalNoise<double>(20.0, 2e4))),
        indices
    ));

    auto vec2s = procedural::get(
        procedural::vector2_zip(
                procedural::UnitIntervalNoise<double>(10.0, 1e4),
                procedural::UnitIntervalNoise<double>(20.0, 2e4)), 
        procedural::Range(6000));
    std::vector<double> lengths(vec2s.size());
    metric.length(vec2s, lengths);
    CHECK(whole::max(lengths) <= sqrt(N));
    CHECK(whole::max(lengths) >  sqrt(N)-tolerance);
}

