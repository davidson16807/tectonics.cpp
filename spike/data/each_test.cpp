
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Series.hpp"
#include "each.hpp"
#include "relational.hpp"
#include "whole.hpp"

TEST_CASE( "Series<T> arithmetic purity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto b = data::series({-1,1,-2,2,3});
    auto c1 = data::series({0,0,0,0,0});
    auto c2 = data::series({0,0,0,0,0});

    SECTION("a+b must be called repeatedly without changing the output"){
        data::add(a,b,c1);
        data::add(a,b,c2);
        CHECK(data::equal(c1, c2));
    }

    SECTION("a*b must be called repeatedly without changing the output"){
        data::mult(a,b,c1);
        data::mult(a,b,c2);
        CHECK(data::equal(c1, c2));
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        data::sub(a,b,c1);
        data::sub(a,b,c2);
        CHECK(data::equal(c1, c2));
    }

    SECTION("a/b must be called repeatedly without changing the output"){
        data::div(a,b,c1);
        data::div(a,b,c2);
        CHECK(data::equal(c1, c2));
    }
}

TEST_CASE( "Series<T> arithmetic identity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto zeros = data::series({0,0,0,0,0});
    auto ones  = data::series({1,1,1,1,1});
    auto c = data::series({0,0,0,0,0});

    SECTION("a+I must equal a"){
        data::add(a,zeros, c);
        CHECK(data::equal(c, a));
        data::sub(a,zeros, c);
        CHECK(data::equal(c, a));
        data::mult(a,ones, c);
        CHECK(data::equal(c, a));
        data::div(a,ones, c);
        CHECK(data::equal(c, a));
        data::sub(a,a, c);
        CHECK(data::equal(c, zeros));
        data::div(a,a, c);
        CHECK(data::equal(c, ones ));
    }
}

TEST_CASE( "Series<T> arithmetic commutativity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto b = data::series({-1,1,-2,2,3});
    auto ab = data::series({0,0,0,0,0});
    auto ba = data::series({0,0,0,0,0});

    SECTION("a+b must equal b+a"){
        data::add(a, b, ab);
        data::add(b, a, ba);
        CHECK(data::equal(ab, ba));
    }
    SECTION("a*b must equal b*a"){
        data::mult(a, b, ab);
        data::mult(b, a, ba);
        CHECK(data::equal(ab, ba));
    }
}

TEST_CASE( "Series<T> arithmetic associativity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto b = data::series({-1,1,-2,2,3});
    auto c = data::series({1,1,2,3,5});
    auto ab_c = data::series({0,0,0,0,0});
    auto a_bc = data::series({0,0,0,0,0});

    SECTION("(a+b)+c must equal a+(b+c)"){
        data::add(a,b,    ab_c);
        data::add(ab_c,c, ab_c);
        data::add(b,c,    a_bc);
        data::add(a,a_bc, a_bc);
        CHECK(data::equal(ab_c, a_bc));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        data::mult(a,b,    ab_c);
        data::mult(ab_c,c, ab_c);
        data::mult(b,c,    a_bc);
        data::mult(a,a_bc, a_bc);
        CHECK(data::equal(ab_c, a_bc));
    }
}

TEST_CASE( "Series<T> arithmetic distributivity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto b = data::series({-1,1,-2,2,3});
    auto c = data::series({1.0,1.0,2.0,3.0,5.0});
    auto ab_c  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ac_bc = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ac = data::series({0.0,0.0,0.0,0.0,0.0});
    auto bc = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("a+b*c must equal a*c+b*c"){
        data::add(a,b,    ab_c);
        data::mult(ab_c,c, ab_c);
        data::mult(a,c,    ac);
        data::mult(b,c,    bc);
        data::add(ac,bc,  ac_bc);
        CHECK(data::equal(ab_c, ac_bc));
    }
    SECTION("a+b/c must equal a/c+b/c"){
        data::add(a,b,    ab_c);
        data::div(ab_c,c, ab_c);
        data::div(a,c,    ac);
        data::div(b,c,    bc);
        data::add(ac,bc,  ac_bc);
        CHECK(data::equal(ab_c, ac_bc));
    }
}










