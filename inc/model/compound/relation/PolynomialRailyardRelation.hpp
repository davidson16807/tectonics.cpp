#pragma once

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/PolynomialRailyard_library.hpp>
#include <units/si.hpp>

namespace compound {
namespace relation {

    /*
    `PolynomialRailyardRelation` represents an arbitrary function that uses a railyard to map quantities from one unit to another
    See `math/expression/Railyard.hpp` for the definition and motivation of a "railyard".
    */
    template<typename Tx, typename Ty, int Plo, int Phi>
    struct PolynomialRailyardRelation
    {
        math::PolynomialRailyard<float, Plo, Phi> yard;
        Tx xunits;
        Ty yunits;
        
        using value_type = Ty;

        // zero constructor
        constexpr PolynomialRailyardRelation():
            yard(),
            xunits(1.0),
            yunits(1.0)
        {
        }

        // constant constructor
        constexpr PolynomialRailyardRelation(const Ty& other):
            yard(other/Ty(1.0)),
            xunits(1.0),
            yunits(1.0)
        {
        }

        // copy constructor
        template<int Qlo, int Qhi>
        constexpr PolynomialRailyardRelation(
            const PolynomialRailyardRelation<Tx, Ty, Qlo, Qhi>& other
        ):
            yard(other.yard),
            xunits(other.xunits),
            yunits(other.yunits)
        {
        }

