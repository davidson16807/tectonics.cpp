#pragma once
#include <series/arithmetic.hpp>
#include <series/glm/geometric.hpp>
#include "../Grid/Raster.hpp"

namespace rasters
{
    /*
    properties used:
        metrics->arrow_dual_lengths
        metrics->arrow_dual_normals
        metrics->arrow_lengths
        metrics->vertex_dual_areas
        structure->arrow_vertex_ids
        structure->arrow_vertex_id_from
    */

    /*
    NOTE: See VectorCalculus.md for more explanation!
    */
    template<typename T, typename Tgrid, glm::qualifier Q>
    void gradient(
        const Raster<T, Tgrid>& scalar_field, 
        Raster<glm::vec<3,T,Q>, Tgrid>& out, 
        Raster<T, Tgrid, mapping::arrow>& arrow_differential, 
        Raster<glm::vec<3,T,Q>, Tgrid, mapping::arrow>& arrow_flow
    ) {
        const auto metrics = scalar_field.grid.metrics;
        const auto structure = scalar_field.grid.structure;
        glm::uvec2 arrow;
        for (std::size_t i = 0; i < structure->arrow_vertex_ids.size(); ++i)
        {
            arrow                 = structure->arrow_vertex_ids[i]; 
            arrow_differential[i] = scalar_field[arrow.y] - scalar_field[arrow.x]; // differential across dual of the arrow
        }
        each::mult   (arrow_differential, metrics->arrow_dual_normals,   arrow_flow);      // flux across dual of the arrow
        each::mult   (arrow_flow,         metrics->arrow_dual_lengths,   arrow_flow);      // flow across dual of the arrow
        each::fill   (out,                glm::vec<3,T,Q>(0.f));
        each::aggregate_into(arrow_flow,  structure->arrow_vertex_id_from, [](glm::vec<3,T,Q> a, glm::vec<3,T,Q> b){ return a+b; }, out); // flow out from the vertex
        each::div      (out,              metrics->vertex_dual_areas,    out);             // gradient
    }

    template<typename T, typename Tgrid, glm::qualifier Q>
    void divergence(
        const Raster<glm::vec<3,T,Q>, Tgrid>& vector_field, 
        Raster<T, Tgrid>& out, 
        Raster<glm::vec<3,T,Q>, Tgrid, mapping::arrow>& arrow_differential, 
        Raster<T, Tgrid, mapping::arrow>& arrow_projection
    ) {
        const auto metrics = vector_field.grid.metrics;
        const auto structure = vector_field.grid.structure;
        glm::uvec2 arrow;
        for (std::size_t i = 0; i < structure->arrow_vertex_ids.size(); ++i)
        {
            arrow                 = structure->arrow_vertex_ids[i]; 
            arrow_differential[i] = vector_field[arrow.y] - vector_field[arrow.x]; // differential across dual of the arrow
        }
        each::dot    (arrow_differential, metrics->arrow_dual_normals,   arrow_projection);      // flux across dual of the arrow
        each::mult   (arrow_projection,   metrics->arrow_dual_lengths,   arrow_projection);      // flow across dual of the arrow
        each::fill   (out,                0.f);
        each::aggregate_into(arrow_projection, structure->arrow_vertex_id_from, [](T a, T b){ return a+b; }, out);  // flow out from the vertex
        each::div    (out,                metrics->vertex_dual_areas,    out);             // divergence
    }


    template<typename T, typename Tgrid, glm::qualifier Q>
    void curl(
        const Raster<glm::vec<3,T,Q>, Tgrid>& vector_field, 
        Raster<glm::vec<3,T,Q>, Tgrid>& out, 
        Raster<glm::vec<3,T,Q>, Tgrid, mapping::arrow>& arrow_differential, 
        Raster<glm::vec<3,T,Q>, Tgrid, mapping::arrow>& arrow_rejection
    ) {
        const auto metrics = vector_field.grid.metrics;
        const auto structure = vector_field.grid.structure;
        glm::uvec2 arrow;
        for (std::size_t i = 0; i < structure->arrow_vertex_ids.size(); ++i)
        {
            arrow                 = structure->arrow_vertex_ids[i]; 
            arrow_differential[i] = vector_field[arrow.y] - vector_field[arrow.x]; // differential across dual of the arrow
        }
        each::cross      (arrow_differential, metrics->arrow_dual_normals,   arrow_rejection);      // flux across dual of the arrow
        each::mult   (arrow_rejection,    metrics->arrow_dual_lengths,   arrow_rejection);      // flow across dual of the arrow
        each::fill   (out,                glm::vec<3,T,Q>(0.f));
        each::aggregate_into(arrow_rejection, structure->arrow_vertex_id_from, [](glm::vec<3,T,Q> a, glm::vec<3,T,Q> b){ return a+b; }, out);  // flow out from the vertex
        each::div    (out,                metrics->vertex_dual_areas,    out);             // curl
    }


    /*
    NOTE: See VectorCalculus.md for more explanation!
    */
    template<typename T, typename Tgrid>
    void laplacian(
        const Raster<T, Tgrid>& scalar_field, 
        Raster<T, Tgrid>& out, 
        Raster<T, Tgrid, mapping::arrow>& arrow_scratch
    ) {
        const auto metrics = scalar_field.grid.metrics;
        const auto structure = scalar_field.grid.structure;
        glm::uvec2 arrow;
        for (std::size_t i = 0; i < structure->arrow_vertex_ids.size(); ++i)
        {
            arrow            = structure->arrow_vertex_ids[i]; 
            arrow_scratch[i] = scalar_field[arrow.y] - scalar_field[arrow.x]; // differential across dual of the arrow
        }
        each::div      (arrow_scratch,      metrics->arrow_lengths,      arrow_scratch); // slope along the arrow
        each::mult     (arrow_scratch,      metrics->arrow_dual_lengths, arrow_scratch); // differential weighted by dual length
        each::fill     (out,                T(0.f));
        each::aggregate_into(arrow_scratch, structure->arrow_vertex_id_from, [](T a, T b){ return a+b; }, out);  // weight average difference across neighbors
        each::div      (out,                metrics->vertex_dual_areas,    out);             // laplacian
    }

    template<unsigned int L, typename T, typename Tgrid, glm::qualifier Q>
    void laplacian(
        const Raster<glm::vec<L,T,Q>, Tgrid>& vector_field, 
        Raster<glm::vec<L,T,Q>, Tgrid>& out, 
        Raster<glm::vec<L,T,Q>, Tgrid, mapping::arrow>& arrow_scratch
    ) {
        const auto metrics = vector_field.grid.metrics;
        const auto structure = vector_field.grid.structure;
        glm::uvec2 arrow;
        for (std::size_t i = 0; i < structure->arrow_vertex_ids.size(); ++i)
        {
            arrow            = structure->arrow_vertex_ids[i]; 
            arrow_scratch[i] = vector_field[arrow.y] - vector_field[arrow.x]; // differential across dual of the arrow
        }
        each::div      (arrow_scratch,      metrics->arrow_lengths,      arrow_scratch);
        each::mult     (arrow_scratch,      metrics->arrow_dual_lengths, arrow_scratch); // differential weighted by dual length
        each::fill     (out,                glm::vec<L,T,Q>(0.f));
        each::aggregate_into(arrow_scratch, structure->arrow_vertex_id_from, [](glm::vec<L,T,Q> a, glm::vec<L,T,Q> b){ return a+b; }, out);  // weight average difference across neighbors
        each::div      (out,                metrics->vertex_dual_areas,    out);             // laplacian
    }

}

