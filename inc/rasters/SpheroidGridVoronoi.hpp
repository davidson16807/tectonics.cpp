#pragma once

#include <math.h>       // ceil, round 
#include <algorithm> 	// max, min, clamp
#include <vector>		// vectors
#include <array>		// arrays
#include <iostream>		// cout

#include <many/types.hpp>
#include <many/glm/types.hpp>
#include <many/glm/geometric.hpp>

namespace rasters
{
	namespace
	{
		using namespace glm;
		using namespace many;
	}

	// performs O(1) lookups on the surface of a unit sphere 
	// using an octahedron where each side is a 2d cartesian grid
	template <class T>
	class SpheroidGridLookup
	{
	protected:
		static const vec3s OCTAHEDRON_SIDE_Z;
		static const vec3s OCTAHEDRON_SIDE_X;
		static const vec3s OCTAHEDRON_SIDE_Y;
		static constexpr int OCTAHEDRON_SIDE_COUNT = 8;	// number of sides on the data cube

		ivec2 dimensions; // dimensions of the grid on each side of the data cube 
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
		int get_memory_id(const ivec3 conceptual_id) const {
			return get_memory_id(conceptual_id.x, conceptual_id.y, conceptual_id.z);
		}

		vec3 get_midpoint(const int xi2d, const int yi2d, const int side_id) const 
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
		vec3 get_midpoint(const ivec3 conceptual_id) const 
		{
			return get_midpoint(conceptual_id.x, conceptual_id.y, conceptual_id.z);
		}
	public:
		~SpheroidGridLookup()
		{
		}
		explicit SpheroidGridLookup(
			const float min_cell_width
		) : 
			dimensions((int)ceil(2./min_cell_width)+1),
			cell_width(min_cell_width),
			cells(cell_count(), T(0))
		{
		}

		ivec3 get_conceptual_id(const int xi2d, const int yi2d, const int side_id) const
		{
			return ivec3(xi2d, yi2d, side_id);
		}
		ivec3 get_conceptual_id(const vec3 point, const int side_id) const
		{
			const double x2d = dot( OCTAHEDRON_SIDE_X[side_id], point );
			const double y2d = dot( OCTAHEDRON_SIDE_Y[side_id], point );

			const int xi2d = (x2d + 1.) / cell_width;
			const int yi2d = (y2d + 1.) / cell_width;
			
			return ivec3(xi2d, yi2d, side_id);
		}
		ivec3 get_conceptual_id(const vec3 point) const
		{
			const unsigned int side_id = 
			  (( point.x > 0) << 0) +
			  (( point.y > 0) << 1) +
			  (( point.z > 0) << 2) ; 
			
			return get_conceptual_id(point, side_id);
		}

		T& get_ref(const ivec3 conceptual_id)
		{
			return cells[get_memory_id(conceptual_id)];
		}

		T get_value(const ivec3 conceptual_id) const
		{
			return cells[get_memory_id(conceptual_id)];
		}
		T& get_ref(const vec3 point)
		{
			return get_ref(get_conceptual_id(point));
		}

		T get_value(const vec3 point) const
		{
			return get_value(get_conceptual_id(point));
		}

		void get_values(const vec3s& points, tmany<T>& out) const
		{
			for (unsigned int i = 0; i < points.size(); ++i)
			{
				out[i] = get_value(get_conceptual_id(points[i]));
			}
		}

	};


	template <class T>
	const vec3s SpheroidGridLookup<T>::OCTAHEDRON_SIDE_Z = normalize(
		vec3s {
			vec3(-1,-1,-1),
			vec3( 1,-1,-1),
			vec3(-1, 1,-1),
			vec3( 1, 1,-1),
			vec3(-1,-1, 1),
			vec3( 1,-1, 1),
			vec3(-1, 1, 1),
			vec3( 1, 1, 1)
		} 
	);
	template <class T>
	const vec3s SpheroidGridLookup<T>::OCTAHEDRON_SIDE_X = normalize(
		cross(SpheroidGridLookup<T>::OCTAHEDRON_SIDE_Z, vec3(0,0,1))
	);
	template <class T>
	const vec3s SpheroidGridLookup<T>::OCTAHEDRON_SIDE_Y = normalize(
		cross(SpheroidGridLookup<T>::OCTAHEDRON_SIDE_Z, SpheroidGridLookup<T>::OCTAHEDRON_SIDE_X)
	);