TEST_CASE( "Series<T>/Uniform<T> arithmetic purity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto b = data::uniform(-2);
    auto c1 = data::series({0,0,0,0,0});
    auto c2 = data::series({0,0,0,0,0});

    SECTION("a+b must be called repeatedly without changing the output"){
        data::add(a,b,c1);
        data::add(a,b,c2);
        CHECK(data::equal(c1, c2));
    }

    SECTION("a*b must be called repeatedly without changing the output"){
        data::mult(a,b,c1);
        data::mult(a,b,c2);
        CHECK(data::equal(c1, c2));
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        data::sub(a,b,c1);
        data::sub(a,b,c2);
        CHECK(data::equal(c1, c2));
    }

    SECTION("a/b must be called repeatedly without changing the output"){
        data::div(a,b,c1);
        data::div(a,b,c2);
        CHECK(data::equal(c1, c2));
    }
}

TEST_CASE( "Series<T>/Uniform<T> arithmetic identity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto zeros = data::uniform(0);
    auto ones  = data::uniform(1);
    auto c = data::series({0,0,0,0,0});

    SECTION("a+I must equal a"){
        data::add(a,zeros, c);
        CHECK(data::equal(c, a));
        data::sub(a,zeros, c);
        CHECK(data::equal(c, a));
        data::mult(a,ones, c);
        CHECK(data::equal(c, a));
        data::div(a,ones, c);
        CHECK(data::equal(c, a));
        data::sub(a,a, c);
        CHECK(data::equal(c, zeros, 1e-7));
        data::div(a,a, c);
        CHECK(data::equal(c, ones,  1e-7 ));
    }
}

TEST_CASE( "Series<T>/Uniform<T> arithmetic commutativity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto b = data::uniform(-2);
    auto ab = data::series({0,0,0,0,0});
    auto ba = data::series({0,0,0,0,0});

    SECTION("a+b must equal b+a"){
        data::add(a, b, ab);
        data::add(b, a, ba);
        CHECK(data::equal(ab, ba));
    }
    SECTION("a*b must equal b*a"){
        data::mult(a, b, ab);
        data::mult(b, a, ba);
        CHECK(data::equal(ab, ba));
    }
}

TEST_CASE( "Series<T>/Uniform<T> arithmetic associativity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto b = data::uniform(-2);
    auto c = data::series({1,1,2,3,5});
    auto ab_c = data::series({0,0,0,0,0});
    auto a_bc = data::series({0,0,0,0,0});

    SECTION("(a+b)+c must equal a+(b+c)"){
        data::add(a,b,    ab_c);
        data::add(ab_c,c, ab_c);
        data::add(b,c,    a_bc);
        data::add(a,a_bc, a_bc);
        CHECK(data::equal(ab_c, a_bc));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        data::mult(a,b,    ab_c);
        data::mult(ab_c,c, ab_c);
        data::mult(b,c,    a_bc);
        data::mult(a,a_bc, a_bc);
        CHECK(data::equal(ab_c, a_bc));
    }
}

TEST_CASE( "Series<T>/Uniform<T> arithmetic distributivity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto b = data::uniform(-2);
    auto c = data::series({1.0,1.0,2.0,3.0,5.0});
    auto ab_c  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ac_bc = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ac = data::series({0.0,0.0,0.0,0.0,0.0});
    auto bc = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("a+b*c must equal a*c+b*c"){
        data::add(a,b,    ab_c);
        data::mult(ab_c,c, ab_c);
        data::mult(a,c,    ac);
        data::mult(b,c,    bc);
        data::add(ac,bc,  ac_bc);
        CHECK(data::equal(ab_c, ac_bc));
    }
    SECTION("a+b/c must equal a/c+b/c"){
        data::add(a,b,    ab_c);
        data::div(ab_c,c, ab_c);
        data::div(a,c,    ac);
        data::div(b,c,    bc);
        data::add(ac,bc,  ac_bc);
        CHECK(data::equal(ab_c, ac_bc));
    }
}







