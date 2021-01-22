#pragma once

// C libraries
#include <cmath>        // std::abs

// 3rd party libraries
#include <si.hpp>

namespace si{

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	bool is_within_fraction(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b, const T1 fraction){
		return abs(a-b) < fraction*b;
	}
	template <typename T1>
	bool is_within_fraction(const T1 a, const T1 b, const T1 fraction){
		return std::abs(a-b) < fraction*b;
	}

	template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
	bool is_within_absolute_value(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b, const T1 fraction){
		return abs(a-b) < fraction*b;
	}
	template <typename T1>
	bool is_within_absolute_value(const T1 a, const T1 b, const T1 fraction){
		return std::abs(a-b) < fraction*b;
	}

}