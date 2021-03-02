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
#include <glm/mat3x3.hpp>        // *mat3

namespace rasters
{
	/*
	An `Octosphere` allows for O(1) lookups and traversals on the surface of a unit sphere.
	It does this using a 2d square cartesian grid that has been stretched around a sphere, forming an spheroidal octahedron.

	`Octosphere` exclusively represents data structures and functions that define a mathematical category
	where arrows map between positions and different kinds of ids on this spheroidal octahedron.
	
	The category diagram can be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:

	```
	% https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZAZgBoBGAXVJADcBDAGwFcYkQBjGRxgdyyg4AFiAC+pdJlz5CKMgCZqdJq3ZQsAWxhhsBBOMnY8BImWJKGLNok7dGHCMzA4xEkBiMyiABlLeLKtYgABquhtImKOR+AVbswQAEADpJeFpwCaEG7lLGssi+VDSWqjYAysmpmvCZYTmekQWk5sWB7BUpaTUA4nUeEfnRLcpxNgCyfbleKL4ALLGlIL3Z-XlE0fOto0uVXRnLbqvTyNGKW4tlYkowUADm8ESgAGYAThAaSNEgOBBI8tmvd5IXzfX6IcgAt4fRAAThoPyQAA5zkEANR1QHQ2bwsFkEaLdGQoGIZGgpAANiJ0PJOKRNDgQiwTxciAAtF9GFgwEEoBAcDgbhioRTaYgAOw0Rj0ABG3AAClNIiAXlhbkIXFSRWTEABWTW60U0kAMpks9n6vEIg3GxnMpDsyVcnl8gVQIXEnWi4j6z3a8ggznctQuwX6o1W-2OoM2Xn80OUURAA
	\begin{tikzcd}
	X \arrow[d, dotted, shift left]                                                  & X \times X \arrow[l, "+"] &  &                      \\
	S \times X \arrow[dd, shift left] \arrow[u] \arrow[rd, dotted]                   &                           &  & cellwidth \arrow[d]  \\
	                                                                                 & S                         &  & dimensions \arrow[d] \\
	S \times G \arrow[d, dotted, shift left] \arrow[r] \arrow[uu] \arrow[ru, dotted] & M                         &  & cellcount            \\
	G \arrow[u]                                                                      & G \times G \arrow[l, "+"] &  &                     
	\end{tikzcd}
	```
	*/

	class OctosphereGrid
	{
		struct OctosphereSide
		{
			glm::vec3 origin;
			glm::vec3 i;
			glm::vec3 j;
			glm::vec3 normal;
			glm::mat3 transform;
			glm::mat3 inverse;
			OctosphereSide(
			  const glm::vec3 origin, 
			  const glm::vec3 x, 
			  const glm::vec3 y 
			): 
			  origin(origin), 
			  i((x-origin)),
			  j((y-origin)),
			  normal(glm::normalize(  glm::cross(i, j) * (glm::dot(glm::cross(i, j), origin) > 0.0f? 1.0f : -1.0f)) ),
			  transform(glm::mat3(i, j, normal)),
			  inverse(glm::inverse(glm::mat3(i, j, normal)))
			{}
		};
		static constexpr float pi = 3.141592652653589793f;
		static constexpr float epsilon = 1e-4;
		static constexpr uint side_count = 8;	// number of sides on the data cube
		static constexpr glm::vec3 top       = glm::vec3(0,0, 1);
		static constexpr glm::vec3 bottom    = glm::vec3(0,0,-1);
		static constexpr glm::vec3 corner_px = glm::vec3(1, 0,0);
		static constexpr glm::vec3 corner_nx = glm::vec3(-1,0,0);
		static constexpr glm::vec3 corner_py = glm::vec3(0, 1,0);
		static constexpr glm::vec3 corner_ny = glm::vec3(0,-1,0);
		std::vector<OctosphereSide> sides;

		glm::vec2 standardize_grid_position(const glm::vec2 grid_position) const 
		{
			const glm::vec2 tesselation_id = glm::round((grid_position)/float(tesselation_leg_cell_count));
			float negative_if_tesselation_is_inverted = cos(pi*tesselation_id.x) * cos(pi*tesselation_id.y);
			const glm::vec2 standard_grid_position = 
				(grid_position - tesselation_id * float(tesselation_leg_cell_count)) * negative_if_tesselation_is_inverted;

			return standard_grid_position;
		}

		uint get_side_id(const glm::vec3 sphere_position) const
		{
			return
			  (( sphere_position.x < 0) << 0) +
			  (( sphere_position.y < 0) << 1) +
			  (( sphere_position.z < 0) << 2) ; 
		}

