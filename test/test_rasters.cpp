

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/string_cast.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/string_cast.hpp>  
#include <many/glm/convenience.hpp> //  operators, etc.

#include <rasters/types.hpp>
#include <rasters/string_cast.hpp>  
#include <rasters/glm/glm.hpp>
#include <rasters/glm/string_cast.hpp>  
#include <rasters/glm/vector_calculus.hpp>

// #include "academics/tectonics.hpp"

using namespace glm;
using namespace many;
using namespace rasters;

/* 
"diamond" is a simple 2d grid for testing raster operations 
 that do not require spatial awareness (e.g. arithmetic on scalar fields)
It looks like this:
     2  
    /|\ 
   3-0-1
    \|/ 
     4   
*/
std::shared_ptr<Grid> diamond = 
    std::make_shared<Grid>(
        vec3s({
                vec3( 0, 0, 0),
                vec3( 1, 0, 0),
                vec3( 0, 1, 0),
                vec3(-1, 0, 0),
                vec3( 0,-1, 0)
            }),
        uvec3s({
                uvec3(0,1,2),
                uvec3(0,1,4),
                uvec3(0,3,2),
                uvec3(0,3,4)
            })
    );
/*
"tetrahedron" is a simple 3d grid for testing basic raster operations
 that require spatial awareness without requiring a particular shape.
 (e.g. gradient, divergence, laplacian)
*/
std::shared_ptr<Grid> tetrahedron = 
    std::make_shared<Grid>(
        vec3s({
                vec3(0,0,0),
                vec3(1,0,0),
                vec3(0,1,0),
                vec3(0,0,1)
            }),
        uvec3s({
                uvec3(0,1,2),
                uvec3(0,1,3),
                uvec3(0,2,3),
                uvec3(1,2,3)
            })
    );
/*
"octahedron" is a simple 3d grid for testing raster operations that require 
something comparable to a unit sphere (e.g. nearest neighbor lookups using SphereGridVoronoi)
*/
std::shared_ptr<Grid> octahedron = 
    std::make_shared<Grid>(
        normalize(vec3s({
                vec3( 1, 0, 0),
                vec3(-1, 0, 0),
                vec3( 0, 1,-1),
                vec3( 0,-1,-1),
                vec3( 0, 1, 1),
                vec3( 0,-1, 1),
            })),
        uvec3s({
                uvec3( 0, 2, 3),
                uvec3( 0, 2, 4),
                uvec3( 0, 2, 5),
                uvec3( 0, 3, 4),
                uvec3( 0, 3, 5),
                uvec3( 0, 4, 5),
                uvec3( 1, 2, 3),
                uvec3( 1, 2, 4),
                uvec3( 1, 2, 5),
                uvec3( 1, 3, 4),
                uvec3( 1, 3, 5),
                uvec3( 1, 4, 5),
            })
    );

SphereGridVoronoi voronoi_test(
        vec3s({
                normalize(vec3( 1, 0, 0)),
                normalize(vec3( 0, 1, 0)),
                normalize(vec3( 0, 0, 1)),
                normalize(vec3(-1, 0, 0)),
                normalize(vec3( 0,-1, 0)),
                normalize(vec3( 0, 0,-1)),
                normalize(vec3(-1,-1,-1)),
                normalize(vec3( 1,-1,-1)),
                normalize(vec3(-1, 1,-1)),
                normalize(vec3( 1, 1,-1)),
                normalize(vec3(-1,-1, 1)),
                normalize(vec3( 1,-1, 1)),
                normalize(vec3(-1, 1, 1)),
                normalize(vec3( 1, 1, 1)),
                normalize(vec3( 1, 1, 1))
            }),
        1./100.
    );


TEST_CASE( "Must be able to test many<T> equivalence using the catch framework", "[many]" ) {
    floats ref       = floats({1,2,3,4,5});
    floats ref_copy  = floats({1,2,3,4,5});
    floats ref_tweak = floats({1,2,3,4,0});
    floats ref_fewer = floats({1,2,3,4});

    SECTION("Must be able to equate object with itself"){
        CHECK(ref == ref);
    }
    SECTION("Must be able to equate many of the same content and size"){
        CHECK(ref == ref_copy);
    }
    SECTION("Must be able to distinguish many of slightly different content"){
        CHECK(ref != ref_tweak);
    }
    SECTION("Must be able to distinguish many of slightly different size"){
        CHECK(ref != ref_fewer);
    }
}


