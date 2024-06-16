#pragma once

// std libraries
#include <cmath>
#include <limits>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide p main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <index/known.hpp>
#include <index/series/Range.hpp>
#include <index/series/Uniform.hpp>
#include <index/series/noise/UnitIntervalNoise.hpp>

#include <index/adapted/ScalarMetric.hpp>
#include <index/adapted/ScalarStrings.hpp>
#include <index/adapted/SymbolicOrder.hpp>
#include <index/adapted/SymbolicArithmetic.hpp>

#include <index/aggregated/Metric.hpp>
#include <index/aggregated/Order.hpp>
#include <index/aggregated/Strings.hpp>

#include "Arithmetic.hpp"

#include <test/macros.hpp>
#include <test/structures/ringlike.hpp>

namespace iterated {

    template<typename T, typename Metric, typename Strings>
    struct Adapter{
        T threshold;
        Metric metric;
        Strings strings;

        Adapter(const T threshold, const Metric& metric, const Strings& strings):
            threshold(threshold),
            metric(metric),
            strings(strings)
        {}

        template<typename Series1, typename Series2>
        bool equal(const Series1& a, const Series2& b) const {
            return metric.distance(a,b) <= threshold;
        }

        template<typename Series>
        std::string print(const Series& a) const {
            return strings.to_string(a);
        }

    };

}

#define ITERATED_TEST_UNARY_OUT_PARAMETER(TYPE,SIZE,F)   [=](auto x){ std::vector<TYPE> out(SIZE); (F(x,out)); return out; }
#define ITERATED_TEST_BINARY_OUT_PARAMETER(TYPE,SIZE,F)  [=](auto x, auto y){ std::vector<TYPE> out(SIZE); (F(x,y,out)); return out; }
#define ITERATED_TEST_TRINARY_OUT_PARAMETER(TYPE,SIZE,F) [=](auto x, auto y, auto z){ std::vector<TYPE> out(SIZE); (F(x,y,z,out)); return out; }


TEST_CASE( "arithmetic on each nonzero of a series is a field", "[each]" ) {

    adapted::ScalarMetric submetric;    
    aggregated::Metric metric(submetric);
    adapted::ScalarStrings substrings;
    adapted::SymbolicOrder order;
    aggregated::Order ordered(order);
    aggregated::Strings strings(substrings, ordered);
    iterated::Adapter broad (1e-6, metric, strings);
    iterated::Adapter narrow(1e-6, metric, strings);

    auto subarithmetic = adapted::SymbolicArithmetic(0.0, 1.0);
    auto arithmetic = iterated::Arithmetic(subarithmetic);

    std::vector<series::UnitIntervalNoise<double>> noises {
        series::UnitIntervalNoise<double>(10.0, 1e4),
        series::UnitIntervalNoise<double>(20.0, 2e4),
        series::UnitIntervalNoise<double>(30.0, 3e4),
        series::UnitIntervalNoise<double>(40.0, 4e4)
    };

    std::vector<series::Range<double>> nonzero_ranges {
        series::Range<double>(1.0,101.0),
        series::Range<double>(-101.0,-1.0)
    };

    std::vector<series::Uniform<double>> nonzero_uniforms {
        series::Uniform<double>(-2.0),
        series::Uniform<double>(1.0),
        series::Uniform<double>(2.0),
    };

    test::Field field(
        "0", series::uniform(0.0), 
        "1", series::uniform(1.0), 
        "arithmetic.add",       ITERATED_TEST_BINARY_OUT_PARAMETER(double, 100, arithmetic.add),
        "arithmetic.subtract",  ITERATED_TEST_BINARY_OUT_PARAMETER(double, 100, arithmetic.subtract),
        "arithmetic.multiply",  ITERATED_TEST_BINARY_OUT_PARAMETER(double, 100, arithmetic.multiply),
        "arithmetic.divide",    ITERATED_TEST_BINARY_OUT_PARAMETER(double, 100, arithmetic.divide)
    );

    // UNARY TESTS
    // REQUIRE(field.valid(broad, noises));
    REQUIRE(field.valid(broad, nonzero_ranges));
    REQUIRE(field.valid(broad, nonzero_uniforms));

}


