#pragma once

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3


// in-house libraries
#include <units/si.hpp>             // si::units
#include "Tesselation.hpp"
#include "../Interleaving.hpp"

namespace collignon 
{

    /*
    `Voronoi` is a wrapper around a `Tesselation` 
    that introduces the concepts of sphere radius, vertex count, and boundary alignment. That is all.
    */
    template<typename id, typename scalar>
    class Voronoi
    {

        using ivec2 = glm::vec<2,id,glm::defaultp>;
        using vec2 = glm::vec<2,scalar,glm::defaultp>;
        using vec3 = glm::vec<3,scalar,glm::defaultp>;

        const Tesselation<scalar> tesselation;
        const Interleaving<id> interleaving;

    public:
        const id vertex_count_per_half_meridian;
        const id vertex_count_per_meridian;
        const id vertex_count;
        const scalar vertex_count_per_half_meridian_scalar;
        const scalar half_cell = 0.5;
        const scalar radius;

        constexpr Voronoi(const scalar radius, const id vertex_count_per_half_meridian) : 
            tesselation(Projection<scalar>()),
            interleaving(2*vertex_count_per_half_meridian),
            vertex_count_per_half_meridian(vertex_count_per_half_meridian),
            vertex_count_per_meridian(2*vertex_count_per_half_meridian),
            vertex_count(2*vertex_count_per_half_meridian*2*vertex_count_per_half_meridian),
            vertex_count_per_half_meridian_scalar(vertex_count_per_half_meridian),
            radius(radius)
        {
        }

        inline constexpr id memory_id(const ivec2 grid_id) const {
            return memory_id(vec2(grid_id));
        }

        constexpr id memory_id(const vec2 grid_position) const {
            const vec2 world_position = (grid_position + half_cell) / vertex_count_per_half_meridian_scalar;
            const vec2 standardized_world_position = tesselation.standardize(world_position);
            const vec2 standard_grid_position = standardized_world_position * vertex_count_per_half_meridian_scalar - half_cell;
            const ivec2 standard_grid_uid = ivec2(glm::round(standard_grid_position)) + vertex_count_per_half_meridian;
            const id memory_id = interleaving.interleaved_id(
                std::clamp(standard_grid_uid.y, 0, vertex_count_per_meridian-1), 
                std::clamp(standard_grid_uid.x, 0, vertex_count_per_meridian-1)
            );
            return memory_id;
        }

        inline constexpr id memory_id(const vec3 sphere_position) const {
            return memory_id(grid_position(sphere_position));
        }



        inline constexpr ivec2 grid_id(const id memory_id) const {
            return ivec2 (interleaving.element_id(memory_id), interleaving.block_id(memory_id)) - vertex_count_per_half_meridian;
        }
        inline constexpr ivec2 grid_id(const vec2 grid_position) const
        {
            return grid_position;
        }
        inline constexpr ivec2 grid_id(const vec3 sphere_position) const
        {
            return glm::round(tesselation.sphere_to_tesselation(glm::normalize(sphere_position)) * vertex_count_per_half_meridian_scalar - half_cell);
        }



        inline constexpr vec2 grid_position(const id memory_id) const {
            return vec2 (interleaving.element_id(memory_id), interleaving.block_id(memory_id)) - vertex_count_per_half_meridian_scalar;
        }
        inline constexpr vec2 grid_position(const ivec2 grid_id) const
        {
            return grid_id;
        }
        inline constexpr vec2 grid_position(const vec3 sphere_position) const
        {
            return tesselation.sphere_to_tesselation(glm::normalize(sphere_position)) * vertex_count_per_half_meridian_scalar - half_cell;
        }



        inline constexpr vec3 unit_sphere_position(const id memory_id) const
        {
            return unit_sphere_position(grid_id(memory_id));
        }
        inline constexpr vec3 unit_sphere_position(const ivec2 grid_id) const
        {
            return tesselation.tesselation_to_sphere((vec2(grid_id) + half_cell) / vertex_count_per_half_meridian_scalar);
        }
        inline constexpr vec3 unit_sphere_position(const vec2 grid_position) const
        {
            return tesselation.tesselation_to_sphere((grid_position + half_cell) / vertex_count_per_half_meridian_scalar);
        }



        inline constexpr vec3 sphere_position(const id memory_id) const
        {
            return unit_sphere_position(memory_id) * radius;
        }
        inline constexpr vec3 sphere_position(const ivec2 grid_id) const
        {
            return unit_sphere_position(grid_id) * radius;
        }
        inline constexpr vec3 sphere_position(const vec2 grid_position) const
        {
            return unit_sphere_position(grid_position) * radius;
        }

    };
}