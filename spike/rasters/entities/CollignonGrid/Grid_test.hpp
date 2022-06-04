

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
// #include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <series/types.hpp>
#include <series/common.hpp>
#include <series/glm/types.hpp>
#include <series/glm/convenience.hpp>

#include <meshes/mesh.hpp>

#include "Grid.hpp"

#include "Grid_test_utils.hpp"

using namespace rasters;

TEST_CASE( "Grid predictability", "[rasters]" ) {
    SECTION("Grid must consist of mutually consistent container attributes"){
        CHECK(tetrahedron_grid.cell_count(rasters::mapping::cell) == 4);
        CHECK(tetrahedron_grid.cell_count(rasters::mapping::arrow) == 12);
    }
}

TEST_CASE( "Grid consistency", "[rasters]" ) {
    SECTION("Components of grid must consist of mutually consistent container attributes"){
        CHECK(tetrahedron_grid.metrics->vertex_positions.size() == tetrahedron_grid.structure->vertex_count );
        CHECK(tetrahedron_grid.structure->face_vertex_ids.size() == tetrahedron_grid.structure->face_count );
        CHECK(tetrahedron_grid.structure->edge_vertex_ids.size() == tetrahedron_grid.structure->edge_count );
        CHECK(tetrahedron_grid.structure->arrow_vertex_ids.size() == tetrahedron_grid.structure->arrow_count );
    }
}

TEST_CASE( "Grid nontriviality", "[rasters]" ) {
    SECTION("Grid attributes must contain nonzero elements"){
        CHECK(series::sum(series::abs(tetrahedron_grid.metrics->vertex_areas)) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_grid.metrics->vertex_positions))) > 0.01f);
    }
}