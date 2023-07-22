#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include "compatibility.hpp"

namespace data
{

	#define EACH_BINARY_OPERATION(SYMBOL, NAME) \
	template <typename T1, typename T2, typename Tout>\
	void NAME (const T1& a, const T2& b, Tout& out)\
	{\
		assert(compatible(a,b,out));\
		auto size = out.size();\
		for (std::size_t i = 0; i < size; ++i)\
		{\
			out[i] = (a[i] SYMBOL b[i]);\
		}\
	}
	EACH_BINARY_OPERATION(+,  add)
	EACH_BINARY_OPERATION(-,  sub)
	EACH_BINARY_OPERATION(*,  mult)
	EACH_BINARY_OPERATION(/,  div)
	#undef EACH_BINARY_OPERATION

	#define EACH_UNARY_PREFIX_OPERATION(SYMBOL, NAME) \
	template <typename T1, typename Tout>\
	void NAME (const T1& a, Tout& out)\
	{\
		assert(compatible(a,out));\
		auto size = out.size();\
		for (std::size_t i = 0; i < size; ++i)\
		{\
			out[i] = SYMBOL a[i];\
		}\
	}
	#undef EACH_UNARY_PREFIX_OPERATION


	#define EACH_UNARY_POSTFIX_OPERATION(SYMBOL, NAME) \
	template <typename T1, typename Tout>\
	void NAME (const T1& a, Tout& out)\
	{\
		assert(compatible(a,out));\
		auto size = out.size();\
		for (std::size_t i = 0; i < size; ++i)\
		{\
			out[i] =a[i] SYMBOL;\
		}\
	}
	#undef EACH_UNARY_POSTFIX_OPERATION


	#define EACH_UNARY_FUNCTION(NAME) \
	template <typename T1, typename Tout>\
	void NAME(const T1& a, Tout& out)\
	{\
		assert(compatible(a,out));\
		auto size = out.size();\
		for (std::size_t i = 0; i < size; ++i)\
		{\
			out[i] = NAME(a[i]);\
		}\
	}
	template <typename T1> inline auto abs  (const T1 a) { return std::abs(a); }
	template <typename T1> inline auto floor(const T1 a) { return std::floor(a); }
	template <typename T1> inline auto sign (const T1 a)	{ return (T1(0) < a) - (a < T1(0)); }
	template <typename T1> inline auto trunc(const T1 a) { return std::trunc(a); }
	template <typename T1> inline auto round(const T1 a) { return std::round(a); }
	template <typename T1> inline auto ceil (const T1 a) { return std::ceil(a); }
	template <typename T1> inline auto fract(const T1 a) { return a-floor(a); }
	template <typename T1> inline auto isnan(const T1 a) { return std::isnan(a); }
	template <typename T1> inline auto isinf(const T1 a) { return std::isinf(a); }
	template <typename T1> inline auto exp(const T1 a) { return std::exp(a); }
	template <typename T1> inline auto exp2(const T1 a) { return std::exp2(a); }
	template <typename T1> inline auto exp10(const T1 a) { return std::pow(a,T1(10)); }
	template <typename T1> inline auto log(const T1 a) { return std::log(a); }
	template <typename T1> inline auto log2(const T1 a) { return std::log2(a); }
	template <typename T1> inline auto log10(const T1 a) { return std::log10(a); }
	template <typename T1> inline auto sqrt(const T1 a) { return std::sqrt(a); }
	template <typename T1> inline auto inversesqrt(const T1 a) { return T1(1)/std::sqrt(a); }
	template <typename T1, int N> inline auto pow(const T1 a) { return std::pow(a,N); }
	template <typename T1> inline auto negate(const T1 a) { return !a; }
	EACH_UNARY_FUNCTION(abs)   
	EACH_UNARY_FUNCTION(floor) 
	EACH_UNARY_FUNCTION(sign)  
	EACH_UNARY_FUNCTION(trunc) 
	EACH_UNARY_FUNCTION(round) 
	EACH_UNARY_FUNCTION(ceil)  
	EACH_UNARY_FUNCTION(fract) 
	EACH_UNARY_FUNCTION(isnan) 
	EACH_UNARY_FUNCTION(isinf) 
	EACH_UNARY_FUNCTION(exp) 
	EACH_UNARY_FUNCTION(exp2) 
	EACH_UNARY_FUNCTION(exp10) 
	EACH_UNARY_FUNCTION(log) 
	EACH_UNARY_FUNCTION(log2) 
	EACH_UNARY_FUNCTION(log10) 
	EACH_UNARY_FUNCTION(sqrt) 
	EACH_UNARY_FUNCTION(inversesqrt) 
	EACH_UNARY_FUNCTION(negate) 
	#undef EACH_UNARY_FUNCTION


