#pragma once

// in-house libraries
#include <math/expression/Scaling.hpp>
#include <math/expression/Spline.hpp>
#include <math/expression/Spline_library.hpp>

namespace compound {
namespace relation {



    /*
    `SplineRelation` represents an arbitrary function that uses a spline to map quantities from one unit to another
    */
    template<typename Tx, typename Ty, int Plo, int Phi>
    struct SplineRelation
    {
        math::Spline<float, Plo, Phi> spline;
        Tx xunits;
        Ty yunits;
        
        using value_type = Ty;

        constexpr SplineRelation(
            const math::Spline<float, Plo, Phi> spline,
            const Tx xunits,
            const Ty yunits
        ):
            spline(spline),
            xunits(xunits),
            yunits(yunits)
        {
        }

        constexpr SplineRelation<Tx,Ty,Plo,Phi>& operator=(const SplineRelation<Tx,Ty,Plo,Phi>& other)
        {
            spline = other.spline;
            xunits = other.xunits;
            yunits = other.yunits;
            return *this;
        }

        Ty operator()(const Tx x) const
        {
            return spline(float(x/xunits)) * yunits;
        }

        SplineRelation<Tx,Ty,Plo,Phi> restriction(
            const Tx xlo, const Tx xhi,
            const float known_max_fractional_error
        ) const
        {
            return SplineRelation<Tx,Ty,Plo,Phi>(restriction(spline, xlo/xunits, xhi/xunits), xunits, yunits);
        }

        SplineRelation<Tx,Ty,Plo,Phi>& operator+=(const Ty offset)
        {
            spline += offset/yunits;
            return *this;
        }

        SplineRelation<Tx,Ty,Plo,Phi>& operator-=(const Ty offset)
        {
            spline -= offset/yunits;
            return *this;
        }

        SplineRelation<Tx,Ty,Plo,Phi>& operator*=(const float scalar)
        {
            spline *= scalar;
            return *this;
        }

        SplineRelation<Tx,Ty,Plo,Phi> operator/=(const float scalar)
        {
            spline *= scalar;
            return *this;
        }

        SplineRelation<Tx,Ty,Plo,Phi>& operator+=(const SplineRelation<Tx,Ty,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            spline += yscale * compose(other.spline, math::Scaling(Tscale));
            return *this;
        }

