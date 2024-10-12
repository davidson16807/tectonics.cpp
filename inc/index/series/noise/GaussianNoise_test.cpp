
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
#include <index/series/Range.hpp>
#include <index/series/Get.hpp>

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/adapter.hpp>  

#include "GaussianNoise.hpp"

TEST_CASE( "GaussianNoise()", "[series]" ) {
    auto noise = series::gaussian<double>();
    test::OperatorAdapter exact;
    std::vector<int> indices   {
        -1, 0, 1, 2, 3, 
        std::numeric_limits<int>::max(), 
        std::numeric_limits<int>::min()
    };

    REQUIRE(test::determinism(exact,
        "GaussianNoise(…)", TEST_INDEX(series::gaussian<double>()), 
        indices
    ));

    auto out = series::get(noise, series::Range(3000));
    CHECK(std::abs(whole::mean(out)-0.0) < 0.05);
    CHECK(std::abs(whole::standard_deviation(out)-1.0) < 0.01);
}

