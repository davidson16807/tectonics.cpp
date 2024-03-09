
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
#include <index/series/noise/UnitIntervalNoise.hpp>
#include <index/series/Map.hpp>

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/adapter.hpp>  

#include <math/analytic/Polynomial.hpp>  
#include <math/analytic/Identity.hpp>  

#include "Map.hpp"
#include "test_tools.cpp"

TEST_CASE( "Map", "[series]" ) {
    series::Adapter adapter(1e-7);
    std::vector<analytic::Polynomial<double,0,3>> polynomials {
        analytic::Polynomial<double,0,3>({1.0,2.0,3.0,4.0}),
        analytic::Polynomial<double,0,3>({-1.0,0.0,1.0,2.0})
    };
    std::vector<series::UnitIntervalNoise<double>> noises {
        series::UnitIntervalNoise(10.0, 1.0e4),
        series::UnitIntervalNoise(11.0, 1.1e4),
        series::UnitIntervalNoise(12.0, 1.2e4)
    };
    REQUIRE(test::determinism(adapter, "series::map", TEST_BINARY(series::map), noises, noises));
    REQUIRE(test::determinism(adapter, "series::map", TEST_BINARY(series::map), polynomials, noises));
    // REQUIRE(test::associativity(adapter, "series::map", TEST_BINARY(series::map), polynomials, polynomials, noises));
    REQUIRE(test::left_identity(adapter, 
        "id", analytic::Identity<double>(), 
        "series::map", TEST_BINARY(series::map), 
        noises));
}
