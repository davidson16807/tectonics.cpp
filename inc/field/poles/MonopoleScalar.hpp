#pragma once

#include <cmath>

#include <glm/gtx/norm.hpp>

namespace field
{

	template<int exponent, typename Weight, typename Vector>
	struct MonopoleScalar
	{

		Vector weighted_position;
		Weight weight;

		MonopoleScalar(
			const Vector position,
			const Weight weight
		): 
			weighted_position(weight*position),
			weight(weight)
		{}

		// zero constuctor
		MonopoleScalar(): 
			weighted_position(0),
			weight(0)
		{}

	    inline MonopoleScalar<exponent,Weight,Vector>& operator+=(const MonopoleScalar<exponent,Weight,Vector>& other) noexcept
	    {
	    	weighted_position += other.weighted_position;
	    	weight += other.weight;
	        return *this;
	    }

	    inline MonopoleScalar<exponent,Weight,Vector>& operator-=(const MonopoleScalar<exponent,Weight,Vector>& other) noexcept
	    {
	    	weighted_position -= other.weighted_position;
	    	weight -= other.weight;
	        return *this;
	    }

	    inline Vector offset_for_position(const Vector& position) const
	    {
			Vector center_of_weight = weighted_position/weight;
            return center_of_weight - position;
	    }

	    inline auto value_for_offset(const Vector& offset) const
	    {
		    return weight * std::pow(glm::length(offset), -exponent);
	    }

	    static inline auto zero()
	    {
		    return Weight(0); // incremented exponent is needed to quickly normalize the offset
	    }

		[[nodiscard]] constexpr inline Vector operator()(const Vector& position) const
		{
			return value_for_offset(offset_for_position(position));
		}

	};

}
