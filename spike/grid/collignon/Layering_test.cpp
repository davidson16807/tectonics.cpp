
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

// in-house libraries

#include "Layering.hpp"


TEST_CASE( "Layering.height_to_layer() purity", "[collignon]" ) {
    SECTION("Layering.height_to_layer() must be called repeatedly without changing the output"){
        for(float x = -1; x < 6.0; x+=0.5){
            CHECK(collignon::Layering(0.0f, 5.0f, 7).height_to_layer(x) == 
                  collignon::Layering(0.0f, 5.0f, 7).height_to_layer(x));
        }
    }
}


TEST_CASE( "Layering height_to_layer() closeness preservation", "[collignon]" ) {
    SECTION("changes in height_to_layer must not result in changes that exceed a reasonable multiple"){
        collignon::Layering<int, float> layering(0.0f, 5.0f, 7);
        for(int i = -1; i < 8; ++i){
            CHECK(std::abs(layering.layer_to_height(i+1) - layering.layer_to_height(i)) <= layering.layer_height * 1.01);
        }
    }
}


TEST_CASE( "Layering.layer_to_height() purity", "[collignon]" ) {
    SECTION("Layering.layer_to_height() must be called repeatedly without changing the output"){
        for(int i = -1; i < 8; ++i){
            CHECK(collignon::Layering(0.0f, 5.0f, 7).layer_to_height(i) == 
                  collignon::Layering(0.0f, 5.0f, 7).layer_to_height(i));
        }
    }
}


TEST_CASE( "Layering layer_to_height() closeness preservation", "[collignon]" ) {
    SECTION("changes in layer_to_height must not result in changes that exceed a reasonable multiple"){
        collignon::Layering<int, float> layering(0.0f, 5.0f, 7);
        for(float x = -1; x < 6.0; x+=0.5){
            CHECK(std::abs(layering.layer_to_height(x+layering.layer_height*0.1) - layering.layer_to_height(x)) <= 1);
        }
    }
}


TEST_CASE( "Layering.layer_to_height() / height_to_layer() invertibility", "[collignon]" ) {
    SECTION("Layering.height_to_layer() must reconstruct input passed to layer_to_height() when restricted to a [bottom_height, top_height]"){
        collignon::Layering<int, float> layering(0.0f, 5.0f, 7);
        for(int i = 0; i < 7; ++i){
            CHECK( layering.height_to_layer( layering.layer_to_height(i) ) == i );
        }
    }
}