        template<typename T>
        constexpr PolynomialRailyardRelation(
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
        explicit constexpr PolynomialRailyardRelation(
            const math::PolynomialRailyard<T, Qlo, Qhi> yard,
            const Tx xunits,
            const Ty yunits
        ):
            yard(yard),
            xunits(xunits),
            yunits(yunits)
        {
        }

        constexpr PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator=(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
        {
            yard = other.yard;
            xunits = other.xunits;
            yunits = other.yunits;
            return *this;
        }

        constexpr PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator=(const Ty& other)
        {
            yard = other/Ty(1.0);
            xunits = Tx(1.0);
            yunits = Ty(1.0);
            return *this;
        }

        Ty operator()(const Tx x) const
        {
            return yard(float(x/xunits)) * yunits;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> restriction(
            const Tx xlo, const Tx xhi,
            const float known_max_fractional_error
        ) const
        {
            return PolynomialRailyardRelation<Tx,Ty,Plo,Phi>(restriction(yard, xlo/xunits, xhi/xunits), xunits, yunits);
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator+=(const Ty offset)
        {
            yard += offset/yunits;
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator-=(const Ty offset)
        {
            yard -= offset/yunits;
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator*=(const float scalar)
        {
            yard *= scalar;
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator/=(const float scalar)
        {
            yard *= scalar;
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator+=(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            yard += yscale * compose(other.yard, math::Scaling(Tscale));
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator-=(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            yard += yscale * compose(other.yard, math::Scaling(Tscale));
            return *this;
        }

    };

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator+(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result += other;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator-(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result -= other;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator+(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const Ty offset)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator-(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const Ty offset)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result -= offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator*(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const float scalar)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator/(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const float scalar)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result /= scalar;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator+(const Ty offset, const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator-(const Ty offset, const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator-(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= -1.0f;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator*(const float scalar, const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation)
    {
        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }


    // // functions that cast input to the yard, or output from the yard
    // template<typename Tx, typename Ty, int Plo, int Phi>
    // PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator*(const PolynomialRailyardRelation<Tx,float,Plo,Phi>& relation, const Ty yunits2)
    // {
    //     return PolynomialRailyardRelation<Tx,Ty,Plo,Phi>(relation.yard, relation.xunits, yunits2);
    // }
    // template<typename Tx, typename Ty, int Plo, int Phi>
    // PolynomialRailyardRelation<Tx,float,Plo,Phi> operator/(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const Ty yunits2)
    // {
    //     return PolynomialRailyardRelation<Tx,float,Plo,Phi>(relation.yard, relation.xunits, relation.yunits/yunits2);
    // }
    // template<typename Tx, int Plo, int Phi>
    // math::PolynomialRailyard<float,Plo,Phi> compose(const PolynomialRailyardRelation<Tx,float,Plo,Phi>& relation, const math::Cast<float,Tx>& cast)
    // {
    //     return relation.yard * (cast.yunits/Tx(1.0f))/cast.xunits;
    // }
    // template<typename Tx, int Plo, int Phi>
    // PolynomialRailyardRelation<Tx,float,Plo,Phi> compose(const math::PolynomialRailyard<float,Plo,Phi>& yard, const math::Cast<Tx,float>& cast)
    // {
    //     return PolynomialRailyardRelation<Tx,float,Plo,Phi>(yard, 1.0f/cast.xunits, 1.0f);
    // }


    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    float distance(
        const relation::PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& a, 
        const relation::PolynomialRailyardRelation<Tx,Ty,Qlo,Qhi>& b, 
        const Tx xlo, 
        const Tx xhi
    ){
        return math::distance(
            compose(a.yard*float(Ty(1.0)/a.yunits), math::Scaling<float>(float(Tx(1.0)/a.xunits))),
            compose(b.yard*float(Ty(1.0)/b.yunits), math::Scaling<float>(float(Tx(1.0)/b.xunits))),
            float(xlo/Tx(1.0f)), 
            float(xhi/Tx(1.0f))
        );
    }

    // TODO: rename `spectral_linear_yard`
    template<typename T, typename Ty>
    PolynomialRailyardRelation<si::temperature<T>,Ty,0,1> get_linear_interpolation_function(
        const si::celcius_type<T> celcius, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return PolynomialRailyardRelation<si::temperature<T>,Ty,0,1>(
                math::compose(
                    math::spline::linear_spline<double>(xs, ys), 
                    math::Shifting(-si::standard_temperature/si::kelvin)), 
                si::kelvin, yunits);
    }

    // TODO: rename `spectral_linear_yard`
    template<typename T, typename Ty>
    PolynomialRailyardRelation<si::temperature<T>,Ty,0,1> get_linear_interpolation_function(
        const si::temperature<T> xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return PolynomialRailyardRelation<si::temperature<T>,Ty,0,1>(math::spline::linear_spline<double>(xs, ys), xunits, yunits);
    }

    // TODO: rename `spectral_linear_yard`
    template<typename T, typename Ty>
    PolynomialRailyardRelation<si::pressure<T>,Ty,0,1> get_linear_interpolation_function(
        const si::pressure<T> xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return PolynomialRailyardRelation<si::pressure<T>,Ty,0,1>(math::spline::linear_spline<double>(xs, ys), xunits, yunits);
    }
    
    /*
    `get_dippr_quartic_temperature_function_100()` is equivalent to dippr function 100,
    for liquid thermal conductivity, heat capacity, and solid density
    */
    // 26 uses, for liquid thermal conductivity and heat capacity
    template<typename Ty>
    PolynomialRailyardRelation<si::temperature<double>, Ty, 0,4> get_dippr_quartic_temperature_relation_100( 
        const si::temperature<double> Tunits, const Ty yunits,
        const double intercept, const double slope, const double square, const double cube, const double fourth,
        const double Tmin, double Tmax
    ){
        using P = math::Polynomial<double,0,4>;
        using C = math::Clamped<double,P>;
        using R = math::Railyard<double,P>;
        math::Polynomial<double,0,4> p = math::Polynomial<double,0,4>({intercept, slope, square, cube, fourth});
        return PolynomialRailyardRelation<si::temperature<double>,Ty,0,4>(R(C(Tmin,Tmax, p)), Tunits, yunits);
    }

    // Sheffy Johnson: https://chemicals.readthedocs.io/chemicals.thermal_conductivity.html#pure-low-pressure-liquid-correlations
    PolynomialRailyardRelation<si::temperature<double>, si::thermal_conductivity<double>,0,1> estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(
        const si::molar_mass<double> molar_mass,
        const si::temperature<double> normal_melting_point,
        const si::temperature<double> critical_point_temperature
    ){
        double M = (molar_mass / (si::gram / si::mole));
        double Tm = (normal_melting_point / si::kelvin);
        double Tc = (critical_point_temperature / si::kelvin);
        using P = math::Polynomial<double,0,1>;
        using C = math::Clamped<double,P>;
        using R = math::Railyard<double,P>;
        auto T = math::Identity<double>();
        P kl = 1.951 * (1.0 - 0.00126 * (T - Tm)) / (std::pow(Tm, 0.216)*std::pow(M, 0.3));
        // Tmax is set either to give the lowest thermal conductivity ever observed, or Tc, whichever comes first
        double Tmax = std::min(Tc, math::solution(kl,0.011)); 
        return PolynomialRailyardRelation<si::temperature<double>, si::thermal_conductivity<double>,0,1>(
            R(C(0.0, Tmax, kl)),
            si::kelvin, si::watt / (si::meter*si::kelvin));
    }

    /*
    `get_perry_johnson_temperature_function()` uses Perry coefficients for high temperature,
    and interpolated values from Johnson (1960) for low temperature.
    */
    // 23 uses, all for heat capacity of solids
    PolynomialRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>,  -2,3> get_perry_johnson_temperature_function( 
        const si::temperature<double> Tunits, 
        const si::specific_heat_capacity<double> y_units_johnson, 
        const double linear_johnson, const double cube_johnson, 
        const double T_max_johnson,
        const si::specific_heat_capacity<double> y_units_perry,
        const double intercept_perry, const double linear_perry, const double inverse_square_perry, const double square_perry, 
        const double T_min_perry, const double T_max_perry
    ){
            const double oo = std::numeric_limits<double>::max();
            using P = math::Polynomial<double,-2,3>;
            using R = math::Railcar<double,P>;
            math::Polynomial<double,1,3>  johnson    = math::Polynomial<double,1,3> ({linear_johnson, 0.0, cube_johnson});
            math::Polynomial<double,-2,2> perry      = y_units_perry/y_units_johnson * math::Polynomial<double,-2,2>({inverse_square_perry, 0.0, intercept_perry, linear_perry, square_perry});
            math::Polynomial<double,0,1>  transition = math::linear_newton_polynomial(T_max_johnson, T_min_perry, johnson(T_max_johnson), perry(T_min_perry));
            return PolynomialRailyardRelation<si::temperature<double>,si::specific_heat_capacity<double>,-2,3>(
                math::PolynomialRailyard<double,-2,3>({
                    R(0.0, T_max_johnson,         P(johnson)),            // johnson polynomial
                    R(T_max_johnson, T_min_perry, P(transition)),         // linear perry/johnson interpolation
                    R(T_min_perry, T_max_perry,   P(perry)),              // perry polynomial
                    R(T_max_perry, oo,            P(perry(T_max_perry))), // upper bound of perry
                }), Tunits, y_units_johnson);
    }
    
    template<typename Ty>
    PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength(
        const si::length<double> lunits, const Ty yunits,
        const std::vector<double> ls, 
        const std::vector<double> lys
    ){
        assert(ls.size() == lys.size());
        const si::wavenumber<double> nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> ys;
        for (std::size_t i=0; i<lys.size(); i++){
            ns.push_back(1.0/(ls[i]));
            ys.push_back(lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(ys.begin(), ys.end());
        return PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1>(math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double> ns, 
        const std::vector<double> ys
    ){
        assert(ns.size() == ys.size());
        return PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1>(math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double>      ns, 
        const std::vector<double> log10ys
    ){
        assert(ns.size() == log10ys.size());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,3> get_spectral_cubic_interpolation_function_of_wavenumber_for_log10_sample_output(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double>      ns, 
        const std::vector<double> log10ys
    ){
        assert(ns.size() == log10ys.size());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,3> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_output(
        const si::length<double> lunits, const Ty yunits,
        const std::vector<double>      ls, 
        const std::vector<double>log10lys
    ){
        assert(ls.size() == log10lys.size());
        const si::wavenumber<double> nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> log10ys;
        for (std::size_t i=0; i<log10lys.size(); i++){
            ns.push_back(1.0/(ls[i]));
            log10ys.push_back(log10lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(log10ys.begin(), log10ys.end());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }


    template<typename Ty>
    PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_input(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double> log10ns, 
        const std::vector<double>      ys
    ){
        assert(log10ns.size() == ys.size());
        std::vector<double> ns;
        for (std::size_t i=0; i<log10ns.size(); i++){
            ns.push_back(pow(10.0, log10ns[i]));
        }
        return PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_input(
        const si::length<double> lunits, const Ty yunits,
        const std::vector<double> log10ls, 
        const std::vector<double>      lys
    ){
        assert(log10ls.size() == lys.size());
        const si::wavenumber<double> nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> ys;
        for (std::size_t i=0; i<lys.size(); i++){
            ns.push_back(1.0/(pow(10.0, log10ls[i])));
            ys.push_back(lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(ys.begin(), ys.end());
        return PolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }



}}

