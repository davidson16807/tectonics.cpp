

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/convenience.hpp> //  operators, etc.

#include <meshes/mesh.hpp>

#include "LayeredSpheroidGrid.hpp"
#include "../Grid/Raster.hpp"

#include "LayeredSpheroidGrid_test_utils.hpp"

using namespace rasters;

TEST_CASE( "LayeredSpheroidGrid consistency", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    SECTION("Components of grid must consist of mutually consistent container attributes"){
        CHECK(tetrahedron_grid.cache->vertex_positions.size() == tetrahedron_grid.cache->vertex_count );
        CHECK(tetrahedron_grid.cache->face_vertex_ids.size() == tetrahedron_grid.cache->face_count );
        CHECK(tetrahedron_grid.cache->edge_vertex_ids.size() == tetrahedron_grid.cache->edge_count );
        CHECK(tetrahedron_grid.cache->arrow_vertex_ids.size() == tetrahedron_grid.cache->arrow_count );
    }
    SpheroidGrid icosahedron_grid(meshes::icosahedron.vertices, meshes::icosahedron.faces);
    SECTION("Components of grid must consist of mutually consistent container attributes"){
        CHECK(icosahedron_grid.voronoi->get_value( icosahedron_grid.cache->vertex_positions.front() ) == 0);
    }
}

TEST_CASE( "LayeredSpheroidGrid nontriviality", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    SECTION("LayeredSpheroidGrid attributes must contain nonzero elements"){
        CHECK(many::sum(many::abs(tetrahedron_grid.cache->vertex_areas)) > 0.01f);
        CHECK(many::sum(many::abs(many::get_x(tetrahedron_grid.cache->vertex_positions))) > 0.01f);
    }
}

TEST_CASE( "LayeredSpheroidGrid Raster consistency", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    auto a = make_Raster<float>(tetrahedron_grid);
    SECTION("Elements of a LayeredSpheroidGrid Raster must consist of mutually consistent container attributes"){
        CHECK(a.size() == a.grid.cell_count(rasters::mapping::cell) );
    }
}