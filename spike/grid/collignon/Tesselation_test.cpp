
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>             // *vec3
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries

#include "Tesselation.hpp"




TEST_CASE( "Tesselation.tesselation_to_sphere() purity", "[collignon]" ) {
    SECTION("Tesselation.tesselation_to_sphere() must be called repeatedly without changing the output"){
        collignon::Projection projection;
        collignon::Tesselation tesselation(projection);
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            CHECK(collignon::Tesselation(projection).tesselation_to_sphere(glm::vec2(x,y)) == 
                  collignon::Tesselation(projection).tesselation_to_sphere(glm::vec2(x,y)));
        }}
    }
}
TEST_CASE( "Tesselation tesselation_to_sphere() closeness preservation", "[collignon]" ) {
    SECTION("changes in sphere_to_tesselation must not result in changes to tesselation_to_sphere that exceed a reasonable multiple"){
        collignon::Projection projection;
        collignon::Tesselation tesselation(projection);
        const float factor(3.0);
        const glm::vec2 dx(0.01, 0.0);
        const glm::vec2 dy(0.0, 0.01);
        for(float x = -2.0f; x < 2.0f; x+=0.1f){
        for(float y = -2.0f; y < 2.0f; y+=0.1f){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(tesselation.tesselation_to_sphere(v), tesselation.tesselation_to_sphere(v+dx)) < factor * glm::length(dx) );
            CHECK( glm::distance(tesselation.tesselation_to_sphere(v), tesselation.tesselation_to_sphere(v-dx)) < factor * glm::length(dx) );
            CHECK( glm::distance(tesselation.tesselation_to_sphere(v), tesselation.tesselation_to_sphere(v+dy)) < factor * glm::length(dy) );
            CHECK( glm::distance(tesselation.tesselation_to_sphere(v), tesselation.tesselation_to_sphere(v-dy)) < factor * glm::length(dy) );
        }}
    }
}

TEST_CASE( "Tesselation tesselation_to_sphere() congruence", "[collignon]" ) {
    SECTION("a modulo can be applied to input which results in the same output"){
        collignon::Projection projection;
        collignon::Tesselation tesselation(projection);
        const float epsilon(1e-4f);
        const glm::vec2 nx(4, 0);
        const glm::vec2 ny(0, 4);
        for(float x = -2.0f; x < 2.0f; x+=0.1f){
        for(float y = -2.0f; y < 2.0f; y+=0.1f){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(tesselation.tesselation_to_sphere(v), tesselation.tesselation_to_sphere(v+nx)) < epsilon );
            CHECK( glm::distance(tesselation.tesselation_to_sphere(v), tesselation.tesselation_to_sphere(v-nx)) < epsilon );
            CHECK( glm::distance(tesselation.tesselation_to_sphere(v), tesselation.tesselation_to_sphere(v+ny)) < epsilon );
            CHECK( glm::distance(tesselation.tesselation_to_sphere(v), tesselation.tesselation_to_sphere(v-ny)) < epsilon );
        }}
    }
}

TEST_CASE( "Tesselation.sphere_to_tesselation() purity", "[collignon]" ) {
    SECTION("Tesselation.sphere_to_tesselation() must be called repeatedly without changing the output"){
        collignon::Projection projection;
        collignon::Tesselation tesselation(projection);
        const float epsilon(1e-4f);
        for(float x = -1.0f; x < 1.0f; x+=0.5f){
        for(float y = -1.0f; y < 1.0f; y+=0.5f){
        for(float z = -1.0f; z < 1.0f; z+=0.5f){
            glm::vec3 v(x,y,z);
            if(glm::length(v) > epsilon){
                CHECK(collignon::Tesselation(projection).sphere_to_tesselation(glm::vec3(x,y,z)) == 
                      collignon::Tesselation(projection).sphere_to_tesselation(glm::vec3(x,y,z)));
            }
        }}}
    }
}

TEST_CASE( "Tesselation sphere_to_tesselation() / tesselation_to_sphere() invertibility", "[collignon]" ) {
    SECTION("Tesselation.tesselation_to_sphere() must reconstruct input passed to sphere_to_tesselation() for any unit vector"){
        collignon::Projection projection;
        collignon::Tesselation tesselation(projection);
        const float epsilon(1e-4f);
        for(float x = -1.0f; x < 1.0f; x+=0.5f){
        for(float y = -1.0f; y < 1.0f; y+=0.5f){
        for(float z = -1.0f; z < 1.0f; z+=0.5f){
            if(glm::length(glm::vec3(x,y,z)) > epsilon){
                glm::vec3 v = glm::normalize(glm::vec3(x,y,z));
                glm::vec2 u = tesselation.sphere_to_tesselation(v);
                glm::vec3 v2 = tesselation.tesselation_to_sphere( u );
                CHECK( v2.x == Approx(v.x).margin(epsilon) );
                CHECK( v2.y == Approx(v.y).margin(epsilon) );
                CHECK( v2.z == Approx(v.z).margin(epsilon) );
            }
        }}}
    }
}





// TEST_CASE( "Tesselation memory_id() congruence", "[collignon]" ) {
//     SECTION("an modulo can be applied to input which results in the same output"){
//         const glm::vec2 nx(2, 0);
//         const glm::vec2 ny(0, 2);
//         for(float x = -2.0f; x < 2.0f; x+=0.1f){
//         for(float y = -2.0f; y < 2.0f; y+=0.1f){
//             glm::vec2 v = glm::vec2(x,y);
//             CHECK( tesselation.memory_id(v) == tesselation.memory_id(v+nx));
//             CHECK( tesselation.memory_id(v) == tesselation.memory_id(v-nx));
//             CHECK( tesselation.memory_id(v) == tesselation.memory_id(v+ny));
//             CHECK( tesselation.memory_id(v) == tesselation.memory_id(v-ny));
//         }}
//     }
// }

// TEST_CASE( "Tesselation memory_id() / sphere_to_tesselation() invertibility", "[collignon]" ) {
//     SECTION("Tesselation.sphere_to_tesselation() must reconstruct input passed to memory_id() for any unit vector"){
//         for(int i = 0; i < tesselation.tesselation_cell_count; i++){
//             CHECK( i == tesselation.memory_id(tesselation.sphere_to_tesselation(i)) );
//         }
//     }
// }
// TEST_CASE( "Tesselation memory_id() range restrictions", "[collignon]" ) {
//     SECTION("Tesselation.memory_id() must not produce results outside valid range"){
//         for(float x = -2.0f; x < 2.0f; x+=0.1f){
//         for(float y = -2.0f; y < 2.0f; y+=0.1f){
//             int i = tesselation.memory_id(glm::vec2(x,y));
//             CHECK( 0 <= i );
//             CHECK( i < tesselation.tesselation_cell_count );
//         }}
//     }
// }