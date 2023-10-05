
// in-house libraries
#include <test/macros.hpp>
#include <test/properties.hpp>
#include <test/adapter.hpp>
#include <test/glm/adapter.hpp>

#include <store/series/Range.hpp>

#include "Grid.hpp"

TEST_CASE( "Grid arrow_offset_memory_id() / arrow_offset_grid_position()", "[dymaxion]" ) {

    double radius(2.0);
    int vertex_count_per_meridian(20);

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

    series::Range arrow_offset_memory_ids(4);

    dymaxion::Grid grid(radius, vertex_count_per_meridian);

    REQUIRE(test::determinism(precise,
        "Grid.arrow_offset_memory_id(…)", TEST_UNARY(grid.arrow_offset_memory_id),
        arrow_offset_grid_ids
    ));

    REQUIRE(test::determinism(precise,
        "Grid.arrow_offset_grid_position(…)",   TEST_UNARY(grid.arrow_offset_grid_position),
        arrow_offset_memory_ids
    ));

    REQUIRE(test::left_invertibility(precise,
        "Grid.arrow_offset_memory_id(…)", TEST_UNARY(grid.arrow_offset_memory_id),
        "Grid.arrow_offset_grid_position(…)",   TEST_UNARY(grid.arrow_offset_grid_position),
        arrow_offset_memory_ids
    ));

    REQUIRE(test::left_invertibility(precise,
        "Grid.arrow_offset_grid_position(…)",   TEST_UNARY(grid.arrow_offset_grid_position),
        "Grid.arrow_offset_memory_id(…)", TEST_UNARY(grid.arrow_offset_memory_id),
        arrow_offset_grid_ids
    ));

}

/*



TEST_CASE( "Grid arrow_target/source_memory/grid_id() purity", "[collignon]" ) {
    SECTION("Grid arrow_target/source_memory/grid_id() must be called repeatedly without changing the output"){
        float radius(2.0f);
        int vertex_count_per_half_meridian(10);
        collignon::Grid grid(radius, vertex_count_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
        for(int j = 0; j < grid.arrows_per_vertex; ++j){
            CHECK(grid.arrow_target_grid_id(i,j) == 
                  grid.arrow_target_grid_id(i,j));
            CHECK(grid.arrow_target_memory_id(i,j) == 
                  grid.arrow_target_memory_id(i,j));
        }}
    }
}


TEST_CASE( "Grid arrow_length purity", "[collignon]" ) {
    SECTION("Grid arrow_length must be called repeatedly without changing the output"){
        float radius(2.0f);
        int vertex_count_per_half_meridian(10);
        collignon::Grid grid(radius, vertex_count_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
        for(int j = 0; j < grid.arrows_per_vertex; ++j){
            CHECK(grid.arrow_length(i,j) == 
                  grid.arrow_length(i,j));
        }}
    }
}

TEST_CASE( "Grid arrow_length codomain", "[collignon]" ) {
    SECTION("Grid arrow_length must output only positive values"){
        float radius(2.0f);
        int vertex_count_per_half_meridian(10);
        collignon::Grid grid(radius, vertex_count_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
        for(int j = 0; j < grid.arrows_per_vertex; ++j){
            CHECK(grid.arrow_length(i,j) > 0);
        }}
    }
}


TEST_CASE( "Grid arrow_length accuracy", "[collignon]" ) {
    SECTION("Grid arrow_length must fall within a narrow range of an expected value"){
        float radius(2.0f);
        int vertex_count_per_half_meridian(10);
        collignon::Grid grid(radius, vertex_count_per_half_meridian);
        float expected(grid.total_circumference() / float(vertex_count_per_half_meridian));
        float sae(0.0);
        for(int i = 0; i < grid.vertex_count(); ++i){
        for(int j = 0; j < grid.arrows_per_vertex; ++j){
            sae += std::abs(grid.arrow_length(i,j) - expected);
        }}
        sae /= grid.arrow_count();
        CHECK(sae < 1.0*expected);
    }
}


TEST_CASE( "Grid arrow_dual_length purity", "[collignon]" ) {
    SECTION("Grid arrow_dual_length must be called repeatedly without changing the output"){
        float radius(2.0f);
        int vertex_count_per_half_meridian(10);
        collignon::Grid grid(radius, vertex_count_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
        for(int j = 0; j < grid.arrows_per_vertex; ++j){
            CHECK(grid.arrow_dual_length(i,j) == 
                  grid.arrow_dual_length(i,j));
        }}
    }
}

TEST_CASE( "Grid arrow_dual_length codomain", "[collignon]" ) {
    SECTION("Grid arrow_dual_length must output only positive values"){
        float radius(2.0f);
        int vertex_count_per_half_meridian(10);
        collignon::Grid grid(radius, vertex_count_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
        for(int j = 0; j < grid.arrows_per_vertex; ++j){
            CHECK(grid.arrow_dual_length(i,j) > 0);
        }}
    }
}


TEST_CASE( "Grid arrow_dual_length accuracy", "[collignon]" ) {
    SECTION("Grid arrow_dual_length must fall within a narrow range of an expected value"){
        float radius(2.0f);
        int vertex_count_per_half_meridian(10);
        collignon::Grid grid(radius, vertex_count_per_half_meridian);
        float expected(grid.total_circumference() / float(vertex_count_per_half_meridian));
        float sae(0.0);
        for(int i = 0; i < grid.vertex_count(); ++i){
        for(int j = 0; j < grid.arrows_per_vertex; ++j){
            sae += std::abs(grid.arrow_dual_length(i,j) - expected);
        }}
        sae /= grid.arrow_count();
        CHECK(sae < 1.0*expected);
    }
}


TEST_CASE( "Grid vertex_dual_area purity", "[collignon]" ) {
    SECTION("Grid vertex_dual_area must be called repeatedly without changing the output"){
        float radius(2.0f);
        int vertex_count_per_half_meridian(10);
        collignon::Grid grid(radius, vertex_count_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
            CHECK(grid.vertex_dual_area(i) == 
                  grid.vertex_dual_area(i));
        }
    }
}

TEST_CASE( "Grid vertex_dual_area codomain", "[collignon]" ) {
    SECTION("Grid vertex_dual_area must output only positive values"){
        float radius(2.0f);
        int vertex_count_per_half_meridian(10);
        collignon::Grid grid(radius, vertex_count_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
            CHECK(grid.vertex_dual_area(i) > 0);
        }
    }
}

TEST_CASE( "Grid vertex_dual_area accuracy", "[collignon]" ) {
    SECTION("Grid vertex_dual_area must fall within a narrow range of an expected value"){
        float radius(2.0f);
        int vertex_count_per_half_meridian(10);
        collignon::Grid grid(radius, vertex_count_per_half_meridian);
        float expected(grid.total_area() / float(grid.vertex_count()));
        float sae(0.0);
        for(int i = 0; i < grid.vertex_count(); ++i){
            sae += std::abs(grid.vertex_dual_area(i) - expected);
        }
        sae /= grid.vertex_count();
        CHECK(sae < 0.03*expected);
    }
}
*/