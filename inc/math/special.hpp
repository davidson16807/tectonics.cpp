#pragma once

#include <cmath>
#include <algorithm>

namespace math{

	constexpr double pi = 3.14159265358979323846264338327950288419716939937;
	constexpr double right_angle = pi/2.0;

	#define MATH_UNARY_STD_WRAPPER(NAME) template <typename In1> inline auto NAME (const In1 a) { return std::NAME(a); }
	MATH_UNARY_STD_WRAPPER(abs)
	MATH_UNARY_STD_WRAPPER(floor)
	MATH_UNARY_STD_WRAPPER(trunc)
	MATH_UNARY_STD_WRAPPER(round)
	MATH_UNARY_STD_WRAPPER(ceil)
	MATH_UNARY_STD_WRAPPER(isnan)
	MATH_UNARY_STD_WRAPPER(isinf)
	MATH_UNARY_STD_WRAPPER(exp)
	MATH_UNARY_STD_WRAPPER(exp2)
	MATH_UNARY_STD_WRAPPER(log)
	MATH_UNARY_STD_WRAPPER(log2)
	MATH_UNARY_STD_WRAPPER(log10)
	MATH_UNARY_STD_WRAPPER(sqrt)
	MATH_UNARY_STD_WRAPPER(cbrt)
	MATH_UNARY_STD_WRAPPER(pow)
	MATH_UNARY_STD_WRAPPER(sin)
	MATH_UNARY_STD_WRAPPER(cos)
	MATH_UNARY_STD_WRAPPER(tan)
	MATH_UNARY_STD_WRAPPER(asin)
	MATH_UNARY_STD_WRAPPER(acos)
	MATH_UNARY_STD_WRAPPER(atan)
	MATH_UNARY_STD_WRAPPER(asinh)
	MATH_UNARY_STD_WRAPPER(acosh)
	MATH_UNARY_STD_WRAPPER(atanh)
	#undef MATH_UNARY_STD_WRAPPER

	#define MATH_BINARY_STD_WRAPPER(NAME) \
	template <typename In1, typename In2> inline auto NAME(const In1 a, const In2 b){return std::NAME(a,b);}
	MATH_BINARY_STD_WRAPPER(min)
	MATH_BINARY_STD_WRAPPER(max)
	MATH_BINARY_STD_WRAPPER(pow)
	MATH_BINARY_STD_WRAPPER(atan2)
	#undef MATH_BINARY_STD_WRAPPER

	#define MATH_TRINARY_STD_WRAPPER(NAME) \
	template <typename In1, typename In2, typename In3> inline auto NAME(const In1 a, const In2 b, const In3 c){return std::NAME(a,b,c);}
	MATH_TRINARY_STD_WRAPPER(fma)
	MATH_TRINARY_STD_WRAPPER(clamp)
	#undef MATH_TRINARY_STD_WRAPPER

	template <typename In1, typename In2> inline auto equal(const In1 a, const In2 b){return a == b;}
	template <typename In1, typename In2> inline auto notEqual(const In1 a, const In2 b){return a != b;}
	template <typename In1, typename In2> inline auto greaterThan(const In1 a, const In2 b){return a > b;}
	template <typename In1, typename In2> inline auto lessThan(const In1 a, const In2 b){return a < b;}
	template <typename In1, typename In2> inline auto greaterThanEqual(const In1 a, const In2 b){return a >= b;}
	template <typename In1, typename In2> inline auto lessThanEqual(const In1 a, const In2 b){return a <= b;}
	template <typename In1, typename In2> inline auto unite(const In1 a, const In2 b){return a || b;}
	template <typename In1, typename In2> inline auto intersect(const In1 a, const In2 b){return a && b;}
	template <typename In1, typename In2> inline auto differ(const In1 a, const In2 b){return a && !b;}

	template<typename In1, typename In2>
	inline constexpr auto remainder(const In1& a, const In2& b)
	{
	    return a - b * math::round(a / b);
	}

	template<typename In1, typename In2>
	inline constexpr auto residue(const In1& a, const In2& b)
	{
	    return a - b * math::floor(a / b);
	}

