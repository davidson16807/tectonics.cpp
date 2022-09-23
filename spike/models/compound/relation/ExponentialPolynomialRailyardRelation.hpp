#pragma once

// in-house libraries
#include <math/expression/Scaling.hpp>
#include <math/expression/PolynomialRailyard.hpp>
#include <math/expression/PolynomialRailyard_library.hpp>
#include <units/si.hpp>

namespace compound {
namespace relation {

    template<typename Tx, typename Ty, int Plo, int Phi>
    using PolynomialRailyardRelation = Relation<Tx,Ty,math::PolynomialRailyard<float,Plo,Phi>>;

    /*
    `ExponentiatedPolynomialRailyardRelation` represents an arbitrary function that uses a railyard to map quantities from one unit to another
    See `math/expression/Railyard.hpp` for the definition and motivation of a "railyard".
    */
    template<typename Tx, typename Ty, int Plo, int Phi>
    struct ExponentiatedPolynomialRailyardRelation
    {
        math::Exponentiated<PolynomialRailyard<float, Plo, Phi>> yard;
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

        // constant constructor
        constexpr ExponentiatedPolynomialRailyardRelation(const Ty& other):
            yard(other/Ty(1.0)),
            xunits(1.0),
            yunits(1.0)
        {
        }

        template<typename T>
        constexpr ExponentiatedPolynomialRailyardRelation(
            const math::ExponentiatedPolynomialRailyard<T, Plo, Phi> yard,
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
            const math::ExponentiatedPolynomialRailyard<T, Qlo, Qhi> yard,
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
            return yard(float(x/xunits)) * yunits;
        }

        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> restriction(
            const Tx xlo, const Tx xhi,
            const float known_max_fractional_error
        ) const
        {
            return ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>(restriction(yard, xlo/xunits, xhi/xunits), xunits, yunits);
        }

        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator+=(const Ty offset)
        {
            yard += offset/yunits;
            return *this;
        }

        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator-=(const Ty offset)
        {
            yard -= offset/yunits;
            return *this;
        }

        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator*=(const float scalar)
        {
            yard *= scalar;
            return *this;
        }

        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator/=(const float scalar)
        {
            yard *= scalar;
            return *this;
        }

        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator+=(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            yard += yscale * compose(other.yard, math::Scaling(Tscale));
            return *this;
        }

        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator-=(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            yard += yscale * compose(other.yard, math::Scaling(Tscale));
            return *this;
        }

    };

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator+(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result += other;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator-(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result -= other;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator+(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const Ty offset)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator-(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const Ty offset)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result -= offset;
        return result;
    }

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

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator+(const Ty offset, const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator-(const Ty offset, const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator-(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= -1.0f;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator*(const float scalar, const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation)
    {
        ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }


    // // functions that cast input to the yard, or output from the yard
    // template<typename Tx, typename Ty, int Plo, int Phi>
    // ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator*(const ExponentiatedPolynomialRailyardRelation<Tx,float,Plo,Phi>& relation, const Ty yunits2)
    // {
    //     return ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>(relation.yard, relation.xunits, yunits2);
    // }
    // template<typename Tx, typename Ty, int Plo, int Phi>
    // ExponentiatedPolynomialRailyardRelation<Tx,float,Plo,Phi> operator/(const ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const Ty yunits2)
    // {
    //     return ExponentiatedPolynomialRailyardRelation<Tx,float,Plo,Phi>(relation.yard, relation.xunits, relation.yunits/yunits2);
    // }
    // template<typename Tx, int Plo, int Phi>
    // math::ExponentiatedPolynomialRailyard<float,Plo,Phi> compose(const ExponentiatedPolynomialRailyardRelation<Tx,float,Plo,Phi>& relation, const math::Cast<float,Tx>& cast)
    // {
    //     return relation.yard * (cast.yunits/Tx(1.0f))/cast.xunits;
    // }
    // template<typename Tx, int Plo, int Phi>
    // ExponentiatedPolynomialRailyardRelation<Tx,float,Plo,Phi> compose(const math::ExponentiatedPolynomialRailyard<float,Plo,Phi>& yard, const math::Cast<Tx,float>& cast)
    // {
    //     return ExponentiatedPolynomialRailyardRelation<Tx,float,Plo,Phi>(yard, 1.0f/cast.xunits, 1.0f);
    // }


    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    float distance(
        const relation::ExponentiatedPolynomialRailyardRelation<Tx,Ty,Plo,Phi>& a, 
        const relation::ExponentiatedPolynomialRailyardRelation<Tx,Ty,Qlo,Qhi>& b, 
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
    ExponentiatedPolynomialRailyardRelation<si::temperature<T>,Ty,0,1> get_linear_interpolation_function(
        const si::celcius_type<T> celcius, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return ExponentiatedPolynomialRailyardRelation<si::temperature<T>,Ty,0,1>(
                math::compose(
                    math::spline::linear_spline<double>(xs, ys), 
                    math::Shifting(-si::standard_temperature/si::kelvin)), 
                si::kelvin, yunits);
    }

    // TODO: rename `spectral_linear_yard`
    template<typename T, typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::temperature<T>,Ty,0,1> get_linear_interpolation_function(
        const si::temperature<T> xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return ExponentiatedPolynomialRailyardRelation<si::temperature<T>,Ty,0,1>(math::spline::linear_spline<double>(xs, ys), xunits, yunits);
    }

    // TODO: rename `spectral_linear_yard`
    template<typename T, typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::pressure<T>,Ty,0,1> get_linear_interpolation_function(
        const si::pressure<T> xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return ExponentiatedPolynomialRailyardRelation<si::pressure<T>,Ty,0,1>(math::spline::linear_spline<double>(xs, ys), xunits, yunits);
    }
    
    /*
    `get_dippr_quartic_temperature_function_100()` is equivalent to dippr function 100,
    for liquid thermal conductivity, heat capacity, and solid density
    */
    // 26 uses, for liquid thermal conductivity and heat capacity
    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>, Ty, 0,4> get_dippr_quartic_temperature_relation_100( 
        const si::temperature<double> Tunits, const Ty yunits,
        const double intercept, const double slope, const double square, const double cube, const double fourth,
        const double Tmin, double Tmax
    ){
        const double oo = std::numeric_limits<double>::max();
        using P = math::Polynomial<double,0,4>;
        using R = math::Railcar<double,P>;
        math::Polynomial<double,0,4> p = math::Polynomial<double,0,4>({intercept, slope, square, cube, fourth});
        return ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,4>(
            math::ExponentiatedPolynomialRailyard<double,0,4>({
                R(-oo, Tmin, P(p(Tmin))), 
                R(Tmin,Tmax, p),
                R(Tmax, oo,  P(p(Tmax)))
            }), Tunits, yunits);
    }

    /*
    `get_perry_johnson_temperature_function()` uses Perry coefficients for high temperature,
    and interpolated values from Johnson (1960) for low temperature.
    */
    // 23 uses, all for heat capacity of solids
    ExponentiatedPolynomialRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>,  -2,3> get_perry_johnson_temperature_function( 
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
            return ExponentiatedPolynomialRailyardRelation<si::temperature<double>,si::specific_heat_capacity<double>,-2,3>(
                math::ExponentiatedPolynomialRailyard<double,-2,3>({
                    R(0.0, T_max_johnson,         P(johnson)),            // johnson polynomial
                    R(T_max_johnson, T_min_perry, P(transition)),         // linear perry/johnson interpolation
                    R(T_min_perry, T_max_perry,   P(perry)),              // perry polynomial
                    R(T_max_perry, oo,            P(perry(T_max_perry))), // perry polynomial
                }), Tunits, y_units_johnson);
    }
    
    // TODO: rename `spectral_linear_yard`
    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength(
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
        return ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1>(math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double> ns, 
        const std::vector<double> ys
    ){
        assert(ns.size() == ys.size());
        return ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1>(math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double>      ns, 
        const std::vector<double> log10ys
    ){
        assert(ns.size() == log10ys.size());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,3> get_spectral_cubic_interpolation_function_of_wavenumber_for_log10_sample_output(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double>      ns, 
        const std::vector<double> log10ys
    ){
        assert(ns.size() == log10ys.size());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,3> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_output(
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
        return ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }


    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_input(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double> log10ns, 
        const std::vector<double>      ys
    ){
        assert(log10ns.size() == ys.size());
        std::vector<double> ns;
        for (std::size_t i=0; i<log10ns.size(); i++){
            ns.push_back(pow(10.0, log10ns[i]));
        }
        return ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_input(
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
        return ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,Ty,0,1> (math::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }




    ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,si::attenuation<double>,0,3> get_absorption_coefficient_function_from_reflectance_at_wavelengths(
        const si::length<double> lunits,
        const si::length<double> particle_diameter, 
        const std::vector<double>wavelengths, 
        const std::vector<double>reflectances
    ){
        std::vector<double> wavenumbers;
        std::vector<double> wavenumber_reflectances = reflectances;
        for (std::size_t i=0; i<reflectances.size(); i++){
            wavenumbers.push_back(1.0/wavelengths[i]);
        }
        std::reverse(wavenumbers.begin(), wavenumbers.end());
        std::reverse(wavenumber_reflectances.begin(), wavenumber_reflectances.end());

        /*
        NOTE: we assume extinction efficiency is close to 1, in which case the 
          single scattering albedo equals scattering efficiency
        In this case:
          absorption_coefficient = approx_single_scatter_albedo_from_reflectance ∘ approx_single_scatter_albedo_from_reflectance
        This is equivalent to:
          absorption_coefficient = max((reflectance + 1.0)*(reflectance + 1.0) / (4.0 * reflectance) - 1.0, 0.0) / De;
        where:
          De = 2*particle_diameter;
        We leverage the fact that 0 ≤ reflectance ≤ 1 to create a segmented 
          linear approximation of 1/reflectance that is suited for that range.
          It has an maximum error of 12% over the targeted range.
        We use a linear approximation since the result will be composed 
          with another segmented linear approximation for reflectance,
          and we don't want the degree of the resulting polynomial 
          to be so high that it produces nans upon evaluation.
        */

        const float oo = std::numeric_limits<float>::max();

        const math::ExponentiatedPolynomialRailyard<float,0,1> inverse{
            math::PolynomialRailcar<float,0,1>(0.0357, 0.0778, math::linear_newton_polynomial<float>(0.0357, 0.0778,  28.041, 11.458)),
            math::PolynomialRailcar<float,0,1>(0.0778, 0.1938, math::linear_newton_polynomial<float>(0.0778, 0.1938,  11.458,  4.937)),
            math::PolynomialRailcar<float,0,1>(0.1938, 0.4649, math::linear_newton_polynomial<float>(0.1938, 0.4649,   4.937,  1.881)),
            math::PolynomialRailcar<float,0,1>(0.4649, 1.0000, math::linear_newton_polynomial<float>(0.4649, 1.0000,   1.881,  1.000)),
            math::PolynomialRailcar<float,0,1>(1.0000,     oo, math::linear_newton_polynomial<float>(1.0000, 2.0000,   1.000,  1.000))
        };

        return ExponentiatedPolynomialRailyardRelation<si::wavenumber<double>,si::attenuation<double>,0,3>(
            compose(
                (0.25f * math::Shifting(1.0f) * math::Shifting(1.0f) * inverse - 1.0f)/(2.0f*float(particle_diameter/si::length<double>(1.0))),
                math::spline::linear_spline<float>(wavenumbers, wavenumber_reflectances)
            ),
            si::wavenumber<double>(1.0),
            si::attenuation<double>(1.0)
        );
    }

}}

