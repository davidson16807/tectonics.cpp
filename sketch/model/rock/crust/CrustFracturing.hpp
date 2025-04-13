#pragma once

// c libraries
#include <cmath>

// standard libraries
#include <queue> // std::priority_queue


// 3rd party libraries
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

// in-house libraries
#include <index/iterated/Nary.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Metric.hpp>
#include <index/procedural/Uniform.hpp>
#include <index/aggregated/Arithmetic.hpp>
#include <index/aggregated/Order.hpp>
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/adapted/glm/GlmMetric.hpp>

#include <raster/unlayered/NeighborBasedFloodFilling.hpp> // unlayered::NeighborBasedFloodFilling

namespace rock
{

    /*
    `CrustIsUnfractured` represents a nonphysical yet physically informed method 
    for modeling the fracture of a world's crust into plates. 
    It does so using a "Priority Fill" watershed algorithm
    that iteratively applies a `NeighborBasedFloodFilling` 
    algorithm on a fixed number of plates. 
    Two neighbors belong to the same plate if local stress
    falls below tensile, compressive, and shear strengths.
    */

    template<typename scalar, glm::qualifier quality=glm::defaultp>
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

    };

    template<typename scalar, glm::qualifier quality=glm::defaultp>
    struct CrustDistancePriority
    {
        using vec3 = glm::vec<3,scalar,quality>;
        CrustDistancePriority(){}
        bool operator() (
            const vec3 A, const vec3 U, const vec3 B, const vec3 V, const vec3 O
        ) const { 
            return glm::distance(A,O);
        }
    };

    /*
    Fractures a crust using a physically informed version of the "priority fill" watershed algorithm
    */
    template<typename id, typename scalar, glm::qualifier quality=glm::defaultp>
    struct CrustFracturing
    {

        using iraster = std::vector<id>;

        using Fracture = unlayered::FloodFillState<id,scalar>;
        using Fractures = std::vector<Fracture>;

        const unlayered::NeighborBasedFloodFilling<id,scalar,
            CrustIsUnfractured<scalar,quality>,
            CrustDistancePriority<scalar,quality>> fill;
        const aggregated::Arithmetic<adapted::TypedSymbolicArithmetic<id>> stats;
        const aggregated::Order<adapted::SymbolicOrder> order;
        const iterated::Arithmetic<adapted::SymbolicArithmetic> arithmetic;
        const iterated::Metric<adapted::GlmMetric> metric3;
        const iterated::Ternary ternary;
        const iterated::Identity copy;
        const id max_plate_start_count;

        CrustFracturing():
            fill(
                CrustIsUnfractured<scalar,quality>{1e-30, 3e-11, 3e-10}, 
                CrustDistancePriority<scalar,quality>{}),
            stats(adapted::TypedSymbolicArithmetic{0,1}),
            order(adapted::SymbolicOrder{}),
            arithmetic(adapted::SymbolicArithmetic{}),
            metric3(adapted::GlmMetric{}),
            ternary(),
            copy(),
            max_plate_start_count(30)
        {}

        template<typename Grid>
        Fractures initialize(
            const Grid& grid,
            const id plate_count
        ) const {
            Fractures fractures;
            for (id i = 0; i < plate_count; ++i)
            {
                fractures.emplace_back(id(-1), id(grid.vertex_count()));
            }
            return fractures;
        }

        template<typename Grid, typename VectorRaster>
        void fracture(
            const Grid& grid, 
            const VectorRaster& stress, 
            Fractures& fractures
        ) const {

            // segmentation
            std::vector<scalar> lengths(stress.size());
            metric3.length(stress, lengths);
            std::vector<bool> is_considered(stress.size(), true);
    std::cout << "9a" << std::endl;
            for(std::size_t j(0); j<fractures.size(); j++)
            {
                fill.reset(id(order.max_id(lengths)), fractures[j]);
                for (id i = 0; i < max_plate_start_count; ++i)
                {
                    fill.advance(grid, stress, is_considered, fractures[j]);
                }
                ternary(is_considered, lengths, procedural::uniform(0), lengths);
            }

    std::cout << "9b" << std::endl;
            std::size_t candidate_count;
            do {
                for (std::size_t j(1); j < fractures.size(); ++j)
                {
                    fill.advance(grid, stress, is_considered, fractures[j]);
                }
                candidate_count = 0;
                for (std::size_t j(1); j < fractures.size(); ++j)
                {
                  candidate_count += fractures[j].candidates.size();
                }
    // std::cout << std::to_string(candidate_count) << std::endl;
            } while(candidate_count > 0);
    std::cout << "9c" << std::endl;

        }

        template<typename Fractures>
        void plate_counts(
            const Fractures& fractures,
            iraster& out
        ) const {

            copy(procedural::uniform(0), out);
            for (std::size_t i = 0; i < fractures.size(); ++i)
            {
                arithmetic.add(fractures[i].is_included, out, out);
            }

        }

        template<typename Fractures>
        void plate_sizes(
            const Fractures& fractures,
            iraster& out
        ) const {

            copy(procedural::uniform(0), out);
            for (std::size_t i = 0; i < fractures.size(); ++i)
            {
                out[i] = stats.sum(fractures[i].is_included);
            }

        }

        template<typename Fractures>
        void plate_map(
            const Fractures& fractures,
            iraster& out
        ) const {

            // results from fractures() are always disjoint, 
            // therefore `out` does not need to be filled with 0s
            for (std::size_t j(1); j < fractures.size(); ++j)
            {
                ternary(fractures[j].is_included, procedural::uniform(j), out, out);
            }

        }

    };

}

/*
properties to satisfy:
x regions represented within `fractures` are disjoint
  cells with largest vectors must occur at boundaries 
  lowest complexity allowable
  in-order traversal if possible
*/

