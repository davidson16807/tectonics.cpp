#pragma once

#include <cmath>

#include <glm/gtx/norm.hpp>

namespace field
{

	template<int exponent, typename Weight, typename Vector>
	struct MonopoleVector
	{

		Vector weighted_position;
		Weight weight;

		MonopoleVector(
			const Vector position,
			const Weight weight
		): 
			weighted_position(weight*position),
			weight(weight)
		{}

		// zero constuctor
		MonopoleVector(): 
			weighted_position(0),
			weight(0)
		{}

	    inline MonopoleVector<exponent,Weight,Vector>& operator+=(const MonopoleVector<exponent,Weight,Vector>& other) noexcept
	    {
	    	weighted_position += other.weighted_position;
	    	weight += other.weight;
	        return *this;
	    }

	    inline MonopoleVector<exponent,Weight,Vector>& operator-=(const MonopoleVector<exponent,Weight,Vector>& other) noexcept
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
		    return offset * weight * std::pow(glm::length2(offset), 0.5f * float(-exponent-1)); 
		    // the incremented exponent is needed to quickly normalize the offset
	    }

	    static inline auto zero()
	    {
		    return Vector(0) * Weight(0); // incremented exponent is needed to quickly normalize the offset
	    }

		[[nodiscard]] constexpr inline Vector operator()(const Vector& position) const
		{
			return value_for_offset(offset_for_position(position));
		}

	};

}

