#pragma once

// std libraries

// in-house libraries
#include "properties.hpp"

namespace test {

    template<typename Harness, typename E, typename F, typename A, typename B, typename C>
    bool monoid(const Harness& harness, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return 

            associativity(harness,            f_name, f, as, bs, cs) &&
            identity     (harness, e_name, e, f_name, f, as)         &&

        true; // added so lines above can be easily swapped
    }

    template<typename Harness, typename E, typename Finv, typename F, typename A, typename B, typename C>
    bool group(const Harness& harness, 
        const std::string e_name,    const E& e, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return 

            associativity(harness,                             f_name, f, as, bs, cs) &&
            identity     (harness, e_name, e,                  f_name, f, as)         &&
            invertibility(harness, e_name, e, finv_name, finv, f_name, f, as)         &&

        true; // added so lines above can be easily swapped
    }

    /*
    NOTE: in a commutative structure, arguments can be swapped, 
    so if multiple data types are provided as test data, 
    these data types can be used by tests in any combination,
    which vastly increases the number of tests we could (and should) perform.
    This is why we implement specialized functions to test for commutative structures.
    However we want to be able to call multiple test functions without unecessarily repeating tests,
    and we also want to reduce the size of each test function's implementation,
    so therefore each function only considers those tests that make use of all their arguments
    */

    template<typename Harness, typename E, typename F, typename A>
    bool commutative_monoid(const Harness& harness, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return 

            determinism  (harness,            f_name, f, as, as)     &&
            associativity(harness,            f_name, f, as, as, as) &&
            identity     (harness, e_name, e, f_name, f, as)         &&
            commutativity(harness,            f_name, f, as, as)     &&

        true; // added so lines above can be easily swapped
    }

    template<typename Harness, typename E, typename F, typename A, typename B>
    bool commutative_monoid(const Harness& harness, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return 

            determinism  (harness,            f_name, f, as, bs)     &&
            determinism  (harness,            f_name, f, bs, as)     &&
            associativity(harness,            f_name, f, as, as, bs) &&
            associativity(harness,            f_name, f, as, bs, as) &&
            associativity(harness,            f_name, f, as, bs, bs) &&
            associativity(harness,            f_name, f, bs, as, as) &&
            associativity(harness,            f_name, f, bs, as, bs) &&
            associativity(harness,            f_name, f, bs, bs, as) &&
            commutativity(harness,            f_name, f, as, bs)     &&
            commutativity(harness,            f_name, f, bs, as)     &&

        true; // added so lines above can be easily swapped
    }

    template<typename Harness, typename E, typename F, typename A, typename B, typename C>
    bool commutative_monoid(const Harness& harness, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return 

            associativity(harness,            f_name, f, as, bs, cs) &&
            associativity(harness,            f_name, f, as, cs, bs) &&
            associativity(harness,            f_name, f, bs, as, cs) &&
            associativity(harness,            f_name, f, bs, cs, as) &&
            associativity(harness,            f_name, f, cs, as, bs) &&
            associativity(harness,            f_name, f, cs, bs, as) &&

        true; // added so lines above can be easily swapped
    }

    template<typename Harness, typename E, typename Finv, typename F, typename A>
    bool commutative_group(const Harness& harness, 
        const std::string e_name,    const E& e, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const many<A>& as
    ) {
        return 
            commutative_monoid(harness, e_name, e, f_name, f, as)             &&
            invertibility(harness, e_name, e, finv_name, finv, f_name, f, as) &&
        true; // added so lines above can be easily swapped
    }

    template<typename Harness, typename E, typename Finv, typename F, typename A, typename B>
    bool commutative_group(const Harness& harness, 
        const std::string e_name,    const E& e, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return 
            commutative_monoid(harness, e_name, e, f_name, f, as, bs)         &&
            invertibility(harness, e_name, e, finv_name, finv, f_name, f, as) &&
            invertibility(harness, e_name, e, finv_name, finv, f_name, f, bs) &&
        true; // added so lines above can be easily swapped
    }

    template<typename Harness, typename E, typename Finv, typename F, typename A, typename B, typename C>
    bool commutative_group(const Harness& harness, 
        const std::string e_name,    const E& e, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return 
            commutative_monoid(harness, e_name, e, f_name, f, as, bs, cs)     &&
        true; // added so lines above can be easily swapped
    }

    template<typename Harness, 
        typename Zero, typename One, 
        typename Add, typename Sub, typename Mult, 
        typename A>
    bool commutative_ring(const Harness& harness, 
        const std::string zero_name, const Zero& zero, 
        const std::string one_name,  const One& one, 
        const std::string add_name,  const Add& add, 
        const std::string sub_name,  const Sub& sub, 
        const std::string mult_name, const Mult& mult, 
        const many<A>& as
    ) {
        return 

            commutative_group(harness,
                zero_name,    zero,
                sub_name,     sub,
                add_name,     add,
                as) &&

            commutative_monoid(harness,
                one_name,  one, 
                mult_name, mult,
                as) &&

            distributivity(harness, mult_name, mult, add_name, add, as, as, as) &&

        true; // added so lines above can be easily swapped
    }

    template<typename Harness, 
        typename Zero, typename One, 
        typename Add, typename Sub, typename Mult, 
        typename A, typename B>
    bool commutative_ring(const Harness& harness, 
        const std::string zero_name, const Zero& zero, 
        const std::string one_name,  const One& one, 
        const std::string add_name,  const Add& add, 
        const std::string sub_name,  const Sub& sub, 
        const std::string mult_name, const Mult& mult, 
        const many<A>& as, const many<B>& bs
    ) {
        return 

            commutative_group(harness,
                zero_name,    zero,
                sub_name,     sub,
                add_name,     add,
                as, bs) &&

            commutative_monoid(harness,
                one_name,  one, 
                mult_name, mult,
                as, bs) &&

            distributivity(harness, mult_name, mult, add_name, add, as, as, bs) &&
            distributivity(harness, mult_name, mult, add_name, add, as, bs, as) &&
            distributivity(harness, mult_name, mult, add_name, add, as, bs, bs) &&
            distributivity(harness, mult_name, mult, add_name, add, bs, as, as) &&
            distributivity(harness, mult_name, mult, add_name, add, bs, as, bs) &&
            distributivity(harness, mult_name, mult, add_name, add, bs, bs, as) &&

        true; // added so lines above can be easily swapped
    }

    template<typename Harness, 
        typename Zero, typename One, 
        typename Add, typename Sub, typename Mult, 
        typename A, typename B, typename C>
    bool commutative_ring(const Harness& harness, 
        const std::string zero_name, const Zero& zero, 
        const std::string one_name,  const One& one, 
        const std::string add_name,  const Add& add, 
        const std::string sub_name,  const Sub& sub, 
        const std::string mult_name, const Mult& mult, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return 

            commutative_group(harness,
                zero_name,    zero,
                sub_name,     sub,
                add_name,     add,
                as, bs, cs) &&

            commutative_monoid(harness,
                one_name,  one, 
                mult_name, mult,
                as, bs, cs) &&

            distributivity(harness, mult_name, mult, add_name, add, as, bs, cs) &&
            distributivity(harness, mult_name, mult, add_name, add, as, cs, bs) &&
            distributivity(harness, mult_name, mult, add_name, add, bs, as, cs) &&
            distributivity(harness, mult_name, mult, add_name, add, bs, cs, as) &&
            distributivity(harness, mult_name, mult, add_name, add, cs, as, bs) &&
            distributivity(harness, mult_name, mult, add_name, add, cs, bs, as) &&

        true; // added so lines above can be easily swapped
    }

}

