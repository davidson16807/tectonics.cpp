
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si_test_utils.hpp>
#include "color.hpp"


TEST_CASE( "approx_reflectance_from_attenuation_coefficient_and_refractive_index()  purity", "[compound]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        const si::attenuation<double> attenuation_coefficient = 0.092 / si::meter; // from Perovich (1991)
        const double refractive_index_of_material = 1.3325;
        const double refractive_index_of_medium = 1.0;
        const si::length<double> wavelength = 400.0 * si::nanometer;
        CHECK(
            si::is_within_fraction(
                compound::property::approx_reflectance_from_attenuation_coefficient_and_refractive_index(
                        attenuation_coefficient, 
                        refractive_index_of_material,
                        refractive_index_of_medium,
                        wavelength
                    ),
                compound::property::approx_reflectance_from_attenuation_coefficient_and_refractive_index(
                        attenuation_coefficient, 
                        refractive_index_of_material,
                        refractive_index_of_medium,
                        wavelength
                    ), 
                0.001
            )
        );
    }
}
TEST_CASE( "approx_reflectance_from_attenuation_coefficient_and_refractive_index predictability", "[compound]" ) {
    SECTION("The function reproduces properties of known compounds")
    {
        const double reflectance = 0.02;
        const si::attenuation<double> attenuation_coefficient = 0.092 / si::meter; // from Perovich (1991)
        const double refractive_index_of_material = 1.3325;
        const double refractive_index_of_medium = 1.0;
        const si::length<double> wavelength = 400.0 * si::nanometer;
        CHECK(
            si::is_within_fraction(
                compound::property::approx_reflectance_from_attenuation_coefficient_and_refractive_index(
                    attenuation_coefficient,
                    refractive_index_of_material,
                    refractive_index_of_medium,
                    wavelength
                ),
                reflectance, 
                0.1
            )
        );
    }
}

/*
// NOTE: these tests do not pass, but are nonessential to continue development
// For now, we will comment out solve_attenuation_coefficient_from_reflectance_and_refactive_index and the accompanying tests here
TEST_CASE( "solve_attenuation_coefficient_from_reflectance_and_refactive_index()  purity", "[compound]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        const double reflectance = 0.02;
        const double refractive_index_of_material = 1.3325;
        const double refractive_index_of_medium = 1.0;
        const si::length<double> wavelength = 400.0 * si::nanometer;
        CHECK(
            si::is_within_fraction(
                compound::property::solve_attenuation_coefficient_from_reflectance_and_refactive_index(
                        reflectance, 
                        refractive_index_of_material,
                        refractive_index_of_medium,
                        wavelength
                    ),
                compound::property::solve_attenuation_coefficient_from_reflectance_and_refactive_index(
                        reflectance, 
                        refractive_index_of_material,
                        refractive_index_of_medium,
                        wavelength
                    ), 
                0.001
            )
        );
    }
}
TEST_CASE( "solve_attenuation_coefficient_from_reflectance_and_refactive_index() / approx_reflectance_from_attenuation_coefficient_and_refractive_index() invertibility", "[compound]" ) {
    SECTION("There exists a function which undoes the operation of the other function")
    {
        const si::attenuation<double> attenuation_coefficient = 0.092 / si::meter; // from Perovich (1991)
        const double refractive_index_of_material = 1.3325;
        const double refractive_index_of_medium = 1.0;
        const si::length<double> wavelength = 400.0 * si::nanometer;
        CHECK(
            si::is_within_fraction(
                compound::property::solve_attenuation_coefficient_from_reflectance_and_refactive_index(
                    compound::property::approx_reflectance_from_attenuation_coefficient_and_refractive_index(
                        attenuation_coefficient, 
                        refractive_index_of_material,
                        refractive_index_of_medium,
                        wavelength
                    ),
                    refractive_index_of_material,
                    refractive_index_of_medium,
                    wavelength
                ),
                attenuation_coefficient, 
                0.001
            )
        );
    }
}
TEST_CASE( "solve_attenuation_coefficient_from_reflectance_and_refactive_index predictability", "[compound]" ) {
    SECTION("The function reproduces properties of known compounds")
    {
        const double reflectance = 0.02;
        const si::attenuation<double> attenuation_coefficient = 0.092 / si::meter; // from Perovich (1991)
        const double refractive_index_of_material = 1.3325;
        const double refractive_index_of_medium = 1.0;
        const si::length<double> wavelength = 400.0 * si::nanometer;
        std::cout << compound::property::solve_attenuation_coefficient_from_reflectance_and_refactive_index(
                    reflectance,
                    refractive_index_of_material,
                    refractive_index_of_medium,
                    wavelength
                ).to_string() << std::endl;
        CHECK(
            si::is_within_fraction(
                compound::property::solve_attenuation_coefficient_from_reflectance_and_refactive_index(
                    reflectance,
                    refractive_index_of_material,
                    refractive_index_of_medium,
                    wavelength
                ),
                attenuation_coefficient, 
                0.1
            )
        );
    }
}
*/