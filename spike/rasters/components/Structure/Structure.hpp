#pragma once

#include <unordered_set>  // std::unordered_set
#include <vector>         // std::vector

#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3
#include <series/types.hpp>           // floats, etc.
#include <series/arithmetic.hpp>      // add, sub, mult, div, etc.
#include <series/common.hpp>          // max
#include <series/statistic.hpp>       // mean
#include <series/glm/types.hpp>       // *vec*s
#include <series/glm/geometric.hpp>   // cross, dot, etc.
#include <series/glm/convenience.hpp> // dot
#include <series/convenience.hpp>     // in place add, sub, mult, div, etc.
#include <series/operators.hpp>       // +, -, *, /

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>           // unordered_set<vec*>

namespace rasters 
{

    /*
    A "grid" is a collection of interconnected grid cells on the surface of 
    an object that are intended to store values in a Raster. 
    It may be represented by an underlying mesh in which grid cells are 
    represented by vertices, or there may be a more complex relationship, 
    where a vertex in a mesh corresponds to several grid cells 
    (see `LayeredGrid` for an example). 
    A grid caches additional attributes that may be used when 
    performing spatially aware operations on rasters. 
    These additional attributes are derived from vertices and faces. 
    In this way, a grid could be thought of as a "mesh cache"
    */
    template<typename id = std::uint16_t>
    struct Structure
    {
        using ivec3 = glm::vec<3,id,glm::defaultp>;
        using ivec2 = glm::vec<2,id,glm::defaultp>;

        /*
        Flattened version of `face_vertex_ids`,
        This is often used to map vertex aligned data to a format that can be ingested by OpenGL programs,
        so it pays to have a precomputed version.
        */
        series::Series<id>    flattened_face_vertex_ids;

        //ivecNs              vertex_neighbor_ids;
        const std::size_t     vertex_count;
        series::Series<id>    vertex_neighbor_counts;

        const std::size_t     face_count;
        series::Series<ivec3> face_vertex_ids;
        series::Series<id>    face_vertex_id_a;
        series::Series<id>    face_vertex_id_b;
        series::Series<id>    face_vertex_id_c;
        //series::Series<id>  face_edge_id_a;
        //series::Series<id>  face_edge_id_b;
        //series::Series<id>  face_edge_id_c;

        std::size_t           edge_count;
        series::Series<ivec2> edge_vertex_ids;
        series::Series<id>    edge_vertex_id_a;
        series::Series<id>    edge_vertex_id_b;
        series::Series<ivec2> edge_face_ids;
        series::Series<id>    edge_face_id_a;
        series::Series<id>    edge_face_id_b;
        
        std::size_t           arrow_count;
        series::Series<ivec2> arrow_vertex_ids;
        series::Series<id>    arrow_vertex_id_from;
        series::Series<id>    arrow_vertex_id_to;
        series::Series<ivec2> arrow_face_ids;
        series::Series<id>    arrow_face_id_a;
        series::Series<id>    arrow_face_id_b;

        ~Structure()
        {

        }
    private:
        Structure(const std::size_t vertex_count, const std::size_t face_count, const std::size_t edge_count)
            : 
                flattened_face_vertex_ids        (3*face_count),

            //  vertex_neighbor_ids    (0),
                vertex_count           (vertex_count),
                vertex_neighbor_counts (vertex_count),

                face_count             (face_count),
                face_vertex_ids        (face_count),
                face_vertex_id_a       (face_count),
                face_vertex_id_b       (face_count),
                face_vertex_id_c       (face_count),
            //  face_edge_ids          (face_count),
            //  face_edge_id_a         (face_count),
            //  face_edge_id_b         (face_count),
            //  face_edge_id_c         (face_count),

                edge_count             (edge_count),
                edge_vertex_ids        (edge_count),
                edge_vertex_id_a       (edge_count),
                edge_vertex_id_b       (edge_count),
                edge_face_ids          (edge_count),
                edge_face_id_a         (edge_count),
                edge_face_id_b         (edge_count),
                
                arrow_count            (2*edge_count),
                arrow_vertex_ids       (2*edge_count),
                arrow_vertex_id_from   (2*edge_count),
                arrow_vertex_id_to     (2*edge_count),
                arrow_face_ids         (2*edge_count),
                arrow_face_id_a        (2*edge_count),
                arrow_face_id_b        (2*edge_count)

        {

        }

