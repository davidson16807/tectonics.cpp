#pragma once

#include <cmath>

#include <glm/gtx/norm.hpp>

namespace field
{

	template<int exponent, typename value_type, typename vector>
	struct Monopole
	{

		vector weighted_position;
		value_type weight;

		Monopole(
			const vector position,
			const value_type weight
		): 
			weighted_position(weight*position),
			weight(weight)
		{}

		// zero constuctor
		Monopole(): 
			weighted_position(0),
			weight(0)
		{}

	    inline Monopole<exponent,value_type,vector>& operator+=(const Monopole<exponent,value_type,vector>& other) noexcept
	    {
	    	weighted_position += other.weighted_position;
	    	weight += other.weight;
	        return *this;
	    }

	    inline Monopole<exponent,value_type,vector>& operator-=(const Monopole<exponent,value_type,vector>& other) noexcept
	    {
	    	weighted_position -= other.weighted_position;
	    	weight -= other.weight;
	        return *this;
	    }

	    inline vector offset_for_position(const vector& position) const
	    {
			vector center_of_weight = weighted_position/weight;
            return center_of_weight - position;
	    }

	    inline vector value_for_offset(const vector& offset) const
	    {
            value_type distance = glm::length(offset);
		    return offset * weight * std::pow(distance, -exponent-1); // incremented exponent is needed to quickly normalize the offset
	    }

		[[nodiscard]] constexpr inline vector operator()(const vector& position) const
		{
			return value_for_offset(offset_for_position(position));
		}

	};

}
