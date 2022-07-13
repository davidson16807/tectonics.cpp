#pragma once

// 3rd party libraries

namespace compound{
    namespace property
    {
        /*
        NOTE: 
        The namespace here each serves as a "category of primitives" where objects are variables of primitive types.
        We may consider moving to the academic layer if we discover its implementation can be done in a general way.

        This namespace maps to and from aspects that are needed to calculate reflectance
        in mixtures of particles for any state of matter (solid, liquid, or gas)
        */



        // UTILITY FUNCTIONS FOR BRDFS

        /*
        "get_fraction_of_microfacets_accessible_to_ray" is Schlick's fast approximation for Smith's function.
          see Hoffmann (2015) for a gentle introduction to the concept
          see Schlick (1994) for even more details.
        */
        constexpr double get_fraction_of_microfacets_accessible_to_ray(
            const double cos_view_angle, 
            const double root_mean_slope_squared
        ){
            const double pi = 3.14159265358979;
            double m = root_mean_slope_squared;
            double v = cos_view_angle;
            // double k = m/2.0; return 2.0*v/(v+sqrt(m*m+(1.0-m*m)*v*v)); // Schlick-GGX
            double k = m*std::sqrt(2./pi); return v/(v*(1.0-k)+k); // Schlick-Beckmann
        }

        /*
        `approx_chandrasekhar_H()` is an approximate solution to the "Chandrasekar H" integral provided by Hapke (2012, eq. 8.56)
        Its error is no more than 1% across its entire domain.
        The following table compares variable names with those of Hapke (2012) and provide descriptions:
        The parameters are:
            PBR  Hapke  description
            w    w      average single scattering albedo
            NV   μ      the dot product between the view ray and surface normal, A.K.A the cosine of the angle of scattered light 
        */
        constexpr double approx_chandrasekhar_H(const double NV, const double w){
            double gamma = std::sqrt(1.0 - w);
            const double r0 = (1.0 - gamma) / (1.0 + gamma);
            return 1.0 / (1.0 - w*NV * (r0 + 0.5*(1.0 - 2.0*r0*NV) * std::log((1.0+NV)/NV) ));
        }

        /*
        `approx_chandrasekhar_H_fast()` is a fast approximate solution to the "Chandrasekar H" integral provided by Hapke (1981, eq. 10)
        Its error is no more than 3% across its entire domain.
        The following table compares variable names with those of Hapke (2012) and provide descriptions:
        The parameters are:
            PBR  Hapke  description
            w    w      average single scattering albedo
            NV   μ      the dot product between the view ray and surface normal, A.K.A the cosine of the angle of scattered light 
        */
        constexpr double approx_chandrasekhar_H_fast(const double NV, const double w){
            double gamma = std::sqrt(1.0 - w);
            return (1.0 + 2.0*NV) / (1.0 + 2.0*gamma*NV);
        }






        // BRDFS FOR SINGLE AND MULTIPLE SCATTERS

        /*
        `get_fraction_of_light_reflected_by_single_scatter_in_mixture()` is the single scatter component of the Hapke "slab"-based BRDF.
        It calculates the reflectance of any solid that is produced by mixing particles of any size and material, 
        modeling the substance as a series of slabs. 
        See Hapke 1981 (eq. 36) and Hapke (2012) for a full description, and Clark (1999) or Villanueva (2021) for a summary.
        Parameter names here are standardized here to follow conventions seen in modern PBR literature. 
        These differ from the variables used by Hapke (2012).
        The following table compares variable names and provide descriptions:
            Hapke   PBR  
            μ       NV   
            μ₀      NL   
            w       single_scatter_albedo
            G(Ω,Ω') fraction_of_light_scattered_at_angle                        equal to 1 if all particles are isotropic scatterers
            Bₛ(g)   correction_for_shadow_hiding_opposition                     equal to 0 if backscatter is ignored
            Bc(g)   correction_for_coherent_backscatter_opposition              equal to 0 if backscatter is ignored
            K       porosity_coefficient                                        equal to 1 if mixture is nonporoous
        */
        constexpr double get_fraction_of_light_reflected_by_single_scatter_in_mixture(
            const double NV, 
            const double NL, 
            const double single_scatter_albedo,
            const double fraction_of_light_scattered_at_angle,
            const double root_mean_slope_squared,
            const double correction_for_shadow_hiding_opposition,
            const double correction_for_coherent_backscatter_opposition,
            const double porosity_coefficient
        ) {
            const double _4pi = 4.0 * 3.14159265358979;
            return 1.0
                * get_fraction_of_microfacets_accessible_to_ray(NL, root_mean_slope_squared) 
                * porosity_coefficient
                * single_scatter_albedo/_4pi 
                * 1.0/(NV+NL) 
                * ((1.0 + correction_for_shadow_hiding_opposition)*fraction_of_light_scattered_at_angle)
                * (1.0 + correction_for_coherent_backscatter_opposition)
                * get_fraction_of_microfacets_accessible_to_ray(NV, root_mean_slope_squared);
        }

