#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE          // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL // disable anonymous structs so we can build with ISO C++
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>  // to_string

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <test/adapter.hpp>
#include <test/glm/adapter.hpp>
#include <test/properties.hpp>
#include <test/macros.hpp>
#include "Ids.hpp"


TEST_CASE( "generic orthtree::Ids properties", "[orthtree]" ) {
    test::GlmAdapter<int,double> exact(1e-7);
    orthtree::Ids<2,int,double> ids(10.0, glm::vec2(-5.0));
    std::vector<int> memory_ids{0, 1, 2, 3, 4, 5, 6, 7, 8 , 9};
    std::vector<int> levels    {0, 1, 2, 3};
    std::vector<glm::ivec2> cell_ids {
        glm::ivec2(0, 0),

        glm::ivec2(-1, 0),
        glm::ivec2(0, -1),
        glm::ivec2(-1, -1),
        glm::ivec2(1, 0),
        glm::ivec2(0, 1),
        glm::ivec2(1, 1),
        glm::ivec2(-1, 1),
        glm::ivec2(1, -1),

        glm::ivec2(-5, 0),
        glm::ivec2(0, -5),
        glm::ivec2(-5, -5),
        glm::ivec2(5, 0),
        glm::ivec2(0, 5),
        glm::ivec2(5, 5),
        glm::ivec2(-5, 5),
        glm::ivec2(5, -5)
    };

    REQUIRE(test::determinism(exact,
        "Ids.level_cell_count(…)", TEST_UNARY(ids.level_cell_count), 
        levels ));

    REQUIRE(test::determinism(exact,
        "Ids.level_cell_width(…)", TEST_UNARY(ids.level_cell_width), 
        levels ));

    REQUIRE(test::determinism(exact,
        "Ids.level_dimension_cell_count(…)", TEST_UNARY(ids.level_dimension_cell_count), 
        levels ));


    REQUIRE(test::determinism(exact,
        "Ids.level_start_id(…)", TEST_UNARY(ids.level_start_id), 
        levels ));

    REQUIRE(test::determinism(exact,
        "Ids.level_end_id(…)", TEST_UNARY(ids.level_end_id), 
        levels ));

    REQUIRE(test::determinism(exact,
        "Ids.level_from_memory_id(…)", TEST_UNARY(ids.level_from_memory_id), 
        memory_ids ));

    REQUIRE(test::left_invertibility(exact,
        "Ids.level_from_memory_id(…)",                             TEST_UNARY(ids.level_from_memory_id),
        "Ids.level_end_id(…) when restricted to positive numbers", TEST_UNARY(ids.level_end_id),
        levels ));

    REQUIRE(test::left_invertibility(exact,
        "Ids.level_from_memory_id(…)",                               TEST_UNARY(ids.level_from_memory_id),
        "Ids.level_start_id(…) when restricted to positive numbers", TEST_UNARY(ids.level_end_id),
        levels ));

    REQUIRE(test::left_invertibility(exact,
        "Ids.parent_cell_id_from_child_cell_id(…)", TEST_UNARY(ids.parent_cell_id_from_child_cell_id),
        "Ids.lowest_child_id_from_parent_id(…)",    TEST_UNARY(ids.lowest_child_id_from_parent_id),
        cell_ids ));

}


TEST_CASE( "generic orthtree::Ids2 properties", "[orthtree]" ) {
    test::GlmAdapter<int,double> exact(1e-7);
    orthtree::Ids2<int,double> ids(10.0, glm::vec2(-5.0));
    std::vector<int> memory_ids{0, 1, 2, 3, 4, 5, 6, 7, 8 , 9};
    std::vector<int> levels    {0, 1, 2, 3};
    std::vector<glm::vec2> positions {
        glm::vec2(0.0, 0.0),

        glm::vec2(-1.0, 0.0),
        glm::vec2(0.0, -1.0),
        glm::vec2(-1.0, -1.0),
        glm::vec2(1.0, 0.0),
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 1.0),
        glm::vec2(-1.0, 1.0),
        glm::vec2(1.0, -1.0),

        glm::vec2(-3.14, 0.0),
        glm::vec2(0.0, -3.14),
        glm::vec2(-3.14, -3.14),
        glm::vec2(3.14, 0.0),
        glm::vec2(0.0, 3.14),
        glm::vec2(3.14, 3.14),
        glm::vec2(-3.14, 3.14),
        glm::vec2(3.14, -3.14)
    };
    std::vector<double> level_cell_widths{
        10.0,
        5.0,
        2.5,
        1.25,
        0.625
    };

    for(int level:levels){
        REQUIRE(test::left_invertibility(exact,
            "Ids.memory_id_from_cell_id(…)",                                     [=](glm::ivec2 id){return ids.memory_id_from_cell_id(id,level);},
            "Ids.cell_id_from_memory_id(…) when restricted to valid memory ids", [=](int id)       {return ids.cell_id_from_memory_id(id,level);},
            memory_ids));
    }


    // REQUIRE(test::determinism(exact,
    //     "Ids.midpoint(…)", TEST_BINARY(ids.midpoint), 
    //     positions, level_cell_widths
    // ));

    // REQUIRE(test::determinism(exact,
    //     "Ids.interleaved_id(…)", TEST_BINARY(collignon::Ids(10).interleaved_id),
    //     ordinates, ordinates
    // ));

    // REQUIRE(test::codomain(exact,
    //     "within expected range",          TEST_RANGE(0, 100),
    //     "Ids.interleaved_id(…)", TEST_BINARY(interleaving.interleaved_id),
    //     ordinates, ordinates
    // ));

    // REQUIRE(test::codomain(exact,
    //     "within expected range",          TEST_RANGE(0, 10),
    //     "Ids.block_id(…)",       TEST_UNARY(interleaving.block_id), 
    //     ordinates
    // ));

    // REQUIRE(test::codomain(exact,
    //     "within expected range",          TEST_RANGE(0, 10),
    //     "Ids.element_id(…)",     TEST_UNARY(interleaving.element_id),
    //     ordinates
    // ));

    // SECTION("interleaved_id() must reconstruct input passed to block_id() / element_id() for any input"){
    //     collignon::Ids interleaving(10);
    //     for(int interleaved_id = 0; interleaved_id < 100; interleaved_id+=1){
    //         CHECK(interleaving.interleaved_id( interleaving.block_id(interleaved_id), interleaving.element_id(interleaved_id) )  == interleaved_id );
    //     }
    // }

    // SECTION("block_id() / element_id() must reconstruct input passed to interleaved_id() for any input"){
    //     collignon::Ids interleaving(10);
    //     for(int block_id = 0; block_id < 10; block_id+=1){
    //     for(int element_id = 0; element_id < 10; element_id+=1){
    //         int interleaved_id = interleaving.interleaved_id( block_id, element_id );
    //         CHECK( interleaving.block_id(interleaved_id) == block_id );
    //         CHECK( interleaving.element_id(interleaved_id) == element_id );
    //     }}
    // }

}

