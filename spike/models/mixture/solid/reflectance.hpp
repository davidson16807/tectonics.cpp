#pragma once

// 3rd party libraries

namespace mixture{
    namespace solid
    {




        /*
        `approx_single_scatter_albedo_of_mixture_from_scattering_efficiencies_and_densities() ` 
        calculates w from Hapke (1981, eq. 17)
        It is easier to find values for, but it assumes extinction efficiencies Eᵢ≈1,
        which only holds if particles are larged compared to the wavelength.
        */
        constexpr float approx_single_scatter_albedo_of_mixture_from_scattering_efficiencies_and_densities(
            const std::vector<si::density> bulk_densities, 
            const std::vector<si::density> solid_densities, 
            const std::vector<si::length> diameters,
            const std::vector<float> scattering_efficiencies,
        ){
            float denominator = 0.0;
            float numerator = 0.0;
            for(int i=0; i<bulk_densities.size(); i++){
                numerator   += bulk_densities[i] / (solid_densities[i] * diameters[i]) * scattering_efficiencies[i];
                denominator += bulk_densities[i] / (solid_densities[i] * diameters[i]);
            }
            return numerator / denominator;
        }
        /*
        `get_single_scatter_albedo_of_mixture_from_scattering_efficiencies_and_number_densities()` calculates w from Hapke (1981)
        given the extinction and scattering efficiencies for a mixture
        */
        constexpr float get_single_scatter_albedo_of_mixture_from_scattering_efficiencies_and_number_densities(
            const std::vector<si::number_density> number_densities, 
            const std::vector<si::area> cross_sections_of_component_particles, 
            const std::vector<float> scattering_efficiencies,
            const std::vector<float> extinction_efficiencies
        ){
            float extinction_efficiencies_of_mixture = 0.0;
            float scattering_efficiencies_of_mixture = 0.0;
            for(int i=0; i<number_densities.size(); i++){
                scattering_efficiencies_of_mixture += number_densities[i] * cross_sections_of_component_particles[i] * scattering_efficiencies[i];
                extinction_efficiencies_of_mixture += number_densities[i] * cross_sections_of_component_particles[i] / extinction_efficiencies[i];
            }
            return scattering_efficiencies_of_mixture / extinction_efficiencies_of_mixture;
        }
        /*
        `get_scattering_efficiencies_of_mixture_from_number_density()` 
        equivalent to the "S" function of Hapke (1981)
        */
        constexpr float get_scattering_efficiencies_of_mixture_from_number_density(
            const std::vector<si::number_density> number_densities, 
            const std::vector<si::area> cross_sections_of_component_particles, 
            const std::vector<float> scattering_efficiencies
        ){
            float scattering_efficiencies_of_mixture = 0.0;
            for(int i=0; i<number_densities.size(); i++){
                scattering_efficiencies_of_mixture   += number_densities[i] * cross_sections_of_component_particles[i] * scattering_efficiencies[i];
            }
            return scattering_efficiencies_of_mixture;
        }
        /*
        `get_scattering_efficiencies_of_mixture_from_number_density()` 
        equivalent to the "E" function of Hapke (1981)
        */
        constexpr float get_extinction_efficiencies_of_mixture_from_number_density(
            const std::vector<si::number_density> number_densities, 
            const std::vector<float> extinction_efficiencies
        ){
            float extinction_efficiencies_of_mixture = 0.0;
            for(int i=0; i<number_densities.size(); i++){
                extinction_efficiencies_of_mixture += number_densities[i] * cross_sections_of_component_particles[i] * extinction_efficiencies[i];
            }
            return extinction_efficiencies_of_mixture;
        }
        /*
        `get_fraction_of_light_scattered_by_angle_in_mixture()` 
        is the average particle phase function "P(g)" of the Hapke "slab"-based BRDF.
            Hapke   PBR                                                         Notes
            σᵢ      cross_sections_of_component_particles                       
            Qₛᵢ     scattering_efficiencies_of_components                       
            pᵢ(g)   fractions_of_light_scattered_by_angle_in_components         equal to 1 for isotropic scatterers
        */
        constexpr float get_fraction_of_light_scattered_by_angle_in_mixture(
            const std::vector<si::area> cross_sections_of_component_particles, 
            const std::vector<float> scattering_efficiencies_of_components, 
            const std::vector<float> fractions_of_light_scattered_by_angle_in_components
        ){
            float weighted_phase = 0.0;
            float scattering_efficiencies_of_mixture = 0.0;
            for(int i=0; i<cross_sections_of_component_particles.size(); i++){
                weighted_phase += cross_sections_of_component_particles[i] * scattering_efficiencies_of_components[i] * fractions_of_light_scattered_by_angle_in_components[i];
                scattering_efficiencies_of_mixture += number_densities[i] * cross_sections_of_component_particles[i] * scattering_efficiencies[i];
            }
            return weighted_phase/scattering_efficiencies_of_mixture;
        }
    }
}