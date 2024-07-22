#pragma once

// C libraries

// 3rd-party libraries

// in-house libraries
#include <unit/si.hpp>

namespace adapted
{

	/*
	The following are alternate definitions of the above that allow for support of other data types using classes of the adapter pattern
	*/

	struct SiMetric
	{
		SiMetric(){}

		template<int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
		inline constexpr auto distance(const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b) const
		{
		    return si::distance(a, b);
		}

		template<int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
		inline constexpr auto length(const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a) const
		{
		    return si::abs(a);
		}

		template<int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
		inline constexpr auto normalize(const si::units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a) const
		{
		    return a / length(a);
		}

	};

}

