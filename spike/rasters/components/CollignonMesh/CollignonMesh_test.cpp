
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>             // *vec3
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries
#include "CollignonMesh.hpp"


TEST_CASE( "CollignonMesh.sphere_position() purity", "[rasters]" ) {
    SECTION("CollignonMesh.sphere_position() must be called repeatedly without changing the output"){
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            CHECK(rasters::CollignonMesh(2.0f, 10).sphere_position(glm::vec2(x,y)) == 
                  rasters::CollignonMesh(2.0f, 10).sphere_position(glm::vec2(x,y)));
        }}
    }
}
TEST_CASE( "CollignonMesh sphere_position() closeness preservation", "[rasters]" ) {
    SECTION("changes in grid_position must not result in changes to sphere_position that exceed a reasonable multiple"){
        rasters::CollignonMesh mesh(2.0f, 10);
        const float factor(3.0*2.0/10.0);
        const glm::vec2 dx(0.01, 0.0);
        const glm::vec2 dy(0.0, 0.01);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v+dx)) < factor * glm::length(dx) );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v-dx)) < factor * glm::length(dx) );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v+dy)) < factor * glm::length(dy) );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v-dy)) < factor * glm::length(dy) );
        }}
    }
}

TEST_CASE( "CollignonMesh sphere_position() congruence", "[rasters]" ) {
    SECTION("a modulo can be applied to input which results in the same output"){
        rasters::CollignonMesh mesh(2.0f, 10);
        const float epsilon(1e-4f);
        const glm::vec2 nx(40, 0);
        const glm::vec2 ny(0, 40);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v+nx)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v-nx)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v+ny)) < epsilon );
            CHECK( glm::distance(mesh.sphere_position(v), mesh.sphere_position(v-ny)) < epsilon );
        }}
    }
}




TEST_CASE( "CollignonMesh.grid_position() purity", "[rasters]" ) {
    SECTION("CollignonMesh.grid_position() must be called repeatedly without changing the output"){
        rasters::CollignonMesh mesh(2.0f, 10);
        const float epsilon(1e-4f);
        for(float x = -2.0f; x < 2.0f; x+=0.5f){
        for(float y = -2.0f; y < 2.0f; y+=0.5f){
        for(float z = -2.0f; z < 2.0f; z+=0.5f){
            glm::vec3 v(x,y,z);
            if(glm::length(v) > epsilon){
                CHECK(rasters::CollignonMesh(2.0f, 10).grid_position(glm::vec3(x,y,z)) == 
                      rasters::CollignonMesh(2.0f, 10).grid_position(glm::vec3(x,y,z)));
            }
        }}}
    }
}

TEST_CASE( "CollignonMesh grid_position() / sphere_position() invertibility", "[rasters]" ) {
    SECTION("CollignonMesh.sphere_position() must reconstruct input passed to grid_position() for any unit vector"){
        rasters::CollignonMesh mesh(2.0f, 10);
        const float epsilon(1e-4f);
        for(float x = -2.0f; x < 2.0f; x+=0.5f){
        for(float y = -2.0f; y < 2.0f; y+=0.5f){
        for(float z = -2.0f; z < 2.0f; z+=0.5f){
            if(glm::length(glm::vec3(x,y,z)) > epsilon){
                glm::vec3 v = 2.0f*glm::normalize(glm::vec3(x,y,z));
                glm::vec2 u = mesh.grid_position(v);
                glm::vec3 v2 = mesh.sphere_position( u );
                CHECK( v2.x == Approx(v.x).margin(epsilon) );
                CHECK( v2.y == Approx(v.y).margin(epsilon) );
                CHECK( v2.z == Approx(v.z).margin(epsilon) );
            }
        }}}
    }
}





TEST_CASE( "CollignonMesh memory_id() / grid_position() invertibility", "[rasters]" ) {
    SECTION("CollignonMesh.grid_position() must reconstruct input passed to memory_id() for any unit vector"){
        rasters::CollignonMesh mesh(2.0f, 10);
        for(int i = 0; i < mesh.vertex_count; i++){
            CHECK( i == mesh.memory_id(mesh.grid_position(i)) );
        }
    }
}
TEST_CASE( "CollignonMesh memory_id() congruence", "[rasters]" ) {
    SECTION("an modulo can be applied to input which results in the same output"){
        rasters::CollignonMesh mesh(2.0f, 10);
        const glm::vec2 nx(40, 0);
        const glm::vec2 ny(0, 40);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( mesh.memory_id(v) == mesh.memory_id(v+nx));
            CHECK( mesh.memory_id(v) == mesh.memory_id(v-nx));
            CHECK( mesh.memory_id(v) == mesh.memory_id(v+ny));
            CHECK( mesh.memory_id(v) == mesh.memory_id(v-ny));
        }}
    }
}
TEST_CASE( "CollignonMesh memory_id() range restrictions", "[rasters]" ) {
    SECTION("CollignonMesh.memory_id() must not produce results outside valid range"){
        rasters::CollignonMesh mesh(2.0f, 10);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            int i = mesh.memory_id(glm::vec2(x,y));
            CHECK( 0 <= i );
            CHECK( i < mesh.vertex_count );
        }}
    }
}
/*
*/