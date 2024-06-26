#pragma once

// std libraries
#include <iostream>
#include <string>   // std::string
#include <regex>

// in-house libraries
#include "predicate.hpp"

namespace test {

    std::string trim(const std::string text)
    {
        return std::regex_replace(text, std::regex("  +"), "");
    }

    std::string indent(const std::string text, const std::string tab)
    {
        return std::regex_replace(
            (std::regex_search(text, std::regex("\n"))? "\n":"") + text, 
            std::regex("\n"), 
            "\n" +tab);
    }

    struct Results{
        const bool pass;
        const std::string diagnostics;
        Results(
            const bool pass, 
            const std::string diagnostics):
            pass(pass),
            diagnostics(diagnostics)
        {}
    };

    template<typename Adapter, typename Predicate, typename A>
    bool predicate(const Adapter& adapter, 
            const std::string& predicate_name, const Predicate& predicate, 
            const A& a) {
        using size_type = typename A::size_type;
        for (size_type i = 0; i < a.size(); ++i) {
            Results results = predicate(a[i]);
            if (!results.pass) {
                std::cout << std::endl;
                std::cout << "Test failed:" << std::endl;
                std::cout << "  " << predicate_name << std::endl;
                std::cout <<  indent(results.diagnostics, "  ") << std::endl;
                std::cout << "  a : " << indent(adapter.print(a[i]), "    ") << " [from index "<< i <<"]" << std::endl;
                return false; 
            }
        }
        return true;
    }

    template<typename Adapter, typename Predicate, typename A, typename B>
    bool predicate(const Adapter& adapter, 
            const std::string& predicate_name, const Predicate& predicate, 
            const A& a, const B& b) {
        using size_type = typename A::size_type;
        for (size_type i = 0; i < a.size(); ++i) {
        for (size_type j = 0; j < b.size(); ++j) {
            Results results = predicate(a[i], b[j]);
            if (!results.pass) {
                std::cout << std::endl;
                std::cout << "Test failed:" << std::endl;
                std::cout << "  " << predicate_name << std::endl;
                std::cout <<  indent(results.diagnostics, "  ") << std::endl;
                std::cout << "  a : " << indent(adapter.print(a[i]), "    ") << " [from index "<< i <<"]" << std::endl;
                std::cout << "  b : " << indent(adapter.print(b[j]), "    ") << " [from index "<< j <<"]" << std::endl;
                return false; 
            }
        }}
        return true;
    }

    template<typename Adapter, typename Predicate, typename A, typename B, typename C>
    bool predicate(const Adapter& adapter, 
            const std::string& predicate_name, const Predicate& predicate, 
            const A& a, const B& b, const C& c) {
        using size_type = typename A::size_type;
        for (size_type i = 0; i < a.size(); ++i) {
        for (size_type j = 0; j < b.size(); ++j) {
        for (size_type k = 0; k < c.size(); ++k) {
            Results results = predicate(a[i], b[j], c[k]);
            if (!results.pass) {
                std::cout << std::endl;
                std::cout << "Test failed:" << std::endl;
                std::cout << "  " << predicate_name << std::endl;
                std::cout <<  indent(results.diagnostics, "  ") << std::endl;
                std::cout << "  a : " << indent(adapter.print(a[i]), "    ") << " [from index "<< i <<"]" << std::endl;
                std::cout << "  b : " << indent(adapter.print(b[j]), "    ") << " [from index "<< j <<"]" << std::endl;
                std::cout << "  c : " << indent(adapter.print(c[k]), "    ") << " [from index "<< k <<"]" << std::endl;
                return false; 
            }
        }}}
        return true;
    }

}
