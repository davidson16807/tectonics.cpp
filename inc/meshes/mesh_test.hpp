

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec3.hpp>               // *vec3

#include <many/convenience.hpp>  
#include <many/glm/convenience.hpp> //  operators, etc.

#include <meshes/mesh.hpp>

TEST_CASE( "mesh subdivision purity", "[rasters]" ) {
    SECTION("subdivide(mesh) must be called repeatedly without changing the output"){
        meshes::mesh icosphere1 = meshes::subdivide(meshes::tetrahedron);
        meshes::mesh icosphere2 = meshes::subdivide(meshes::tetrahedron);
        CHECK(icosphere1.vertices==icosphere2.vertices);
        CHECK(icosphere1.faces==icosphere2.faces);
    }
}
TEST_CASE( "mesh subdivision correctness", "[rasters]" ) {
    SECTION("subdivide(mesh) must generate a sensible number of vertices and faces"){
        meshes::mesh icosphere = meshes::subdivide(meshes::tetrahedron);
        CHECK(icosphere.vertices.size() == 10);
        CHECK(icosphere.faces.size() == 4 * meshes::tetrahedron.faces.size());
    }
}
