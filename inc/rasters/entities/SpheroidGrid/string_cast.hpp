#pragma once

#include <cmath>  // isnan, isinf
#include <algorithm> //std::min
#include <string>    //std::string

#include <glm/vec3.hpp>   // *vec3
#include <glm/common.hpp> // smoothstep

#include <series/types.hpp>
#include <series/common.hpp>

#include "../Grid/Raster.hpp"
#include "SpheroidGrid.hpp"

namespace rasters
{
	namespace 
	{
		#if defined(__clang__)
			const std::vector<const std::string> 
		#else
			const std::array<const std::string, 5>
		#endif
		shades {" ", "░", "▒", "▓", "█" };

		#if defined(__clang__)
			const std::vector<const std::string> 
		#else
			const std::array<const std::string, 16> 
		#endif
		weak 
		{
			"←","↙","↙","↓","↓","↘","↘","→","→","↗","↗","↑","↑","↖","↖","←"
		};

		#if defined(__clang__)
			const std::vector<const std::string> 
		#else
			const std::array<const std::string, 16> 
		#endif
		strong 
		{
			"⬅","⬋","⬋","⬇","⬇","⬊","⬊","➡","➡","⬈","⬈","⬆","⬆","⬉","⬉","⬅"
		};
	}


	template <typename Tgrid, typename T>
	std::string to_string(const Raster<T,Tgrid>& a, const T lo, const T hi, const uint line_char_width = 80)
	{
		float lat(0.);
		float lon(0.);
		float r  (0.);
		float z  (0.);
		glm::vec3  pos(0.,0.,0.);
		uint id(0);
		std::string out("");

		for (uint i = 0; i < line_char_width/4; ++i)
		{
			lat = M_PI*i/float((line_char_width)/4.f-1) - M_PI/2.f;
			for (uint j = 0; j < line_char_width; ++j)
			{
				lon = 2.f*M_PI*(j+1)/float(line_char_width+1);
				z = sin(lat);
				r = sqrt(1.f-z*z);
				pos = glm::vec3(r*sin(lon), r*cos(lon), z);
				id = a.grid.voronoi->get_value(pos);
				if ( !(0 <= id && id < a.size()) )
				{
					out += "X";
				}
				else if (std::isnan(a[id]))
				{
					out += "N";
				}
				else if (std::isinf(a[id]))
				{
					out += "∞";
				} 
				else 
				{
					float shade_fraction = series::linearstep(lo, hi, a[id]);
					int shade_id = int(std::min(float(shades.size()-1), (shades.size() * shade_fraction) ));
				    out += shades[shade_id];
				}
			}
	    	out += "\n";
		}
		out += "\n";
		for (uint i = 0; i < shades.size(); ++i)
		{
			out += shades[i];
			out += " ≥ ";
			out += std::to_string(glm::mix(float(lo), float(hi), float(i)/float(shades.size())));
			out += "\n";
			// out += ", ";
		}
		return out;
	}

