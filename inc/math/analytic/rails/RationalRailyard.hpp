#pragma once

// in-house libraries
#include "../Identity.hpp"
#include "../Scaling.hpp"
#include "../Shifting.hpp"
#include "../Rational.hpp"

#include "Railcar.hpp"
#include "Railyard.hpp"
#include "RationalRailcar.hpp"

namespace analytic {

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    using RationalRailyard = Railyard<T,Rational<T,Plo,Phi,Qlo,Qhi>>;


    // operations that are closed under Rationals
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        using R = Rational<T,std::min(P1lo,P2lo),std::max(P1hi,P2hi),std::min(Q1lo,Q2lo),std::max(Q1hi,Q2hi)>;
        using C = Railcar<T,R>;
        Railyard<T,R> y;
        for (auto ri: r.cars)
        {
            y += C(ri);
        }
        for (auto ri: s.cars)
        {
            y += C(ri);
        }
        return y;
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        using R = Rational<T,std::min(P1lo,P2lo),std::max(P1hi,P2hi),std::min(Q1lo,Q2lo),std::max(Q1hi,Q2hi)>;
        using C = Railcar<T,R>;
        Railyard<T,R> y;
        for (auto ri: r.cars)
        {
            y += C(ri);
        }
        for (auto ri: s.cars)
        {
            y -= C(ri);
        }
        return y;
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        using R = Rational<T,P1lo+P2lo,P1hi+P2hi,Q1lo+Q2lo,Q1hi+Q2hi>;
        Railyard<T,R> y;
        Railcar<T,R> yij;
        for (auto ri: r.cars)
        {
            for (auto si: s.cars)
            {
                yij = ri*si;
                if (yij.lo < yij.hi)
                {
                    y += yij;
                }
            }
        }
        return y;
    }







    // OPERATIONS WITH POLYNOMIAL RAILCARS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailyard<T,P2lo,P2hi>& p)
    {
        return r + RationalRailyard<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailyard<T,P2lo,P2hi>& p)
    {
        return r - RationalRailyard<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailyard<T,P2lo,P2hi>& p)
    {
        return r * RationalRailyard<T,P2lo,P2hi,0,0>(p);
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailyard<T,P2lo,P2hi>& p)
    // {
    //     return r * RationalRailyard<T,P2lo,P2hi,0,0>(p);
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars,
    And though the sum of rationals is a rational, there are an arbitrary number of terms in the sum, 
    and the degree of the resulting rational could not be known at compile time.
    */ 

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const PolynomialRailyard<T,P2lo,P2hi>& p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailyard<T,P2lo,P2hi,0,0>(p) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const PolynomialRailyard<T,P2lo,P2hi>& p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailyard<T,P2lo,P2hi,0,0>(p) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialRailyard<T,P2lo,P2hi>& p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailyard<T,P2lo,P2hi,0,0>(p) * r;
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const PolynomialRailyard<T,P2lo,P2hi>& p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return RationalRailyard<T,P2lo,P2hi,0,0>(p) / r;
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars,
    And though the sum of rationals is a rational, there are an arbitrary number of terms in the sum, 
    and the degree of the resulting rational could not be known at compile time.
    */ 






    // OPERATIONS WITH RATIONAL RAILCARS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r + RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r - RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r * RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    // constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s)
    // {
    //     return s / RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars,
    And though the sum of rationals is a rational, there are an arbitrary number of terms in the sum, 
    and the degree of the resulting rational could not be known at compile time.
    */ 

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s) * r;
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    // constexpr auto operator/(const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s) / r;
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars,
    And though the sum of rationals is a rational, there are an arbitrary number of terms in the sum, 
    and the degree of the resulting rational could not be known at compile time.
    */ 



    // OPERATIONS WITH POLYNOMIAL RAILCARS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    {
        return r + RationalRailcar<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    {
        return r - RationalRailcar<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    {
        return r * RationalRailcar<T,P2lo,P2hi,0,0>(p);
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const PolynomialRailcar<T,P2lo,P2hi> p)
    // {
    //     return r * RationalRailcar<T,P2lo,P2hi,0,0>(p);
    // }
    /*
    NOTE: we cannot support division by railcars of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars
    */ 

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailcar<T,P2lo,P2hi,0,0>(p) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailcar<T,P2lo,P2hi,0,0>(p) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailcar<T,P2lo,P2hi,0,0>(p) * r;
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const PolynomialRailcar<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return RationalRailcar<T,P2lo,P2hi,0,0>(p) / r;
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars,
    And though the sum of rationals is a rational, there are an arbitrary number of terms in the sum, 
    and the degree of the resulting rational could not be known at compile time.
    */ 



    // OPERATIONS WITH RATIONALS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r + RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r - RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r * RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        return r / RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator+(const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator-(const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s) * r;
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    // constexpr auto operator/(const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s) / r;
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railyards
    */ 



    // OPERATIONS WITH POLYNOMIALS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r + Rational<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r - Rational<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r * Rational<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r * Rational<T,P2lo,P2hi,0,0>(p);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const Polynomial<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const Polynomial<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Polynomial<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Rational<T,P2lo,P2hi,0,0>(p) * r;
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    // constexpr auto operator/(const Polynomial<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return Rational<T,P2lo,P2hi,0,0>(p) / r;
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars,
    And though the sum of rationals is a rational, there are an arbitrary number of terms in the sum, 
    and the degree of the resulting rational could not be known at compile time.
    */ 



    // OPERATIONS WITH SCALINGS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r + Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r - Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r * Polynomial<T,1,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Scaling<T> f)
    {
        return r / Polynomial<T,1,1>(f);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Scaling<T> f, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Scaling<T> f, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Scaling<T> f, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) * r;
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator/(const Scaling<T> f, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return Polynomial<T,1,1>(f) / r;
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars,
    And though the sum of rationals is a rational, there are an arbitrary number of terms in the sum, 
    and the degree of the resulting rational could not be known at compile time.
    */ 


    // OPERATIONS WITH SHIFTINGS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r + Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r - Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r * Polynomial<T,0,1>(f);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Shifting<T> f)
    {
        return r / Polynomial<T,0,1>(f);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Shifting<T> f, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Shifting<T> f, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Shifting<T> f, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) * r;
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator/(const Shifting<T> f, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return Polynomial<T,0,1>(f) / r;
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars,
    And though the sum of rationals is a rational, there are an arbitrary number of terms in the sum, 
    and the degree of the resulting rational could not be known at compile time.
    */ 



    // OPERATIONS WITH IDENTITY
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r + Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r - Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r * Polynomial<T,1,1>(e);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Identity<T> e)
    {
        return r / Polynomial<T,1,1>(e);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const Identity<T> e, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const Identity<T> e, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const Identity<T> e, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) * r;
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator/(const Identity<T> e, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return Polynomial<T,1,1>(e) / r;
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars,
    And though the sum of rationals is a rational, there are an arbitrary number of terms in the sum, 
    and the degree of the resulting rational could not be known at compile time.
    */ 



    // OPERATIONS WITH SCALARS
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    {
        return r + Polynomial<T,0,0>(k);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    {
        return r - Polynomial<T,0,0>(k);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    {
        return r * Polynomial<T,0,0>(k);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const T k)
    {
        return r / Polynomial<T,0,0>(k);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator+(const T k, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,0>(k) + r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator-(const T k, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,0>(k) - r;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator*(const T k, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,0>(k) * r;
    }
    // template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    // constexpr auto operator/(const T k, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    // {
    //     return Polynomial<T,0,0>(k) / r;
    // }
    /*
    NOTE: we cannot support division by railyards of any kind.
    This is because the denominator could not distribute across the numerator to produce something that is closed under rational railcars,
    And though the sum of rationals is a rational, there are an arbitrary number of terms in the sum, 
    and the degree of the resulting rational could not be known at compile time.
    */ 









    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    template<typename T>
    constexpr T derivative(const RationalRailyard<T,0,1,0,0> r) 
    {
        T y;
        for (int i = 0; i < r.size(); ++i)
        {
            y += r.p[0];
        }
        return y;
    }
    template<typename T>
    constexpr T derivative(const RationalRailyard<T,1,1,0,0> r) 
    {
        return T(0);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    auto derivative(const RationalRailyard<T,Plo,Phi,Qlo,Qhi>& r)
    {
        RationalRailyard<T,Plo,Phi,Qlo,Qhi> y;
        for (int i = 0; i < r.size(); ++i)
        {
            y += r[i].derivative();
        }
        return y;
    }






    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr RationalRailyard<T,Plo,Phi,Qlo,Qhi> compose(const RationalRailyard<T,Plo,Phi,Qlo,Qhi>& r, const Identity<T> e)
    {
        return r;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr RationalRailyard<T,Plo,Phi,Qlo,Qhi> compose(const Identity<T> e, const RationalRailyard<T,Plo,Phi,Qlo,Qhi>& r)
    {
        return r;
    }

    template<typename T, int Phi, int Qhi>
    constexpr RationalRailyard<T,0,Phi,0,Qhi> compose(const RationalRailyard<T,0,Phi,0,Qhi>& r, const Shifting<T> f)
    {
        using R = Rational<T,0,Phi,0,Qhi>;
        Railyard<T,R> y;
        for (auto ri: r.cars)
        {
            y += compose(ri,f);
        }
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr RationalRailyard<T,Plo,Phi,Qlo,Qhi> compose(const RationalRailyard<T,Plo,Phi,Qlo,Qhi>& r, const Scaling<T> f)
    {
        using R = Rational<T,Plo,Phi,Qlo,Qhi>;
        Railyard<T,R> y;
        for (auto ri: r.cars)
        {
            y += compose(ri,f);
        }
        return y;
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi> compose(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        using R = Rational<T,P1lo*P2lo,P1hi*P2hi,Q1lo+P2lo,Q1hi+P2hi>;
        Railyard<T,R> y;
        for (auto ri: r.cars)
        {
            y += compose(ri,p);
        }
        return y;
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi> compose(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi> s)
    {
        using R = Rational<T,P1lo*P2lo,P1hi*P2hi,Q1lo*Q2lo,Q1hi*Q2hi>;
        Railyard<T,R> y;
        for (auto ri: r.cars)
        {
            y += compose(ri,s);
        }
        return y;
    }

}