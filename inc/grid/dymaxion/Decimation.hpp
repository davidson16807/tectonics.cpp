#pragma once

// std libraries
#include <limits>

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries
#include "Point.hpp"
#include "Indexing.hpp"

namespace dymaxion
{

    /*
    `Decimation` represents a downsampling by decimation: 
        https://en.wikipedia.org/wiki/Downsampling_(signal_processing) 
    
    Downsampling is represented as a series that maps memory ids 
    of the larger grid to memory ids of the smaller grid. 

    Decimation is faster than most downsampling since it operates on fewer domains (i.e. memory and grid ids).
    It should also produce better results for some use cases, such as for producing fractal brownian noise.
    However unlike most downsampling, `Decimation` requires knowledge of how closeness is represented between grid ids,
    so `Decimation` must be implemented specific to `dymaxion::`.
    */
    template<typename id=int, typename scalar=double, glm::qualifier Q=glm::defaultp>
    class Decimation
    {

        using ivec2 = glm::vec<2,id,glm::defaultp>;
        using vec2  = glm::vec<2,scalar,glm::defaultp>;
        using vec3  = glm::vec<3,scalar,glm::defaultp>;
        using IdPoint = Point<id,id>;
        using ScalarPoint = Point<id,scalar>;

        static constexpr vec2 half_cell = vec2(0.5);
        static constexpr vec2 epsilon = std::numeric_limits<scalar>::epsilon();

        const dymaxion::Indexing<id,scalar> coarse;
        const dymaxion::Indexing<id,scalar> fine;
        const scalar scaling_factor;

    public:
        constexpr Decimation(const id vertices_per_side) : 
            interleave(vertices_per_side),
            scaling_factor(coarse.vertices_per_square_side_scalar/fine.vertices_per_square_side_scalar)
        {
        }

        constexpr inline size_type size() const { return 1; } \

        constexpr inline value_type operator()(const size_type memory_id ) const { 
            return coarse.memory_id(IdPoint(round(
                ScalarPoint(fine.grid_id(memory_id))+half_cell * scaling_factor)+epsilon));
        }

        constexpr inline value_type operator[](const size_type memory_id ) const { 
            return coarse.memory_id(IdPoint(round(
                ScalarPoint(fine.grid_id(memory_id))+half_cell * scaling_factor)+epsilon));
        }

    };

}

