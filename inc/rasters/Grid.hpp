#pragma once

#include <memory>         // std::unique_ptr
#include <unordered_set>  // std::unordered_set
#include <vector>         // std::vector
//#include <iostream>     // std::cout

#include <glm/vec2.hpp>          // *vec2
#include <glm/vec3.hpp>          // *vec3
#include <many/types.hpp>    // floats, etc.
#include <many/common.hpp>       // max
#include <many/convenience.hpp>       // sign
#include <many/statistic.hpp>    // mean
#include <many/glm/types.hpp>// *vec*s
#include <many/glm/convenience.hpp>       // dot
#include <many/glm/geometric.hpp>// cross, dot, etc.

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>           // unordered_set<vec*>

#include "SpheroidGridVoronoi.hpp"

namespace rasters {
	namespace {
		using namespace glm;
		using namespace many;
	}

	// The Grid class is the one stop shop for high performance grid cell operations
	// You can find grid cells by neighbor, by position, and by the index of a WebGL buffer array
	// All raster functionality depends on it.
	class Grid
	{
	public:

		// Precomputed map between buffer array ids and grid cell ids
		// This helps with mapping cells within the model to buffer arrays in three.js
		// Map is created by flattening this.parameters.faces
		uints 		buffer_array_vertex_ids;

		//ivecNs 	vertex_neighbor_ids;
		uint        vertex_count;
		uints 		vertex_neighbor_counts;
		vec3s 		vertex_positions;
		vec3s 		vertex_normals;
		floats 		vertex_areas;
		float		vertex_average_area;

		uint        face_count;
		uvec3s 		face_vertex_ids;
		uints 		face_vertex_id_a;
		uints 		face_vertex_id_b;
		uints 		face_vertex_id_c;
		//uints 	face_edge_id_a;
		//uints 	face_edge_id_b;
		//uints 	face_edge_id_c;
		vec3s 		face_endpoint_a;
		vec3s 		face_endpoint_b;
		vec3s 		face_endpoint_c;
		vec3s 		face_midpoints;
		vec3s 		face_normals;
		floats 		face_areas;
		float 		face_average_area;

		uint        edge_count;
		uvec2s 		edge_vertex_ids;
		uints 		edge_vertex_id_a;
		uints 		edge_vertex_id_b;
		uvec2s	    edge_face_ids;
		uints 	    edge_face_id_a;
		uints 	    edge_face_id_b;
		vec3s 		edge_endpoint_a;
		vec3s 		edge_endpoint_b;
		vec3s 		edge_midpoints;
		floats 		edge_lengths;
		vec3s 		edge_normals;
		//floats 	edge_areas;
		float		edge_average_length;
		
		uint        arrow_count;
		uvec2s 		arrow_vertex_ids;
		uints 		arrow_vertex_id_from;
		uints 		arrow_vertex_id_to;
		uvec2s 		arrow_face_ids;
		uints 		arrow_face_id_a;
		uints 		arrow_face_id_b;
		vec3s 		arrow_endpoint_from;
		vec3s 		arrow_endpoint_to;
		vec3s 		arrow_midpoints;
		vec3s 		arrow_offsets;
		floats 		arrow_lengths;
		vec3s 		arrow_normals;
		//floats 	arrow_areas;
		float		arrow_average_length;

		floats 		vertex_dual_areas;
		vec3s       arrow_dual_endpoint_a;
		vec3s       arrow_dual_endpoint_b;
		floats      arrow_dual_lengths;
		vec3s       arrow_dual_normals;

		~Grid()
		{

		}
		Grid(const unsigned int vertex_count, const unsigned int face_count, const unsigned int edge_count)
			: 
			  	buffer_array_vertex_ids(3*face_count),

			//	vertex_neighbor_ids    (0),
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
			//	face_edge_ids          (face_count),
			//	face_edge_id_a         (face_count),
			//	face_edge_id_b         (face_count),
			//	face_edge_id_c         (face_count),
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
			  	edge_lengths         (edge_count),
			  	edge_normals           (edge_count),
			//	edge_areas             (edge_count),
			  	edge_average_length  (0),
			  	
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
			  	arrow_lengths        (2*edge_count), 
			  	arrow_normals          (2*edge_count),
			//	arrow_areas            (0),
			  	arrow_average_length   (0),

