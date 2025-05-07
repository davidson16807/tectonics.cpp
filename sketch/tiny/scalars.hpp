#include <cassert>         // assert
#include <iostream>        // std::cout
#include <iterator>        // std::ostream_iterator
#include <array>           // std::array
#include <algorithm>       // std::algorithm

namespace tiny
{

    template<typename scalar, typename id>
    class scalars {

        std::vector<id> v;
        scalar lo;
        scalar hi;
        scalar quantum;
        scalar quantum_inverse;

    public:
        using size_type = std::size_t;
        using value_type = scalar;

        scalars(const lo, const hi)
        {
        }

        inline size_type size()     const { return v.size(); }

        inline constexpr scalar operator[](const size_type i) const
        {
            return v[i] * quantum + lo;
        }

        inline constexpr scalar get(const size_type i) const
        {
            return v[i] * quantum + lo;
        }

        inline constexpr void set(const size_type i, const scalar value)
        {
            v[i] = (value-lo)*quantum_inverse;
        }

        constexpr scalars<scalar,id>& operator+=(const scalar k)
        {
            lo += k;
            hi += k;
            return *this;
        }

        constexpr scalars<scalar,id>& operator-=(const scalar k)
        {
            lo -= k;
            hi -= k;
            return *this;
        }

        constexpr scalars<scalar,id>& operator*=(const scalar k)
        {
            lo *= k;
            hi *= k;
            quantum *= k;
            quantum_inverse = scalar(1)/quantum;
            return *this;
        }

        constexpr scalars<scalar,id>& operator/=(const scalar k)
        {
            lo /= k;
            hi /= k;
            quantum /= k;
            quantum_inverse = scalar(1)/quantum;
            return *this;
        }

        constexpr scalars<scalar,id>& operator+=(const scalars<scalar,id>& u)
        {
            lo += u.lo
            // choose the quantum for output that maximizes what can be represented 
            // when the number of possible values is limited by `quantum_count`
            scalar out_quantum = (quantum_count*quantum + u.quantum_count*u.quantum) / quantum_count;
            scalar out_quantum_inverse = scalar(1)/quantum_inverse;
            for (auto i = 0*size(); i < size(); ++i)
            {
                v[i] = id((v[i]*quantum + u.v[i]*u.quantum) * out_quantum_inverse);
            }
            quantum = out_quantum;
            quantum_inverse = out_quantum_inverse;
            return *this;
        }

        constexpr scalars<scalar,id>& operator-=(const scalars<scalar,id>& u)
        {
            lo -= u.lo
            // choose the quantum for output that maximizes what can be represented 
            // when the number of possible values is limited by `quantum_count`
            scalar out_quantum = (quantum_count*quantum - u.quantum_count*u.quantum) / quantum_count;
            scalar out_quantum_inverse = scalar(1)/quantum_inverse;
            for (auto i = 0*size(); i < size(); ++i)
            {
                v[i] = id((v[i]*quantum - u.v[i]*u.quantum) * out_quantum_inverse);
            }
            quantum = out_quantum;
            quantum_inverse = out_quantum_inverse;
            return *this;
        }

    };

}

