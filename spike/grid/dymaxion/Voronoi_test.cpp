
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>             // *vec3
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries
#include <test/macros.hpp>
#include <test/adapter.hpp>
#include <test/properties.hpp>
#include "Voronoi.hpp"

#include "test_tools.cpp"

TEST_CASE( "Voronoi grid_position() / sphere_position() invertibility", "[collignon]" ) {
    DymaxionAdapter adapter;

    const float epsilon(1e-4f);
    dymaxion::Voronoi voronoi(2.0, 20);

    std::vector<glm::dvec3> sphere_positions{};
    for(float x = -2.0; x <= 2.0; x+=0.5){
    for(float y = -2.0; y <= 2.0; y+=0.5){
    for(float z = -2.0; z <= 2.0; z+=0.5){
        glm::dvec3 v(x,y,z);
        if(glm::length(v) > epsilon){
            sphere_positions.push_back(2.0*glm::normalize(v));
        }
    }}}

    std::vector<dymaxion::Point<int,int>> grid_ids {};
    for(int i = 0; i < 10; i++){
    for(int x = 0; x < 20; x+=1){
    for(int y = 0; y < 20; y+=1){
        grid_ids.push_back(dymaxion::Point(i,glm::ivec2(x,y)));
    }}}

    // NOTE: right invertibility and closeness cannot be tested, 
    // since the equivalence of grid ids cannot be determined without using the very code that we are testing

    REQUIRE(test::determinism(adapter,
        "Voronoi.grid_id(…)", TEST_UNARY(voronoi.grid_id),
        sphere_positions
    ));

    REQUIRE(test::determinism(adapter,
        "Voronoi.sphere_position(…)", TEST_UNARY(voronoi.sphere_position),
        grid_ids
    ));

}

/*

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




TEST_CASE( "Voronoi unit_sphere_position() congruence", "[collignon]" ) {
    SECTION("a modulo can be applied to input which results in the same output"){
        collignon::Voronoi voronoi(2.0f, 10);
        const float epsilon(1e-4f);
        const glm::vec2 nx(40, 0);
        const glm::vec2 ny(0, 40);
        for(int x = -10; x < 10; x+=1){
        for(int y = -10; y < 10; y+=1){
            glm::vec2 v = glm::vec2(x,y);
            CHECK( glm::distance(voronoi.unit_sphere_position(v), voronoi.unit_sphere_position(v+nx)) < epsilon );
            CHECK( glm::distance(voronoi.unit_sphere_position(v), voronoi.unit_sphere_position(v-nx)) < epsilon );
            CHECK( glm::distance(voronoi.unit_sphere_position(v), voronoi.unit_sphere_position(v+ny)) < epsilon );
            CHECK( glm::distance(voronoi.unit_sphere_position(v), voronoi.unit_sphere_position(v-ny)) < epsilon );
        }}
    }
}


TEST_CASE( "Voronoi grid_position() / unit_sphere_position() invertibility", "[collignon]" ) {
    SECTION("Voronoi.unit_sphere_position() must reconstruct input passed to grid_position() for any unit vector"){
        collignon::Voronoi voronoi(2.0f, 10);
        const float epsilon(1e-4f);
        for(float x = -2.0f; x < 2.0f; x+=0.5f){
        for(float y = -2.0f; y < 2.0f; y+=0.5f){
        for(float z = -2.0f; z < 2.0f; z+=0.5f){
            if(glm::length(glm::vec3(x,y,z)) > epsilon){
                glm::vec3 v = glm::normalize(glm::vec3(x,y,z));
                glm::vec2 u = voronoi.grid_position(v);
                glm::vec3 v2 = voronoi.unit_sphere_position( u );
                CHECK( v2.x == Approx(v.x).margin(epsilon) );
                CHECK( v2.y == Approx(v.y).margin(epsilon) );
                CHECK( v2.z == Approx(v.z).margin(epsilon) );
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

*/
