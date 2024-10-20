
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

// in-house libraries
#include <test/macros.hpp>
#include <test/properties.hpp>
#include <test/adapter.hpp>
#include <test/glm/adapter.hpp>

#include <index/procedural/Range.hpp>

#include "Grid.hpp"

TEST_CASE( "Grid nearest_vertex_id() / vertex_position()", "[dymaxion]" ) {

    double radius(2.0);
    int vertices_per_square_side(40);

    test::GlmAdapter<int,double> precise(1e-7);

    dymaxion::Grid grid(radius, vertices_per_square_side);

    procedural::Range vertex_ids(grid.vertex_count());

    REQUIRE(test::left_invertibility(precise,
        "Grid.nearest_vertex_id(…)", TEST_UNARY(grid.nearest_vertex_id),
        "Grid.vertex_position(…)",   TEST_UNARY(grid.vertex_position),
        vertex_ids
    ));

}

TEST_CASE( "Grid arrow_offset_id() / arrow_offset_grid_position()", "[dymaxion]" ) {

    double radius(2.0);
    int vertices_per_square_side(40);

    test::GlmAdapter<int,double> precise(1e-7);
    test::GlmAdapter<int,double> imprecise(1e-4);

    std::vector<glm::ivec2> arrow_offset_grid_ids
    {
        glm::ivec2(1,0),
        glm::ivec2(1,0),
        glm::ivec2(0,1),
        glm::ivec2(0,1),
        glm::ivec2(-1,0),
        glm::ivec2(-1,0),
        glm::ivec2(0,-1),
        glm::ivec2(0,-1)
    };

    procedural::Range arrow_offset_ids(4);

    dymaxion::Grid grid(radius, vertices_per_square_side);

    REQUIRE(test::determinism(precise,
        "Grid.arrow_offset_id(…)", TEST_UNARY(grid.arrow_offset_id),
        arrow_offset_grid_ids
    ));

    REQUIRE(test::determinism(precise,
        "Grid.arrow_offset_grid_position(…)",   TEST_UNARY(grid.arrow_offset_grid_position),
        arrow_offset_ids
    ));

    REQUIRE(test::left_invertibility(precise,
        "Grid.arrow_offset_id(…)",      TEST_UNARY(grid.arrow_offset_id),
        "Grid.arrow_offset_grid_position(…)",  TEST_UNARY(grid.arrow_offset_grid_position),
        arrow_offset_ids
    ));

    REQUIRE(test::left_invertibility(precise,
        "Grid.arrow_offset_grid_position(…)", TEST_UNARY(grid.arrow_offset_grid_position),
        "Grid.arrow_offset_id(…)",     TEST_UNARY(grid.arrow_offset_id),
        arrow_offset_grid_ids
    ));

}

TEST_CASE( "Grid arrow properties", "[dymaxion]" ) {

    double radius(2.0);
    int vertices_per_square_side(40);

    test::GlmAdapter<int,double> precise(1e-7);
    test::GlmAdapter<int,double> imprecise(1e-4);

    dymaxion::Grid grid(radius, vertices_per_square_side);

    procedural::Range vertex_ids(grid.vertex_count());
    procedural::Range arrow_offset_ids(4);

    REQUIRE(test::determinism(precise,
        "Grid.arrow_target_id(…)", TEST_BINARY(grid.arrow_target_id),
        vertex_ids,
        arrow_offset_ids
    ));

    REQUIRE(test::determinism(precise,
        "Grid.arrow_length(…)", TEST_BINARY(grid.arrow_length),
        vertex_ids,
        arrow_offset_ids
    ));

    REQUIRE(test::determinism(precise,
        "Grid.arrow_dual_length(…)", TEST_BINARY(grid.arrow_dual_length),
        vertex_ids,
        arrow_offset_ids
    ));

    REQUIRE(test::determinism(precise,
        "Grid.vertex_dual_area(…)", TEST_UNARY(grid.vertex_dual_area),
        vertex_ids
    ));

    REQUIRE(test::codomain(precise,
        "positive", [=](auto y){ return y > 0; },
        "Grid.arrow_length(…)", TEST_BINARY(grid.arrow_length),
        vertex_ids,
        arrow_offset_ids
    ));

    REQUIRE(test::codomain(precise,
        "within a narrow fraction of an expected value", 
        [=](auto y){ 
            double expected(grid.total_circumference() / (2.0*grid.vertices_per_meridian()));
            return (std::abs(y-expected)/expected) < 2.5; 
        },
        "Grid.arrow_length(…)", TEST_BINARY(grid.arrow_length),
        vertex_ids,
        arrow_offset_ids
    ));

    REQUIRE(test::codomain(precise,
        "positive", [=](auto y){ return y > 0; },
        "Grid.arrow_dual_length(…)", TEST_BINARY(grid.arrow_dual_length),
        vertex_ids,
        arrow_offset_ids
    ));

    REQUIRE(test::codomain(precise,
        "within a narrow fraction of an expected value", 
        [=](auto y){ 
            double expected(grid.total_circumference() / (2.0*grid.vertices_per_meridian()));
            return (std::abs(y-expected)/expected) < 2.5; 
        },
        "Grid.arrow_dual_length(…)", TEST_BINARY(grid.arrow_dual_length),
        vertex_ids,
        arrow_offset_ids
    ));

    REQUIRE(test::codomain(precise,
        "positive", [=](auto y){ return y > 0; },
        "Grid.vertex_dual_area(…)", TEST_UNARY(grid.vertex_dual_area),
        vertex_ids
    ));

    REQUIRE(test::codomain(precise,
        "within a narrow fraction of an expected value", 
        [=](auto y){ 
            double expected(grid.total_area() / float(grid.vertex_count()));
            return (std::abs(y-expected)/expected) < 0.5; 
        },
        "Grid.vertex_dual_area(…)", TEST_UNARY(grid.vertex_dual_area),
        vertex_ids
    ));

}

/*
*/

