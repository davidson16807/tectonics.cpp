#pragma once

// std libraries
#include <limits>

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries
#include <grid/cartesian/Interleaving.hpp>
#include "Projection.hpp"

namespace dymaxion
{

    /*
    A `Voronoi` class represents a voronoi diagram,
    where each position in a space has a single point that is nearest to it.

    From another point of view, a `dymaxion::Voronoi` is a wrapper around a `Projection` 
    that introduces the concepts of sphere radius, vertex count, and boundary alignment. That is all.
    */
    template<typename id, typename scalar, glm::qualifier Q=glm::defaultp>
    class Voronoi
    {

        using ivec2 = glm::vec<2,id,glm::defaultp>;
        using vec2  = glm::vec<2,scalar,glm::defaultp>;
        using vec3  = glm::vec<3,scalar,glm::defaultp>;
        using IdPoint = Point<id,id>;
        using ScalarPoint = Point<id,scalar>;

        static constexpr vec2 half_cell = vec2(0.5);
        static constexpr scalar s1 = 1;
        static constexpr scalar s2 = 2;

        const Projection<id,scalar,Q> projection;

    public:

        const id vertices_per_square_side;
        const scalar vertices_per_square_side_scalar;
        const scalar vertices_per_meridian;
        const id vertices_per_square;
        const id vertex_count;
        const scalar radius;

        static constexpr scalar square_side_to_meridian_vertex_ratio = s2*(s1+std::sqrt(s2));
        static constexpr id square_count = 10;

        constexpr Voronoi(const scalar radius, const id vertices_per_square_side) : 
            projection(Projection<id,scalar,Q>()),
            vertices_per_square_side(vertices_per_square_side),
            vertices_per_square_side_scalar(vertices_per_square_side),
            vertices_per_meridian(vertices_per_square_side * square_side_to_meridian_vertex_ratio),
            vertices_per_square(vertices_per_square_side * vertices_per_square_side),
            vertex_count(square_count*vertices_per_square),
            radius(radius)
        {
        }


        inline constexpr IdPoint grid_id(const ScalarPoint& grid_position) const
        {
            return min(IdPoint(grid_position), vertices_per_square_side-1);
        }
        inline constexpr IdPoint grid_id(const vec3 sphere_position) const
        {
            return min(IdPoint(grid_position(sphere_position)), vertices_per_square_side-1);
        }



        inline constexpr ScalarPoint grid_position(const IdPoint& grid_id) const
        {
            return grid_id;
        }
        inline constexpr ScalarPoint grid_position(const vec3 sphere_position) const
        {
            return ScalarPoint(projection.grid_id(glm::normalize(sphere_position)) * vertices_per_square_side_scalar);
        }



        inline constexpr vec3 sphere_normal(const IdPoint& grid_id) const
        {
            ScalarPoint scalable(grid_id);
            return projection.sphere_position((scalable+half_cell)/vertices_per_square_side_scalar);
        }
        inline constexpr vec3 sphere_normal(const ScalarPoint& grid_position) const
        {
            return projection.sphere_position(grid_position/vertices_per_square_side_scalar);
        }



        inline constexpr vec3 sphere_position(const IdPoint& grid_id) const
        {
            return sphere_normal(grid_id) * radius;
        }
        inline constexpr vec3 sphere_position(const ScalarPoint& grid_position) const
        {
            return sphere_normal(grid_position) * radius;
        }

    };

    template<typename id, typename scalar, glm::qualifier Q=glm::defaultp>
    Voronoi<id,scalar,Q> voronoi_from_vertices_per_meridian(const scalar radius, const id vertices_per_meridian)
    {
        // the boundary of two polar squares consists of cells for those boundaries plus the diagonal of a square, hence the 1+√2 factors
        scalar vertices_per_square_side(vertices_per_meridian / Voronoi<id,scalar,Q>::square_side_to_meridian_vertex_ratio);
    }
}
