#pragma once

#include <vector>         	  	 // std::vector
#include <unordered_map>         // std::unordered_map
#include <algorithm>             // std::min

#include <glm/vec3.hpp>       	 // *vec3
#include <many/types.hpp>     	 // floats, etc.
#include <many/glm/types.hpp>	 // *vec*s
#include <many/glm/geometric.hpp>// normalize, etc.

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>  // map<vec*, *>

namespace rasters 
{
	namespace meshes
	{
		namespace 
		{
			float t((1.0 + sqrt(5.0)) / 2.0);

			template<glm::length_t L, typename T, glm::qualifier Q>
			inline many::tmany<glm::vec<L,T,Q>> normalize(const many::tmany<glm::vec<L,T,Q>>& u) 
			{
				many::tmany<glm::vec<L,T,Q>> out = many::tmany<glm::vec<L,T,Q>>(u.size());
				many::normalize(u, out);
				return out;
			}
		}
		/*
		"mesh" is a simple data structure that stores the minimum data needed to represent a well formed 3d mesh
		It consists only of a list of vertices and edges.
		It is accompanied by a minimal list of functions needed to manipulate mesh structures for use on spherical grids
		(namely just functions to generate and subdivide regular polyhedra)
		*/
		struct mesh
		{
			many::vec3s  vertices;
			many::uvec3s faces;

			~mesh()
			{

			}
			mesh(const many::vec3s& vertices, const many::uvec3s& faces)
				: vertices(vertices),
				  faces(faces)
			{
				if (vertices.size() < 1)
				{
					throw std::out_of_range("cannot initialize a mesh with no vertices");
				}
				if (faces.size() < 1)
				{
					throw std::out_of_range("cannot initialize a mesh with no faces");
				}
			}
		};

		const mesh icosahedron(
				meshes::normalize(many::vec3s({
						glm::vec3(-1, t, 0),
						glm::vec3( 1, t, 0),
						glm::vec3(-1,-t, 0),
						glm::vec3( 1,-t, 0),

						glm::vec3( 0,-1, t),
						glm::vec3( 0, 1, t),
						glm::vec3( 0,-1,-t),
						glm::vec3( 0, 1,-t),

						glm::vec3( t, 0,-1),
						glm::vec3( t, 0, 1),
						glm::vec3(-t, 0,-1),
						glm::vec3(-t, 0, 1)
					})),
				many::uvec3s({
						// 5 faces around point 0
						glm::uvec3(0, 11, 5),
						glm::uvec3(0, 5, 1),
						glm::uvec3(0, 1, 7),
						glm::uvec3(0, 7, 10),
						glm::uvec3(0, 10, 11),

						// 5 adjacent faces
						glm::uvec3(1, 5, 9),
						glm::uvec3(5, 11, 4),
						glm::uvec3(11, 10, 2),
						glm::uvec3(10, 7, 6),
						glm::uvec3(7, 1, 8),

						// 5 faces around point 3
						glm::uvec3(3, 9, 4),
						glm::uvec3(3, 4, 2),
						glm::uvec3(3, 2, 6),
						glm::uvec3(3, 6, 8),
						glm::uvec3(3, 8, 9),

						// 5 adjacent faces
						glm::uvec3(4, 9, 5),
						glm::uvec3(2, 4, 11),
						glm::uvec3(6, 2, 10),
						glm::uvec3(8, 6, 7),
						glm::uvec3(9, 8, 1)
					})
			);

		const mesh octahedron(
		        meshes::normalize(many::vec3s({
		                glm::vec3( 1, 0, 0),
		                glm::vec3(-1, 0, 0),
		                glm::vec3( 0, 1, 0),
		                glm::vec3( 0,-1, 0),
		                glm::vec3( 0, 0, 1),
		                glm::vec3( 0, 0,-1),
		            })),
		        many::uvec3s({
		                glm::uvec3( 0, 2, 4),
		                glm::uvec3( 0, 2, 5),
		                glm::uvec3( 0, 3, 4),
		                glm::uvec3( 0, 3, 5),
		                glm::uvec3( 1, 2, 4),
		                glm::uvec3( 1, 2, 5),
		                glm::uvec3( 1, 3, 4),
		                glm::uvec3( 1, 3, 5)
		            })
			);

		const mesh tetrahedron(
		        many::vec3s({
		                glm::vec3(0,0,0),
		                glm::vec3(1,0,0),
		                glm::vec3(0,1,0),
		                glm::vec3(0,0,1)
		            }),
		        many::uvec3s({
		                glm::uvec3(0,1,2),
		                glm::uvec3(0,1,3),
		                glm::uvec3(0,2,3),
		                glm::uvec3(1,2,3)
		            })
			);

