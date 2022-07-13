#pragma once

// 3rd party libraries

namespace compound{
    namespace property
    {
        /*
        NOTE: 
        The namespace here each serves as a "category of primitives" where objects are variables of primitive types.
        We may consider moving to the academic layer if we discover its implementation can be done in a general way.

        This namespace maps to and from different expressions of a compound's color, 
        namely absorption coefficients and spectral reflectivities.
        */

        namespace 
        {
            /*
            This private namespace describes a category that maps relations between 
            attenuation measures, number density, and path length through a material
            */
            // double get_number_density(const double density, const double particle_mass)
            // {
            //     return density / particle_mass;
            // }
            // double get_transmittance(const double attenuation_coefficient, const double path_length)
            // {
            //     return exp(-attenuation_coefficient*path_length);
            // }
            // double get_path_length(const double transmittance, const double attenuation_coefficient)
            // {
            //     return -log(transmittance)/attenuation_coefficient;
            // }
            // double get_attenuation_coefficient(const double transmittance, const double path_length)
            // {
            //     return -log(transmittance)/path_length;
            // }
            const double pi = 3.141592653589;
        }

        si::attenuation<double> get_attenuation_coefficient_from_cross_section(const si::area<double> attenuation_cross_section, const si::number_density<double> number_density)
        {
            return attenuation_cross_section * number_density;
        }

        si::area<double> get_cross_section_from_attenuation_coefficient(const si::attenuation<double> attenuation_coefficient, const si::number_density<double> number_density)
        {
            return attenuation_coefficient / number_density;
        }

        /*
        `get_extinction_coefficient_from_absorption_coefficient` converts from 
        absorption coefficient (i.e., the coefficient used in Beer's law) 
        to extinction coefficient (i.e., the imaginary index of refraction)
        See Clark (1999, eq. 1.3) for discussion surrounding it.
        */
        float get_extinction_coefficient_from_absorption_coefficient(const si::attenuation<double> absorption_coefficient, const si::length<double> wavelength)
        {
            const float _4pi = 4.0 * 3.1415926535;
            return absorption_coefficient * wavelength / _4pi;
        }
        /*
        `get_absorption_coefficient_from_extinction_coefficient` converts 
        from extinction coefficient (i.e., the imaginary index of refraction)
        to absorption coefficient (i.e., the coefficient used in Beer's law).
        See Clark (1999, eq. 1.3) for discussion surrounding it.
        */
        si::attenuation<double> get_absorption_coefficient_from_extinction_coefficient(const float extinction_coefficient, const si::length<double> wavelength)
        {
            const float _4pi = 4.0 * 3.1415926535;
            return _4pi * extinction_coefficient / wavelength;
        }

        /*
        `get_reflectance_from_extinction_coefficient_and_refractive_index` converts 
        from refractive index and extinction coefficient (i.e., the complex index of refraction)
        to reflectance (i.e., the fraction of light reflected back at the source on the first scatter event of a head-on collision with a uniform material).
        See Clark (1999, eq. 1.3) for discussion surrounding it.
        */
        double get_reflectance_from_extinction_coefficient_and_refractive_index(
            const double extinction_coefficient, 
            const double refractive_index_of_material,
            const double refractive_index_of_medium
        ){
            const double K2 = extinction_coefficient*extinction_coefficient;
            const double delta_n = refractive_index_of_material - refractive_index_of_medium;
            const double sum_n = refractive_index_of_material + refractive_index_of_medium;
            return (delta_n*delta_n + K2) / (sum_n*sum_n + K2);
        }

