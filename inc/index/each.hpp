#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include <math/special.hpp>

namespace each
{

	template <typename In1, typename Out>
	bool compatible(const In1& a, Out& out)
	{
		return 
			out.size() >= a.size() &&
			out.size() % a.size() == 0 &&
			a.size() > 0 &&
		true;
	}

	template <typename In1, typename In2, typename Out>
	bool compatible(const In1& a, const In2& b, Out& out)
	{
		return 
			out.size() >= a.size() &&
			out.size() >= b.size() &&
			out.size() % a.size() == 0 &&
			out.size() % b.size() == 0 &&
			a.size() > 0 &&
			b.size() > 0 &&
		true;
	}

	template <typename In1, typename In2, typename In3, typename Out>
	bool compatible(const In1& a, const In2& b, const In3& c, Out& out)
	{
		return 
			out.size() >= a.size() &&
			out.size() >= b.size() &&
			out.size() >= c.size() &&
			out.size() % a.size() == 0 &&
			out.size() % b.size() == 0 &&
			out.size() % c.size() == 0 &&
			a.size() > 0 &&
			b.size() > 0 &&
			c.size() > 0 &&
		true;
	}

	#define EACH_BINARY_OPERATION(SYMBOL, NAME) \
	template <typename In1, typename In2, typename Out>\
	void NAME (const In1& a, const In2& b, Out& out)\
	{\
		assert(compatible(a,b,out));\
		auto size = out.size();\
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)\
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
	template <typename In1, typename Out>\
	void NAME (const In1& a, Out& out)\
	{\
		assert(compatible(a,out));\
		auto size = out.size();\
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)\
		{\
			out[i] = SYMBOL a[i];\
		}\
	}
	#undef EACH_UNARY_PREFIX_OPERATION


	#define EACH_UNARY_POSTFIX_OPERATION(SYMBOL, NAME) \
	template <typename In1, typename Out>\
	void NAME (const In1& a, Out& out)\
	{\
		assert(compatible(a,out));\
		auto size = out.size();\
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)\
		{\
			out[i] =a[i] SYMBOL;\
		}\
	}
	#undef EACH_UNARY_POSTFIX_OPERATION

	#define EACH_UNARY_FUNCTION(NAME) \
	template <typename In1, typename Out>\
	void NAME(const In1& a, Out& out)\
	{\
		assert(compatible(a,out));\
		auto size = out.size();\
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)\
		{\
			out[i] = math::NAME(a[i]);\
		}\
	}

	EACH_UNARY_FUNCTION(abs)
	EACH_UNARY_FUNCTION(floor)
	EACH_UNARY_FUNCTION(trunc)
	EACH_UNARY_FUNCTION(round)
	EACH_UNARY_FUNCTION(ceil)
	EACH_UNARY_FUNCTION(isnan)
	EACH_UNARY_FUNCTION(isinf)
	EACH_UNARY_FUNCTION(exp)
	EACH_UNARY_FUNCTION(exp2)
	EACH_UNARY_FUNCTION(log)
	EACH_UNARY_FUNCTION(log2)
	EACH_UNARY_FUNCTION(log10)
	EACH_UNARY_FUNCTION(sqrt)
	EACH_UNARY_FUNCTION(cbrt)
	EACH_UNARY_FUNCTION(sin)
	EACH_UNARY_FUNCTION(cos)
	EACH_UNARY_FUNCTION(tan)
	EACH_UNARY_FUNCTION(asinh)
	EACH_UNARY_FUNCTION(acosh)
	EACH_UNARY_FUNCTION(atanh)


	// EACH_UNARY_FUNCTION(pow)
	EACH_UNARY_FUNCTION(sign)
	EACH_UNARY_FUNCTION(fract)
	EACH_UNARY_FUNCTION(inversesqrt)
	EACH_UNARY_FUNCTION(negate)
	EACH_UNARY_FUNCTION(exp10)
	EACH_UNARY_FUNCTION(sec)
	EACH_UNARY_FUNCTION(csc)
	EACH_UNARY_FUNCTION(cot)
	// EACH_UNARY_FUNCTION(sinh)
	// EACH_UNARY_FUNCTION(cosh)
	// EACH_UNARY_FUNCTION(tanh)
	// EACH_UNARY_FUNCTION(asec)
	// EACH_UNARY_FUNCTION(acsc)
	// EACH_UNARY_FUNCTION(acot)
	// EACH_UNARY_FUNCTION(asech)
	// EACH_UNARY_FUNCTION(acsch)
	// EACH_UNARY_FUNCTION(acoth)
	EACH_UNARY_FUNCTION(radians)
	EACH_UNARY_FUNCTION(degrees)

	#undef EACH_UNARY_FUNCTION

	#define EACH_BINARY_FUNCTION(NAME) \
	template <typename In1, typename In2, typename Out>\
	void NAME(const In1& a, const In2& b, Out& out)\
	{\
		assert(compatible(a,b,out));\
		auto size = out.size();\
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)\
		{\
			out[i] = math::NAME(a[i], b[i]);\
		}\
	}

	EACH_BINARY_FUNCTION(min)  
	EACH_BINARY_FUNCTION(max)  
	EACH_BINARY_FUNCTION(pow)  
	EACH_BINARY_FUNCTION(atan2)

	EACH_BINARY_FUNCTION(modulus)  
	EACH_BINARY_FUNCTION(residue)  
	EACH_BINARY_FUNCTION(remainder)
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


	#define EACH_TRINARY_FUNCTION(NAME) \
	template <typename In1, typename In2, typename In3, typename Out>\
	void NAME(const In1& a, const In2& b, const In3& c, Out& out)\
	{\
		assert(compatible(a,b,c,out));\
		auto size = out.size();\
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)\
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
	EACH_TRINARY_FUNCTION(mix)  
	EACH_TRINARY_FUNCTION(step) 
	EACH_TRINARY_FUNCTION(smoothstep)
	EACH_TRINARY_FUNCTION(linearstep)
	#undef EACH_TRINARY_FUNCTION


	// SPECIAL FUNCTIONS THAT MUST BE DEFINED WITHOUT MACROS

	template <typename In1, typename In2, typename Out>
	void equal(const In1& a, const In2& b, Out& out, const float threshold)
	{
		assert(compatible(a,b,out));
		auto size = a.size();
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)
		{
			out[i] = distance(a[i], b[i]) < threshold;
		}
	}

	template <typename In1, typename In2, typename In3, typename Out>
	void clamp(const In1& a, const In2& lo, const In3& hi, Out& out)
	{
		assert(compatible(a,lo,hi,out));
		each::min(a, hi, out);
		each::max(out, lo, out);
	}

	template <typename T, typename Tint, typename Out>
	void modf(const T& a, Tint& intout, Out& fractout)
	{
		floor(a, fractout);
		auto size = fractout.size();
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)
		{
			intout[i] = int(a[i]);
		}
	}

	/// Computes and returns a * b + c.
	template <typename In1, typename In2, typename In3, typename Out>
	void fma(const In1& a, const In2& b, const In3& c, Out& out)
	{
		assert(compatible(a,b,c,out));
		mult(a,   b, out);
		add (out, c, out);
	}


	/// Computes and returns f(a[i]).
	template <typename In1, typename F, typename Out>
	void map(const F& f, const In1& a, Out& out)
	{
		assert(compatible(a,out));
		auto size = out.size();
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)
		{
			out[i] = f(a[i]);
		}
	}

	/// Computes and returns f[a[i]].
	template <typename In1, typename F, typename Out>
	void get(const F& f, const In1& a, Out& out)
	{
		assert(compatible(a,out));
		auto size = out.size();
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)
		{
			out[i] = f[a[i]];
		}
	}

	/// Computes and returns f[a[i]].
	template <typename In1, typename Out>
	void copy(const In1& a, Out& out)
	{
		assert(compatible(a,out));
		auto size = out.size();
		using size_type = typename Out::size_type;\
		for (size_type i = 0; i < size; ++i)
		{
			out[i] = a[i];
		}
	}

}

