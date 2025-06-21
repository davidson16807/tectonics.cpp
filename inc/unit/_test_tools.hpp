#pragma once

#include <unit/si.hpp>

namespace si
{

    /*
    `UnitAdapter` leverages standardized function names
    to compare and display its variables
    */
    template<typename T>
    struct UnitAdapter{
        const T threshold;

        UnitAdapter(T threshold):
            threshold(threshold)
        {}

        template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1>
        bool equal(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T>& a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T>& b) const {
            const units<M1,KG1,S1,K1,MOL1,A1,CD1,T> denominator(si::abs(a)+si::abs(b));
            return denominator == 0.0*denominator || si::distance(a, b) / denominator < threshold;
        }
        bool equal(const T a, const T b) const {
            const T denominator(std::abs(a)+std::abs(b));
            return denominator == 0.0*denominator || 2 * std::abs(a-b) / denominator < threshold;
        }

        template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1>
        std::string print(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T>& a) const {
            return si::to_string(a);
        }
        std::string print(const T& a) const {
            return std::to_string(a);
        }

    };

    /*
    `UnitAdapter` leverages standardized function names to compare and display its variables
    Equality is determined by whether distance exceeds a absolute threshold.
    */
    template<typename T>
    struct AbsoluteUnitAdapter{
        const T threshold;

        AbsoluteUnitAdapter(T threshold):
            threshold(threshold)
        {}

        bool equal(const T& a, const T& b) const {
            return si::distance(a, b) < threshold;
        }

        template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1, typename T1>
        std::string print(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>& a) const {
            return si::to_string(a);
        }

    };
    
}
