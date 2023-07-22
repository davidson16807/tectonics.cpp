
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// HACK: order of tests is arbitrary and glm each must be loaded first to permit template specialization
// so we include it here despite not necessarily needing it.
#include "glm/each.hpp"

#include "series/Interleave.hpp"
#include "each.hpp"
#include "whole.hpp"

TEST_CASE( "Series<T> arithmetic purity", "[each]" ) {
    auto a = series::interleave({1,2,3,4,5});
    auto b = series::interleave({-1,1,-2,2,3});
    auto c1 = series::interleave({0,0,0,0,0});
    auto c2 = series::interleave({0,0,0,0,0});

    SECTION("a+b must be called repeatedly without changing the output"){
        each::add(a,b,c1);
        each::add(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a*b must be called repeatedly without changing the output"){
        each::mult(a,b,c1);
        each::mult(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        each::sub(a,b,c1);
        each::sub(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a/b must be called repeatedly without changing the output"){
        each::div(a,b,c1);
        each::div(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }
}

TEST_CASE( "Series<T> arithmetic identity", "[each]" ) {
    auto a = series::interleave({1,2,3,4,5});
    auto zeros = series::interleave({0,0,0,0,0});
    auto ones  = series::interleave({1,1,1,1,1});
    auto c = series::interleave({0,0,0,0,0});

    SECTION("a+I must equal a"){
        each::add(a,zeros, c);
        CHECK(whole::equal(c, a));
        each::sub(a,zeros, c);
        CHECK(whole::equal(c, a));
        each::mult(a,ones, c);
        CHECK(whole::equal(c, a));
        each::div(a,ones, c);
        CHECK(whole::equal(c, a));
        each::sub(a,a, c);
        CHECK(whole::equal(c, zeros));
        each::div(a,a, c);
        CHECK(whole::equal(c, ones ));
    }
}

TEST_CASE( "Series<T> arithmetic commutativity", "[each]" ) {
    auto a = series::interleave({1,2,3,4,5});
    auto b = series::interleave({-1,1,-2,2,3});
    auto ab = series::interleave({0,0,0,0,0});
    auto ba = series::interleave({0,0,0,0,0});

    SECTION("a+b must equal b+a"){
        each::add(a, b, ab);
        each::add(b, a, ba);
        CHECK(whole::equal(ab, ba));
    }
    SECTION("a*b must equal b*a"){
        each::mult(a, b, ab);
        each::mult(b, a, ba);
        CHECK(whole::equal(ab, ba));
    }
}

TEST_CASE( "Series<T> arithmetic associativity", "[each]" ) {
    auto a = series::interleave({1,2,3,4,5});
    auto b = series::interleave({-1,1,-2,2,3});
    auto c = series::interleave({1,1,2,3,5});
    auto ab_c = series::interleave({0,0,0,0,0});
    auto a_bc = series::interleave({0,0,0,0,0});

    SECTION("(a+b)+c must equal a+(b+c)"){
        each::add(a,b,    ab_c);
        each::add(ab_c,c, ab_c);
        each::add(b,c,    a_bc);
        each::add(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        each::mult(a,b,    ab_c);
        each::mult(ab_c,c, ab_c);
        each::mult(b,c,    a_bc);
        each::mult(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
    }
}

TEST_CASE( "Series<T> arithmetic distributivity", "[each]" ) {
    auto a = series::interleave({1,2,3,4,5});
    auto b = series::interleave({-1,1,-2,2,3});
    auto c = series::interleave({1.0,1.0,2.0,3.0,5.0});
    auto ab_c  = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto ac_bc = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto ac = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto bc = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("a+b*c must equal a*c+b*c"){
        each::add(a,b,    ab_c);
        each::mult(ab_c,c, ab_c);
        each::mult(a,c,    ac);
        each::mult(b,c,    bc);
        each::add(ac,bc,  ac_bc);
        CHECK(whole::equal(ab_c, ac_bc));
    }
    SECTION("a+b/c must equal a/c+b/c"){
        each::add(a,b,    ab_c);
        each::div(ab_c,c, ab_c);
        each::div(a,c,    ac);
        each::div(b,c,    bc);
        each::add(ac,bc,  ac_bc);
        CHECK(whole::equal(ab_c, ac_bc));
    }
}










TEST_CASE( "Series<T>/Uniform<T> arithmetic purity", "[each]" ) {
    auto a = series::interleave({1,2,3,4,5});
    auto b = each::uniform(-2);
    auto c1 = series::interleave({0,0,0,0,0});
    auto c2 = series::interleave({0,0,0,0,0});

    SECTION("a+b must be called repeatedly without changing the output"){
        each::add(a,b,c1);
        each::add(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a*b must be called repeatedly without changing the output"){
        each::mult(a,b,c1);
        each::mult(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        each::sub(a,b,c1);
        each::sub(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a/b must be called repeatedly without changing the output"){
        each::div(a,b,c1);
        each::div(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }
}

TEST_CASE( "Series<T>/Uniform<T> arithmetic identity", "[each]" ) {
    auto a = series::interleave({1,2,3,4,5});
    auto zeros = each::uniform(0);
    auto ones  = each::uniform(1);
    auto c = series::interleave({0,0,0,0,0});

    SECTION("a+I must equal a"){
        each::add(a,zeros, c);
        CHECK(whole::equal(c, a));
        each::sub(a,zeros, c);
        CHECK(whole::equal(c, a));
        each::mult(a,ones, c);
        CHECK(whole::equal(c, a));
        each::div(a,ones, c);
        CHECK(whole::equal(c, a));
        each::sub(a,a, c);
        CHECK(whole::equal(c, zeros, 1e-7));
        each::div(a,a, c);
        CHECK(whole::equal(c, ones,  1e-7 ));
    }
}

TEST_CASE( "Series<T>/Uniform<T> arithmetic commutativity", "[each]" ) {
    auto a = series::interleave({1,2,3,4,5});
    auto b = each::uniform(-2);
    auto ab = series::interleave({0,0,0,0,0});
    auto ba = series::interleave({0,0,0,0,0});

    SECTION("a+b must equal b+a"){
        each::add(a, b, ab);
        each::add(b, a, ba);
        CHECK(whole::equal(ab, ba));
    }
    SECTION("a*b must equal b*a"){
        each::mult(a, b, ab);
        each::mult(b, a, ba);
        CHECK(whole::equal(ab, ba));
    }
}

TEST_CASE( "Series<T>/Uniform<T> arithmetic associativity", "[each]" ) {
    auto a = series::interleave({1,2,3,4,5});
    auto b = each::uniform(-2);
    auto c = series::interleave({1,1,2,3,5});
    auto ab_c = series::interleave({0,0,0,0,0});
    auto a_bc = series::interleave({0,0,0,0,0});

    SECTION("(a+b)+c must equal a+(b+c)"){
        each::add(a,b,    ab_c);
        each::add(ab_c,c, ab_c);
        each::add(b,c,    a_bc);
        each::add(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        each::mult(a,b,    ab_c);
        each::mult(ab_c,c, ab_c);
        each::mult(b,c,    a_bc);
        each::mult(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
    }
}

TEST_CASE( "Series<T>/Uniform<T> arithmetic distributivity", "[each]" ) {
    auto a = series::interleave({1,2,3,4,5});
    auto b = each::uniform(-2);
    auto c = series::interleave({1.0,1.0,2.0,3.0,5.0});
    auto ab_c  = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto ac_bc = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto ac = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto bc = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("a+b*c must equal a*c+b*c"){
        each::add(a,b,    ab_c);
        each::mult(ab_c,c, ab_c);
        each::mult(a,c,    ac);
        each::mult(b,c,    bc);
        each::add(ac,bc,  ac_bc);
        CHECK(whole::equal(ab_c, ac_bc));
    }
    SECTION("a+b/c must equal a/c+b/c"){
        each::add(a,b,    ab_c);
        each::div(ab_c,c, ab_c);
        each::div(a,c,    ac);
        each::div(b,c,    bc);
        each::add(ac,bc,  ac_bc);
        CHECK(whole::equal(ab_c, ac_bc));
    }
}







TEST_CASE( "Series<T> sqrt purity", "[each]" ) {
    auto a  = series::interleave({1.0,2.0,3.0,4.0,5.0});
    auto c1 = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto c2 = series::interleave({0.0,0.0,0.0,0.0,0.0});
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        each::sqrt(a,c1);
        each::sqrt(a,c2);
        CHECK(whole::equal(c1,c2, 1e-7));
    }
}
TEST_CASE( "Series<T> sqrt consistency", "[each]" ) {
    auto a     = series::interleave({1.0,2.0,3.0,4.0,5.0});
    auto b     = series::interleave({1.0,1.0,2.0,3.0,5.0});
    auto sqrt1 = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto sqrt2 = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto log1_ = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto log2_ = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("sqrt(a) must equal pow(a,1/2)"){
        each::sqrt(a,    sqrt1);
        each::pow (a,each::uniform(0.5f),sqrt2);
        CHECK(whole::equal(sqrt1 , sqrt2, 1e-7));

        each::sqrt(b,    sqrt1);
        each::pow (b,each::uniform(0.5f),sqrt2);
        CHECK(whole::equal(sqrt1 , sqrt2, 1e-7));
    }

}
TEST_CASE( "Series<T> log consistency", "[each]" ) {
    auto a     = series::interleave({1.0,2.0,3.0,4.0,5.0});
    auto b     = series::interleave({1.0,1.0,2.0,3.0,5.0});
    auto log1_ = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto log2_ = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("log2(a) must equal log(a)/log(2)"){
        each::log2(a, log1_);
        each::log (a, log2_);
        each::div (log2_, each::uniform(std::log(2.f)), log2_);
        CHECK(whole::equal(log1_ , log2_, 1e-7));
    }
}
TEST_CASE( "Series<T> log/exp consistency", "[each]" ) {
    auto a     = series::interleave({1.0,2.0,3.0,4.0,5.0});
    auto b     = series::interleave({1.0,1.0,2.0,3.0,5.0});
    auto loga  = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto logb  = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto lalb  = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto elalb = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto ab    = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto out   = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("exp(log(a)+log(b)) must equal a*b"){
        each::log(a, loga);
        each::log(b, logb);
        each::add(loga, logb, lalb);
        each::exp(lalb, elalb);
        each::mult(a, b, ab);
        CHECK(whole::equal(elalb, ab, 1e-7));
    }
}
TEST_CASE( "Series<T> log/exp invertibility", "[each]" ) {
    auto a   = series::interleave({1.0,2.0,3.0,4.0,5.0});
    auto b   = series::interleave({1.0,1.0,2.0,3.0,5.0});
    auto out = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("log(exp(a)) must equal a"){
        each::log (a,   out);
        each::exp (out, out);
        CHECK(whole::equal(a, out, 1e-7));
    }
    SECTION("log2(exp2(a)) must equal a"){
        each::log2(a,   out);
        each::exp2(out, out);
        CHECK(whole::equal(a, out, 1e-7));
    }
}

TEST_CASE( "Series<T> log2/exp2 consistency", "[each]" ) {
    auto a     = series::interleave({1.0,2.0,3.0,4.0,5.0});
    auto b     = series::interleave({1.0,1.0,2.0,3.0,5.0});
    auto loga  = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto logb  = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto lalb  = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto ab    = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto out   = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("exp2(log2(a)+log2(b)) must equal a*b"){
        each::log2(a, loga);
        each::log2(b, logb);
        each::add(loga, logb, lalb);
        each::exp2(lalb, out);
        each::mult(a, b, ab);
        CHECK(whole::equal(out, ab, 1e-7));
    }
}
TEST_CASE( "Series<T> log2/exp2 invertibility", "[each]" ) {
    auto a   = series::interleave({1.0,2.0,3.0,4.0,5.0});
    auto b   = series::interleave({1.0,1.0,2.0,3.0,5.0});
    auto out = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("log2(exp2(a)) must equal a"){
        each::log2 (a,   out);
        each::exp2 (out, out);
        CHECK(whole::equal(a, out, 1e-7));
    }
    SECTION("log2(exp2(a)) must equal a"){
        each::log2(a,   out);
        each::exp2(out, out);
        CHECK(whole::equal(a, out, 1e-7));
    }
}






TEST_CASE( "Series<T> morphologic purity", "[each]" ) {
    auto a = series::interleave({1,0,0,1,0});
    auto b = series::interleave({0,1,0,1,0});
    auto c1 = series::interleave({0,0,0,0,0});
    auto c2 = series::interleave({0,0,0,0,0});

    SECTION("a∩b must be called repeatedly without changing the output"){
        each::intersect(a,b,c1);
        each::intersect(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a∪b must be called repeatedly without changing the output"){
        each::unite(a,b,c1);
        each::unite(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        each::differ(a,b,c1);
        each::differ(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("-b must be called repeatedly without changing the output"){
        each::negate(a,c1);
        each::negate(a,c2);
        CHECK(whole::equal(c1, c2));
    }
}

TEST_CASE( "Series<T> morphologic identity", "[each]" ) {
    auto a = series::interleave({1,0,0,1,0});
    auto zeros = series::interleave({0,0,0,0,0});
    auto ones  = series::interleave({1,1,1,1,1});
    auto c = series::interleave({0,0,0,0,0});

    SECTION("a∪∅ must equal a"){
        each::unite(a,zeros, c);
        CHECK(whole::equal(c, a));
    }
    SECTION("a∩1 must equal a"){
        each::intersect(a,ones, c);
        CHECK(whole::equal(c, a));
    }
    SECTION("a-∅ must equal a"){
        each::differ(a,zeros, c);
        CHECK(whole::equal(c, a));
    }
}

TEST_CASE( "Series<T> morphologic commutativity", "[each]" ) {
    auto a = series::interleave({1,0,0,1,0});
    auto b = series::interleave({0,1,0,1,0});
    auto ab = series::interleave({0,0,0,0,0});
    auto ba = series::interleave({0,0,0,0,0});

    SECTION("a∪b must equal b∪a"){
        each::unite(a, b, ab);
        each::unite(b, a, ba);
        CHECK(whole::equal(ab, ba));
    }

    SECTION("a∩b must equal b∩a"){
        each::intersect(a, b, ab);
        each::intersect(b, a, ba);
        CHECK(whole::equal(ab, ba));
    }

}

TEST_CASE( "Series<T> negation involutivity", "[each]" ) {
    auto a = series::interleave({1,0,0,1,0});
    auto c = series::interleave({1,1,2,3,5});

    SECTION("--a must equal a"){
        each::negate(a, c);
        each::negate(c, c);
        CHECK(whole::equal(c, a));
    }

}

TEST_CASE( "Series<T> morphologic associativity", "[each]" ) {

    auto a = series::interleave({1,0,0,1,0});
    auto b = series::interleave({0,1,0,1,0});
    auto c = series::interleave({1,1,2,3,5});
    auto ab_c = series::interleave({0,0,0,0,0});
    auto a_bc = series::interleave({0,0,0,0,0});

    SECTION("(a∪b)∪c must equal a∪(b∪c)"){
        each::unite(a,b,    ab_c);
        each::unite(ab_c,c, ab_c);
        each::unite(b,c,    a_bc);
        each::unite(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
    }
    SECTION("(a∩b)∩c must equal a∩(b∩c)"){
        each::intersect(a,b,    ab_c);
        each::intersect(ab_c,c, ab_c);
        each::intersect(b,c,    a_bc);
        each::intersect(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
    }
}

TEST_CASE( "Series<T> morphologic distributivity", "[each]" ) {
    auto a = series::interleave({1,0,0,1,0});
    auto b = series::interleave({0,1,0,1,0});
    auto c = series::interleave({1.0,1.0,2.0,3.0,5.0});
    auto ab_c  = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto ac_bc = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto ac = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto bc = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("a+b*c must equal a*c+b*c"){
        each::unite(a,b,    ab_c);
        each::intersect(ab_c,c, ab_c);
        each::intersect(a,c,    ac);
        each::intersect(b,c,    bc);
        each::unite(ac,bc,  ac_bc);
        CHECK(whole::equal(ab_c, ac_bc));
    }
}


TEST_CASE( "Series<T> trigonometric purity", "[each]" ) {
    auto a = series::interleave({1.0,2.0,3.0,4.0,5.0});
    auto b = series::interleave({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto c2 = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("sin must be called repeatedly without changing the output"){
        each::sin(a,c1);
        each::sin(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::sin(b,c1);
        each::sin(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("cos must be called repeatedly without changing the output"){
        each::cos(a,c1);
        each::cos(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::cos(b,c1);
        each::cos(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("tan must be called repeatedly without changing the output"){
        each::tan(a,c1);
        each::tan(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::tan(b,c1);
        each::tan(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

}

TEST_CASE( "Series<T> trigonometric cofunctions", "[each]" ) {
    const double pi = 3.14159265358979;
    auto a = series::interleave({1.0,2.0,3.0,4.0,5.0});
    auto b = series::interleave({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto c2 = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("sin must equal cos for input that is rotated by π/2"){
        each::sub(each::uniform(pi/2.0), a, c1);
        each::cos(c1,c1);
        each::sin(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::sub(each::uniform(pi/2.0), b, c1);
        each::cos(c1,c1);
        each::sin(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }
    SECTION("tan must equal cot for input that is rotated by π/2"){
        each::sub(each::uniform(pi/2.0), a, c1);
        each::cot(c1,c1);
        each::tan(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::sub(each::uniform(pi/2.0), b, c1);
        each::cot(c1,c1);
        each::tan(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }
}



TEST_CASE( "Series<T> trigonometric negative angle identities", "[each]" ) {
    auto a = series::interleave({1.0,2.0,3.0,4.0,5.0});
    auto b = series::interleave({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = series::interleave({0.0,0.0,0.0,0.0,0.0});
    auto c2 = series::interleave({0.0,0.0,0.0,0.0,0.0});

    SECTION("negated sin must equal sin for negated input"){
        each::mult(each::uniform(-1.0), a, c1);
        each::sin(c1,c1);
        each::sin(a,c2);
        each::mult(each::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::mult(each::uniform(-1.0), b, c1);
        each::sin(c1,c1);
        each::sin(b,c2);
        each::mult(each::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("negated cos must equal cos for negated input"){
        each::mult(each::uniform(-1.0), a, c1);
        each::cos(c1,c1);
        each::cos(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::mult(each::uniform(-1.0), b, c1);
        each::cos(c1,c1);
        each::cos(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("negated tan must equal tan for negated input"){
        each::mult(each::uniform(-1.0), a, c1);
        each::tan(c1,c1);
        each::tan(a,c2);
        each::mult(each::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::mult(each::uniform(-1.0), b, c1);
        each::tan(c1,c1);
        each::tan(b,c2);
        each::mult(each::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("negated csc must equal csc for negated input"){
        each::mult(each::uniform(-1.0), a, c1);
        each::csc(c1,c1);
        each::csc(a,c2);
        each::mult(each::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::mult(each::uniform(-1.0), b, c1);
        each::csc(c1,c1);
        each::csc(b,c2);
        each::mult(each::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("negated sec must equal sec for negated input"){
        each::mult(each::uniform(-1.0), a, c1);
        each::sec(c1,c1);
        each::sec(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::mult(each::uniform(-1.0), b, c1);
        each::sec(c1,c1);
        each::sec(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("negated cot must equal cot for negated input"){
        each::mult(each::uniform(-1.0), a, c1);
        each::cot(c1,c1);
        each::cot(a,c2);
        each::mult(each::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        each::mult(each::uniform(-1.0), b, c1);
        each::cot(c1,c1);
        each::cot(b,c2);
        each::mult(each::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }
}

