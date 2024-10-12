#pragma once

#include <math/analytic/Polynomial.hpp>
#include <math/analytic/Identity.hpp>

#include <index/whole.hpp>
#include <index/procedural/Range.hpp>

namespace procedural {

template<typename T>
struct Adapter{
    T threshold;
    std::size_t test_size;

    Adapter(const T threshold, const std::size_t test_size=30):
        threshold(threshold),
        test_size(test_size)
    {}

    template<typename Series1, typename Series2>
    bool equal(const Series1& a, const Series2& b) const {
        return whole::distance(
            map(a,procedural::range(test_size)),
            map(b,procedural::range(test_size))) <= threshold;
    }

    template<typename Series>
    std::string print(const Series& a) const {
        return whole::to_string(a);
    }

    // the functions below are required for some tests for procedural::Map

    template<int Plo, int Phi>
    std::string print(const analytic::Polynomial<T,Plo,Phi>& p) const {
        return analytic::to_string(p);
    }

    std::string print(const analytic::Identity<T>& e) const {
        return analytic::to_string(e);
    }

};

}