TEST_CASE( "Equivalent many<T> arithmetic must behave equivalently", "[many]" ) {
    floats a1 = floats({1,2,3,4,5});
    floats a2 = floats({1,2,3,4,5});
    floats a3 = floats({1,2,3,4,5});
    floats b = floats({-1,1,-2,2,3});

    SECTION("a+=b, a=a+b, and add(a,b,a) must all work the same"){
        a1 += b;
        a2 = a2+b;
        add(a3,b,a3);
        
        CHECK(a1==a2);
        CHECK(a2==a3);
    }

    SECTION("a*=b, a=a*b, and add(a,b,a) must all work the same"){
        a1 *= b;
        a2 = a2*b;
        mult(a3,b,a3);
        
        CHECK(a1==a2);
        CHECK(a2==a3);
    }

    SECTION("a-=b, a=a-b, and add(a,b,a) must all work the same"){
        a1 -= b;
        a2 = a2-b;
        sub(a3,b,a3);
        
        CHECK(a1==a2);
        CHECK(a2==a3);
    }

    SECTION("a/=b, a=a/b, and add(a,b,a) must all work the same"){
        a1 /= b;
        a2 = a2/b;
        div(a3,b,a3);
        
        CHECK(a1==a2);
        CHECK(a2==a3);
    }
}


TEST_CASE( "many<T> arithmetic must have idempotence: the operation can be called repeatedly without changing the value", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats b = floats({-1,1,-2,2,3});
    floats c1 = floats({0,0,0,0,0});
    floats c2 = floats({0,0,0,0,0});

    SECTION("a+b must be called repeatedly without changing the output"){
        c1 = a + b;
        c2 = a + b;
        CHECK(c1==c2);

        add(a,b,c1);
        add(a,b,c2);
        CHECK(c1==c2);
    }

    SECTION("a*b must be called repeatedly without changing the output"){
        c1 = a * b;
        c2 = a * b;
        CHECK(c1==c2);
        
        mult(a,b,c1);
        mult(a,b,c2);
        CHECK(c1==c2);
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        c1 = a - b;
        c2 = a - b;
        CHECK(c1==c2);
        
        sub(a,b,c1);
        sub(a,b,c2);
        CHECK(c1==c2);
    }

    SECTION("a/b must be called repeatedly without changing the output"){
        c1 = a / b;
        c2 = a / b;
        CHECK(c1==c2);
        
        div(a,b,c1);
        div(a,b,c2);
        CHECK(c1==c2);
    }
}



TEST_CASE( "many<T> arithmetic must have an identity: a value exists that can be applied without effect", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats zeros = floats({0,0,0,0,0});
    floats ones  = floats({1,1,1,1,1});

    SECTION("a+I must equal a"){
        CHECK(a+zeros==a);
        CHECK(a-zeros==a);
        CHECK(a*ones ==a);
        CHECK(a/ones ==a);
        CHECK(a-a==zeros);
        CHECK(a/a==ones );
    }
}


TEST_CASE( "many<T> arithmetic must have commutativity: values can be swapped to the same effect", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats b = floats({-1,1,-2,2,3});

    SECTION("a+b must equal b+a"){
        CHECK(a+b==b+a);
    }
    SECTION("a*b must equal b*a"){
        CHECK(a*b==b*a);
    }
}


TEST_CASE( "many<T> arithmetic must have associativity: operations can be applied in a different order to the same effect", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats b = floats({-1,1,-2,2,3});
    floats c = floats({1,1,2,3,5});

    SECTION("(a+b)+c must equal a+(b+c)"){
        CHECK((a+b)+c==a+(b+c));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        CHECK((a*b)*c==a*(b*c));
    }
}


TEST_CASE( "many<T> arithmetic must have distributivity: an operation can be distributed as values of another", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats b = floats({-1,1,-2,2,3});
    floats c = floats({1,1,2,3,5});

    SECTION("a+b must equal b+a"){
        CHECK((a+b)*c==(a*c+b*c));
    }
    SECTION("a+b must equal b+a"){
        CHECK((a+b)/c==(a/c+b/c));
    }
}


