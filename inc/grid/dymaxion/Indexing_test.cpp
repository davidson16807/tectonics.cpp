
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

#include <index/series/Range.hpp>

#include "Indexing.hpp"

#include "test_tools.cpp"

TEST_CASE( "Indexing grid_id() / memory_id()", "[dymaxion]" ) {

    const double pi(3.1415926535);
    const int vertices_per_meridian(100);

    DymaxionAdapter precise(1e-4);
    DymaxionAdapter imprecise(2*pi/double(vertices_per_meridian));

    dymaxion::Indexing<int,double> indexing(vertices_per_meridian);

    std::vector<dymaxion::Point<int,int>> grid_ids {};
    for(int i = 0; i < 10; i++){
    for(int x = 0; x < indexing.vertices_per_square_side; x+=10){
    for(int y = 0; y < indexing.vertices_per_square_side; y+=10){
        grid_ids.push_back(dymaxion::Point(i,glm::ivec2(x,y)));
    }}}

    series::Range memory_ids(indexing.vertex_count);

    // NOTE: right invertibility and closeness cannot be tested, 
    // since the equivalence of grid ids cannot be determined without using the very code that we are testing

    REQUIRE(test::determinism(precise,
        "Indexing.memory_id(…)", TEST_UNARY(indexing.memory_id),
        grid_ids
    ));

    REQUIRE(test::codomain(precise,
        "between 0 and " + std::to_string(indexing.vertex_count), 
        [=](auto memory_id){
            return 0<=memory_id&&memory_id<indexing.vertex_count;
        },
        "Indexing.memory_id(…)", TEST_UNARY(indexing.memory_id),
        grid_ids
    ));

    REQUIRE(test::codomain(precise,
        "within expected range", 
        [=](auto iV2){
            auto i = iV2.square_id;
            auto V2 = iV2.square_position;
            return 
                0<=i&&i<indexing.square_count && 
                0<=V2.x&&V2.x<indexing.vertices_per_square_side && 
                0<=V2.y&&V2.y<indexing.vertices_per_square_side;
        },
        "Indexing.grid_id(…)", TEST_UNARY(indexing.grid_id),
        memory_ids
    ));

    REQUIRE(test::left_invertibility(precise,
        "Indexing.grid_id(…) when restricted to indexed grid_ids", TEST_UNARY(indexing.grid_id),
        "Indexing.memory_id(…)",                                   TEST_UNARY(indexing.memory_id),
        grid_ids
    ));

    REQUIRE(test::left_invertibility(precise,
        "Indexing.memory_id(…)",                                   TEST_UNARY(indexing.memory_id),
        "Indexing.grid_id(…) when restricted to indexed grid_ids", TEST_UNARY(indexing.grid_id),
        memory_ids
    ));

}

/*
*/
