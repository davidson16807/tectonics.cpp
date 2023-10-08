#pragma once

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

// in-house libraries
#include <test/macros.hpp>
#include <test/adapter.hpp>
#include <test/properties.hpp>
#include "Layering.hpp"

TEST_CASE( "Layering.layer_to_height() / Layering.height_to_layer()", "[bijective]" ) {
    test::OperatorAdapter exact;
    test::StandardAdapter inexact(1e-7f);
    std::vector<float> floats {-1.0f, 0.0f, 1.0f, 4.0f, 5.0f, 6.0f};
    std::vector<float> heights{0.0f, 1.0f, 4.0f, 5.0f};
    std::vector<int>   ints   {-1, 0, 1, 5, 6, 7, 8};
    std::vector<int>   layers {0, 1, 5, 6};
    bijective::Layering<int, float> layering(0.0f, 5.0f, 7);

    REQUIRE(test::determinism(exact,
        "Layering.height_to_layer(…)", TEST_UNARY(bijective::Layering(0.0f, 5.0f, 7).height_to_layer),
        heights
    ));

    REQUIRE(test::determinism(exact,
        "Layering.layer_to_height(…)", TEST_UNARY(bijective::Layering(0.0f, 5.0f, 7).layer_to_height),
        ints
    ));

    REQUIRE(test::codomain(exact,
        "within expected range",       TEST_RANGE(0, layering.layer_count),
        "Layering.height_to_layer(…)", TEST_UNARY(layering.height_to_layer),
        heights
    ));

    REQUIRE(test::codomain(exact,
        "within expected range",       TEST_RANGE(layering.bottom_height, layering.top_height),
        "Layering.layer_to_height(…)", TEST_UNARY(layering.layer_to_height),
        heights
    ));

    REQUIRE(test::left_invertibility(exact,
        "Layering.height_to_layer(…)",                                   TEST_UNARY(layering.height_to_layer),
        "Layering.layer_to_height(…) when restricted to indexed layers", TEST_UNARY(layering.layer_to_height),
        layers
    ));

    REQUIRE(test::preservation(
        test::PrimitiveAdapter(1), 
        "Layering.height_to_layer(…) when restricted to indexed heights", TEST_UNARY(layering.height_to_layer),
        "closeness to within a single index",                             TEST_NUDGE(layering.layer_height * 0.001),
        heights
    ));

    REQUIRE(test::preservation(
        test::PrimitiveAdapter(layering.layer_height*1.001),
        "Layering.layer_to_height(…) when restricted to indexed heights", TEST_UNARY(layering.layer_to_height),
        "closeness to within a reasonable multiple of layer_height",      TEST_NUDGE(1),
        heights
    ));

}

