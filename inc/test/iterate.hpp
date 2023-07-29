#pragma once

// std libraries
#include <string>

namespace test {

    template<typename Print, typename Test, typename A>
    bool iterate(const std::string& property, const Print& print, const Test& test, const std::vector<A>& a) {
        for (std::size_t i = 0; i < a.size(); ++i) {
            bool pass = test(a[i]);
            if (!pass) {
                std::cout << "Test failed for " << property << ":" << std::endl;
                std::cout << "    a[" << i << "]:" << std::endl << print(a[i]) << std::endl;
                return false; 
            }
        }
        return true;
    }

    template<typename Print, typename Test, typename A, typename B>
    bool iterate(const std::string& property, const Print& print, const Test& test, const std::vector<A>& a, const std::vector<B>& b) {
        for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < b.size(); ++j) {
            bool pass = test(a[i], b[j]);
            if (!pass) {
                std::cout << "Test failed for " << property << ":" << std::endl;
                std::cout << "    a[" << i << "]:" << std::endl << print(a[i]) << std::endl;
                std::cout << "    b[" << j << "]:" << std::endl << print(b[j]) << std::endl;
                return false; 
            }
        }}
        return true;
    }

    template<typename Print, typename Test, typename A, typename B, typename C>
    bool iterate(const std::string& property, const Print& print, const Test& test, const std::vector<A>& a, const std::vector<B>& b, const std::vector<C>& c) {
        for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < b.size(); ++j) {
        for (std::size_t k = 0; k < c.size(); ++k) {
            bool pass = test(a[i], b[j], c[k]);
            if (!pass) {
                std::cout << "Test failed for " << property << ":" << std::endl;
                std::cout << "    a[" << i << "]:" << std::endl << print(a[i]) << std::endl;
                std::cout << "    b[" << j << "]:" << std::endl << print(b[j]) << std::endl;
                std::cout << "    c[" << k << "]:" << std::endl << print(c[k]) << std::endl;
                return false; 
            }
        }}}
        return true;
    }

}

