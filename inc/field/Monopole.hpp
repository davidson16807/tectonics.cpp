#pragma once

#include <glm/gtx/norm.hpp>

namespace field
{

	template<typename value_type, typename vector>
	struct Monopole
	{

		vector position;
		value_type weight;

		Monopole(
			const vector position,
			const value_type weight
		): 
			position(position),
			weight(weight)
		{}

		// zero constuctor
		Monopole(): 
			position(0),
			weight(0)
		{}

	    inline Monopole<value_type,vector>& operator+=(const Monopole<value_type,vector>& other) noexcept
	    {
	    	position += other.position;
	    	weight += other.weight;
	        return *this;
	    }

	    inline Monopole<value_type,vector>& operator-=(const Monopole<value_type,vector>& other) noexcept
	    {
	    	position -= other.position;
	    	weight -= other.weight;
	        return *this;
	    }

	    [[nodiscard]]
		constexpr inline value_type operator()(const vector& v ) const
		{
		    return weight / glm::distance2(position/weight, v);
		}

	};

}