	#define EACH_BINARY_FUNCTION(NAME) \
	template <typename T1, typename T2, typename Tout>\
	void NAME(const T1& a, const T2& b, Tout& out)\
	{\
		assert(compatible(a,b,out));\
		auto size = out.size();\
		for (std::size_t i = 0; i < size; ++i)\
		{\
			out[i] = NAME(a[i], b[i]);\
		}\
	}
	template <typename T1, typename T2> inline auto mod(const T1 a, const T2 b){return a-b*floor(a/b);}
	template <typename T1, typename T2> inline auto min(const T1 a, const T2 b){return std::min(a,b);}
	template <typename T1, typename T2> inline auto max(const T1 a, const T2 b){return std::max(a,b);}
	template <typename T1, typename T2> inline auto pow(const T1 a, const T2 b){return std::pow(a,b);}
	template <typename T1, typename T2> inline auto distance(const T1 a, const T2 b){return std::abs(a-b);}
	template <typename T1, typename T2> inline auto equal(const T1 a, const T2 b){return a == b;}
	template <typename T1, typename T2> inline auto notEqual(const T1 a, const T2 b){return a != b;}
	template <typename T1, typename T2> inline auto greaterThan(const T1 a, const T2 b){return a > b;}
	template <typename T1, typename T2> inline auto lessThan(const T1 a, const T2 b){return a < b;}
	template <typename T1, typename T2> inline auto greaterThanEqual(const T1 a, const T2 b){return a >= b;}
	template <typename T1, typename T2> inline auto lessThanEqual(const T1 a, const T2 b){return a <= b;}
	template <typename T1, typename T2> inline auto unite(const T1 a, const T2 b){return a || b;}
	template <typename T1, typename T2> inline auto intersect(const T1 a, const T2 b){return a && b;}
	template <typename T1, typename T2> inline auto differ(const T1 a, const T2 b){return a && !b;}
	EACH_BINARY_FUNCTION(mod)  
	EACH_BINARY_FUNCTION(min)  
	EACH_BINARY_FUNCTION(max)  
	EACH_BINARY_FUNCTION(pow)  
	EACH_BINARY_FUNCTION(distance)  
	EACH_BINARY_FUNCTION(equal)
	EACH_BINARY_FUNCTION(notEqual)
	EACH_BINARY_FUNCTION(greaterThan)
	EACH_BINARY_FUNCTION(lessThan)
	EACH_BINARY_FUNCTION(greaterThanEqual)
	EACH_BINARY_FUNCTION(lessThanEqual)
	EACH_BINARY_FUNCTION(unite)
	EACH_BINARY_FUNCTION(intersect)
	EACH_BINARY_FUNCTION(differ)
	#undef EACH_BINARY_FUNCTION


	#define EACH_TRINARY_STD_FUNCTION(NAME) \
	template <typename T1, typename T2, typename T3, typename Tout>\
	void NAME(const T1& a, const T2& b, const T3& c, Tout& out)\
	{\
		assert(compatible(a,b,c,out));\
		auto size = out.size();\
		for (std::size_t i = 0; i < size; ++i)\
		{\
			out[i] = NAME(a[i], b[i], c[i]);\
		}\
	}
	template <typename T> inline auto mix(const T x, const T y, const T a) { return x*(T(1.0)-a) + y*a; }
	template <typename T> inline auto step(const T edge, const T x, const T out) { return x < edge? T(0) : T(1); }
	template <typename T>
	inline auto smoothstep(const T lo, const T hi, const T x)
	{
		T t = x<=lo? T(0) : x >= hi? T(1) : ((x-lo)/(hi-lo)); 
		return t*t*(T(3)-T(2)*t); 
	}
	template <typename T>
	inline auto linearstep(const T lo, const T hi, const T x)
	{
		return x<=lo? T(0) : x >= hi? T(1) : ((x-lo)/(hi-lo));
	}
	EACH_TRINARY_STD_FUNCTION(mix)  
	EACH_TRINARY_STD_FUNCTION(step) 
	EACH_TRINARY_STD_FUNCTION(smoothstep)
	EACH_TRINARY_STD_FUNCTION(linearstep)
	#undef EACH_TRINARY_STD_FUNCTION


	// SPECIAL FUNCTIONS THAT MUST BE DEFINED WITHOUT MACROS

	template <typename T1, typename T2, typename Tout>
	void equal(const T1& a, const T2& b, Tout& out, const float threshold)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			out[i] = distance(a[i], b[i]) < threshold;
		}
	}

	template <typename T1, typename T2, typename T3, typename Tout>
	void clamp(const T1& a, const T2& lo, const T3& hi, Tout& out)
	{
		assert(compatible(a,lo,hi,out));
		min(a, hi, out);
		max(out, lo, out);
	}

	template <typename T, typename Tint, typename Tout>
	void modf(const T& a, Tint& intout, Tout& fractout)
	{
		floor(a, fractout);
		auto size = fractout.size();
		for (std::size_t i = 0; i < size; ++i)
		{
			intout[i] = int(a[i]);
		}
	}

	/// Computes and returns a * b + c.
	template <typename T1, typename T2, typename T3, typename Tout>
	void fma(const T1& a, const T2& b, const T3& c, Tout& out)
	{
		assert(compatible(a,b,c,out));
		mult(a,   b, out);
		add (out, c, out);
	}


}
