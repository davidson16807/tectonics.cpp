#pragma once

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

        template<typename T2>
        bool equal(const T2& a, const T2& b) const {
            const T2 denominator(si::abs(a)+si::abs(b));
            return denominator == 0.0*denominator || 2 * si::distance(a, b) / denominator < threshold;
        }
        bool equal(const double a, const double b) const {
            const double denominator(std::abs(a)+std::abs(b));
            return denominator == 0.0*denominator || 2 * std::abs(a-b) / denominator < threshold;
        }

        template<typename T2>
        std::string print(const T2& a) const {
            return si::to_string(a);
        }
        std::string print(const double& a) const {
            return std::to_string(a);
        }

    };
    
}
