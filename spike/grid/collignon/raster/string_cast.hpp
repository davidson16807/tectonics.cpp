
#include "../Grid.hpp"

namespace collignon 
{

	// template <typename Series, typename id, typename scalar>
	// std::string to_string(const Grid<id, scalar>& grid, const Series& a, const scalar lo, const scalar hi, const id line_char_width = 80)
	// {
	// 	std::string out("\n");
	// 	using ivec2 = glm::vec<2,id,glm::defaultp>;
	// 	double ratio = grid.voronoi.vertex_count_per_meridian / double(line_char_width);
	// 	for (id i = 0; i < line_char_width/2; ++i){
	// 		for (id j = 0; j < line_char_width; ++j){
	// 		    out += whole::character(a[grid.voronoi.memory_id(ivec2(2*ratio*i,ratio*j))],lo,hi);
	// 		}
	// 		out += "\n";
	// 	}
	// 	out += "\n";
	// 	out += whole::legend(a[0], lo, hi);
	// 	return out;
	// }


	template<int L, typename T, glm::qualifier Q>
	std::string character(
		glm::mat<3,3,T,Q> frame, 
		const glm::vec<L,T,Q> V, 
		const T lo, const T hi
	){
		return whole::character(frame*V,lo,hi);
	}

	template<typename T, glm::qualifier Q>
	std::string character(
		glm::mat<3,3,T,Q> frame, 
		const T a, const T lo, const T hi
	){
		return whole::character(a,lo,hi);
	}

	template<typename T, glm::qualifier Q>
	inline std::string character(
		glm::mat<3,3,T,Q> frame, 
		const bool a, const bool lo, const bool hi
	){
		return whole::character(a,lo,hi);
	}


	template <typename Series, typename id, typename scalar, glm::qualifier Q>
	std::string to_string(
		const Grid<id, scalar, Q>& grid, 
		const Series& a, const scalar lo, const scalar hi, 
		const id line_char_width = 80, 
		const glm::vec<3,scalar,Q> north_pole = glm::vec<3,scalar,Q>(0,0,1))
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;
		std::string out("\n");
		id vertex_id;
		int aspect_ratio(4);
		double pi(M_PI);
		double dlon(2.0*pi/line_char_width);
		double dlat(pi/(line_char_width/aspect_ratio));
		for (id i = 0; i < line_char_width/aspect_ratio; ++i){
			for (id j = 0; j < line_char_width; ++j){
				// std::cout << i << " " << j << " " << pi/2.0-dlat*i << " " << dlon*j << std::endl;
				// std::cout << "    " << " " << std::cos(pi/2.0-dlat*i)*std::cos(dlon*j) << " " << std::cos(pi/2.0-dlat*i)*std::sin(dlon*j) << " " << std::sin(pi/2.0-dlat*i) << std::endl; 
				vertex_id = grid.voronoi.memory_id(
					vec3(
						std::cos(pi/2.0-dlat*i)*std::cos(dlon*j),
						std::cos(pi/2.0-dlat*i)*std::sin(dlon*j),
						std::sin(pi/2.0-dlat*i)
					));
				// std::cout << "    " << vertex_id << " " << std::endl;
				// std::cout << "    " << character(
				//     	grid.vertex_frame(vertex_id, north_pole),
				//     	a[vertex_id],
				//     	lo,hi) << std::endl;
			    out += character(
			    	grid.vertex_frame(vertex_id, north_pole),
			    	a[vertex_id],
			    	lo,hi);
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

