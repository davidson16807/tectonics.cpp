#pragma once

// C libraries
#include <cmath>     /* std math */
#include <assert.h>  /* assert */

// in-house libraries
#include <math/special.hpp>

namespace known
{

	#define KNOWN_BINARY_OPERATION(SYMBOL, TITLE, LOWER)\
	template<typename A, typename B>\
	struct TITLE {\
		using size_type = typename std::size_t;\
		using value_type = typename A::value_type;\
		A a; B b;\
		constexpr explicit TITLE(const A& a, const B& b): a(a), b(b) {}\
		constexpr inline auto size() const { return std::max(a.size(), b.size()); }\
		constexpr inline auto operator()(const size_type i ) const { return a(i) SYMBOL b(i); }\
		constexpr inline auto operator[](const size_type i ) const { return a(i) SYMBOL b(i); }\
	};\
	template<typename A, typename B>\
	constexpr inline TITLE<A,B> LOWER(const A a, const B b)\
	{\
		return TITLE<A,B>(a, b);\
	}

	KNOWN_BINARY_OPERATION(+,  Add,  add)
	KNOWN_BINARY_OPERATION(-,  Sub,  sub)
	KNOWN_BINARY_OPERATION(*,  Mult, mult)
	KNOWN_BINARY_OPERATION(/,  Div,  div)

	#undef KNOWN_BINARY_OPERATION

	#define KNOWN_UNARY_PREFIX_OPERATION(SYMBOL, TITLE, LOWER)\
	template<typename A>\
	struct TITLE {\
		using size_type = typename A::size_type;\
		using value_type = typename A::value_type;\
		A a; \
		constexpr explicit TITLE(const A& a):       a(a) {}\
		constexpr inline auto size() const { return a.size(); }\
		constexpr inline auto operator()(const size_type i ) const { return SYMBOL a[i]; }\
		constexpr inline auto operator[](const size_type i ) const { return SYMBOL a[i]; }\
	};\
	template<typename A>\
	constexpr inline TITLE<A> LOWER(const A a)\
	{\
		return TITLE<A>(a);\
	}
	// KNOWN_BINARY_OPERATION(++,  increment)
	// KNOWN_BINARY_OPERATION(--,  decrement)
	// KNOWN_BINARY_OPERATION(-,   negate)
	#undef KNOWN_UNARY_PREFIX_OPERATION

	#define KNOWN_UNARY_POSTFIX_OPERATION(SYMBOL, TITLE, LOWER)\
	template<typename A>\
	struct TITLE {\
		using size_type = typename A::size_type;\
		using value_type = typename A::value_type;\
		A a; \
		constexpr explicit TITLE(const A& a):       a(a) {}\
		constexpr inline auto size() const { return a.size(); }\
		constexpr inline auto operator()(const size_type i ) const { return a[i] SYMBOL; }\
		constexpr inline auto operator[](const size_type i ) const { return a[i] SYMBOL; }\
	};\
	template<typename A>\
	constexpr inline TITLE<A> LOWER(const A a)\
	{\
		return TITLE<A>(a);\
	}
	#undef KNOWN_UNARY_POSTFIX_OPERATION

	#define KNOWN_UNARY_FUNCTION(TITLE, LOWER)\
	template<typename A>\
	struct TITLE {\
		using size_type = typename A::size_type;\
		using value_type = typename A::value_type;\
		A a; \
		constexpr explicit TITLE(const A& a):       a(a) {}\
		constexpr inline auto size() const { return a.size(); }\
		constexpr inline auto operator()(const size_type i ) const { return math::LOWER(a[i]); }\
		constexpr inline auto operator[](const size_type i ) const { return math::LOWER(a[i]); }\
	};\
	template<typename A>\
	constexpr inline TITLE<A> LOWER(const A a)\
	{\
		return TITLE<A>(a);\
	}