TEST_CASE( "many<T> min/max must have associativity: operations can be applied in a different order to the same effect", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats b = floats({-1,1,-2,2,3});
    floats c = floats({1,1,2,3,5});
    floats min1 = floats({0,0,0,0,0});
    floats min2 = floats({0,0,0,0,0});
    floats max1 = floats({0,0,0,0,0});
    floats max2 = floats({0,0,0,0,0});

    SECTION("min(min(a,b),c) must equal min(a, min(b,c))"){
        min(a,b,min1);
        min(min1,c,min1);

        min(b,c,min2);
        min(a,min2,min2);

        CHECK(min1 == min2);
    }
    SECTION("max(max(a,b),c) must equal max(a, max(b,c))"){
        max(a,b,max1);
        max(max1,c,max1);

        max(b,c,max2);
        max(a,max2,max2);

        CHECK(max1 == max2);
    }
}

TEST_CASE( "many<T> comparators must be consistant", "[many]" ) {
    floats a    = floats({1,2,3,4,5});
    floats b    = floats({1,1,2,3,5});
    bools  gt   = bools({0,0,0,0,0});
    bools  lt   = bools({0,0,0,0,0});
    bools  eq   = bools({0,0,0,0,0});
    bools  ne1  = bools({0,0,0,0,0});
    bools  ne2  = bools({0,0,0,0,0});
    bools  gte1 = bools({0,0,0,0,0});
    bools  gte2 = bools({0,0,0,0,0});
    bools  lte1 = bools({0,0,0,0,0});
    bools  lte2 = bools({0,0,0,0,0});

    SECTION("a>=b must equal a>b || a==b"){
        greaterThanEqual(a,b,   gte1);
        greaterThan     (a,b,   gt);
        equal           (a,b,   eq);
        unite           (gt,eq, gte2);
        CHECK(gte1 == gte2);
    }

    SECTION("a<=b must equal a<b || a==b"){
        greaterThanEqual(a,b,   lte1);
        greaterThan     (a,b,   lt);
        equal           (a,b,   eq);
        unite           (lt,eq, lte2);
        CHECK(lte1 == lte2);
    }

    SECTION("a!=b must equal !(a==b)"){
        equal           (a,b, eq);
        negate          (eq,  ne1);
        notEqual        (a,b, ne2);
        CHECK(ne1 == ne2);
    }
}

TEST_CASE( "many<T> arithmetic must be consistant", "[many]" ) {
    floats a     = floats({1,2,3,4,5});
    floats b     = floats({1,1,2,3,5});
    floats sqrt1 = floats({0,0,0,0,0});
    floats sqrt2 = floats({0,0,0,0,0});
    floats log1_ = floats({0,0,0,0,0});
    floats log2_ = floats({0,0,0,0,0});

    SECTION("sqrt(a) must equal pow(a,1/2)"){
        sqrt(a,    sqrt1);
        pow (a,0.5f,sqrt2);
        CHECK(sqrt1 == sqrt2);

        sqrt(b,    sqrt1);
        pow (b,0.5f,sqrt2);
        CHECK(sqrt1 == sqrt2);
    }

    // TODO: not sure why this doesn't pass, off by factor of 2
    // SECTION("log2(a) must equal log(a)/log(2)"){
    //     log2(a, log1_);
    //     log (a, log2_);
    //     div (log2_, std::log(2.f), log2_);
    //     CHECK(log1_ == log2_);
    // }
}

TEST_CASE( "floats string cast must render correct representation", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats b = floats({1,1,2,3,5});
    floats c = floats({4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
                       4,2,5,INFINITY,9,4,6,2,8,1,5,3,7,5,8,5,6,6,7,6,
                       1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
                       4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7,5,
                       4,8,3,8,2,4,5,9,8,2,3,5,2,1,3,3,3,1,6,1,
                       4,2,5,9,9,9,9,9,9,9,9,9,9,9,8,5,6,6,7,6,
                       1,2,4,1,4,9,9,8,1,3,7,2,5,5,1,8,9,4,7,6,
                       4,4,4,5,3,1,3,5,8,3,4,1,3,7,5,6,2,9,7});
    std::string stra = to_string(a);
    std::string strb = to_string(b);
    std::string strc = to_string(c);
    SECTION("to_string() must render correct representation"){
        REQUIRE_THAT(stra, Catch::Contains(" ░▒▓█"));
        REQUIRE_THAT(strb, Catch::Contains("  ░▒█"));
        REQUIRE_THAT(to_string(floats({INFINITY})), Catch::Contains("∞"));
        // REQUIRE_THAT(to_string(floats({std::sqrt(-1)})), Catch::Contains("N"));
    }
}

