#pragma once

#include <string>

#include <many/types.hpp>
#include <many/common.hpp>

namespace rasters
{
	namespace 
	{
		#if defined(__clang__)
			const std::vector<const std::string> 
		#else
			const std::array<const std::string, 6>
		#endif
		shades {" ", "░", "▒", "▓", "█" };
	} 

	template <typename T>
	std::string to_string(const traster<T>& a, const T lo, const T hi, const int line_char_width = 80)
	{
		float lat(0.);
		float lon(0.);
		float r  (0.);
		float z  (0.);
		vec3  pos(0.,0.,0.);
		unsigned int id(0);
		std::string out("");

		for (unsigned int i = 0; i < line_char_width/4; ++i)
		{
			lat = M_PI*i/float((line_char_width)/4.f-1) - M_PI/2.f;
			for (unsigned int j = 0; j < line_char_width; ++j)
			{
				lon = 2.f*M_PI*(j+1)/float(line_char_width+1);
				z = sin(lat);
				r = sqrt(1.f-z*z);
				pos = vec3(r*sin(lon), r*cos(lon), z);
				id = a.grid->voronoi->nearest_id(pos);
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
				    out += shades[ int( float(shades.size()-1) * float(linearstep(lo, hi, a[id])) ) ];
				}
			}
	    	out += "\n";
		}
		out += "\n";
		for (unsigned int i = 0; i < shades.size(); ++i)
		{
			out += shades[i];
			out += " > ";
			out += std::to_string(mix(float(lo), float(hi), float(i)/float(shades.size())));
			out += "\n";
			// out += ", ";
		}
		return out;
	}

	template <typename T>
	std::string to_string(const traster<T>& a, const int line_char_width = 80)
	{
		return to_string(a, min(a), max(a), line_char_width);
	}
}//namespace many



