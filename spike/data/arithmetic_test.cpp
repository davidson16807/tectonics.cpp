
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Store.hpp"

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
        CHECK(data::equal(c, zeros));
        data::div(a,a, c);
        CHECK(data::equal(c, ones ));
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


