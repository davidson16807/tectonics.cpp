#pragma once

// C libraries
#include <math.h>       // ceil, round 

// std libraries
#include <algorithm> 	// max, min, clamp
#include <vector>		// vectors
#include <array>		// arrays
#include <limits>       // infinity

// 3rd party libraries
#include <glm/vec2.hpp>       	 // *vec3
#include <glm/vec3.hpp>       	 // *vec3

// in-house libraries
#include <many/types.hpp>
#include <many/glm/types.hpp>
#include <many/glm/geometric.hpp>
#include <many/glm/convenience.hpp>

namespace rasters
{

	/*
	A "SpheroidLookup" allows for O(1) lookups on the surface of a unit sphere.
	It does this using an octahedron where each side is a 2d cartesian grid.
	*/
	template <class T>
	class SpheroidLookup
	{
	protected:
		static constexpr int OCTAHEDRON_SIDE_COUNT = 8;	// number of sides on the data cube
		many::vec3s OCTAHEDRON_SIDE_Z;
		many::vec3s OCTAHEDRON_SIDE_X;
		many::vec3s OCTAHEDRON_SIDE_Y;

		glm::ivec2 dimensions; // dimensions of the grid on each side of the data cube 
		float cell_width;
		std::vector<T> cells;

		int cell_count() const {
			return OCTAHEDRON_SIDE_COUNT * dimensions.x * dimensions.y;
		}
		int get_memory_id(const int xi2d, const int yi2d, const int side_id) const {
			return  std::clamp(side_id, 0, OCTAHEDRON_SIDE_COUNT-1) * dimensions.x * dimensions.y 
				  + std::clamp(xi2d,    0, dimensions.x-1)          * dimensions.y 
				  + std::clamp(yi2d,    0, dimensions.y-1);
		}
		int get_memory_id(const glm::ivec3 conceptual_id) const {
			return get_memory_id(conceptual_id.x, conceptual_id.y, conceptual_id.z);
		}

		glm::vec3 get_midpoint(const int xi2d, const int yi2d, const int side_id) const 
		{
			// get position of the cell that's projected onto the 2d grid
			float x2d = (float)xi2d * cell_width - 1.;
			float y2d = (float)yi2d * cell_width - 1.;
			// reconstruct the dimension omitted from the grid using pythagorean theorem
			float z2d = sqrt(std::max(1. - (x2d*x2d) - (y2d*y2d), 0.));

			return OCTAHEDRON_SIDE_X[side_id] * x2d +
				   OCTAHEDRON_SIDE_Y[side_id] * y2d +
				   OCTAHEDRON_SIDE_Z[side_id] * z2d ;
		}
		glm::vec3 get_midpoint(const glm::ivec3 conceptual_id) const 
		{
			return get_midpoint(conceptual_id.x, conceptual_id.y, conceptual_id.z);
		}
	public:
		~SpheroidLookup()
		{
		}
		explicit SpheroidLookup(
			const float cell_width,
			const T default_value
		) : 
			OCTAHEDRON_SIDE_Z(many::normalize(
				many::vec3s {
					glm::vec3(-1,-1,-1),
					glm::vec3( 1,-1,-1),
					glm::vec3(-1, 1,-1),
					glm::vec3( 1, 1,-1),
					glm::vec3(-1,-1, 1),
					glm::vec3( 1,-1, 1),
					glm::vec3(-1, 1, 1),
					glm::vec3( 1, 1, 1)
				}
			)),
			OCTAHEDRON_SIDE_X(many::normalize(
				many::cross(SpheroidLookup<T>::OCTAHEDRON_SIDE_Z, glm::vec3(0,0,1))
			)),
			OCTAHEDRON_SIDE_Y(many::normalize(
				many::cross(SpheroidLookup<T>::OCTAHEDRON_SIDE_Z, SpheroidLookup<T>::OCTAHEDRON_SIDE_X)
			)),
			dimensions((int)std::ceil(2./cell_width)+1),
			cell_width(cell_width),
			cells(cell_count(), default_value)
		{
		}
		explicit SpheroidLookup(
			const float cell_width
		) : SpheroidLookup(cell_width, T(0))
		{
		}

