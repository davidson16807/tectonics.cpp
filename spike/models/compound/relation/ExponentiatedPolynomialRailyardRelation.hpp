#pragma once

// in-house libraries
#include <math/expression/Scaling.hpp>
#include <math/expression/PolynomialRailyard.hpp>
#include <math/expression/PolynomialRailyard_library.hpp>
#include <math/expression/Exponentiated.hpp>
#include <units/si.hpp>

namespace compound {
namespace relation {


    /*
    `ExponentiatedPolynomialRailyardRelation` represents an arbitrary function that uses a railyard to map quantities from one unit to another
    See `math/expression/Railyard.hpp` for the definition and motivation of a "railyard".
    */
    template<typename Tx, typename Ty, int Plo, int Phi>
    struct ExponentiatedPolynomialRailyardRelation
    {
        math::PolynomialRailyard<float, Plo, Phi> yard;
        Tx xunits;
        Ty yunits;
        
        using value_type = Ty;

        // zero constructor
        constexpr ExponentiatedPolynomialRailyardRelation():
            yard(),
            xunits(1.0),
            yunits(1.0)
        {
        }

        template<typename T>
        constexpr ExponentiatedPolynomialRailyardRelation(
            const math::PolynomialRailyard<T, Plo, Phi> yard,
            const Tx xunits,
            const Ty yunits
        ):
            yard(yard),
            xunits(xunits),
            yunits(yunits)
        {
        }

        template<typename T, int Qlo, int Qhi>
        explicit constexpr ExponentiatedPolynomialRailyardRelation(
            const math::PolynomialRailyard<T, Qlo, Qhi> yard,
            const Tx xunits,
            const Ty yunits
        ):
            yard(yard),
            xunits(xunits),
            yunits(yunits)
        {
        }

        // copy constructor
        template<int Qlo, int Qhi>
        constexpr ExponentiatedPolynomialRailyardRelation(
            const ExponentiatedPolynomialRailyardRelation<Tx, Ty, Qlo, Qhi>& other
        ):
            yard(other.yard),
            xunits(other.xunits),
            yunits(other.yunits)
        {
        }

        constexpr ExponentiatedPolynomialRailyardRelation(const Ty& other):
            yard(other/Ty(1.0)),
            xunits(1.0),
            yunits(1.0)
        {
        }

        constexpr ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator=(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
        {
            yard = other.yard;
            xunits = other.xunits;
            yunits = other.yunits;
            return *this;
        }

        constexpr ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator=(const Ty& other)
        {
            yard = other/Ty(1.0);
            xunits = Tx(1.0);
            yunits = Ty(1.0);
            return *this;
        }

        Ty operator()(const Tx x) const
        {
            return std::exp(yard(float(x/xunits))) * yunits;
        }

        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> restriction(
            const Tx xlo, const Tx xhi,
            const float known_max_fractional_error
        ) const
        {
            return ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>(restriction(yard, xlo/xunits, xhi/xunits), xunits, yunits);
        }

        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator*=(const float scalar)
        {
            yard += scalar;
            return *this;
        }

        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator/=(const float scalar)
        {
            yard -= scalar;
            return *this;
        }

    };

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator*(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const float scalar)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator/(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const float scalar)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result /= scalar;
        return result;
    }








    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1> get_left_unbounded_exponential_interpolated_temperature_function(
        const si::temperature<double> Tunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            logys.push_back(std::log(ys[i]));
        }
        auto yard = math::spline::linear_spline<float>(xs, logys);
        yard.cars[0].lo = 0.0f;
        return relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1>(yard, Tunits, yunits);
    }

    // 175 uses
    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1> get_left_unbounded_exponential_interpolated_temperature_function(
        const si::celcius_type<double> Tunits, const Ty yunits,
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
        auto yard = math::spline::linear_spline<float>(kelvins, logys);
        yard.cars[0].lo = 0.0f;
        return relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1>(yard, si::kelvin, yunits);
    }

    // 175 uses
    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1> get_exponential_interpolated_temperature_function(
        const si::temperature<double> Tunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            logys.push_back(std::log(ys[i]));
        }
        return relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1>(
            math::spline::linear_spline<float>(xs, logys), Tunits, yunits);
    }

    // 175 uses
    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1> get_exponential_interpolated_temperature_function(
        const si::celcius_type<double> Tunits, const Ty yunits,
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
        return relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1>(
            math::spline::linear_spline<float>(kelvins, logys), si::kelvin, yunits);
    }


    // 175 uses
    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,-1,0> get_exponential_interpolated_inverse_temperature_function(
        const si::temperature<double> Tunits, const Ty yunits,
        const std::vector<double>xs,
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            logys.push_back(std::log(ys[i]));
        }
        return relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,-1,0>(
            math::spline::inverse_linear_spline<float>(xs, logys), Tunits, yunits);
    }

}}

