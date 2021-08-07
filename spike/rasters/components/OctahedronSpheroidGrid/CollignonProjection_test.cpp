
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>             // *vec3
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries
#include <series/types.hpp>
#include <series/common.hpp>
#include <series/convenience.hpp>  
#include <series/glm/glm.hpp>         // *vec*s
#include <series/glm/convenience.hpp> //  operators, etc.

#include "CollignonProjection.hpp"

rasters::CollignonProjection projection;

TEST_CASE( "CollignonProjection.collignon_to_hemisphere() regularity", "[rasters]" ) {
    SECTION("CollignonProjection.collignon_to_hemisphere() must be called repeatedly without changing the output"){
        for(float x = -1.0; x < 1.0; x+=0.1){
        for(float y = -1.0; y < 1.0; y+=0.1){
            if (std::abs(x)+std::abs(y) < 0.95)
            {
                CHECK(rasters::CollignonProjection().collignon_to_hemisphere(glm::vec2(x,y), 0.0f) == 
                      rasters::CollignonProjection().collignon_to_hemisphere(glm::vec2(x,y), 0.0f));
            }
        }}
    }
}
TEST_CASE( "CollignonProjection collignon_to_hemisphere() closeness preservation", "[rasters]" ) {
    SECTION("changes in hemisphere_to_collignon must not result in changes that exceed a reasonable multiple"){
        const float factor(3.0f);
        const glm::vec2 dx(0.001, 0.0);
        const glm::vec2 dy(0.0, 0.001);
        for(float x = -1.0; x < 1.0; x+=0.1){
        for(float y = -1.0; y < 1.0; y+=0.1){
            if (std::abs(x)+std::abs(y) < 0.95)
            {
                glm::vec2 v = glm::vec2(x,y);
                CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v+dx, 0.0f)) < factor * glm::distance(v, v+dx) );
                CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v-dx, 0.0f)) < factor * glm::distance(v, v-dx) );
                CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v+dy, 0.0f)) < factor * glm::distance(v, v+dy) );
                CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v-dy, 0.0f)) < factor * glm::distance(v, v-dy) );
            }
        }}
    }
}
TEST_CASE( "CollignonProjection.collignon_to_hemisphere() area preservation", "[rasters]" ) {
    SECTION("CollignonProjection.collignon_to_hemisphere() must not result in changes to areas that exceed a reasonable multiple"){
        const float epsilon(0.003);
        const float area(0.03*0.03);
        const glm::vec2 dx(0.03, 0.0);
        const glm::vec2 dy(0.0, 0.03);
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            if (std::abs(x)+std::abs(y) < 0.95)
            {
                glm::vec2 v = glm::vec2(x,y);
                const float width = glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v+dx, 0.0f));
                const float height = glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v+dy, 0.0f));
                CHECK( (width * height) == Approx(area).margin(epsilon) );
            }
        }}
    }
}
// TEST_CASE( "CollignonProjection collignon_to_hemisphere() congruence", "[rasters]" ) {
//     SECTION("a modulo can be applied to input which results in the same output"){
//         const float epsilon(1e-4f);
//         const glm::vec2 nx(4.0, 0);
//         for(float x = -1.0; x < 1.0; x+=0.1){
//         for(float y = -1.0; y < 1.0; y+=0.1){
//             if (std::abs(x)+std::abs(y) < 0.95)
//             {
//                 glm::vec2 v = glm::vec2(x,y);
//                 CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v+nx, 0.0f)) < epsilon );
//                 CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v-nx, 0.0f)) < epsilon );
//             }
//         }}
//     }
// }




