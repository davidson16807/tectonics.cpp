#pragma once

#include <cmath>  // isnan, isinf
#include <algorithm> //std::min
#include <string>    //std::string

#include <many/types.hpp>
#include <many/common.hpp>
#include <rasters/SpheroidGrid.hpp>

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
	} 

	template <typename T>
	std::string to_string(const SpheroidGrid& grid, const tmany<T>& a, const T lo, const T hi, const uint line_char_width = 80)
	{
		float lat(0.);
		float lon(0.);
		float r  (0.);
		float z  (0.);
		vec3  pos(0.,0.,0.);
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
				pos = vec3(r*sin(lon), r*cos(lon), z);
				id = grid.voronoi.get_value(pos);
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
					float shade_fraction = linearstep(lo, hi, a[id]);
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
			out += std::to_string(mix(float(lo), float(hi), float(i)/float(shades.size())));
			out += "\n";
			// out += ", ";
		}
		return out;
	}

	template <typename T>
	std::string to_string(const SpheroidGrid& grid, const tmany<T>& a, const int line_char_width = 80)
	{
		return to_string(grid, a, min(a), max(a), line_char_width);
	}
}//namespace many



