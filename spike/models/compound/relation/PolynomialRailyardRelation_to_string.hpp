#pragma once

// 3rd party libraries
#include <string>

// in house libraries
#include <units/si.hpp>

#include <math/expression/Polynomial_to_string.hpp>

#include "PolynomialRailyardRelation.hpp"

namespace compound {
namespace relation {

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename Tx, typename Ty, int Plo, int Phi>
	std::string to_string(const PolynomialRailyardRelation<Tx, si::units<M1,KG1,S1,K1,MOL1,A1,CD1,Ty>, Plo, Phi>& known)
	{
		return si::to_string(known.xunits)  + "→" + std::to_string(known.yunits)  + ": " + math::to_string(known.spline);
	}

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename Tx, typename Ty, int Plo, int Phi>
	std::string to_string(const PolynomialRailyardRelation<si::units<M1,KG1,S1,K1,MOL1,A1,CD1,Tx>, Ty, Plo, Phi>& known)
	{
		return si::to_string(known.xunits)  + "→" + std::to_string(known.yunits)  + ": " + math::to_string(known.spline);
	}

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, 
	          int M2, int KG2, int S2, int K2, int MOL2, int A2, int CD2, typename Tx, typename Ty, int Plo, int Phi>
	std::string to_string(const PolynomialRailyardRelation<si::units<M1,KG1,S1,K1,MOL1,A1,CD1,Tx>, 
		                                                   si::units<M2,KG2,S2,K2,MOL2,A2,CD2,Ty>, Plo, Phi>& known)
	{
		return si::to_string(known.xunits)  + "→" + si::to_string(known.yunits)  + ": " + math::to_string(known.spline);
	}

	template <typename Tx, typename Ty, int Plo, int Phi>
	std::string to_string(const PolynomialRailyardRelation<Tx, Ty, Plo, Phi>& known)
	{
		return std::to_string(known.xunits) + "→" + std::to_string(known.yunits) + ": " + math::to_string(known.spline);
	}

} //namespace field
} //namespace compound

