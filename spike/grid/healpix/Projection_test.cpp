
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE              // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL     // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>             // *vec3
#include <glm/gtx/string_cast.hpp>  // to_string

// in-house libraries
#include <test/macros.hpp>
#include <test/adapter.hpp>
#include <test/properties.hpp>
#include "Projection.hpp"

#include "test_tools.cpp"

TEST_CASE( "Projection.sphere_position() / Projection.grid_id()", "[healpix]" ) {
    DymaxionAdapter adapter;

    const float epsilon(1e-4f);
    healpix::Projection projection;

    std::vector<glm::dvec3> sphere_positions{};
    for(float x = -1.0f; x <= 1.0f; x+=0.5f){
    for(float y = -1.0f; y <= 1.0f; y+=0.5f){
    for(float z = -1.0f; z <= 1.0f; z+=0.5f){
        glm::vec3 v(x,y,z);
        if(glm::length(v) > epsilon){
            sphere_positions.push_back(glm::normalize(v));
        }
    }}}

    std::vector<healpix::Point<int,double>> grid_ids {};
    for(int    i = 0; i < 10; i++){
    for(double x = 0.0; x <= 1.0; x+=0.1){
    for(double y = 0.0; y <= 1.0; y+=0.1){
        grid_ids.push_back(healpix::Point(i,glm::dvec2(x,y)));
    }}}

    std::vector<healpix::Point<int,double>> nonedge_grid_ids {};
    for(int    i = 0; i < 10; i++){
    for(double x = -0.05; x <= 1.05; x+=0.1){
    for(double y = -0.05; y <= 1.05; y+=0.1){
        grid_ids.push_back(healpix::Point(i,glm::dvec2(x,y)));
    }}}

    // NOTE: right invertibility and closeness cannot be tested, 
    // since the equivalence of grid ids cannot be determined without using the very code that we are testing

    REQUIRE(test::determinism(adapter,
        "Projection.grid_id(…)", TEST_UNARY(healpix::Projection().grid_id),
        sphere_positions
    ));

    REQUIRE(test::determinism(adapter,
        "Projection.sphere_position(…)", TEST_UNARY(healpix::Projection().sphere_position),
        grid_ids
    ));

    REQUIRE(test::codomain(adapter,
        "within expected range", [](auto iV2){
            auto i = iV2.square_id;
            auto V2 = iV2.square_position;
            return 
                0<=i&&i<10 && 
                0<=V2.x&&V2.x<1+1e-4 && 
                0<=V2.y&&V2.y<1+1e-4;
        },
        "Projection.grid_id(…)", TEST_UNARY(projection.grid_id),
        sphere_positions
    ));

    REQUIRE(test::codomain(adapter,
        "within expected range", [](auto V3){
            auto length = glm::length(V3);
            return std::abs(length-1) < 1e-4;
        },
        "Projection.sphere_position(…)", TEST_UNARY(projection.sphere_position),
        grid_ids
    ));

    REQUIRE(test::left_invertibility(adapter,
        "Projection.sphere_position(…) when restricted to indexed grid_ids", TEST_UNARY(projection.sphere_position),
        "Projection.grid_id(…)",                                             TEST_UNARY(projection.grid_id),
        sphere_positions
    ));

    REQUIRE(test::congruence(adapter,
        "Projection.sphere_position(…) when restricted to indexed sphere_positions", TEST_UNARY(projection.sphere_position),
        "offsets to square_id equal to square_count", [](healpix::Point<int,double> p){return healpix::Point<int,double>(p.square_id+10, p.square_position);},
        grid_ids
    ));

    REQUIRE(test::unary_idempotence(adapter,
        "Projection.standardize(…) when restricted to xs and ys of (0,1)", TEST_UNARY(projection.standardize),
        nonedge_grid_ids
    ));

}

/*


// TEST_CASE( "Projection memory_id() congruence", "[healpix]" ) {
//     SECTION("an modulo can be applied to input which results in the same output"){
//         const glm::vec2 nx(2, 0);
//         const glm::vec2 ny(0, 2);
//         for(float x = -2.0f; x < 2.0f; x+=0.1f){
//         for(float y = -2.0f; y < 2.0f; y+=0.1f){
//             glm::vec2 v = glm::vec2(x,y);
//             CHECK( projection.memory_id(v) == projection.memory_id(v+nx));
//             CHECK( projection.memory_id(v) == projection.memory_id(v-nx));
//             CHECK( projection.memory_id(v) == projection.memory_id(v+ny));
//             CHECK( projection.memory_id(v) == projection.memory_id(v-ny));
//         }}
//     }
// }

// TEST_CASE( "Projection memory_id() / grid_id() invertibility", "[healpix]" ) {
//     SECTION("Projection.grid_id() must reconstruct input passed to memory_id() for any unit vector"){
//         for(int i = 0; i < projection.tesselation_cell_count; i++){
//             CHECK( i == projection.memory_id(projection.grid_id(i)) );
//         }
//     }
// }
// TEST_CASE( "Projection memory_id() range restrictions", "[healpix]" ) {
//     SECTION("Projection.memory_id() must not produce results outside valid range"){
//         for(float x = -2.0f; x < 2.0f; x+=0.1f){
//         for(float y = -2.0f; y < 2.0f; y+=0.1f){
//             int i = projection.memory_id(glm::vec2(x,y));
//             CHECK( 0 <= i );
//             CHECK( i < projection.tesselation_cell_count );
//         }}
//     }
// }

*/