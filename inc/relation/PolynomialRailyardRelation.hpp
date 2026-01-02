#pragma once

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
    template<typename X, typename Y, int Plo, int Phi>
    struct PolynomialRailyardRelation
    {
        analytic::PolynomialRailyard<float, Plo, Phi> yard;
        X xunits;
        Y yunits;
        
        using value_type = Y;

        // zero constructor
        constexpr PolynomialRailyardRelation():
            yard(),
            xunits(1.0),
            yunits(1.0)
        {
        }

        // constant constructor
        constexpr PolynomialRailyardRelation(const Y& other):
            yard(other/Y(1.0)),
            xunits(1.0),
            yunits(1.0)
        {
        }

        // copy constructor
        template<int Qlo, int Qhi>
        constexpr PolynomialRailyardRelation(
            const PolynomialRailyardRelation<X, Y, Qlo, Qhi>& other
        ):
            yard(other.yard),
            xunits(other.xunits),
            yunits(other.yunits)
        {
        }

        template<typename T>
        constexpr PolynomialRailyardRelation(
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
        explicit constexpr PolynomialRailyardRelation(
            const analytic::PolynomialRailyard<T, Qlo, Qhi> yard,
            const X xunits,
            const Y yunits
        ):
            yard(yard),
            xunits(xunits),
            yunits(yunits)
        {
        }

        constexpr PolynomialRailyardRelation<X,Y,Plo,Phi>& operator=(const PolynomialRailyardRelation<X,Y,Plo,Phi>& other)
        {
            yard = other.yard;
            xunits = other.xunits;
            yunits = other.yunits;
            return *this;
        }

        constexpr PolynomialRailyardRelation<X,Y,Plo,Phi>& operator=(const Y& other)
        {
            yard = other/Y(1.0);
            xunits = X(1.0);
            yunits = Y(1.0);
            return *this;
        }

        Y operator()(const X x) const
        {
            return yard(float(x/xunits)) * yunits;
        }

        PolynomialRailyardRelation<X,Y,Plo,Phi> restriction(
            const X xlo, const X xhi,
            const float known_max_fractional_error
        ) const
        {
            return PolynomialRailyardRelation<X,Y,Plo,Phi>(restriction(yard, xlo/xunits, xhi/xunits), xunits, yunits);
        }

        PolynomialRailyardRelation<X,Y,Plo,Phi>& operator+=(const Y offset)
        {
            yard += offset/yunits;
            return *this;
        }

        PolynomialRailyardRelation<X,Y,Plo,Phi>& operator-=(const Y offset)
        {
            yard -= offset/yunits;
            return *this;
        }

        PolynomialRailyardRelation<X,Y,Plo,Phi>& operator*=(const float scalar)
        {
            yard *= scalar;
            return *this;
        }

        PolynomialRailyardRelation<X,Y,Plo,Phi>& operator/=(const float scalar)
        {
            yard *= scalar;
            return *this;
        }

        PolynomialRailyardRelation<X,Y,Plo,Phi>& operator+=(const PolynomialRailyardRelation<X,Y,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            yard += yscale * compose(other.yard, analytic::Scaling(Tscale));
            return *this;
        }

        PolynomialRailyardRelation<X,Y,Plo,Phi>& operator-=(const PolynomialRailyardRelation<X,Y,Plo,Phi>& other)
        {
            const float Tscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            yard += yscale * compose(other.yard, analytic::Scaling(Tscale));
            return *this;
        }

    };

    template<typename X, typename Y, int Plo, int Phi>
    PolynomialRailyardRelation<X,Y,Plo,Phi> operator+(const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation, const PolynomialRailyardRelation<X,Y,Plo,Phi>& other)
    {
        PolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result += other;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    PolynomialRailyardRelation<X,Y,Plo,Phi> operator-(const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation, const PolynomialRailyardRelation<X,Y,Plo,Phi>& other)
    {
        PolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result -= other;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    PolynomialRailyardRelation<X,Y,Plo,Phi> operator+(const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation, const Y offset)
    {
        PolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result += offset;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    PolynomialRailyardRelation<X,Y,Plo,Phi> operator-(const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation, const Y offset)
    {
        PolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result -= offset;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    PolynomialRailyardRelation<X,Y,Plo,Phi> operator*(const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation, const float scalar)
    {
        PolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    PolynomialRailyardRelation<X,Y,Plo,Phi> operator/(const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation, const float scalar)
    {
        PolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result /= scalar;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    PolynomialRailyardRelation<X,Y,Plo,Phi> operator+(const Y offset, const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation)
    {
        PolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result += offset;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    PolynomialRailyardRelation<X,Y,Plo,Phi> operator-(const Y offset, const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation)
    {
        PolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    PolynomialRailyardRelation<X,Y,Plo,Phi> operator-(const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation)
    {
        PolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result *= -1.0f;
        return result;
    }

    template<typename X, typename Y, int Plo, int Phi>
    PolynomialRailyardRelation<X,Y,Plo,Phi> operator*(const float scalar, const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation)
    {
        PolynomialRailyardRelation<X,Y,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }


    // // functions that cast input to the yard, or output from the yard
    // template<typename X, typename Y, int Plo, int Phi>
    // PolynomialRailyardRelation<X,Y,Plo,Phi> operator*(const PolynomialRailyardRelation<X,float,Plo,Phi>& relation, const Y yunits2)
    // {
    //     return PolynomialRailyardRelation<X,Y,Plo,Phi>(relation.yard, relation.xunits, yunits2);
    // }
    // template<typename X, typename Y, int Plo, int Phi>
    // PolynomialRailyardRelation<X,float,Plo,Phi> operator/(const PolynomialRailyardRelation<X,Y,Plo,Phi>& relation, const Y yunits2)
    // {
    //     return PolynomialRailyardRelation<X,float,Plo,Phi>(relation.yard, relation.xunits, relation.yunits/yunits2);
    // }
    // template<typename X, int Plo, int Phi>
    // analytic::PolynomialRailyard<float,Plo,Phi> compose(const PolynomialRailyardRelation<X,float,Plo,Phi>& relation, const analytic::Cast<float,X>& cast)
    // {
    //     return relation.yard * (cast.yunits/X(1.0f))/cast.xunits;
    // }
    // template<typename X, int Plo, int Phi>
    // PolynomialRailyardRelation<X,float,Plo,Phi> compose(const analytic::PolynomialRailyard<float,Plo,Phi>& yard, const analytic::Cast<X,float>& cast)
    // {
    //     return PolynomialRailyardRelation<X,float,Plo,Phi>(yard, 1.0f/cast.xunits, 1.0f);
    // }


    template<typename X, typename Y, int Plo, int Phi, int Qlo, int Qhi>
    float distance(
        const relation::PolynomialRailyardRelation<X,Y,Plo,Phi>& a, 
        const relation::PolynomialRailyardRelation<X,Y,Qlo,Qhi>& b, 
        const X xlo, 
        const X xhi
    ){
        return analytic::distance(
            compose(a.yard*float(Y(1.0)/a.yunits), analytic::Scaling<float>(float(X(1.0)/a.xunits))),
            compose(b.yard*float(Y(1.0)/b.yunits), analytic::Scaling<float>(float(X(1.0)/b.xunits))),
            float(xlo/X(1.0f)), 
            float(xhi/X(1.0f))
        );
    }


    // TODO: rename `spectral_linear_yard`
    template<typename X, typename Y>
    PolynomialRailyardRelation<X,Y,0,1> get_linear_interpolation_function(
        const X xunits, const Y yunits,
        const std::vector<double>& xs, 
        const std::vector<double>& ys
    ){
        assert(xs.size() == ys.size());
        return PolynomialRailyardRelation<X,Y,0,1>(analytic::spline::linear_spline<double>(xs, ys), xunits, yunits);
    }
    
    // TODO: rename `spectral_linear_yard`
    template<typename X, typename Y, typename Xs, typename Ys>
    PolynomialRailyardRelation<X,Y,0,1> get_linear_interpolation_function_from_series(
        const X xunits, const Y yunits,
        const Xs& xs, 
        const Ys& ys
    ){
        using scalar = typename Xs::value_type;
        assert(xs.size() == ys.size());
        return PolynomialRailyardRelation<X,Y,0,1>(analytic::spline::linear_spline<scalar>(xs, ys), xunits, yunits);
    }
    
}