	KNOWN_UNARY_FUNCTION(Abs,   abs)
	KNOWN_UNARY_FUNCTION(Floor, floor)
	KNOWN_UNARY_FUNCTION(Trunc, trunc)
	KNOWN_UNARY_FUNCTION(Round, round)
	KNOWN_UNARY_FUNCTION(Ceil,  ceil)
	KNOWN_UNARY_FUNCTION(Isnan, isnan)
	KNOWN_UNARY_FUNCTION(Isinf, isinf)
	KNOWN_UNARY_FUNCTION(Exp,   exp)
	KNOWN_UNARY_FUNCTION(Exp2,  exp2)
	KNOWN_UNARY_FUNCTION(Log,   log)
	KNOWN_UNARY_FUNCTION(Log2,  log2)
	KNOWN_UNARY_FUNCTION(Log10, log10)
	KNOWN_UNARY_FUNCTION(Sqrt,  sqrt)
	// KNOWN_UNARY_FUNCTION(Pow,   pow)
	KNOWN_UNARY_FUNCTION(Sin,   sin)
	KNOWN_UNARY_FUNCTION(Cos,   cos)
	KNOWN_UNARY_FUNCTION(Tan,   tan)
	KNOWN_UNARY_FUNCTION(Asin,  asin)
	KNOWN_UNARY_FUNCTION(Acos,  acos)
	KNOWN_UNARY_FUNCTION(Atan,  atan)
	KNOWN_UNARY_FUNCTION(Asinh, asinh)
	KNOWN_UNARY_FUNCTION(Acosh, acosh)
	KNOWN_UNARY_FUNCTION(Atanh, atanh)

	// KNOWN_UNARY_FUNCTION(Abs,   abs)
	// KNOWN_UNARY_FUNCTION(Floor, floor)
	// KNOWN_UNARY_FUNCTION(Trunc, trunc)
	// KNOWN_UNARY_FUNCTION(Round, round)
	// KNOWN_UNARY_FUNCTION(Ceil,  ceil)
	// KNOWN_UNARY_FUNCTION(Isnan, isnan)
	// KNOWN_UNARY_FUNCTION(Isinf, isinf)
	// KNOWN_UNARY_FUNCTION(Exp,   exp)
	// KNOWN_UNARY_FUNCTION(Exp2,  exp2)
	// KNOWN_UNARY_FUNCTION(Log,   log)
	// KNOWN_UNARY_FUNCTION(Log2,  log2)
	// KNOWN_UNARY_FUNCTION(Log10, log10)
	// KNOWN_UNARY_FUNCTION(Sqrt,  sqrt)
	// KNOWN_UNARY_FUNCTION(Cbrt,  cbrt)
	// KNOWN_UNARY_FUNCTION(Sin,   sin)
	// KNOWN_UNARY_FUNCTION(Cos,   cos)
	// KNOWN_UNARY_FUNCTION(Tan,   tan)
	// KNOWN_UNARY_FUNCTION(Asinh, asinh)
	// KNOWN_UNARY_FUNCTION(Acosh, acosh)
	// KNOWN_UNARY_FUNCTION(Atanh, atanh)

	// KNOWN_UNARY_FUNCTION(Pow,         pow)
	KNOWN_UNARY_FUNCTION(Sign,        sign)
	// KNOWN_UNARY_FUNCTION(Fract,       fract)
	KNOWN_UNARY_FUNCTION(Inversesqrt, inversesqrt)
	KNOWN_UNARY_FUNCTION(Negate,      negate)
	KNOWN_UNARY_FUNCTION(Exp10,       exp10)
	KNOWN_UNARY_FUNCTION(Sec,         sec)
	KNOWN_UNARY_FUNCTION(Csc,         csc)
	KNOWN_UNARY_FUNCTION(Cot,         cot)
	// KNOWN_UNARY_FUNCTION(Sinh,        sinh)
	// KNOWN_UNARY_FUNCTION(Cosh,        cosh)
	// KNOWN_UNARY_FUNCTION(Tanh,        tanh)
	// KNOWN_UNARY_FUNCTION(Asec,        asec)
	// KNOWN_UNARY_FUNCTION(Acsc,        acsc)
	// KNOWN_UNARY_FUNCTION(Acot,        acot)
	// KNOWN_UNARY_FUNCTION(Asech,       asech)
	// KNOWN_UNARY_FUNCTION(Acsch,       acsch)
	// KNOWN_UNARY_FUNCTION(Acoth,       acoth)
	KNOWN_UNARY_FUNCTION(Radians,     radians)
	KNOWN_UNARY_FUNCTION(Degrees,     degrees)

