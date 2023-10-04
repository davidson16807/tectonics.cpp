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

        const id vertices_per_meridian;
        const id vertices_per_half_triangle_leg;
        const id vertices_per_triangle_leg;
        const id vertices_per_square;
        const id vertex_count;
        const scalar vertices_per_half_triangle_leg_scalar;
        const scalar vertices_per_triangle_leg_scalar;
        const grid::Interleaving<id> row_interleave;
        const grid::Interleaving<id> square_interleave;
        const scalar radius;

        static constexpr id subgrid_count = 10;

        constexpr Voronoi(const scalar radius, const id vertices_per_meridian) : 
            // the boundary of two polar squares consists of cells for those boundaries plus the diagonal of a square, hence the 1+√2 factors
            projection(Projection<id,scalar,Q>()),
            vertices_per_meridian(vertices_per_meridian),
            vertices_per_half_triangle_leg(vertices_per_meridian/(s2*(s1+std::sqrt(s2)))), 
            vertices_per_triangle_leg(2*vertices_per_half_triangle_leg),
            vertices_per_square(vertices_per_triangle_leg*vertices_per_triangle_leg),
            vertex_count(10*vertices_per_square),
            vertices_per_half_triangle_leg_scalar(vertices_per_half_triangle_leg),
            vertices_per_triangle_leg_scalar(vertices_per_triangle_leg),
            row_interleave(vertices_per_triangle_leg),
            square_interleave(vertex_count),
            radius(radius)
        {
        }

        inline constexpr id memory_id(const IdPoint grid_id) const {
            return memory_id(ScalarPoint(grid_id));
        }

        constexpr id memory_id(const ScalarPoint grid_position) const {
            const ScalarPoint world_position((grid_position + half_cell) / vertices_per_triangle_leg_scalar);
            const ScalarPoint standardized_world_position(projection.standardize(world_position));
            const ScalarPoint standard_grid_position(standardized_world_position * vertices_per_triangle_leg_scalar - half_cell);
            const IdPoint standard_grid_uid = IdPoint(round(standard_grid_position));
            const id memory_id = 
                square_interleave.interleaved_id(
                    standard_grid_uid.square_id,
                    row_interleave.interleaved_id(
                        std::clamp(standard_grid_uid.square_position.y, 0, vertices_per_triangle_leg-1),
                        std::clamp(standard_grid_uid.square_position.x, 0, vertices_per_triangle_leg-1)
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
            return min(IdPoint(grid_position(sphere_position)), vertices_per_triangle_leg-1);
        }



        inline constexpr ScalarPoint grid_position(const id memory_id) const {
            id square_element_id(square_interleave.element_id(memory_id));
            return ScalarPoint(
                square_interleave.block_id(memory_id), 
                vec2(row_interleave.element_id(square_element_id), row_interleave.block_id(square_element_id))
            ) - vertices_per_triangle_leg_scalar;
        }
        inline constexpr ScalarPoint grid_position(const IdPoint grid_id) const
        {
            return grid_id;
        }
        inline constexpr ScalarPoint grid_position(const vec3 sphere_position) const
        {
            return ScalarPoint(projection.grid_id(glm::normalize(sphere_position)) * vertices_per_triangle_leg_scalar);
        }



        inline constexpr vec3 unit_sphere_position(const id memory_id) const
        {
            return unit_sphere_position(grid_id(memory_id));
        }
        inline constexpr vec3 unit_sphere_position(const IdPoint grid_id) const
        {
            ScalarPoint scalable(grid_id);
            return projection.sphere_position((scalable+half_cell)/vertices_per_triangle_leg_scalar);
        }
        inline constexpr vec3 unit_sphere_position(const ScalarPoint grid_position) const
        {
            return projection.sphere_position(grid_position/vertices_per_triangle_leg_scalar);
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
