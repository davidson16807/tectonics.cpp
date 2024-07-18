#pragma once

#include <math/special.hpp>

namespace spheroidal 
{

	template<int L, typename T, glm::qualifier Q>
	std::string character(
		glm::mat<3,3,T,Q> frame, 
		const glm::vec<L,T,Q> V, 
		const T lo, const T hi
	){
		return whole::character(frame*V,lo,hi);
	}

	template<typename value, typename scalar, glm::qualifier Q>
	std::string character(
		glm::mat<3,3,scalar,Q> frame, 
		const value a, const value lo, const value hi
	){
		return whole::character(a,lo,hi);
	}

	template <typename Grid, typename Series, typename id, typename scalar, typename value, glm::qualifier Q>
	std::string to_string(
		const Grid& grid, 
		const Series& a, const value lo, const value hi, 
		const id line_char_width, 
		const glm::vec<3,scalar,Q> north_pole)
	{
		std::string out("\n");
		int aspect_ratio(4);
		double dlon(2.0*math::pi/line_char_width);
		double dlat(math::pi/(line_char_width/aspect_ratio));
		using vec3 = glm::vec<3,scalar,glm::defaultp>;
		for (id i = 0; i < line_char_width/aspect_ratio; ++i){
			for (id j = 0; j < line_char_width; ++j){
				auto vertex_id = grid.nearest_vertex_id(
					vec3(
						std::cos(math::pi/2.0-dlat*i)*std::cos(dlon*j),
						std::cos(math::pi/2.0-dlat*i)*std::sin(dlon*j),
						std::sin(math::pi/2.0-dlat*i)
					));
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

	template <typename Grid, typename Series>
	std::string to_string(
		const Grid& grid, 
		const Series& a)
	{
		using scalar = typename Grid::value_type;
		using id = typename Grid::size_type;
		auto hi(whole::magnitude(a[0]));
		auto lo(whole::magnitude(a[0]));
		for (id i = 0; i < int(grid.vertex_count()); ++i)
		{
			hi = std::max(hi, whole::magnitude(a[i]));
			lo = std::min(lo, whole::magnitude(a[i]));
		}
		return to_string(grid, a, lo, hi, id(120), glm::vec<3,scalar>(0,0,1));
	}

	/*
	The following is intended to replace the above functionality.
	It uses the Adapter/Dependency Injection pattern to better support other data types and modes of operation,
	and to avoid tricks that are needed to do the same thing with namespaces and #include juggling.
	Methods are conceptually pure functions.
	*/
	template <typename ElementStrings, typename OrderAggregation>
	class Strings
	{
		const ElementStrings strings;
		const OrderAggregation aggregation;
		template<int L, typename T, glm::qualifier Q>
		std::string character(
			glm::mat<3,3,T,Q> frame, 
			const glm::vec<L,T,Q> V, 
			const T lo, const T hi
		){
			return strings.character(frame*V,lo,hi);
		}
		template<typename value, typename scalar, glm::qualifier Q>
		std::string character(
			glm::mat<3,3,scalar,Q> frame, 
			const value a, const value lo, const value hi
		){
			return strings.character(a,lo,hi);
		}
	public:
		Strings(const ElementStrings& strings, const OrderAggregation& aggregation):
			strings(strings),
			aggregation(aggregation)
		{}
		template <typename Grid, typename Raster, typename id, typename scalar, typename value, glm::qualifier qualifier>
		std::string format(
			const Grid& grid, 
			const Raster& raster, const value lo, const value hi, 
			const id line_char_width,
			const glm::vec<3,scalar,qualifier> north_pole)
		{
			std::string out("\n");
			int aspect_ratio(4);
			double dlon(2.0*math::pi/line_char_width);
			double dlat(math::pi/(line_char_width/aspect_ratio));
			using vec3 = glm::vec<3,scalar,glm::defaultp>;
			for (id i = 0; i < line_char_width/aspect_ratio; ++i){
				for (id j = 0; j < line_char_width; ++j){
					auto vertex_id = grid.nearest_vertex_id(
						vec3(
							std::cos(math::pi/2.0-dlat*i)*std::cos(dlon*j),
							std::cos(math::pi/2.0-dlat*i)*std::sin(dlon*j),
							std::sin(math::pi/2.0-dlat*i)
						));
					out += character(
				    	grid.vertex_frame(vertex_id, north_pole),
				    	raster[vertex_id],
				    	lo,hi);
				}
				out += "\n";
			}
			out += "\n";
			out += strings.legend(raster[0], lo, hi);
			return out;
		}
		template <typename Grid, typename Raster>
		std::string format(
			const Grid& grid, 
			const Raster& raster)
		{
			using scalar = typename Grid::value_type;
			using id = typename Grid::size_type;
			return format(grid, raster, 
				aggregation.min(raster), aggregation.max(raster), 
				id(120), glm::vec<3,scalar>(0,0,1));
		}
	};

}