	#undef KNOWN_UNARY_FUNCTION

	#define KNOWN_BINARY_FUNCTION(TITLE, LOWER)\
	template<typename A, typename B>\
	struct TITLE {\
		using size_type = typename std::size_t;\
		using value_type = typename A::value_type;\
		A a; B b;\
		constexpr explicit TITLE(const A& a, const B& b): a(a), b(b) {}\
		constexpr inline auto size() const { return std::max(a.size(), b.size()); }\
		constexpr inline auto operator()(const size_type i ) const { return math::LOWER(a(i),b(i)); }\
		constexpr inline auto operator[](const size_type i ) const { return math::LOWER(a(i),b(i)); }\
	};\
	template<typename A, typename B>\
	constexpr inline TITLE<A,B> LOWER(const A a, const B b)\
	{\
		return TITLE<A,B>(a, b);\
	}


	// KNOWN_BINARY_FUNCTION(Min,   min)  
	// KNOWN_BINARY_FUNCTION(Max,   max)  
	// KNOWN_BINARY_FUNCTION(Pow,   pow)  
	// KNOWN_BINARY_FUNCTION(Atan2, atan2)

	KNOWN_BINARY_FUNCTION(Residue,          residue)
	KNOWN_BINARY_FUNCTION(Modulus,          modulus)
	KNOWN_BINARY_FUNCTION(Remainder,        remainder)
	KNOWN_BINARY_FUNCTION(Distance,         distance)  
	KNOWN_BINARY_FUNCTION(Equal,            equal)
	KNOWN_BINARY_FUNCTION(NotEqual,         notEqual)
	KNOWN_BINARY_FUNCTION(GreaterThan,      greaterThan)
	KNOWN_BINARY_FUNCTION(LessThan,         lessThan)
	KNOWN_BINARY_FUNCTION(GreaterThanEqual, greaterThanEqual)
	KNOWN_BINARY_FUNCTION(LessThanEqual,    lessThanEqual)
	KNOWN_BINARY_FUNCTION(Unite,            unite)
	KNOWN_BINARY_FUNCTION(Intersect,        intersect)
	KNOWN_BINARY_FUNCTION(Differ,           differ)
	KNOWN_BINARY_FUNCTION(Min,              min)
	KNOWN_BINARY_FUNCTION(Max,              max)
	KNOWN_BINARY_FUNCTION(Pow,              pow)
	KNOWN_BINARY_FUNCTION(Atan2,            atan2)

	#undef KNOWN_BINARY_FUNCTION

	#define KNOWN_TRINARY_FUNCTION(TITLE, LOWER)\
	template<typename A, typename B, typename C>\
	struct TITLE {\
		using size_type = typename std::size_t;\
		using value_type = typename A::value_type;\
		A a; B b; C c;\
		constexpr explicit TITLE(const A& a, const B& b, const C& c): a(a),   b(b),   c(c) {}\
		constexpr inline auto size() const { return std::max(a.size(), b.size(), c.size()); }\
		constexpr inline auto operator()(const size_type i ) const { return math::LOWER(a(i),b(i),c(i)); }\
		constexpr inline auto operator[](const size_type i ) const { return math::LOWER(a(i),b(i),c(i)); }\
	};

	template <typename T> inline auto equal(const T a, const T b, const T threshold) { return distance(a,b)<threshold; }
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
	KNOWN_TRINARY_FUNCTION(Mix,        mix)  
	KNOWN_TRINARY_FUNCTION(Step,       step) 
	KNOWN_TRINARY_FUNCTION(Smoothstep, smoothstep)
	KNOWN_TRINARY_FUNCTION(Linearstep, linearstep)
	KNOWN_TRINARY_FUNCTION(Clamp,      clamp)
	KNOWN_TRINARY_FUNCTION(Fma,        fma)
	#undef KNOWN_TRINARY_FUNCTION

	// SPECIAL FUNCTIONS THAT MUST BE DEFINED WITHOUT MACROS

	/// returns a vector that stores a
	template <typename A, typename I>
	auto store(const I size, const A& a)
	{
		using T = typename A::value_type;
		std::vector<T> out(size);
		for (I i = 0; i < size; ++i)
		{
			out[i] = a[i];
		}
		return out;
	}

}

