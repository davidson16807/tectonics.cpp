#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>

#include <field/SphericalHarmonic.hpp>              // field::SphericalHarmonic

namespace field
{

	/*
	Given a `Series`: ℕ→ℝ, a `VectorZip` maps: ℕ→ℝᴸ.
	*/
    template<typename T, int Lhi>
	class SphericalHarmonics
	{

        constexpr int coefficient_count(const int L) const
        {
            return L*L;
        }

        constexpr int coefficient_id(const int M, const int L) const
        {
            return L*L + M+L;
        }

        template<int M, int L>
        T linear_combination_for_orders(const T z, const T phi) const
        {
            if constexpr (M<-L)
            {
                return T(0);
            }
            else
            {
                return
                    coefficients[coefficient_id(M,L)] * SphericalHarmonic<T,M,L>().call(z, phi) + 
                    linear_combination_for_orders<M-1,L>(z, phi);
            }
        }

        template<int L>
        T linear_combination_for_degrees(const T z, const T phi) const
        {
            if constexpr(L<0)
            {
                return T(0);
            }
            else
            {
                return 
                    linear_combination_for_orders<L,L>(z,phi) + 
                    linear_combination_for_degrees<L-1>(z,phi);
            }
        }

        const std::array<T,Lhi*Lhi> coefficients;

    public:

        using value_type = T;

        SphericalHarmonics(const std::array<T,Lhi*Lhi> coefficients):
            coefficients(coefficients)
        {}

        inline T operator()(const T z, const T phi) const
        {
            return linear_combination_for_degrees<Lhi*Lhi>(z, phi);
        }

        inline T operator()(const T x, const T y, const T z) const
        {
            return linear_combination_for_degrees<Lhi*Lhi>(z, std::atan2(y,x));
        }

        template<int N, glm::qualifier Q=glm::defaultp>
        inline T operator()(const glm::vec<N,T,Q> v) const
        {
            auto u = glm::normalize(v);
            return linear_combination_for_degrees<Lhi*Lhi>(u.z, std::atan2(u.y, u.x));
        }

	};

/*

    template<typename T, int Plo, int Phi>
    constexpr std::string to_string(const SphericalHarmonic<T,Plo,Phi>& p)
    {
        // const std::string exponents("⁰¹²³⁴⁵⁶⁷⁸⁹");
        std::string output;
        for (int i = Plo; i <= Phi; ++i)
        {
            output += std::to_string(p[i]);
            output += i==0?       "" : "x";
            output += 0<=i&&i<=1? "" : "^" + std::to_string(i);
            output += i>=Phi?     "" : " + ";
        }
        return output;
    }

    template<typename T, int Plo, int Phi>
    std::ostream& operator<<(std::ostream& os, const SphericalHarmonic<T,Plo,Phi>& p) { 
        os << to_string(p);
        return os;
    }

    template<typename T, int Plo, int Phi>
    constexpr SphericalHarmonic<T,Plo,Phi> operator*(const SphericalHarmonic<T,Plo,Phi>& p, const T k)
    {
        SphericalHarmonic<T,Plo,Phi> y(p);
        y *= k;
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr SphericalHarmonic<T,Plo,Phi> operator*(const T k, const SphericalHarmonic<T,Plo,Phi>& p)
    {
        SphericalHarmonic<T,Plo,Phi> y(p);
        y *= k;
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr SphericalHarmonic<T,Plo,Phi> operator/(const SphericalHarmonic<T,Plo,Phi>& p, const T k)
    {
        SphericalHarmonic<T,Plo,Phi> y(p);
        y /= k;
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr SphericalHarmonic<T,Plo,Phi> operator-(const SphericalHarmonic<T,Plo,Phi>& p)
    {
        SphericalHarmonic<T,Plo,Phi> y(p);
        y *=-1.0f;
        return y;
    }

    // operators that are not closed under SphericalHarmonic<T,Plo,Phi> relations, but produce SphericalHarmonic<T,> relations of some degree
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr SphericalHarmonic<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> operator+(const SphericalHarmonic<T,Plo,Phi>& p, const SphericalHarmonic<T,P,Q>& q)
    {
        SphericalHarmonic<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y(p);
        y += q;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr SphericalHarmonic<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> operator-(const SphericalHarmonic<T,Plo,Phi>& p, const SphericalHarmonic<T,P,Q>& q)
    {
        SphericalHarmonic<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y(p);
        y -= q;
        return y;
    }

*/


}

