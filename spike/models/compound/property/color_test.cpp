
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "color.hpp"


TEST_CASE( "solve_attenuation_coefficient_from_reflectance_and_refactive_index()  purity", "[compound]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        const double reflectance = 0.02;
        const double refractive_index_of_material = 1.3325;
        const double refractive_index_of_medium = 1.0;
        const double wavelength = 600.0;
        CHECK(
            compound::color::solve_attenuation_coefficient_from_reflectance_and_refactive_index(
                    reflectance, 
                    refractive_index_of_material,
                    refractive_index_of_medium,
                    wavelength
                ) == Approx(compound::color::solve_attenuation_coefficient_from_reflectance_and_refactive_index(
                    reflectance, 
                    refractive_index_of_material,
                    refractive_index_of_medium,
                    wavelength
                )).margin(0.001)
        );
    }
}
TEST_CASE( "approx_reflectance_from_attenuation_coefficient_and_refractive_index()  purity", "[compound]" ) {
    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        const double attenuation_coefficient = 7.79e-9;
        const double refractive_index_of_material = 1.3325;
        const double refractive_index_of_medium = 1.0;
        const double wavelength = 600.0;
        CHECK(
            compound::color::approx_reflectance_from_attenuation_coefficient_and_refractive_index(
                    attenuation_coefficient, 
                    refractive_index_of_material,
                    refractive_index_of_medium,
                    wavelength
                ) == Approx(compound::color::approx_reflectance_from_attenuation_coefficient_and_refractive_index(
                    attenuation_coefficient, 
                    refractive_index_of_material,
                    refractive_index_of_medium,
                    wavelength
                )).margin(0.001)
        );
    }
}
TEST_CASE( "solve_attenuation_coefficient_from_reflectance_and_refactive_index() / approx_reflectance_from_attenuation_coefficient_and_refractive_index()  left invertibility", "[compound]" ) {
    SECTION("There exists a value that when applied as the first operand to a function returns the original value")
    {
        const double attenuation_coefficient = 7.79e-9;
        const double refractive_index_of_material = 1.3325;
        const double refractive_index_of_medium = 1.0;
        const double wavelength = 600.0;
        CHECK(
            compound::color::solve_attenuation_coefficient_from_reflectance_and_refactive_index(
                compound::color::approx_reflectance_from_attenuation_coefficient_and_refractive_index(
                    attenuation_coefficient, 
                    refractive_index_of_material,
                    refractive_index_of_medium,
                    wavelength
                ),
                refractive_index_of_material,
                refractive_index_of_medium,
                wavelength
            ) == Approx(attenuation_coefficient).margin(0.001)
        );
    }
}
TEST_CASE( "solve_attenuation_coefficient_from_reflectance_and_refactive_index predictability", "[compound]" ) {
    SECTION("The function reproduces properties of known compounds")
    {
        const double reflectance = 0.02;
        const double attenuation_coefficient = 7.79e-9;
        const double refractive_index_of_material = 1.3325;
        const double refractive_index_of_medium = 1.0;
        const double wavelength = 600.0;
        CHECK(
            compound::color::solve_attenuation_coefficient_from_reflectance_and_refactive_index(
                reflectance,
                refractive_index_of_material,
                refractive_index_of_medium,
                wavelength
            ) == Approx(attenuation_coefficient).margin(0.001)
        );
    }
}
TEST_CASE( "approx_reflectance_from_attenuation_coefficient_and_refractive_index predictability", "[compound]" ) {
    SECTION("The function reproduces properties of known compounds")
    {
        const double reflectance = 0.02;
        const double attenuation_coefficient = 7.79e-9;
        const double refractive_index_of_material = 1.3325;
        const double refractive_index_of_medium = 1.0;
        const double wavelength = 600.0;
        CHECK(
            compound::color::approx_reflectance_from_attenuation_coefficient_and_refractive_index(
                attenuation_coefficient,
                refractive_index_of_material,
                refractive_index_of_medium,
                wavelength
            ) == Approx(reflectance).margin(0.001)
        );
    }
}
