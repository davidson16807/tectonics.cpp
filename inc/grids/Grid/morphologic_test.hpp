

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/convenience.hpp> //  operators, etc.

#include <meshes/mesh.hpp>

#include "morphologic.hpp"

#include "Grid_test_utils.hpp"

using namespace glm;
using namespace many;
using namespace rasters;


TEST_CASE( "raster dilation purity", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch    = bools({false, false, false, false, false });
    SECTION("dilate(grid, top_only) must generate the same output when called repeatedly"){
        dilate(diamond, top_only, out1);
        dilate(diamond, top_only, out2);
        CHECK(out1==out2);
    }
    SECTION("dilate(grid, top_only, radius) must generate the same output when called repeatedly"){
        dilate(diamond, top_only, out1, 2);
        dilate(diamond, top_only, out2, 2);
        CHECK(out1==out2);
    }
    SECTION("dilate(grid, top_only, radius, scratch) must generate the same output when called repeatedly"){
        dilate(diamond, top_only, out1, 2, scratch);
        dilate(diamond, top_only, out2, 2, scratch);
        CHECK(out1==out2);
    }
}
TEST_CASE( "raster dilation increasing", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    SECTION("dilate(grid, top_only) must increase the number of flagged vertices"){
        dilate(diamond, top_only, out1);
        dilate(diamond, out1,     out2);
        CHECK(sum(out1) >= sum(top_only));
        CHECK(sum(out2) >= sum(out1));
    }
}
TEST_CASE( "raster dilation associative", "[rasters]" ) {
    bools A   = bools({false, false, true,  false, false });
    bools AB  = bools({false, false, false, false, false });
    bools ABC = bools({false, false, false, false, false });
    bools BC  = bools({false, false, false, false, false });
    bools BCA = bools({false, false, false, false, false });
    SECTION("dilation (A+B)+C must equal A+(B+C) where B and C are circles of radius 1 and 2"){
        dilate(diamond, A,  AB,  1);
        dilate(diamond, AB, ABC, 2);
        dilate(diamond, A,  BCA, 3);
        CHECK(ABC == BCA);
    }
}
TEST_CASE( "raster dilation distributive over union", "[rasters]" ) {
    bools A       = bools({false, false, true,  false, false });
    bools B       = bools({true,  false, true,  false, false });
    bools AC      = bools({false, false, false, false, false });
    bools BC      = bools({false, false, false, false, false });
    bools AB      = bools({false, false, false, false, false });
    bools AB_C    = bools({false, false, false, false, false });
    bools AC_BC   = bools({false, false, false, false, false });
    SECTION("dilation (A ∪ B)+C must equal (A+C) ∪ (B+C)"){
        unite (A,  B,  AB);
        dilate(diamond, AB, AB_C, 1);

        dilate(diamond, A,   AC,  1);
        dilate(diamond, B,   BC,  1);
        unite (AC, BC, AC_BC);

        CHECK(AB_C == AC_BC);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "raster dilation happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("dilate(grid, top_only) must return predictable results"){
        dilate(diamond, top_only, out1);
        CHECK(out1 == upper_half);
    }
    SECTION("dilate(grid, upper_half) must return predictable results"){
        dilate(diamond, upper_half, out1);
        CHECK(out1 == full);
    }
    SECTION("dilate(grid, empty) must return predictable results"){
        dilate(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("dilate(grid, full) must return predictable results"){
        dilate(diamond, full, out1);
        CHECK(out1 == full);
    }
}
TEST_CASE( "raster erosion purity", "[rasters]" ) {
    bools lower_half = bools({true,  true,  false,  true,  true });
    bools bottom_only= bools({true,  true,  false,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch    = bools({false, false, false, false, false });
    SECTION("erode(grid, bottom_only) must generate the same output when called repeatedly"){
        erode(diamond, bottom_only, out1);
        erode(diamond, bottom_only, out2);
        CHECK(out1==out2);
    }
    SECTION("erode(grid, bottom_only, radius) must generate the same output when called repeatedly"){
        erode(diamond, bottom_only, out1, 2);
        erode(diamond, bottom_only, out2, 2);
        CHECK(out1==out2);
    }
    SECTION("erode(grid, bottom_only, radius, scratch) must generate the same output when called repeatedly"){
        erode(diamond, bottom_only, out1, 2, scratch);
        erode(diamond, bottom_only, out2, 2, scratch);
        CHECK(out1==out2);
    }
}
TEST_CASE( "raster erosion decreasing", "[rasters]" ) {
    bools lower_half = bools({true,  true,  false,  true,  true });
    bools bottom_only= bools({true,  true,  false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    SECTION("erode(grid, bottom_only) must increase the number of flagged vertices"){
        erode(diamond, bottom_only, out1);
        erode(diamond, out1,        out2);
        CHECK(sum(out1) <= sum(bottom_only));
        CHECK(sum(out2) <= sum(out1));
    }
}
TEST_CASE( "raster erosion distributive over intersection", "[rasters]" ) {
    bools A       = bools({false, false, true,  false, false });
    bools B       = bools({true,  false, true,  false, false });
    bools AC      = bools({false, false, false, false, false });
    bools BC      = bools({false, false, false, false, false });
    bools AB      = bools({false, false, false, false, false });
    bools AB_C    = bools({false, false, false, false, false });
    bools AC_BC   = bools({false, false, false, false, false });
    SECTION("erosion (A ∩ B)+C must equal (A+C) ∩ (B+C)"){
        intersect (A,  B,  AB);
        erode     (diamond, AB, AB_C, 1);

        erode     (diamond, A,   AC,    1);
        erode     (diamond, B,   BC,    1);
        intersect (AC, BC, AC_BC);

        CHECK(AC_BC == AC_BC);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "raster erosion happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("erode(grid, top_only) must return predictable results"){
        erode(diamond, top_only,   out1);
        CHECK(out1 == empty);
    }
    SECTION("erode(grid, upper_half) must return predictable results"){
        erode(diamond, upper_half, out1);
        CHECK(out1 == top_only);
    }
    SECTION("erode(grid, none) must return predictable results"){
        erode(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("erode(grid, full) must return predictable results"){
        erode(diamond, full, out1);
        CHECK(out1 == full);
    }
}


TEST_CASE( "opening purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("opening(grid, bottom_edge) must generate the same output when called repeatedly"){
        opening(diamond, bottom_edge, out1);
        opening(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("opening(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        opening(diamond, bottom_edge, out1, 1);
        opening(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("opening(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        opening(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        opening(diamond, bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
TEST_CASE( "opening idempotence", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("subsequent calls to opening(grid, bottom_edge) must generate the same output"){
        opening(diamond, bottom_edge, out1);
        opening(diamond, out1,        out2);
        CHECK(out1==out2);
    }
    SECTION("subsequent calls to opening(grid, bottom_edge, radius) must generate the same output"){
        opening(diamond, bottom_edge, out1, 1);
        opening(diamond, out1,        out2, 1);
        CHECK(out1==out2);
    }
    SECTION("subsequent calls to opening(grid, bottom_edge, radius, scratch) must generate the same output"){
        opening(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        opening(diamond, out1,        out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "opening happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("opening(grid, top_only) must return predictable results"){
        opening(diamond, top_only,   out1);
        CHECK(out1 == empty);
    }
    SECTION("opening(grid, upper_half) must return predictable results"){
        opening(diamond, upper_half, out1);
        CHECK(out1 == upper_half);
    }
    SECTION("opening(grid, none) must return predictable results"){
        opening(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("opening(grid, full) must return predictable results"){
        opening(diamond, full, out1);
        CHECK(out1 == full);
    }
}

TEST_CASE( "closing purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("closing(grid, bottom_edge) must generate the same output when called repeatedly"){
        closing(diamond, bottom_edge, out1);
        closing(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("closing(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        closing(diamond, bottom_edge, out1, 1);
        closing(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("closing(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        closing(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        closing(diamond, bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
TEST_CASE( "closing idempotence", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("subsequent calls to closing(grid, bottom_edge) must generate the same output"){
        closing(diamond, bottom_edge, out1);
        closing(diamond, out1,        out2);
        CHECK(out1==out2);
    }
    SECTION("subsequent calls to closing(grid, bottom_edge, radius) must generate the same output"){
        closing(diamond, bottom_edge, out1, 1);
        closing(diamond, out1,        out2, 1);
        CHECK(out1==out2);
    }
    SECTION("subsequent calls to closing(grid, bottom_edge, radius, scratch) must generate the same output"){
        closing(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        closing(diamond, out1,        out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "closing happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("closing(grid, top_only) must return predictable results"){
        closing(diamond, top_only,   out1);
        CHECK(out1 == top_only);
    }
    SECTION("closing(grid, upper_half) must return predictable results"){
        closing(diamond, upper_half, out1);
        CHECK(out1 == full);
    }
    SECTION("closing(grid, none) must return predictable results"){
        closing(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("closing(grid, full) must return predictable results"){
        closing(diamond, full, out1);
        CHECK(out1 == full);
    }
}


TEST_CASE( "white top hat purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("white_top_hat(grid, bottom_edge) must generate the same output when called repeatedly"){
        white_top_hat(diamond, bottom_edge, out1);
        white_top_hat(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("white_top_hat(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        white_top_hat(diamond, bottom_edge, out1, 1);
        white_top_hat(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("white_top_hat(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        white_top_hat(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        white_top_hat(diamond, bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "raster white_top_hat happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools bottom_only= bools({false, false, false, false, true  });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("white_top_hat(grid, top_only) must return predictable results"){
        white_top_hat(diamond, top_only,   out1);
        CHECK(out1 == empty);
    }
    SECTION("white_top_hat(grid, upper_half) must return predictable results"){
        white_top_hat(diamond, upper_half, out1);
        CHECK(out1 == bottom_only);
    }
    SECTION("white_top_hat(grid, none) must return predictable results"){
        white_top_hat(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("white_top_hat(grid, full) must return predictable results"){
        white_top_hat(diamond, full, out1);
        CHECK(out1 == empty);
    }
}

TEST_CASE( "black top hat purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("black_top_hat(grid, bottom_edge) must generate the same output when called repeatedly"){
        black_top_hat(diamond, bottom_edge, out1);
        black_top_hat(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("black_top_hat(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        black_top_hat(diamond, bottom_edge, out1, 1);
        black_top_hat(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("black_top_hat(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        black_top_hat(diamond, bottom_edge, out1, 1, scratch1, scratch2);
        black_top_hat(diamond, bottom_edge, out2, 1, scratch1, scratch2);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "raster black_top_hat happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("black_top_hat(grid, top_only) must return predictable results"){
        black_top_hat(diamond, top_only,   out1);
        CHECK(out1 == top_only);
    }
    SECTION("black_top_hat(grid, upper_half) must return predictable results"){
        black_top_hat(diamond, upper_half, out1);
        CHECK(out1 == empty);
    }
    SECTION("black_top_hat(grid, none) must return predictable results"){
        black_top_hat(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("black_top_hat(grid, full) must return predictable results"){
        black_top_hat(diamond, full, out1);
        CHECK(out1 == empty);
    }
}

TEST_CASE( "margin purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("margin(grid, bottom_edge) must generate the same output when called repeatedly"){
        margin(diamond, bottom_edge, out1);
        margin(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("margin(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        margin(diamond, bottom_edge, out1, 1);
        margin(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("margin(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        margin(diamond, bottom_edge, out1, 1, scratch1);
        margin(diamond, bottom_edge, out2, 1, scratch1);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "margin happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools bottom_only= bools({false, false, false, false, true  });
    bools center     = bools({true,  true,  false, true,  false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("margin(grid, top_only) must return predictable results"){
        margin(diamond, top_only,   out1);
        CHECK(out1 == center);
    }
    SECTION("margin(grid, upper_half) must return predictable results"){
        margin(diamond, upper_half, out1);
        CHECK(out1 == bottom_only);
    }
    SECTION("margin(grid, none) must return predictable results"){
        margin(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("margin(grid, full) must return predictable results"){
        margin(diamond, full, out1);
        CHECK(out1 == empty);
    }
}

TEST_CASE( "padding purity", "[rasters]" ) {
    bools bottom_edge= bools({false,  true, false,  true,  true });
    bools out1       = bools({false, false, false, false, false });
    bools out2       = bools({false, false, false, false, false });
    bools scratch1   = bools({false, false, false, false, false });
    bools scratch2   = bools({false, false, false, false, false });
    SECTION("padding(grid, bottom_edge) must generate the same output when called repeatedly"){
        padding(diamond, bottom_edge, out1);
        padding(diamond, bottom_edge, out2);
        CHECK(out1==out2);
    }
    SECTION("padding(grid, bottom_edge, radius) must generate the same output when called repeatedly"){
        padding(diamond, bottom_edge, out1, 1);
        padding(diamond, bottom_edge, out2, 1);
        CHECK(out1==out2);
    }
    SECTION("padding(grid, bottom_edge, radius, scratch) must generate the same output when called repeatedly"){
        padding(diamond, bottom_edge, out1, 1, scratch1);
        padding(diamond, bottom_edge, out2, 1, scratch1);
        CHECK(out1==out2);
    }
}
/* 
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
TEST_CASE( "padding happy path", "[rasters]" ) {
    bools upper_half = bools({true,  true,  true,  true,  false });
    bools top_only   = bools({false, false, true,  false, false });
    bools center     = bools({true,  true,  false, true,  false });
    bools empty      = bools({false, false, false, false, false });
    bools full       = bools({true,  true,  true,  true,  true  });
    bools out1       = bools({false, false, false, false, false });
    SECTION("padding(grid, top_only) must return predictable results"){
        padding(diamond, top_only,   out1);
        CHECK(out1 == top_only);
    }
    SECTION("padding(grid, upper_half) must return predictable results"){
        padding(diamond, upper_half, out1);
        CHECK(out1 == center);
    }
    SECTION("padding(grid, none) must return predictable results"){
        padding(diamond, empty, out1);
        CHECK(out1 == empty);
    }
    SECTION("padding(grid, full) must return predictable results"){
        padding(diamond, full, out1);
        CHECK(out1 == empty);
    }
}

