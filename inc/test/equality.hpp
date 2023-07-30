#pragma once

// std libraries
#include <iostream>
#include <string>   // std::string
#include <regex>

namespace test {

    template<typename Harness, typename LHS, typename RHS, typename A>
    bool equality( const Harness& harness, const std::string& property, 
                const std::string& lhs_name, const LHS& lhs, 
                const std::string& rhs_name, const RHS& rhs, 
                const std::vector<A>& a) {
        for (std::size_t i = 0; i < a.size(); ++i) {
            auto lhs_value = lhs(a[i]);
            auto rhs_value = rhs(a[i]);
            bool pass = harness.equal(lhs_value, rhs_value);
            if (!pass) {
                std::cout << std::endl;
                std::cout << "Test failed: " << std::endl << std::endl;
                std::cout << "  " << property << std::endl;
                std::cout << "  such that:" << std::endl << std::endl;
                std::cout << "    " << std::regex_replace(lhs_name, std::regex(" +$"), "") << " = " << rhs_name << std::endl << std::endl;
                std::cout << "  " << lhs_name << " :  " << harness.print(lhs_value) << std::endl;
                std::cout << "  " << rhs_name << " :  " << harness.print(rhs_value) << std::endl;
                std::cout << "  a :  " << harness.print(a[i]) << " [from index "<< i <<"]" << std::endl;
                return false; 
            }
        }
        return true;
    }

    template<typename Harness, typename LHS, typename RHS, typename A, typename B>
    bool equality( const Harness& harness, const std::string& property, 
                const std::string& lhs_name, const LHS& lhs, 
                const std::string& rhs_name, const RHS& rhs, 
                const std::vector<A>& a, const std::vector<B>& b) {
        for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < b.size(); ++j) {
            auto lhs_value = lhs(a[i], b[j]);
            auto rhs_value = rhs(a[i], b[j]);
            bool pass = harness.equal(lhs_value, rhs_value);
            if (!pass) {
                std::cout << std::endl;
                std::cout << "Test failed: " << std::endl << std::endl;
                std::cout << "  " << property << std::endl;
                std::cout << "  such that:" << std::endl << std::endl;
                std::cout << "    " << std::regex_replace(lhs_name, std::regex(" +$"), "") << " = " << rhs_name << std::endl << std::endl;
                std::cout << "  " << lhs_name << " :  " << harness.print(lhs_value) << std::endl;
                std::cout << "  " << rhs_name << " :  " << harness.print(rhs_value) << std::endl;
                std::cout << "  a :  " << harness.print(a[i]) << " [from index "<< i <<"]" << std::endl;
                std::cout << "  b :  " << harness.print(b[j]) << " [from index "<< j <<"]" << std::endl;
                return false; 
            }
        }}
        return true;
    }

    template<typename Harness, typename LHS, typename RHS, typename A, typename B, typename C>
    bool equality( const Harness& harness, const std::string& property, 
                const std::string& lhs_name, const LHS& lhs, 
                const std::string& rhs_name, const RHS& rhs, 
                const std::vector<A>& a, const std::vector<B>& b, const std::vector<C>& c) {
        for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < b.size(); ++j) {
        for (std::size_t k = 0; k < c.size(); ++k) {
            auto lhs_value = lhs(a[i], b[j], c[k]);
            auto rhs_value = rhs(a[i], b[j], c[k]);
            bool pass = harness.equal(lhs_value, rhs_value);
            if (!pass) {
                std::cout << std::endl;
                std::cout << "Test failed: " << std::endl;
                std::cout << "  " << property << std::endl;
                std::cout << "  such that:" << std::regex_replace(lhs_name, std::regex(" +$"), "") << " = " << rhs_name << std::endl;
                std::cout << "  " << lhs_name << " :  " << harness.print(lhs_value) << std::endl;
                std::cout << "  " << rhs_name << " :  " << harness.print(rhs_value) << std::endl;
                std::cout << "  a :  " << harness.print(a[i]) << " [from index "<< i <<"]" << std::endl;
                std::cout << "  b :  " << harness.print(b[j]) << " [from index "<< j <<"]" << std::endl;
                std::cout << "  c :  " << harness.print(c[k]) << " [from index "<< k <<"]" << std::endl;
                return false; 
            }
        }}}
        return true;
    }







}

