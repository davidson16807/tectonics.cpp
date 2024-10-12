#pragma once

// std libraries
#include <cmath>
#include <limits>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide p main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <index/known.hpp>
#include <index/procedural/Range.hpp>
#include <index/procedural/Uniform.hpp>
#include <index/procedural/noise/UnitIntervalNoise.hpp>

#include <index/adapted/boolean/BooleanMetric.hpp>
#include <index/adapted/boolean/BooleanBitset.hpp>
#include <index/adapted/boolean/BooleanStrings.hpp>
#include <index/adapted/scalar/ScalarMetric.hpp>
#include <index/adapted/scalar/ScalarStrings.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>

#include <index/aggregated/Bitset.hpp>
#include <index/aggregated/Metric.hpp>
#include <index/aggregated/Order.hpp>
#include <index/aggregated/Strings.hpp>

#include <index/iterated/Bitset.hpp>

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
            return strings.format(a);
        }

    };

}

#define ITERATED_TEST_UNARY_OUT_PARAMETER(TYPE,F)   [=](auto x){ std::vector<TYPE> out(x.size()); (F(x,out)); return out; }
#define ITERATED_TEST_BINARY_OUT_PARAMETER(TYPE,F)  [=](auto x, auto y){ std::vector<TYPE> out(std::max(x.size(), y.size())); (F(x,y,out)); return out; }
#define ITERATED_TEST_TRINARY_OUT_PARAMETER(TYPE,F) [=](auto x, auto y, auto z){ std::vector<TYPE> out(std::max(x.size(), std::max(y.size(), z.size()))); (F(x,y,z,out)); return out; }


TEST_CASE( "arithmetic on each nonzero of a series is a field", "[iterated]" ) {

    adapted::ScalarMetric<double> submetric;    
    aggregated::Metric metric(submetric);
    adapted::ScalarStrings<double> substrings;
    adapted::SymbolicOrder order;
    aggregated::Order ordered(order);
    aggregated::Strings strings(substrings, ordered);
    iterated::Adapter broad (1e-6, metric, strings);
    iterated::Adapter narrow(1e-6, metric, strings);

    auto subarithmetic = adapted::SymbolicArithmetic(0.0, 1.0);
    auto arithmetic = iterated::Arithmetic(subarithmetic);

    std::vector<procedural::UnitIntervalNoise<double>> noises {
        procedural::UnitIntervalNoise<double>(10.0, 1e4),
        procedural::UnitIntervalNoise<double>(20.0, 2e4),
        procedural::UnitIntervalNoise<double>(30.0, 3e4),
        procedural::UnitIntervalNoise<double>(40.0, 4e4)
    };

    std::vector<procedural::Range<int>> nonzero_ranges {
        procedural::Range<int>(1,101),
        procedural::Range<int>(2,102)
        // procedural::Range<int>(-101,-1)
    };

    std::vector<procedural::Uniform<double>> nonzero_uniforms {
        procedural::Uniform<double>(-2.0),
        procedural::Uniform<double>(1.0),
        procedural::Uniform<double>(2.0),
    };

    test::Field field(
        "0", procedural::uniform(0.0), 
        "1", procedural::uniform(1.0), 
        "arithmetic.add",       ITERATED_TEST_BINARY_OUT_PARAMETER(double, arithmetic.add),
        "arithmetic.subtract",  ITERATED_TEST_BINARY_OUT_PARAMETER(double, arithmetic.subtract),
        "arithmetic.multiply",  ITERATED_TEST_BINARY_OUT_PARAMETER(double, arithmetic.multiply),
        "arithmetic.divide",    ITERATED_TEST_BINARY_OUT_PARAMETER(double, arithmetic.divide)
    );

    // UNARY TESTS
    REQUIRE(field.valid(broad, noises));
    REQUIRE(field.valid(broad, nonzero_ranges));
    REQUIRE(field.valid(broad, nonzero_uniforms));

    // NOTE: properties such as associativity are not valid when mixing different types of procedural::, 
    // since the output is a vector and does not share their properties

    // REQUIRE(field.valid(broad, noises, nonzero_ranges));
    // REQUIRE(field.valid(broad, nonzero_ranges, nonzero_uniforms));
    // REQUIRE(field.valid(broad, noises, nonzero_ranges, nonzero_uniforms));

}