	template <typename Tgrid, typename T>
	std::string to_string(const Raster<T,Tgrid>& a, const int line_char_width = 80)
	{
		return to_string(a, series::min(a), series::max(a), line_char_width);
	}

	
	/*
	This "to_string()" extension returns a string representation for a vec2Raster,
	  drawing 2d vectors as if on the surface of a sphere using an equirectangular projection.
	It assumes the following:
	  * the mesh is at least a rough approximation of a unit sphere
	  * 2d vectors are expressed using local x,y coordinates on the surface of a sphere.
	  * "up" is indicated by the z axis of a 3d vector
	*/
	template <typename Tgrid, typename T, glm::qualifier Q>
	std::string to_string(const Raster<glm::vec<2,T,Q>,Tgrid>& a, const uint line_char_width = 80)
	{
		Raster<T,Tgrid> a_length(a.grid);
		series::length(a, a_length);
		T a_length_max = series::max(a_length);

		float lat(0.);
		float lon(0.);
		float r  (0.);
		float z  (0.);
		glm::vec3  pos(0.,0.,0.);
		uint id(0);
		std::string out("");

		for (uint i = 0; i < line_char_width/4; ++i)
		{
			lat = M_PI*i/float((line_char_width)/4.f-1) - M_PI/2.f;
			for (uint j = 0; j < line_char_width; ++j)
			{
				lon = 2.f*M_PI*(j+1)/float(line_char_width+1);
				z = sin(lat);
				r = sqrt(1.f-z*z);
				pos = glm::vec3(r*sin(lon), r*cos(lon), z);
				id = a.grid.voronoi->get_value(pos);
				if ( !(0 <= id && id < a.size()) )
				{
					out += "X";
				}
				else if (std::isnan(a[id].x) || std::isnan(a[id].y))
				{
					out += "N";
				}
				else if (std::isinf(a[id].x) || std::isinf(a[id].y))
				{
					out += "∞";
				} 
				else if (glm::length(a[id]) < a_length_max * T(1./3.))
				{
					out += " ";
				}
				else if (glm::length(a[id]) < a_length_max * T(2./3.))
				{
					T turn = (std::atan2(a[id].y, a[id].x)+M_PI)/(2.*M_PI);
					int arrow_id  = std::clamp(int(weak.size()*turn), 0, int(weak.size()-1));
					out   += weak[arrow_id];
				}
				else
				{
					T turn = (std::atan2(a[id].y, a[id].x)+M_PI)/(2.*M_PI);
					int arrow_id  = std::clamp(int(strong.size()*turn), 0, int(strong.size()-1));
					out   += strong[arrow_id];
				}
			}
	    	out += "\n";
		}
		out += "\n";
		out += "|→| ≥ ";
		out += std::to_string(a_length_max * T(1./3.));
		out += "\n";
		out += "|➡| ≥ ";
		out += std::to_string(a_length_max * T(2./3.));
		out += "\n";
		return out;
	}


	/*
	This "to_string()" extension returns a string representation for a vec2Raster,
	  drawing vectors as if on the surface of a sphere using an equirectangular projection.
	Since 3d vectors can't be easily represented in 2d ascii art, 
	  the 3d vectors are portrayed as the 2d projections on the surface of a sphere. 
	It assumes the following:
	  * the mesh is at least a rough approximation of a unit sphere
	  * "up" is indicated by the z axis of a 3d vector
	*/
	template <typename Tgrid, typename T, glm::qualifier Q>
	std::string to_string(
		const series::Series<glm::vec<3,T,Q>>& vertex_normals, 
		const Raster<glm::vec<3,T,Q>,Tgrid>& a, 
		const uint line_char_width = 80, 
		const glm::vec3 up = glm::vec3(0,0,1)
	) {
		series::vec3s		surface_basis_x(a.size());
		series::vec3s		surface_basis_y(a.size());
		series::vec3s		surface_basis_z(vertex_normals);
		series::floats	a2dx(a.size());
		series::floats	a2dy(a.size());
		Raster<glm::vec<2,T,Q>,Tgrid> a2d (a.grid);

		series::cross		(surface_basis_z, up, 				surface_basis_x);
		series::normalize	(surface_basis_x, 					surface_basis_x);

		series::cross		(surface_basis_z, surface_basis_x, 	surface_basis_y);
		series::normalize	(surface_basis_y, 					surface_basis_y);

		series::dot 		(surface_basis_x, a, 				a2dx);
		series::dot 		(surface_basis_y, a, 				a2dy);

		series::set_x 	(a2d, a2dx);
		series::set_y 	(a2d, a2dy);

		return rasters::to_string(a2d, line_char_width);
	}

	template <typename Tgrid, typename T, glm::qualifier Q>
	std::string to_string(
		const Raster<glm::vec<3,T,Q>,Tgrid>& a, 
		const uint line_char_width = 80, 
		const glm::vec3 up = glm::vec3(0,0,1)
	) {
		return to_string(a.grid.cache->vertex_normals, a, line_char_width, up);
	}

	
}//namespace rasters