    public:
        explicit Structure(const std::size_t vertex_count, const series::Series<glm::vec<3,id, glm::defaultp>>& faces):
            Structure(vertex_count, faces.size(), 0)
        {
            copy(face_vertex_ids,  faces);
            
            flatten (face_vertex_ids,                             flattened_face_vertex_ids         );

            get_x   (face_vertex_ids,                       face_vertex_id_a);
            get_y   (face_vertex_ids,                       face_vertex_id_b);
            get_z   (face_vertex_ids,                       face_vertex_id_c);

            // Step 1: generate arrows from faces
            std::unordered_set<ivec2> arrow_vertex_ids_set;
            std::unordered_map<ivec2, std::unordered_set<id>> arrow_face_ids_map;
            for (std::size_t i=0; i<face_vertex_ids.size(); i++)
            {
                arrow_vertex_ids_set.insert(ivec2(face_vertex_ids[i].x, face_vertex_ids[i].y));
                arrow_vertex_ids_set.insert(ivec2(face_vertex_ids[i].y, face_vertex_ids[i].x));
                arrow_face_ids_map.emplace(ivec2(face_vertex_ids[i].x, face_vertex_ids[i].y), std::unordered_set<id>()).first->second.insert(i);
                arrow_face_ids_map.emplace(ivec2(face_vertex_ids[i].y, face_vertex_ids[i].x), std::unordered_set<id>()).first->second.insert(i);

                arrow_vertex_ids_set.insert(ivec2(face_vertex_ids[i].x, face_vertex_ids[i].z));
                arrow_vertex_ids_set.insert(ivec2(face_vertex_ids[i].z, face_vertex_ids[i].x));
                arrow_face_ids_map.emplace(ivec2(face_vertex_ids[i].x, face_vertex_ids[i].z), std::unordered_set<id>()).first->second.insert(i);
                arrow_face_ids_map.emplace(ivec2(face_vertex_ids[i].z, face_vertex_ids[i].x), std::unordered_set<id>()).first->second.insert(i);

                arrow_vertex_ids_set.insert(ivec2(face_vertex_ids[i].y, face_vertex_ids[i].z));
                arrow_vertex_ids_set.insert(ivec2(face_vertex_ids[i].z, face_vertex_ids[i].y));
                arrow_face_ids_map.emplace(ivec2(face_vertex_ids[i].y, face_vertex_ids[i].z), std::unordered_set<id>()).first->second.insert(i);
                arrow_face_ids_map.emplace(ivec2(face_vertex_ids[i].z, face_vertex_ids[i].y), std::unordered_set<id>()).first->second.insert(i);
            }
            // sort arrows using a vector to avoid cache misses when retrieving indices
            std::copy(
                arrow_vertex_ids_set.begin(), 
                arrow_vertex_ids_set.end(), 
                std::back_inserter(arrow_vertex_ids.vector())
            );
            std::sort(
                arrow_vertex_ids.begin(), 
                arrow_vertex_ids.end(), 
                [](ivec2 a, ivec2 b) 
                { 
                    return std::min(a.x,a.y)  < std::min(b.x,b.y) || 
                          (std::min(a.x,a.y) == std::min(b.x,b.y) && std::max(a.x,a.y) < std::max(b.x,b.y)); 
                }
            );

            // Step 2: generate edges from arrows,
            //  an arrow should only become an edge if y > x
            std::copy_if (
                arrow_vertex_ids.begin(), 
                arrow_vertex_ids.end(), 
                std::back_inserter(edge_vertex_ids.vector()), 
                [](ivec2 a){return a.y > a.x;}
            );

            edge_count = edge_vertex_ids.size();

            edge_vertex_id_a       .vector().resize(edge_count);
            edge_vertex_id_b       .vector().resize(edge_count);
            edge_face_ids          .vector().resize(edge_count);
            edge_face_id_a         .vector().resize(edge_count);
            edge_face_id_b         .vector().resize(edge_count);
            
            arrow_count = arrow_vertex_ids.size();

            arrow_vertex_id_from   .vector().resize(2*edge_count);
            arrow_vertex_id_to     .vector().resize(2*edge_count);
            arrow_face_ids         .vector().resize(2*edge_count);
            arrow_face_id_a        .vector().resize(2*edge_count);
            arrow_face_id_b        .vector().resize(2*edge_count);


            // generate arrow_face_ids and edge_face_ids
            std::unordered_set<id> face_ids_set;
            for (std::size_t i=0; i<edge_vertex_ids.size(); i++)
            {
                face_ids_set = arrow_face_ids_map[edge_vertex_ids[i]];
                if (face_ids_set.size() == 2)
                {
                    edge_face_ids[i] = ivec2(*(face_ids_set.begin()), *std::next(face_ids_set.begin()));
                }
            }
            for (std::size_t i=0; i<arrow_vertex_ids.size(); i++)
            {
                face_ids_set = arrow_face_ids_map[arrow_vertex_ids[i]];
                if (face_ids_set.size() == 2)
                {
                    arrow_face_ids[i] = ivec2(*(face_ids_set.begin()), *std::next(face_ids_set.begin()));
                }
            }

            get_x   (edge_vertex_ids,                      edge_vertex_id_a);
            get_y   (edge_vertex_ids,                      edge_vertex_id_b);
            get_x   (edge_face_ids,                        edge_face_id_a  );
            get_y   (edge_face_ids,                        edge_face_id_b  );

            get_x   (arrow_vertex_ids,                        arrow_vertex_id_from); 
            get_y   (arrow_vertex_ids,                        arrow_vertex_id_to );
            get_x   (arrow_face_ids,                          arrow_face_id_a    );
            get_y   (arrow_face_ids,                          arrow_face_id_b    );

            fill    (vertex_neighbor_counts, id(0));
            aggregate_into(arrow_vertex_id_from, [](id a){ return a+1; }, vertex_neighbor_counts);
        }
    };
}