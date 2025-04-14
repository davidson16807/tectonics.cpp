#pragma once

#include <math/analytic/Polynomial.hpp>
#include <math/analytic/Identity.hpp>

#include <index/procedural/Range.hpp>
#include <index/adapted/scalar/ScalarStrings.hpp>
#include <index/adapted/scalar/ScalarMetric.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/aggregated/Order.hpp>
#include <index/aggregated/Strings.hpp>
#include <index/aggregated/Metric.hpp>

namespace procedural {

template<typename T>
struct Adapter{
    T threshold;
    std::size_t test_size;
    aggregated::Strings<adapted::ScalarStrings<T>, aggregated::Order<adapted::SymbolicOrder>> strings;
    aggregated::Metric<adapted::ScalarMetric<T>> metric;

    Adapter(const T threshold, const std::size_t test_size=30):
        threshold(threshold),
        test_size(test_size),
        strings(adapted::ScalarStrings<T>(), 
            aggregated::Order<adapted::SymbolicOrder>(
                adapted::SymbolicOrder())),
        metric(adapted::ScalarMetric<T>())
    {}

    template<typename Series1, typename Series2>
    bool equal(const Series1& a, const Series2& b) const {
        return metric.distance(
            map(a,procedural::range(test_size)),
            map(b,procedural::range(test_size))) <= threshold;
    }

    template<typename Series>
    std::string print(const Series& a) const {
        return strings.format(a);
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