	// performs cached O(1) nearest neighbor lookups on the surface of a unit sphere
	// using a SpheroidGridLookup of vectors to optimize initialization
	class SpheroidGridVoronoi : public SpheroidGridLookup<int>
	{
	public:
		explicit SpheroidGridVoronoi(
			const vec3s& points, 
			const float min_cell_width,
			const float max_cell_width
		  )	: 
			SpheroidGridLookup<int>(min_cell_width)
		{
			// populate a slower lookup based on a list of vectors and their ids in `points`
			SpheroidGridLookup<std::vector<std::pair<int, glm::vec3>>> temp(max_cell_width);
			glm::ivec3 conceptual_id;
			std::cout << "populating slow lookup" << std::endl;
			std::vector<std::pair<int, glm::vec3>> temp2(0);
			for (unsigned int point_id = 0; point_id < points.size(); ++point_id)
			{
				for (unsigned int side_id = 0; side_id < OCTAHEDRON_SIDE_COUNT; ++side_id)
				{
					if (dot(OCTAHEDRON_SIDE_Z[side_id], points[point_id]) < (1/sqrt(3)) - max_cell_width) { continue; }
					conceptual_id = temp.get_conceptual_id(points[point_id], side_id);
					temp.get_ref(conceptual_id + ivec3(-1,-1, 0)).emplace_back(point_id, points[point_id]);
					temp.get_ref(conceptual_id + ivec3(-1, 0, 0)).emplace_back(point_id, points[point_id]);
					temp.get_ref(conceptual_id + ivec3(-1, 1, 0)).emplace_back(point_id, points[point_id]);
					temp.get_ref(conceptual_id + ivec3( 0,-1, 0)).emplace_back(point_id, points[point_id]);
					temp.get_ref(conceptual_id + ivec3( 0, 0, 0)).emplace_back(point_id, points[point_id]);
					temp.get_ref(conceptual_id + ivec3( 0, 1, 0)).emplace_back(point_id, points[point_id]);
					temp.get_ref(conceptual_id + ivec3( 1,-1, 0)).emplace_back(point_id, points[point_id]);
					temp.get_ref(conceptual_id + ivec3( 1, 0, 0)).emplace_back(point_id, points[point_id]);
					temp.get_ref(conceptual_id + ivec3( 1, 1, 0)).emplace_back(point_id, points[point_id]);
				}
			}
			std::cout << "populating fast lookup" << std::endl;
			// use the slower lookup to quickly populate a faster long term lookup
			glm::vec3  midpoint;
			std::pair<int, glm::vec3>  nearest;
			for (unsigned int side_id = 0; side_id < OCTAHEDRON_SIDE_COUNT; ++side_id)
			{
				for (int xi2d = 0; xi2d < dimensions.x; ++xi2d)
				{
					for (int yi2d = 0; yi2d < dimensions.y; ++yi2d)
					{
						midpoint   = get_midpoint(xi2d, yi2d, side_id);
						std::vector<std::pair<int, glm::vec3>>& candidates = temp.get_ref(midpoint);
						if (candidates.size() < 1) { continue; }
						nearest = *std::min_element(
							candidates.begin(), 
							candidates.end(), 
							[midpoint](std::pair<int, glm::vec3> a, std::pair<int, glm::vec3> b){
								return glm::distance(a.second, midpoint) < glm::distance(b.second, midpoint);
							}
						);
						get_ref(get_conceptual_id(xi2d, yi2d, side_id)) = nearest.first;
					}
				}
			}
		}
	};
}