TEST_CASE( "vec2s string cast must render correct representation", "[many]" ) {
    vec2s v2d = vec2s({
        vec2(-1,-1),
        vec2(-1, 1),
        vec2(-1,-0.1),
        vec2(-1, 0.1),
        vec2(-1, 0.0),
    });
    vec3s v3d = vec3s({
        vec3(-1,-1,-1),
        vec3( 0,-1,-1),
        vec3( 1,-1,-1),
        vec3(-1, 0,-1),
        vec3( 0, 0,-1),
        vec3( 1, 0,-1),
        vec3(-1, 1,-1),
        vec3( 0, 1,-1),
        vec3( 1, 1,-1),

        vec3(-1,-1, 0),
        vec3( 0,-1, 0),
        vec3( 1,-1, 0),
        vec3(-1, 0, 0),
        vec3( 0, 0, 0),
        vec3( 1, 0, 0),
        vec3(-1, 1, 0),
        vec3( 0, 1, 0),
        vec3( 1, 1, 0),

        vec3(-1,-1, 1),
        vec3( 0,-1, 1),
        vec3( 1,-1, 1),
        vec3(-1, 0, 1),
        vec3( 0, 0, 1),
        vec3( 1, 0, 1),
        vec3(-1, 1, 1),
        vec3( 0, 1, 1),
        vec3( 1, 1, 1),
    });
    SECTION("to_string() must produce obvious results for straight forward 2d vectors"){
        REQUIRE_THAT(to_string(vec2s({vec2( 0,   2),vec2( 0,   5)})), Catch::Contains("↑") && Catch::Contains("⬆"));
        REQUIRE_THAT(to_string(vec2s({vec2( 0,  -2),vec2( 0,  -5)})), Catch::Contains("↓") && Catch::Contains("⬇"));
        REQUIRE_THAT(to_string(vec2s({vec2( 2,   0),vec2( 5,   0)})), Catch::Contains("→") && Catch::Contains("➡"));
    }
    SECTION("to_string() must behave correctly around edge case for atan2()"){
        // WARNING: white box testing, we need to test left arrows extra carefully since they cause a discontinuity in atan2()
        REQUIRE_THAT(to_string(vec2s({vec2(-2,   2),vec2(-5,   5)})), Catch::Contains("↖") && Catch::Contains("⬉"));
        REQUIRE_THAT(to_string(vec2s({vec2(-2,-0.1),vec2(-5,-0.1)})), Catch::Contains("←") && Catch::Contains("⬅"));
        REQUIRE_THAT(to_string(vec2s({vec2(-2,   0),vec2(-5,   0)})), Catch::Contains("←") && Catch::Contains("⬅"));
        REQUIRE_THAT(to_string(vec2s({vec2(-2, 0.1),vec2(-5,-0.1)})), Catch::Contains("←") && Catch::Contains("⬅"));
        REQUIRE_THAT(to_string(vec2s({vec2(-2,  -2),vec2(-5,  -5)})), Catch::Contains("↙") && Catch::Contains("⬋"));
    }
}

TEST_CASE( "Must be able to test equivalence of rasters using the catch framework", "[rasters]" ) {
    raster ref       = raster(diamond, {1,2,3,4,5});
    raster ref_copy  = raster(diamond, {1,2,3,4,5});
    raster ref_tweak = raster(diamond, {1,2,3,4,0});
    raster ref_fewer = raster(tetrahedron, {1,2,3,4});

    raster a = raster(octahedron, {1,2,3,4,5,6});

    vec3raster v = vec3raster(octahedron, {
        vec3(-1,-1,-1),
        vec3(-1,-1,-1),
        vec3(-1,-1,-1),
        vec3(-1,-1,-1),
        vec3(-1,-1,-1),
        vec3(-1,-1,-1)
    });

    SECTION("Must be able to equate object with itself"){
        CHECK(ref == ref);
    }
    SECTION("Must be able to equate rasters of the same content and size"){
        CHECK(ref == ref_copy);
    }
    SECTION("Must be able to distinguish rasters of slightly different content"){
        CHECK(ref != ref_tweak);
    }
    SECTION("Must be able to distinguish rasters of slightly different size"){
        CHECK(ref != ref_fewer);
    }
}
