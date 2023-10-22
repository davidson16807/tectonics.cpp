
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

#include <store/series/Range.hpp>

#include "Voronoi.hpp"

#include "test_tools.cpp"

TEST_CASE( "Voronoi grid_position() / sphere_position()", "[healpix]" ) {

    const double pi(3.1415926535);
    const double radius(2);
    const int vertex_count_per_meridian(100);
    const float epsilon(1e-4f);

    DymaxionAdapter precise(1e-4);
    DymaxionAdapter imprecise(2*pi/double(vertex_count_per_meridian));

    healpix::Voronoi voronoi(radius, vertex_count_per_meridian);

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

    std::vector<healpix::Point<int,int>> grid_ids {};
    for(int i = 0; i < 10; i++){
    for(int x = 0; x < voronoi.vertex_count_per_triangle_leg; x+=10){
    for(int y = 0; y < voronoi.vertex_count_per_triangle_leg; y+=10){
        grid_ids.push_back(healpix::Point(i,glm::ivec2(x,y)));
    }}}

    series::Range memory_ids(voronoi.vertex_count);

    // NOTE: right invertibility and closeness cannot be tested, 
    // since the equivalence of grid ids cannot be determined without using the very code that we are testing

    REQUIRE(test::determinism(precise,
        "Voronoi.memory_id(…)", TEST_UNARY(voronoi.memory_id),
        grid_ids
    ));

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
        "Voronoi.unit_sphere_position(…)", TEST_UNARY(voronoi.unit_sphere_position),
        grid_ids
    ));

    REQUIRE(test::codomain(precise,
        "between 0 and " + std::to_string(voronoi.vertex_count), 
        [=](auto memory_id){
            return 0<=memory_id&&memory_id<voronoi.vertex_count;
        },
        "Voronoi.memory_id(…)", TEST_UNARY(voronoi.memory_id),
        grid_ids
    ));

    REQUIRE(test::codomain(precise,
        "within expected range", 
        [=](auto iV2){
            auto i = iV2.square_id;
            auto V2 = iV2.square_position;
            return 
                0<=i&&i<voronoi.subgrid_count && 
                0<=V2.x&&V2.x<voronoi.vertex_count_per_triangle_leg && 
                0<=V2.y&&V2.y<voronoi.vertex_count_per_triangle_leg;
        },
        "Voronoi.grid_id(…)", TEST_UNARY(voronoi.grid_id),
        memory_ids
    ));

    REQUIRE(test::codomain(precise,
        "within expected range", 
        [=](auto iV2){
            auto i = iV2.square_id;
            auto V2 = iV2.square_position;
            return 
                0<=i&&i<voronoi.subgrid_count && 
                0<=V2.x&&V2.x<voronoi.vertex_count_per_triangle_leg && 
                0<=V2.y&&V2.y<voronoi.vertex_count_per_triangle_leg;
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
                0<=i&&i<voronoi.subgrid_count && 
                0<=V2.x&&V2.x<=voronoi.vertex_count_per_triangle_leg && 
                0<=V2.y&&V2.y<=voronoi.vertex_count_per_triangle_leg;
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
        "Voronoi.unit_sphere_position(…)", TEST_UNARY(voronoi.unit_sphere_position),
        grid_ids
    ));

    REQUIRE(test::left_invertibility(precise,
        "Voronoi.grid_id(…) when restricted to indexed grid_ids", TEST_UNARY(voronoi.grid_id),
        "Voronoi.memory_id(…)",                                   TEST_UNARY(voronoi.memory_id),
        grid_ids
    ));

    REQUIRE(test::left_invertibility(precise,
        "Voronoi.memory_id(…)",                                   TEST_UNARY(voronoi.memory_id),
        "Voronoi.grid_id(…) when restricted to indexed grid_ids", TEST_UNARY(voronoi.grid_id),
        memory_ids
    ));

    REQUIRE(test::left_invertibility(imprecise,
        "Voronoi.sphere_position(…) when restricted to indexed grid_ids", TEST_UNARY(voronoi.sphere_position),
        "Voronoi.grid_id(…)",                                             TEST_UNARY(voronoi.grid_id),
        sphere_positions
    ));

    REQUIRE(test::left_invertibility(imprecise,
        "Voronoi.unit_sphere_position(…) when restricted to indexed grid_ids", TEST_UNARY(voronoi.unit_sphere_position),
        "Voronoi.grid_id(…)",                                                  TEST_UNARY(voronoi.grid_id),
        unit_sphere_positions
    ));

    REQUIRE(test::left_invertibility(precise,
        "Voronoi.sphere_position(…) when restricted to indexed grid_ids", TEST_UNARY(voronoi.sphere_position),
        "Voronoi.grid_position(…)",                                       TEST_UNARY(voronoi.grid_position),
        sphere_positions
    ));

    REQUIRE(test::left_invertibility(precise,
        "Voronoi.unit_sphere_position(…) when restricted to indexed grid_ids", TEST_UNARY(voronoi.unit_sphere_position),
        "Voronoi.grid_position(…)",                                            TEST_UNARY(voronoi.grid_position),
        unit_sphere_positions
    ));

    REQUIRE(test::congruence(precise,
        "Voronoi.sphere_position(…) when restricted to indexed sphere_positions", TEST_UNARY(voronoi.sphere_position),
        "offsets to square_id equal to square_count", [](healpix::Point<int,int> p){return healpix::Point<int,int>(p.square_id+10, p.square_position);},
        grid_ids
    ));

    REQUIRE(test::congruence(precise,
        "Voronoi.unit_sphere_position(…) when restricted to indexed sphere_positions", TEST_UNARY(voronoi.unit_sphere_position),
        "offsets to square_id equal to square_count", [](healpix::Point<int,int> p){return healpix::Point<int,int>(p.square_id+10, p.square_position);},
        grid_ids
    ));

}

/*
*/
