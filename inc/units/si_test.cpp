#pragma once

// std libraries
#include <cmath>
#include <limits>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide r main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include "si.hpp"

#include <test/macros.hpp>
#include <test/adapter.hpp>
#include <test/structures.hpp>

// #include "test_tools.hpp"


/*
`UnitAdapter` leverages standardized function names
to compare and display its variables
*/
template<typename T>
struct UnitAdapter{
    const T threshold;

    UnitAdapter(T threshold):
        threshold(threshold)
    {}

    template<typename T2>
    bool equal(const T2& a, const T2& b) const {
        const T2 denominator(si::abs(a)+si::abs(b));
        return denominator == 0.0*denominator || 2 * si::distance(a, b) / denominator < threshold;
    }
    bool equal(const double a, const double b) const {
        const double denominator(std::abs(a)+std::abs(b));
        return denominator == 0.0*denominator || 2 * std::abs(a-b) / denominator < threshold;
    }

    template<typename T2>
    std::string print(const T2& a) const {
        return si::to_string(a);
    }
    std::string print(const double& a) const {
        return std::to_string(a);
    }

};


TEST_CASE( "SI dimensions are individually modules and are collectively a commutative group over multiplication", "[math]" ) {

    UnitAdapter<double> adapter(1e-2);

    std::vector<double> scalars { -2.0, 2.0 }; // NOTE: we exclude zero since division by zero is not defined

    std::vector<si::molar_heat_capacity<double>> heat_capacities {
        2.0*si::joule/(si::kelvin*si::mole),
        -3.0*si::joule/(si::kelvin*si::mole),
        4.0*si::joule/(si::kelvin*si::mole)
    };

    std::vector<si::luminous_intensity<double>> luminous_intensities {
        3.0*si::candela,
        -4.0*si::candela,
        5.0*si::candela
    };

    std::vector<si::voltage<double>> voltages {
        -5.0*si::volt,
        6.0*si::volt,
        -7.0*si::volt
    };

    test::Field heat_capacity_addition(
        "0", 0.0*si::joule/(si::kelvin*si::mole), 
        "1", 1.0,  
        "SI unit addition",       TEST_SYMBOL(+),
        "SI unit subtraction",    TEST_SYMBOL(-),
        "SI unit multiplication", TEST_SYMBOL(*),
        "SI unit division",       TEST_SYMBOL(/)
    );

    test::Field luminous_intensity_addition(
        "0", 0.0*si::candela, 
        "1", 1.0,  
        "SI unit addition",       TEST_SYMBOL(+),
        "SI unit subtraction",    TEST_SYMBOL(-),
        "SI unit multiplication", TEST_SYMBOL(*),
        "SI unit division",       TEST_SYMBOL(/)
    );

    test::Field voltage_addition(
        "0", 0.0*si::volt, 
        "1", 1.0,  
        "SI unit addition",       TEST_SYMBOL(+),
        "SI unit subtraction",    TEST_SYMBOL(-),
        "SI unit multiplication", TEST_SYMBOL(*),
        "SI unit division",       TEST_SYMBOL(/)
    );

    test::CommutativeGroup multiplication(
        "1", 1.0,  
        "SI unit multiplication", TEST_SYMBOL(*),
        "SI unit division",       TEST_SYMBOL(/)
    );

    // UNARY TESTS
    REQUIRE(heat_capacity_addition      .valid(adapter, heat_capacities     ));
    REQUIRE(luminous_intensity_addition .valid(adapter, luminous_intensities));
    REQUIRE(voltage_addition            .valid(adapter, voltages            ));

    REQUIRE(multiplication.valid(adapter, heat_capacities     ));
    REQUIRE(multiplication.valid(adapter, luminous_intensities));
    REQUIRE(multiplication.valid(adapter, voltages            ));

    // BINARY TESTS
    REQUIRE(multiplication.valid(adapter, luminous_intensities, luminous_intensities));
    REQUIRE(multiplication.valid(adapter, luminous_intensities, heat_capacities     ));
    REQUIRE(multiplication.valid(adapter, luminous_intensities, voltages            ));
    REQUIRE(multiplication.valid(adapter, luminous_intensities, scalars             ));
    REQUIRE(multiplication.valid(adapter, heat_capacities,      luminous_intensities));
    REQUIRE(multiplication.valid(adapter, heat_capacities,      heat_capacities     ));
    REQUIRE(multiplication.valid(adapter, heat_capacities,      voltages            ));
    REQUIRE(multiplication.valid(adapter, heat_capacities,      scalars             ));
    REQUIRE(multiplication.valid(adapter, voltages,             luminous_intensities));
    REQUIRE(multiplication.valid(adapter, voltages,             heat_capacities     ));
    REQUIRE(multiplication.valid(adapter, voltages,             voltages            ));
    REQUIRE(multiplication.valid(adapter, voltages,             scalars             ));

    // TRINARY TESTS
    REQUIRE(multiplication.valid(adapter, luminous_intensities, heat_capacities, voltages ));
    REQUIRE(multiplication.valid(adapter, luminous_intensities, heat_capacities, scalars  ));
    REQUIRE(multiplication.valid(adapter, luminous_intensities, voltages,        scalars  ));
    REQUIRE(multiplication.valid(adapter, heat_capacities,      voltages,        scalars  ));

}