        // NOTE: these tests do not pass, but are nonessential to continue development
        double approx_reflectance_from_attenuation_coefficient_and_refractive_index(
            const si::attenuation<double> attenuation_coefficient, 
            const double refractive_index_of_material,
            const double refractive_index_of_medium,
            const si::length<double> wavelength
        ){
            const double delta_n = refractive_index_of_material - refractive_index_of_medium;
            const double sum_n = refractive_index_of_material + refractive_index_of_medium;
            const auto n1_lambda_over_4pi_squared = refractive_index_of_material * refractive_index_of_material * wavelength * wavelength / (16.0 * pi*pi);
            const auto n1_lambda_alpha_over_4pi_squared = n1_lambda_over_4pi_squared * attenuation_coefficient * attenuation_coefficient;
            return (delta_n * delta_n + n1_lambda_alpha_over_4pi_squared) / (sum_n * sum_n + n1_lambda_alpha_over_4pi_squared);
        }
        
        /*
        // For now, we will comment out solve_attenuation_coefficient_from_reflectance_and_refactive_index and the accompanying tests here
        // the following is a first order approximation from Vincent and Hunt (1968), copied from Eastes (1989)
        si::attenuation<double> solve_attenuation_coefficient_from_reflectance_and_refactive_index(
            const double reflectance, 
            const double refractive_index_of_material,
            const double refractive_index_of_medium,
            const si::length<double> wavelength,
            const int iteration_count = 10
        ){
            const double delta_n = refractive_index_of_material - refractive_index_of_medium;
            // std::cout << "delta_n:                                " << delta_n << std::endl;
            // const double sum_n = refractive_index_of_material + refractive_index_of_medium;
            const auto n1_lambda_over_4pi_squared = refractive_index_of_material * refractive_index_of_material * wavelength * wavelength / (16.0 * pi*pi);
            // std::cout << "n1_lambda_over_4pi_squared.to_string(): " << n1_lambda_over_4pi_squared.to_string() << std::endl;

            // This function uses Newton's method to solve for `attenuation_coefficient` 
            // where `approx_reflectance_from_attenuation_coefficient_and_refractive_index()` equals `reflectance`.
            // If one assumes realistic values for refractive indices, reflectances, and wavelengths, 
            // then the correct solution is guaranteed to occur after the asymptote in that function.
            // This occurs where the denominator in the function is zero.
            // An adequate starting estimate occurs at the x-intercept where the numerator is zero, 
            // since the derivative at this point will always be appreciably finite and nonzero, 
            // unlike near the asymptote or towards infinity.
            // Since reflectance is always positive for realistic values, 
            // the x-intercept is also guaranteed to occur before the desired reflectance is reached, 
            // which means we can set solutions not to fall below the x-intercept to prevent nonphysical solutions.
            const auto intercept = si::sqrt(delta_n * delta_n / n1_lambda_over_4pi_squared);
            // std::cout << "intercept.to_string():                  " << intercept.to_string() << std::endl;
            // const auto asymptote = si::sqrt(sum_n * sum_n / n1_lambda_over_4pi_squared);
            si::attenuation<double> alpha = intercept;
            // std::cout << "alpha.to_string():                      " << alpha.to_string() << std::endl;
            double error(0.0);
            auto dF_dalpha(0.0 * si::meter);
            auto dalpha(1e-4 / si::meter);
            for (int i = 0; i < iteration_count; ++i)
            {
                dF_dalpha = 
                    (approx_reflectance_from_attenuation_coefficient_and_refractive_index(alpha + dalpha, refractive_index_of_material, refractive_index_of_medium, wavelength) - 
                     approx_reflectance_from_attenuation_coefficient_and_refractive_index(alpha, refractive_index_of_material, refractive_index_of_medium, wavelength)) / dalpha;
                // std::cout << "dF_dalpha.to_string():                  " << dF_dalpha.to_string() << std::endl;
                error = approx_reflectance_from_attenuation_coefficient_and_refractive_index(alpha, refractive_index_of_material, refractive_index_of_medium, wavelength) - reflectance;
                // std::cout << "error:                                  " << error << std::endl;
                alpha = si::max(alpha - error / dF_dalpha, 0.0/si::meter);
                // std::cout << "alpha.to_string():                      " << alpha.to_string() << std::endl;
            }
            return alpha;
        }
        */

    }

}