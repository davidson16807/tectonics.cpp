#pragma once

// 3rd party libraries
#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

// in-house libraries
#include <rasters/components/CollignonMesh/CollignonTesselation.hpp>
#include <rasters/components/Interleaving.hpp>

namespace rasters 
{

    /*
    `CollignonMesh` is a wrapper around a `CollignonTesselation` 
    that introduces the concepts of sphere radius, vertex count, and boundary alignment. That is all.
    */
    template<typename Tid, typename Tfloat>
    class CollignonMesh
    {
    private:
        CollignonTesselation<Tfloat> tesselation;
        Interleaving<Tid> interleaving;
        const Tfloat epsilon_for_integer_cast = 0.1; // ensures that integer casts do not produce numbers that are one value lower than expected

    public:
        const Tid vertex_count_per_half_meridian;
        const Tid vertex_count_per_meridian;
        const Tid vertex_count;
        const Tfloat vertex_count_per_half_meridian_float;
        const Tfloat half_cell = 0.5;
        const Tfloat radius;

        ~CollignonMesh()
        {

        }
        CollignonMesh(const Tfloat radius, const Tid vertex_count_per_half_meridian) : 
            tesselation(CollignonProjection<Tfloat>()),
            interleaving(2*vertex_count_per_half_meridian),
            vertex_count_per_half_meridian(vertex_count_per_half_meridian),
            vertex_count_per_meridian(2*vertex_count_per_half_meridian),
            vertex_count(4*vertex_count_per_half_meridian*vertex_count_per_half_meridian),
            vertex_count_per_half_meridian_float(vertex_count_per_half_meridian),
            radius(radius)
        {
        }

        Tid memory_id(const glm::vec<2,Tid,glm::defaultp> grid_id) const {
            const glm::vec<2,Tfloat,glm::defaultp> world_position = (grid_position + half_cell) / vertex_count_per_half_meridian_float;
            const glm::vec<2,Tfloat,glm::defaultp> standardized_world_position = tesselation.standardize(world_position);
            const glm::vec<2,Tfloat,glm::defaultp> standard_grid_position = standardized_world_position * vertex_count_per_half_meridian_float - half_cell;

            const glm::vec<2,Tid,glm::defaultp> standard_grid_uid = glm::vec<2,Tid,glm::defaultp>(glm::round(standard_grid_position)) + vertex_count_per_half_meridian;
            const Tid memory_id = interleaving.interleaved_id(
                std::clamp(standard_grid_uid.y, 0, vertex_count_per_meridian-1), 
                std::clamp(standard_grid_uid.x, 0, vertex_count_per_meridian-1)
            );
            return memory_id;
        }
        Tid memory_id(const glm::vec<2,Tfloat,glm::defaultp> grid_position) const {
            const glm::vec<2,Tfloat,glm::defaultp> world_position = (grid_position + half_cell) / vertex_count_per_half_meridian_float;
            const glm::vec<2,Tfloat,glm::defaultp> standardized_world_position = tesselation.standardize(world_position);
            const glm::vec<2,Tfloat,glm::defaultp> standard_grid_position = standardized_world_position * vertex_count_per_half_meridian_float - half_cell;

            const glm::vec<2,Tid,glm::defaultp> standard_grid_uid = glm::vec<2,Tid,glm::defaultp>(glm::round(standard_grid_position)) + vertex_count_per_half_meridian;
            const Tid memory_id = interleaving.interleaved_id(
                std::clamp(standard_grid_uid.y, 0, vertex_count_per_meridian-1), 
                std::clamp(standard_grid_uid.x, 0, vertex_count_per_meridian-1)
            );
            return memory_id;
        }

        glm::vec<2,Tid,glm::defaultp> grid_id(const Tid memory_id) const {
            return glm::vec<2,Tid,glm::defaultp> (interleaving.element_id(memory_id), interleaving.block_id(memory_id)) - vertex_count_per_half_meridian;
        }


        glm::vec<2,Tid,glm::defaultp> grid_id(const glm::vec<2,Tfloat,glm::defaultp> grid_position) const
        {
            return grid_position;
        }
        glm::vec<2,Tid,glm::defaultp> grid_id(const glm::vec<3,Tfloat,glm::defaultp> sphere_position) const
        {
            return glm::round(tesselation.sphere_to_tesselation(glm::normalize(sphere_position)) * vertex_count_per_half_meridian_float - half_cell) + epsilon_for_integer_cast;
        }



        glm::vec<2,Tfloat,glm::defaultp> grid_position(const Tid memory_id) const {
            return glm::vec<2,Tfloat,glm::defaultp> (interleaving.element_id(memory_id), interleaving.block_id(memory_id)) - vertex_count_per_half_meridian_float;
        }
        glm::vec<2,Tfloat,glm::defaultp> grid_position(const glm::vec<2,Tid,glm::defaultp> grid_id) const
        {
            return grid_id;
        }
        glm::vec<2,Tfloat,glm::defaultp> grid_position(const glm::vec<3,Tfloat,glm::defaultp> sphere_position) const
        {
            return tesselation.sphere_to_tesselation(glm::normalize(sphere_position)) * vertex_count_per_half_meridian_float - half_cell;
        }


        glm::vec<3,Tfloat,glm::defaultp> sphere_position(const glm::vec<2,Tid,glm::defaultp> grid_id) const
        {
            return tesselation.tesselation_to_sphere((glm::vec<2,Tfloat,glm::defaultp>(grid_id) + half_cell) / vertex_count_per_half_meridian_float) * radius;
        }
        glm::vec<3,Tfloat,glm::defaultp> sphere_position(const glm::vec<2,Tfloat,glm::defaultp> grid_position) const
        {
            return tesselation.tesselation_to_sphere((grid_position + half_cell) / vertex_count_per_half_meridian_float) * radius;
        }

    };
}