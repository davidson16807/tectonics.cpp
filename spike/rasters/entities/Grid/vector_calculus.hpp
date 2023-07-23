#pragma once
#include <series/arithmetic.hpp>
#include <series/glm/geometric.hpp>
#include "../Grid/Raster.hpp"

namespace rasters
{
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
        series::mult   (arrow_differential, metrics->arrow_dual_normals,   arrow_flow);      // flux across dual of the arrow
        series::mult   (arrow_flow,         metrics->arrow_dual_lengths,   arrow_flow);      // flow across dual of the arrow
        series::fill   (out,                glm::vec<3,T,Q>(0.f));
        series::aggregate_into(arrow_flow,  structure->arrow_vertex_id_from, [](glm::vec<3,T,Q> a, glm::vec<3,T,Q> b){ return a+b; }, out); // flow out from the vertex
        series::div      (out,              metrics->vertex_dual_areas,    out);             // gradient
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
        series::dot    (arrow_differential, metrics->arrow_dual_normals,   arrow_projection);      // flux across dual of the arrow
        series::mult   (arrow_projection,   metrics->arrow_dual_lengths,   arrow_projection);      // flow across dual of the arrow
        series::fill   (out,                0.f);
        series::aggregate_into(arrow_projection, structure->arrow_vertex_id_from, [](T a, T b){ return a+b; }, out);  // flow out from the vertex
        series::div    (out,                metrics->vertex_dual_areas,    out);             // divergence
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
        series::cross      (arrow_differential, metrics->arrow_dual_normals,   arrow_rejection);      // flux across dual of the arrow
        series::mult   (arrow_rejection,    metrics->arrow_dual_lengths,   arrow_rejection);      // flow across dual of the arrow
        series::fill   (out,                glm::vec<3,T,Q>(0.f));
        series::aggregate_into(arrow_rejection, structure->arrow_vertex_id_from, [](glm::vec<3,T,Q> a, glm::vec<3,T,Q> b){ return a+b; }, out);  // flow out from the vertex
        series::div      (out,                metrics->vertex_dual_areas,    out);             // curl
    }


    /*
    By the Gauss-Green theorem:
      ∫∫∫ᵥ ∇²ϕ ∂V = ∫∫∫ᵥ ∇⋅∇ϕ ∂V = ∫∫ₛ n̂⋅∇ϕ ∂s
    for infinitesimal volumes, we can assume that contents are uniform, so results scale with volume, and:
      ∇²ϕ = 1/V ∫∫ₛ n̂⋅∇ϕ ∂s
    Now consider a vertex surrounded by neighbors.
    For each vertex pair, indicated by i, 
    there is a change in ϕ from one vertex to another, Δϕᵢ.
    This occurs over a set distance, |Δxᵢ|,
    and across a boundary of given length, ΔSᵢ. 
    We know n̂⋅∇ϕ indicates the slope of ϕ in the direction n̂, since n̂⋅∇ϕ = xₙ∂/∂x + yₙ∂/∂x + zₙ∂/∂z. 
    So if we wanted to estimate n̂⋅∇ϕ for the region covered by a single vertex pair, we'd say it was Δϕᵢ/|Δxᵢ|. 
    And if we want to estimate ∇²ϕ for the entire vertex neighborhood, then:
      ∇²ϕ = 1/V Σᵢ Δϕᵢ/|Δxᵢ| ΔSᵢ
    And Bob's your uncle! 
    So the laplacian on an unstructured mesh is a weighted average of slope between neighbors, 
    weighted by the sections of the boundary shared between those neighbors, 
    and divided by the area that's enclosed by the boundary. 
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
        series::div      (arrow_scratch,      metrics->arrow_lengths,      arrow_scratch); // slope along the arrow
        series::mult     (arrow_scratch,      metrics->arrow_dual_lengths, arrow_scratch); // differential weighted by dual length
        series::fill     (out,                T(0.f));
        series::aggregate_into(arrow_scratch, structure->arrow_vertex_id_from, [](T a, T b){ return a+b; }, out);  // weight average difference across neighbors
        series::div      (out,                metrics->vertex_dual_areas,    out);             // laplacian
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
        series::div      (arrow_scratch,      metrics->arrow_lengths,      arrow_scratch);
        series::mult     (arrow_scratch,      metrics->arrow_dual_lengths, arrow_scratch); // differential weighted by dual length
        series::fill     (out,                glm::vec<L,T,Q>(0.f));
        series::aggregate_into(arrow_scratch, structure->arrow_vertex_id_from, [](glm::vec<L,T,Q> a, glm::vec<L,T,Q> b){ return a+b; }, out);  // weight average difference across neighbors
        series::div      (out,                metrics->vertex_dual_areas,    out);             // laplacian
    }

}

