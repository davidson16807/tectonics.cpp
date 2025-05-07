#include <cassert>         // assert
#include <iostream>        // std::cout
#include <iterator>        // std::ostream_iterator
#include <array>           // std::array
#include <algorithm>       // std::algorithm

namespace tiny
{

    template<typename scalar, typename id>
    class logscalars {

        std::vector<id> v;
        scalar lo;
        scalar quantum;
        scalar quantum_inverse;
        
        static constexpr scalar quantum_count = id(std::numeric_limits<id>::max());

    public:
        using size_type = std::size_t;
        using value_type = scalar;

        logscalars(const lo, const hi)
        {
        }

        inline size_type size()     const { return v.size(); }

        inline constexpr scalar operator[](const size_type i) const
        {
            return v[i] * quantum + lo;
        }

        inline constexpr scalar get(const size_type i) const
        {
            return std::exp(scalar(v[i]) * quantum + lo);
        }

        inline constexpr void set(const size_type i, const scalar value)
        {
            v[i] = id((std::log(value) - lo) * quantum_inverse);
        }

        constexpr vector<scalar,id>& operator*=(const scalar k)
        {
            lo += k;
            return *this;
        }

        constexpr vector<scalar,id>& operator/=(const scalar k)
        {
            lo -= k;
            return *this;
        }

        // DIFFERENT REPRESENTATIONS FOR FUTURE REFERENCE:

        // constexpr vector<scalar,id>& operator/=(const vector<scalar,id>& u)
        // {
        //     lo += u.lo;
        //     for (auto i = 0*size(); i < size(); ++i)
        //     {
        //         v[i] = id((std::log(
        //             std::exp(v[i] * quantum + lo - (u.v[i] * u.quantum + u.lo))
        //         ) - lo) * quantum_inverse);
        //     }
        //     return *this;
        // }

        // constexpr vector<scalar,id>& operator/=(const vector<scalar,id>& u)
        // {
        //     for (auto i = 0*size(); i < size(); ++i)
        //     {
        //         scalar(out.v[i]) * out.quantum + out.lo = 
        //             v[i] * quantum + lo - (u.v[i] * u.quantum + u.lo)
        //         ;
        //     }
        //     return *this;
        // }

        // constexpr vector<scalar,id>& operator/=(const vector<scalar,id>& u)
        // {
        //     lo -= u.lo
        //     for (auto i = 0*size(); i < size(); ++i)
        //     {
        //         scalar(out.v[i]) * out.quantum = v[i]*quantum - u.v[i]*u.quantum
        //             ;
        //     }
        //     return *this;
        // }

        constexpr vector<scalar,id>& operator*=(const vector<scalar,id>& u)
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

        constexpr vector<scalar,id>& operator/=(const vector<scalar,id>& u)
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

