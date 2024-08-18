
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>             // *vec3
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries
#include "Voronoi.hpp"


TEST_CASE( "Voronoi.sphere_position() purity", "[collignon]" ) {
    SECTION("Voronoi.sphere_position() must be called repeatedly without changing the output"){
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            CHECK(collignon::Voronoi(2.0f, 10).sphere_position(glm::vec2(x,y)) == 
                  collignon::Voronoi(2.0f, 10).sphere_position(glm::vec2(x,y)));
        }}
    }
}
TEST_CASE( "Voronoi sphere_position() closeness preservation", "[collignon]" ) {
    SECTION("changes in grid_position must not result in changes to sphere_position that exceed a reasonable multiple"){
        collignon::Voronoi voronoi(2.0f, 10);
        const float factor(3.0*2.0/10.0);
        const glm::vec2 dx(0.01, 0.0);
        const glm::vec2 dy(0.0, 0.01);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(voronoi.sphere_position(v), voronoi.sphere_position(v+dx)) < factor * glm::length(dx) );
            CHECK( glm::distance(voronoi.sphere_position(v), voronoi.sphere_position(v-dx)) < factor * glm::length(dx) );
            CHECK( glm::distance(voronoi.sphere_position(v), voronoi.sphere_position(v+dy)) < factor * glm::length(dy) );
            CHECK( glm::distance(voronoi.sphere_position(v), voronoi.sphere_position(v-dy)) < factor * glm::length(dy) );
        }}
    }
}

TEST_CASE( "Voronoi sphere_position() congruence", "[collignon]" ) {
    SECTION("a modulo can be applied to input which results in the same output"){
        collignon::Voronoi voronoi(2.0f, 10);
        const float epsilon(1e-4f);
        const glm::vec2 nx(40, 0);
        const glm::vec2 ny(0, 40);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(voronoi.sphere_position(v), voronoi.sphere_position(v+nx)) < epsilon );
            CHECK( glm::distance(voronoi.sphere_position(v), voronoi.sphere_position(v-nx)) < epsilon );
            CHECK( glm::distance(voronoi.sphere_position(v), voronoi.sphere_position(v+ny)) < epsilon );
            CHECK( glm::distance(voronoi.sphere_position(v), voronoi.sphere_position(v-ny)) < epsilon );
        }}
    }
}


TEST_CASE( "Voronoi grid_position() / sphere_position() invertibility", "[collignon]" ) {
    SECTION("Voronoi.sphere_position() must reconstruct input passed to grid_position() for any unit vector"){
        const float radius = 2.0f;
        collignon::Voronoi voronoi(radius, 10);
        const float epsilon(1e-4f);
        for(float x = -2.0f; x < 2.0f; x+=0.5f){
        for(float y = -2.0f; y < 2.0f; y+=0.5f){
        for(float z = -2.0f; z < 2.0f; z+=0.5f){
            if(glm::length(glm::vec3(x,y,z)) > epsilon){
                glm::vec3 v = radius*glm::normalize(glm::vec3(x,y,z));
                glm::vec2 u = voronoi.grid_position(v);
                glm::vec3 v2 = voronoi.sphere_position( u );
                CHECK( v2.x == Approx(v.x).margin(epsilon) );
                CHECK( v2.y == Approx(v.y).margin(epsilon) );
                CHECK( v2.z == Approx(v.z).margin(epsilon) );
            }
        }}}
    }
}





TEST_CASE( "Voronoi.sphere_normal() purity", "[collignon]" ) {
    SECTION("Voronoi.sphere_normal() must be called repeatedly without changing the output"){
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            CHECK(collignon::Voronoi(2.0f, 10).sphere_normal(glm::vec2(x,y)) == 
                  collignon::Voronoi(2.0f, 10).sphere_normal(glm::vec2(x,y)));
        }}
    }
}
TEST_CASE( "Voronoi sphere_normal() closeness preservation", "[collignon]" ) {
    SECTION("changes in grid_position must not result in changes to sphere_normal that exceed a reasonable multiple"){
        collignon::Voronoi voronoi(2.0f, 10);
        const float factor(3.0*2.0/10.0);
        const glm::vec2 dx(0.01, 0.0);
        const glm::vec2 dy(0.0, 0.01);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(voronoi.sphere_normal(v), voronoi.sphere_normal(v+dx)) < factor * glm::length(dx) );
            CHECK( glm::distance(voronoi.sphere_normal(v), voronoi.sphere_normal(v-dx)) < factor * glm::length(dx) );
            CHECK( glm::distance(voronoi.sphere_normal(v), voronoi.sphere_normal(v+dy)) < factor * glm::length(dy) );
            CHECK( glm::distance(voronoi.sphere_normal(v), voronoi.sphere_normal(v-dy)) < factor * glm::length(dy) );
        }}
    }
}

