
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
#include <index/procedural/noise/UnitIntervalNoise.hpp>
#include <index/procedural/Map.hpp>

#include <test/properties.hpp>  
#include <test/macros.hpp>  
#include <test/adapter.hpp>  

#include <math/analytic/Polynomial.hpp>  
#include <math/analytic/Identity.hpp>  

#include "Map.hpp"
#include "test_tools.cpp"

TEST_CASE( "Map", "[series]" ) {
    procedural::Adapter adapter(1e-7);
    std::vector<analytic::Polynomial<double,0,3>> polynomials {
        analytic::Polynomial<double,0,3>({1.0,2.0,3.0,4.0}),
        analytic::Polynomial<double,0,3>({-1.0,0.0,1.0,2.0})
    };
    std::vector<procedural::UnitIntervalNoise<double>> noises {
        procedural::UnitIntervalNoise(10.0, 1.0e4),
        procedural::UnitIntervalNoise(11.0, 1.1e4),
        procedural::UnitIntervalNoise(12.0, 1.2e4)
    };
    REQUIRE(test::determinism(adapter, "procedural::map", TEST_BINARY(procedural::map), noises, noises));
    REQUIRE(test::determinism(adapter, "procedural::map", TEST_BINARY(procedural::map), polynomials, noises));
    // REQUIRE(test::associativity(adapter, "procedural::map", TEST_BINARY(procedural::map), polynomials, polynomials, noises));
    REQUIRE(test::left_identity(adapter, 
        "id", analytic::Identity<double>(), 
        "procedural::map", TEST_BINARY(procedural::map), 
        noises));
}
