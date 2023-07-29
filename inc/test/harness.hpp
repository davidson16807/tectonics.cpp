#pragma once

// std libraries
#include <iostream>
#include <sstream>
#include <string>

namespace test {

    template<typename Equal, typename Print>
    struct WrapperHarness{
        const Equal equal;
        const Print print;

        WrapperHarness(const Equal& equal, const Print& print):
            equal(equal),
            print(print)
        {

        }

    };

    struct OperatorHarness{

        OperatorHarness()
        {
        }

        template<typename T>
        bool equal(const T& a, const T& b) const {
            return a == b;
        }

        template<typename T>
        std::string print(const T& a) const {
            std::ostringstream os;
            os << a << std::endl;
            return os.str();
        }

    };

    template<typename T>
    struct FunctionHarness{
        const T threshold;

        FunctionHarness(T threshold)
        {
        }

        bool equal(const T& a, const T& b) const {
            return distance(a, b) < threshold;
        }

        std::string print(const T& a) const {
            return to_string(a);
        }

    };

}

