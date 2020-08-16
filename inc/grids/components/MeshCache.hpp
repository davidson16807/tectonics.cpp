#pragma once

#include <unordered_set>  // std::unordered_set
#include <vector>         // std::vector

#include <glm/vec2.hpp>             // *vec2
#include <glm/vec3.hpp>             // *vec3
#include <many/types.hpp>           // floats, etc.
#include <many/arithmetic.hpp>      // add, sub, mult, div, etc.
#include <many/common.hpp>          // max
#include <many/convenience.hpp>     // operator overloads
#include <many/statistic.hpp>       // mean
#include <many/glm/types.hpp>       // *vec*s
#include <many/glm/convenience.hpp> // dot
#include <many/glm/geometric.hpp>   // cross, dot, etc.

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>           // unordered_set<vec*>

namespace rasters 
{

    /*
    A "grid" is a collection of interconnected grid cells on the surface of 
    an object that are intended to store values in a raster. 
    It may be represented by an underlying mesh in which grid cells are 
    represented by vertices, or there may be a more complex relationship, 
    where a vertex in a mesh corresponds to several grid cells 
    (see `LayeredGrid` for an example). 
    A grid caches additional attributes that may be used when 
    performing spatially aware operations on rasters. 
    These additional attributes are derived from vertices and faces. 
    In this way, a grid could be thought of as a "mesh cache"
    */
    template<typename Tid = std::uint16_t, typename Tfloat = float>
    struct MeshCache
    {
        /*
        Flattened version of `face_vertex_ids`,
        This is often used to map vertex aligned data to a format that can be ingested by OpenGL programs,
        so it pays to have a precomputed version.
        */
        many::series<Tid>                              flattened_face_vertex_ids;
        /*
        This stores the content of `vertex_positions` in a format that can be accepted by OpenGL
        This is used every frame to map vertex aligned data to a format that can be ingested by OpenGL programs,
        so it pays to have a precomputed version.
        */
        many::series<Tfloat>                           flattened_face_vertex_coordinates;

        //ivecNs                                       vertex_neighbor_ids;
        uint                                           vertex_count;
        many::series<Tid>                              vertex_neighbor_counts;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> vertex_positions;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> vertex_normals;
        many::series<Tfloat>                           vertex_areas;
        float                                          vertex_average_area;

        uint                                           face_count;
        many::series<glm::vec<3,Tid,glm::defaultp>>    face_vertex_ids;
        many::series<Tid>                              face_vertex_id_a;
        many::series<Tid>                              face_vertex_id_b;
        many::series<Tid>                              face_vertex_id_c;
        //many::series<Tid>                            face_edge_id_a;
        //many::series<Tid>                            face_edge_id_b;
        //many::series<Tid>                            face_edge_id_c;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> face_endpoint_a;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> face_endpoint_b;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> face_endpoint_c;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> face_midpoints;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> face_normals;
        many::series<Tfloat>                           face_areas;
        float                                          face_average_area;

        uint                                           edge_count;
        many::series<glm::vec<2,Tid,glm::defaultp>>    edge_vertex_ids;
        many::series<Tid>                              edge_vertex_id_a;
        many::series<Tid>                              edge_vertex_id_b;
        many::series<glm::vec<2,Tid,glm::defaultp>>    edge_face_ids;
        many::series<Tid>                              edge_face_id_a;
        many::series<Tid>                              edge_face_id_b;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> edge_endpoint_a;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> edge_endpoint_b;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> edge_midpoints;
        many::series<Tfloat>                           edge_lengths;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> edge_normals;
        //many::series<Tfloat>                         edge_areas;
        float                                          edge_average_length;
        
        uint                                           arrow_count;
        many::series<glm::vec<2,Tid,glm::defaultp>>    arrow_vertex_ids;
        many::series<Tid>                              arrow_vertex_id_from;
        many::series<Tid>                              arrow_vertex_id_to;
        many::series<glm::vec<2,Tid,glm::defaultp>>    arrow_face_ids;
        many::series<Tid>                              arrow_face_id_a;
        many::series<Tid>                              arrow_face_id_b;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> arrow_endpoint_from;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> arrow_endpoint_to;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> arrow_midpoints;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> arrow_offsets;
        many::series<Tfloat>                           arrow_lengths;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> arrow_normals;
        //many::series<Tfloat>                         arrow_areas;
        float                                          arrow_average_length;

