#include "Interleaving.hpp"

TEST_CASE( "Interleaving.interleaved_id() purity", "[collignon]" ) {
    SECTION("Interleaving.interleaved_id() must be called repeatedly without changing the output"){
        for(int x = 0; x < 10; x+=1){
        for(int y = 0; y < 10; y+=1){
            CHECK(collignon::Interleaving(10).interleaved_id(y,x) == 
                  collignon::Interleaving(10).interleaved_id(y,x));
        }}
    }
}
TEST_CASE( "Interleaving block_id() / element_id() / interleaved_id() invertibility", "[collignon]" ) {
    SECTION("block_id() / element_id() must reconstruct input passed to interleaved_id() for any input"){
        collignon::Interleaving interleaving(10);
        for(int block_id = 0; block_id < 10; block_id+=1){
        for(int element_id = 0; element_id < 10; element_id+=1){
            int interleaved_id = interleaving.interleaved_id( block_id, element_id );
            CHECK( interleaving.block_id(interleaved_id) == block_id );
            CHECK( interleaving.element_id(interleaved_id) == element_id );
        }}
    }
    SECTION("interleaved_id() must reconstruct input passed to block_id() / element_id() for any input"){
        collignon::Interleaving interleaving(10);
        for(int interleaved_id = 0; interleaved_id < 100; interleaved_id+=1){
            CHECK(interleaving.interleaved_id( interleaving.block_id(interleaved_id), interleaving.element_id(interleaved_id) )  == interleaved_id );
        }
    }
}
TEST_CASE( "Interleaving block_id() / element_id() / interleaved_id() range restrictions", "[collignon]" ) {
    SECTION("interleaved_id() must not produce results outside expected range"){
        collignon::Interleaving interleaving(10);
        for(int block_id = 0; block_id < 10; block_id+=1){
        for(int element_id = 0; element_id < 10; element_id+=1){
            int interleaved_id = interleaving.interleaved_id( block_id, element_id );
            CHECK( interleaved_id < 100 );
            CHECK( interleaved_id >= 0 );
        }}
    }
    SECTION("block_id() / element_id() must not produce results outside expected range"){
        collignon::Interleaving interleaving(10);
        for(int interleaved_id = 0; interleaved_id < 100; interleaved_id+=1){
            CHECK(interleaving.block_id(interleaved_id) >= 0 );
            CHECK(interleaving.block_id(interleaved_id) < 10 );
            CHECK(interleaving.element_id(interleaved_id) >= 0 );
            CHECK(interleaving.element_id(interleaved_id) < 10 );
        }
    }
}
/*
*/