		glm::ivec3 get_conceptual_id(const int xi2d, const int yi2d, const int side_id) const
		{
			return glm::ivec3(xi2d, yi2d, side_id);
		}
		glm::ivec3 get_conceptual_id(const glm::vec3 point, const int side_id) const
		{
			const double x2d = glm::dot( OCTAHEDRON_SIDE_X[side_id], point );
			const double y2d = glm::dot( OCTAHEDRON_SIDE_Y[side_id], point );

			const int xi2d = (x2d + 1.) / cell_width;
			const int yi2d = (y2d + 1.) / cell_width;
			
			return glm::ivec3(xi2d, yi2d, side_id);
		}
		glm::ivec3 get_conceptual_id(const glm::vec3 point) const
		{
			const unsigned int side_id = 
			  (( point.x > 0) << 0) +
			  (( point.y > 0) << 1) +
			  (( point.z > 0) << 2) ; 
			
			return get_conceptual_id(point, side_id);
		}

		T& get_ref(const glm::ivec3 conceptual_id)
		{
			return cells[get_memory_id(conceptual_id)];
		}

		T get_value(const glm::ivec3 conceptual_id) const
		{
			return cells[get_memory_id(conceptual_id)];
		}
		T& get_ref(const glm::vec3 point)
		{
			return get_ref(get_conceptual_id(point));
		}

		T get_value(const glm::vec3 point) const
		{
			return get_value(get_conceptual_id(glm::normalize(point)));
		}

		void get_values(const many::vec3s& points, many::Series<T>& out) const
		{
			for (unsigned int i = 0; i < points.size(); ++i)
			{
				out[i] = get_value(get_conceptual_id(glm::normalize(points[i])));
			}
		}

	};



	/*
	A "SpheroidVoronoi" is an instance of "SpheroidLookup" in which
	each cell stores the index of the vertex nearest to the cell.
	*/
	class SpheroidVoronoi : public SpheroidLookup<uint>
	{
	public:
		explicit SpheroidVoronoi(
			const many::vec3s& points, 
			const float cell_width,
			const float max_vertex_distance
		  )	: 
			SpheroidLookup<uint>(cell_width, 0)
		{
			// populate a slower lookup based on a list of vectors and their ids in `points`
			std::vector<std::pair<int, float>> temp(cells.size(), std::pair<int, float>(-1, std::numeric_limits<float>::infinity()));
			int vicinity_radius = (max_vertex_distance/cell_width)/2 + 1;
			for (uint point_id = 0; point_id < points.size(); ++point_id)
			{
				glm::vec3 point(normalize(points[point_id]));
				for (uint side_id = 0; side_id < OCTAHEDRON_SIDE_COUNT; ++side_id)
				{
					if (glm::dot(OCTAHEDRON_SIDE_Z[side_id], point) < (1/sqrt(3)) - max_vertex_distance) { continue; }
					glm::ivec3 center_id = get_conceptual_id(point, side_id);
					float center_distance = glm::distance(point, get_midpoint(center_id));
					if (center_distance > max_vertex_distance) { continue; }
					for (int xi2d = -vicinity_radius; xi2d < vicinity_radius; ++xi2d)
					{
						for (int yi2d = -vicinity_radius; yi2d < vicinity_radius; ++yi2d)
						{
							glm::ivec3 offset_id = center_id + glm::ivec3(xi2d, yi2d, 0);
							glm::vec3 midpoint = get_midpoint(offset_id);
							float point_distance = glm::distance(point, midpoint);
							if (point_distance > max_vertex_distance) { continue; }
							float min_distance = temp[get_memory_id(offset_id)].second;
							if (point_distance >= min_distance) { continue; }
							temp[get_memory_id(offset_id)].first  = point_id;
							temp[get_memory_id(offset_id)].second = point_distance;
						}
					}
				}
			}
			for (uint i = 0; i < cells.size(); ++i)
			{
				cells[i] = temp[i].first;
			}
		}
	};
}