        /*
        `get_fraction_of_light_reflected_by_multiple_scatters_in_mixture()` is the multi-scatter component of the Hapke "slab"-based BRDF.
        It calculates the reflectance of any solid that is produced by mixing particles of any size and material, 
        modeling the substance as a series of slabs. 
        See Hapke 1981 (eq. 36) and Hapke (2012) for a full description, and Clark (1999) or Villanueva (2021) for a summary.
        Parameter names here are standardized here to follow conventions seen in modern PBR literature. 
        These differ from the variables used by Hapke (1981).
        The following table compares variable names and provide descriptions:
            Hapke   PBR  
            μ       NV   
            μ₀      NL   
            w       single_scatter_albedo
            Bc(g)   correction_for_coherent_backscatter_opposition              equal to 0 if backscatter is ignored
            K       porosity_coefficient                                        equal to 1 if mixture is nonporoous
        */
        constexpr double get_fraction_of_light_reflected_by_multiple_scatters_in_mixture(
            const double NV, 
            const double NL, 
            const double single_scatter_albedo,
            const double root_mean_slope_squared,
            const double correction_for_coherent_backscatter_opposition,
            const double porosity_coefficient
        ) {
            const double _4pi = 4.0 * 3.14159265358979;
            return 1.0
                * get_fraction_of_microfacets_accessible_to_ray(NL, root_mean_slope_squared) 
                * porosity_coefficient
                * single_scatter_albedo/_4pi 
                * 1.0/(NV+NL) 
                * ( approx_chandrasekhar_H(NV/porosity_coefficient, single_scatter_albedo)
                   *approx_chandrasekhar_H(NL/porosity_coefficient, single_scatter_albedo) - 1.0)
                * (1.0 + correction_for_coherent_backscatter_opposition)
                * get_fraction_of_microfacets_accessible_to_ray(NV, root_mean_slope_squared);
        }










        // SINGLE SCATTER ALBEDOS
        // single scatter albedo is w = S/E, where S and E are efficiencies for scattering and extinction
        // in most compounds, E=1, so it can be assumed w=S

        /*
        `approx_single_scatter_albedo_from_reflectance() ` calculates w from Hapke (2012 and 1981)
        given a reflectance observed within a laboratory. See Villanueva (2021) for a description
        */
        constexpr double approx_single_scatter_albedo_from_reflectance(
            const double reflectance
        ){
            const double R_1 = reflectance + 1.0;
            return 4.0 * reflectance / R_1*R_1;
        }

        /*
        `approx_single_scatter_albedo_from_attenuation_coefficient() ` calculates w from Hapke (2012 and 1981)
        given an attenuation coefficient and the average length of a path taken by photons in the material.
        I'm not sure the accuracy of modeling a property related to a "single scatter" using parameters that intrinsically
        involve multiple scattering events, but it is cited in 
        */
        constexpr double approx_single_scatter_albedo_from_attenuation_coefficient(
            const double attenuation_coefficient,
            const double mean_path_length
        ){
            return std::exp(-attenuation_coefficient*mean_path_length);
        }






