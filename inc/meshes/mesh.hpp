#pragma once

// std libraries
#include <vector>         	  	 // std::vector
#include <unordered_map>         // std::unordered_map
#include <algorithm>             // std::min

// 3rd party libraries
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>       	 // *vec3
#include <glm/gtx/hash.hpp>  // map<vec*, *>

// in-house libraries
#include <many/types.hpp>     	 // floats, etc.
#include <many/glm/types.hpp>	 // *vec*s
#include <many/glm/geometric.hpp>// normalize, etc.


namespace meshes
{
	namespace 
	{
		const float PHI((1.0 + sqrt(5.0)) / 2.0);

		template<glm::length_t L, typename T, glm::qualifier Q>
		inline many::Series<glm::vec<L,T,Q>> normalize(const many::Series<glm::vec<L,T,Q>>& u) 
		{
			many::Series<glm::vec<L,T,Q>> out = many::Series<glm::vec<L,T,Q>>(u.size());
			many::normalize(u, out);
			return out;
		}
		inline glm::uvec2 edge_id(uint a, uint b)
		{
			return glm::uvec2(std::min(a,b), std::max(a,b));
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
					glm::vec3(-1, PHI, 0),
					glm::vec3( 1, PHI, 0),
					glm::vec3(-1,-PHI, 0),
					glm::vec3( 1,-PHI, 0),

					glm::vec3( 0,-1, PHI),
					glm::vec3( 0, 1, PHI),
					glm::vec3( 0,-1,-PHI),
					glm::vec3( 0, 1,-PHI),

					glm::vec3( PHI, 0,-1),
					glm::vec3( PHI, 0, 1),
					glm::vec3(-PHI, 0,-1),
					glm::vec3(-PHI, 0, 1)
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

	mesh subdivide(const mesh& input)
	{
		std::vector<glm::vec3>  vertices;
		std::vector<glm::uvec3> faces;
		std::unordered_map<glm::uvec2, unsigned int> midpoints {};
		copy(input.vertices.begin(), input.vertices.end(), back_inserter(vertices));
		
		glm::uvec3 face(0,0,0);
		for (unsigned int i(0); i < input.faces.size(); ++i)
		{
			face = input.faces[i];

			if (midpoints.find(edge_id(face.x, face.y)) == midpoints.end())
			{
				midpoints[edge_id(face.x, face.y)] = vertices.size();
				vertices.push_back((vertices[face.x]+vertices[face.y])/2.f);
			}
			if (midpoints.find(edge_id(face.y, face.z)) == midpoints.end())
			{
				midpoints[edge_id(face.y, face.z)] = vertices.size();
				vertices.push_back((vertices[face.y]+vertices[face.z])/2.f);
			}
			if (midpoints.find(edge_id(face.z, face.x)) == midpoints.end())
			{
				midpoints[edge_id(face.z, face.x)] = vertices.size();
				vertices.push_back((vertices[face.z]+vertices[face.x])/2.f);
			}

			faces.emplace_back(face.x,                             midpoints[edge_id(face.x, face.y)], midpoints[edge_id(face.x, face.z)]);
			faces.emplace_back(face.y,                             midpoints[edge_id(face.y, face.x)], midpoints[edge_id(face.y, face.z)]);
			faces.emplace_back(face.z,                             midpoints[edge_id(face.z, face.x)], midpoints[edge_id(face.z, face.y)]);
			faces.emplace_back(midpoints[edge_id(face.x, face.y)], midpoints[edge_id(face.y, face.z)], midpoints[edge_id(face.z, face.x)]);
		}
		return mesh(many::vec3s(vertices), many::uvec3s(faces));
	}
}


