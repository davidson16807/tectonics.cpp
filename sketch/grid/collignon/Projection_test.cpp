
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <glm/vec3.hpp>             // *vec3
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries

#include "Projection.hpp"

collignon::Projection projection;

TEST_CASE( "Projection.collignon_to_hemisphere() purity", "[collignon]" ) {
    SECTION("Projection.collignon_to_hemisphere() must be called repeatedly without changing the output"){
        const float quadrant(std::sqrt(3.14159265f)); // length of a quadrant when projected
        for(float x = -quadrant; x < quadrant; x+=0.1){
        for(float y = -quadrant; y < quadrant; y+=0.1){
            if (std::abs(x)+std::abs(y) < 0.95 * quadrant)
            {
                CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(x,y), 0.0f) == 
                      collignon::Projection().collignon_to_hemisphere(glm::vec2(x,y), 0.0f));
            }
        }}
    }
}
TEST_CASE( "Projection collignon_to_hemisphere() limiting cases", "[collignon]" ) {
    SECTION("Projection.collignon_to_hemisphere() must reproduce results for limiting cases"){
        const float margin(1e-4f);
        const float pi(3.14159);
        const float quadrant(std::sqrt(pi)); // length of a quadrant when projected

        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,0), 0.0f).x == Approx(0).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,0), 0.0f).y == Approx(0).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,0), 0.0f).z == Approx(1).margin(margin) );

        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,0), pi).x == Approx(0).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,0), pi).y == Approx(0).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,0), pi).z == Approx(-1).margin(margin) );

        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,quadrant), 0.0f).x == Approx(0).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,quadrant), 0.0f).y == Approx(1).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,quadrant), 0.0f).z == Approx(0).margin(margin) );

        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,-quadrant), 0.0f).x == Approx(0).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,-quadrant), 0.0f).y == Approx(-1).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,-quadrant), 0.0f).z == Approx(0).margin(margin) );

        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(quadrant,0), 0.0f).x == Approx(1).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(quadrant,0), 0.0f).y == Approx(0).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(quadrant,0), 0.0f).z == Approx(0).margin(margin) );

        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(-quadrant,0), 0.0f).x == Approx(-1).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(-quadrant,0), 0.0f).y == Approx(0).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(-quadrant,0), 0.0f).z == Approx(0).margin(margin) );



        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,quadrant), pi).x == Approx(0).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,quadrant), pi).y == Approx(1).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,quadrant), pi).z == Approx(0).margin(margin) );

        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,-quadrant), pi).x == Approx(0).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,-quadrant), pi).y == Approx(-1).margin(margin) );
        CHECK(collignon::Projection().collignon_to_hemisphere(glm::vec2(0,-quadrant), pi).z == Approx(0).margin(margin) );

    }
}
TEST_CASE( "Projection collignon_to_hemisphere() closeness preservation", "[collignon]" ) {
    SECTION("changes in collignon_to_hemisphere must not result in changes that exceed a reasonable multiple"){
        const float quadrant(std::sqrt(3.14159265f)); // length of a quadrant when projected
        const float factor(2.0f);
        const glm::vec2 dx(0.001, 0.0);
        const glm::vec2 dy(0.0, 0.001);
        for(float x = -quadrant; x < quadrant; x+=0.1){
        for(float y = -quadrant; y < quadrant; y+=0.1){
            if (std::abs(x)+std::abs(y) < 0.95 * quadrant)
            {
                glm::vec2 v = glm::vec2(x,y);
                CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v+dx, 0.0f)) < factor * glm::length(dx) );
                CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v-dx, 0.0f)) < factor * glm::length(dx) );
                CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v+dy, 0.0f)) < factor * glm::length(dy) );
                CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v-dy, 0.0f)) < factor * glm::length(dy) );
            }
        }}
    }
}
TEST_CASE( "Projection.collignon_to_hemisphere() area preservation", "[collignon]" ) {
    SECTION("Projection.collignon_to_hemisphere() must not result in changes to areas that exceed a reasonable multiple"){
        const float quadrant(std::sqrt(3.14159265f)); // length of a quadrant when projected
        const float epsilon(0.003);
        const float area(0.03*0.03);
        const glm::vec2 dx(0.03, 0.0);
        const glm::vec2 dy(0.0, 0.03);
        for(float x = -quadrant; x < quadrant; x+=0.1f){
        for(float y = -quadrant; y < quadrant; y+=0.1f){
            if (std::abs(x)+std::abs(y) < 0.95 * quadrant)
            {
                glm::vec2 v = glm::vec2(x,y);
                const float width = glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v+dx, 0.0f));
                const float height = glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v+dy, 0.0f));
                CHECK( (width * height) == Approx(area).margin(epsilon) );
            }
        }}
    }
}
// TEST_CASE( "Projection collignon_to_hemisphere() congruence", "[collignon]" ) {
//     SECTION("a modulo can be applied to input which results in the same output"){
//         const float epsilon(1e-4f);
//         const glm::vec2 nx(4.0, 0);
//         for(float x = -1.0; x < 1.0; x+=0.1){
//         for(float y = -1.0; y < 1.0; y+=0.1){
//             if (std::abs(x)+std::abs(y) < 0.95 * quadrant)
//             {
//                 glm::vec2 v = glm::vec2(x,y);
//                 CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v+nx, 0.0f)) < epsilon );
//                 CHECK( glm::distance(projection.collignon_to_hemisphere(v, 0.0f), projection.collignon_to_hemisphere(v-nx, 0.0f)) < epsilon );
//             }
//         }}
//     }
// }




