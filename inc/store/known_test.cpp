#pragma once

// std libraries
#include <cmath>
#include <limits>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide p main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <store/known.hpp>
#include <store/whole.hpp>
#include <store/series/Range.hpp>
#include <store/series/Uniform.hpp>
#include <store/series/noise/UnitIntervalNoise.hpp>

#include <test/macros.hpp>
#include <test/structures/ringlike.hpp>

namespace known {

    template<typename T>
    struct Adapter{
        T threshold;
        std::size_t test_size;

        Adapter(const T threshold, const std::size_t test_size):
            threshold(threshold),
            test_size(test_size)
        {}

        template<typename Series1, typename Series2>
        bool equal(const Series1& a, const Series2& b) const {
            return whole::distance(
                map(a,series::range(test_size)),
                map(b,series::range(test_size))) <= threshold;
        }

        template<typename Series>
        std::string print(const Series& a) const {
            return whole::to_string(a);
        }

    };

}

TEST_CASE( "arithmetic on nonzero knowns are a field", "[known]" ) {

    known::Adapter<double> broad (1e-6, 30);
    known::Adapter<double> narrow(1e-6, 30);

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
        "known::add",  TEST_BINARY(known::add),
        "known::sub",  TEST_BINARY(known::sub),
        "known::mult", TEST_BINARY(known::mult),
        "known::div",  TEST_BINARY(known::div)
    );

    // UNARY TESTS
    // REQUIRE(field.valid(broad, noises));
    REQUIRE(field.valid(broad, nonzero_ranges));
    REQUIRE(field.valid(broad, nonzero_uniforms));

}


TEST_CASE( "arithmetic on any known is a commutative ring", "[known]" ) {

    known::Adapter<double> broad (1e-6, 30);
    known::Adapter<double> narrow(1e-6, 30);

    std::vector<series::UnitIntervalNoise<double>> noises {
        series::UnitIntervalNoise<double>(10.0, 1e4),
        series::UnitIntervalNoise<double>(20.0, 2e4),
        series::UnitIntervalNoise<double>(30.0, 3e4),
        series::UnitIntervalNoise<double>(40.0, 4e4)
    };

    std::vector<series::Range<double>> ranges {
        series::Range<double>(100),
        series::Range<double>(-30,30)
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
        "known::add",  TEST_BINARY(known::add),
        "known::sub",  TEST_BINARY(known::sub),
        "known::mult", TEST_BINARY(known::mult)
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

