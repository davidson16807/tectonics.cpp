#include <array>

/*
"math" provides interpolation functionality that goes beyond that which is provided by glm
*/
namespace math
{
    /*
    "mix" duplicates glm::mix so that we don't have to require glm just to use it.
    It's in a private namespace to discourage its use outside of the library.
    */
    template <typename T>
    constexpr T mix(const T x, const T y, const T a) {
        return x*(T(1)-a) + y*a;
    }

    /*
    "linearstep" provides a strictly linear alternative to glm::smoothstep()
    */
    template <typename T>
    constexpr T linearstep(const T edge0, const T edge1, const T x) 
    {
        T fraction = (x - edge0) / (edge1 - edge0);
        return fraction > T(1)? T(1) : fraction < T(0)? T(0) : fraction;
    }

    /*
    "lerp" performs basic Linear piecewise intERPolation:
    given a list of control points mapping 1d space to 1d scalars, 
    and a point in 1d space, returns a 1d scalar that maps to the point
    */
    template <typename T>
    constexpr typename T::value_type lerp(
        const T control_points_x, 
        const T control_points_y, 
        const typename T::value_type x
    ) {
        typename T::value_type result = control_points_y[0];
        for (std::size_t i = 1; i < control_points_x.size(); i++) 
        {
            result = mix(result, control_points_y[i], linearstep(control_points_x[i-1], control_points_x[i], x));
        }
        return result;
    }

    /*
    "integral_of_lerp" finds the integral of the lerp() function from a to b
    */
    template <typename T>
    constexpr typename T::value_type integral_of_lerp(
        const T control_points_x, 
        const T control_points_y, 
        const typename T::value_type a, 
        const typename T::value_type b
    ) {
        std::size_t li = control_points_x.size();
        typename T::value_type fa, fb, ya, yb, dydf, dxdf;
        typename T::value_type I  = (a < control_points_x[0]?    std::min(control_points_x[0] - a,    b-a) * control_points_y[0]    : 0)
             + (b > control_points_x[li-1]? std::min(b - control_points_x[li-1], b-a) * control_points_y[li-1] : 0);
        for (std::size_t i = 1; i < li; i++) 
        {
            // "f*" is fraction through the control point for a or b
            fa = linearstep(control_points_x[i-1], control_points_x[i], a);
            fb = linearstep(control_points_x[i-1], control_points_x[i], b);
            dydf = control_points_y[i] - control_points_y[i-1];
            dxdf = control_points_x[i] - control_points_x[i-1];
            yb = dydf * fb * fb / 2.0f + fb * control_points_y[i-1];
            ya = dydf * fa * fa / 2.0f + fa * control_points_y[i-1];
            I += (yb - ya) * dxdf;
        }
        return I;
    }
}