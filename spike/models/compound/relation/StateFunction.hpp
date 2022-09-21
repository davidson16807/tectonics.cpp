#pragma once

// std libraries
#include <functional>          // function

// in-house libraries
#include <math/lerp.hpp>
#include <models/compound/property/phase.hpp>

namespace compound { 
namespace relation {
	template<typename T1>
    using StateFunction = std::function<T1(
    	const si::pressure<double> p, 
    	const si::temperature<double> T)>;

    template<typename Ty>
    relation::StateFunction<Ty> state_invariant(
        const Ty value
    ){
        return relation::StateFunction<Ty>(
            [value]
            (const si::pressure<double> p, 
             const si::temperature<double> T)
            {
                return value;
            }
        );
    }

    // 7 uses
    
    relation::StateFunction<int> get_simon_glatzel_phase_function( 
        const si::pressure<double>        p0, /*triple point pressure*/
        const si::temperature<double>     t0, /*triple point temperature*/
        const si::pressure<double>        pc, /*critical point pressure*/
        const si::temperature<double>     tc, /*critical point temperature*/
        const si::specific_energy<double> L,  /*latent heat of vaporization at boiling point*/
        const si::molar_mass<double>      M,  /*molar mass*/
        const si::pressure<double>        a,  /*simon glatzel slope*/
        const si::pressure<double>        b,  /*simon glatzel intercept*/
        const float               c   /*simon glatzel exponent*/
    ){
        return relation::StateFunction<int>(
            [p0, t0, pc, tc, L,  M, a, b, c]
            (const si::pressure<double> p, 
             const si::temperature<double> T)
            {
                return property::get_simon_glatzel_phase(p, T, p0, t0, pc, tc, L,  M, a, b, c);
            }
        );
    }

    // 175 uses
    template<typename Tx, typename Ty>
    relation::StateFunction<Ty> get_interpolated_temperature_function(
        const Tx xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        return relation::StateFunction<Ty>(
            [xunits, yunits, xs, ys]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                return math::lerp(xs, ys, T/xunits) * yunits;
            }
        );
    }


    // 3 uses: gold, silver, and copper, for dynamic viscosity of liquids
    template<typename Tx, typename Ty>
    relation::StateFunction<Ty> get_interpolated_inverse_temperature_function( 
        const Tx xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        return relation::StateFunction<Ty>(
            [xunits, yunits, xs, ys]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                return math::lerp(xs, ys, 1.0/(T/xunits)) * yunits;
            }
        );
    }
    
    /*
    `get_dippr_quartic_temperature_function_100()` is equivalent to dippr function 100,
    for liquid thermal conductivity, heat capacity, and solid density
    */
    // 26 uses, for liquid thermal conductivity and heat capacity
    template<typename Tx, typename Ty>
    relation::StateFunction<Ty> get_dippr_quartic_temperature_function_100( 
        const Tx Tunits, const Ty yunits,
        const double intercept, const double slope, const double square, const double cube, const double fourth,
        const double Tmin, double Tmax
    ){
        return relation::StateFunction<Ty>(
            [Tunits, yunits, Tmin, Tmax, intercept, slope, square, cube, fourth]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return (intercept + slope*t + square*t*t + cube*t*t*t + fourth*t*t*t*t)*yunits;
            }
        );
    }

}}