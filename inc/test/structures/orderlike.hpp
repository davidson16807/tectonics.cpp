#pragma once

// std libraries

// in-house libraries
#include "../properties.hpp"

namespace test {

    /*
    NOTE1: Unlike tests for properties, tests for structure are bulky and are expected to be called 
    multiple times on multiple kinds of input (See math/analytic/Polynomial_test.cpp for an example).
    to save us trouble, we implement classes that store parameters that are likely not to change.
    */

    template<typename R>
    struct PreOrder{
        const std::string r_name;    const R r; 
        PreOrder(
            const std::string r_name,    const R& r
        ): 
            r_name(r_name),       r(r)
        {}

        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            discernability(adapter,           as)          &&
            determinism   (adapter, r_name, r, as, as)     &&
            reflexivity   (adapter, r_name, r, as, as)     &&
            transitivity  (adapter, r_name, r, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            determinism   (adapter, r_name, r, as, bs)     &&
            determinism   (adapter, r_name, r, bs, as)     &&
            determinism   (adapter, r_name, r, as, as)     &&
            reflexivity   (adapter, r_name, r, as, bs)     &&
            reflexivity   (adapter, r_name, r, bs, as)     &&
            transitivity  (adapter, r_name, r, as, as, bs) &&
            transitivity  (adapter, r_name, r, as, bs, as) &&
            transitivity  (adapter, r_name, r, as, bs, bs) &&
            transitivity  (adapter, r_name, r, bs, as, bs) &&
            transitivity  (adapter, r_name, r, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            transitivity  (adapter, r_name, r, as, bs, cs) &&
            transitivity  (adapter, r_name, r, as, cs, bs) &&
            transitivity  (adapter, r_name, r, bs, as, cs) &&
            transitivity  (adapter, r_name, r, bs, cs, as) &&
            transitivity  (adapter, r_name, r, cs, as, bs) &&
            transitivity  (adapter, r_name, r, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }

    };

    template<typename R>
    struct PartialOrder{
        const std::string r_name;    const R r; 
        PreOrder(
            const std::string r_name,    const R& r
        ): 
            r_name(r_name),       r(r)
        {}

        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            discernability(adapter,           as)          &&
            determinism   (adapter, r_name, r, as, as)     &&
            reflexivity   (adapter, r_name, r, as, as)     &&
            antisymmetry  (adapter, r_name, r, as, as)     &&
            transitivity  (adapter, r_name, r, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            determinism   (adapter, r_name, r, as, bs)     &&
            determinism   (adapter, r_name, r, bs, as)     &&
            determinism   (adapter, r_name, r, as, as)     &&
            reflexivity   (adapter, r_name, r, as, bs)     &&
            reflexivity   (adapter, r_name, r, bs, as)     &&
            antisymmetry  (adapter, r_name, r, as, bs)     &&
            antisymmetry  (adapter, r_name, r, bs, as)     &&
            transitivity  (adapter, r_name, r, as, as, bs) &&
            transitivity  (adapter, r_name, r, as, bs, as) &&
            transitivity  (adapter, r_name, r, as, bs, bs) &&
            transitivity  (adapter, r_name, r, bs, as, bs) &&
            transitivity  (adapter, r_name, r, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            transitivity  (adapter, r_name, r, as, bs, cs) &&
            transitivity  (adapter, r_name, r, as, cs, bs) &&
            transitivity  (adapter, r_name, r, bs, as, cs) &&
            transitivity  (adapter, r_name, r, bs, cs, as) &&
            transitivity  (adapter, r_name, r, cs, as, bs) &&
            transitivity  (adapter, r_name, r, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }

    };


    template<typename R>
    struct TotalPartialOrder{
        const std::string r_name;    const R r; 
        PreOrder(
            const std::string r_name,    const R& r
        ): 
            r_name(r_name),       r(r)
        {}

        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            discernability(adapter,           as)          &&
            determinism   (adapter, r_name, r, as, as)     &&
            reflexivity   (adapter, r_name, r, as, as)     &&
            antisymmetry  (adapter, r_name, r, as, as)     &&
            totality      (adapter, r_name, r, as, as)     &&
            transitivity  (adapter, r_name, r, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            determinism   (adapter, r_name, r, as, bs)     &&
            determinism   (adapter, r_name, r, bs, as)     &&
            determinism   (adapter, r_name, r, as, as)     &&
            reflexivity   (adapter, r_name, r, as, bs)     &&
            reflexivity   (adapter, r_name, r, bs, as)     &&
            antisymmetry  (adapter, r_name, r, as, bs)     &&
            antisymmetry  (adapter, r_name, r, bs, as)     &&
            totality      (adapter, r_name, r, as, bs)     &&
            totality      (adapter, r_name, r, bs, as)     &&
            transitivity  (adapter, r_name, r, as, as, bs) &&
            transitivity  (adapter, r_name, r, as, bs, as) &&
            transitivity  (adapter, r_name, r, as, bs, bs) &&
            transitivity  (adapter, r_name, r, bs, as, bs) &&
            transitivity  (adapter, r_name, r, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            transitivity  (adapter, r_name, r, as, bs, cs) &&
            transitivity  (adapter, r_name, r, as, cs, bs) &&
            transitivity  (adapter, r_name, r, bs, as, cs) &&
            transitivity  (adapter, r_name, r, bs, cs, as) &&
            transitivity  (adapter, r_name, r, cs, as, bs) &&
            transitivity  (adapter, r_name, r, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }

    };


    template<typename R>
    struct Equivalence{
        const std::string r_name;    const R r; 
        PreOrder(
            const std::string r_name,    const R& r
        ): 
            r_name(r_name),       r(r)
        {}

        template<typename Adapter, typename A>
        bool valid(const Adapter& adapter, const many<A>& as) const {
            return 

            discernability(adapter,           as)          &&
            determinism   (adapter, r_name, r, as, as)     &&
            reflexivity   (adapter, r_name, r, as, as)     &&
            symmetry      (adapter, r_name, r, as, as)     &&
            transitivity  (adapter, r_name, r, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs) const {
            return 

            determinism   (adapter, r_name, r, as, bs)     &&
            determinism   (adapter, r_name, r, bs, as)     &&
            determinism   (adapter, r_name, r, as, as)     &&
            reflexivity   (adapter, r_name, r, as, bs)     &&
            reflexivity   (adapter, r_name, r, bs, as)     &&
            symmetry      (adapter, r_name, r, as, bs)     &&
            symmetry      (adapter, r_name, r, bs, as)     &&
            transitivity  (adapter, r_name, r, as, as, bs) &&
            transitivity  (adapter, r_name, r, as, bs, as) &&
            transitivity  (adapter, r_name, r, as, bs, bs) &&
            transitivity  (adapter, r_name, r, bs, as, bs) &&
            transitivity  (adapter, r_name, r, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename A, typename B, typename C>
        bool valid(const Adapter& adapter, const many<A>& as, const many<B>& bs, const many<C>& cs) const {
            return 

            transitivity  (adapter, r_name, r, as, bs, cs) &&
            transitivity  (adapter, r_name, r, as, cs, bs) &&
            transitivity  (adapter, r_name, r, bs, as, cs) &&
            transitivity  (adapter, r_name, r, bs, cs, as) &&
            transitivity  (adapter, r_name, r, cs, as, bs) &&
            transitivity  (adapter, r_name, r, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }

    };

}

