#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>

namespace series
{

	/*
	Given a `UnitInterval` map: ℕ→[0,1], `UnitVector3Interleave` maps ℕ to 3d vectors 
	that are evenly distributed over the set of all 3d unit vectors.
	*/
	template<typename UnitInterval, glm::qualifier Q=glm::highp>
	struct UnitVector3Interleave
	{
	    using R = typename UnitInterval::value_type;

	    using size_type = std::size_t;
		using value_type = glm::vec<3,R,Q>;

		UnitInterval values;

		constexpr inline explicit UnitVector3Interleave(const UnitInterval& values) : 
			values(values)
		{}

		// copy constructor
		constexpr inline UnitVector3Interleave(const UnitVector3Interleave<UnitInterval,Q>& a):
			values(a.values)
		{}

		constexpr inline size_type size() const { return std::max(1,values.size()/2); }

		constexpr value_type operator[](const size_type memory_id ) const
		{
	        double theta = R(2) * M_PI * values[2*memory_id];
	        double phi = std::acos(R(1) - R(2) * values[2*memory_id+1]);
	        return glm::vec<3,R,Q>(
		        std::sin(phi) * std::cos(theta),
		        std::sin(phi) * std::sin(theta),
		        std::cos(phi)
	    	);
		}

	};



	/*
	Given a `UnitInterval` map: ℕ→[0,1], `UnitVector2Interleave` maps ℕ to 2d vectors 
	that are evenly distributed over the set of all 2d unit vectors.
	*/
	template<typename UnitInterval, glm::qualifier Q=glm::highp>
	struct UnitVector2Interleave
	{
	    using R = typename UnitInterval::value_type;

	    using size_type = std::size_t;
		using value_type = glm::vec<2,R,Q>;

		UnitInterval values;

		constexpr inline explicit UnitVector2Interleave(const UnitInterval& values) : 
			values(values)
		{}

		// copy constructor
		constexpr inline UnitVector2Interleave(const UnitVector2Interleave<UnitInterval,Q>& a):
			values(a.values)
		{}

		constexpr inline size_type size() const { return values.size(); }

		constexpr value_type operator[](const size_type memory_id ) const
		{
	        double theta = R(2) * M_PI * values[memory_id];
	        return glm::vec<2,R,Q>(
		        std::cos(theta),
		        std::sin(theta)
	    	);
		}

	};

}