				vertex_dual_areas      (vertex_count),
				arrow_dual_endpoint_a  (2*edge_count),
				arrow_dual_endpoint_b  (2*edge_count),
				arrow_dual_lengths     (2*edge_count),
				arrow_dual_normals     (2*edge_count)
		{

		}

		explicit Grid(const vec3s& vertices, const uvec3s& faces)
			: Grid(vertices.size(), faces.size(), 0)
		{
			if (faces.size() < 1)
			{
				throw std::out_of_range("cannot initialize a Grid with no faces");
			}

			copy(vertex_positions, vertices);
			copy(face_vertex_ids,  faces);
			for (unsigned int i=0, i3=0; i<faces.size(); i++, i3+=3) 
			{
				buffer_array_vertex_ids[i3+0] = faces[i].x;
				buffer_array_vertex_ids[i3+1] = faces[i].y;
				buffer_array_vertex_ids[i3+2] = faces[i].z;
			};

			get_x 	(faces, 								face_vertex_id_a);
			get_y 	(faces, 								face_vertex_id_b);
			get_z 	(faces, 								face_vertex_id_c);
			get 	(vertex_positions, face_vertex_id_a, 	face_endpoint_a);
			get 	(vertex_positions, face_vertex_id_b, 	face_endpoint_b);
			get 	(vertex_positions, face_vertex_id_c, 	face_endpoint_c);
			face_midpoints = (face_endpoint_a + face_endpoint_b + face_endpoint_c) / 3.f;

			face_areas     = length   (cross(face_endpoint_c - face_endpoint_b, face_endpoint_a - face_endpoint_b)) / 2.f; 
			// ^^^ NOTE: the magnitude of cross product is the area of a parallelogram, so half that is the area of a triangle
			face_average_area = mean(face_areas);

			face_normals   = normalize(cross(face_endpoint_c - face_endpoint_b, face_endpoint_a - face_endpoint_b)); 
			face_normals  *= sign(dot(face_normals, normalize(face_midpoints)));
			// ^^^ NOTE: we correct by the sign of the cosine similarity of normals and midpoints.
			// This way, the face normals will be somewhat standardized to face outward.
			// This will hold for most well centered convex shapes. 

			floats face_vertex_areas_a = length(cross((face_endpoint_c - face_endpoint_a)/2.f, (face_endpoint_b - face_endpoint_a)/2.f)) / 2.f; 
			floats face_vertex_areas_b = length(cross((face_endpoint_a - face_endpoint_b)/2.f, (face_endpoint_c - face_endpoint_b)/2.f)) / 2.f; 
			floats face_vertex_areas_c = length(cross((face_endpoint_b - face_endpoint_c)/2.f, (face_endpoint_a - face_endpoint_c)/2.f)) / 2.f; 
			// ^^^ NOTE: these 3 represent the surface area of the face that lies within a vertex's region of influence
			aggregate_into(face_vertex_areas_a, face_vertex_id_a, [](float a, float b){ return a+b; }, vertex_areas);
			aggregate_into(face_vertex_areas_b, face_vertex_id_b, [](float a, float b){ return a+b; }, vertex_areas);
			aggregate_into(face_vertex_areas_c, face_vertex_id_c, [](float a, float b){ return a+b; }, vertex_areas);
			vertex_average_area = mean(vertex_areas);

			vec3s face_area_weighted_normals = face_normals * face_areas;
			aggregate_into(face_area_weighted_normals, face_vertex_id_a, [](vec3 a, vec3 b){ return a+b; }, vertex_normals);
			aggregate_into(face_area_weighted_normals, face_vertex_id_b, [](vec3 a, vec3 b){ return a+b; }, vertex_normals);
			aggregate_into(face_area_weighted_normals, face_vertex_id_c, [](vec3 a, vec3 b){ return a+b; }, vertex_normals);
			normalize(vertex_normals, vertex_normals);

			// Step 1: generate arrows from faces
			std::unordered_set<uvec2> arrow_vertex_ids_set;
			std::unordered_map<uvec2, std::unordered_set<uint>> arrow_face_ids_map;
			for (uint i=0; i<faces.size(); i++)
			{
				arrow_vertex_ids_set.insert(uvec2(faces[i].x, faces[i].y));
				arrow_vertex_ids_set.insert(uvec2(faces[i].y, faces[i].x));
				arrow_face_ids_map.emplace(uvec2(faces[i].x, faces[i].y), std::unordered_set<uint>()).first->second.insert(i);
				arrow_face_ids_map.emplace(uvec2(faces[i].y, faces[i].x), std::unordered_set<uint>()).first->second.insert(i);

				arrow_vertex_ids_set.insert(uvec2(faces[i].x, faces[i].z));
				arrow_vertex_ids_set.insert(uvec2(faces[i].z, faces[i].x));
				arrow_face_ids_map.emplace(uvec2(faces[i].x, faces[i].z), std::unordered_set<uint>()).first->second.insert(i);
				arrow_face_ids_map.emplace(uvec2(faces[i].z, faces[i].x), std::unordered_set<uint>()).first->second.insert(i);

				arrow_vertex_ids_set.insert(uvec2(faces[i].y, faces[i].z));
				arrow_vertex_ids_set.insert(uvec2(faces[i].z, faces[i].y));
				arrow_face_ids_map.emplace(uvec2(faces[i].y, faces[i].z), std::unordered_set<uint>()).first->second.insert(i);
				arrow_face_ids_map.emplace(uvec2(faces[i].z, faces[i].y), std::unordered_set<uint>()).first->second.insert(i);
			}
		    // sort arrows using a vector to avoid cache misses when retrieving indices
			std::copy(
				arrow_vertex_ids_set.begin(), 
				arrow_vertex_ids_set.end(), 
				std::back_inserter(arrow_vertex_ids)
			);
		    std::sort(
		    	arrow_vertex_ids.begin(), 
		    	arrow_vertex_ids.end(), 
		    	[](uvec2 a, uvec2 b) 
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
				std::back_inserter(edge_vertex_ids), 
				[](uvec2 a){return a.y > a.x;}
			);

			edge_count = edge_vertex_ids.size();

		  	edge_vertex_id_a       .resize(edge_count);
		  	edge_vertex_id_b       .resize(edge_count);
			edge_face_ids          .resize(edge_count);
			edge_face_id_a         .resize(edge_count);
			edge_face_id_b         .resize(edge_count);
		  	edge_endpoint_a        .resize(edge_count);
		  	edge_endpoint_b        .resize(edge_count);
		  	edge_midpoints         .resize(edge_count);
		  	edge_lengths           .resize(edge_count);
		  	edge_normals           .resize(edge_count);
		//	edge_areas             .resize(edge_count);
		//  edge_average_length  = 0.0f;
		  	
			arrow_count = arrow_vertex_ids.size();

		  	arrow_vertex_id_from   .resize(2*edge_count);
		  	arrow_vertex_id_to     .resize(2*edge_count);
			arrow_face_ids         .resize(2*edge_count);
			arrow_face_id_a        .resize(2*edge_count);
			arrow_face_id_b        .resize(2*edge_count);
		  	arrow_endpoint_from    .resize(2*edge_count);
		  	arrow_endpoint_to      .resize(2*edge_count);
		  	arrow_midpoints        .resize(2*edge_count);
		  	arrow_offsets          .resize(2*edge_count);
		  	arrow_lengths          .resize(2*edge_count); 
		  	arrow_normals          .resize(2*edge_count);
		//	arrow_areas            .resize(0);
		//  arrow_average_length = 0.0f;

		  	arrow_dual_endpoint_a  .resize(2*edge_count);
		  	arrow_dual_endpoint_b  .resize(2*edge_count);
		  	arrow_dual_lengths     .resize(2*edge_count);
		  	arrow_dual_normals     .resize(2*edge_count);

	    	// generate arrow_face_ids and edge_face_ids
		  	std::unordered_set<uint> face_ids_set;
			for (uint i=0; i<edge_vertex_ids.size(); i++)
	    	{
	    		face_ids_set = arrow_face_ids_map[edge_vertex_ids[i]];
	    		if (face_ids_set.size() == 2)
	    		{
	    			edge_face_ids[i] = uvec2(*(face_ids_set.begin()), *std::next(face_ids_set.begin()));
	    		}
	    	}
			for (uint i=0; i<arrow_vertex_ids.size(); i++)
	    	{
	    		face_ids_set = arrow_face_ids_map[arrow_vertex_ids[i]];
	    		if (face_ids_set.size() == 2)
	    		{
	    			arrow_face_ids[i] = uvec2(*(face_ids_set.begin()), *std::next(face_ids_set.begin()));
	    		}
	    	}

			get_x	(edge_vertex_ids,                      edge_vertex_id_a);
			get_y	(edge_vertex_ids,                      edge_vertex_id_b);
			get_x	(edge_face_ids,                        edge_face_id_a  );
			get_y	(edge_face_ids,                        edge_face_id_b  );
			get 	(vertex_positions,   edge_vertex_id_a, edge_endpoint_a );
			get 	(vertex_positions,   edge_vertex_id_b, edge_endpoint_b );
			distance(edge_endpoint_a,    edge_endpoint_b,  edge_lengths    );
			add 	(edge_endpoint_a,    edge_endpoint_b,  edge_midpoints  ); edge_midpoints /= 2.f;
			add 	(get(vertex_normals, edge_vertex_id_b), 
				     get(vertex_normals, edge_vertex_id_a), 
				     									   edge_normals    ); edge_normals /= 2.f;
			edge_average_length = mean(edge_lengths);

			get_x 	(arrow_vertex_ids,                        arrow_vertex_id_from); 
			get_y 	(arrow_vertex_ids,                        arrow_vertex_id_to );
			get_x	(arrow_face_ids,                          arrow_face_id_a    );
			get_y	(arrow_face_ids,                          arrow_face_id_b    );
			get 	(vertex_positions,   arrow_vertex_id_from,arrow_endpoint_from);
			get 	(vertex_positions,   arrow_vertex_id_to,  arrow_endpoint_to  );
			distance(arrow_endpoint_from,arrow_endpoint_to,   arrow_lengths      );
			sub 	(arrow_endpoint_to,  arrow_endpoint_from, arrow_offsets      );
			add 	(arrow_endpoint_to,  arrow_endpoint_from, arrow_midpoints    ); arrow_midpoints /= 2.f;
			add 	(get(vertex_normals, arrow_vertex_id_from), 
				     get(vertex_normals, arrow_vertex_id_to), 
				     										  arrow_normals 	 ); arrow_normals /= 2.f;
			arrow_average_length = mean(arrow_lengths);

			vec3s  arrow_dual_offset_a    (2*edge_count);
			vec3s  arrow_dual_offset_b    (2*edge_count);
			vec3s  arrow_dual_offset_cross(2*edge_count);
			floats arrow_dual_areas       (2*edge_count);

			normalize(arrow_offsets,                                arrow_dual_normals      );
			get     (face_midpoints,         arrow_face_id_a,       arrow_dual_endpoint_a   );
			get     (face_midpoints,         arrow_face_id_b,       arrow_dual_endpoint_b   );
			distance(arrow_dual_endpoint_a,  arrow_dual_endpoint_b, arrow_dual_lengths      );
			sub		(arrow_dual_endpoint_a,  arrow_endpoint_from,   arrow_dual_offset_a     );
			sub		(arrow_dual_endpoint_b,  arrow_endpoint_from,   arrow_dual_offset_b     );
			cross   (arrow_dual_offset_a,    arrow_dual_offset_b,   arrow_dual_offset_cross );
			length  (arrow_dual_offset_cross,                       arrow_dual_areas        );
			div     (arrow_dual_areas,       2.f,                   arrow_dual_areas        );
			fill    (vertex_dual_areas,      0.f);
			aggregate_into(arrow_dual_areas, arrow_vertex_id_from, [](float a, float b) -> float { return a+b; }, vertex_dual_areas);

			fill    (vertex_neighbor_counts, 0u);
			aggregate_into(arrow_vertex_id_from, [](uint a){ return a+1; }, vertex_neighbor_counts);
		}
	};
}