#pragma once

// in-house libraries
#include <math/expression/Scaling.hpp>
#include <math/expression/PolynomialRailyard.hpp>
#include <math/expression/PolynomialRailyard_library.hpp>
#include <units/si.hpp>

namespace compound {
namespace relation {

    /*
    `PolynomialRailyardRelation` represents an arbitrary function that uses a spline to map quantities from one unit to another
    */
    template<typename Tx, typename Ty, int Plo, int Phi>
    struct PolynomialRailyardRelation
    {
        math::PolynomialRailyard<float, Plo, Phi> spline;
        Tx xunits;
        Ty yunits;
        
        using value_type = Ty;

        // zero constructor
        constexpr PolynomialRailyardRelation():
            spline(),
            xunits(1.0),
            yunits(1.0)
        {
        }

        template<typename T>
        constexpr PolynomialRailyardRelation(
            const math::PolynomialRailyard<T, Plo, Phi> spline,
            const Tx xunits,
            const Ty yunits
        ):
            spline(spline),
            xunits(xunits),
            yunits(yunits)
        {
        }

        template<typename T, int Qlo, int Qhi>
        explicit constexpr PolynomialRailyardRelation(
            const math::PolynomialRailyard<T, Qlo, Qhi> spline,
            const Tx xunits,
            const Ty yunits
        ):
            spline(spline),
            xunits(xunits),
            yunits(yunits)
        {
        }

        // copy constructor
        template<int Qlo, int Qhi>
        constexpr PolynomialRailyardRelation(
            const PolynomialRailyardRelation<Tx, Ty, Qlo, Qhi>& other
        ):
            spline(other.spline),
            xunits(other.xunits),
            yunits(other.yunits)
        {
        }

        constexpr PolynomialRailyardRelation(const Ty& other):
            spline(other/Ty(1.0)),
            xunits(1.0),
            yunits(1.0)
        {
        }

        constexpr PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator=(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
        {
            spline = other.spline;
            xunits = other.xunits;
            yunits = other.yunits;
            return *this;
        }

        constexpr PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator=(const Ty& other)
        {
            spline = other/Ty(1.0);
            xunits = Tx(1.0);
            yunits = Ty(1.0);
            return *this;
        }

        Ty operator()(const Tx x) const
        {
            return spline(float(x/xunits)) * yunits;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> restriction(
            const Tx xlo, const Tx xhi,
            const float known_max_fractional_error
        ) const
        {
            return PolynomialRailyardRelation<Tx,Ty,Plo,Phi>(restriction(spline, xlo/xunits, xhi/xunits), xunits, yunits);
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator+=(const Ty offset)
        {
            spline += offset/yunits;
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator-=(const Ty offset)
        {
            spline -= offset/yunits;
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator*=(const float scalar)
        {
            spline *= scalar;
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator/=(const float scalar)
        {
            spline *= scalar;
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator+=(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            spline += yscale * compose(other.spline, math::Scaling(Tscale));
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator-=(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            spline += yscale * compose(other.spline, math::Scaling(Tscale));
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


    // // functions that cast input to the spline, or output from the spline
    // template<typename Tx, typename Ty, int Plo, int Phi>
    // PolynomialRailyardRelation<Tx,Ty,Plo,Phi> operator*(const PolynomialRailyardRelation<Tx,float,Plo,Phi>& relation, const Ty yunits2)
    // {
    //     return PolynomialRailyardRelation<Tx,Ty,Plo,Phi>(relation.spline, relation.xunits, yunits2);
    // }
    // template<typename Tx, typename Ty, int Plo, int Phi>
    // PolynomialRailyardRelation<Tx,float,Plo,Phi> operator/(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& relation, const Ty yunits2)
    // {
    //     return PolynomialRailyardRelation<Tx,float,Plo,Phi>(relation.spline, relation.xunits, relation.yunits/yunits2);
    // }
    // template<typename Tx, int Plo, int Phi>
    // math::PolynomialRailyard<float,Plo,Phi> compose(const PolynomialRailyardRelation<Tx,float,Plo,Phi>& relation, const math::Cast<float,Tx>& cast)
    // {
    //     return relation.spline * (cast.yunits/Tx(1.0f))/cast.xunits;
    // }
    // template<typename Tx, int Plo, int Phi>
    // PolynomialRailyardRelation<Tx,float,Plo,Phi> compose(const math::PolynomialRailyard<float,Plo,Phi>& spline, const math::Cast<Tx,float>& cast)
    // {
    //     return PolynomialRailyardRelation<Tx,float,Plo,Phi>(spline, 1.0f/cast.xunits, 1.0f);
    // }


    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    float distance(
        const relation::PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& a, 
        const relation::PolynomialRailyardRelation<Tx,Ty,Qlo,Qhi>& b, 
        const Tx xlo, 
        const Tx xhi
    ){
        return math::distance(
            compose(a.spline*float(Ty(1.0)/a.yunits), math::Scaling<float>(float(Tx(1.0)/a.xunits))),
            compose(b.spline*float(Ty(1.0)/b.yunits), math::Scaling<float>(float(Tx(1.0)/b.xunits))),
            float(xlo/Tx(1.0f)), 
            float(xhi/Tx(1.0f))
        );
    }

    // TODO: rename `spectral_linear_spline`
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




    PolynomialRailyardRelation<si::wavenumber<double>,si::attenuation<double>,0,3> get_absorption_coefficient_function_from_reflectance_at_wavelengths(
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

        const math::PolynomialRailyard<float,0,1> inverse{
            math::PolynomialRailcar<float,0,1>(0.0357, 0.0778, math::linear_newton_polynomial<float>(0.0357, 0.0778,  28.041, 11.458)),
            math::PolynomialRailcar<float,0,1>(0.0778, 0.1938, math::linear_newton_polynomial<float>(0.0778, 0.1938,  11.458,  4.937)),
            math::PolynomialRailcar<float,0,1>(0.1938, 0.4649, math::linear_newton_polynomial<float>(0.1938, 0.4649,   4.937,  1.881)),
            math::PolynomialRailcar<float,0,1>(0.4649, 1.0000, math::linear_newton_polynomial<float>(0.4649, 1.0000,   1.881,  1.000)),
            math::PolynomialRailcar<float,0,1>(1.0000,     oo, math::linear_newton_polynomial<float>(1.0000, 2.0000,   1.000,  1.000))
        };

        return PolynomialRailyardRelation<si::wavenumber<double>,si::attenuation<double>,0,3>(
            compose(
                (0.25f * math::Shifting(1.0f) * math::Shifting(1.0f) * inverse - 1.0f)/(2.0f*float(particle_diameter/si::length<double>(1.0))),
                math::spline::linear_spline<float>(wavenumbers, wavenumber_reflectances)
            ),
            si::wavenumber<double>(1.0),
            si::attenuation<double>(1.0)
        );
    }

}}