TEST_CASE( "Series<T> sqrt purity", "[data]" ) {
    auto a  = data::series({1.0,2.0,3.0,4.0,5.0});
    auto c1 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto c2 = data::series({0.0,0.0,0.0,0.0,0.0});
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        data::sqrt(a,c1);
        data::sqrt(a,c2);
        CHECK(data::equal(c1,c2, 1e-7));
    }
}
TEST_CASE( "Series<T> sqrt consistency", "[data]" ) {
    auto a     = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b     = data::series({1.0,1.0,2.0,3.0,5.0});
    auto sqrt1 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto sqrt2 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto log1_ = data::series({0.0,0.0,0.0,0.0,0.0});
    auto log2_ = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("sqrt(a) must equal pow(a,1/2)"){
        data::sqrt(a,    sqrt1);
        data::pow (a,data::uniform(0.5f),sqrt2);
        CHECK(data::equal(sqrt1 , sqrt2, 1e-7));

        data::sqrt(b,    sqrt1);
        data::pow (b,data::uniform(0.5f),sqrt2);
        CHECK(data::equal(sqrt1 , sqrt2, 1e-7));
    }

}
TEST_CASE( "Series<T> log consistency", "[data]" ) {
    auto a     = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b     = data::series({1.0,1.0,2.0,3.0,5.0});
    auto log1_ = data::series({0.0,0.0,0.0,0.0,0.0});
    auto log2_ = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("log2(a) must equal log(a)/log(2)"){
        data::log2(a, log1_);
        data::log (a, log2_);
        data::div (log2_, data::uniform(std::log(2.f)), log2_);
        CHECK(data::equal(log1_ , log2_, 1e-7));
    }
}
TEST_CASE( "Series<T> log/exp consistency", "[data]" ) {
    auto a     = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b     = data::series({1.0,1.0,2.0,3.0,5.0});
    auto loga  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto logb  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto lalb  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto elalb = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ab    = data::series({0.0,0.0,0.0,0.0,0.0});
    auto out   = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("exp(log(a)+log(b)) must equal a*b"){
        data::log(a, loga);
        data::log(b, logb);
        data::add(loga, logb, lalb);
        data::exp(lalb, elalb);
        data::mult(a, b, ab);
        CHECK(data::equal(elalb, ab, 1e-7));
    }
}
TEST_CASE( "Series<T> log/exp invertibility", "[data]" ) {
    auto a   = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b   = data::series({1.0,1.0,2.0,3.0,5.0});
    auto out = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("log(exp(a)) must equal a"){
        data::log (a,   out);
        data::exp (out, out);
        CHECK(data::equal(a, out, 1e-7));
    }
    SECTION("log2(exp2(a)) must equal a"){
        data::log2(a,   out);
        data::exp2(out, out);
        CHECK(data::equal(a, out, 1e-7));
    }
}

TEST_CASE( "Series<T> log2/exp2 consistency", "[data]" ) {
    auto a     = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b     = data::series({1.0,1.0,2.0,3.0,5.0});
    auto loga  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto logb  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto lalb  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ab    = data::series({0.0,0.0,0.0,0.0,0.0});
    auto out   = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("exp2(log2(a)+log2(b)) must equal a*b"){
        data::log2(a, loga);
        data::log2(b, logb);
        data::add(loga, logb, lalb);
        data::exp2(lalb, out);
        data::mult(a, b, ab);
        CHECK(data::equal(out, ab, 1e-7));
    }
}
TEST_CASE( "Series<T> log2/exp2 invertibility", "[data]" ) {
    auto a   = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b   = data::series({1.0,1.0,2.0,3.0,5.0});
    auto out = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("log2(exp2(a)) must equal a"){
        data::log2 (a,   out);
        data::exp2 (out, out);
        CHECK(data::equal(a, out, 1e-7));
    }
    SECTION("log2(exp2(a)) must equal a"){
        data::log2(a,   out);
        data::exp2(out, out);
        CHECK(data::equal(a, out, 1e-7));
    }
}

