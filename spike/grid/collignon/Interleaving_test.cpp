#include "Interleaving.hpp"

TEST_CASE( "Interleaving.interleaved_id()", "[collignon]" ) {
    collignon::Interleaving interleaving(10);
    test::OperatorAdapter exact;
    std::vector<int> ordinates {0, 1, 2, 3, 4, 5, 6, 7, 8 , 9};
    std::vector<int> indices   {0, 1, 8, 9, 10, 11, 20, 30, 40, 50, 60, 70, 80, 90, 99};

    REQUIRE(test::determinism(exact,
        "Interleaving.interleaved_id(…)", 
        [=](auto x, auto y){ 
            return collignon::Interleaving(10).interleaved_id(y,x); }, 
        ordinates, ordinates
    ));

    REQUIRE(test::determinism(exact,
        "Interleaving.interleaved_id(…)", 
        [=](auto x, auto y){ 
            return collignon::Interleaving(10).interleaved_id(y,x); }, 
        ordinates, ordinates
    ));

    REQUIRE(test::codomain(exact,
        "within expected range",
        [=](auto x){ return 0 <= x&&x < 100; },
        "Interleaving.interleaved_id(…)", 
        [=](auto x, auto y){ 
            return interleaving.interleaved_id(y,x); }, 
        ordinates, ordinates
    ));

    REQUIRE(test::codomain(exact,
        "within expected range",
        [=](auto x){ return 0 <= x&&x < 10; },
        "Interleaving.block_id(…)", 
        [=](auto x){ return interleaving.block_id(x); }, 
        ordinates
    ));

    REQUIRE(test::codomain(exact,
        "within expected range",
        [=](auto x){ return 0 <= x&&x < 10; },
        "Interleaving.element_id(…)", 
        [=](auto x){ return interleaving.element_id(x); }, 
        ordinates
    ));

    SECTION("interleaved_id() must reconstruct input passed to block_id() / element_id() for any input"){
        collignon::Interleaving interleaving(10);
        for(int interleaved_id = 0; interleaved_id < 100; interleaved_id+=1){
            CHECK(interleaving.interleaved_id( interleaving.block_id(interleaved_id), interleaving.element_id(interleaved_id) )  == interleaved_id );
        }
    }

    SECTION("block_id() / element_id() must reconstruct input passed to interleaved_id() for any input"){
        collignon::Interleaving interleaving(10);
        for(int block_id = 0; block_id < 10; block_id+=1){
        for(int element_id = 0; element_id < 10; element_id+=1){
            int interleaved_id = interleaving.interleaved_id( block_id, element_id );
            CHECK( interleaving.block_id(interleaved_id) == block_id );
            CHECK( interleaving.element_id(interleaved_id) == element_id );
        }}
    }

}

