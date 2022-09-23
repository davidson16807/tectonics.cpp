#pragma once

// in-house libraries
#include <math/expression/Scaling.hpp>
#include <math/expression/PolynomialRailyard.hpp>
#include <math/expression/PolynomialRailyard_library.hpp>
#include <units/si.hpp>

namespace compound {
namespace relation {

    /*
    `Relation` represents an arbitrary relation between quantites of different dimension
    */
    template<typename Tx, typename Ty, typename F>
    struct Relation
    {
        F expression;
        Tx xunits;
        Ty yunits;
        
        using input_type = Tx;
        using value_type = Ty;

        // zero constructor
        constexpr Relation():
            expression(),
            xunits(1.0),
            yunits(1.0)
        {
        }

        // constant constructor
        constexpr Relation(const Ty& other):
            expression(other/Ty(1.0)),
            xunits(1.0),
            yunits(1.0)
        {
        }

        // default constructor
        constexpr Relation(
            const F& expression,
            const Tx xunits,
            const Ty yunits
        ):
            expression(expression),
            xunits(xunits),
            yunits(yunits)
        {
        }

        // copy constructor
        constexpr Relation(
            const Relation<Tx, Ty, F>& other
        ):
            expression(other.expression),
            xunits(other.xunits),
            yunits(other.yunits)
        {
        }

        // cast constructor
        template<typename G>
        explicit constexpr Relation(
            const G expression,
            const Tx xunits,
            const Ty yunits
        ):
            expression(expression),
            xunits(xunits),
            yunits(yunits)
        {
        }

        // cast constructor
        template<typename G>
        constexpr Relation(
            const Relation<Tx, Ty, G>& other
        ):
            expression(other.expression),
            xunits(other.xunits),
            yunits(other.yunits)
        {
        }

        // constant assignment
        constexpr Relation<Tx,Ty,F>& operator=(const Ty& other)
        {
            expression = F(other/Ty(1.0));
            xunits = Tx(1.0);
            yunits = Ty(1.0);
            return *this;
        }

        // default assignment
        constexpr Relation<Tx,Ty,F>& operator=(const Relation<Tx,Ty,F>& other)
        {
            expression = other.expression;
            xunits = other.xunits;
            yunits = other.yunits;
            return *this;
        }

        // cast assignment
        template<typename G>
        constexpr Relation<Tx,Ty,F>& operator=(const Relation<Tx,Ty,G>& other)
        {
            expression = F(other.expression);
            xunits = other.xunits;
            yunits = other.yunits;
            return *this;
        }

        Ty operator()(const Tx x) const
        {
            return expression(float(x/xunits)) * yunits;
        }

        Relation<Tx,Ty,F> restriction(
            const Tx xlo, const Tx xhi,
            const float known_max_fractional_error
        ) const
        {
            return Relation<Tx,Ty,F>(restriction(expression, xlo/xunits, xhi/xunits), xunits, yunits);
        }

        Relation<Tx,Ty,F>& operator+=(const Ty offset)
        {
            expression += offset/yunits;
            return *this;
        }

        Relation<Tx,Ty,F>& operator-=(const Ty offset)
        {
            expression -= offset/yunits;
            return *this;
        }

        Relation<Tx,Ty,F>& operator*=(const float scalar)
        {
            expression *= scalar;
            return *this;
        }

        Relation<Tx,Ty,F> operator/=(const float scalar)
        {
            expression *= scalar;
            return *this;
        }

        Relation<Tx,Ty,F>& operator+=(const Relation<Tx,Ty,F>& other)
        {
            const float xscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            expression += yscale * compose(other.expression, math::Scaling(xscale));
            return *this;
        }

        Relation<Tx,Ty,F>& operator-=(const Relation<Tx,Ty,F>& other)
        {
            const float xscale = float(other.xunits / xunits);
            const float yscale = float(other.yunits / yunits);
            expression += yscale * compose(other.expression, math::Scaling(xscale));
            return *this;
        }

    };

    template<typename Tx, typename Ty, typename F>
    Relation<Tx,Ty,F> operator+(const Relation<Tx,Ty,F>& relation, const Relation<Tx,Ty,F>& other)
    {
        Relation<Tx,Ty,F> result = relation;
        result += other;
        return result;
    }

