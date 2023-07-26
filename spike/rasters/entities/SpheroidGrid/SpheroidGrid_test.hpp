

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
// #include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <meshes/mesh.hpp>  

#include "SpheroidGrid.hpp"
#include "../Grid/Raster.hpp"

#include "SpheroidGrid_test_utils.hpp"

using namespace rasters;

TEST_CASE( "SpheroidGrid consistency", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    SECTION("Components of grid must consist of mutually consistent container attributes"){
        CHECK(tetrahedron_grid.metrics->vertex_positions.size() == tetrahedron_grid.structure->vertex_count );
        CHECK(tetrahedron_grid.structure->face_vertex_ids.size() == tetrahedron_grid.structure->face_count );
        CHECK(tetrahedron_grid.structure->edge_vertex_ids.size() == tetrahedron_grid.structure->edge_count );
        CHECK(tetrahedron_grid.structure->arrow_vertex_ids.size() == tetrahedron_grid.structure->arrow_count );
    }
    SpheroidGrid icosahedron_grid(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    SECTION("Components of grid must consist of mutually consistent container attributes"){
        CHECK(icosahedron_grid.voronoi->get_value( icosahedron_grid.metrics->vertex_positions.front() ) == 0);
    }
}

TEST_CASE( "SpheroidGrid nontriviality", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    SECTION("SpheroidGrid attributes must contain nonzero elements"){
        CHECK(whole::sum(series::abs(tetrahedron_grid.metrics->vertex_areas)) > 0.01f);
        CHECK(whole::sum(series::abs(series::get_x(tetrahedron_grid.metrics->vertex_positions))) > 0.01f);
    }
}

TEST_CASE( "SpheroidGrid Raster consistency", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    auto a = make_Raster<float>(tetrahedron_grid);
    SECTION("Elements of a SpheroidGrid Raster must consist of mutually consistent container attributes"){
        CHECK(a.size() == a.grid.raster_size(rasters::mapping::cell) );
    }
}