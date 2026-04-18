#pragma once

#include <glm/gtx/norm.hpp>

namespace field
{

	template<typename value_type, typename vector>
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

	    inline Monopole<value_type,vector>& operator+=(const Monopole<value_type,vector>& other) noexcept
	    {
	    	weighted_position += other.weighted_position;
	    	weight += other.weight;
	        return *this;
	    }

	    inline Monopole<value_type,vector>& operator-=(const Monopole<value_type,vector>& other) noexcept
	    {
	    	weighted_position -= other.weighted_position;
	    	weight -= other.weight;
	        return *this;
	    }

		[[nodiscard]] constexpr inline vector operator()(const vector& position) const
		{
			vector center_of_weight = weighted_position/weight;
            vector offset = position - center_of_weight;
            value_type distance2 = glm::length2(offset);
		    return offset * weight / distance2;
		}

	};

}
