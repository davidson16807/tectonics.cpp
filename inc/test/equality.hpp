#pragma once

// std libraries
#include <iostream>
#include <string>   // std::string
#include <regex>

// in-house libraries
#include "predicate.hpp"

namespace test {

    template<typename Adapter, typename LHS, typename RHS, typename A>
    bool equality( const Adapter& adapter, const std::string& property, 
                const std::string& lhs_name, const LHS& lhs, 
                const std::string& rhs_name, const RHS& rhs, 
                const std::vector<A>& as) {
        return predicate(adapter, property, 
            [=](auto a){
                auto lhs_value = lhs(a);
                auto rhs_value = rhs(a);
                return Results(adapter.equal(lhs_value, rhs_value),
                    "such that: \n  " + lhs_name + " = " + rhs_name + "\n" + 
                    lhs_name+" : " + indent(adapter.print(lhs_value), "  ")+"\n" +
                    rhs_name+" : " + indent(adapter.print(rhs_value), "  "));
            }, as);
    }

    template<typename Adapter, typename LHS, typename RHS, typename A, typename B>
    bool equality( const Adapter& adapter, const std::string& property, 
                const std::string& lhs_name, const LHS& lhs, 
                const std::string& rhs_name, const RHS& rhs, 
                const std::vector<A>& as, const std::vector<B>& bs) {
        return predicate(adapter, property, 
            [=](auto a, auto b){
                auto lhs_value = lhs(a, b);
                auto rhs_value = rhs(a, b);
                return Results(adapter.equal(lhs_value, rhs_value),
                    "such that: \n  " + lhs_name + " = " + rhs_name + "\n" + 
                    lhs_name+" : " + indent(adapter.print(lhs_value), "  ")+"\n" +
                    rhs_name+" : " + indent(adapter.print(rhs_value), "  "));
            }, as, bs);
    }

    template<typename Adapter, typename LHS, typename RHS, typename A, typename B, typename C>
    bool equality( const Adapter& adapter, const std::string& property, 
                const std::string& lhs_name, const LHS& lhs, 
                const std::string& rhs_name, const RHS& rhs, 
                const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return predicate(adapter, property, 
            [=](auto a, auto b, auto c){
                auto lhs_value = lhs(a, b, c);
                auto rhs_value = rhs(a, b, c);
                return Results(adapter.equal(lhs_value, rhs_value),
                    "such that: \n  " + lhs_name + " = " + rhs_name + "\n" + 
                    lhs_name+" : " + indent(adapter.print(lhs_value), "  ")+"\n" +
                    rhs_name+" : " + indent(adapter.print(rhs_value), "  "));
            }, as, bs, cs);
    }

}