TEST_CASE( "arithmetic on each nonzero of a series is a commutative ring", "[iterated]" ) {

    adapted::ScalarMetric<double> submetric;    
    aggregated::Metric metric(submetric);
    adapted::ScalarStrings<double> substrings;
    adapted::SymbolicOrder order;
    aggregated::Order ordered(order);
    aggregated::Strings strings(substrings, ordered);
    iterated::Adapter broad (1e-6, metric, strings);
    iterated::Adapter narrow(1e-6, metric, strings);

    auto subarithmetic = adapted::SymbolicArithmetic(0.0, 1.0);
    auto arithmetic = iterated::Arithmetic(subarithmetic);

    std::vector<procedural::UnitIntervalNoise<double>> noises {
        procedural::UnitIntervalNoise<double>(10.0, 1e4),
        procedural::UnitIntervalNoise<double>(20.0, 2e4),
        procedural::UnitIntervalNoise<double>(30.0, 3e4),
        procedural::UnitIntervalNoise<double>(40.0, 4e4)
    };

    std::vector<procedural::Range<int>> nonzero_ranges {
        procedural::Range<int>(1,101),
        procedural::Range<int>(2,102)
        // procedural::Range<int>(-101,-1)
    };

    std::vector<procedural::Uniform<double>> nonzero_uniforms {
        procedural::Uniform<double>(-2.0),
        procedural::Uniform<double>(1.0),
        procedural::Uniform<double>(2.0)
    };

    test::CommutativeRing ring(
        "0", procedural::uniform(0.0), 
        "1", procedural::uniform(1.0), 
        "arithmetic.add",       ITERATED_TEST_BINARY_OUT_PARAMETER(double, arithmetic.add),
        "arithmetic.subtract",  ITERATED_TEST_BINARY_OUT_PARAMETER(double, arithmetic.subtract),
        "arithmetic.multiply",  ITERATED_TEST_BINARY_OUT_PARAMETER(double, arithmetic.multiply)
    );

    // UNARY TESTS
    REQUIRE(ring.valid(broad, noises));
    REQUIRE(ring.valid(broad, nonzero_ranges));
    REQUIRE(ring.valid(broad, nonzero_uniforms));

    // NOTE: properties such as associativity are not valid when mixing different types of procedural::, 
    // since the output is a vector and does not share their properties

    // REQUIRE(ring.valid(broad, noises, nonzero_ranges));
    // REQUIRE(ring.valid(broad, nonzero_ranges, nonzero_uniforms));
    // REQUIRE(ring.valid(broad, noises, nonzero_ranges, nonzero_uniforms));

}






TEST_CASE( "bitset on each of a series is a commutative rig", "[iterated]" ) {

    adapted::BooleanMetric submetric;    
    aggregated::Metric metric(submetric);
    adapted::BooleanStrings substrings;
    adapted::BooleanBitset subbitset;
    aggregated::Bitset aggregation(subbitset);
    iterated::Bitset iteration(subbitset);
    aggregated::Strings strings(substrings, aggregation);
    iterated::Adapter exact (0, metric, strings);

    std::vector<std::vector<bool>> vectors {
        std::vector<bool>({1,0,0,1,0}),
        std::vector<bool>({0,1,0,1,0}),
    };

    test::CommutativeRig rig(
        "false", procedural::uniform(false), 
        "true", procedural::uniform(true), 
        "bitset.unite",       ITERATED_TEST_BINARY_OUT_PARAMETER(bool, iteration.unite),
        "bitset.intersect",       ITERATED_TEST_BINARY_OUT_PARAMETER(bool, iteration.intersect)
    );

    REQUIRE(rig.valid(exact, vectors));

}









