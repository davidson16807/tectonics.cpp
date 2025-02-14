#pragma once

// standard libraries
#include <queue> // std::priority_queue

// in-house libraries
#include <index/iterated/Nary.hpp>

namespace rock
{

    /*
    `CrustFracture` represents a nonphysical yet physically informed method 
    for modeling the fracture of a world's crust into plates. 
    It does so using a "Priority Fill" watershed algorithm
    that iteratively applies a `NeighborBasedFloodFilling` 
    algorithm on a fixed number of plates. 
    Two neighbors belong to the same plate if local stress
    falls below tensile, compressive, and shear strengths.
    */

    template<typename scalar, typename quality=glm::defaultp>
    struct CrustIsUnfractured
    {
        using vec3 = glm::vec<3,scalar,quality>;

        const scalar compressive_strength;
        const scalar tensile_strength;
        const scalar shear_strength;

        CrustIsUnfractured(
            const scalar compressive_strength,
            const scalar tensile_strength,
            const scalar shear_strength
        ):
            compressive_strength(compressive_strength),
            tensile_strength(tensile_strength),
            shear_strength(shear_strength)
        {}

        bool operator() (
            const vec3 A, const vec3 U, const vec3 B, const vec3 V, const vec3 O
        ) const {
            // vec3 A2 = A+U;
            // vec3 B2 = B+V;
            vec3 I = B-A;
            scalar l = glm::length(I);
            vec3 K = glm::normalize(A+B);
            vec3 J = l*glm::normalize(glm::cross(I,K)); // orthogonal to I, running along the surface, same scale as I
            // vec3 I2 = B2-A2;
            vec3 DL = V-U; // equivalent to I2-I, calculated as V-U to avoid precision issues;
            scalar tensile_DL = glm::dot(DL,I) / l; // effectively the scalar projection of DL onto I
            scalar shear_DL = glm::dot(DL,J) / l; // effectively the scalar projection of DL onto J
            scalar tensile_stress_factor = tensile_DL / l; // proportionate to tensile stress
            scalar shear_stress_factor = shear_DL / l; // proportionate to shear stress
            return 
              -compressive_strength < tensile_stress_factor && tensile_stress_factor < tensile_strength
              && std::abs(shear_stress_factor) < shear_strength;
            ;
        }

    }

    template<typename scalar, typename quality=glm::defaultp>
    struct CrustDistancePriority
    {
        using vec3 = glm::vec<3,scalar,quality>;
        CrustDistancePriority(){}
        bool operator() (
            const vec3 A, const vec3 U, const vec3 B, const vec3 V, const vec3 O
        ) const { 
            return glm::distance(A,O);
        }
    }

    /*
    Fractures a crust using a physically informed version of the "priority fill" watershed algorithm
    */
    template<typename id, typename scalar, typename quality=glm::defaultp>
    struct CrustFracture
    {

        const unlayered::NeighborBasedFloodFilling<id,scalar> fill;
        const iterated::Metric<adapted::GlmMetric> metric3;
        const iterated::Ternary ternary;
        const id max_plate_start_count;

        CrustFracture():
            fill(CrustIsUnfractured{1e-30, 3e-11, 3e-10}, CrustDistancePriority{}),
            metric3(adapted::GlmMetric{}),
            ternary(),
            max_plate_start_count(30)
        {}

        template<typename Grid, typename VectorRaster, typename States>
        void advance(
            const Grid& grid, 
            const VectorRaster& stress, 
            States& states
        ) const {

            // segmentation
            std::vector<scalar> lengths(stress.size());
            metric3.length(stress, lengths);
            std::vector<bool> is_considered(stress.size(), true);
            std::vector<unlayered::FloodFillState<id,scalar>> states;
            for(std::size_t j(0); j<states.size(); j++)
            {
                states.emplace_back(id(order.max_id(lengths)), id(stress.size()));
                for (id i = 0; i < max_plate_start_count; ++i)
                {
                    fill.advance(coarse, stress, is_considered, states[j]);
                }
                ternary(is_considered, lengths, procedural::uniform(0), lengths);
            }

            id candidate_count(1);
            while(candidate_count > 0) {

                if (frame_id == 0)
                {
                    for (std::size_t j(1); j < states.size(); ++j)
                    {
                        fill.advance(coarse, stress, is_considered, states[j]);
                    }
                    for (std::size_t j(1); j < states.size(); ++j)
                    {
                        ternary(states[j].is_included, procedural::uniform(j), buffer_scalars1, buffer_scalars1);
                    }
                    candidate_count = 0;
                    for (std::size_t j(1); j < states.size(); ++j)
                    {
                        candidate_count += states[j].candidates.size();
                    }
                }

            }

        }

    };

}

/*
properties to satisfy:
* contiguous (requires floodfill)
* symmetric with respect to plate seeds
* cells with largest vectors must occur at boundaries 
* lowest complexity allowable
* in-order traversal if possible
*/