TEST_CASE( "Projection.hemisphere_to_collignon() purity", "[collignon]" ) {
    SECTION("Projection.hemisphere_to_collignon() must be called repeatedly without changing the output"){
        const float z(1.0f);
        for(float x = -1.0; x < 1.0; x+=0.1f){
        for(float y = -1.0; y < 1.0; y+=0.1f){
            glm::vec3 v(x,y,z);
            CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(x,y,z), 0.0f) == 
                  collignon::Projection().hemisphere_to_collignon(glm::vec3(x,y,z), 0.0f));
        }}
    }
}

TEST_CASE( "Projection.hemisphere_to_collignon() limiting cases", "[collignon]" ) {
    SECTION("Projection.hemisphere_to_collignon() must reproduce results for limiting cases"){
        const float margin(1e-4f);
        const float pi(3.14159);
        const float quadrant(std::sqrt(pi)); // length of a quadrant when projected

        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(0,1,0), 0.0f).x == Approx(0).margin(margin) );
        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(0,1,0), 0.0f).y == Approx(quadrant).margin(margin) );

        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(0,-1,0), 0.0f).x == Approx(0).margin(margin) );
        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(0,-1,0), 0.0f).y == Approx(-quadrant).margin(margin) );


        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(1,0,0), 0.0f).x == Approx(quadrant).margin(margin) );
        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(1,0,0), 0.0f).y == Approx(0).margin(margin) );

        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(-1,0,0), 0.0f).x == Approx(-quadrant).margin(margin) );
        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(-1,0,0), 0.0f).y == Approx(0).margin(margin) );


        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(0,0,1), 0.0f).x == Approx(0).margin(margin) );
        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(0,0,1), 0.0f).y == Approx(0).margin(margin) );

        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(0,0,-1), 0.0f).x == Approx(0).margin(margin) );
        CHECK(collignon::Projection().hemisphere_to_collignon(glm::vec3(0,0,-1), 0.0f).y == Approx(0).margin(margin) );

    }
}

TEST_CASE( "Projection.hemisphere_to_collignon() closeness preservation", "[collignon]" ) {
    SECTION("Projection.hemisphere_to_collignon() must not result in changes that exceed a reasonable multiple"){
        const float pi(3.141592653589f);
        const float factor(2.0f);
        const glm::vec3 dx(0.1, 0.0, 0.0);
        const glm::vec3 dy(0.0, 0.1, 0.0);
        const glm::vec3 dz(0.0, 0.0, 0.1);
        for(float x = -1.0; x < 1.0; x+=0.1f){
        for(float y = -1.0; y < 1.0; y+=0.1f){
            if (std::abs(x)+std::abs(y) < 0.95)
            {
                glm::vec3 v = glm::normalize(glm::vec3(x,y,0.1f));
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v+dx), 0.0f)) < factor * glm::length(dx) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v-dx), 0.0f)) < factor * glm::length(dx) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v+dy), 0.0f)) < factor * glm::length(dy) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v-dy), 0.0f)) < factor * glm::length(dy) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v+dz), 0.0f)) < factor * glm::length(dz) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, 0.0f), projection.hemisphere_to_collignon(glm::normalize(v-dz), 0.0f)) < factor * glm::length(dz) );
            }
        }}
        for(float x = -1.0; x < 1.0; x+=0.1f){
        for(float y = -1.0; y < 1.0; y+=0.1f){
            if (std::abs(x)+std::abs(y) < 0.95)
            {
                glm::vec3 v = glm::normalize(glm::vec3(x,y,-0.1f));
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v+dx), pi)) < factor * glm::length(dx) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v-dx), pi)) < factor * glm::length(dx) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v+dy), pi)) < factor * glm::length(dy) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v-dy), pi)) < factor * glm::length(dy) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v+dz), pi)) < factor * glm::length(dz) );
                CHECK( glm::distance(projection.hemisphere_to_collignon(v, pi), projection.hemisphere_to_collignon(glm::normalize(v-dz), pi)) < factor * glm::length(dz) );
            }
        }}
    }
}
TEST_CASE( "Projection.hemisphere_to_collignon() area preservation", "[collignon]" ) {
    SECTION("Projection.hemisphere_to_collignon() must not result in changes to areas that exceed a reasonable multiple"){
        const float epsilon(1.0);
        const float area(0.03*0.03);
        const glm::vec3 dx(0.03, 0.0, 0.0);
        const glm::vec3 dy(0.0, 0.03, 0.0);
        const float z(1.0f);
        for(float x = -1.0; x < 1.0; x+=0.1f){
        for(float y = -1.0; y < 1.0; y+=0.1f){
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
TEST_CASE( "Projection.hemisphere_to_collignon() / collignon_to_hemisphere() invertibility", "[collignon]" ) {
    SECTION("Projection.collignon_to_hemisphere() must reconstruct input passed to hemisphere_to_collignon() for any unit vector"){
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


