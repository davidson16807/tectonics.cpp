#pragma once

#include <string>
#include <iostream>

#include <many/types.hpp>
#include <many/common.hpp>
#include <many/glm/matrix.hpp>

namespace rasters
{
	namespace 
	{
		const std::vector<const std::string> weak 
		{
			"←","↙","↙","↓","↓","↘","↘","→","→","↗","↗","↑","↑","↖","↖","←"
		};
		const std::vector<const std::string> strong 
		{
			"⬅","⬋","⬋","⬇","⬇","⬊","⬊","➡","➡","⬈","⬈","⬆","⬆","⬉","⬉","⬅"
		};
	}

	/*
	This "to_string()" extension returns a string representation for a vec2raster,
	  drawing 2d vectors as if on the surface of a sphere using an equirectangular projection.
	It assumes the following:
	  * the mesh is at least a rough approximation of a unit sphere
	  * 2d vectors are expressed using local x,y coordinates on the surface of a sphere.
	  * "up" is indicated by the z axis of a 3d vector
	*/
	template <typename T, glm::qualifier Q>
	std::string to_string(const traster<glm::vec<2,T,Q>>& a, const int line_char_width = 80)
	{
		tmany<T> a_length(a.size());
		length(a, a_length);
		T a_length_max = max(a_length);

		float lat(0.);
		float lon(0.);
		float r  (0.);
		float z  (0.);
		vec3  pos(0.,0.,0.);
		unsigned int id(0);
		std::string out("");

		for (unsigned int i = 0; i < line_char_width/4; ++i)
		{
			lat = M_PI*i/float(line_char_width/4) - M_PI/2;
			for (unsigned int j = 0; j < line_char_width; ++j)
			{
				lon = 2.*M_PI*j/float(line_char_width);
				z = cos(lat);
				r = sqrt(1-z*z);
				pos = vec3(r*sin(lon), r*cos(lon), z);
				id = a.grid->voronoi->nearest_id(pos);
				// vec3 surface_basis_z = a.grid->vertex_normals[id];
				// vec3 surface_basis_x = glm::normalize(glm::cross(surface_basis_z, vec3(0,0,1)));
				// vec3 surface_basis_y = glm::normalize(glm::cross(surface_basis_x, surface_basis_z));
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
					int i  = std::clamp(int(weak.size()*turn), 0, int(weak.size()-1));
					out   += weak[i];
				}
				else
				{
					T turn = (std::atan2(a[id].y, a[id].x)+M_PI)/(2.*M_PI);
					int i  = std::clamp(int(strong.size()*turn), 0, int(strong.size()-1));
					out   += strong[i];
				}
			}
	    	out += "\n";
		}
		out += "\n";
		out += "→ > ";
		out += std::to_string(a_length_max * T(1./3.));
		out += "\n";
		out += "➡ > ";
		out += std::to_string(a_length_max * T(2./3.));
		out += "\n";
		return out;
	}

	/*
	This "to_string()" extension returns a string representation for a vec2raster,
	  drawing vectors as if on the surface of a sphere using an equirectangular projection.
	Since 3d vectors can't be easily represented in 2d ascii art, 
	  the 3d vectors are portrayed as the 2d projections on the surface of a sphere. 
	It assumes the following:
	  * the mesh is at least a rough approximation of a unit sphere
	  * "up" is indicated by the z axis of a 3d vector
	*/
	template <typename T, glm::qualifier Q>
	std::string to_string(const traster<glm::vec<3,T,Q>>& a, const int line_char_width = 80)
	{
		glm::vec3		up(0,0,1);
		many::vec3s		surface_basis_x(a.size());
		many::vec3s		surface_basis_y(a.size());
		many::vec3s		surface_basis_z(a.grid->vertex_normals);
		many::floats	a2dx(a.size());
		many::floats	a2dy(a.size());
		traster<glm::vec<2,T,Q>> a2d (a.grid);

		cross		(surface_basis_z, up, 				surface_basis_x);
		normalize	(surface_basis_x, 					surface_basis_x);

		cross		(surface_basis_z, surface_basis_x, 	surface_basis_y);
		normalize	(surface_basis_y, 					surface_basis_y);

		dot 		(surface_basis_x, a, 				a2dx);
		dot 		(surface_basis_y, a, 				a2dy);

		set_x 		(a2d, a2dx);
		set_y 		(a2d, a2dy);

		return rasters::to_string(a2d, line_char_width);
	}
}//namespace many



