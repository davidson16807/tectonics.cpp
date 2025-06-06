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
    template<typename id, typename id2, typename scalar, glm::qualifier Q=glm::defaultp>
    class Voronoi
    {

        using vec2  = glm::vec<2,scalar,glm::defaultp>;
        using vec3  = glm::vec<3,scalar,glm::defaultp>;
        using ipoint = Point<id2,id>;
        using point = Point<id2,scalar>;

        static constexpr vec2 half_cell = vec2(0.5);
        static constexpr scalar s1 = 1;
        static constexpr scalar s2 = 2;
        static constexpr id2 i1 = 1;

        const Projection<id,id2,scalar,Q> projection;

    public:

        const id2 vertices_per_square_side;
        const scalar vertices_per_square_side_inverse;
        const scalar vertices_per_meridian;
        const id2 vertices_per_square;
        const id2 vertex_count;
        const scalar radius;

        static constexpr id2 square_count = 10;

        Voronoi(const scalar radius, const id2 vertices_per_square_side) : 
            projection(Projection<id,id2,scalar,Q>()),
            vertices_per_square_side(vertices_per_square_side),
            vertices_per_square_side_inverse(1.0/vertices_per_square_side),
            vertices_per_meridian(vertices_per_square_side * s2*(s1+std::sqrt(s2))),
            vertices_per_square(vertices_per_square_side * vertices_per_square_side),
            vertex_count(square_count*vertices_per_square),
            radius(radius)
        {
        }


        inline constexpr ipoint grid_id(const point& grid_position) const
        {
            return min(ipoint(grid_position), vertices_per_square_side-i1);
        }
        inline constexpr ipoint grid_id(const vec3 sphere_position) const
        {
            return min(ipoint(grid_position(sphere_position)), id(vertices_per_square_side-i1));
        }



        inline constexpr point grid_position(const ipoint& grid_id) const
        {
            return grid_id;
        }
        inline constexpr point grid_position(const vec3 normalized_sphere_position) const
        {
            return point(projection.grid_id(normalized_sphere_position) * scalar(vertices_per_square_side));
        }



        inline constexpr vec3 sphere_normal(const ipoint& grid_id) const
        {
            point scalable(grid_id);
            return projection.sphere_position((scalable+half_cell) * vertices_per_square_side_inverse);
        }
        inline constexpr vec3 sphere_normal(const point& grid_position) const
        {
            return projection.sphere_position(grid_position * vertices_per_square_side_inverse);
        }



        inline constexpr vec3 sphere_position(const ipoint& grid_id) const
        {
            return sphere_normal(grid_id) * radius;
        }
        inline constexpr vec3 sphere_position(const point& grid_position) const
        {
            return sphere_normal(grid_position) * radius;
        }

    };
}
