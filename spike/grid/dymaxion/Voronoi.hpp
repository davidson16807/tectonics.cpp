#pragma once

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
    `Voronoi` is a wrapper around a `Projection` 
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
            square_interleave(vertex_count),
            radius(radius)
        {
        }

        inline constexpr id memory_id(const IdPoint grid_id) const {
            return memory_id(ScalarPoint(grid_id));
        }

        constexpr id memory_id(const ScalarPoint grid_position) const {
            const ScalarPoint world_position((grid_position + half_cell) / vertex_count_per_triangle_leg_scalar);
            const ScalarPoint standardized_world_position(projection.standardize(world_position));
            const ScalarPoint standard_grid_position(standardized_world_position * vertex_count_per_triangle_leg_scalar - half_cell);
            const IdPoint standard_grid_uid = IdPoint(round(standard_grid_position));
            const id memory_id = 
                square_interleave.interleaved_id(
                    standard_grid_uid.square_id,
                    row_interleave.interleaved_id(
                        std::clamp(standard_grid_uid.square_position.y, 0, vertex_count_per_triangle_leg-1),
                        std::clamp(standard_grid_uid.square_position.x, 0, vertex_count_per_triangle_leg-1)
                    )
                );
            return memory_id;
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
            return grid_position;
        }
        inline constexpr IdPoint grid_id(const vec3 sphere_position) const
        {
            return IdPoint(round(ScalarPoint(projection.grid_id(glm::normalize(sphere_position))) * vertex_count_per_triangle_leg_scalar - half_cell));
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
            ScalarPoint grid_id(projection.grid_id(glm::normalize(sphere_position)));
            return grid_id * vertex_count_per_triangle_leg_scalar - half_cell;
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
            return projection.sphere_position((grid_position+half_cell)/vertex_count_per_triangle_leg_scalar);
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
