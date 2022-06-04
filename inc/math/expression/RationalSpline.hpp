#pragma once

// std libraries
#include <algorithm> // copy, back_inserter
#include <initializer_list>
#include <iostream>

// in-house libraries
#include "Identity.hpp"
#include "Scaling.hpp"
#include "Shifting.hpp"
#include "Railcar.hpp"
#include "Boxcar_to_string.hpp"
#include "Rational.hpp"
#include "Rational_to_string.hpp"
#include "Polynomial.hpp"
#include "Polynomial_to_string.hpp"
#include "Spline.hpp"

namespace math {

    /*
    A `RationalSpline` in this code base represents a sum of piecewise rationals.
    */
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    struct RationalSpline {
        std::vector<Railcar<T,Rational<T,Plo,Phi,Qlo,Qhi>>> boxes; 
        RationalSpline() : boxes() 
        {
        }
        RationalSpline(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p) : boxes(p.boxes) 
        {
        }
        explicit RationalSpline(const std::vector<Railcar<T,Rational<T,Plo,Phi,Qlo,Qhi>>> pieces_) : 
            boxes(pieces_) 
        {
        }
        explicit RationalSpline(std::initializer_list<Railcar<T,Rational<T,Plo,Phi,Qlo,Qhi>>> ts)
        {
            std::copy(ts.begin(), ts.end(), std::back_inserter(boxes));
        }
        T operator()(const T x) const
        {
            T y(0.0f);
            for (std::size_t i=0; i<boxes.size(); i++)
            {
                y += boxes[i](x);
            }
            return y;
        }

        constexpr std::vector<T> knots() const
        {
            // gather all boundaries for all boxes
            std::vector<T> bounds;
            for (std::size_t i=0; i<s.boxes.size(); i++)
            {
                bounds.push_back(s.boxes[i].lo);
                bounds.push_back(s.boxes[i].hi);
            }
            std::sort(bounds.begin(), bounds.end());
            auto last = std::unique(bounds.begin(), bounds.end());
            bounds.erase(last, bounds.end());
            return bounds;
        }

        RationalSpline<T,Plo,Phi,Qlo,Qhi>& operator+=(const T k)
        {
            using F = Rational<T,Plo,Phi,Qlo,Qhi>;
            using G = Railcar<T,F>;
            const T oo = std::numeric_limits<T>::max();
            Polynomial<T,Plo,Phi> p; p[0] = k; 
            Polynomial<T,Qlo,Qhi> q; q[0] = T(1);
            boxes.push_back(G(-oo, oo, p/q));
            return *this;
        }

        RationalSpline<T,Plo,Phi,Qlo,Qhi>& operator-=(const T k)
        {
            using F = Rational<T,Plo,Phi,Qlo,Qhi>;
            using G = Railcar<T,F>;
            const T oo = std::numeric_limits<T>::max();
            Polynomial<T,Plo,Phi> p; p[0] = -k;
            Polynomial<T,Qlo,Qhi> q; q[0] = T(1);
            boxes.push_back(G(-oo, oo, p/q));
            return *this;
        }

        RationalSpline<T,Plo,Phi,Qlo,Qhi>& operator*=(const T k)
        {
            for (std::size_t i=0; i<boxes.size(); i++)
            {
                boxes[i].f *= k;
            }
            return *this;
        }

        RationalSpline<T,Plo,Phi,Qlo,Qhi>& operator/=(const T k)
        {
            for (std::size_t i=0; i<boxes.size(); i++)
            {
                boxes[i].f /= k;
            }
            return *this;
        }

        RationalSpline<T,Plo,Phi,Qlo,Qhi>& operator+=(const Polynomial<T,Phi,Plo>& p)
        {
            using F = Rational<T,Plo,Phi,Qlo,Qhi>;
            using G = Railcar<T,F>;
            const T oo = std::numeric_limits<T>::max();
            boxes.push_back(G(-oo, oo, F(p)));
            return *this;
        }

        RationalSpline<T,Plo,Phi,Qlo,Qhi>& operator-=(const Polynomial<T,Phi,Plo>& p)
        {
            using F = Rational<T,Plo,Phi,Qlo,Qhi>;
            using G = Railcar<T,F>;
            const T oo = std::numeric_limits<T>::max();
            boxes.push_back(G(-oo, oo, F(-p)));
            return *this;
        }

        RationalSpline<T,Plo,Phi,Qlo,Qhi>& operator+=(const Rational<T,Plo,Phi,Qlo,Qhi>& p)
        {
            using F = Rational<T,Plo,Phi,Qlo,Qhi>;
            using G = Railcar<T,F>;
            const T oo = std::numeric_limits<T>::max();
            F f(p);
            boxes.push_back(G(-oo, oo, p));
            return *this;
        }
        RationalSpline<T,Plo,Phi,Qlo,Qhi>& operator-=(const Rational<T,Plo,Phi,Qlo,Qhi>& p)
        {
            using F = Rational<T,Plo,Phi,Qlo,Qhi>;
            using G = Railcar<T,F>;
            const T oo = std::numeric_limits<T>::max();
            F f(p);
            boxes.push_back(G(-oo, oo, p));
            return *this;
        }

        template<int Plo2, int Phi2, int Qlo2, int Qhi2>
        RationalSpline<T,Plo,Phi,Qlo,Qhi>& operator+=(const RationalSpline<T,Plo2,Phi2,Qlo2,Qhi2>& q)
        {
            using F = Rational<T,Plo,Phi,Qlo,Qhi>;
            using G = Railcar<T,F>;
            for (std::size_t i=0; i<q.boxes.size(); i++)
            {
                boxes.push_back(G(q.boxes[i].lo, q.boxes[i].hi, F(q.boxes[i].f)));
            }
            return *this;
        }
        template<int Plo2, int Phi2, int Qlo2, int Qhi2>
        RationalSpline<T,Plo,Phi,Qlo,Qhi>& operator-=(const RationalSpline<T,Plo2,Phi2,Qlo2,Qhi2>& q)
        {
            using F = Rational<T,Plo,Phi,Qlo,Qhi>;
            using G = Railcar<T,F>;
            for (std::size_t i=0; i<q.boxes.size(); i++)
            {
                boxes.push_back(G(q.boxes[i].lo, q.boxes[i].hi, F(-q.boxes[i].f)));
            }
            return *this;
        }
    };

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr std::string to_string(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& s)
    {
        std::string output("\r\n");
        for (std::size_t i=0; i<s.boxes.size(); i++)
        {
            output += to_string(s.boxes[i]);
            output += "\r\n";
        }
        return output;
    }



    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& f, const T k)
    {
        RationalSpline<T,std::min(0,Plo),std::max(0,Phi),std::min(0,Qlo),std::max(0,Qhi)>  y;
        y += f;
        y += k;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const T k, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& f)
    {
        RationalSpline<T,std::min(0,Plo),std::max(0,Phi),std::min(0,Qlo),std::max(0,Qhi)>  y;
        y += f;
        y += k;
        return y;
    }


    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& f, const T k)
    {
        RationalSpline<T,std::min(0,Plo),std::max(0,Phi),std::min(0,Qlo),std::max(0,Qhi)>  y;
        y += f;
        y -= k;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const T k, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& f)
    {
        RationalSpline<T,std::min(0,Plo),std::max(0,Phi),std::min(0,Qlo),std::max(0,Qhi)>  y;
        y -= f;
        y += k;
        return y;
    }


    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& f, const T k)
    {
        RationalSpline<T,Plo,Phi,Qlo,Qhi> y(f);
        y *= k;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const T k, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& f)
    {
        RationalSpline<T,Plo,Phi,Qlo,Qhi> y(f);
        y *= k;
        return y;
    }


    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& f, const T k)
    {
        RationalSpline<T,Plo,Phi,Qlo,Qhi> y(f);
        y /= k;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& f)
    {
        RationalSpline<T,Plo,Phi,Qlo,Qhi> y(f);
        y *= -1;
        return y;
    }








    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2>
    constexpr auto operator+(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& s, const Polynomial<T,Plo2,Phi2> p)
    {
        RationalSpline<T,std::min(Plo,Plo2),std::max(Phi,Phi2),Qlo,Qhi> y;
        y += s;
        y += p;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2>
    constexpr auto operator-(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& s, const Polynomial<T,Plo2,Phi2> p)
    {
        RationalSpline<T,std::min(Plo,Plo2),std::max(Phi,Phi2),Qlo,Qhi> y;
        y += s;
        y -= p;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2>
    constexpr auto operator*(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Polynomial<T,Plo2,Phi2> q)
    {
        using F = Rational<T,Plo+Plo2,Phi+Phi2,Qlo,Qhi>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f*q)));
        }
        return RationalSpline<T,Plo+Plo2,Phi+Phi2,Qlo,Qhi>(boxes);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2>
    constexpr auto operator/(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Polynomial<T,Plo2,Phi2> q)
    {
        using F = Rational<T,Plo,Phi,Qlo+Plo2,Qhi+Phi2>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f/q)));
        }
        return RationalSpline<T,Plo,Phi,Qlo+Plo2,Qhi+Phi2>(boxes);
    }


    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int P2>
    constexpr auto operator*(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Polynomial<T,P2,P2> q)
    {
        using F = Rational<T,Plo,Phi,Qlo-P2,Qhi-P2>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f*q)));
        }
        return RationalSpline<T,Plo,Phi,Qlo-P2,Qhi-P2>(boxes);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int P2>
    constexpr auto operator/(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Polynomial<T,P2,P2> q)
    {
        using F = Rational<T,Plo-P2,Phi-P2,Qlo,Qhi>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f/q)));
        }
        return RationalSpline<T,Plo-P2,Phi-P2,Qlo,Qhi>(boxes);
    }









    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2>
    constexpr auto operator+(const Polynomial<T,Plo2,Phi2> p, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& s)
    {
        RationalSpline<T,std::min(Plo,Plo2),std::max(Phi,Phi2),Qlo,Qhi> y;
        y += p;
        y += s;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2>
    constexpr auto operator-(const Polynomial<T,Plo2,Phi2> p, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& s)
    {
        RationalSpline<T,std::min(Plo,Plo2),std::max(Phi,Phi2),Qlo,Qhi> y;
        y += p;
        y -= s;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2>
    constexpr auto operator*(const Polynomial<T,Plo2,Phi2> q, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        using F = Rational<T,Plo+Plo2,Phi+Phi2,Qlo,Qhi>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f*q)));
        }
        return RationalSpline<T,Plo+Plo2,Phi+Phi2,Qlo,Qhi>(boxes);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2>
    constexpr auto operator/(const Polynomial<T,Plo2,Phi2> q, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        using F = Rational<T,Plo,Phi,Qlo+Plo2,Qhi+Phi2>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(q/p.boxes[i].f)));
        }
        return RationalSpline<T,Plo,Phi,Qlo+Plo2,Qhi+Phi2>(boxes);
    }


    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int P2>
    constexpr auto operator*(const Polynomial<T,P2,P2> q, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        using F = Rational<T,Plo,Phi,Qlo-P2,Qhi-P2>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f*q)));
        }
        return RationalSpline<T,Plo,Phi,Qlo-P2,Qhi-P2>(boxes);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int P2>
    constexpr auto operator/(const Polynomial<T,P2,P2> q, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        using F = Rational<T,Plo-P2,Phi-P2,Qlo,Qhi>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(q/p.boxes[i].f)));
        }
        return RationalSpline<T,Plo-P2,Phi-P2,Qlo,Qhi>(boxes);
    }









    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator+(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& s, const Rational<T,Plo2,Phi2,Qlo2,Qhi2> p)
    {
        RationalSpline<T,std::min(Plo,Plo2),std::max(Phi,Phi2),Qlo,Qhi> y;
        y += s;
        y += p;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator-(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& s, const Rational<T,Plo2,Phi2,Qlo2,Qhi2> p)
    {
        RationalSpline<T,std::min(Plo,Plo2),std::max(Phi,Phi2),Qlo,Qhi> y;
        y += s;
        y -= p;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator*(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Rational<T,Plo2,Phi2,Qlo2,Qhi2> q)
    {
        using F = Rational<T,Plo+Plo2,Phi+Phi2,Qlo,Qhi>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f*q)));
        }
        return RationalSpline<T,Plo+Plo2,Phi+Phi2,Qlo,Qhi>(boxes);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator/(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Rational<T,Plo2,Phi2,Qlo2,Qhi2> q)
    {
        using F = Rational<T,Plo,Phi,Qlo+Plo2,Qhi+Phi2>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f/q)));
        }
        return RationalSpline<T,Plo,Phi,Qlo+Plo2,Qhi+Phi2>(boxes);
    }









    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator+(const Rational<T,Plo2,Phi2,Qlo2,Qhi2> p, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& s)
    {
        RationalSpline<T,std::min(Plo,Plo2),std::max(Phi,Phi2),Qlo,Qhi> y;
        y += p;
        y += s;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator-(const Rational<T,Plo2,Phi2,Qlo2,Qhi2> p, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& s)
    {
        RationalSpline<T,std::min(Plo,Plo2),std::max(Phi,Phi2),Qlo,Qhi> y;
        y += p;
        y -= s;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator*(const Rational<T,Plo2,Phi2,Qlo2,Qhi2> q, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        using F = Rational<T,Plo+Plo2,Phi+Phi2,Qlo,Qhi>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f*q)));
        }
        return RationalSpline<T,Plo+Plo2,Phi+Phi2,Qlo,Qhi>(boxes);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator/(const Rational<T,Plo2,Phi2,Qlo2,Qhi2> q, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        using F = Rational<T,Plo,Phi,Qlo+Plo2,Qhi+Phi2>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(q/p.boxes[i].f)));
        }
        return RationalSpline<T,Plo,Phi,Qlo+Plo2,Qhi+Phi2>(boxes);
    }








    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator+(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const RationalSpline<T,Plo2,Phi2,Qlo2,Qhi2>& q)
    {
        using F = Rational<T,std::min(Plo,Plo2),std::max(Phi,Phi2),std::min(Qlo,Qlo2),std::max(Qhi,Qhi2)>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f)));
        }
        for (std::size_t i=0; i<q.boxes.size(); i++)
        {
            boxes.push_back(G(q.boxes[i].lo, q.boxes[i].hi, F(q.boxes[i].f)));
        }
        return RationalSpline<T,std::min(Plo,Plo2),std::max(Phi,Phi2),std::min(Qlo,Qlo2),std::max(Qhi,Qhi2)>(boxes);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator-(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const RationalSpline<T,Plo2,Phi2,Qlo2,Qhi2>& q)
    {
        using F = Rational<T,std::min(Plo,Plo2),std::max(Phi,Phi2),std::min(Qlo,Qlo2),std::max(Qhi,Qhi2)>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, F(p.boxes[i].f)));
        }
        for (std::size_t i=0; i<q.boxes.size(); i++)
        {
            boxes.push_back(G(q.boxes[i].lo, q.boxes[i].hi, F(-q.boxes[i].f)));
        }
        return RationalSpline<T,std::min(Plo,Plo2),std::max(Phi,Phi2),std::min(Qlo,Qlo2),std::max(Qhi,Qhi2)>(boxes);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator*(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const RationalSpline<T,Plo2,Phi2,Qlo2,Qhi2>& q)
    {
        using F = Rational<T,Plo+Plo2,Phi+Phi2,Qlo+Qlo2,Qhi+Qhi2>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        T lo, hi;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            for (std::size_t j=0; j<q.boxes.size(); j++)
            {
                lo = std::max(p.boxes[i].lo, q.boxes[j].lo);
                hi = std::min(p.boxes[i].hi, q.boxes[j].hi);
                if (lo<hi)
                {
                    boxes.push_back(G(lo, hi, p.boxes[i].f * q.boxes[j].f));
                }
            }
        }
        return RationalSpline<T,Plo+Plo2,Phi+Phi2,Qlo+Qlo2,Qhi+Qhi2>(boxes);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2, int Qlo2, int Qhi2>
    constexpr auto operator/(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const RationalSpline<T,Plo2,Phi2,Qlo2,Qhi2>& q)
    {
        using F = Rational<T,Plo+Qlo2,Phi+Qhi2,Qlo+Plo2,Qhi+Phi2>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        T lo, hi;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            for (std::size_t j=0; j<q.boxes.size(); j++)
            {
                lo = std::max(p.boxes[i].lo, q.boxes[j].lo);
                hi = std::min(p.boxes[i].hi, q.boxes[j].hi);
                if (lo<hi)
                {
                    boxes.push_back(G(lo, hi, p.boxes[i].f / q.boxes[j].f));
                }
            }
        }
        return RationalSpline<T,Plo+Qlo2,Phi+Qhi2,Qlo+Plo2,Qhi+Phi2>(boxes);
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const Spline<T,Plo,Phi>& p, const Spline<T,Qlo,Qhi>& q)
    {
        using F = Rational<T,Plo,Phi,Qlo,Qhi>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        T lo, hi;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            for (std::size_t j=0; j<q.boxes.size(); j++)
            {
                lo = std::max(p.boxes[i].lo, q.boxes[j].lo);
                hi = std::min(p.boxes[i].hi, q.boxes[j].hi);
                if (lo<hi)
                {
                    boxes.push_back(G(lo, hi, p.boxes[i].f / q.boxes[j].f));
                }
            }
        }
        return RationalSpline<T,Plo,Phi,Qlo,Qhi>(boxes);
    }













    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Shifting<T> f)
    {
        return p + Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Shifting<T> f, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        return Polynomial<T,0,1>(f) + p;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Shifting<T> f)
    {
        return p - Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Shifting<T> f, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        return Polynomial<T,0,1>(f) - p;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Shifting<T> f)
    {
        return p * Polynomial<T,0,1>(f);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Shifting<T> f, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        return Polynomial<T,0,1>(f) * p;
    }











    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Scaling<T> f)
    {
        return p + Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Scaling<T> f, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        return Polynomial<T,1,1>(f) + p;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Scaling<T> f)
    {
        return p - Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Scaling<T> f, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        return Polynomial<T,1,1>(f) - p;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Scaling<T> f)
    {
        return p * Polynomial<T,1,1>(f);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Scaling<T> f, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        return Polynomial<T,1,1>(f) * p;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Scaling<T> f)
    {
        return p / Polynomial<T,1,1>(f);
    }












    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Identity<T> e)
    {
        return p + Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator+(const Identity<T> e, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        return Polynomial<T,1,1>(e) + p;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Identity<T> e)
    {
        return p - Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator-(const Identity<T> e, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        return Polynomial<T,1,1>(e) - p;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Identity<T> e)
    {
        return p * Polynomial<T,1,1>(e);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator*(const Identity<T> e, const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p)
    {
        return Polynomial<T,1,1>(e) * p;
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr auto operator/(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Identity<T> e)
    {
        return p / Polynomial<T,1,1>(e);
    }












    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Plo2, int Phi2>
    constexpr RationalSpline<T,
        std::min(Plo*Plo2,Phi*Phi2),std::max(Plo*Plo2,Phi*Phi2),
        std::min(Qlo*Plo2,Qhi*Phi2),std::max(Qlo*Plo2,Qhi*Phi2)> compose(
            const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, 
            const Polynomial<T,Plo2,Phi2> q
    ){
        using F = Rational<T,
            std::min(Plo*Plo2,Phi*Phi2),std::max(Plo*Plo2,Phi*Phi2),
            std::min(Qlo*Plo2,Qhi*Phi2),std::max(Qlo*Plo2,Qhi*Phi2)>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, compose(p.boxes[i].f, q)));
        }
        return RationalSpline(boxes);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr RationalSpline<T,Plo,Phi,Qlo,Qhi> compose(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Scaling<T> g)
    {
        const int Plo2 = 1;
        const int Phi2 = 1;
        using F = Rational<T,
            std::min(Plo*Plo2,Phi*Phi2),std::max(Plo*Plo2,Phi*Phi2),
            std::min(Qlo*Plo2,Qhi*Phi2),std::max(Qlo*Plo2,Qhi*Phi2)>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, compose(p.boxes[i].f, g)));
        }
        return RationalSpline(boxes);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr RationalSpline<T,Plo,Phi,Qlo,Qhi> compose(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Shifting<T> g)
    {
        const int Plo2 = 0;
        const int Phi2 = 1;
        using F = Rational<T,
            std::min(Plo*Plo2,Phi*Phi2),std::max(Plo*Plo2,Phi*Phi2),
            std::min(Qlo*Plo2,Qhi*Phi2),std::max(Qlo*Plo2,Qhi*Phi2)>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(G(p.boxes[i].lo, p.boxes[i].hi, compose(p.boxes[i].f, g)));
        }
        return RationalSpline(boxes);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr RationalSpline<T,Plo,Phi,Qlo,Qhi> compose(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const Identity<T> e)
    {
        return p;
    }


    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr RationalSpline<T,Qlo-1+Plo, Qhi-1+Phi, Qlo*2, Qhi*2> derivative(
        const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p
    ){
        using PiecewisePolynomial = Railcar<T,Polynomial<T,Plo-1,Phi-1>>;
        std::vector<PiecewisePolynomial> boxes;
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            boxes.push_back(PiecewisePolynomial(p.boxes[i].lo, p.boxes[i].hi, derivative(p.boxes[i].f)));
        }
        return RationalSpline(boxes);
    }

    /*
    `derivative` returns the derivative of a polynomial at a point.
    It is meant to compliment the method signature for integral(p, lo, hi)
    */
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    T derivative(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const T x)
    {
        T dydx(0.0f);
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            dydx += derivative(p.boxes[i], x);
        }
        return dydx;
    }





    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr RationalSpline<T,Plo,Phi,Qlo,Qhi> restriction(const RationalSpline<T,Plo,Phi,Qlo,Qhi>& p, const T lo, const T hi)
    {
        using F = Rational<T,Plo,Phi,Qlo,Qhi>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        const T oo = std::numeric_limits<T>::max();
        for (std::size_t i=0; i<p.boxes.size(); i++)
        {
            if (lo < p.boxes[i].lo || p.boxes[i].hi <= hi)
            {
                if (boxes.size() < 1)
                {
                    boxes.push_back( G(oo, p.boxes[i].lo, F( p.boxes[i].f(p.boxes[i].lo) ) ));
                }
                boxes.push_back(p.boxes[i]);
            }
        }
        boxes.push_back(
            Railcar<T,Rational<T,Plo,Phi,Qlo,Qhi>>( 
                boxes[boxes.size()-1].hi, std::numeric_limits<T>::max(), 
                Rational<T,Plo,Phi,Qlo,Qhi>( boxes[boxes.size()-1].f( boxes[boxes.size()-1].hi ) )
            ));
        return RationalSpline(boxes);

    }



    /*
    `distance` is the distance between the polynomials that are created when cross-multiplying rationals.
    We wish to find a distance metric for the rationals so that we can determine when:
        p/q = r/s
    Or in other words:
        ps  = rq
    The distance metric for polynomials already exists and is defined so that in this case:
        d(ps,rq) = 0
    Whereas the distance metric of the rationals (denoted d') must satisfy:
        d'(p/q,r/s) = 0
    So we equate the two:
        d'(p/q,r/s) = d(ps,rq)
    Under this definition, we see that the distance for rationals inherits 
    most of the properties that are guaranteed by the distance for polynomials:
        d'(p/q,r/s) ∈ ℝ⁺ 
        d'(p/q,r/s) = d'(r/s,p/q)
    Other properties of a metric are harder to demonstrate.
    We'll provide unit tests to see if this definition upholds properties 
    of a metric for a small set of examples, but will forego formal proof.
    */
    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Rlo, int Rhi, int Slo, int Shi>
    constexpr T distance(
        const RationalSpline<T,Plo,Phi,Qlo,Qhi> r, 
        const RationalSpline<T,Rlo,Rhi,Slo,Shi> s, 
        const T lo, 
        const T hi
    ){
        std::vector<T> knots;
        auto rknots = r.knots();
        auto sknots = s.knots();
        knots.insert(knots.end(), rknots.begin(), rknots.end());
        knots.insert(knots.end(), sknots.begin(), sknots.end());
        std::sort(knots.begin(), knots.end());
        auto last = std::unique(knots.begin(), knots.end());
        knots.erase(last, knots.end());

        using F = ArbitraryDegreePolynomial<T,std::int8_t>;
        using G = Railcar<T,F>;
        std::vector<G> boxes;
        for (std::size_t i=1; i<knots.size(); i++)
        {
            F f;
            // add together all boxes that intersect the region from knots[i-1] to knots[i]
            for (std::size_t j=0; j<s.boxes.size(); j++)
            {
                if (std::max(s.boxes[j].lo, knots[i-1]) < std::min(s.boxes[j].hi, knots[i]))
                {
                    f += s.boxes[j].f;
                }
            }
            boxes.push_back(G(knots[i-1], knots[i], f));
        }

        for (auto box = r.boxes.begin(); box != r.boxes.end(); ++box)
        {
            box->
        }
        return distance(r.p*s.q, s.p*r.q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Nlo, int Nhi>
    constexpr T distance(
        const RationalSpline<T,Plo,Phi,Qlo,Qhi> r, 
        const Rational<T,Plo,Phi,Qlo,Qhi> s, 
        const T lo, 
        const T hi
    ){
        return distance(r.p*s.q, s.p*r.q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Nlo, int Nhi>
    constexpr T distance(
        const Rational<T,Plo,Phi,Qlo,Qhi> s, 
        const RationalSpline<T,Plo,Phi,Qlo,Qhi> r, 
        const T lo, 
        const T hi
    ){
        return distance(r.p*s.q, s.p*r.q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Nlo, int Nhi>
    constexpr T distance(
        const RationalSpline<T,Plo,Phi,Qlo,Qhi> r, 
        const Polynomial<T,Nlo,Nhi> p, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, p*r.q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi, int Nlo, int Nhi>
    constexpr T distance(
        const Polynomial<T,Nlo,Nhi> p, 
        const RationalSpline<T,Plo,Phi,Qlo,Qhi> r, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, p*r.q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(
        const RationalSpline<T,Plo,Phi,Qlo,Qhi> r, 
        const T k, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, k*r.q, lo, hi);
    }
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr T distance(
        const T k, 
        const RationalSpline<T,Plo,Phi,Qlo,Qhi> r, 
        const T lo, 
        const T hi
    ){
        return distance(r.p, k*r.q, lo, hi);
    }

}