/*









TEST_CASE( "Series<T> negation involutivity", "[iterated]" ) {
    auto a = std::vector({1,0,0,1,0});
    auto c = std::vector({1,1,2,3,5});

    SECTION("--a must equal a"){
        each::negate(a, c);
        each::negate(c, c);
        CHECK(metric.equal(c, a));
    }

}










TEST_CASE( "Series<T> sqrt purity", "[iterated]" ) {
    auto a  = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto c1 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto c2 = std::vector({0.0,0.0,0.0,0.0,0.0});
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        each::sqrt(a,c1);
        each::sqrt(a,c2);
        CHECK(metric.equal(c1,c2, 1e-7));
    }
}
TEST_CASE( "Series<T> sqrt consistency", "[iterated]" ) {
    auto a     = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b     = std::vector({1.0,1.0,2.0,3.0,5.0});
    auto sqrt1 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto sqrt2 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto log1_ = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto log2_ = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("sqrt(a) must equal pow(a,1/2)"){
        each::sqrt(a,    sqrt1);
        each::pow (a,procedural::uniform(0.5f),sqrt2);
        CHECK(metric.equal(sqrt1 , sqrt2, 1e-7));

        each::sqrt(b,    sqrt1);
        each::pow (b,procedural::uniform(0.5f),sqrt2);
        CHECK(metric.equal(sqrt1 , sqrt2, 1e-7));
    }

}
TEST_CASE( "Series<T> log consistency", "[iterated]" ) {
    auto a     = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b     = std::vector({1.0,1.0,2.0,3.0,5.0});
    auto log1_ = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto log2_ = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("log2(a) must equal log(a)/log(2)"){
        each::log2(a, log1_);
        each::log (a, log2_);
        each::div (log2_, procedural::uniform(std::log(2.f)), log2_);
        CHECK(metric.equal(log1_ , log2_, 1e-7));
    }
}
TEST_CASE( "Series<T> log/exp consistency", "[iterated]" ) {
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
TEST_CASE( "Series<T> log/exp invertibility", "[iterated]" ) {
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

TEST_CASE( "Series<T> log2/exp2 consistency", "[iterated]" ) {
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
TEST_CASE( "Series<T> log2/exp2 invertibility", "[iterated]" ) {
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







TEST_CASE( "Series<T> trigonometric purity", "[iterated]" ) {
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

TEST_CASE( "Series<T> trigonometric cofunctions", "[iterated]" ) {
    const double pi = 3.14159265358979;
    auto a = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b = std::vector({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto c2 = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("sin must equal cos for input that is rotated by π/2"){
        each::sub(procedural::uniform(pi/2.0), a, c1);
        each::cos(c1,c1);
        each::sin(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::sub(procedural::uniform(pi/2.0), b, c1);
        each::cos(c1,c1);
        each::sin(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }
    SECTION("tan must equal cot for input that is rotated by π/2"){
        each::sub(procedural::uniform(pi/2.0), a, c1);
        each::cot(c1,c1);
        each::tan(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::sub(procedural::uniform(pi/2.0), b, c1);
        each::cot(c1,c1);
        each::tan(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }
}



TEST_CASE( "Series<T> trigonometric negative angle identities", "[iterated]" ) {
    auto a = std::vector({1.0,2.0,3.0,4.0,5.0});
    auto b = std::vector({-1.0,1.0,-2.0,2.0,3.0});
    auto c1 = std::vector({0.0,0.0,0.0,0.0,0.0});
    auto c2 = std::vector({0.0,0.0,0.0,0.0,0.0});

    SECTION("negated sin must equal sin for negated input"){
        each::mult(procedural::uniform(-1.0), a, c1);
        each::sin(c1,c1);
        each::sin(a,c2);
        each::mult(procedural::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(procedural::uniform(-1.0), b, c1);
        each::sin(c1,c1);
        each::sin(b,c2);
        each::mult(procedural::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("negated cos must equal cos for negated input"){
        each::mult(procedural::uniform(-1.0), a, c1);
        each::cos(c1,c1);
        each::cos(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(procedural::uniform(-1.0), b, c1);
        each::cos(c1,c1);
        each::cos(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("negated tan must equal tan for negated input"){
        each::mult(procedural::uniform(-1.0), a, c1);
        each::tan(c1,c1);
        each::tan(a,c2);
        each::mult(procedural::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(procedural::uniform(-1.0), b, c1);
        each::tan(c1,c1);
        each::tan(b,c2);
        each::mult(procedural::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("negated csc must equal csc for negated input"){
        each::mult(procedural::uniform(-1.0), a, c1);
        each::csc(c1,c1);
        each::csc(a,c2);
        each::mult(procedural::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(procedural::uniform(-1.0), b, c1);
        each::csc(c1,c1);
        each::csc(b,c2);
        each::mult(procedural::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("negated sec must equal sec for negated input"){
        each::mult(procedural::uniform(-1.0), a, c1);
        each::sec(c1,c1);
        each::sec(a,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(procedural::uniform(-1.0), b, c1);
        each::sec(c1,c1);
        each::sec(b,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }

    SECTION("negated cot must equal cot for negated input"){
        each::mult(procedural::uniform(-1.0), a, c1);
        each::cot(c1,c1);
        each::cot(a,c2);
        each::mult(procedural::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
        each::mult(procedural::uniform(-1.0), b, c1);
        each::cot(c1,c1);
        each::cot(b,c2);
        each::mult(procedural::uniform(-1.0),c2,c2);
        CHECK(metric.equal(c1, c2, 1e-7));
    }
}

*/

#undef ITERATED_TEST_UNARY_OUT_PARAMETER
#undef ITERATED_TEST_BINARY_OUT_PARAMETER
#undef ITERATED_TEST_TRINARY_OUT_PARAMETER