    template<typename Tx, typename Ty, typename F>
    Relation<Tx,Ty,F> operator-(const Relation<Tx,Ty,F>& relation, const Relation<Tx,Ty,F>& other)
    {
        Relation<Tx,Ty,F> result = relation;
        result -= other;
        return result;
    }

    template<typename Tx, typename Ty, typename F>
    Relation<Tx,Ty,F> operator+(const Relation<Tx,Ty,F>& relation, const Ty offset)
    {
        Relation<Tx,Ty,F> result = relation;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, typename F>
    Relation<Tx,Ty,F> operator-(const Relation<Tx,Ty,F>& relation, const Ty offset)
    {
        Relation<Tx,Ty,F> result = relation;
        result -= offset;
        return result;
    }

    template<typename Tx, typename Ty, typename F>
    Relation<Tx,Ty,F> operator*(const Relation<Tx,Ty,F>& relation, const float scalar)
    {
        Relation<Tx,Ty,F> result = relation;
        result *= scalar;
        return result;
    }

    template<typename Tx, typename Ty, typename F>
    Relation<Tx,Ty,F> operator/(const Relation<Tx,Ty,F>& relation, const float scalar)
    {
        Relation<Tx,Ty,F> result = relation;
        result /= scalar;
        return result;
    }

    template<typename Tx, typename Ty, typename F>
    Relation<Tx,Ty,F> operator+(const Ty offset, const Relation<Tx,Ty,F>& relation)
    {
        Relation<Tx,Ty,F> result = relation;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, typename F>
    Relation<Tx,Ty,F> operator-(const Ty offset, const Relation<Tx,Ty,F>& relation)
    {
        Relation<Tx,Ty,F> result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    template<typename Tx, typename Ty, typename F>
    Relation<Tx,Ty,F> operator-(const Relation<Tx,Ty,F>& relation)
    {
        Relation<Tx,Ty,F> result = relation;
        result *= -1.0f;
        return result;
    }

    template<typename Tx, typename Ty, typename F>
    Relation<Tx,Ty,F> operator*(const float scalar, const Relation<Tx,Ty,F>& relation)
    {
        Relation<Tx,Ty,F> result = relation;
        result *= scalar;
        return result;
    }


    // // functions that cast input to the expression, or output from the expression
    // template<typename Tx, typename Ty, typename F>
    // Relation<Tx,Ty,F> operator*(const Relation<Tx,float,F>& relation, const Ty yunits2)
    // {
    //     return Relation<Tx,Ty,F>(relation.expression, relation.xunits, yunits2);
    // }
    // template<typename Tx, typename Ty, typename F>
    // Relation<Tx,float,F> operator/(const Relation<Tx,Ty,F>& relation, const Ty yunits2)
    // {
    //     return Relation<Tx,float,F>(relation.expression, relation.xunits, relation.yunits/yunits2);
    // }
    // template<typename Tx, typename F>
    // math::PolynomialRailyard<float,F> compose(const Relation<Tx,float,F>& relation, const math::Cast<float,Tx>& cast)
    // {
    //     return relation.expression * (cast.yunits/Tx(1.0f))/cast.xunits;
    // }
    // template<typename Tx, typename F>
    // Relation<Tx,float,F> compose(const math::PolynomialRailyard<float,F>& expression, const math::Cast<Tx,float>& cast)
    // {
    //     return Relation<Tx,float,F>(expression, 1.0f/cast.xunits, 1.0f);
    // }


    template<typename Tx, typename Ty, typename F, typename G>
    float distance(
        const relation::Relation<Tx,Ty,F>& a, 
        const relation::Relation<Tx,Ty,G>& b, 
        const Tx xlo, 
        const Tx xhi
    ){
        return math::distance(
            compose(a.expression*float(Ty(1.0)/a.yunits), math::Scaling<float>(float(Tx(1.0)/a.xunits))),
            compose(b.expression*float(Ty(1.0)/b.yunits), math::Scaling<float>(float(Tx(1.0)/b.xunits))),
            float(xlo/Tx(1.0f)), 
            float(xhi/Tx(1.0f))
        );
    }

}}

