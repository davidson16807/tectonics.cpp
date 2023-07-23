

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

#include "LayeredGrid.hpp"

#include "LayeredGrid_test_utils.hpp"

using namespace rasters;


//More thorough tests are available in components/*_test.hpp files,
//these tests simply establish that the components have been instantiated
//and with the correct paramater values

TEST_CASE( "Layered Grid predictability", "[rasters]" ) {
    SECTION("Layered Grid must consist of mutually consistent container attributes"){
        CHECK(layered_tetrahedron_grid.cell_count(mapping::cell) == 4*layered_tetrahedron_grid.layering->layer_count);
        CHECK(layered_tetrahedron_grid.cell_count(mapping::arrow) == 12*layered_tetrahedron_grid.layering->layer_count);
    }
}

TEST_CASE( "Layered Grid consistency", "[rasters]" ) {
    SECTION("Components of layered grid must consist of mutually consistent container attributes"){
        CHECK(layered_tetrahedron_grid.metrics->vertex_positions.size() == layered_tetrahedron_grid.structure->vertex_count );
        CHECK(layered_tetrahedron_grid.structure->face_vertex_ids.size() == layered_tetrahedron_grid.structure->face_count );
        CHECK(layered_tetrahedron_grid.structure->edge_vertex_ids.size() == layered_tetrahedron_grid.structure->edge_count );
        CHECK(layered_tetrahedron_grid.structure->arrow_vertex_ids.size() == layered_tetrahedron_grid.structure->arrow_count );
    }
}

TEST_CASE( "Layered Grid nontriviality", "[rasters]" ) {
    SECTION("Layered Grid attributes must contain nonzero elements"){
        CHECK(series::sum(series::abs(layered_tetrahedron_grid.metrics->vertex_areas)) > 0.01f);
        CHECK(series::sum(series::abs(series::get_x(layered_tetrahedron_grid.metrics->vertex_positions))) > 0.01f);
    }
}
