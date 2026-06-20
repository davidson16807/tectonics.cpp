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

        template <
            int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1,
            int M2, int KG2, int S2, int K2, int MOL2, int A2, int CD2
        >
        /*
        Units behaves as a mathematical field but with one subtlety:
        in a field, there is an "annihilation" property where additive identity, "0",
        can be multiplied by anything to produce itself.
        In dimensional quantities additive identity has a dimension like "0 meters",
        however we see e.g. 0 meters * 1 meters = 0 meter² which is of different dimension from 0 meters.
        This is not a problem mathematically since "meter" behaves like a variable,
        which goes away with multiplication by 0 so that 0 meters = 0.
        However standard tests in test::Field will fail at compile time if something isn't done.
        We do not want to edit si:: to weaken its compile time guarantees,
        so the best solution we have found is to allow this `equal` method 
        to accept quantities of different dimension,
        and to consider two quantities of different dimension equal if both are 0.
        */
        bool equal(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T>& a, const units<M2,KG2,S2,K2,MOL2,A2,CD2,T>& b) const {
            using A = units<M1,KG1,S1,K1,MOL1,A1,CD1,T>;
            using B = units<M2,KG2,S2,K2,MOL2,A2,CD2,T>;
            const T denominator(std::abs(a/A(1))+std::abs(b/B(1)));
            if (denominator < threshold){
                return true;
            }
            if (M1!=M2||KG1!=KG2||S1!=S2||K1!=K2||MOL1!=MOL2||A1!=A2||CD1!=CD2) {
                return false;
            }
            return std::abs(a/A(1)-b/B(1)) / denominator < threshold;
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
