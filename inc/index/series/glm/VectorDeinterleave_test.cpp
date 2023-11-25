
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
#include <test/adapter.hpp>  

#include "VectorInterleave.hpp"
#include "VectorDeinterleave.hpp"

TEST_CASE( "VectorDeinterleave<4>()", "[series]" ) {
    // const double tolerance(0.11);
    const int N(4);
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::PrimitiveAdapter<double> adapter(1e-7);

    REQUIRE(test::determinism(adapter,
        "VectorDeinterleave(…)", 
        TEST_INDEX(
            series::vector_deinterleave<N>(
                series::vector_interleave<N>(
                    series::UnitIntervalNoise<double>()))),
        indices
    ));
}

TEST_CASE( "VectorDeinterleave<3>()", "[series]" ) {
    // const double tolerance(0.11);
    const int N(3);
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::PrimitiveAdapter<double> adapter(1e-7);

    REQUIRE(test::determinism(adapter,
        "VectorDeinterleave(…)", 
        TEST_INDEX(
            series::vector_deinterleave<N>(
                series::vector_interleave<N>(
                    series::UnitIntervalNoise<double>()))),
        indices
    ));
}

TEST_CASE( "VectorDeinterleave<2>()", "[series]" ) {
    // const double tolerance(0.11);
    const int N(2);
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };
    test::PrimitiveAdapter<double> adapter(1e-7);

    REQUIRE(test::determinism(adapter,
        "VectorDeinterleave(…)", 
        TEST_INDEX(
            series::vector_deinterleave<N>(
                series::vector_interleave<N>(
                    series::UnitIntervalNoise<double>()))),
        indices
    ));
}
