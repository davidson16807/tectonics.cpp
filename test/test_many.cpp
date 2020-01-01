

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable SIMD support for glm so we can work with webassembly
#include <glm/vec2.hpp>               // *vec3
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/string_cast.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/string_cast.hpp>  
#include <many/glm/random.hpp>      // get_elias_noise
#include <many/glm/convenience.hpp> //  operators, etc.

using namespace glm;
using namespace many;

TEST_CASE( "Must be able to test many<float> equivalence using the catch framework", "[many]" ) {
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

TEST_CASE( "Must be able to test many<bool> equivalence using the catch framework", "[many]" ) {
    bools ref       = bools({true,false,true,false,true});
    bools ref_copy  = bools({true,false,true,false,true});
    bools ref_tweak = bools({true,false,true,false,false});
    bools ref_fewer = bools({true,false,true,false});

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


TEST_CASE( "many<T> arithmetic consistency", "[many]" ) {
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


TEST_CASE( "many<T> arithmetic purity", "[many]" ) {
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



TEST_CASE( "many<T> arithmetic identity", "[many]" ) {
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

TEST_CASE( "many<T> arithmetic commutativity", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats b = floats({-1,1,-2,2,3});

    SECTION("a+b must equal b+a"){
        CHECK(a+b==b+a);
    }
    SECTION("a*b must equal b*a"){
        CHECK(a*b==b*a);
    }
}


TEST_CASE( "many<T> arithmetic associativity", "[many]" ) {
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


TEST_CASE( "many<T> arithmetic distributivity", "[many]" ) {
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


TEST_CASE( "many<T> min/max purity", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats b = floats({1,1,2,3,5});
    floats c1 = floats({0,0,0,0,0});
    floats c2 = floats({0,0,0,0,0});

    SECTION("min(a,b) must be called repeatedly without changing the output"){
        min(a,b,c1);
        min(a,b,c2);
        CHECK(c1==c2);
    }
    SECTION("max(a,b) must be called repeatedly without changing the output"){
        max(a,b,c1);
        max(a,b,c2);
        CHECK(c1==c2);
    }
}
TEST_CASE( "many<T> min/max associativity", "[many]" ) {
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


TEST_CASE( "many<T> comparator purity", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats b = floats({1,1,2,3,5});
    bools c1 = bools({0,0,0,0,0});
    bools c2 = bools({0,0,0,0,0});
    SECTION("greaterThanEqual(a,b) must be called repeatedly without changing the output"){
        greaterThanEqual(a,b,c1);
        greaterThanEqual(a,b,c2);
        CHECK(c1==c2);
    }
    SECTION("greaterThan(a,b) must be called repeatedly without changing the output"){
        greaterThan(a,b,c1);
        greaterThan(a,b,c2);
        CHECK(c1==c2);
    }
    SECTION("lessThanEqual(a,b) must be called repeatedly without changing the output"){
        lessThanEqual(a,b,c1);
        lessThanEqual(a,b,c2);
        CHECK(c1==c2);
    }
    SECTION("lessThan(a,b) must be called repeatedly without changing the output"){
        lessThan(a,b,c1);
        lessThan(a,b,c2);
        CHECK(c1==c2);
    }
    SECTION("equal(a,b) must be called repeatedly without changing the output"){
        equal(a,b,c1);
        equal(a,b,c2);
        CHECK(c1==c2);
    }
}
TEST_CASE( "many<T> comparator consistency", "[many]" ) {
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

TEST_CASE( "many<T> sqrt purity", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    floats c1 = floats({0,0,0,0,0});
    floats c2 = floats({0,0,0,0,0});
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        sqrt(a,c1);
        sqrt(a,c2);
        CHECK(c1==c2);
    }
}
TEST_CASE( "many<T> sqrt consistency", "[many]" ) {
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

}
TEST_CASE( "many<T> log consistency", "[many]" ) {
    floats a     = floats({1,2,3,4,5});
    floats b     = floats({1,1,2,3,5});
    floats log1_ = floats({0,0,0,0,0});
    floats log2_ = floats({0,0,0,0,0});

    // TODO: not sure why this doesn't pass, off by factor of 2
    // SECTION("log2(a) must equal log(a)/log(2)"){
    //     log2(a, log1_);
    //     log (a, log2_);
    //     div (log2_, std::log(2.f), log2_);
    //     CHECK(log1_ == log2_);
    // }
}

TEST_CASE( "many<T> string cast purity", "[many]" ) {
    floats a = floats({1,2,3,4,5});
    SECTION("to_string(a) must be called repeatedly without changing the output"){
        CHECK(to_string(a) == to_string(a));
    }
}
TEST_CASE( "many<T> string cast correctness", "[many]" ) {
    floats a = floats({1,2,3,4,5,6});
    floats b = floats({1,1,2,3,5,8});
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
        REQUIRE_THAT(stra, Catch::Contains("█"));
        REQUIRE_THAT(strb, Catch::Contains("█"));
        REQUIRE_THAT(to_string(floats({INFINITY})), Catch::Contains("∞"));
        // REQUIRE_THAT(to_string(floats({std::sqrt(-1)})), Catch::Contains("N"));
    }
}

TEST_CASE( "many<vec2> string cast correctness", "[many]" ) {
    SECTION("to_string() must produce obvious results for straight forward 2d vectors"){
        REQUIRE_THAT(to_string(vec2s({vec2( 0,   2),vec2( 0,   5)})), Catch::Contains("↑") && Catch::Contains("⬆"));
        REQUIRE_THAT(to_string(vec2s({vec2( 0,  -2),vec2( 0,  -5)})), Catch::Contains("↓") && Catch::Contains("⬇"));
        REQUIRE_THAT(to_string(vec2s({vec2( 2,   0),vec2( 5,   0)})), Catch::Contains("→") && Catch::Contains("➡"));
        REQUIRE_THAT(to_string(vec2s({vec2(-2,   0),vec2(-5,   0)})), Catch::Contains("←") && Catch::Contains("⬅"));
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

TEST_CASE( "many noise generation purity", "[many]" ) {
    SECTION("noise(seed) must generate the same output when called repeatedly"){
        CHECK(noise(0.1f)==noise(0.1f));
    }
    SECTION("noise(seeds) must generate the same output when called repeatedly"){
        CHECK(noise(vec3(0.1f))==noise(vec3(0.1f)));
    }
}
TEST_CASE( "many noise generation nontriviality", "[many]" ) {
    SECTION("noise(seed) must generate nontrivial output"){
        CHECK(noise(0.1f) > 0.f);
    }
    SECTION("noise(seeds) must generate nontrivial output"){
        CHECK(dot(noise(vec3(1.f, 2.f, 3.f)), vec3(1.f)) > 0.f);
    }
}

TEST_CASE( "many get_elias_noise generation determinism", "[many]" ) {
    floats a = floats({1,2,3,4,5,6});
    floats b = floats({1,1,2,3,5,8});
    vec3s positions = many::vec3s({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_elias_noise(positions, generator) must generate the same many when given the same state of generator"){
        std::stringstream ss;
        std::mt19937 generator(2);
        ss << generator;
        get_elias_noise(positions, generator, a);
        ss >> generator;
        get_elias_noise(positions, generator, b);
        CHECK(a==b);
    }
}
TEST_CASE( "many get_elias_noise generation nonpurity", "[many]" ) {
    floats a = floats({1,2,3,4,5,6});
    floats b = floats({1,2,3,4,5,6});
    vec3s positions = many::vec3s({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_elias_noise(positions, generator) must generate different output when called repeatedly"){
        std::mt19937 generator(2);
        get_elias_noise(positions, generator, a);
        get_elias_noise(positions, generator, b);
        CHECK(a!=b);
    }
}
TEST_CASE( "many get_elias_noise generation nontriviality", "[many]" ) {
    floats a = floats({1,2,3,4,5,6});
    vec3s positions = many::vec3s({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_elias_noise(positions, generator) must generate nontrivial output"){
        std::mt19937 generator(2);
        get_elias_noise(positions, generator, a);
        CHECK(sum(a) > 0.f);
    }
}

TEST_CASE( "many get_perlin_noise generation purity", "[many]" ) {
    floats a = floats({1,2,3,4,5,6});
    floats b = floats({1,2,3,4,5,6});
    vec3s positions = many::vec3s({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_perlin_noise(positions) must generate different output when called repeatedly"){
        get_perlin_noise(positions, a);
        get_perlin_noise(positions, b);
        CHECK(a==b);
    }
}
TEST_CASE( "many get_perlin_noise generation nontriviality", "[many]" ) {
    floats a = floats({1,2,3,4,5,6});
    vec3s positions = many::vec3s({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_perlin_noise(positions) must generate nontrivial output"){
        get_perlin_noise(positions, a);
        CHECK(sum(a) > 0.f);
    }
}

TEST_CASE( "many get_worley_noise generation purity", "[many]" ) {
    floats a = floats({1,2,3,4,5,6});
    floats b = floats({1,2,3,4,5,6});
    vec3s positions = many::vec3s({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_worley_noise(positions) must generate different output when called repeatedly"){
        get_worley_noise(positions, a);
        get_worley_noise(positions, b);
        CHECK(a==b);
    }
}
TEST_CASE( "many get_worley_noise generation nontriviality", "[many]" ) {
    floats a = floats({1,2,3,4,5,6});
    vec3s positions = many::vec3s({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_worley_noise(positions) must generate nontrivial output"){
        get_worley_noise(positions, a);
        CHECK(sum(a) > 0.f);
    }
}