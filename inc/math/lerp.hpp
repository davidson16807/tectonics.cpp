#pragma once

#include <array>

/*
"math" provides interpolation functionality that goes beyond that which is provided by glm
*/
namespace math
{
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