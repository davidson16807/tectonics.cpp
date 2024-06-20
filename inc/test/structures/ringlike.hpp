#pragma once

// std libraries

// in-house libraries
#include "grouplike.hpp"

namespace test {

    /*
    NOTE1: Unlike tests for properties, tests for structure are bulky and are expected to be called 
    multiple times on multiple kinds of input (See math/analytic/Polynomial_test.cpp for an example).
    to save us trouble, we implement classes that store parameters that are likely not to change.
    */

    template<typename Zero, typename One, typename Add, typename Mult>
    struct CommutativeSemiRing{
        const CommutativeMonoid  <Zero, Add>  addition;
        const CommutativeMonoid <One,  Mult>  multiplication;
        const std::string zero_name; const Zero zero;   
        const std::string one_name;  const One  one;   
        const std::string add_name;  const Add  add;   
        const std::string mult_name; const Mult mult;
        CommutativeSemiRing(
            const std::string zero_name, const Zero& zero, 
            const std::string one_name,  const One&  one, 
            const std::string add_name,  const Add&  add, 
            const std::string mult_name, const Mult& mult
        ): 
            addition      (zero_name, zero, add_name,  add),
            multiplication(one_name,  one,  mult_name, mult),
            zero_name (zero_name), zero (zero), 
            one_name  (one_name),  one  (one), 
            add_name  (add_name),  add  (add), 
            mult_name (mult_name), mult (mult)
        {}
        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            addition      .valid(adapter, as) &&
            multiplication.valid(adapter, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
    };

    template<typename Zero, typename One, typename Add, typename Sub, typename Mult>
    struct CommutativeRing{
        const CommutativeGroup  <Zero, Add, Sub>  addition;
        const CommutativeMonoid <One,  Mult>      multiplication;
        const std::string zero_name; const Zero zero;   
        const std::string one_name;  const One  one;   
        const std::string add_name;  const Add  add;   
        const std::string sub_name;  const Sub  sub;   
        const std::string mult_name; const Mult mult;
        CommutativeRing(
            const std::string zero_name, const Zero& zero, 
            const std::string one_name,  const One&  one, 
            const std::string add_name,  const Add&  add, 
            const std::string sub_name,  const Sub&  sub, 
            const std::string mult_name, const Mult& mult
        ): 
            addition      (zero_name, zero, add_name,  add, sub_name,  sub),
            multiplication(one_name,  one,  mult_name, mult),
            zero_name (zero_name), zero (zero), 
            one_name  (one_name),  one  (one), 
            add_name  (add_name),  add  (add), 
            sub_name  (sub_name),  sub  (sub), 
            mult_name (mult_name), mult (mult)
        {}
        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            addition      .valid(adapter, as) &&
            multiplication.valid(adapter, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
    };


    template<typename Zero, typename One, typename Add, typename Sub, typename Mult, typename Div>
    struct Field{
        const CommutativeGroup  <Zero, Add, Sub>  addition;
        const CommutativeGroup  <One,  Mult,Div>  multiplication;
        const std::string zero_name; const Zero zero;   
        const std::string one_name;  const One  one;   
        const std::string add_name;  const Add  add;   
        const std::string sub_name;  const Sub  sub;   
        const std::string mult_name; const Mult mult;
        const std::string div_name;  const Div  div;
        Field(
            const std::string zero_name, const Zero& zero, 
            const std::string one_name,  const One&  one, 
            const std::string add_name,  const Add&  add, 
            const std::string sub_name,  const Sub&  sub, 
            const std::string mult_name, const Mult& mult,
            const std::string div_name,  const Div&  div
        ): 
            addition      (zero_name, zero, add_name,  add, sub_name, sub),
            multiplication(one_name,  one,  mult_name, mult,div_name, div),
            zero_name (zero_name), zero (zero), 
            one_name  (one_name),  one  (one), 
            add_name  (add_name),  add  (add), 
            sub_name  (sub_name),  sub  (sub), 
            mult_name (mult_name), mult (mult),
            div_name  (div_name),  div  (div)
        {}
        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            addition      .valid(adapter, as) &&
            multiplication.valid(adapter, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
    };


}

