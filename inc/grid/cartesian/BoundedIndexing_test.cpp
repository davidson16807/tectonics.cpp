#pragma once

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <test/properties.hpp>
#include <test/macros.hpp>
#include <test/glm/adapter.hpp>
#include <index/series/Range.hpp>
#include "BoundedIndexing.hpp"

TEST_CASE( "BoundedIndexing", "[cartesian]" ) {

    test::GlmAdapter<int,double> precise(1e-4);

    cartesian::BoundedIndexing<3> indexing(3);
    series::Range memory_ids(indexing.size);

    REQUIRE(test::left_invertibility(precise,
        "BoundedIndexing.memory_id(…)",                                   TEST_UNARY(indexing.memory_id),
        "BoundedIndexing.grid_id(…) when restricted to indexed grid_ids", TEST_UNARY(indexing.grid_id),
        memory_ids
    ));

}

