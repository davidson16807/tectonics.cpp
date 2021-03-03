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

#include <rasters/components/Structure/Structure.hpp>

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
    template<typename Tid = std::uint16_t, typename Tfloat = float>
    struct Metrics
    {
        /*
        This stores the content of `vertex_positions` in a format that can be accepted by OpenGL
        This is used every frame to map vertex aligned data to a format that can be ingested by OpenGL programs,
        so it pays to have a precomputed version.
        */
        series::Series<Tfloat>                           flattened_face_vertex_coordinates;

        series::Series<glm::vec<3,Tfloat,glm::defaultp>> vertex_positions;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> vertex_normals;
        series::Series<Tfloat>                           vertex_areas;
        Tfloat                                           vertex_average_area;

        series::Series<glm::vec<3,Tfloat,glm::defaultp>> face_endpoint_a;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> face_endpoint_b;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> face_endpoint_c;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> face_midpoints;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> face_normals;
        series::Series<Tfloat>                           face_areas;
        Tfloat                                           face_average_area;

        series::Series<glm::vec<3,Tfloat,glm::defaultp>> edge_endpoint_a;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> edge_endpoint_b;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> edge_midpoints;
        series::Series<Tfloat>                           edge_lengths;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> edge_normals;
        //series::Series<Tfloat>                         edge_areas;
        Tfloat                                           edge_average_length;
        
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> arrow_endpoint_from;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> arrow_endpoint_to;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> arrow_midpoints;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> arrow_offsets;
        series::Series<Tfloat>                           arrow_lengths;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> arrow_normals;
        //series::Series<Tfloat>                         arrow_areas;
        Tfloat                                           arrow_average_length;

        series::Series<Tfloat>                           vertex_dual_areas;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> arrow_dual_endpoint_a;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> arrow_dual_endpoint_b;
        series::Series<Tfloat>                           arrow_dual_lengths;
        series::Series<glm::vec<3,Tfloat,glm::defaultp>> arrow_dual_normals;

        Tfloat                                           total_area;


        ~Metrics()
        {

        }
    private:
        Metrics(const Tid vertex_count, const Tid face_count, const Tid edge_count)
            : 
                flattened_face_vertex_coordinates(3*3*face_count),

                vertex_positions       (vertex_count),
                vertex_normals         (vertex_count),
                vertex_areas           (vertex_count),
                vertex_average_area    (0),

                face_endpoint_a        (face_count),
                face_endpoint_b        (face_count),
                face_endpoint_c        (face_count),
                face_midpoints         (face_count),
                face_normals           (face_count),
                face_areas             (face_count),
                face_average_area      (0),

                edge_endpoint_a        (edge_count),
                edge_endpoint_b        (edge_count),
                edge_midpoints         (edge_count),
                edge_lengths           (edge_count),
                edge_normals           (edge_count),
            //  edge_areas             (edge_count),
                edge_average_length    (0),
                
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
                arrow_dual_normals     (2*edge_count),

                total_area             (0)
        {

        }

    public:
        template <typename Tfloat2>
        explicit Metrics(const series::Series<glm::vec<3,Tfloat2,glm::defaultp>>& vertices, const Structure<Tid,Tfloat>& structure):
            Metrics(structure.vertex_count, structure.face_count, structure.edge_count)
        {
            copy(vertex_positions, vertices);
            
            series::Series<glm::vec<3,Tfloat,glm::defaultp>>      flattened_face_vertex_positions(3*structure.face_count);
            get     (vertex_positions, structure.flattened_face_vertex_ids, flattened_face_vertex_positions   );
            flatten (flattened_face_vertex_positions,                       flattened_face_vertex_coordinates );

            get     (vertex_positions, structure.face_vertex_id_a,    face_endpoint_a);
            get     (vertex_positions, structure.face_vertex_id_b,    face_endpoint_b);
            get     (vertex_positions, structure.face_vertex_id_c,    face_endpoint_c);
            face_midpoints = (face_endpoint_a + face_endpoint_b + face_endpoint_c) / Tfloat(3);

            face_areas     = length   (cross(face_endpoint_c - face_endpoint_b, face_endpoint_a - face_endpoint_b)) / Tfloat(2); 
            // ^^^ NOTE: the magnitude of cross product is the area of a parallelogram, so half that is the area of a triangle
            face_average_area = mean(face_areas);
            total_area = sum(face_areas);

            face_normals   = normalize(cross(face_endpoint_c - face_endpoint_b, face_endpoint_a - face_endpoint_b)); 
            face_normals  *= sign(dot(face_normals, normalize(face_midpoints)));
            // ^^^ NOTE: we correct by the sign of the cosine similarity of normals and midpoints.
            // This way, the face normals will be somewhat standardized to face outward.
            // This will hold for most well centered convex shapes. 

            series::Series<Tfloat> face_vertex_areas_a = length(cross((face_endpoint_c - face_endpoint_a)/Tfloat(2), (face_endpoint_b - face_endpoint_a)/Tfloat(2))) / Tfloat(2); 
            series::Series<Tfloat> face_vertex_areas_b = length(cross((face_endpoint_a - face_endpoint_b)/Tfloat(2), (face_endpoint_c - face_endpoint_b)/Tfloat(2))) / Tfloat(2); 
            series::Series<Tfloat> face_vertex_areas_c = length(cross((face_endpoint_b - face_endpoint_c)/Tfloat(2), (face_endpoint_a - face_endpoint_c)/Tfloat(2))) / Tfloat(2); 
            // ^^^ NOTE: these 3 represent the surface area of the face that lies within a vertex's region of influence
            aggregate_into(face_vertex_areas_a, structure.face_vertex_id_a, [](Tfloat a, Tfloat b){ return a+b; }, vertex_areas);
            aggregate_into(face_vertex_areas_b, structure.face_vertex_id_b, [](Tfloat a, Tfloat b){ return a+b; }, vertex_areas);
            aggregate_into(face_vertex_areas_c, structure.face_vertex_id_c, [](Tfloat a, Tfloat b){ return a+b; }, vertex_areas);
            vertex_average_area = mean(vertex_areas);

            series::Series<glm::vec<3,Tfloat,glm::defaultp>> face_area_weighted_normals = face_normals * face_areas;
            aggregate_into(face_area_weighted_normals, structure.face_vertex_id_a, [](glm::vec<3,Tfloat,glm::defaultp> a, glm::vec<3,Tfloat,glm::defaultp> b){ return a+b; }, vertex_normals);
            aggregate_into(face_area_weighted_normals, structure.face_vertex_id_b, [](glm::vec<3,Tfloat,glm::defaultp> a, glm::vec<3,Tfloat,glm::defaultp> b){ return a+b; }, vertex_normals);
            aggregate_into(face_area_weighted_normals, structure.face_vertex_id_c, [](glm::vec<3,Tfloat,glm::defaultp> a, glm::vec<3,Tfloat,glm::defaultp> b){ return a+b; }, vertex_normals);
            normalize(vertex_normals, vertex_normals);

            //  edge_average_length  = 0.0f;
            //  arrow_average_length = 0.0f;

            get     (vertex_positions,   structure.edge_vertex_id_a, edge_endpoint_a );
            get     (vertex_positions,   structure.edge_vertex_id_b, edge_endpoint_b );
            distance(edge_endpoint_a,    edge_endpoint_b,            edge_lengths    );
            add     (edge_endpoint_a,    edge_endpoint_b,            edge_midpoints  ); 
            div     (edge_midpoints,     Tfloat(2),                  edge_midpoints  );
            add     (get(vertex_normals, structure.edge_vertex_id_b), 
                     get(vertex_normals, structure.edge_vertex_id_a), 
                                                           edge_normals    ); 
            normalize(edge_normals,                        edge_normals    );
            edge_average_length = mean(edge_lengths);

            get     (vertex_positions,   structure.arrow_vertex_id_from,arrow_endpoint_from);
            get     (vertex_positions,   structure.arrow_vertex_id_to,  arrow_endpoint_to  );
            distance(arrow_endpoint_from,arrow_endpoint_to,   arrow_lengths      );
            sub     (arrow_endpoint_to,  arrow_endpoint_from, arrow_offsets      );
            add     (arrow_endpoint_to,  arrow_endpoint_from, arrow_midpoints    );
            div     (arrow_midpoints,    Tfloat(2),           arrow_midpoints    );
            add     (get(vertex_normals, structure.arrow_vertex_id_from), 
                     get(vertex_normals, structure.arrow_vertex_id_to), 
                                                              arrow_normals      ); 
            normalize(arrow_normals,                          arrow_normals      );
            arrow_average_length = mean(arrow_lengths);

            series::Series<glm::vec<3,Tfloat,glm::defaultp>>  arrow_dual_offset_a    (2*structure.edge_count);
            series::Series<glm::vec<3,Tfloat,glm::defaultp>>  arrow_dual_offset_b    (2*structure.edge_count);
            series::Series<glm::vec<3,Tfloat,glm::defaultp>>  arrow_dual_offset_cross(2*structure.edge_count);
            series::Series<glm::vec<3,Tfloat,glm::defaultp>>  arrow_dual_offset_ab   (2*structure.edge_count);
            series::Series<Tfloat>                            arrow_dual_areas       (2*structure.edge_count);

            get     (face_midpoints,         structure.arrow_face_id_a,       arrow_dual_endpoint_a   );
            get     (face_midpoints,         structure.arrow_face_id_b,       arrow_dual_endpoint_b   );
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
            aggregate_into(arrow_dual_areas, structure.arrow_vertex_id_from, [](Tfloat a, Tfloat b) -> Tfloat { return a+b; }, vertex_dual_areas);

        }
    };

    template<typename Tid, typename Tfloat>
    void scale(const Metrics<Tid,Tfloat>& input, const Tfloat length_factor, Metrics<Tid,Tfloat>& output){

        const Tfloat invariant_factor = Tfloat(1.0);
        const Tfloat area_factor = length_factor * length_factor;

        mult(input.flattened_face_vertex_coordinates, length_factor, output.flattened_face_vertex_coordinates);

        mult(input.vertex_positions,      length_factor,    output.vertex_positions     );
        mult(input.vertex_normals,        invariant_factor, output.vertex_normals       );
        mult(input.vertex_areas,          area_factor,      output.vertex_areas         );
        mult(input.vertex_average_area,   area_factor,      output.vertex_average_area  );

        mult(input.face_endpoint_a,       length_factor,    output.face_endpoint_a      );
        mult(input.face_endpoint_b,       length_factor,    output.face_endpoint_b      );
        mult(input.face_endpoint_c,       length_factor,    output.face_endpoint_c      );
        mult(input.face_midpoints,        length_factor,    output.face_midpoints       );
        mult(input.face_normals,          invariant_factor, output.face_normals         );
        mult(input.face_areas,            area_factor,      output.face_areas           );
        mult(input.face_average_area,     area_factor,      output.face_average_area    );

        mult(input.edge_endpoint_a,       length_factor,    output.edge_endpoint_a      );
        mult(input.edge_endpoint_b,       length_factor,    output.edge_endpoint_b      );
        mult(input.edge_midpoints,        length_factor,    output.edge_midpoints       );
        mult(input.edge_lengths,          length_factor,    output.edge_lengths         );
        mult(input.edge_normals,          invariant_factor, output.edge_normals         );
    //  edge_areas
        mult(input.edge_average_length,   length_factor,    output.edge_average_length  );
        
        mult(input.arrow_endpoint_from,   length_factor,    output.arrow_endpoint_from  );
        mult(input.arrow_endpoint_to,     length_factor,    output.arrow_endpoint_to    );
        mult(input.arrow_midpoints,       length_factor,    output.arrow_midpoints      );
        mult(input.arrow_offsets,         length_factor,    output.arrow_offsets        );
        mult(input.arrow_lengths,         length_factor,    output.arrow_lengths        );
        mult(input.arrow_normals,         invariant_factor, output.arrow_normals        );
    //  arrow_areas
        mult(input.arrow_average_length,  length_factor,    output.arrow_average_length );

        mult(input.vertex_dual_areas,     area_factor,      output.vertex_dual_areas    );
        mult(input.arrow_dual_endpoint_a, length_factor,    output.arrow_dual_endpoint_a);
        mult(input.arrow_dual_endpoint_b, length_factor,    output.arrow_dual_endpoint_b);
        mult(input.arrow_dual_lengths,    length_factor,    output.arrow_dual_lengths   );
        mult(input.arrow_dual_normals,    invariant_factor, output.arrow_dual_normals   );

        mult(input.total_area,            area_factor,      output.total_area           );
    }
}