TEST_CASE( "CollignonProjection.hemisphere_to_collignon() regularity", "[rasters]" ) {
    SECTION("CollignonProjection.hemisphere_to_collignon() must be called repeatedly without changing the output"){
        const float z(1.0f);
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            glm::vec3 v(x,y,z);
            CHECK(rasters::CollignonProjection().hemisphere_to_collignon(glm::vec3(x,y,z), 0.0f) == 
                  rasters::CollignonProjection().hemisphere_to_collignon(glm::vec3(x,y,z), 0.0f));
        }}
    }
}
TEST_CASE( "CollignonProjection.hemisphere_to_collignon() closeness preservation", "[rasters]" ) {
    SECTION("CollignonProjection.hemisphere_to_collignon() must not result in changes that exceed a reasonable multiple"){
        const float pi(3.141592653589f);
        const float factor(1.3f);
        const glm::vec3 dx(0.1, 0.0, 0.0);
        const glm::vec3 dy(0.0, 0.1, 0.0);
        const glm::vec3 dz(0.0, 0.0, 0.1);
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            if (std::abs(x)+std::abs(y) < 0.99)
            {
                glm::vec3 v = glm::normalize(glm::vec3(x,y,0.1f));
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v+dx), 0.0f)) < factor * glm::distance(v, v+dx) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v-dx), 0.0f)) < factor * glm::distance(v, v-dx) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v+dy), 0.0f)) < factor * glm::distance(v, v+dy) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v-dy), 0.0f)) < factor * glm::distance(v, v-dy) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v+dz), 0.0f)) < factor * glm::distance(v, v+dz) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v-dz), 0.0f)) < factor * glm::distance(v, v-dz) );
            }
        }}
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            if (std::abs(x)+std::abs(y) < 0.99)
            {
                glm::vec3 v = glm::normalize(glm::vec3(x,y,-0.1f));
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v+dx), pi)) < factor * glm::distance(v, v+dx) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v-dx), pi)) < factor * glm::distance(v, v-dx) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v+dy), pi)) < factor * glm::distance(v, v+dy) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v-dy), pi)) < factor * glm::distance(v, v-dy) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v+dz), pi)) < factor * glm::distance(v, v+dz) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v-dz), pi)) < factor * glm::distance(v, v-dz) );
            }
        }}
    }
}
TEST_CASE( "CollignonProjection.hemisphere_to_collignon() area preservation", "[rasters]" ) {
    SECTION("CollignonProjection.hemisphere_to_collignon() must not result in changes to areas that exceed a reasonable multiple"){
        const float epsilon(1.0);
        const float area(0.03*0.03);
        const glm::vec3 dx(0.03, 0.0, 0.0);
        const glm::vec3 dy(0.0, 0.03, 0.0);
        const float z(1.0f);
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            if (std::abs(x)+std::abs(y) < 0.95)
            {
                glm::vec3 v = glm::normalize(glm::vec3(x,y,z));
                const float width = glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v+dx), 0.0f));
                const float height = glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v+dy), 0.0f));
                CHECK( (width * height) == Approx(area).epsilon(epsilon) );
            }
        }}
    }
}
TEST_CASE( "CollignonProjection hemisphere_to_collignon() / collignon_to_hemisphere() invertibility", "[rasters]" ) {
    SECTION("CollignonProjection.collignon_to_hemisphere() must reconstruct input passed to hemisphere_to_collignon() for any unit vector"){
        const float pi(3.141592653589f);
        const float epsilon(1e-4f);
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            glm::vec3 v = glm::normalize(glm::vec3(x,y,1.0f));
            glm::vec3 v2 = projection.collignon_to_hemisphere( projection.hemisphere_to_collignon(v, 0.0f), 0.0f );
            CHECK( v2.x == Approx(v.x).margin(epsilon) );
            CHECK( v2.y == Approx(v.y).margin(epsilon) );
            CHECK( v2.z == Approx(v.z).margin(epsilon) );
        }}
        for(float x = -1.0f; x < 1.0f; x+=0.1f){
        for(float y = -1.0f; y < 1.0f; y+=0.1f){
            glm::vec3 v = glm::normalize(glm::vec3(x,y,-1.0f));
            glm::vec3 v2 = projection.collignon_to_hemisphere( projection.hemisphere_to_collignon(v, pi), pi );
            CHECK( v2.x == Approx(v.x).margin(epsilon) );
            CHECK( v2.y == Approx(v.y).margin(epsilon) );
            CHECK( v2.z == Approx(v.z).margin(epsilon) );
        }}
    }
}


