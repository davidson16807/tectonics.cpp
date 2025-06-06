
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

#include <index/procedural/Range.hpp>

#include "Voronoi.hpp"

#include "_test_tools.hpp"

TEST_CASE( "Voronoi grid_position() / sphere_position()", "[dymaxion]" ) {

    const double pi(3.1415926535);
    const double radius(2);
    const int vertices_per_meridian(100);
    const float epsilon(1e-4f);

    DymaxionAdapter precise(1e-4);
    DymaxionAdapter imprecise(2*pi/double(vertices_per_meridian));

    dymaxion::Voronoi<int,int,double> voronoi(radius, vertices_per_meridian);

    std::vector<glm::dvec3> sphere_positions{};
    std::vector<glm::dvec3> unit_sphere_positions{};
    for(float x = -2.0; x <= 2.0; x+=0.5){
    for(float y = -2.0; y <= 2.0; y+=0.5){
    for(float z = -2.0; z <= 2.0; z+=0.5){
        glm::dvec3 v(x,y,z);
        if(glm::length(v) > epsilon){
            unit_sphere_positions.push_back(glm::normalize(v));
            sphere_positions.push_back(2.0*glm::normalize(v));
        }
    }}}

    std::vector<dymaxion::Point<int,int>> grid_ids {};
    for(int i = 0; i < 10; i++){
    for(int x = 0; x < voronoi.vertices_per_square_side; x+=10){
    for(int y = 0; y < voronoi.vertices_per_square_side; y+=10){
        grid_ids.push_back(dymaxion::Point(i,glm::ivec2(x,y)));
    }}}

    // NOTE: right invertibility and closeness cannot be tested, 
    // since the equivalence of grid ids cannot be determined without using the very code that we are testing

    REQUIRE(test::determinism(precise,
        "Voronoi.grid_id(…)", TEST_UNARY(voronoi.grid_id),
        sphere_positions
    ));

    REQUIRE(test::determinism(precise,
        "Voronoi.grid_position(…)", TEST_UNARY(voronoi.grid_position),
        sphere_positions
    ));

    REQUIRE(test::determinism(precise,
        "Voronoi.sphere_position(…)", TEST_UNARY(voronoi.sphere_position),
        grid_ids
    ));

    REQUIRE(test::determinism(precise,
        "Voronoi.sphere_normal(…)", TEST_UNARY(voronoi.sphere_normal),
        grid_ids
    ));

    REQUIRE(test::codomain(precise,
        "within expected range", 
        [=](auto iV2){
            auto i = iV2.square_id;
            auto V2 = iV2.square_position;
            return 
                0<=i&&i<voronoi.square_count && 
                0<=V2.x&&V2.x<voronoi.vertices_per_square_side && 
                0<=V2.y&&V2.y<voronoi.vertices_per_square_side;
        },
        "Voronoi.grid_id(…)", TEST_UNARY(voronoi.grid_id),
        sphere_positions
    ));

    REQUIRE(test::codomain(precise,
        "within expected range", 
        [=](auto iV2){
            auto i = iV2.square_id;
            auto V2 = iV2.square_position;
            return 
                0<=i&&i<voronoi.square_count && 
                0<=V2.x&&V2.x<=voronoi.vertices_per_square_side && 
                0<=V2.y&&V2.y<=voronoi.vertices_per_square_side;
        },
        "Voronoi.grid_position(…)", TEST_UNARY(voronoi.grid_position),
        sphere_positions
    ));

    REQUIRE(test::codomain(precise,
        "within expected range", 
        [=](auto V3){
            auto length = glm::length(V3);
            return std::abs(length-radius) < 1e-4;
        },
        "Voronoi.sphere_position(…)", TEST_UNARY(voronoi.sphere_position),
        grid_ids
    ));

    REQUIRE(test::codomain(precise,
        "within expected range", 
        [=](auto V3){
            auto length = glm::length(V3);
            return std::abs(length-1.0) < 1e-4;
        },
        "Voronoi.sphere_normal(…)", TEST_UNARY(voronoi.sphere_normal),
        grid_ids
    ));

    REQUIRE(test::left_invertibility(imprecise,
        "Voronoi.sphere_position(…) when restricted to indexed grid_ids", TEST_UNARY(voronoi.sphere_position),
        "Voronoi.grid_id(…)",                                             TEST_UNARY(voronoi.grid_id),
        sphere_positions
    ));

    REQUIRE(test::left_invertibility(imprecise,
        "Voronoi.sphere_normal(…) when restricted to indexed grid_ids", TEST_UNARY(voronoi.sphere_normal),
        "Voronoi.grid_id(…)",                                                  TEST_UNARY(voronoi.grid_id),
        unit_sphere_positions
    ));

    REQUIRE(test::left_invertibility(precise,
        "Voronoi.sphere_position(…) when restricted to indexed grid_ids", TEST_UNARY(voronoi.sphere_position),
        "Voronoi.grid_position(…)",                                       TEST_UNARY(voronoi.grid_position),
        sphere_positions
    ));

    REQUIRE(test::left_invertibility(precise,
        "Voronoi.sphere_normal(…) when restricted to indexed grid_ids", TEST_UNARY(voronoi.sphere_normal),
        "Voronoi.grid_position(…)",                                            TEST_UNARY(voronoi.grid_position),
        unit_sphere_positions
    ));

    REQUIRE(test::congruence(precise,
        "Voronoi.sphere_position(…) when restricted to indexed sphere_positions", TEST_UNARY(voronoi.sphere_position),
        "offsets to square_id equal to square_count", [](dymaxion::Point<int,int> p){return dymaxion::Point<int,int>(p.square_id+10, p.square_position);},
        grid_ids
    ));

    REQUIRE(test::congruence(precise,
        "Voronoi.sphere_normal(…) when restricted to indexed sphere_positions", TEST_UNARY(voronoi.sphere_normal),
        "offsets to square_id equal to square_count", [](dymaxion::Point<int,int> p){return dymaxion::Point<int,int>(p.square_id+10, p.square_position);},
        grid_ids
    ));

}

/*
*/
