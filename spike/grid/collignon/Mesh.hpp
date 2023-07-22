#pragma once

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3


// in-house libraries
#include <units/si.hpp>             // si::units
#include "Tesselation.hpp"
#include "Interleaving.hpp"

namespace collignon 
{

    /*
    `Mesh` is a wrapper around a `Tesselation` 
    that introduces the concepts of sphere radius, vertex count, and boundary alignment. That is all.
    */
    template<typename Tid, typename Tfloat>
    class Mesh
    {
        using ivec2 = glm::vec<2,Tid,glm::defaultp>;
        using vec2 = glm::vec<2,Tfloat,glm::defaultp>;
        using vec3 = glm::vec<3,Tfloat,glm::defaultp>;

        const Tesselation<Tfloat> tesselation;
        const Interleaving<Tid> interleaving;
        const Tfloat epsilon_for_integer_cast = 0.1; // ensures that integer casts do not produce numbers that are one value lower than expected

    public:
        const Tid vertex_count_per_half_meridian;
        const Tid vertex_count_per_meridian;
        const Tid vertex_count;
        const Tfloat vertex_count_per_half_meridian_float;
        const Tfloat half_cell = 0.5;
        const Tfloat radius;

        ~Mesh()
        {

        }
        constexpr Mesh(const Tfloat radius, const Tid vertex_count_per_half_meridian) : 
            tesselation(Projection<Tfloat>()),
            interleaving(2*vertex_count_per_half_meridian),
            vertex_count_per_half_meridian(vertex_count_per_half_meridian),
            vertex_count_per_meridian(2*vertex_count_per_half_meridian),
            vertex_count(4*vertex_count_per_half_meridian*vertex_count_per_half_meridian),
            vertex_count_per_half_meridian_float(vertex_count_per_half_meridian),
            radius(radius)
        {
        }

        constexpr Tid memory_id(const ivec2 grid_id) const {
            const vec2 world_position = (grid_position + half_cell) / vertex_count_per_half_meridian_float;
            const vec2 standardized_world_position = tesselation.standardize(world_position);
            const vec2 standard_grid_position = standardized_world_position * vertex_count_per_half_meridian_float - half_cell;

            const ivec2 standard_grid_uid = ivec2(glm::round(standard_grid_position)) + vertex_count_per_half_meridian;
            const Tid memory_id = interleaving.interleaved_id(
                std::clamp(standard_grid_uid.y, 0, vertex_count_per_meridian-1), 
                std::clamp(standard_grid_uid.x, 0, vertex_count_per_meridian-1)
            );
            return memory_id;
        }
        constexpr Tid memory_id(const vec2 grid_position) const {
            const vec2 world_position = (grid_position + half_cell) / vertex_count_per_half_meridian_float;
            const vec2 standardized_world_position = tesselation.standardize(world_position);
            const vec2 standard_grid_position = standardized_world_position * vertex_count_per_half_meridian_float - half_cell;

            const ivec2 standard_grid_uid = ivec2(glm::round(standard_grid_position)) + vertex_count_per_half_meridian;
            const Tid memory_id = interleaving.interleaved_id(
                std::clamp(standard_grid_uid.y, 0, vertex_count_per_meridian-1), 
                std::clamp(standard_grid_uid.x, 0, vertex_count_per_meridian-1)
            );
            return memory_id;
        }

        inline constexpr ivec2 grid_id(const Tid memory_id) const {
            return ivec2 (interleaving.element_id(memory_id), interleaving.block_id(memory_id)) - vertex_count_per_half_meridian;
        }


        inline constexpr ivec2 grid_id(const vec2 grid_position) const
        {
            return grid_position;
        }
        inline constexpr ivec2 grid_id(const vec3 sphere_position) const
        {
            return glm::round(tesselation.sphere_to_tesselation(glm::normalize(sphere_position)) * vertex_count_per_half_meridian_float - half_cell) + epsilon_for_integer_cast;
        }



        inline constexpr vec2 grid_position(const Tid memory_id) const {
            return vec2 (interleaving.element_id(memory_id), interleaving.block_id(memory_id)) - vertex_count_per_half_meridian_float;
        }
        inline constexpr vec2 grid_position(const ivec2 grid_id) const
        {
            return grid_id;
        }
        inline constexpr vec2 grid_position(const vec3 sphere_position) const
        {
            return tesselation.sphere_to_tesselation(glm::normalize(sphere_position)) * vertex_count_per_half_meridian_float - half_cell;
        }


        inline constexpr vec3 sphere_position(const ivec2 grid_id) const
        {
            return tesselation.tesselation_to_sphere((vec2(grid_id) + half_cell) / vertex_count_per_half_meridian_float) * radius;
        }
        inline constexpr vec3 sphere_position(const vec2 grid_position) const
        {
            return tesselation.tesselation_to_sphere((grid_position + half_cell) / vertex_count_per_half_meridian_float) * radius;
        }


        // inline constexpr si::length<vec3> physical_position(const ivec2 grid_id) const
        // {
        //     return tesselation.tesselation_to_physical((vec2(grid_id) + half_cell) / vertex_count_per_half_meridian_float) * radius;
        // }
        // inline constexpr si::length<vec3> physical_position(const vec2 grid_position) const
        // {
        //     return tesselation.tesselation_to_physical((grid_position + half_cell) / vertex_count_per_half_meridian_float) * radius;
        // }

    };
}