        many::series<Tfloat>                           vertex_dual_areas;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> arrow_dual_endpoint_a;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> arrow_dual_endpoint_b;
        many::series<Tfloat>                           arrow_dual_lengths;
        many::series<glm::vec<3,Tfloat,glm::defaultp>> arrow_dual_normals;

        ~MeshCache()
        {

        }
        MeshCache(const Tid vertex_count, const Tid face_count, const Tid edge_count)
            : 
                flattened_face_vertex_ids        (3*face_count),
                flattened_face_vertex_coordinates(3*3*face_count),

            //  vertex_neighbor_ids    (0),
                vertex_count           (vertex_count),
                vertex_neighbor_counts (vertex_count),
                vertex_positions       (vertex_count),
                vertex_normals         (vertex_count),
                vertex_areas           (vertex_count),
                vertex_average_area    (0),

                face_count             (face_count),
                face_vertex_ids        (face_count),
                face_vertex_id_a       (face_count),
                face_vertex_id_b       (face_count),
                face_vertex_id_c       (face_count),
            //  face_edge_ids          (face_count),
            //  face_edge_id_a         (face_count),
            //  face_edge_id_b         (face_count),
            //  face_edge_id_c         (face_count),
                face_endpoint_a        (face_count),
                face_endpoint_b        (face_count),
                face_endpoint_c        (face_count),
                face_midpoints         (face_count),
                face_normals           (face_count),
                face_areas             (face_count),
                face_average_area      (0),

                edge_count             (edge_count),
                edge_vertex_ids        (edge_count),
                edge_vertex_id_a       (edge_count),
                edge_vertex_id_b       (edge_count),
                edge_face_ids          (edge_count),
                edge_face_id_a         (edge_count),
                edge_face_id_b         (edge_count),
                edge_endpoint_a        (edge_count),
                edge_endpoint_b        (edge_count),
                edge_midpoints         (edge_count),
                edge_lengths           (edge_count),
                edge_normals           (edge_count),
            //  edge_areas             (edge_count),
                edge_average_length    (0),
                
                arrow_count            (2*edge_count),
                arrow_vertex_ids       (2*edge_count),
                arrow_vertex_id_from   (2*edge_count),
                arrow_vertex_id_to     (2*edge_count),
                arrow_face_ids         (2*edge_count),
                arrow_face_id_a        (2*edge_count),
                arrow_face_id_b        (2*edge_count),
                arrow_endpoint_from    (2*edge_count),
                arrow_endpoint_to      (2*edge_count),
                arrow_midpoints        (2*edge_count),
                arrow_offsets          (2*edge_count),
                arrow_lengths          (2*edge_count), 
                arrow_normals          (2*edge_count),
            //  arrow_areas            (0),
                arrow_average_length   (0),

                vertex_dual_areas      (vertex_count),
                arrow_dual_endpoint_a  (2*edge_count),
                arrow_dual_endpoint_b  (2*edge_count),
                arrow_dual_lengths     (2*edge_count),
                arrow_dual_normals     (2*edge_count)
        {

        }

