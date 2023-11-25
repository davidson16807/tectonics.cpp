
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
#include <index/glm/each.hpp>  
#include <index/whole.hpp>  
#include <index/series/Range.hpp>
#include <index/series/Get.hpp>
#include <index/series/noise/UnitIntervalNoise.hpp>

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

    REQUIRE(test::determinism(adapter,
        "Vector4Zip(…)", 
        TEST_INDEX(
            series::vector4_zip(
                series::UnitIntervalNoise<double>(10.0, 1e4),
                series::UnitIntervalNoise<double>(20.0, 2e4),
                series::UnitIntervalNoise<double>(30.0, 3e4),
                series::UnitIntervalNoise<double>(40.0, 4e4))),
        indices
    ));

    auto vec4s = series::get(
        series::vector4_zip(
            series::UnitIntervalNoise<double>(10.0, 1e4),
            series::UnitIntervalNoise<double>(20.0, 2e4),
            series::UnitIntervalNoise<double>(30.0, 3e4),
            series::UnitIntervalNoise<double>(40.0, 4e4)), 
        series::Range(6000));
    std::vector<double> lengths(vec4s.size());
    each::length(vec4s, lengths);
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

    REQUIRE(test::determinism(adapter,
        "Vector3Zip(…)", 
        TEST_INDEX(
            series::vector3_zip(
                series::UnitIntervalNoise<double>(10.0, 1e4),
                series::UnitIntervalNoise<double>(20.0, 2e4),
                series::UnitIntervalNoise<double>(30.0, 3e4))),
        indices
    ));

    auto vec3s = series::get(
        series::vector3_zip(
                series::UnitIntervalNoise<double>(10.0, 1e4),
                series::UnitIntervalNoise<double>(20.0, 2e4),
                series::UnitIntervalNoise<double>(30.0, 3e4)), 
        series::Range(6000));
    std::vector<double> lengths(vec3s.size());
    each::length(vec3s, lengths);
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

    REQUIRE(test::determinism(adapter,
        "Vector2Zip(…)", 
        TEST_INDEX(
            series::vector2_zip(
                series::UnitIntervalNoise<double>(10.0, 1e4),
                series::UnitIntervalNoise<double>(20.0, 2e4))),
        indices
    ));

    auto vec2s = series::get(
        series::vector2_zip(
                series::UnitIntervalNoise<double>(10.0, 1e4),
                series::UnitIntervalNoise<double>(20.0, 2e4)), 
        series::Range(6000));
    std::vector<double> lengths(vec2s.size());
    each::length(vec2s, lengths);
    CHECK(whole::max(lengths) <= sqrt(N));
    CHECK(whole::max(lengths) >  sqrt(N)-tolerance);
}