        // SCATTERING EFFICIENCIES
        // The scattering efficiency of a mixture "S" is approximately equal to single scatter albedo "w" in most cases.
        // It can be calculated from scattering efficiencies of components compounds, "Qₛᵢ",
        // which can be calculated from the volumetric absorption coefficient, "α".


        /*
        `approx_scattering_efficiency_from_absorption_coefficient() ` calculates "Qₛ" from Hapke (1981, eq. 33)
        given the volumetric absorption coefficient α. 
        It makes it possible to estimate scattering efficiency based on laboratory measurements, 
        but only holds when extinction coefficient k<<1.
        */
        constexpr double approx_scattering_efficiency_from_absorption_coefficient(
            const si::attenuation<double> absorption_coefficient,
            const si::length<double> particle_diameter
        ){
            const si::length<double> De = 2.0*particle_diameter;
            return 1.0 / (1.0 + absorption_coefficient * De);
        }
        /*
        `approx_scattering_efficiency_from_refractive_index() ` calculates w from Hapke (2012, eq. 6.20) and Hapke (1981, eq. 26)
        given the complex refractive index (i.e. refractive index "k" and extinction coefficient "n")
        */
        constexpr double approx_scattering_efficiency_from_refractive_index(
            const double refractive_index,
            const double extinction_coefficient,
            const si::length<double> wavelength,
            const si::length<double> particle_diameter
        ){
            const double _4pi = 4.0 * 3.14159265358979;
            const double n = refractive_index;
            const double k = extinction_coefficient;
            const double nm1 = n-1.0;
            const double np1 = n+1.0;
            const double Gamma = (nm1*nm1 + k*k) / (np1*np1 + k*k);
            const double Theta = std::exp(-_4pi*k*particle_diameter/wavelength);
            const double Si = 1.0 - (0.9413 - 0.8543*Gamma - 0.0870*Gamma*Gamma)/(n*n) ;
            const double Se = 0.0587 + 0.8543*Gamma + 0.0870*Gamma*Gamma;
            return Se + (1.0-Se)*(1.0-Si)/(1.0-Si*Theta)*Theta;
        }








        // VOLUMETRIC ABSORPTION COEFFICIENTS
        // The volumetric absorption coefficient of a mixture "α" can be thought of as a measure for reflectance that is independant of grain size.
        // The reflectance of a compound reduces significantly if the sample being measured is broken into smaller grains,
        // so volumetric absorption coefficient allows laboratory measures of reflectance can be adapted to predict other situations.
        // It can be calculated from complex index of reflection, but cannot be converted back into complex index of reflection once calculated.

        /*
        `approx_absorption_coefficient_from_single_scatter_albedo_of_compound() ` calculates "Qₛ" from Hapke (1981, eq. 33)
        given a reflectance observed within a laboratory. 
        It makes it possible to estimate volumetric absorption coefficient based on laboratory measurements, 
        but only holds when extinction coefficient k<<1.
        */
        constexpr si::attenuation<double> approx_absorption_coefficient_from_scattering_efficiency(
            const double scattering_efficiency,
            const si::length<double> particle_diameter
        ){
            const si::length<double> De = 2.0*particle_diameter;
            return std::max(1.0/scattering_efficiency - 1.0, 0.0) / De;
        }

