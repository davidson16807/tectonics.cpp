#pragma once

// std libraries
#include <iostream>
#include <string>

namespace test {

    template<typename Harness, typename Predicate, typename A>
    bool predicate(const Harness& harness, const std::string& property, const Predicate& predicate, const std::vector<A>& a) {
        for (std::size_t i = 0; i < a.size(); ++i) {
            bool pass = predicate(a[i]);
            if (!pass) {
                std::cout << std::endl;
                std::cout << "Test failed for " << property << ":" << std::endl;
                std::cout << "    a[" << i << "]:  " << harness.print(a[i]) << std::endl;
                return false; 
            }
        }
        return true;
    }

    template<typename Harness, typename Predicate, typename A, typename B>
    bool predicate(const Harness& harness, const std::string& property, const Predicate& predicate, const std::vector<A>& a, const std::vector<B>& b) {
        for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < b.size(); ++j) {
            bool pass = predicate(a[i], b[j]);
            if (!pass) {
                std::cout << std::endl;
                std::cout << "Test failed for " << property << ":" << std::endl;
                std::cout << "    a[" << i << "]:  " << harness.print(a[i]) << std::endl;
                std::cout << "    b[" << j << "]:  " << harness.print(b[j]) << std::endl;
                return false; 
            }
        }}
        return true;
    }

    template<typename Harness, typename Predicate, typename A, typename B, typename C>
    bool predicate(const Harness& harness, const std::string& property, const Predicate& predicate, const std::vector<A>& a, const std::vector<B>& b, const std::vector<C>& c) {
        for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < b.size(); ++j) {
        for (std::size_t k = 0; k < c.size(); ++k) {
            bool pass = predicate(a[i], b[j], c[k]);
            if (!pass) {
                std::cout << std::endl;
                std::cout << "Test failed for " << property << ":" << std::endl;
                std::cout << "    a[" << i << "]:  " << harness.print(a[i]) << std::endl;
                std::cout << "    b[" << j << "]:  " << harness.print(b[j]) << std::endl;
                std::cout << "    c[" << k << "]:  " << harness.print(c[k]) << std::endl;
                return false; 
            }
        }}}
        return true;
    }

}

