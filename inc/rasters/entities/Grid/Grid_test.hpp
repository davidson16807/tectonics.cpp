

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
// #include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <many/types.hpp>
#include <many/common.hpp>
#include <many/glm/types.hpp>
#include <many/glm/convenience.hpp>

#include <meshes/mesh.hpp>

#include "Grid.hpp"

#include "Grid_test_utils.hpp"

using namespace rasters;

TEST_CASE( "Grid consistency", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    SECTION("Components of grid must consist of mutually consistent container attributes"){
        CHECK(tetrahedron_grid.cache->vertex_positions.size() == tetrahedron_grid.cache->vertex_count );
        CHECK(tetrahedron_grid.cache->face_vertex_ids.size() == tetrahedron_grid.cache->face_count );
        CHECK(tetrahedron_grid.cache->edge_vertex_ids.size() == tetrahedron_grid.cache->edge_count );
        CHECK(tetrahedron_grid.cache->arrow_vertex_ids.size() == tetrahedron_grid.cache->arrow_count );
    }
}

TEST_CASE( "Grid nontriviality", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    SECTION("Grid attributes must contain nonzero elements"){
        CHECK(many::sum(many::abs(tetrahedron_grid.cache->vertex_areas)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron_grid.cache->vertex_positions))) > 0.01f);
    }
}

TEST_CASE( "Raster consistency", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    auto a = make_raster<float>(tetrahedron_grid);
    SECTION("Elements of raster of grid must consist of mutually consistent container attributes"){
        CHECK(a.size() == a.grid.cell_count() );
    }
}