
#include "../Grid.hpp"

namespace collignon 
{

	template <typename Series, typename id, typename scalar, glm::qualifier Q>
	std::string to_string(const Grid<id, scalar>& grid, const Series& a, 
		const scalar lo, const scalar hi, 
		const id line_char_width = 80, 
		const glm::vec<3,scalar,Q> north_pole = glm::vec<3,scalar,Q>(0,0,1))
	{
		std::string out("\n");
		using ivec2 = glm::vec<2,id,glm::defaultp>;
		double ratio = grid.voronoi.vertices_per_meridian / double(line_char_width);
		for (id i = 0; i < line_char_width/2; ++i){
			for (id j = 0; j < line_char_width; ++j){
				id vertex_id = grid.voronoi.memory_id(ivec2(2*ratio*i,ratio*j));
				auto foo = character(
			    	grid.vertex_frame(vertex_id, north_pole),
			    	a[vertex_id],
			    	lo,hi);
				out += foo.size() > 0? foo : std::to_string(foo.size());
			}
			out += "\n";
		}
		out += "\n";
		out += whole::legend(a[0], lo, hi);
		return out;
	}


	template <typename Series, typename id, typename scalar, glm::qualifier Q>
	std::string to_string(
		const Grid<id, scalar, Q>& grid, 
		const Series& a, 
		const id line_char_width = 80,
		const glm::vec<3,scalar,Q> north_pole = glm::vec<3,scalar,Q>(0,0,1))
	{
		auto hi(whole::magnitude(a[0]));
		auto lo(whole::magnitude(a[0]));
		for (id i = 0; i < int(grid.vertex_count()); ++i)
		{
			hi = std::max(hi, whole::magnitude(a[i]));
			lo = std::min(lo, whole::magnitude(a[i]));
		}
		return to_string(grid, a, lo, hi, line_char_width, north_pole);
	}
}

