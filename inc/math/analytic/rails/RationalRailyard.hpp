#pragma once

// in-house libraries
#include "../Identity.hpp"
#include "../Scaling.hpp"
#include "../Shifting.hpp"
#include "../Rational.hpp"

namespace analytic {

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    using RationalRailyard = Railyard<T,Rational<T,Plo,Phi,Qlo,Qhi>>;

    // an operation that defines Rationals, 
    // implemented first to simplify implementation of other operations that are closed under Rationals
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const PolynomialRailyard<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        using P = Polynomial<T,Plo,Phi>;
        using Q = Polynomial<T,Qlo,Qhi>;
        using R = Rational<T,Plo,Phi,Qlo,Qhi>;
        using C = Railcar<T,R>;
        const std::vector<T> couplers_ = couplers(p,q);
        std::vector<C> cars;
        for (std::size_t i=1; i<couplers_.size(); i++)
        {
            P pi;
            Q qi;
            // add together all cars that intersect the region from couplers_[i-1] to couplers_[i]
            for (auto car: p.cars)
            {
                if (std::max(car.lo, couplers_[i-1]) < std::min(car.hi, couplers_[i]))
                {
                    pi += car.content;
                }
            }
            // add together all cars that intersect the region from couplers_[i-1] to couplers_[i]
            for (auto car: q.cars)
            {
                if (std::max(car.lo, couplers_[i-1]) < std::min(car.hi, couplers_[i]))
                {
                    qi += car.content;
                }
            }
            cars.push_back(C(couplers_[i-1], couplers_[i], pi/qi));
        }
        return Railyard<T,R>(cars);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const PolynomialRailcar<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        return PolynomialRailyard<T,Plo,Phi>(p)/q;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const Polynomial<T,Plo,Phi>& p, const PolynomialRailyard<T,Qlo,Qhi>& q)
    {
        return PolynomialRailyard<T,Qlo,Qhi>(p)/q;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const PolynomialRailyard<T,Plo,Phi>& p, const Polynomial<T,Qlo,Qhi>& q)
    {
        return p/PolynomialRailyard<T,Qlo,Qhi>(q);
    }


    /*
    NOTE: we cannot support division by railcars.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 


    // operations that are closed under Rationals
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
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailcar<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>(r)*s;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const Rational<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>(r)*s;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const RationalRailcar<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return r*RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        return r*RationalRailyard<T,P2lo,P2hi,Q2lo,Q2hi>(s);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi, int Q2lo, int Q2hi>
    constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Rational<T,P2lo,P2hi,Q2lo,Q2hi>& s)
    {
        using R = Rational<T,P1lo+Q2lo,P1hi+Q2hi,Q1lo+P2lo,Q1hi+P2hi>;
        Railyard<T,R> y;
        Railcar<T,R> yij;
        for (auto ri: r.cars)
        {
            for (auto si: s.cars)
            {
                yij = ri/si;
                if (yij.lo < yij.hi)
                {
                    y += yij;
                }
            }
        }
        return y;
    }

    /*
    NOTE: we cannot support division by rational railyards.
    This is because we cannot distibute division across the sum of rationals in the denominator.
    We can support division by rational trains, however if we convert a railyard to a train,
    then the degree of the rationals in the resulting train cannot be known at compile time.
    */ 

    /*
    NOTE: we cannot support division by railcars of any kind.
    This is because the resulting function object would produce a division by 0 for the vast majority of its range.
    */ 

    // Operations with polynomials
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r+RationalRailyard<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r-RationalRailyard<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r*RationalRailyard<T,P2lo,P2hi,0,0>(p);
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r, const Polynomial<T,P2lo,P2hi> p)
    {
        return r.p / (r.q*p);
    }

    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator+(const Polynomial<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (p*r.q+r.p) / r.q;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator-(const Polynomial<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (p*r.q-r.p) / r.q;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator*(const Polynomial<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (r.p*p) / r.q;
    }
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi, int P2lo, int P2hi>
    constexpr auto operator/(const Polynomial<T,P2lo,P2hi> p, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return (p*r.q)/r.p;
    }


    // Operations with scalings
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
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Scaling<T> f, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(f) / r;
    }


    // Operations with shiftings
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
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Shifting<T> f, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,1>(f) / r;
    }



    // Operations with identity
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
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const Identity<T> e, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,1,1>(e) / r;
    }



    // Operations with scalars
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
    template<typename T, int P1lo, int P1hi, int Q1lo, int Q1hi>
    constexpr auto operator/(const T k, const RationalRailyard<T,P1lo,P1hi,Q1lo,Q1hi>& r)
    {
        return Polynomial<T,0,0>(k) / r;
    }









    /* 
    NOTE: we avoid expressing derivatives and integrals as methods,
    since not all equations are closed under derivatives and integrals,
    and such methods would complicate the order with which classes must be declared
    */
    template<typename T>
    constexpr T derivative(const RationalRailyard<T,0,1,0,0> r) 
    {
        return r.p[0];
    }
    template<typename T>
    constexpr T derivative(const RationalRailyard<T,1,1,0,0> r) 
    {
        return T(0);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    auto derivative(const RationalRailyard<T,Plo,Phi,Qlo,Qhi>& r)
    {
        return (r.p.derivative()*r.q + r.q.derivative()*r.p) / (r.q*r.q);
    }





    template<typename T, int Plo, int Phi, int Qlo, int Qhi, typename F>
    constexpr Rational<T,Plo,Phi,Qlo,Qhi> compose(const Rational<T,Plo,Phi,Qlo,Qhi>& r, const F& f)
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
        return compose(r.p, f) / compose(r.q, f);
    }





}