        SplineRelation<Tx,Ty,Plo,Phi>& operator-=(const SplineRelation<Tx,Ty,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            spline += yscale * compose(other.spline, math::Scaling(Tscale));
            return *this;
        }

    };

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> operator+(const SplineRelation<Tx,Ty,Plo,Phi>& relation, const SplineRelation<Tx,Ty,Plo,Phi>& other)
    {
        SplineRelation<Tx,Ty,Plo,Phi> result = relation;
        result += other;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> operator-(const SplineRelation<Tx,Ty,Plo,Phi>& relation, const SplineRelation<Tx,Ty,Plo,Phi>& other)
    {
        SplineRelation<Tx,Ty,Plo,Phi> result = relation;
        result -= other;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> operator+(const SplineRelation<Tx,Ty,Plo,Phi>& relation, const Ty offset)
    {
        SplineRelation<Tx,Ty,Plo,Phi> result = relation;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> operator-(const SplineRelation<Tx,Ty,Plo,Phi>& relation, const Ty offset)
    {
        SplineRelation<Tx,Ty,Plo,Phi> result = relation;
        result -= offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> operator*(const SplineRelation<Tx,Ty,Plo,Phi>& relation, const float scalar)
    {
        SplineRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> operator/(const SplineRelation<Tx,Ty,Plo,Phi>& relation, const float scalar)
    {
        SplineRelation<Tx,Ty,Plo,Phi> result = relation;
        result /= scalar;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> operator+(const Ty offset, const SplineRelation<Tx,Ty,Plo,Phi>& relation)
    {
        SplineRelation<Tx,Ty,Plo,Phi> result = relation;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> operator-(const Ty offset, const SplineRelation<Tx,Ty,Plo,Phi>& relation)
    {
        SplineRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> operator-(const SplineRelation<Tx,Ty,Plo,Phi>& relation)
    {
        SplineRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= -1.0f;
        return result;
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> operator*(const float scalar, const SplineRelation<Tx,Ty,Plo,Phi>& relation)
    {
        SplineRelation<Tx,Ty,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }


    // // functions that cast input to the spline, or output from the spline
    // template<typename Tx, typename Ty, int Plo, int Phi>
    // SplineRelation<Tx,Ty,Plo,Phi> operator*(const SplineRelation<Tx,float,Plo,Phi>& relation, const Ty yunits2)
    // {
    //     return SplineRelation<Tx,Ty,Plo,Phi>(relation.spline, relation.xunits, yunits2);
    // }
    // template<typename Tx, typename Ty, int Plo, int Phi>
    // SplineRelation<Tx,float,Plo,Phi> operator/(const SplineRelation<Tx,Ty,Plo,Phi>& relation, const Ty yunits2)
    // {
    //     return SplineRelation<Tx,float,Plo,Phi>(relation.spline, relation.xunits, relation.yunits/yunits2);
    // }
    // template<typename Tx, int Plo, int Phi>
    // math::Spline<float,Plo,Phi> compose(const SplineRelation<Tx,float,Plo,Phi>& relation, const math::Cast<float,Tx>& cast)
    // {
    //     return relation.spline * (cast.yunits/Tx(1.0f))/cast.xunits;
    // }
    // template<typename Tx, int Plo, int Phi>
    // SplineRelation<Tx,float,Plo,Phi> compose(const math::Spline<float,Plo,Phi>& spline, const math::Cast<Tx,float>& cast)
    // {
    //     return SplineRelation<Tx,float,Plo,Phi>(spline, 1.0f/cast.xunits, 1.0f);
    // }


    template<typename Tx, typename Ty, int Plo, int Phi, int Qlo, int Qhi>
    float distance(
        const relation::SplineRelation<Tx,Ty,Plo,Phi>& a, 
        const relation::SplineRelation<Tx,Ty,Qlo,Qhi>& b, 
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

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<Tx,Ty,Plo,Phi> spline_constant(const Ty k)
    {
        using F = math::Polynomial<float,Plo,Phi>;
        F f; f[0] = k/Ty(1.0f);
        float oo = std::numeric_limits<float>::max();
        return SplineRelation<Tx,Ty,Plo,Phi>(
            math::Spline<float,Plo,Phi>{
                math::Piecewise<float,F>(-oo, oo, f)
            }, Tx(1.0f), Ty(1.0f)
        );
    }

    template<typename Tx, typename Ty, int Plo, int Phi>
    SplineRelation<si::wavenumber,Ty,0,1> spectral_constant(const Ty k)
    {
        return spline_constant<si::wavenumber,Ty,0,1>(k);
    }

    // TODO: rename `spectral_linear_spline`
    template<typename Ty>
    SplineRelation<si::wavenumber,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength(
        const si::length lunits, const Ty yunits,
        const std::vector<double> ls, 
        const std::vector<double> lys
    ){
        assert(ls.size() == lys.size());
        const si::wavenumber nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> ys;
        for (std::size_t i=0; i<lys.size(); i++){
            ns.push_back(1.0/(ls[i]));
            ys.push_back(lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(ys.begin(), ys.end());
        return SplineRelation<si::wavenumber,Ty,0,1>(math::spline::linear_spline(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    SplineRelation<si::wavenumber,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber(
        const si::wavenumber nunits, const Ty yunits,
        const std::vector<double> ns, 
        const std::vector<double> ys
    ){
        assert(ns.size() == ys.size());
        return SplineRelation<si::wavenumber,Ty,0,1>(math::spline::linear_spline(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    SplineRelation<si::wavenumber,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output(
        const si::wavenumber nunits, const Ty yunits,
        const std::vector<double>      ns, 
        const std::vector<double> log10ys
    ){
        assert(ns.size() == log10ys.size());
        std::vector<double> ys;
        for (std::size_t i=0; i<ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return SplineRelation<si::wavenumber,Ty,0,1> (math::spline::linear_spline(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    SplineRelation<si::wavenumber,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_output(
        const si::length lunits, const Ty yunits,
        const std::vector<double>      ls, 
        const std::vector<double>log10lys
    ){
        assert(ls.size() == log10lys.size());
        const si::wavenumber nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> log10ys;
        for (std::size_t i=0; i<log10lys.size(); i++){
            ns.push_back(1.0/(ls[i]));
            log10ys.push_back(log10lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(log10ys.begin(), log10ys.end());
        std::vector<double> ys;
        for (std::size_t i=0; i<ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return SplineRelation<si::wavenumber,Ty,0,1> (math::spline::linear_spline(ns, ys), nunits, yunits);
    }


}}

