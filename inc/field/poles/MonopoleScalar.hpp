#pragma once

#include <cmath>

#include <glm/gtx/norm.hpp>

namespace field
{

	template<int exponent, typename scalar, typename vector>
	struct MonopoleScalar
	{

		static constexpr scalar zero = scalar(0);

		vector weighted_position;
		scalar weight;

		MonopoleScalar(
			const vector position,
			const scalar weight
		): 
			weighted_position(weight*position),
			weight(weight)
		{}

		// zero constuctor
		MonopoleScalar(): 
			weighted_position(0),
			weight(0)
		{}

	    inline MonopoleScalar<exponent,scalar,vector>& operator+=(const MonopoleScalar<exponent,scalar,vector>& other) noexcept
	    {
	    	weighted_position += other.weighted_position;
	    	weight += other.weight;
	        return *this;
	    }

	    inline MonopoleScalar<exponent,scalar,vector>& operator-=(const MonopoleScalar<exponent,scalar,vector>& other) noexcept
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

	    inline scalar value_for_offset(const vector& offset) const
	    {
            scalar distance = glm::length(offset);
		    return weight * std::pow(distance, -exponent-1); // incremented exponent is needed to quickly normalize the offset
	    }

		[[nodiscard]] constexpr inline vector operator()(const vector& position) const
		{
			return value_for_offset(offset_for_position(position));
		}

	};

}
