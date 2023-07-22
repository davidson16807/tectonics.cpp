#pragma once

// 3rd party libraries
#include <units/si.hpp>

// in-house libraries
#include "../each.hpp"

namespace each {

	#define EACH_UNARY_SI_WRAPPER(NAME) \
    template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1> \
		inline auto NAME(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a){ return glm::NAME(a); }

	#define EACH_BINARY_SI_WRAPPER(NAME) \
    template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1> \
		inline auto NAME(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b){ return si::NAME(a,b); }

	EACH_UNARY_SI_WRAPPER(abs)
	EACH_UNARY_SI_WRAPPER(sqrt)
	EACH_UNARY_SI_WRAPPER(cbrt)
	EACH_UNARY_SI_WRAPPER(root)

	EACH_BINARY_SI_WRAPPER(distance)
	EACH_BINARY_SI_WRAPPER(min)
	EACH_BINARY_SI_WRAPPER(max)

	#undef EACH_UNARY_SI_WRAPPER
	#undef EACH_BINARY_SI_WRAPPER

}