        template <typename Tid2, typename Tfloat2>
        explicit MeshCache(const many::series<glm::vec<3,Tfloat2,glm::defaultp>>& vertices, const many::series<glm::vec<3,Tid2, glm::defaultp>>& faces):
            MeshCache(vertices.size(), faces.size(), 0)
        {
            copy(vertex_positions, vertices);
            copy(face_vertex_ids,  faces);
            
            many::series<vec<3,Tfloat,glm::defaultp>> flattened_face_vertex_positions(3*face_count);
            flatten (face_vertex_ids,                             flattened_face_vertex_ids         );
            get     (vertex_positions, flattened_face_vertex_ids, flattened_face_vertex_positions   );
            flatten (flattened_face_vertex_positions,             flattened_face_vertex_coordinates );

            get_x   (face_vertex_ids,                       face_vertex_id_a);
            get_y   (face_vertex_ids,                       face_vertex_id_b);
            get_z   (face_vertex_ids,                       face_vertex_id_c);
            get     (vertex_positions, face_vertex_id_a,    face_endpoint_a);
            get     (vertex_positions, face_vertex_id_b,    face_endpoint_b);
            get     (vertex_positions, face_vertex_id_c,    face_endpoint_c);
            face_midpoints = (face_endpoint_a + face_endpoint_b + face_endpoint_c) / Tfloat(3);

            face_areas     = length   (cross(face_endpoint_c - face_endpoint_b, face_endpoint_a - face_endpoint_b)) / Tfloat(2); 
            // ^^^ NOTE: the magnitude of cross product is the area of a parallelogram, so half that is the area of a triangle
            face_average_area = mean(face_areas);

            face_normals   = normalize(cross(face_endpoint_c - face_endpoint_b, face_endpoint_a - face_endpoint_b)); 
            face_normals  *= sign(dot(face_normals, normalize(face_midpoints)));
            // ^^^ NOTE: we correct by the sign of the cosine similarity of normals and midpoints.
            // This way, the face normals will be somewhat standardized to face outward.
            // This will hold for most well centered convex shapes. 

            many::series<Tfloat> face_vertex_areas_a = length(cross((face_endpoint_c - face_endpoint_a)/Tfloat(2), (face_endpoint_b - face_endpoint_a)/Tfloat(2))) / Tfloat(2); 
            many::series<Tfloat> face_vertex_areas_b = length(cross((face_endpoint_a - face_endpoint_b)/Tfloat(2), (face_endpoint_c - face_endpoint_b)/Tfloat(2))) / Tfloat(2); 
            many::series<Tfloat> face_vertex_areas_c = length(cross((face_endpoint_b - face_endpoint_c)/Tfloat(2), (face_endpoint_a - face_endpoint_c)/Tfloat(2))) / Tfloat(2); 
            // ^^^ NOTE: these 3 represent the surface area of the face that lies within a vertex's region of influence
            aggregate_into(face_vertex_areas_a, face_vertex_id_a, [](Tfloat a, Tfloat b){ return a+b; }, vertex_areas);
            aggregate_into(face_vertex_areas_b, face_vertex_id_b, [](Tfloat a, Tfloat b){ return a+b; }, vertex_areas);
            aggregate_into(face_vertex_areas_c, face_vertex_id_c, [](Tfloat a, Tfloat b){ return a+b; }, vertex_areas);
            vertex_average_area = mean(vertex_areas);

            many::series<vec<3,Tfloat,glm::defaultp>> face_area_weighted_normals = face_normals * face_areas;
            aggregate_into(face_area_weighted_normals, face_vertex_id_a, [](glm::vec<3,Tfloat,glm::defaultp> a, glm::vec<3,Tfloat,glm::defaultp> b){ return a+b; }, vertex_normals);
            aggregate_into(face_area_weighted_normals, face_vertex_id_b, [](glm::vec<3,Tfloat,glm::defaultp> a, glm::vec<3,Tfloat,glm::defaultp> b){ return a+b; }, vertex_normals);
            aggregate_into(face_area_weighted_normals, face_vertex_id_c, [](glm::vec<3,Tfloat,glm::defaultp> a, glm::vec<3,Tfloat,glm::defaultp> b){ return a+b; }, vertex_normals);
            normalize(vertex_normals, vertex_normals);

            // Step 1: generate arrows from faces
            std::unordered_set<glm::vec<2,Tid,glm::defaultp>> arrow_vertex_ids_set;
            std::unordered_map<glm::vec<2,Tid,glm::defaultp>, std::unordered_set<Tid>> arrow_face_ids_map;
            for (Tid i=0; i<face_vertex_ids.size(); i++)
            {
                arrow_vertex_ids_set.insert(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].x, face_vertex_ids[i].y));
                arrow_vertex_ids_set.insert(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].y, face_vertex_ids[i].x));
                arrow_face_ids_map.emplace(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].x, face_vertex_ids[i].y), std::unordered_set<Tid>()).first->second.insert(i);
                arrow_face_ids_map.emplace(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].y, face_vertex_ids[i].x), std::unordered_set<Tid>()).first->second.insert(i);

                arrow_vertex_ids_set.insert(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].x, face_vertex_ids[i].z));
                arrow_vertex_ids_set.insert(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].z, face_vertex_ids[i].x));
                arrow_face_ids_map.emplace(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].x, face_vertex_ids[i].z), std::unordered_set<Tid>()).first->second.insert(i);
                arrow_face_ids_map.emplace(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].z, face_vertex_ids[i].x), std::unordered_set<Tid>()).first->second.insert(i);

                arrow_vertex_ids_set.insert(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].y, face_vertex_ids[i].z));
                arrow_vertex_ids_set.insert(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].z, face_vertex_ids[i].y));
                arrow_face_ids_map.emplace(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].y, face_vertex_ids[i].z), std::unordered_set<Tid>()).first->second.insert(i);
                arrow_face_ids_map.emplace(glm::vec<2,Tid,glm::defaultp>(face_vertex_ids[i].z, face_vertex_ids[i].y), std::unordered_set<Tid>()).first->second.insert(i);
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
                [](glm::vec<2,Tid,glm::defaultp> a, glm::vec<2,Tid,glm::defaultp> b) 
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
                [](glm::vec<2,Tid,glm::defaultp> a){return a.y > a.x;}
            );

            edge_count = edge_vertex_ids.size();

            edge_vertex_id_a       .vector().resize(edge_count);
            edge_vertex_id_b       .vector().resize(edge_count);
            edge_face_ids          .vector().resize(edge_count);
            edge_face_id_a         .vector().resize(edge_count);
            edge_face_id_b         .vector().resize(edge_count);
            edge_endpoint_a        .vector().resize(edge_count);
            edge_endpoint_b        .vector().resize(edge_count);
            edge_midpoints         .vector().resize(edge_count);
            edge_lengths           .vector().resize(edge_count);
            edge_normals           .vector().resize(edge_count);
        //  edge_areas             .vector().resize(edge_count);
        //  edge_average_length  = 0.0f;
            
            arrow_count = arrow_vertex_ids.size();

            arrow_vertex_id_from   .vector().resize(2*edge_count);
            arrow_vertex_id_to     .vector().resize(2*edge_count);
            arrow_face_ids         .vector().resize(2*edge_count);
            arrow_face_id_a        .vector().resize(2*edge_count);
            arrow_face_id_b        .vector().resize(2*edge_count);
            arrow_endpoint_from    .vector().resize(2*edge_count);
            arrow_endpoint_to      .vector().resize(2*edge_count);
            arrow_midpoints        .vector().resize(2*edge_count);
            arrow_offsets          .vector().resize(2*edge_count);
            arrow_lengths          .vector().resize(2*edge_count); 
            arrow_normals          .vector().resize(2*edge_count);
        //  arrow_areas            .vector().resize(0);
        //  arrow_average_length = 0.0f;

            arrow_dual_endpoint_a  .vector().resize(2*edge_count);
            arrow_dual_endpoint_b  .vector().resize(2*edge_count);
            arrow_dual_lengths     .vector().resize(2*edge_count);
            arrow_dual_normals     .vector().resize(2*edge_count);

            // generate arrow_face_ids and edge_face_ids
            std::unordered_set<Tid> face_ids_set;
            for (Tid i=0; i<edge_vertex_ids.size(); i++)
            {
                face_ids_set = arrow_face_ids_map[edge_vertex_ids[i]];
                if (face_ids_set.size() == 2)
                {
                    edge_face_ids[i] = glm::vec<2,Tid,glm::defaultp>(*(face_ids_set.begin()), *std::next(face_ids_set.begin()));
                }
            }
            for (Tid i=0; i<arrow_vertex_ids.size(); i++)
            {
                face_ids_set = arrow_face_ids_map[arrow_vertex_ids[i]];
                if (face_ids_set.size() == 2)
                {
                    arrow_face_ids[i] = glm::vec<2,Tid,glm::defaultp>(*(face_ids_set.begin()), *std::next(face_ids_set.begin()));
                }
            }

            get_x   (edge_vertex_ids,                      edge_vertex_id_a);
            get_y   (edge_vertex_ids,                      edge_vertex_id_b);
            get_x   (edge_face_ids,                        edge_face_id_a  );
            get_y   (edge_face_ids,                        edge_face_id_b  );
            get     (vertex_positions,   edge_vertex_id_a, edge_endpoint_a );
            get     (vertex_positions,   edge_vertex_id_b, edge_endpoint_b );
            distance(edge_endpoint_a,    edge_endpoint_b,  edge_lengths    );
            add     (edge_endpoint_a,    edge_endpoint_b,  edge_midpoints  ); 
            div     (edge_midpoints,     Tfloat(2),        edge_midpoints  );
            add     (get(vertex_normals, edge_vertex_id_b), 
                     get(vertex_normals, edge_vertex_id_a), 
                                                           edge_normals    ); 
            normalize(edge_normals,                        edge_normals    );
            edge_average_length = mean(edge_lengths);

            get_x   (arrow_vertex_ids,                        arrow_vertex_id_from); 
            get_y   (arrow_vertex_ids,                        arrow_vertex_id_to );
            get_x   (arrow_face_ids,                          arrow_face_id_a    );
            get_y   (arrow_face_ids,                          arrow_face_id_b    );
            get     (vertex_positions,   arrow_vertex_id_from,arrow_endpoint_from);
            get     (vertex_positions,   arrow_vertex_id_to,  arrow_endpoint_to  );
            distance(arrow_endpoint_from,arrow_endpoint_to,   arrow_lengths      );
            sub     (arrow_endpoint_to,  arrow_endpoint_from, arrow_offsets      );
            add     (arrow_endpoint_to,  arrow_endpoint_from, arrow_midpoints    );
            div     (arrow_midpoints,    Tfloat(2),           arrow_midpoints    );
            add     (get(vertex_normals, arrow_vertex_id_from), 
                     get(vertex_normals, arrow_vertex_id_to), 
                                                              arrow_normals      ); 
            normalize(arrow_normals,                          arrow_normals      );
            arrow_average_length = mean(arrow_lengths);

            many::series<vec<3,Tfloat,glm::defaultp>>  arrow_dual_offset_a    (2*edge_count);
            many::series<vec<3,Tfloat,glm::defaultp>>  arrow_dual_offset_b    (2*edge_count);
            many::series<vec<3,Tfloat,glm::defaultp>>  arrow_dual_offset_cross(2*edge_count);
            many::series<vec<3,Tfloat,glm::defaultp>>  arrow_dual_offset_ab   (2*edge_count);
            many::series<Tfloat> arrow_dual_areas       (2*edge_count);

            get     (face_midpoints,         arrow_face_id_a,       arrow_dual_endpoint_a   );
            get     (face_midpoints,         arrow_face_id_b,       arrow_dual_endpoint_b   );
            distance(arrow_dual_endpoint_a,  arrow_dual_endpoint_b, arrow_dual_lengths      );
            sub     (arrow_dual_endpoint_a,  arrow_endpoint_from,   arrow_dual_offset_a     );
            sub     (arrow_dual_endpoint_b,  arrow_endpoint_from,   arrow_dual_offset_b     );
            cross   (arrow_dual_offset_a,    arrow_dual_offset_b,   arrow_dual_offset_cross );
            length  (arrow_dual_offset_cross,                       arrow_dual_areas        );
            div     (arrow_dual_areas,       Tfloat(2),             arrow_dual_areas        );
            sub     (arrow_dual_endpoint_b,  arrow_dual_endpoint_a, arrow_dual_offset_ab    );

            cross   (arrow_dual_offset_ab,   arrow_normals,         arrow_dual_normals      );
            normalize(arrow_dual_normals,                           arrow_dual_normals      );
            arrow_dual_normals *= sign(dot(arrow_dual_normals, arrow_offsets));
            
            fill    (vertex_dual_areas,      Tfloat(0));
            aggregate_into(arrow_dual_areas, arrow_vertex_id_from, [](Tfloat a, Tfloat b) -> Tfloat { return a+b; }, vertex_dual_areas);

            fill    (vertex_neighbor_counts, Tid(0));
            aggregate_into(arrow_vertex_id_from, [](Tid a){ return a+1; }, vertex_neighbor_counts);
        }
    };
}