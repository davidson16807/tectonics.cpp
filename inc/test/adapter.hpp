#pragma once

// std libraries
#include <sstream>
#include <string>

/*
A `test::*Adapter` is a class that standardizes a minimal set of function signatures to permit testing.
Many functions must satisfy other concerns that affect their signature, such as performance.
Unit tests require much fewer concerns to be satisfied, 
so such functions could be adapted if needed to another signature that is easier to work with when testing.
However it is important that such adapter logic be clearly separated from production code, 
and this is what `test::*Adapter` provide.

It is important to note that the list of `*Adapter` classes here is not comprehensive.
The definition of `*Adapter` classes is kept deliberately simple to make it easy for 
developers to define their own `*Adapter` classes to suite their needs.
The `*Adapter` classes here are provided only as a convenience to suite the most common use cases.
*/

namespace test {

    /*
    `CustomAdapter` uses custom lambdas 
    in order to compare and display variables.
    */
    template<typename Equal, typename Print>
    struct CustomAdapter{
        const Equal equal;
        const Print print;

        CustomAdapter(const Equal& equal, const Print& print):
            equal(equal),
            print(print)
        {}

    };

    /*
    `OperatorAdapter` leverages operators 
    to compare and display its variables
    */
    struct OperatorAdapter{

        OperatorAdapter() {}

        template<typename T>
        bool equal(const T& a, const T& b) const {
            return a == b;
        }

        template<typename T>
        std::string print(const T& a) const {
            std::ostringstream os;
            os << a;
            return os.str();
        }

    };

    /*
    `StandardAdapter` leverages standardized function names
    to compare and display its variables
    */
    template<typename T>
    struct StandardAdapter{
        const T threshold;

        StandardAdapter(T threshold):
            threshold(threshold)
        {}

        bool equal(const T& a, const T& b) const {
            return distance(a, b) <= threshold;
        }

        std::string print(const T& a) const {
            return to_string(a);
        }

    };

    /*
    `PrimitiveAdapter` uses `std::` function names
    to compare and display primitive scalar data types 
    for which `std::` provides no other `distance` function.
    Such data types include float and double
    */
    template<typename T>
    struct PrimitiveAdapter{
        const T threshold;

        PrimitiveAdapter(T threshold):
            threshold(threshold)
        {}

        bool equal(const T& a, const T& b) const {
            return abs(a - b) <= threshold;
        }

        std::string print(const T& a) const {
            std::ostringstream os;
            os << a;
            return os.str();
        }

    };

}

