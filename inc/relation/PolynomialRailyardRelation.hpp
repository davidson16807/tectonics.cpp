#pragma once

#include <vector>
#include <initializer_list>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/PolynomialRailyard_library.hpp>
#include <unit/si.hpp>

namespace relation {

    /*
    `PolynomialRailyardRelation` represents an arbitrary function that uses a railyard to map quantities from one unit to another
    See `math/expression/Railyard.hpp` for the definition and motivation of a "railyard".
    */
    template<typename Tx, typename Ty, int Plo, int Phi>
    struct PolynomialRailyardRelation
    {
        analytic::PolynomialRailyard<float, Plo, Phi> yard;
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
            const analytic::PolynomialRailyard<T, Plo, Phi> yard,
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
            const analytic::PolynomialRailyard<T, Qlo, Qhi> yard,
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
            yard += yscale * compose(other.yard, analytic::Scaling(Tscale));
            return *this;
        }

        PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& operator-=(const PolynomialRailyardRelation<Tx,Ty,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            yard += yscale * compose(other.yard, analytic::Scaling(Tscale));
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
    // analytic::PolynomialRailyard<float,Plo,Phi> compose(const PolynomialRailyardRelation<Tx,float,Plo,Phi>& relation, const analytic::Cast<float,Tx>& cast)
    // {
    //     return relation.yard * (cast.yunits/Tx(1.0f))/cast.xunits;
    // }
    // template<typename Tx, int Plo, int Phi>
    // PolynomialRailyardRelation<Tx,float,Plo,Phi> compose(const analytic::PolynomialRailyard<float,Plo,Phi>& yard, const analytic::Cast<Tx,float>& cast)
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
        return analytic::distance(
            compose(a.yard*float(Ty(1.0)/a.yunits), analytic::Scaling<float>(float(Tx(1.0)/a.xunits))),
            compose(b.yard*float(Ty(1.0)/b.yunits), analytic::Scaling<float>(float(Tx(1.0)/b.xunits))),
            float(xlo/Tx(1.0f)), 
            float(xhi/Tx(1.0f))
        );
    }


    // TODO: rename `spectral_linear_yard`
    template<typename Tx, typename Ty>
    PolynomialRailyardRelation<Tx,Ty,0,1> get_linear_interpolation_function(
        const Tx xunits, const Ty yunits,
        const std::vector<double>& xs, 
        const std::vector<double>& ys
    ){
        assert(xs.size() == ys.size());
        return PolynomialRailyardRelation<Tx,Ty,0,1>(analytic::spline::linear_spline<double>(xs, ys), xunits, yunits);
    }
    
    // TODO: rename `spectral_linear_yard`
    template<typename Tx, typename Ty, typename Xs, typename Ys>
    PolynomialRailyardRelation<Tx,Ty,0,1> get_linear_interpolation_function_from_series(
        const Tx xunits, const Ty yunits,
        const Xs& xs, 
        const Ys& ys
    ){
        using scalar = typename Xs::value_type;
        assert(xs.size() == ys.size());
        return PolynomialRailyardRelation<Tx,Ty,0,1>(analytic::spline::linear_spline<scalar>(xs, ys), xunits, yunits);
    }

    // Overload for initializer_list to support brace-initialization
    template<typename Tx, typename Ty, typename scalar>
    PolynomialRailyardRelation<Tx,Ty,0,1> get_linear_interpolation_function(
        const Tx xunits, const Ty yunits,
        std::initializer_list<scalar> xs, 
        std::initializer_list<scalar> ys
    ){
        std::vector<scalar> xvec(xs);
        std::vector<scalar> yvec(ys);
        assert(xvec.size() == yvec.size());
        return PolynomialRailyardRelation<Tx,Ty,0,1>(analytic::spline::linear_spline<scalar>(xvec, yvec), xunits, yunits);
    }
    
}