/*
TEST_CASE( "arithmetic on each nonzero of a series is a commutative ring", "[each]" ) {


    adapted::ScalarMetric submetric;
    aggregated::Metric metric(submetric);
    adapted::ScalarStrings substrings;
    adapted::SymbolicOrder order;
    aggregated::Strings strings(substrings);
    iterated::Adapter<double> broad (1e-6, metric, strings);
    iterated::Adapter<double> narrow(1e-6, metric, strings);

    std::vector<series::UnitIntervalNoise<double>> noises {
        series::UnitIntervalNoise<double>(10.0, 1e4),
        series::UnitIntervalNoise<double>(20.0, 2e4),
        series::UnitIntervalNoise<double>(30.0, 3e4),
        series::UnitIntervalNoise<double>(40.0, 4e4)
    };

    std::vector<series::Range<double>> ranges {
        series::Range<double>(),
        series::Range<double>(-50,50)
    };

    std::vector<series::Uniform<double>> uniforms {
        series::Uniform<double>(-2.0),
        series::Uniform<double>(0.0),
        series::Uniform<double>(1.0),
        series::Uniform<double>(2.0),
    };

    test::CommutativeRing ring(
        "0", series::uniform(0.0), 
        "1", series::uniform(1.0), 
        "each::add",  ITERATED_TEST_BINARY_OUT_PARAMETER(double, 100, each::add),
        "each::sub",  ITERATED_TEST_BINARY_OUT_PARAMETER(double, 100, each::sub),
        "each::mult", ITERATED_TEST_BINARY_OUT_PARAMETER(double, 100, each::mult)
    );

    // UNARY TESTS
    REQUIRE(ring.valid(broad, noises));
    REQUIRE(ring.valid(broad, ranges));
    REQUIRE(ring.valid(broad, uniforms));

    // BINARY TESTS
    REQUIRE(ring.valid(broad, noises, ranges));
    REQUIRE(ring.valid(broad, noises, uniforms));
    REQUIRE(ring.valid(broad, ranges, uniforms));

}








// TEST_CASE( "morphology on each of a series is a commutative monoid", "[each]" ) {

//     iterated::Adapter<bool> exact (0);

//     std::vector<std::vector<bool>> vectors {
//         std::vector<bool>({1,0,0,1,0}),
//         std::vector<bool>({0,1,0,1,0}),
//     };

//     test::CommutativeMonoid unite(
//         "0", series::uniform(0), 
//         "each::unite", ITERATED_TEST_BINARY_OUT_PARAMETER(bool, 5, each::unite)
//     );
//     REQUIRE(unite.valid(exact, vectors));

//     test::CommutativeMonoid intersect(
//         "1", series::uniform(1), 
//         "each::intersect", ITERATED_TEST_BINARY_OUT_PARAMETER(bool, 5, each::intersect)
//     );
//     REQUIRE(intersect.valid(exact, vectors));

// }


TEST_CASE( "Series<T> negation involutivity", "[each]" ) {
    auto a = std::vector({1,0,0,1,0});
    auto c = std::vector({1,1,2,3,5});

    SECTION("--a must equal a"){
        each::negate(a, c);
        each::negate(c, c);
        CHECK(metric.equal(c, a));
    }

}

TEST_CASE( "Series<T> morphologic distributivity", "[each]" ) {
    auto a = std::vector({1,0,0,1,0});
    auto b = std::vector({0,1,0,1,0});
    auto c = std::vector({1,1,0,0,1});
    auto ab_c  = std::vector({0,0,0,0,0});
    auto ac_bc = std::vector({0,0,0,0,0});
    auto ac = std::vector({0,0,0,0,0});
    auto bc = std::vector({0,0,0,0,0});

    SECTION("a+b*c must equal a*c+b*c"){
        each::unite(a,b,    ab_c);
        each::intersect(ab_c,c, ab_c);
        each::intersect(a,c,    ac);
        each::intersect(b,c,    bc);
        each::unite(ac,bc,  ac_bc);
        CHECK(metric.equal(ab_c, ac_bc));
    }
}










TEST_CASE( "Series<T> sqrt purity", "[each]" ) {
    auto a  = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto c1 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto c2 = std::vector({0.0,0.0,0.0,0.0,0.0});
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        each::sqrt(a,c1);
        each::sqrt(a,c2);
        CHECK(metric.equal(c1,c2, 1e-7));
    }
}
TEST_CASE( "Series<T> sqrt consistency", "[each]" ) {
    auto a     = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b     = std::vector({1.0,1.0,2.0,3.0,5.0});
    auto sqrt1 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto sqrt2 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto log1_ = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto log2_ = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("sqrt(a) must equal pow(a,1/2)"){
        each::sqrt(a,    sqrt1);
        each::pow (a,series::uniform(0.5f),sqrt2);
        CHECK(metric.equal(sqrt1 , sqrt2, 1e-7));

        each::sqrt(b,    sqrt1);
        each::pow (b,series::uniform(0.5f),sqrt2);
        CHECK(metric.equal(sqrt1 , sqrt2, 1e-7));
    }

}
TEST_CASE( "Series<T> log consistency", "[each]" ) {
    auto a     = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b     = std::vector({1.0,1.0,2.0,3.0,5.0});
    auto log1_ = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto log2_ = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("log2(a) must equal log(a)/log(2)"){
        each::log2(a, log1_);
        each::log (a, log2_);
        each::div (log2_, series::uniform(std::log(2.f)), log2_);
        CHECK(metric.equal(log1_ , log2_, 1e-7));
    }
}
TEST_CASE( "Series<T> log/exp consistency", "[each]" ) {
    auto a     = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b     = std::vector({1.0,1.0,2.0,3.0,5.0});
    auto loga  = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto logb  = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto lalb  = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto elalb = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto ab    = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto out   = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("exp(log(a)+log(b)) must equal a*b"){
        each::log(a, loga);
        each::log(b, logb);
        each::add(loga, logb, lalb);
        each::exp(lalb, elalb);
        each::mult(a, b, ab);
        CHECK(metric.equal(elalb, ab, 1e-7));
    }
}
TEST_CASE( "Series<T> log/exp invertibility", "[each]" ) {
    auto a   = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b   = std::vector({1.0,1.0,2.0,3.0,5.0});
    auto out = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("log(exp(a)) must equal a"){
        each::log (a,   out);
        each::exp (out, out);
        CHECK(metric.equal(a, out, 1e-7));
    }
    SECTION("log2(exp2(a)) must equal a"){
        each::log2(a,   out);
        each::exp2(out, out);
        CHECK(metric.equal(a, out, 1e-7));
    }
}

TEST_CASE( "Series<T> log2/exp2 consistency", "[each]" ) {
    auto a     = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b     = std::vector({1.0,1.0,2.0,3.0,5.0});
    auto loga  = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto logb  = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto lalb  = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto ab    = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto out   = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("exp2(log2(a)+log2(b)) must equal a*b"){
        each::log2(a, loga);
        each::log2(b, logb);
        each::add(loga, logb, lalb);
        each::exp2(lalb, out);
        each::mult(a, b, ab);
        CHECK(metric.equal(out, ab, 1e-7));
    }
}
TEST_CASE( "Series<T> log2/exp2 invertibility", "[each]" ) {
    auto a   = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b   = std::vector({1.0,1.0,2.0,3.0,5.0});
    auto out = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("log2(exp2(a)) must equal a"){
        each::log2 (a,   out);
        each::exp2 (out, out);
        CHECK(metric.equal(a, out, 1e-7));
    }
    SECTION("log2(exp2(a)) must equal a"){
        each::log2(a,   out);
        each::exp2(out, out);
        CHECK(metric.equal(a, out, 1e-7));
    }
}







TEST_CASE( "Series<T> trigonometric purity", "[each]" ) {
    auto a = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b = std::vector({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto c2 = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("sin must be called repeatedly without changing the output"){
        each::sin(a,c1);
        each::sin(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::sin(b,c1);
        each::sin(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("cos must be called repeatedly without changing the output"){
        each::cos(a,c1);
        each::cos(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::cos(b,c1);
        each::cos(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("tan must be called repeatedly without changing the output"){
        each::tan(a,c1);
        each::tan(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::tan(b,c1);
        each::tan(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

}

TEST_CASE( "Series<T> trigonometric cofunctions", "[each]" ) {
    const double pi = 3.14159265358979;
    auto a = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b = std::vector({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto c2 = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("sin must equal cos for input that is rotated by π/2"){
        each::sub(series::uniform(pi/2.0), a, c1);
        each::cos(c1,c1);
        each::sin(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::sub(series::uniform(pi/2.0), b, c1);
        each::cos(c1,c1);
        each::sin(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }
    SECTION("tan must equal cot for input that is rotated by π/2"){
        each::sub(series::uniform(pi/2.0), a, c1);
        each::cot(c1,c1);
        each::tan(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::sub(series::uniform(pi/2.0), b, c1);
        each::cot(c1,c1);
        each::tan(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }
}



TEST_CASE( "Series<T> trigonometric negative angle identities", "[each]" ) {
    auto a = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b = std::vector({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto c2 = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("negated sin must equal sin for negated input"){
        each::mult(series::uniform(-1.0), a, c1);
        each::sin(c1,c1);
        each::sin(a,c2);
        each::mult(series::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(series::uniform(-1.0), b, c1);
        each::sin(c1,c1);
        each::sin(b,c2);
        each::mult(series::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("negated cos must equal cos for negated input"){
        each::mult(series::uniform(-1.0), a, c1);
        each::cos(c1,c1);
        each::cos(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(series::uniform(-1.0), b, c1);
        each::cos(c1,c1);
        each::cos(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("negated tan must equal tan for negated input"){
        each::mult(series::uniform(-1.0), a, c1);
        each::tan(c1,c1);
        each::tan(a,c2);
        each::mult(series::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(series::uniform(-1.0), b, c1);
        each::tan(c1,c1);
        each::tan(b,c2);
        each::mult(series::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("negated csc must equal csc for negated input"){
        each::mult(series::uniform(-1.0), a, c1);
        each::csc(c1,c1);
        each::csc(a,c2);
        each::mult(series::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(series::uniform(-1.0), b, c1);
        each::csc(c1,c1);
        each::csc(b,c2);
        each::mult(series::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("negated sec must equal sec for negated input"){
        each::mult(series::uniform(-1.0), a, c1);
        each::sec(c1,c1);
        each::sec(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(series::uniform(-1.0), b, c1);
        each::sec(c1,c1);
        each::sec(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("negated cot must equal cot for negated input"){
        each::mult(series::uniform(-1.0), a, c1);
        each::cot(c1,c1);
        each::cot(a,c2);
        each::mult(series::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(series::uniform(-1.0), b, c1);
        each::cot(c1,c1);
        each::cot(b,c2);
        each::mult(series::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }
}

*/

#undef ITERATED_TEST_UNARY_OUT_PARAMETER
#undef ITERATED_TEST_BINARY_OUT_PARAMETER
#undef ITERATED_TEST_TRINARY_OUT_PARAMETER