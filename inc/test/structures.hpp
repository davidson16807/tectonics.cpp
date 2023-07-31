#pragma once

// std libraries

// in-house libraries
#include "properties.hpp"

namespace test {

    /*
    Unlike tests for properties, tests for structure are bulky and are expected to be called 
    multiple times on multiple kinds of input (See math/analytic/Polynomial_test.cpp for an example).
    to save us trouble, we implement classes that store parameters that are likely not to change.
    */

    template<typename E, typename F>
    struct Monoid{
        const std::string e_name; const E& e; 
        const std::string f_name; const F& f; 
        Monoid(
            const std::string e_name, const E& e, 
            const std::string f_name, const F& f
        ): 
            e_name(e_name), e(e),
            f_name(f_name), f(f)
        {}
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            associativity(adapter,            f_name, f, as, bs, cs) &&
            identity     (adapter, e_name, e, f_name, f, as)         &&

            true; // added so lines above can be easily swapped
        }
    };

    template<typename E, typename Finv, typename F>
    struct Group{
        const std::string e_name;    const E e; 
        const std::string f_name;    const F f; 
        const std::string finv_name; const Finv finv; 
        Group(
            const std::string e_name,    const E& e, 
            const std::string f_name,    const F& f,
            const std::string finv_name, const Finv& finv
        ): 
            e_name(e_name),       e(e),
            f_name(f_name),       f(f),
            finv_name(finv_name), finv(finv)
        {}
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            associativity(adapter,                             f_name, f, as, bs, cs) &&
            identity     (adapter, e_name, e,                  f_name, f, as)         &&
            invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as)         &&

            true; // added so lines above can be easily swapped
        }
    };

    /*
    NOTE: in a commutative structure, arguments can be swapped, 
    so if multiple data types are provided as test data, 
    these data types can be used by tests in any combination,
    which vastly increases the number of tests we could (and should) perform.
    This is why we implement specialized functions to test for commutative structures.
    However we want to be able to call test functions multiple times on different inputs
    without unecessarily repeating tests, and we also want to reduce the size of each test function's implementation,
    and we've found the best way to do this is to have each method 
    only consider those tests that make use of all their arguments
    */

    template<typename E, typename F>
    struct CommutativeMonoid{
        const std::string e_name; const E& e; 
        const std::string f_name; const F& f; 
        CommutativeMonoid(
            const std::string e_name, const E& e, 
            const std::string f_name, const F& f
        ): 
            e_name(e_name), e(e),
            f_name(f_name), f(f)
        {}
        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            determinism  (adapter,            f_name, f, as, as)     &&
            associativity(adapter,            f_name, f, as, as, as) &&
            identity     (adapter, e_name, e, f_name, f, as)         &&
            commutativity(adapter,            f_name, f, as, as)     &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            determinism  (adapter, f_name, f, as, bs)     &&
            determinism  (adapter, f_name, f, bs, as)     &&
            associativity(adapter, f_name, f, as, as, bs) &&
            associativity(adapter, f_name, f, as, bs, as) &&
            associativity(adapter, f_name, f, as, bs, bs) &&
            associativity(adapter, f_name, f, bs, as, as) &&
            associativity(adapter, f_name, f, bs, as, bs) &&
            associativity(adapter, f_name, f, bs, bs, as) &&
            commutativity(adapter, f_name, f, as, bs)     &&
            commutativity(adapter, f_name, f, bs, as)     &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            associativity(adapter, f_name, f, as, bs, cs) &&
            associativity(adapter, f_name, f, as, cs, bs) &&
            associativity(adapter, f_name, f, bs, as, cs) &&
            associativity(adapter, f_name, f, bs, cs, as) &&
            associativity(adapter, f_name, f, cs, as, bs) &&
            associativity(adapter, f_name, f, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
    };

    template<typename E, typename F, typename Finv>
    struct CommutativeGroup{
        const std::string e_name;    const E e; 
        const std::string f_name;    const F f; 
        const std::string finv_name; const Finv finv; 
        CommutativeGroup(
            const std::string e_name,    const E& e, 
            const std::string f_name,    const F& f,
            const std::string finv_name, const Finv& finv
        ): 
            e_name(e_name),       e(e),
            f_name(f_name),       f(f),
            finv_name(finv_name), finv(finv)
        {}
        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            determinism  (adapter,            f_name, f, as, as)     &&
            associativity(adapter,            f_name, f, as, as, as) &&
            identity     (adapter, e_name, e, f_name, f, as)         &&
            commutativity(adapter,            f_name, f, as, as)     &&
            invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            determinism  (adapter, f_name, f, as, bs)     &&
            determinism  (adapter, f_name, f, bs, as)     &&
            associativity(adapter, f_name, f, as, as, bs) &&
            associativity(adapter, f_name, f, as, bs, as) &&
            associativity(adapter, f_name, f, as, bs, bs) &&
            associativity(adapter, f_name, f, bs, as, as) &&
            associativity(adapter, f_name, f, bs, as, bs) &&
            associativity(adapter, f_name, f, bs, bs, as) &&
            commutativity(adapter, f_name, f, as, bs)     &&
            commutativity(adapter, f_name, f, bs, as)     &&
            invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as) &&
            invertibility(adapter, e_name, e, finv_name, finv, f_name, f, bs) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            associativity(adapter, f_name, f, as, bs, cs) &&
            associativity(adapter, f_name, f, as, cs, bs) &&
            associativity(adapter, f_name, f, bs, as, cs) &&
            associativity(adapter, f_name, f, bs, cs, as) &&
            associativity(adapter, f_name, f, cs, as, bs) &&
            associativity(adapter, f_name, f, cs, bs, as) &&

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
            distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

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
            distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
    };

}

