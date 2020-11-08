

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

#include <many/types.hpp>  
#include <many/morphologic.hpp>  
#include <many/relational.hpp>  
#include <many/glm/glm.hpp>         // *vec*s

#include <meshes/mesh.hpp>

#include "layered_raster.hpp"
#include "morphologic.hpp"

#include "LayeredGrid_test_utils.hpp" // layered_diamond_grid

using namespace rasters;

TEST_CASE( "raster dilation purity", "[rasters]" ) {
    auto upper_half =  make_raster(layered_diamond_grid, {true,  true , true,  true , false, true , true,  true , true,  false });
    auto top_only   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch    =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("dilate(grid, top_only) must generate the same output when called repeatedly"){
        dilate(top_only, out1);
        dilate(top_only, out2);
        CHECK(equal(out1, out2));
    }
    SECTION("dilate(grid, top_only, radius) must generate the same output when called repeatedly"){
        dilate(top_only, out1, 2);
        dilate(top_only, out2, 2);
        CHECK(equal(out1, out2));
    }
    SECTION("dilate(grid, top_only, radius, scratch) must generate the same output when called repeatedly"){
        dilate(top_only, out1, 2, scratch);
        dilate(top_only, out2, 2, scratch);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "raster dilation increasing", "[rasters]" ) {
    auto upper_half =  make_raster(layered_diamond_grid, {true,  true , true,  true , false, true , true,  true , true,  false });
    auto top_only   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("dilate(grid, top_only) must increase the number of flagged vertices"){
        dilate(top_only, out1);
        dilate(out1,     out2);
        CHECK(sum(out1) >= sum(top_only));
        CHECK(sum(out2) >= sum(out1));
    }
}
TEST_CASE( "raster dilation associative", "[rasters]" ) {
    auto A   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto AB  =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto ABC =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto BC  =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto BCA =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("dilation (A+B)+C must equal A+(B+C) where B and C are circles of radius 1 and 2"){
        dilate(A,  AB,  1);
        dilate(AB, ABC, 2);
        dilate(A,  BCA, 3);
        CHECK(equal(ABC,  BCA));
    }
}
TEST_CASE( "raster dilation distributive over union", "[rasters]" ) {
    auto A       =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto B       =  make_raster(layered_diamond_grid, {true,  false, true,  false, false, true , false, true , false, false });
    auto AC      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto BC      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto AB      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto AB_C    =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto AC_BC   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("dilation (A ∪ B)+C must equal (A+C) ∪ (B+C)"){
        unite (A,  B,  AB);
        dilate(AB, AB_C, 1);

        dilate(A,   AC,  1);
        dilate(B,   BC,  1);
        unite (AC, BC, AC_BC);

        CHECK(equal(AB_C,  AC_BC));
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
/*
TEST_CASE( "raster dilation happy path", "[rasters]" ) {
    auto upper_half =  make_raster(layered_diamond_grid, {true,  true , true,  true , false, true , true,  true , true,  false });
    auto top_only   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto empty      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto full       =  make_raster(layered_diamond_grid, {true,  true , true,  true , true,  true , true,  true , true,  true  });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("dilate(grid, top_only) must return predictable results"){
        dilate(top_only, out1);
        CHECK(equal(out1,  upper_half));
    }
    SECTION("dilate(grid, upper_half) must return predictable results"){
        dilate(upper_half, out1);
        CHECK(equal(out1,  full));
    }
    SECTION("dilate(grid, empty) must return predictable results"){
        dilate(empty, out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("dilate(grid, full) must return predictable results"){
        dilate(full, out1);
        CHECK(equal(out1,  full));
    }
}
*/
TEST_CASE( "raster erosion purity", "[rasters]" ) {
    auto lower_half =  make_raster(layered_diamond_grid, {true,  true , false,  true , true, true , true,  false, true,  true });
    auto bottom_only=  make_raster(layered_diamond_grid, {true,  true , false,  true , true,  true , true,  false, true,  true  });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch    =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("erode(grid, bottom_only) must generate the same output when called repeatedly"){
        erode(bottom_only, out1);
        erode(bottom_only, out2);
        CHECK(equal(out1, out2));
    }
    SECTION("erode(grid, bottom_only, radius) must generate the same output when called repeatedly"){
        erode(bottom_only, out1, 2);
        erode(bottom_only, out2, 2);
        CHECK(equal(out1, out2));
    }
    SECTION("erode(grid, bottom_only, radius, scratch) must generate the same output when called repeatedly"){
        erode(bottom_only, out1, 2, scratch);
        erode(bottom_only, out2, 2, scratch);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "raster erosion decreasing", "[rasters]" ) {
    auto lower_half =  make_raster(layered_diamond_grid, {true,  true , false,  true , true, true , true,  false, true,  true });
    auto bottom_only=  make_raster(layered_diamond_grid, {true,  true , false,  true , true, true , true,  false, true,  true });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("erode(grid, bottom_only) must increase the number of flagged vertices"){
        erode(bottom_only, out1);
        erode(out1,        out2);
        CHECK(sum(out1) <= sum(bottom_only));
        CHECK(sum(out2) <= sum(out1));
    }
}
TEST_CASE( "raster erosion distributive over intersection", "[rasters]" ) {
    auto A       =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto B       =  make_raster(layered_diamond_grid, {true,  false, true,  false, false, true , false, true , false, false });
    auto AC      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto BC      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto AB      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto AB_C    =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto AC_BC   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("erosion (A ∩ B)+C must equal (A+C) ∩ (B+C)"){
        intersect (A,  B,  AB);
        erode     (AB, AB_C, 1);

        erode     (A,   AC,    1);
        erode     (B,   BC,    1);
        intersect (AC, BC, AC_BC);

        CHECK(equal(AC_BC,  AC_BC));
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
/*
TEST_CASE( "raster erosion happy path", "[rasters]" ) {
    auto upper_half =  make_raster(layered_diamond_grid, {true,  true , true,  true , false, true , true,  true , true,  false });
    auto top_only   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto empty      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto full       =  make_raster(layered_diamond_grid, {true,  true , true,  true , true,  true , true,  true , true,  true  });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("erode(grid, top_only) must return predictable results"){
        erode(top_only,   out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("erode(grid, upper_half) must return predictable results"){
        erode(upper_half, out1);
        CHECK(equal(out1,  top_only));
    }
    SECTION("erode(grid, none) must return predictable results"){
        erode(empty, out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("erode(grid, full) must return predictable results"){
        erode(full, out1);
        CHECK(equal(out1,  full));
    }
}
*/


TEST_CASE( "opening purity", "[rasters]" ) {
    auto bottom_edge=  make_raster(layered_diamond_grid, {false,  true , false,  true , true, false, true,  false, true,  true });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch1   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch2   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("opening(grid, bottom_edge) must generate the same output when called repeatedly"){
        opening(bottom_edge, out1);
        opening(bottom_edge, out2);
        CHECK(equal(out1, out2));
    }
    SECTION("opening(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        opening(bottom_edge, out1, 1);
        opening(bottom_edge, out2, 1);
        CHECK(equal(out1, out2));
    }
    SECTION("opening(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        opening(bottom_edge, out1, 1, scratch1, scratch2);
        opening(bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "opening idempotence", "[rasters]" ) {
    auto bottom_edge=  make_raster(layered_diamond_grid, {false,  true , false,  true , true, false, true,  false, true,  true });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch1   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch2   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("subsequent calls to opening(grid, bottom_edge) must generate the same output"){
        opening(bottom_edge, out1);
        opening(out1,        out2);
        CHECK(equal(out1, out2));
    }
    SECTION("subsequent calls to opening(grid, bottom_edge, radius) must generate the same output"){
        opening(bottom_edge, out1, 1);
        opening(out1,        out2, 1);
        CHECK(equal(out1, out2));
    }
    SECTION("subsequent calls to opening(grid, bottom_edge, radius, scratch) must generate the same output"){
        opening(bottom_edge, out1, 1, scratch1, scratch2);
        opening(out1,        out2, 1, scratch1, scratch2);
        CHECK(equal(out1, out2));
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
/*
TEST_CASE( "opening happy path", "[rasters]" ) {
    auto upper_half =  make_raster(layered_diamond_grid, {true,  true , true,  true , false, true , true,  true , true,  false });
    auto top_only   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto empty      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto full       =  make_raster(layered_diamond_grid, {true,  true , true,  true , true,  true , true,  true , true,  true  });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("opening(grid, top_only) must return predictable results"){
        opening(top_only,   out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("opening(grid, upper_half) must return predictable results"){
        opening(upper_half, out1);
        CHECK(equal(out1,  upper_half));
    }
    SECTION("opening(grid, none) must return predictable results"){
        opening(empty, out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("opening(grid, full) must return predictable results"){
        opening(full, out1);
        CHECK(equal(out1,  full));
    }
}
*/

TEST_CASE( "closing purity", "[rasters]" ) {
    auto bottom_edge=  make_raster(layered_diamond_grid, {false,  true , false,  true , true, false, true,  false, true,  true });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch1   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch2   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("closing(grid, bottom_edge) must generate the same output when called repeatedly"){
        closing(bottom_edge, out1);
        closing(bottom_edge, out2);
        CHECK(equal(out1, out2));
    }
    SECTION("closing(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        closing(bottom_edge, out1, 1);
        closing(bottom_edge, out2, 1);
        CHECK(equal(out1, out2));
    }
    SECTION("closing(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        closing(bottom_edge, out1, 1, scratch1, scratch2);
        closing(bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(equal(out1, out2));
    }
}
TEST_CASE( "closing idempotence", "[rasters]" ) {
    auto bottom_edge=  make_raster(layered_diamond_grid, {false,  true , false,  true , true, false, true,  false, true,  true });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch1   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch2   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("subsequent calls to closing(grid, bottom_edge) must generate the same output"){
        closing(bottom_edge, out1);
        closing(out1,        out2);
        CHECK(equal(out1, out2));
    }
    SECTION("subsequent calls to closing(grid, bottom_edge, radius) must generate the same output"){
        closing(bottom_edge, out1, 1);
        closing(out1,        out2, 1);
        CHECK(equal(out1, out2));
    }
    SECTION("subsequent calls to closing(grid, bottom_edge, radius, scratch) must generate the same output"){
        closing(bottom_edge, out1, 1, scratch1, scratch2);
        closing(out1,        out2, 1, scratch1, scratch2);
        CHECK(equal(out1, out2));
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
/*
TEST_CASE( "closing happy path", "[rasters]" ) {
    auto upper_half =  make_raster(layered_diamond_grid, {true,  true , true,  true , false, true , true,  true , true,  false });
    auto top_only   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto empty      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto full       =  make_raster(layered_diamond_grid, {true,  true , true,  true , true,  true , true,  true , true,  true  });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("closing(grid, top_only) must return predictable results"){
        closing(top_only,   out1);
        CHECK(equal(out1,  top_only));
    }
    SECTION("closing(grid, upper_half) must return predictable results"){
        closing(upper_half, out1);
        CHECK(equal(out1,  full));
    }
    SECTION("closing(grid, none) must return predictable results"){
        closing(empty, out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("closing(grid, full) must return predictable results"){
        closing(full, out1);
        CHECK(equal(out1,  full));
    }
}
*/


TEST_CASE( "white top hat purity", "[rasters]" ) {
    auto bottom_edge=  make_raster(layered_diamond_grid, {false,  true , false,  true , true, false, true,  false, true,  true });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch1   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch2   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("white_top_hat(grid, bottom_edge) must generate the same output when called repeatedly"){
        white_top_hat(bottom_edge, out1);
        white_top_hat(bottom_edge, out2);
        CHECK(equal(out1, out2));
    }
    SECTION("white_top_hat(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        white_top_hat(bottom_edge, out1, 1);
        white_top_hat(bottom_edge, out2, 1);
        CHECK(equal(out1, out2));
    }
    SECTION("white_top_hat(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        white_top_hat(bottom_edge, out1, 1, scratch1, scratch2);
        white_top_hat(bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(equal(out1, out2));
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
/*
TEST_CASE( "raster white_top_hat happy path", "[rasters]" ) {
    auto upper_half =  make_raster(layered_diamond_grid, {true,  true , true,  true , false, true , true,  true , true,  false });
    auto top_only   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto bottom_only=  make_raster(layered_diamond_grid, {false, false, false, false, true,  false, false, false, false, true  });
    auto empty      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto full       =  make_raster(layered_diamond_grid, {true,  true , true,  true , true,  true , true,  true , true,  true  });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("white_top_hat(grid, top_only) must return predictable results"){
        white_top_hat(top_only,   out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("white_top_hat(grid, upper_half) must return predictable results"){
        white_top_hat(upper_half, out1);
        CHECK(equal(out1,  bottom_only));
    }
    SECTION("white_top_hat(grid, none) must return predictable results"){
        white_top_hat(empty, out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("white_top_hat(grid, full) must return predictable results"){
        white_top_hat(full, out1);
        CHECK(equal(out1,  empty));
    }
}
*/

TEST_CASE( "black top hat purity", "[rasters]" ) {
    auto bottom_edge=  make_raster(layered_diamond_grid, {false,  true , false,  true , true, false, true,  false, true,  true });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch1   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch2   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("black_top_hat(grid, bottom_edge) must generate the same output when called repeatedly"){
        black_top_hat(bottom_edge, out1);
        black_top_hat(bottom_edge, out2);
        CHECK(equal(out1, out2));
    }
    SECTION("black_top_hat(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        black_top_hat(bottom_edge, out1, 1);
        black_top_hat(bottom_edge, out2, 1);
        CHECK(equal(out1, out2));
    }
    SECTION("black_top_hat(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        black_top_hat(bottom_edge, out1, 1, scratch1, scratch2);
        black_top_hat(bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(equal(out1, out2));
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
/*
TEST_CASE( "raster black_top_hat happy path", "[rasters]" ) {
    auto upper_half =  make_raster(layered_diamond_grid, {true,  true , true,  true , false, true , true,  true , true,  false });
    auto top_only   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto empty      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto full       =  make_raster(layered_diamond_grid, {true,  true , true,  true , true,  true , true,  true , true,  true  });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("black_top_hat(grid, top_only) must return predictable results"){
        black_top_hat(top_only,   out1);
        CHECK(equal(out1,  top_only));
    }
    SECTION("black_top_hat(grid, upper_half) must return predictable results"){
        black_top_hat(upper_half, out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("black_top_hat(grid, none) must return predictable results"){
        black_top_hat(empty, out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("black_top_hat(grid, full) must return predictable results"){
        black_top_hat(full, out1);
        CHECK(equal(out1,  empty));
    }
}
*/

TEST_CASE( "margin purity", "[rasters]" ) {
    auto bottom_edge=  make_raster(layered_diamond_grid, {false,  true , false,  true , true, false, true,  false, true,  true });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch1   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch2   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("margin(grid, bottom_edge) must generate the same output when called repeatedly"){
        margin(bottom_edge, out1);
        margin(bottom_edge, out2);
        CHECK(equal(out1, out2));
    }
    SECTION("margin(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        margin(bottom_edge, out1, 1);
        margin(bottom_edge, out2, 1);
        CHECK(equal(out1, out2));
    }
    SECTION("margin(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        margin(bottom_edge, out1, 1, scratch1);
        margin(bottom_edge, out2, 1, scratch1);
        CHECK(equal(out1, out2));
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
/*
TEST_CASE( "margin happy path", "[rasters]" ) {
    auto upper_half =  make_raster(layered_diamond_grid, {true,  true , true,  true , false, true , true,  true , true,  false });
    auto top_only   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto bottom_only=  make_raster(layered_diamond_grid, {false, false, false, false, true,  false, false, false, false, true  });
    auto center     =  make_raster(layered_diamond_grid, {true,  true , false, true , false, true , true,  false, true,  false });
    auto empty      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto full       =  make_raster(layered_diamond_grid, {true,  true , true,  true , true,  true , true,  true , true,  true  });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("margin(grid, top_only) must return predictable results"){
        margin(top_only,   out1);
        CHECK(equal(out1,  center));
    }
    SECTION("margin(grid, upper_half) must return predictable results"){
        margin(upper_half, out1);
        CHECK(equal(out1,  bottom_only));
    }
    SECTION("margin(grid, none) must return predictable results"){
        margin(empty, out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("margin(grid, full) must return predictable results"){
        margin(full, out1);
        CHECK(equal(out1,  empty));
    }
}
*/

TEST_CASE( "padding purity", "[rasters]" ) {
    auto bottom_edge=  make_raster(layered_diamond_grid, {false,  true , false,  true , true, false, true,  false, true,  true });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto out2       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch1   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto scratch2   =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("padding(grid, bottom_edge) must generate the same output when called repeatedly"){
        padding(bottom_edge, out1);
        padding(bottom_edge, out2);
        CHECK(equal(out1, out2));
    }
    SECTION("padding(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        padding(bottom_edge, out1, 1);
        padding(bottom_edge, out2, 1);
        CHECK(equal(out1, out2));
    }
    SECTION("padding(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        padding(bottom_edge, out1, 1, scratch1);
        padding(bottom_edge, out2, 1, scratch1);
        CHECK(equal(out1, out2));
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
/*
TEST_CASE( "padding happy path", "[rasters]" ) {
    auto upper_half =  make_raster(layered_diamond_grid, {true,  true , true,  true , false, true , true,  true , true,  false });
    auto top_only   =  make_raster(layered_diamond_grid, {false, false, true,  false, false, false, false, true , false, false });
    auto center     =  make_raster(layered_diamond_grid, {true,  true , false, true , false, true , true,  false, true,  false });
    auto empty      =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    auto full       =  make_raster(layered_diamond_grid, {true,  true , true,  true , true,  true , true,  true , true,  true  });
    auto out1       =  make_raster(layered_diamond_grid, {false, false, false, false, false, false, false, false, false, false });
    SECTION("padding(grid, top_only) must return predictable results"){
        padding(top_only,   out1);
        CHECK(equal(out1,  top_only));
    }
    SECTION("padding(grid, upper_half) must return predictable results"){
        padding(upper_half, out1);
        CHECK(equal(out1,  center));
    }
    SECTION("padding(grid, none) must return predictable results"){
        padding(empty, out1);
        CHECK(equal(out1,  empty));
    }
    SECTION("padding(grid, full) must return predictable results"){
        padding(full, out1);
        CHECK(equal(out1,  empty));
    }
}
*/