        /*
        `get_absorption_coefficient_from_refractive_index() ` calculates "Qₛ" from Hapke (1981, eq. 33)
        given the complex index of refraction (i.e. refractive index "n" and extinction coefficient "k")
        */
        constexpr si::attenuation<double> get_absorption_coefficient_from_refractive_index(
            const double refractive_index,
            const double extinction_coefficient,
            const si::length<double> wavelength
        ){
            const double _4pi = 4.0 * 3.14159265358979;
            return std::max(_4pi * refractive_index * extinction_coefficient, 0.0) / wavelength;
        }
        constexpr double get_refractive_index_from_absorption_coefficient(
            const si::attenuation<double> absorption_coefficient,
            const double extinction_coefficient,
            const si::length<double> wavelength
        ){
            const double _4pi = 4.0 * 3.14159265358979;
            return absorption_coefficient * wavelength / (_4pi*extinction_coefficient);
        }
        constexpr double get_extinction_coefficient_from_absorption_coefficient(
            const si::attenuation<double> absorption_coefficient,
            const double refractive_index,
            const si::length<double> wavelength
        ){
            const double _4pi = 4.0 * 3.14159265358979;
            return absorption_coefficient * wavelength / (_4pi*refractive_index);
        }








        // NONESSENTIAL BRDF UTILITIES


        /*
        `get_correction_for_shadow_hiding_opposition()` is the "backscatter function" described by Hapke (2012, eq. 9.22)
        It describes a correction to the reflectance function that accounts the path to the detector 
        will be clear of any obstruction if the photon already took that path going in.
        This namely occurs when the phase angle acosVL = 0.
        The parameters are:
            Hapke  PBR  
            g      acosVL
            h      particle_spacing_parameter
        */
        constexpr double get_correction_for_shadow_hiding_opposition(
            const double acosVL, 
            const double particle_spacing_parameter
        ){
            const double abs_g = std::abs(acosVL);
            const double half_tan_g_h = 0.5 * std::tan(abs_g)/particle_spacing_parameter;
            return 1.0 / (1.0 + half_tan_g_h);
        }

        /*
        `get_correction_for_coherent_backscatter_opposition()` is the "backscatter function" described by Hapke (2012, eq. 9.43)
        It describes a correction to the reflectance function that accounts the path to the detector 
        will be clear of any obstruction if the photon already took that path going in.
        This namely occurs when the phase angle acosVL = 0.
        The parameters are:
            Hapke  PBR  
            g      acosVL
            h      particle_spacing_parameter
        */
        constexpr double get_correction_for_coherent_backscatter_opposition(
            const double acosVL, 
            const double particle_spacing_parameter,
            const double porosity_coefficient
        ){
            const double abs_g = std::abs(acosVL);
            const double half_tan_g_h = std::tan(abs_g)/particle_spacing_parameter;
            return 1.0 / (1.0 + (1.3+porosity_coefficient) * (half_tan_g_h + half_tan_g_h*half_tan_g_h));
        }

        /*
        `get_porosity_coefficient()` returns a parameter used by Hapke (2012, eq. 7.45b) to indicate the porosity of a material.
        */
        constexpr double get_porosity_coefficient(
            const double fractional_volume_occupied
        ){
            const double x = 1.209 * std::pow(fractional_volume_occupied, 0.6666);
            return -std::log(1.0 - x) / x;
        }


        /*
        `get_particle_spacing_parameter()` returns a parameter used by Hapke (1981) to indicate the space between particles
        NσQₑy where y is the diameter of a circle with the same cross sectional area as openings between openings
        */
        constexpr double get_particle_spacing_parameter_for_generic_particles(
            const double number_density,
            const double cross_section,
            const double extinction_efficiency,
            const double representative_opening_diameter
        ){
            return number_density * cross_section * extinction_efficiency * representative_opening_diameter;
        }
        constexpr double get_particle_spacing_parameter_for_spherical_particles(
            const double number_density,
            const double extinction_efficiency,
            const double particle_diameter
        ){
            const double pi_over_4 = 3.141592653589 / 4.0;
            const double D = particle_diameter;
            const double L = std::pow(number_density, -1.0/3.0);
            const double D_over_L_2 = D*D / (L*L);
            return extinction_efficiency * pi_over_4 * D_over_L_2 * std::sqrt(1.0/pi_over_4 - D_over_L_2);
        }
        const double representative_particle_spacing_parameter_for_laboratory_powders = 0.15; // from Hapke (1981)
        const double representative_particle_spacing_parameter_for_lunar_regolith = 0.4;  // from Hapke (1966)

    }
}