		uint get_side_id(const glm::vec2 standard_grid_position) const
		{
			const uint side_id =
			  ((standard_grid_position.x < 0) << 0) +
			  ((standard_grid_position.y < 0) << 1) +
			  ((std::abs(standard_grid_position.x) + std::abs(standard_grid_position.y) > side_leg_cell_count) << 2);

			return side_id;
		}

	public:
		const float min_cell_width;
		const int side_leg_cell_count;
		const int tesselation_leg_cell_count;
		const int quadrant_cell_count;
		const int side_cell_count;
		const int tesselation_cell_count;

		~OctosphereGrid()
		{
		}
		explicit OctosphereGrid(
			const float min_cell_width
		) : 
			sides(
				std::vector<OctosphereSide> {
					OctosphereSide(top, corner_px, corner_py), 
					OctosphereSide(top, corner_nx, corner_py),
					OctosphereSide(top, corner_px, corner_ny),
					OctosphereSide(top, corner_nx, corner_ny),

					OctosphereSide(bottom, corner_py, corner_px),
					OctosphereSide(bottom, corner_py, corner_nx),
					OctosphereSide(bottom, corner_ny, corner_px),
					OctosphereSide(bottom, corner_ny, corner_nx)
				}
			),
			min_cell_width(min_cell_width),
			side_leg_cell_count(std::max(std::floor(0.5f*pi/min_cell_width), 1.0f) + epsilon),
			tesselation_leg_cell_count(2*side_leg_cell_count),
			quadrant_cell_count(side_leg_cell_count * side_leg_cell_count),
			side_cell_count(quadrant_cell_count / 2),
			tesselation_cell_count(quadrant_cell_count * 4)
		{
		}

		glm::vec2 grid_position(const glm::vec3 sphere_position) const {

			const uint side_id = get_side_id(sphere_position);
			const float side_distance = 
				glm::dot(sides[side_id].origin, sides[side_id].normal) / glm::dot(glm::normalize(sphere_position), sides[side_id].normal);
			const glm::vec3 side_projection = glm::normalize(sphere_position) * side_distance;
			const glm::vec3 offset = side_projection - sides[side_id].origin;
			const glm::vec3 side_position = sides[side_id].inverse * offset;
		    const glm::vec2 quadrant_id = side_id < 4? 
		    	glm::vec2(side_position.x, side_position.y) : 1.0f - glm::vec2(side_position.x, side_position.y);
		    const glm::vec2 border_aligned = quadrant_id * glm::sign(glm::vec2(sides[side_id].normal.x, sides[side_id].normal.y)) * float(side_leg_cell_count);
		    const glm::vec2 grid_position = border_aligned - 0.5f;
		    return grid_position;
		}

		glm::vec3 sphere_position(const glm::vec2 grid_position) const {

			const glm::vec2 border_aligned = grid_position + 0.5f;
			const glm::vec2 standard_grid_position = standardize_grid_position(border_aligned);
			const uint side_id = get_side_id(standard_grid_position);
			const glm::vec2 quadrant_id = 
				standard_grid_position * glm::sign(glm::vec2(sides[side_id].normal.x, sides[side_id].normal.y)) / float(side_leg_cell_count);
			const glm::vec2 side_position = side_id < 4? 
		    	quadrant_id : 1.0f - quadrant_id;
		    const glm::vec3 offset = sides[side_id].transform * glm::vec3(side_position.x, side_position.y, 0);
		    const glm::vec3 side_projection = sides[side_id].origin + offset;
		    const glm::vec3 sphere_position = glm::normalize(side_projection);

		    return sphere_position;
		}

		int memory_id(const glm::vec2 grid_position) const {
			const glm::vec2 standard_grid_position = glm::round(grid_position);
			const glm::ivec2 standard_grid_id = glm::ivec2(standard_grid_position) + side_leg_cell_count;
			const glm::ivec2 unique_grid_id = glm::ivec2(standard_grid_id.x % tesselation_leg_cell_count, standard_grid_id.y % tesselation_leg_cell_count);
			const int memory_id = std::clamp(unique_grid_id.x, 0, tesselation_leg_cell_count-1)
				  + std::clamp(unique_grid_id.y, 0, tesselation_leg_cell_count-1) * tesselation_leg_cell_count;
		    return memory_id;
		}

		glm::vec2 grid_position(const int memory_id) const {
			return glm::vec2(memory_id % tesselation_leg_cell_count, memory_id / tesselation_leg_cell_count) - float(side_leg_cell_count);
		}

	};
}