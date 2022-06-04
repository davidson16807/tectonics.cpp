#pragma once

#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3

namespace rasters 
{
    /*
    */
    template<typename Tid=std::uint16_t, typename Tfloat=float>
    struct SpheroidMeshMetrics
    {
        ~SpheroidMeshMetrics()
        {

        }
        SpheroidMeshMetrics() 
        {
        }

        glm::vec<3,Tfloat,glm::defaultp> vertex_normal(const glm::vec<3,Tfloat,glm::defaultp> vertex) const
        {
            return glm::normalize(vertex);
        }

        glm::vec<3,Tfloat,glm::defaultp> edge_normal(const glm::vec<3,Tfloat,glm::defaultp> a, const glm::vec<3,Tfloat,glm::defaultp> b) const
        {
            return glm::normalize((a + b) / Tfloat(2));
        }

        glm::vec<3,Tfloat,glm::defaultp> face_normal(const glm::vec<3,Tfloat,glm::defaultp> a, const glm::vec<3,Tfloat,glm::defaultp> b, const glm::vec<3,Tfloat,glm::defaultp> c) const
        {
            return glm::normalize(glm::cross(c - b, a - b));
        }



        Tfloat face_area(const glm::vec<3,Tfloat,glm::defaultp> a, const glm::vec<3,Tfloat,glm::defaultp> b, const glm::vec<3,Tfloat,glm::defaultp> c) const
        {
            return glm::length(glm::cross(c - b, a - b)) / Tfloat(2);
        }



        glm::vec<3,Tfloat,glm::defaultp> edge_midpoint(const glm::vec<3,Tfloat,glm::defaultp> a, const glm::vec<3,Tfloat,glm::defaultp> b) const
        {
            return (a + b) / Tfloat(2);
        }

        glm::vec<3,Tfloat,glm::defaultp> face_midpoint(const glm::vec<3,Tfloat,glm::defaultp> a, const glm::vec<3,Tfloat,glm::defaultp> b, const glm::vec<3,Tfloat,glm::defaultp> c) const
        {
            return (a + b + c)/Tfloat(3);
        }


        glm::vec<3,Tfloat,glm::defaultp> face_border_normal(
            const glm::vec<3,Tfloat,glm::defaultp> interior_vertex, 
            const glm::vec<3,Tfloat,glm::defaultp> exterior_vertex_1, 
            const glm::vec<3,Tfloat,glm::defaultp> exterior_vertex_2
        ) const {
            // calculate the triple product to get the vector at right angles to the outer edge and surface normal
            const glm::vec<3,Tfloat,glm::defaultp> triple = glm::normalize(
                glm::cross(exterior_vertex_2 - exterior_vertex_1, 
                    glm::cross(exterior_vertex_1 - interior_vertex, exterior_vertex_2 - interior_vertex)
                )
            );
            // the triple product may point towards or away from the interior vertex, 
            // so run a check to make sure the vector points away from the interior vertex
            return glm::dot(triple, exterior_vertex_1 - interior_vertex) > Tfloat(0)? triple : -triple;
        }

    };
}