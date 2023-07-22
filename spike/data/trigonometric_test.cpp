
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Series.hpp"
#include "each.hpp"
#include "trigonometric.hpp"
#include "relational.hpp"
#include "whole.hpp"

TEST_CASE( "Series<T> trigonometric purity", "[many]" ) {
    auto a = data::series({1.0,2.0,3.0,4.0,5.0});
    auto b = data::series({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = data::series({0.0,0.0,0.0,0.0,0.0});
    auto c2 = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("sin must be called repeatedly without changing the output"){
        data::sin(a,c1);
        data::sin(a,c2);
        CHECK(data::equal(c1, c2, 1e-7));
        data::sin(b,c1);
        data::sin(b,c2);
        CHECK(data::equal(c1, c2, 1e-7));
    }

    SECTION("cos must be called repeatedly without changing the output"){
        data::cos(a,c1);
        data::cos(a,c2);
        CHECK(data::equal(c1, c2, 1e-7));
        data::cos(b,c1);
        data::cos(b,c2);
        CHECK(data::equal(c1, c2, 1e-7));
    }

    SECTION("tan must be called repeatedly without changing the output"){
        data::tan(a,c1);
        data::tan(a,c2);
        CHECK(data::equal(c1, c2, 1e-7));
        data::tan(b,c1);
        data::tan(b,c2);
        CHECK(data::equal(c1, c2, 1e-7));
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
        CHECK(data::equal(c1, c2, 1e-7));
        data::sub(data::uniform(pi/2.0), b, c1);
        data::cos(c1,c1);
        data::sin(b,c2);
        CHECK(data::equal(c1, c2, 1e-7));
    }
    SECTION("tan must equal cot for input that is rotated by π/2"){
        data::sub(data::uniform(pi/2.0), a, c1);
        data::cot(c1,c1);
        data::tan(a,c2);
        CHECK(data::equal(c1, c2, 1e-7));
        data::sub(data::uniform(pi/2.0), b, c1);
        data::cot(c1,c1);
        data::tan(b,c2);
        CHECK(data::equal(c1, c2, 1e-7));
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
        CHECK(data::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::sin(c1,c1);
        data::sin(b,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(data::equal(c1, c2, 1e-7));
    }

    SECTION("negated cos must equal cos for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::cos(c1,c1);
        data::cos(a,c2);
        CHECK(data::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::cos(c1,c1);
        data::cos(b,c2);
        CHECK(data::equal(c1, c2, 1e-7));
    }

    SECTION("negated tan must equal tan for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::tan(c1,c1);
        data::tan(a,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(data::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::tan(c1,c1);
        data::tan(b,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(data::equal(c1, c2, 1e-7));
    }

    SECTION("negated csc must equal csc for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::csc(c1,c1);
        data::csc(a,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(data::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::csc(c1,c1);
        data::csc(b,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(data::equal(c1, c2, 1e-7));
    }

    SECTION("negated sec must equal sec for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::sec(c1,c1);
        data::sec(a,c2);
        CHECK(data::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::sec(c1,c1);
        data::sec(b,c2);
        CHECK(data::equal(c1, c2, 1e-7));
    }

    SECTION("negated cot must equal cot for negated input"){
        data::mult(data::uniform(-1.0), a, c1);
        data::cot(c1,c1);
        data::cot(a,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(data::equal(c1, c2, 1e-7));
        data::mult(data::uniform(-1.0), b, c1);
        data::cot(c1,c1);
        data::cot(b,c2);
        data::mult(data::uniform(-1.0),c2,c2);
        CHECK(data::equal(c1, c2, 1e-7));
    }
}

