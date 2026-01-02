#pragma once

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#include <test/properties.hpp>
#include <test/macros.hpp>
#include <test/glm/adapter.hpp>
#include <index/procedural/Range.hpp>
#include "BoundedIndexing.hpp"

TEST_CASE( "BoundedIndexing", "[cartesian]" ) {

    test::GlmAdapter<int,double> precise(1e-4);

    cartesian::BoundedIndexing<4> indexing4(5);
    procedural::Range memory_ids4(indexing4.size);

    REQUIRE(test::left_invertibility(precise,
        "BoundedIndexing.memory_id(…)",                                   TEST_UNARY(indexing4.memory_id),
        "BoundedIndexing.grid_id(…) when restricted to indexed grid_ids", TEST_UNARY(indexing4.grid_id),
        memory_ids4
    ));

    cartesian::BoundedIndexing<3> indexing3(5);
    procedural::Range memory_ids3(indexing3.size);

    REQUIRE(test::left_invertibility(precise,
        "BoundedIndexing.memory_id(…)",                                   TEST_UNARY(indexing3.memory_id),
        "BoundedIndexing.grid_id(…) when restricted to indexed grid_ids", TEST_UNARY(indexing3.grid_id),
        memory_ids3
    ));

    cartesian::BoundedIndexing<2> indexing2(5);
    procedural::Range memory_ids2(indexing2.size);

    REQUIRE(test::left_invertibility(precise,
        "BoundedIndexing.memory_id(…)",                                   TEST_UNARY(indexing2.memory_id),
        "BoundedIndexing.grid_id(…) when restricted to indexed grid_ids", TEST_UNARY(indexing2.grid_id),
        memory_ids2
    ));

    cartesian::BoundedIndexing<1> indexing1(5);
    procedural::Range memory_ids1(indexing1.size);

    REQUIRE(test::left_invertibility(precise,
        "BoundedIndexing.memory_id(…)",                                   TEST_UNARY(indexing1.memory_id),
        "BoundedIndexing.grid_id(…) when restricted to indexed grid_ids", TEST_UNARY(indexing1.grid_id),
        memory_ids1
    ));

}

