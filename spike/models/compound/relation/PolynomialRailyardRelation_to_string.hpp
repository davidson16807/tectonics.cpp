#pragma once

// 3rd party libraries
#include <string>

// in house libraries
#include <math/expression/PolynomialRailyard_to_string.hpp>
#include "PolynomialRailyardRelation.hpp"

namespace compound {
namespace relation {

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename Tx, typename Ty, int Plo, int Phi>
	std::string to_string(const PolynomialRailyardRelation<Tx, si::units<M1,KG1,S1,K1,MOL1,A1,CD1,Ty>, Plo, Phi>& known)
	{
		return math::to_string(known.spline);
	}

} //namespace field
} //namespace compound

