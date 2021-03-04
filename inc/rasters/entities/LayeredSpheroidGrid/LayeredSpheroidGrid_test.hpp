

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

// in-house libraries
#include <series/series.hpp>  
#include <series/convenience.hpp>  
#include <series/glm/glm.hpp>         // *vec*s
#include <series/glm/convenience.hpp> //  operators, etc.

#include <meshes/mesh.hpp>

#include "LayeredSpheroidGrid.hpp"
#include "../Grid/Raster.hpp"

#include "LayeredSpheroidGrid_test_utils.hpp"

using namespace rasters;

TEST_CASE( "LayeredSpheroidGrid consistency", "[rasters]" ) {
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

TEST_CASE( "LayeredSpheroidGrid nontriviality", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    SECTION("LayeredSpheroidGrid attributes must contain nonzero elements"){
        CHECK(series::sum(series::abs(tetrahedron_grid.metrics->vertex_areas)) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(tetrahedron_grid.metrics->vertex_positions))) > 0.01f);
    }
}

TEST_CASE( "LayeredSpheroidGrid Raster consistency", "[rasters]" ) {
    Grid tetrahedron_grid(meshes::tetrahedron.vertices, meshes::tetrahedron.faces);
    auto a = make_Raster<float>(tetrahedron_grid);
    SECTION("Elements of a LayeredSpheroidGrid Raster must consist of mutually consistent container attributes"){
        CHECK(a.size() == a.grid.cell_count(rasters::mapping::cell) );
    }
}