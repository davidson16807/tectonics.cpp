#pragma once

#include <cmath>

namespace math{

	const double pi = 3.141592653589793238462643383279502884197169399;

	template<typename T>
	inline constexpr T remainder(const T& a, const T& b)
	{
	    return a - b * floor(a / b);
	}

	template<typename T>
	inline constexpr T modulus(const T& a, const T& b)
	{
	    return remainder(remainder(a,b) + b, b);
	}

	template <typename T>
	inline constexpr T sign(const T x) {
		return std::signbit(x)? T(1):T(-1);
	}

	template <typename In1, typename In2> 
	inline auto distance(const In1 a, const In2 b){
		return std::abs(a-b);
	}

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
    "linearstep" provides a strictly linear alternative to glm::smoothstep()
    */
    template <typename Tin>
    constexpr auto linearstep(const Tin edge0, const Tin edge1, const Tin x) 
    {
        double fraction = double((x - edge0) / (edge1 - edge0));
        return fraction > 1.0? 1 : fraction < 0.0? 0 : fraction;
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