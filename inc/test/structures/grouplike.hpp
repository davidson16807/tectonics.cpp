#pragma once

// std libraries

// in-house libraries
#include "../properties.hpp"

namespace test {

    /*
    NOTE1: Unlike tests for properties, tests for structure are bulky and are expected to be called 
    multiple times on multiple kinds of input (See math/analytic/Polynomial_test.cpp for an example).
    to save us trouble, we implement classes that store parameters that are likely not to change.

    NOTE2: Any structure here is expected to demonstrate closure with respect to some set of values.
    Closure is enforced by the type system so no explicit tests are required,
    however sometimes a set of values may be implemented as multiple data types.
    For instance, polynomials are closed under multiplication, 
    but there are some degenerate cases of polynomials that we implement as separate types, 
    notably including scalars like `double`s.
    This is why we allow the user to provide sample values through several parameters of different type, 
    (seen below as `as`, `bs`, `cs`). If multiple data types are provided as test data,
    this vastly increases the number of tests we could perform,
    and since operations on each data type may be implemented separately, 
    we really should test for these situations. We do want our tests to run fast though, 
    so while we are not excused from running these tests, we do want to avoid needlessly repeating tests.
    In other words, we want be able to call these functions multiple times on different inputs
    without unecessarily repeating tests, and we also want to reduce the size of each test function's implementation.
    We've found the best way to do this is to have each method 
    only consider those tests that make use of all their arguments.
    */

    template<typename F>
    struct QuasiGroup{
        const std::string f_name;    const F f; 
        QuasiGroup(
            const std::string f_name,    const F& f
        ): 
            f_name(f_name),       f(f)
        {}

        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            discernability(adapter,           as)         &&
            determinism  (adapter, f_name, f, as, as)     &&
            cancellative (adapter, f_name, f, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            determinism  (adapter, f_name, f, as, bs)     &&
            determinism  (adapter, f_name, f, bs, as)     &&
            cancellative (adapter, f_name, f, as, as, bs) &&
            cancellative (adapter, f_name, f, as, bs, as) &&
            cancellative (adapter, f_name, f, as, bs, bs) &&
            cancellative (adapter, f_name, f, bs, as, as) &&
            cancellative (adapter, f_name, f, bs, as, bs) &&
            cancellative (adapter, f_name, f, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            cancellative (adapter, f_name, f, as, bs, cs) &&
            cancellative (adapter, f_name, f, as, cs, bs) &&
            cancellative (adapter, f_name, f, bs, as, cs) &&
            cancellative (adapter, f_name, f, bs, cs, as) &&
            cancellative (adapter, f_name, f, cs, as, bs) &&
            cancellative (adapter, f_name, f, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }

    };

    template<typename F>
    struct SemiGroup{
        const std::string f_name; const F f; 
        SemiGroup(
            const std::string f_name, const F& f
        ): 
            f_name(f_name), f(f)
        {}

        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            discernability(adapter,           as)         &&
            determinism  (adapter,            f_name, f, as, as)     &&
            associativity(adapter,            f_name, f, as, as, as) &&

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

    template<typename E, typename F>
    struct Monoid{
        const std::string e_name; const E e; 
        const std::string f_name; const F f; 
        Monoid(
            const std::string e_name, const E& e, 
            const std::string f_name, const F& f
        ): 
            e_name(e_name), e(e),
            f_name(f_name), f(f)
        {}

        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            discernability (adapter,            as)                    &&
            determinism    (adapter,            f_name, f, as, as)     &&
            associativity  (adapter,            f_name, f, as, as, as) &&
            binary_identity(adapter, e_name, e, f_name, f, as)         &&

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

    template<typename E, typename Finv, typename F>
    struct Loop{
        const std::string e_name;    const E e; 
        const std::string f_name;    const F f; 
        const std::string finv_name; const Finv finv; 
        Loop(
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

            discernability (adapter,           as)         &&
            determinism    (adapter,            f_name, f, as, as)     &&
            binary_identity(adapter, e_name, e, f_name, f, as)         &&
            determinism    (adapter,            finv_name, finv,            as, as) &&
            binary_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as)     &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            determinism  (adapter, f_name, f, as, bs)     &&
            determinism  (adapter, f_name, f, bs, as)     &&
            determinism  (adapter,            finv_name, finv,            as, bs) &&
            binary_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as) &&
            binary_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, bs) &&

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
        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            discernability (adapter,           as)         &&
            determinism    (adapter,            f_name, f, as, as)     &&
            associativity  (adapter,            f_name, f, as, as, as) &&
            binary_identity(adapter, e_name, e, f_name, f, as)         &&
            determinism    (adapter,            finv_name, finv,            as, as) &&
            binary_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as)     &&

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
            binary_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as) &&
            binary_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, bs) &&

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


    template<typename E, typename F>
    struct CommutativeMonoid{
        const std::string e_name; const E e; 
        const std::string f_name; const F f; 
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

            discernability (adapter,           as)         &&
            determinism    (adapter,            f_name, f, as, as)     &&
            associativity  (adapter,            f_name, f, as, as, as) &&
            binary_identity(adapter, e_name, e, f_name, f, as)         &&
            commutativity  (adapter,            f_name, f, as, as)     &&

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

            discernability (adapter,           as)         &&
            determinism    (adapter,            f_name, f, as, as)     &&
            associativity  (adapter,            f_name, f, as, as, as) &&
            binary_identity(adapter, e_name, e, f_name, f, as)         &&
            binary_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as) &&
            commutativity  (adapter,            f_name, f, as, as)     &&

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
            binary_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as) &&
            binary_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, bs) &&
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

}

