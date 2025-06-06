#include "Grid.hpp"

TEST_CASE( "Grid arrow_offset_id()/arrow_offset_grid_position purity", "[collignon]" ) {
    SECTION("Grid arrow_offset_id()/arrow_offset_grid_position must be called repeatedly without changing the output"){
        float radius(2.0f);
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
        CHECK(grid.arrow_offset_id(glm::ivec2(1,0)) == 
              grid.arrow_offset_id(glm::ivec2(1,0)));
        CHECK(grid.arrow_offset_id(glm::ivec2(0,1)) == 
              grid.arrow_offset_id(glm::ivec2(0,1)));
        CHECK(grid.arrow_offset_id(glm::ivec2(-1,0)) == 
              grid.arrow_offset_id(glm::ivec2(-1,0)));
        CHECK(grid.arrow_offset_id(glm::ivec2(0,-1)) == 
              grid.arrow_offset_id(glm::ivec2(0,-1)));
        for(int i = 0; i < grid.arrows_per_vertex; ++i){
            CHECK(grid.arrow_offset_grid_position(i) == 
                  grid.arrow_offset_grid_position(i));
        }
    }
}

TEST_CASE( "Grid arrow_offset_id() / arrow_offset_grid_position() invertibility", "[collignon]" ) {
    SECTION("Grid arrow_offset_id() / arrow_offset_grid_position() must reconstruct input passed to interleaved_id() for any input"){
        float radius(2.0f);
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
        for(int memory_id = 0; memory_id < grid.arrows_per_vertex; memory_id+=1){
            CHECK( grid.arrow_offset_id( grid.arrow_offset_grid_position(memory_id)) == memory_id );
        }
    }
}


TEST_CASE( "Grid arrow_target/source_memory/grid_id() purity", "[collignon]" ) {
    SECTION("Grid arrow_target/source_memory/grid_id() must be called repeatedly without changing the output"){
        float radius(2.0f);
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
        for(int j = 0; j < grid.arrows_per_vertex; ++j){
            CHECK(grid.arrow_target_grid_id(i,j) == 
                  grid.arrow_target_grid_id(i,j));
            CHECK(grid.arrow_target_id(i,j) == 
                  grid.arrow_target_id(i,j));
        }}
    }
}


TEST_CASE( "Grid arrow_length purity", "[collignon]" ) {
    SECTION("Grid arrow_length must be called repeatedly without changing the output"){
        float radius(2.0f);
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
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
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
        for(int j = 0; j < grid.arrows_per_vertex; ++j){
            CHECK(grid.arrow_length(i,j) > 0);
        }}
    }
}


TEST_CASE( "Grid arrow_length accuracy", "[collignon]" ) {
    SECTION("Grid arrow_length must fall within a narrow range of an expected value"){
        float radius(2.0f);
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
        float expected(grid.total_circumference() / float(vertices_per_half_meridian));
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
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
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
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
        for(int j = 0; j < grid.arrows_per_vertex; ++j){
            CHECK(grid.arrow_dual_length(i,j) > 0);
        }}
    }
}


TEST_CASE( "Grid arrow_dual_length accuracy", "[collignon]" ) {
    SECTION("Grid arrow_dual_length must fall within a narrow range of an expected value"){
        float radius(2.0f);
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
        float expected(grid.total_circumference() / float(vertices_per_half_meridian));
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
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
            CHECK(grid.vertex_dual_area(i) == 
                  grid.vertex_dual_area(i));
        }
    }
}

TEST_CASE( "Grid vertex_dual_area codomain", "[collignon]" ) {
    SECTION("Grid vertex_dual_area must output only positive values"){
        float radius(2.0f);
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
        for(int i = 0; i < grid.vertex_count(); ++i){
            CHECK(grid.vertex_dual_area(i) > 0);
        }
    }
}

TEST_CASE( "Grid vertex_dual_area accuracy", "[collignon]" ) {
    SECTION("Grid vertex_dual_area must fall within a narrow range of an expected value"){
        float radius(2.0f);
        int vertices_per_half_meridian(10);
        collignon::Grid grid(radius, vertices_per_half_meridian);
        float expected(grid.total_area() / float(grid.vertex_count()));
        float sae(0.0);
        for(int i = 0; i < grid.vertex_count(); ++i){
            sae += std::abs(grid.vertex_dual_area(i) - expected);
        }
        sae /= grid.vertex_count();
        CHECK(sae < 0.03*expected);
    }
}