	template<typename In1, typename In2>
	inline constexpr auto modulus(const In1& a, const In2& b)
	{
	    return residue(residue(a,b) + b, b);
	}

	template<typename T>
	inline constexpr T maxabs(const T& a, const T& b)
	{
	    return math::max(-math::min(a,b), math::max(a,b));
	}

	template <typename T>
	inline constexpr T sign(const T x) {
		return x==T(0)? T(0) : x>T(0)? T(1) : T(-1);
	}

	template <typename T>
	inline constexpr T bitsign(const T x) {
		return std::signbit(x)? T(-1):T(1);
	}

	template <typename In1, typename In2> 
	inline auto distance(const In1 a, const In2 b){
		return std::abs(a-b);
	}

	template <typename In1, int N> inline auto pow(const In1 a) { return std::pow(a,N); }
	template <typename In1> inline auto fract(const In1 a) { return a-floor(a); }
	template <typename In1> inline auto inversesqrt(const In1 a) { return In1(1)/std::sqrt(a); }
	template <typename In1> inline auto negate(const In1 a) { return -a; }
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

	/*
	"erf" provides an approximation for the "error function": erf(x) = ∫ˣ exp(-t²)dt
	It is accurate to within 0.02% over the range [-10,10].
	*/
	template <typename T>
	constexpr auto erf(const T x) {
		const T a(0.1);
		const T b(1.1295);
	    return std::tanh(a*x*x*x + b*x);
	}

	/*
	"erfinv" provides an approximation for the "inverse error function": erf⁻¹(erf(x)) = x
	It is accurate to within 0.2% over its entire range. From Winitzki (2008).
	*/
	template <typename T>
	constexpr T erfinv(const T x) {
		T a(0.147);
		T f(std::log(T(1)-x*x));
		T g(T(2)/(pi*a) + f/T(2));
		return sign(x) * std::sqrt( std::sqrt(g*g - f/a) - g );
	}

	/*
	"probit" is the "quantile" of the normal distribution, 
	i.e. the inverse cumulative distribution function of the normal distribution.
	It returns the value that's associated for a given quantile of the normal distribution.
	*/
	template <typename T>
	constexpr auto probit(const T p) {
		return std::sqrt(T(2)) * erfinv(T(2)*p-1);
	}

    /*
    "mix" duplicates glm::mix so that we don't have to require glm just to use it.
    */
    template <typename Tout, typename Tin>
    constexpr Tout mix(const Tout edge0, const Tout edge1, const Tin a) {
        return edge0*(Tin(1)-a) + edge1*a;
    }

    /*
    "step" provides a non-glm alternative to glm::step()
    */
    template <typename Tin>
    constexpr auto step(const Tin edge, const Tin x) 
    {
	    return Tin(x >= edge);
    }

    /*
    "linearstep" provides a strictly linear alternative to glm::smoothstep()
    */
    template <typename Tin>
    constexpr auto linearstep(const Tin edge0, const Tin edge1, const Tin x) 
    {
        double fraction = double((x - edge0) / (edge1 - edge0));
        return fraction > 1.0? 1 : fraction < 0.0? 0 : fraction;
    }

    /*
    "smoothstep" provides a non-glm alternative to glm::smoothstep()
    */
    template <typename Tin>
    constexpr auto smoothstep(const Tin edge0, const Tin edge1, const Tin x) 
    {
	    auto t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	    return t * t * (3.0 - 2.0 * t);
    }

    /*
    "lerp" performs basic Linear piecewise intERPolation:
    given a list of control points mapping 1d space to 1d scalars, 
    and a point in 1d space, returns a 1d scalar that maps to the point
    */
    template <typename Tx, typename Ty>
    constexpr typename Ty::value_type lerp(
        const Tx control_points_x, 
        const Ty control_points_y, 
        const typename Tx::value_type x
    ) {
        typename Ty::value_type result = control_points_y[0];
        for (std::size_t i = 1; i < control_points_x.size(); i++) 
        {
            result = mix(result, control_points_y[i], linearstep(control_points_x[i-1], control_points_x[i], x));
        }
        return result;
    }

}
