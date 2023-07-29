#pragma once

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

// in-house libraries

#include <test/adapter.hpp>
#include <test/properties.hpp>
#include "Layering.hpp"


TEST_CASE( "Layering.layer_to_height() / Layering.height_to_layer()", "[collignon]" ) {
    test::OperatorAdapter exact;
    test::StandardAdapter inexact(1e-7f);
    std::vector<float> floats {-1.0f, 0.0f, 1.0f, 4.0f, 5.0f, 6.0f};
    std::vector<float> heights{0.0f, 1.0f, 4.0f, 5.0f};
    std::vector<int>   ints   {-1, 0, 1, 5, 6, 7, 8};
    std::vector<int>   layers {0, 1, 5, 6};
    collignon::Layering<int, float> layering(0.0f, 5.0f, 7);

    REQUIRE(test::determinism(exact,
        "Layering.height_to_layer(…)", 
        [=](auto x){ 
            return collignon::Layering(0.0f, 5.0f, 7).height_to_layer(x); }, 
        heights
    ));

    REQUIRE(test::determinism(exact,
        "Layering.layer_to_height(…)", 
        [=](auto x){ 
            return collignon::Layering(0.0f, 5.0f, 7).layer_to_height(x); }, 
        ints
    ));

    REQUIRE(test::left_invertibility(exact,
        "Layering.height_to_layer(…)", 
        [=](float x)->int{ 
            return layering.height_to_layer(x); }, 
        "Layering.layer_to_height(…) when restricted to indexed layers", 
        [=](int x)->float{ 
            return layering.layer_to_height(x); }, 
        layers
    ));

    REQUIRE(test::preservation(
        test::PrimitiveAdapter(1),
        "closeness to within a single index", 
        [=](float x)->float{
            return x+layering.layer_height * 0.001; },
        "Layering.height_to_layer(…) when restricted to indexed heights", 
        [=](int x)->float{ 
            return layering.height_to_layer(x); }, 
        heights
    ));

    REQUIRE(test::preservation(
        test::PrimitiveAdapter(layering.layer_height*1.001),
        "closeness to within a reasonable multiple of layer_height", 
        [=](float x)->float{
            return x+1; },
        "Layering.layer_to_height(…) when restricted to indexed heights", 
        [=](int x)->float{ 
            return layering.layer_to_height(x); }, 
        heights
    ));


    // int id = 0;
    // REQUIRE(
    //     test::determinism(exact, 
    //         "Layering.height_to_layer(…)", [&](auto x){ 
    //             return ++id; }, 
    //         ints
    //     ));

}