		namespace
		{	
			inline glm::uvec3 eid(uint a, uint b, uint i)
			{
				return glm::uvec3(std::min(a,b), std::max(a,b), i);
			}
			inline glm::uvec3 vid(
				std::unordered_map<glm::uvec2, uint>& vmap,
				std::unordered_map<glm::uvec3, uint>& emap,
				std::unordered_map<uint,       uint>& fmap,
				glm::uvec3 face, 
				uint i, 
				uint j
				uint n
			){
				if (i==0 && j==0)
				{
					return fmap[face.x];
				} 
				if (i==n && j==0)
				{
					return fmap[face.y];
				} 
				if (i==n && j==n)
				{
					return fmap[face.z];
				} 
				if (j==0)
				{
					return emap[ glm::uvec3(std::min(face.x,face.y), std::max(face.x,face.y), i) ];
				}
				if (i==n)
				{
					return emap[ glm::uvec3(std::min(face.y,face.z), std::max(face.y,face.z), j) ];
				}
				if (i==j)
				{
					return emap[ glm::uvec3(std::min(face.x,face.z), std::max(face.x,face.z), i) ];
				}
				else 
				{
					return vmap[glm::uvec2(i,j)]
				}
			}
		}

		mesh subdivide(const mesh& input, const uint subdivisions_per_edge = 1)
		{
			const uint vertices_per_edge(subdivisions_per_edge + 2);
			const float n(vertices_per_edge-1);

			std::vector<glm::vec3>  vertices;
			std::vector<glm::uvec3> faces;

			// "vmap" is a map for vertices that are not placed on edges or corners
			std::unordered_map<glm::uvec2, uint> vmap {};
			//"emap" is a map for vertices that are placed against the edges of faces
			// edge vertices must be shared between faces so a special lookup is needed
			std::unordered_map<glm::uvec3, uint> emap {};
			//"fmap" is a map for vertices that are placed at corners of faces
			// face vertices must be shared between several edges so a special lookup is needed
			std::unordered_map<uint,       uint> fmap {};
			
			glm::uvec3 face(0,0,0);
			for (uint face_id(0); face_id < input.faces.size(); ++face_id)
			{

				face = input.faces[face_id];

				uint ai = face.x;
				uint bi = face.y;
				uint ci = face.z;

				uint a = input.vertices[ai];
				uint b = input.vertices[bi];
				uint c = input.vertices[ci];

				vmap.clear();

				// POPULATE VERTICES
				if (fmap.find(ai) == fmap.end())
				{
					// corner vertex
					fmap.find(ai) = vertices.size();
					vertices.push_back(a);
				}
				for (uint i = 1; i < n; ++i)
				{
					if (emap.find(eid(ai,bi,i)) == emap.end())
					{
						// edge vertices, j=0
						emap[eid(ai,bi,i)] = vertices.size();
						vertices.push_back(a + (b-a)*i/n + (c-b)*0/n);
					}

					for (uint j = 1; j < i; ++j)
					{
						// center vertices
						vmap[uvec2(i,j)] = vertices.size();
						vertices.push_back(a + (b-a)*i/n + (c-b)*j/n);
					}

					if (emap.find(eid(ai,ci,i)) == emap.end())
					{
						// edge vertices, i=j
						emap[eid(ai,ci,i)] = vertices.size();
						vertices.push_back(a + (b-a)*i/n + (c-b)*i/n);
					}
				}
				if (fmap.find(bi) == fmap.end())
				{
					// corner vertex
					fmap.find(bi) = vertices.size();
					vertices.push_back(b);
				}
				for (uint j = 1, ; j < n; ++j)
				{
					if (emap.find(eid(bi,ci,j)) == emap.end())
					{
						// edge vertices, i=vertices_per_edge-1
						emap[eid(bi,ci,j)] = vertices.size();
						vertices.push_back(a + (b-a)*n/n + (c-b)*j/n);
					}
				}
				if (fmap.find(ci) == fmap.end())
				{
					// corner vertex
					fmap.find(ci) = vertices.size();
					vertices.push_back(c);
				}


				// POPULATE FACES
				for (uint i = 0; i < vertices_per_edge-1; ++i)
				{
					for (uint j = 0; j < i; ++j)
					{
						// TODO: fix this
						face = input.faces[face_id];
						faces.emplace_back(vid(emap, vmap, face, i,  j),   vid(emap, vmap, face, i+1,j), vid(emap, vmap, face, i,j+1));
						faces.emplace_back(vid(emap, vmap, face, i+1,j+1), vid(emap, vmap, face, i+1,j), vid(emap, vmap, face, i,j+1));
					}
						faces.emplace_back(vid(emap, vmap, face, i,  i),   vid(emap, vmap, face, i+1,i), vid(emap, vmap, face, i,i+1));
				}
			}
			return mesh(many::vec3s(vertices), many::uvec3s(faces));
		}
	}
}


