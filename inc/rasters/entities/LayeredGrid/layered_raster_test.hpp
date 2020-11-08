

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
#include "../raster.hpp"

#include "LayeredGrid.hpp"

#include "LayeredGrid_test_utils.hpp"

using namespace rasters;


//More thorough tests are available in components/*_test.hpp files,
//these tests simply establish that the components have been instantiated
//and with the correct paramater values

TEST_CASE( "LayeredRaster consistency", "[rasters]" ) {
    auto a = make_raster<float>(layered_tetrahedron_grid);
    auto b = make_raster<float, mapping::arrow>(layered_tetrahedron_grid);
    SECTION("Elements of raster of layered grid must consist of mutually consistent container attributes"){
        CHECK(a.size() == a.grid.cell_count(mapping::cell) );
        CHECK(b.size() == b.grid.cell_count(mapping::arrow) );
    }
}