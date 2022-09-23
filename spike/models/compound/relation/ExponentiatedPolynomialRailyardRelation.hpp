#pragma once

// in-house libraries
#include <math/expression/Scaling.hpp>
#include <math/expression/PolynomialRailyard.hpp>
#include <math/expression/PolynomialRailyard_library.hpp>
#include <math/expression/Exponentiated.hpp>
#include <units/si.hpp>

namespace compound {
namespace relation {

    template<typename Tx, typename Ty, int Plo, int Phi>
    using ExponentiatedPolynomialRailyardRelation = Relation<Tx,Ty, math::Exponentiated<float, math::PolynomialRailyard<float,Plo,Phi>>>;

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

