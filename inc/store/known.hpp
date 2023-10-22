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

	#define KNOWN_UNARY_STD_WRAPPER(TITLE, LOWER)\
	template<typename A>\
	struct TITLE {\
		using size_type = typename A::size_type;\
		using value_type = typename A::value_type;\
		A a; \
		constexpr explicit TITLE(const A& a):       a(a) {}\
		constexpr inline auto size() const { return a.size(); }\
		constexpr inline auto operator()(const size_type i ) const { return std::LOWER(a[i]); }\
		constexpr inline auto operator[](const size_type i ) const { return std::LOWER(a[i]); }\
	};\
	template<typename A>\
	constexpr inline TITLE<A> LOWER(const A a)\
	{\
		return TITLE<A>(a);\
	}

	#define KNOWN_UNARY_FUNCTION(TITLE, LOWER)\
	template<typename A>\
	struct TITLE {\
		using size_type = typename A::size_type;\
		using value_type = typename A::value_type;\
		A a; \
		constexpr explicit TITLE(const A& a):       a(a) {}\
		constexpr inline auto size() const { return a.size(); }\
		constexpr inline auto operator()(const size_type i ) const { return LOWER(a[i]); }\
		constexpr inline auto operator[](const size_type i ) const { return LOWER(a[i]); }\
	};\
	template<typename A>\
	constexpr inline TITLE<A> LOWER(const A a)\
	{\
		return TITLE<A>(a);\
	}


	KNOWN_UNARY_STD_WRAPPER(Abs,   abs)
	KNOWN_UNARY_STD_WRAPPER(Floor, floor)
	KNOWN_UNARY_STD_WRAPPER(Trunc, trunc)
	KNOWN_UNARY_STD_WRAPPER(Round, round)
	KNOWN_UNARY_STD_WRAPPER(Ceil,  ceil)
	KNOWN_UNARY_STD_WRAPPER(Isnan, isnan)
	KNOWN_UNARY_STD_WRAPPER(Isinf, isinf)
	KNOWN_UNARY_STD_WRAPPER(Exp,   exp)
	KNOWN_UNARY_STD_WRAPPER(Exp2,  exp2)
	KNOWN_UNARY_STD_WRAPPER(Log,   log)
	KNOWN_UNARY_STD_WRAPPER(Log2,  log2)
	KNOWN_UNARY_STD_WRAPPER(Log10, log10)
	KNOWN_UNARY_STD_WRAPPER(Sqrt,  sqrt)
	// KNOWN_UNARY_STD_WRAPPER(Pow,   pow)
	KNOWN_UNARY_STD_WRAPPER(Sin,   sin)
	KNOWN_UNARY_STD_WRAPPER(Cos,   cos)
	KNOWN_UNARY_STD_WRAPPER(Tan,   tan)
	KNOWN_UNARY_STD_WRAPPER(Asin,  asin)
	KNOWN_UNARY_STD_WRAPPER(Acos,  acos)
	KNOWN_UNARY_STD_WRAPPER(Atan,  atan)
	KNOWN_UNARY_STD_WRAPPER(Asinh, asinh)
	KNOWN_UNARY_STD_WRAPPER(Acosh, acosh)
	KNOWN_UNARY_STD_WRAPPER(Atanh, atanh)

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

	template <typename In1, int N> inline auto pow(const In1 a) { return std::pow(a,N); }
	template <typename In1> inline auto sign (const In1 a)	{ return (In1(0) < a) - (a < In1(0)); }
	template <typename In1> inline auto fract(const In1 a) { return a-floor(a); }
	template <typename In1> inline auto inversesqrt(const In1 a) { return In1(1)/std::sqrt(a); }
	template <typename In1> inline auto negate(const In1 a) { return !a; }
	template <typename In1> inline auto exp10(const In1 a) { return std::pow(a,In1(10)); }
	template <typename In1> inline auto sec(const In1 a){ return In1(1)/std::cos(a);}
	template <typename In1> inline auto csc(const In1 a){ return In1(1)/std::sin(a);}
	template <typename In1> inline auto cot(const In1 a){ return In1(1)/std::tan(a);}
	// template <typename In1> inline auto asec(const In1 a){ return std::asec(a);}
	// template <typename In1> inline auto acsc(const In1 a){ return std::acsc(a);}
	// template <typename In1> inline auto acot(const In1 a){ return std::acot(a);}
	// template <typename In1> inline auto sinh(const In1 a){ return std::sinh(a);}
	// template <typename In1> inline auto cosh(const In1 a){ return std::cosh(a);}
	// template <typename In1> inline auto tanh(const In1 a){ return std::tanh(a);}
	// template <typename In1> inline auto asech(const In1 a){ return std::asech(a);}
	// template <typename In1> inline auto acsch(const In1 a){ return std::acsch(a);}
	// template <typename In1> inline auto acoth(const In1 a){ return std::acoth(a);}
	template <typename In1> inline auto radians(const In1 a){ return In1(math::pi/180.) * a;}
	template <typename In1> inline auto degrees(const In1 a){ return In1(180./math::pi) * a;}

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
	#undef KNOWN_UNARY_STD_WRAPPER

	#define KNOWN_BINARY_STD_WRAPPER(TITLE, LOWER)\
	template<typename A, typename B>\
	struct TITLE {\
		using size_type = typename std::size_t;\
		using value_type = typename A::value_type;\
		A a; B b;\
		constexpr explicit TITLE(const A& a, const B& b): a(a), b(b) {}\
		constexpr inline auto size() const { return std::max(a.size(), b.size()); }\
		constexpr inline auto operator()(const size_type i ) const { return std::LOWER(a(i),b(i)); }\
		constexpr inline auto operator[](const size_type i ) const { return std::LOWER(a(i),b(i)); }\
	};\
	template<typename A, typename B>\
	constexpr inline TITLE<A,B> LOWER(const A a, const B b)\
	{\
		return TITLE<A,B>(a, b);\
	}

	#define KNOWN_BINARY_FUNCTION(TITLE, LOWER)\
	template<typename A, typename B>\
	struct TITLE {\
		using size_type = typename std::size_t;\
		using value_type = typename A::value_type;\
		A a; B b;\
		constexpr explicit TITLE(const A& a, const B& b): a(a), b(b) {}\
		constexpr inline auto size() const { return std::max(a.size(), b.size()); }\
		constexpr inline auto operator()(const size_type i ) const { return each::LOWER(a(i),b(i)); }\
		constexpr inline auto operator[](const size_type i ) const { return each::LOWER(a(i),b(i)); }\
	};\
	template<typename A, typename B>\
	constexpr inline TITLE<A,B> LOWER(const A a, const B b)\
	{\
		return TITLE<A,B>(a, b);\
	}

	KNOWN_BINARY_STD_WRAPPER(Min,   min)
	KNOWN_BINARY_STD_WRAPPER(Max,   max)
	KNOWN_BINARY_STD_WRAPPER(Pow,   pow)
	KNOWN_BINARY_STD_WRAPPER(Atan2, atan2)

	// KNOWN_BINARY_FUNCTION(Min,   min)  
	// KNOWN_BINARY_FUNCTION(Max,   max)  
	// KNOWN_BINARY_FUNCTION(Pow,   pow)  
	// KNOWN_BINARY_FUNCTION(Atan2, atan2)

	// template <typename In1, typename In2> inline auto mod(const In1 a, const In2 b){return a-b*floor(a/b);}
	// template <typename In1, typename In2> inline auto distance(const In1 a, const In2 b){return std::abs(a-b);}
	// template <typename In1, typename In2> inline auto equal(const In1 a, const In2 b){return a == b;}
	// template <typename In1, typename In2> inline auto notEqual(const In1 a, const In2 b){return a != b;}
	// template <typename In1, typename In2> inline auto greaterThan(const In1 a, const In2 b){return a > b;}
	// template <typename In1, typename In2> inline auto lessThan(const In1 a, const In2 b){return a < b;}
	// template <typename In1, typename In2> inline auto greaterThanEqual(const In1 a, const In2 b){return a >= b;}
	// template <typename In1, typename In2> inline auto lessThanEqual(const In1 a, const In2 b){return a <= b;}
	// template <typename In1, typename In2> inline auto unite(const In1 a, const In2 b){return a || b;}
	// template <typename In1, typename In2> inline auto intersect(const In1 a, const In2 b){return a && b;}
	// template <typename In1, typename In2> inline auto differ(const In1 a, const In2 b){return a && !b;}

	KNOWN_BINARY_FUNCTION(Mod,              mod)  
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

	#undef KNOWN_BINARY_FUNCTION
	#undef KNOWN_BINARY_STD_WRAPPER

	#define KNOWN_TRINARY_FUNCTION(TITLE, LOWER)\
	template<typename A, typename B, typename C>\
	struct TITLE {\
		using size_type = typename std::size_t;\
		using value_type = typename A::value_type;\
		A a; B b; C c;\
		constexpr explicit TITLE(const A& a, const B& b, const C& c): a(a),   b(b),   c(c) {}\
		constexpr inline auto size() const { return std::max(a.size(), b.size(), c.size()); }\
		constexpr inline auto operator()(const size_type i ) const { return LOWER(a(i),b(i),c(i)); }\
		constexpr inline auto operator[](const size_type i ) const { return LOWER(a(i),b(i),c(i)); }\
	};

	#define KNOWN_TRINARY_STD_WRAPPER(TITLE, LOWER)\
	template<typename A, typename B, typename C>\
	struct TITLE {\
		using size_type = typename std::size_t;\
		using value_type = typename A::value_type;\
		A a; B b; C c;\
		constexpr explicit TITLE(const A& a, const B& b, const C& c): a(a),   b(b),   c(c) {}\
		constexpr inline auto size() const { return std::max(a.size(), b.size(), c.size()); }\
		constexpr inline auto operator()(const size_type i ) const { return std::LOWER(a(i),b(i),c(i)); }\
		constexpr inline auto operator[](const size_type i ) const { return std::LOWER(a(i),b(i),c(i)); }\
	};

	KNOWN_TRINARY_STD_WRAPPER(Clamp, clamp)
	KNOWN_TRINARY_STD_WRAPPER(Fma,   fma)
	#undef KNOWN_TRINARY_STD_WRAPPER


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

