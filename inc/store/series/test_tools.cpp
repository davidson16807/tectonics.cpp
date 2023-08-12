#pragma once

#include <math/analytic/Polynomial.hpp>
#include <math/analytic/Identity.hpp>

#include <store/whole.hpp>
#include <store/series/Range.hpp>

namespace series {

template<typename T>
struct SeriesAdapter{
    T threshold;
    std::size_t test_size;

    SeriesAdapter(const T threshold, const std::size_t test_size=30):
        threshold(threshold),
        test_size(test_size)
    {}

    template<typename Series1, typename Series2>
    bool equal(const Series1& a, const Series2& b) const {
        return whole::distance(
            map(a,series::range(test_size)),
            map(b,series::range(test_size))) <= threshold;
    }

    template<typename Series>
    std::string print(const Series& a) const {
        return whole::to_string(a);
    }

    // the functions below are required for some tests for series::Map

    template<int Plo, int Phi>
    std::string print(const math::Polynomial<T,Plo,Phi>& p) const {
        return math::to_string(p);
    }

    std::string print(const math::Identity<T>& e) const {
        return math::to_string(e);
    }

};

}

