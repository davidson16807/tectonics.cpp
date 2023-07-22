
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Series.hpp"
#include "each.hpp"
#include "each.hpp"
#include "whole.hpp"

TEST_CASE( "Series<T> arithmetic purity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto b = data::series({-1,1,-2,2,3});
    auto c1 = data::series({0,0,0,0,0});
    auto c2 = data::series({0,0,0,0,0});

    SECTION("a+b must be called repeatedly without changing the output"){
        data::add(a,b,c1);
        data::add(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a*b must be called repeatedly without changing the output"){
        data::mult(a,b,c1);
        data::mult(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        data::sub(a,b,c1);
        data::sub(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a/b must be called repeatedly without changing the output"){
        data::div(a,b,c1);
        data::div(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }
}

TEST_CASE( "Series<T> arithmetic identity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto zeros = data::series({0,0,0,0,0});
    auto ones  = data::series({1,1,1,1,1});
    auto c = data::series({0,0,0,0,0});

    SECTION("a+I must equal a"){
        data::add(a,zeros, c);
        CHECK(whole::equal(c, a));
        data::sub(a,zeros, c);
        CHECK(whole::equal(c, a));
        data::mult(a,ones, c);
        CHECK(whole::equal(c, a));
        data::div(a,ones, c);
        CHECK(whole::equal(c, a));
        data::sub(a,a, c);
        CHECK(whole::equal(c, zeros));
        data::div(a,a, c);
        CHECK(whole::equal(c, ones ));
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
        CHECK(whole::equal(ab, ba));
    }
    SECTION("a*b must equal b*a"){
        data::mult(a, b, ab);
        data::mult(b, a, ba);
        CHECK(whole::equal(ab, ba));
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
        CHECK(whole::equal(ab_c, a_bc));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        data::mult(a,b,    ab_c);
        data::mult(ab_c,c, ab_c);
        data::mult(b,c,    a_bc);
        data::mult(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
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
        CHECK(whole::equal(ab_c, ac_bc));
    }
    SECTION("a+b/c must equal a/c+b/c"){
        data::add(a,b,    ab_c);
        data::div(ab_c,c, ab_c);
        data::div(a,c,    ac);
        data::div(b,c,    bc);
        data::add(ac,bc,  ac_bc);
        CHECK(whole::equal(ab_c, ac_bc));
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
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a*b must be called repeatedly without changing the output"){
        data::mult(a,b,c1);
        data::mult(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        data::sub(a,b,c1);
        data::sub(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a/b must be called repeatedly without changing the output"){
        data::div(a,b,c1);
        data::div(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }
}

TEST_CASE( "Series<T>/Uniform<T> arithmetic identity", "[many]" ) {
    auto a = data::series({1,2,3,4,5});
    auto zeros = data::uniform(0);
    auto ones  = data::uniform(1);
    auto c = data::series({0,0,0,0,0});

    SECTION("a+I must equal a"){
        data::add(a,zeros, c);
        CHECK(whole::equal(c, a));
        data::sub(a,zeros, c);
        CHECK(whole::equal(c, a));
        data::mult(a,ones, c);
        CHECK(whole::equal(c, a));
        data::div(a,ones, c);
        CHECK(whole::equal(c, a));
        data::sub(a,a, c);
        CHECK(whole::equal(c, zeros, 1e-7));
        data::div(a,a, c);
        CHECK(whole::equal(c, ones,  1e-7 ));
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
        CHECK(whole::equal(ab, ba));
    }
    SECTION("a*b must equal b*a"){
        data::mult(a, b, ab);
        data::mult(b, a, ba);
        CHECK(whole::equal(ab, ba));
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
        CHECK(whole::equal(ab_c, a_bc));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        data::mult(a,b,    ab_c);
        data::mult(ab_c,c, ab_c);
        data::mult(b,c,    a_bc);
        data::mult(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
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
        CHECK(whole::equal(ab_c, ac_bc));
    }
    SECTION("a+b/c must equal a/c+b/c"){
        data::add(a,b,    ab_c);
        data::div(ab_c,c, ab_c);
        data::div(a,c,    ac);
        data::div(b,c,    bc);
        data::add(ac,bc,  ac_bc);
        CHECK(whole::equal(ab_c, ac_bc));
    }
}







TEST_CASE( "Series<T> sqrt purity", "[data]" ) {
    auto a  = data::series({1.0,2.0,3.0,4.0,5.0});
    auto c1 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto c2 = data::series({0.0,0.0,0.0,0.0,0.0});
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        data::sqrt(a,c1);
        data::sqrt(a,c2);
        CHECK(whole::equal(c1,c2, 1e-7));
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
        CHECK(whole::equal(sqrt1 , sqrt2, 1e-7));

        data::sqrt(b,    sqrt1);
        data::pow (b,data::uniform(0.5f),sqrt2);
        CHECK(whole::equal(sqrt1 , sqrt2, 1e-7));
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
        CHECK(whole::equal(log1_ , log2_, 1e-7));
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
        CHECK(whole::equal(elalb, ab, 1e-7));
    }
}
TEST_CASE( "Series<T> log/exp invertibility", "[data]" ) {
    auto a   = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b   = data::series({1.0,1.0,2.0,3.0,5.0});
    auto out = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("log(exp(a)) must equal a"){
        data::log (a,   out);
        data::exp (out, out);
        CHECK(whole::equal(a, out, 1e-7));
    }
    SECTION("log2(exp2(a)) must equal a"){
        data::log2(a,   out);
        data::exp2(out, out);
        CHECK(whole::equal(a, out, 1e-7));
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
        CHECK(whole::equal(out, ab, 1e-7));
    }
}
TEST_CASE( "Series<T> log2/exp2 invertibility", "[data]" ) {
    auto a   = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b   = data::series({1.0,1.0,2.0,3.0,5.0});
    auto out = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("log2(exp2(a)) must equal a"){
        data::log2 (a,   out);
        data::exp2 (out, out);
        CHECK(whole::equal(a, out, 1e-7));
    }
    SECTION("log2(exp2(a)) must equal a"){
        data::log2(a,   out);
        data::exp2(out, out);
        CHECK(whole::equal(a, out, 1e-7));
    }
}






TEST_CASE( "Series<T> morphologic purity", "[many]" ) {
    auto a = data::series({1,0,0,1,0});
    auto b = data::series({0,1,0,1,0});
    auto c1 = data::series({0,0,0,0,0});
    auto c2 = data::series({0,0,0,0,0});

    SECTION("a∩b must be called repeatedly without changing the output"){
        data::intersect(a,b,c1);
        data::intersect(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a∪b must be called repeatedly without changing the output"){
        data::unite(a,b,c1);
        data::unite(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        data::differ(a,b,c1);
        data::differ(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("-b must be called repeatedly without changing the output"){
        data::negate(a,c1);
        data::negate(a,c2);
        CHECK(whole::equal(c1, c2));
    }
}

TEST_CASE( "Series<T> morphologic identity", "[many]" ) {
    auto a = data::series({1,0,0,1,0});
    auto zeros = data::series({0,0,0,0,0});
    auto ones  = data::series({1,1,1,1,1});
    auto c = data::series({0,0,0,0,0});

    SECTION("a∪∅ must equal a"){
        data::unite(a,zeros, c);
        CHECK(whole::equal(c, a));
    }
    SECTION("a∩1 must equal a"){
        data::intersect(a,ones, c);
        CHECK(whole::equal(c, a));
    }
    SECTION("a-∅ must equal a"){
        data::differ(a,zeros, c);
        CHECK(whole::equal(c, a));
    }
}

TEST_CASE( "Series<T> morphologic commutativity", "[many]" ) {
    auto a = data::series({1,0,0,1,0});
    auto b = data::series({0,1,0,1,0});
    auto ab = data::series({0,0,0,0,0});
    auto ba = data::series({0,0,0,0,0});

    SECTION("a∪b must equal b∪a"){
        data::unite(a, b, ab);
        data::unite(b, a, ba);
        CHECK(whole::equal(ab, ba));
    }

    SECTION("a∩b must equal b∩a"){
        data::intersect(a, b, ab);
        data::intersect(b, a, ba);
        CHECK(whole::equal(ab, ba));
    }

}

TEST_CASE( "Series<T> negation involutivity", "[many]" ) {
    auto a = data::series({1,0,0,1,0});
    auto c = data::series({1,1,2,3,5});

    SECTION("--a must equal a"){
        data::negate(a, c);
        data::negate(c, c);
        CHECK(whole::equal(c, a));
    }

}

TEST_CASE( "Series<T> morphologic associativity", "[many]" ) {

    auto a = data::series({1,0,0,1,0});
    auto b = data::series({0,1,0,1,0});
    auto c = data::series({1,1,2,3,5});
    auto ab_c = data::series({0,0,0,0,0});
    auto a_bc = data::series({0,0,0,0,0});

    SECTION("(a∪b)∪c must equal a∪(b∪c)"){
        data::unite(a,b,    ab_c);
        data::unite(ab_c,c, ab_c);
        data::unite(b,c,    a_bc);
        data::unite(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
    }
    SECTION("(a∩b)∩c must equal a∩(b∩c)"){
        data::intersect(a,b,    ab_c);
        data::intersect(ab_c,c, ab_c);
        data::intersect(b,c,    a_bc);
        data::intersect(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
    }
}

TEST_CASE( "Series<T> morphologic distributivity", "[many]" ) {
    auto a = data::series({1,0,0,1,0});
    auto b = data::series({0,1,0,1,0});
    auto c = data::series({1.0,1.0,2.0,3.0,5.0});
    auto ab_c  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ac_bc = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ac = data::series({0.0,0.0,0.0,0.0,0.0});
    auto bc = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("a+b*c must equal a*c+b*c"){
        data::unite(a,b,    ab_c);
        data::intersect(ab_c,c, ab_c);
        data::intersect(a,c,    ac);
        data::intersect(b,c,    bc);
        data::unite(ac,bc,  ac_bc);
        CHECK(whole::equal(ab_c, ac_bc));
    }
}


TEST_CASE( "Series<T> trigonometric purity", "[many]" ) {
    auto a = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b = data::series({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto c2 = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("sin must be called repeatedly without changing the output"){
        data::sin(a,c1);
        data::sin(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::sin(b,c1);
        data::sin(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("cos must be called repeatedly without changing the output"){
        data::cos(a,c1);
        data::cos(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::cos(b,c1);
        data::cos(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("tan must be called repeatedly without changing the output"){
        data::tan(a,c1);
        data::tan(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::tan(b,c1);
        data::tan(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

}

TEST_CASE( "Series<T> trigonometric cofunctions", "[many]" ) {
    const double pi = 3.14159265358979;
    auto a = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b = data::series({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto c2 = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("sin must equal cos for input that is rotated by π/2"){
        data::sub(data::uniform(pi/2.0), a, c1);
        data::cos(c1,c1);
        data::sin(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::sub(data::uniform(pi/2.0), b, c1);
        data::cos(c1,c1);
        data::sin(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }
    SECTION("tan must equal cot for input that is rotated by π/2"){
        data::sub(data::uniform(pi/2.0), a, c1);
        data::cot(c1,c1);
        data::tan(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::sub(data::uniform(pi/2.0), b, c1);
        data::cot(c1,c1);
        data::tan(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }
}



TEST_CASE( "Series<T> trigonometric negative angle identities", "[many]" ) {
    auto a = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b = data::series({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto c2 = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("negated sin must equal sin for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::sin(c1,c1);
        data::sin(a,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::sin(c1,c1);
        data::sin(b,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("negated cos must equal cos for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::cos(c1,c1);
        data::cos(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::cos(c1,c1);
        data::cos(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("negated tan must equal tan for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::tan(c1,c1);
        data::tan(a,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::tan(c1,c1);
        data::tan(b,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("negated csc must equal csc for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::csc(c1,c1);
        data::csc(a,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::csc(c1,c1);
        data::csc(b,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("negated sec must equal sec for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::sec(c1,c1);
        data::sec(a,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::sec(c1,c1);
        data::sec(b,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }

    SECTION("negated cot must equal cot for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::cot(c1,c1);
        data::cot(a,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::cot(c1,c1);
        data::cot(b,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(whole::equal(c1, c2, 1e-7));
    }
}

