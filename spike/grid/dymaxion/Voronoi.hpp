#pragma once

// std libraries
#include <limits>

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries
#include <units/si.hpp>             // si::units
#include "../Interleaving.hpp"
#include "Projection.hpp"

namespace dymaxion
{

    /*
    A `Voronoi` class represents a voronoi diagram,
    where points in space are indexed by an unique integer ("memory id")
    and correspond to regions in space ("positions") where those points are closest,
    such that the diagram commutes:

    position → point ↔ memory_id

    From another point of view, a `dymaxion::Voronoi` is a wrapper around a `Projection` 
    that introduces the concepts of sphere radius, vertex count, and boundary alignment. That is all.
    */
    template<typename id=int, typename scalar=double, glm::qualifier Q=glm::defaultp>
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

        const id vertex_count_per_meridian;
        const id vertex_count_per_half_triangle_leg;
        const id vertex_count_per_triangle_leg;
        const id vertex_count_per_square;
        const id vertex_count;
        const scalar vertex_count_per_half_triangle_leg_scalar;
        const scalar vertex_count_per_triangle_leg_scalar;
        const grid::Interleaving<id> row_interleave;
        const grid::Interleaving<id> square_interleave;
        const scalar radius;

        static constexpr id subgrid_count = 10;

        constexpr Voronoi(const scalar radius, const id vertex_count_per_meridian) : 
            // the boundary of two polar squares consists of cells for those boundaries plus the diagonal of a square, hence the 1+√2 factors
            projection(Projection<id,scalar,Q>()),
            vertex_count_per_meridian(vertex_count_per_meridian),
            vertex_count_per_half_triangle_leg(vertex_count_per_meridian/(s2*(s1+std::sqrt(s2)))), 
            vertex_count_per_triangle_leg(2*vertex_count_per_half_triangle_leg),
            vertex_count_per_square(vertex_count_per_triangle_leg*vertex_count_per_triangle_leg),
            vertex_count(10*vertex_count_per_square),
            vertex_count_per_half_triangle_leg_scalar(vertex_count_per_half_triangle_leg),
            vertex_count_per_triangle_leg_scalar(vertex_count_per_triangle_leg),
            row_interleave(vertex_count_per_triangle_leg),
            square_interleave(vertex_count_per_square),
            radius(radius)
        {
        }

        inline constexpr id memory_id(const IdPoint grid_id) const {
            return memory_id(ScalarPoint(grid_id));
        }

        constexpr id memory_id(const ScalarPoint grid_position) const {
            const ScalarPoint standardized(
                    (projection.standardize(grid_position / vertex_count_per_triangle_leg_scalar)
                        // apply epsilon while in the [0,1] domain so that the cast to `id` is correct
                        + vec2(std::numeric_limits<scalar>::epsilon()))
                    * vertex_count_per_triangle_leg_scalar
            );
            const IdPoint clamped(clamp(IdPoint(standardized), 0, vertex_count_per_triangle_leg-1));
            return square_interleave.interleaved_id(
                    clamped.square_id, 
                    row_interleave.interleaved_id(clamped.square_position.y, clamped.square_position.x)
                );
        }

        inline constexpr id memory_id(const vec3 sphere_position) const {
            return memory_id(grid_position(sphere_position));
        }



        inline constexpr IdPoint grid_id(const id memory_id) const {
            id square_element_id(square_interleave.element_id(memory_id));
            return IdPoint(
                square_interleave.block_id(memory_id), 
                vec2(row_interleave.element_id(square_element_id), row_interleave.block_id(square_element_id))
            );
        }
        inline constexpr IdPoint grid_id(const ScalarPoint grid_position) const
        {
            return min(IdPoint(grid_position), vertex_count_per_triangle_leg-1);
        }
        inline constexpr IdPoint grid_id(const vec3 sphere_position) const
        {
            return min(IdPoint(grid_position(sphere_position)), vertex_count_per_triangle_leg-1);
        }



        inline constexpr ScalarPoint grid_position(const id memory_id) const {
            id square_element_id(square_interleave.element_id(memory_id));
            return ScalarPoint(
                square_interleave.block_id(memory_id), 
                vec2(row_interleave.element_id(square_element_id), row_interleave.block_id(square_element_id))
            ) - vertex_count_per_triangle_leg_scalar;
        }
        inline constexpr ScalarPoint grid_position(const IdPoint grid_id) const
        {
            return grid_id;
        }
        inline constexpr ScalarPoint grid_position(const vec3 sphere_position) const
        {
            return ScalarPoint(projection.grid_id(glm::normalize(sphere_position)) * vertex_count_per_triangle_leg_scalar);
        }



        inline constexpr vec3 unit_sphere_position(const id memory_id) const
        {
            return unit_sphere_position(grid_id(memory_id));
        }
        inline constexpr vec3 unit_sphere_position(const IdPoint grid_id) const
        {
            ScalarPoint scalable(grid_id);
            return projection.sphere_position((scalable+half_cell)/vertex_count_per_triangle_leg_scalar);
        }
        inline constexpr vec3 unit_sphere_position(const ScalarPoint grid_position) const
        {
            return projection.sphere_position(grid_position/vertex_count_per_triangle_leg_scalar);
        }



        inline constexpr vec3 sphere_position(const id memory_id) const
        {
            return unit_sphere_position(memory_id) * radius;
        }
        inline constexpr vec3 sphere_position(const IdPoint grid_id) const
        {
            return unit_sphere_position(grid_id) * radius;
        }
        inline constexpr vec3 sphere_position(const ScalarPoint grid_position) const
        {
            return unit_sphere_position(grid_position) * radius;
        }

    };
}