TEST_CASE( "Voronoi sphere_normal() congruence", "[collignon]" ) {
    SECTION("a modulo can be applied to input which results in the same output"){
        collignon::Voronoi voronoi(2.0f, 10);
        const float epsilon(1e-4f);
        const glm::vec2 nx(40, 0);
        const glm::vec2 ny(0, 40);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(voronoi.sphere_normal(v), voronoi.sphere_normal(v+nx)) < epsilon );
            CHECK( glm::distance(voronoi.sphere_normal(v), voronoi.sphere_normal(v-nx)) < epsilon );
            CHECK( glm::distance(voronoi.sphere_normal(v), voronoi.sphere_normal(v+ny)) < epsilon );
            CHECK( glm::distance(voronoi.sphere_normal(v), voronoi.sphere_normal(v-ny)) < epsilon );
        }}
    }
}


TEST_CASE( "Voronoi grid_position() / sphere_normal() invertibility", "[collignon]" ) {
    SECTION("Voronoi.sphere_normal() must reconstruct input passed to grid_position() for any unit vector"){
        collignon::Voronoi voronoi(2.0f, 10);
        const float epsilon(1e-4f);
        for(float x = -2.0f; x < 2.0f; x+=0.5f){
        for(float y = -2.0f; y < 2.0f; y+=0.5f){
        for(float z = -2.0f; z < 2.0f; z+=0.5f){
            if(glm::length(glm::vec3(x,y,z)) > epsilon){
                glm::vec3 v = glm::normalize(glm::vec3(x,y,z));
                glm::vec2 u = voronoi.grid_position(v);
                glm::vec3 v2 = voronoi.sphere_normal( u );
                CHECK( v2.x == Approx(v.x).margin(epsilon) );
                CHECK( v2.y == Approx(v.y).margin(epsilon) );
                CHECK( v2.z == Approx(v.z).margin(epsilon) );
            }
        }}}
    }
}



TEST_CASE( "Voronoi.grid_position() purity", "[collignon]" ) {
    SECTION("Voronoi.grid_position() must be called repeatedly without changing the output"){
        collignon::Voronoi voronoi(2.0f, 10);
        const float epsilon(1e-4f);
        for(float x = -2.0f; x < 2.0f; x+=0.5f){
        for(float y = -2.0f; y < 2.0f; y+=0.5f){
        for(float z = -2.0f; z < 2.0f; z+=0.5f){
            glm::vec3 v(x,y,z);
            if(glm::length(v) > epsilon){
                CHECK(collignon::Voronoi(2.0f, 10).grid_position(glm::vec3(x,y,z)) == 
                      collignon::Voronoi(2.0f, 10).grid_position(glm::vec3(x,y,z)));
            }
        }}}
    }
}





TEST_CASE( "Voronoi memory_id() / grid_position() invertibility", "[collignon]" ) {
    SECTION("Voronoi.grid_position() must reconstruct input passed to memory_id() for any unit vector"){
        collignon::Voronoi voronoi(2.0f, 10);
        for(int i = 0; i < voronoi.vertex_count; i++){
            CHECK( i == voronoi.memory_id(voronoi.grid_position(i)) );
        }
    }
}
TEST_CASE( "Voronoi memory_id() congruence", "[collignon]" ) {
    SECTION("an modulo can be applied to input which results in the same output"){
        collignon::Voronoi voronoi(2.0f, 10);
        const glm::vec2 nx(40, 0);
        const glm::vec2 ny(0, 40);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( voronoi.memory_id(v) == voronoi.memory_id(v+nx));
            CHECK( voronoi.memory_id(v) == voronoi.memory_id(v-nx));
            CHECK( voronoi.memory_id(v) == voronoi.memory_id(v+ny));
            CHECK( voronoi.memory_id(v) == voronoi.memory_id(v-ny));
        }}
    }
}
TEST_CASE( "Voronoi memory_id() range restrictions", "[collignon]" ) {
    SECTION("Voronoi.memory_id() must not produce results outside valid range"){
        collignon::Voronoi voronoi(2.0f, 10);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            int i = voronoi.memory_id(glm::vec2(x,y));
            CHECK( 0 <= i );
            CHECK( i < voronoi.vertex_count );
        }}
    }
}
/*
*/
