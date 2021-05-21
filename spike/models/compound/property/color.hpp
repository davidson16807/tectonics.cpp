#pragma once

// 3rd party libraries

namespace compound{
    namespace color
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
            // double get_attenuation_coefficient_from_cross_section(const double attenuation_cross_section, const double number_density)
            // {
            //     return attenuation_cross_section * number_density;
            // }
            // double get_cross_section_from_attenuation_coefficient(const double attenuation_coefficient, const double number_density)
            // {
            //     return attenuation_coefficient / number_density;
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
        
        // the following is a first order approximation from Vincent and Hunt (1968), copied from Eastes (1989)
        double approx_reflectance_from_attenuation_coefficient_and_refractive_index(
            const double attenuation_coefficient, 
            const double refractive_index_of_material,
            const double refractive_index_of_medium,
            const double wavelength
        ){
            const double delta_n = refractive_index_of_material - refractive_index_of_medium;
            const double sum_n = refractive_index_of_material + refractive_index_of_medium;
            const double n1_lambda_over_4pi_squared = refractive_index_of_material * refractive_index_of_material * wavelength * wavelength / (16.0 * pi*pi);
            const double n1_lambda_alpha_over_4pi_squared = n1_lambda_over_4pi_squared * attenuation_coefficient * attenuation_coefficient;
            return (delta_n * delta_n + n1_lambda_alpha_over_4pi_squared) / (sum_n * sum_n + n1_lambda_alpha_over_4pi_squared);
        }
        
        double solve_attenuation_coefficient_from_reflectance_and_refactive_index(
            const double reflectance, 
            const double refractive_index_of_material,
            const double refractive_index_of_medium,
            const double wavelength,
            const int iteration_count = 10
        ){
            const double delta_n = refractive_index_of_material - refractive_index_of_medium;
            // const double sum_n = refractive_index_of_material + refractive_index_of_medium;
            const double n1_lambda_over_4pi_squared = refractive_index_of_material * refractive_index_of_material * wavelength * wavelength / (16.0 * pi*pi);

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
            const double intercept = sqrt(-delta_n * delta_n / n1_lambda_over_4pi_squared);
            // const double asymptote = sqrt(-sum_n * sum_n / n1_lambda_over_4pi_squared);
            double alpha = intercept;
            double error = 0.0;
            double dF_dalpha = 0.0;
            double dalpha = 1e-4;
            for (int i = 0; i < iteration_count; ++i)
            {
                dF_dalpha = 
                    (approx_reflectance_from_attenuation_coefficient_and_refractive_index(alpha + dalpha, refractive_index_of_material, refractive_index_of_medium, wavelength) - 
                     approx_reflectance_from_attenuation_coefficient_and_refractive_index(alpha, refractive_index_of_material, refractive_index_of_medium, wavelength)) / dalpha;
                error = reflectance - approx_reflectance_from_attenuation_coefficient_and_refractive_index(alpha, refractive_index_of_material, refractive_index_of_medium, wavelength);
                alpha = std::max(alpha + error / dF_dalpha, intercept);
            }
            return alpha;
        }

    }

}