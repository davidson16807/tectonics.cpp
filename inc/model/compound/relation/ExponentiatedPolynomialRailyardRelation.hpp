#pragma once

// std libraries
#include <cmath>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/PolynomialRailyard_library.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/PolynomialRailyard_library.hpp>
#include <unit/si.hpp>

namespace compound {
namespace relation {

    /*
    `ExponentiatedPolynomialRailyardRelation` represents an arbitrary function that uses a railyard to map quantities from one unit to another
    See `math/expression/Railyard.hpp` for the definition and motivation of a "railyard".
    */
    template<typename X, typename Y, int Plo, int Phi>
    struct ExponentiatedPolynomialRailyardRelation
    {
        analytic::PolynomialRailyard<float, Plo, Phi> yard;
        X xunits;
        Y yunits;
        
        using value_type = Y;

        // zero constructor
        constexpr ExponentiatedPolynomialRailyardRelation():
            yard(),
            xunits(1.0),
            yunits(1.0)
        {
        }

        // constant constructor
        constexpr ExponentiatedPolynomialRailyardRelation(const Y& other):
            yard(std::log(other/Y(1.0))),
            xunits(1.0),
            yunits(1.0)
        {
        }

        template<typename T>
        constexpr ExponentiatedPolynomialRailyardRelation(
            const analytic::PolynomialRailyard<T, Plo, Phi> yard,
            const X xunits,
            const Y yunits
        ):
            yard(yard),
            xunits(xunits),
            yunits(yunits)
        {
        }

        template<typename T, int Qlo, int Qhi>
        explicit constexpr ExponentiatedPolynomialRailyardRelation(
            const analytic::PolynomialRailyard<T, Qlo, Qhi> yard,
            const X xunits,
            const Y yunits
        ):
            yard(yard),
            xunits(xunits),
            yunits(yunits)
        {
        }

        // copy constructor
        template<int Qlo, int Qhi>
        constexpr ExponentiatedPolynomialRailyardRelation(
            const ExponentiatedPolynomialRailyardRelation<X, Y, Qlo, Qhi>& other
        ):
            yard(other.yard),
            xunits(other.xunits),
            yunits(other.yunits)
        {
        }

        constexpr ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi>& operator=(const ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi>& other)
        {
            yard = other.yard;
            xunits = other.xunits;
            yunits = other.yunits;
            return *this;
        }

        constexpr ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi>& operator=(const Y& other)
        {
            yard = other/Y(1.0);
            xunits = X(1.0);
            yunits = Y(1.0);
            return *this;
        }

        Y operator()(const X x) const
        {
            return std::exp(yard(float(x/xunits))) * yunits;
        }

        ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi> restriction(
            const X xlo, const X xhi,
            const float known_max_fractional_error
        ) const
        {
            return ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi>(restriction(yard, xlo/xunits, xhi/xunits), xunits, yunits);
        }

        ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi>& operator*=(const float scalar)
        {
            yard += scalar;
            return *this;
        }

        ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi>& operator/=(const float scalar)
        {
            yard -= scalar;
            return *this;
        }

    };

    template<typename X, typename Y, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi> operator*(const ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi>& relation, const float scalar)
    {
        ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi> operator*(const float scalar, const ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi>& relation)
    {
        ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi> operator/(const ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi>& relation, const float scalar)
    {
        ExponentiatedPolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result /= scalar;
        return result;
    }







    template<typename Y>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Y,0,1> get_left_unbounded_exponential_interpolated_temperature_function(
        const si::temperature<double> Tunits, const Y yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            logys.push_back(std::log(ys[i]));
        }
        auto yard = analytic::spline::linear_spline<float>(xs, logys);
        yard.cars[0].lo = 0.0f;
        return relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Y,0,1>(yard, Tunits, yunits);
    }

    // 175 uses
    template<typename Y>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Y,0,1> get_left_unbounded_exponential_interpolated_temperature_function(
        const si::celsius_type<double> Tunits, const Y yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        std::vector<double> kelvins;
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            kelvins.push_back(xs[i]*Tunits/si::kelvin);
            logys.push_back(std::log(ys[i]));
        }
        auto yard = analytic::spline::linear_spline<float>(kelvins, logys);
        yard.cars[0].lo = 0.0f;
        return relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Y,0,1>(yard, si::kelvin, yunits);
    }

    // 175 uses
    template<typename Y>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Y,0,1> get_exponential_interpolated_temperature_function(
        const si::temperature<double> Tunits, const Y yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            logys.push_back(std::log(ys[i]));
        }
        return relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Y,0,1>(
            analytic::spline::linear_spline<float>(xs, logys), Tunits, yunits);
    }

    // 175 uses
    template<typename Y>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Y,0,1> get_exponential_interpolated_temperature_function(
        const si::celsius_type<double> Tunits, const Y yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        std::vector<double> kelvins;
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            kelvins.push_back(xs[i]*Tunits/si::kelvin);
            logys.push_back(std::log(ys[i]));
        }
        return relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Y,0,1>(
            analytic::spline::linear_spline<float>(kelvins, logys), si::kelvin, yunits);
    }


    // 175 uses
    template<typename Y>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Y,-1,0> get_exponential_interpolated_inverse_temperature_function(
        const si::temperature<double> Tunits, const Y yunits,
        const std::vector<double>xs,
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            logys.push_back(std::log(ys[i]));
        }
        return relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Y,-1,0>(
            analytic::spline::inverse_linear_spline<float>(xs, logys), Tunits, yunits);
    }

}}

