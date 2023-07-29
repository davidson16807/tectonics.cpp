#pragma once

// std libraries
#include <vector> // std::vector
#include <string> // std::string

// in-house libraries
#include "equality.hpp"
#include "predicate.hpp"

namespace test {

    template<typename T>
    using many = std::vector<T>;

    template<typename Harness, typename F, typename A>
    bool determinism(const Harness& harness, const F& f, const many<A>& as) {
        return equality(
            harness,
            "determinism", 
            "the first call ", [=](A a){ return f(a); },
            "the second call", [=](A a){ return f(a); },
            as);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool determinism(const Harness& harness, const F& f, const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            "determinism", 
            "the first call ", [=](A a, B b){ return f(a,b); },
            "the second call", [=](A a, B b){ return f(a,b); },
            as, bs);
    }

    template<typename Harness, typename F, typename A, typename B, typename C>
    bool determinism(const Harness& harness, const F& f, const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            "determinism", 
            "the first call ",[=](A a, B b, C c){ return f(a,b,c); },
            "the second call",[=](A a, B b, C c){ return f(a,b,c); },
            as, bs, cs);
    }

    template<typename Harness, typename F, typename A, typename E>
    bool identity(const Harness& harness, const F& f, const E& e, const many<A>& as) {
        return 
            left_identity (harness, f, e, as) &&
            right_identity(harness, f, e, as);
    }

    template<typename Harness, typename F, typename A, typename E>
    bool left_identity(const Harness& harness, const F& f, const E& e, const many<A>& as) {
        return equality(
            harness,
            "left identity", 
            "f(e,a)",[=](A a){ return f(e, a); },
            "a     ",[=](A a){ return a; },
            as);
    }

    template<typename Harness, typename F, typename A, typename E>
    bool right_identity(const Harness& harness, const F& f, const E& e, const many<A>& as) {
        return equality(
            harness,
            "right identity", 
            "f(a,e)", [=](A a){ return f(a, e); },
            "f(a)  ", [=](A a){ return a; },
            as);
    }

    template<typename Harness, typename F, typename A, typename N>
    bool dominance(const Harness& harness, const F& f, const N& n, const many<A>& as) {
        return 
            left_dominance (harness, f, n, as) &&
            right_dominance(harness, f, n, as);
    }

    template<typename Harness, typename F, typename A, typename N>
    bool left_dominance(const Harness& harness, const F& f, const N& n, const many<A>& as) {
        return equality(
            harness,
            "left dominance", 
            "f(n,a)", [=](A a){ return f(n, a); },
            "n     ", [=](A a){ return n; },
            as);
    }

    template<typename Harness, typename F, typename A, typename N>
    bool right_dominance(const Harness& harness, const F& f, const N& n, const many<A>& as) {
        return equality(
            harness,
            "right dominance", 
            "f(a,n)", [=](A a){ return f(a, n);  },
            "n     ", [=](A a){ return n; },
            as);
    }

    template<typename Harness, typename F, typename G, typename A, typename B>
    bool inverse(const Harness& harness, const F& f, const G& g, const many<A>& as) {
        return 
            left_inverse(harness, f, g, as) &&
            left_inverse(harness, g, f, as);
    }

    template<typename Harness, typename F, typename Finv, typename A, typename B>
    bool left_inverse(const Harness& harness, const Finv& finv, const F& f, const many<A>& as) {
        return equality(
            harness,
            "left inverse", 
            "f⁻¹(f(a))", [=](A a){  return finv(f(a)); },
            "a        ", [=](A a){  return a; },
            as);
    }

    template<typename Harness, typename F, typename A, typename B, typename C>
    bool associativity(const Harness& harness, const F& f, const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            "associativity", 
            "f(f(a,b), c)", [=](A a, B b, C c){ return f(f(a,b), c); },
            "f(a, f(b,c))", [=](A a, B b, C c){ return f(a, f(b,c)); },
            as, bs, cs);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool commutativity(const Harness& harness, const F& f, const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            "commutativity", 
            "f(a,b)", [=](A a, B b){ return f(a, b); },
            "f(b,a)", [=](A a, B b){ return f(b, a); },
            as, bs);
    }

    template<typename Harness, typename Invert, typename F, typename A, typename B>
    bool anticommutativity(const Harness& harness, const F& f, const Invert& inv, const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            "commutativity", 
            "-f(a,b)", [=](A a, B b){ return invert(f(a, b)); },
            " f(b,a)", [=](A a, B b){ return f(b, a); },
            as, bs);
    }

    template<typename Harness, typename F, typename Over, typename A, typename B, typename C>
    bool distributivity(const Harness& harness, const F& f, const Over& over, const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return 
            left_distributivity (harness, f, over, as, bs, cs) &&
            right_distributivity(harness, f, over, as, bs, cs);
    }

    template<typename Harness, typename F, typename Over, typename A, typename B, typename C>
    bool right_distributivity(const Harness& harness, const F& f, const Over& over, const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            "right distributivity", 
            "g(f(a,c), f(b,c))", [=](A a, B b, C c){ return over(f(a, c), f(b, c)); },
            "f(g(a,b), c)     ", [=](A a, B b, C c){ return f(over(a, b), c);       },
            as, bs, cs);
    }

    template<typename Harness, typename F, typename Over, typename A, typename B, typename C>
    bool left_distributivity(const Harness& harness, const F& f, const Over& over, const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            "left distributivity", 
            "g(f(c,a), f(c,b))", [=](A a, B b, C c){ return over(f(c, a), f(c, b)); },
            "f(c, g(a, b))    ", [=](A a, B b, C c){ return f(c, over(a, b));       },
            as, bs, cs);
    }

    template<typename Harness, typename F, typename A>
    bool idempotence(const Harness& harness, const F& f, const many<A>& as) {
        return equality(
            harness,
            "idempotence", 
            "f(a,a)", [=](A a){ return f(a, a); },
            "a     ", [=](A a){ return a; },
            as);
    }

    template<typename Harness, typename F, typename E, typename A>
    bool idempotence(const Harness& harness, const F& f, const E& e, const many<A>& as) {
        return equality(
            harness,
            "idempotence", 
            "f(a,a)", [=](A a){ return f(a,a);  },
            "e     ", [=](A a){ return e; },
            as);
    }

    template<typename Harness, typename F, typename N, typename A>
    bool nilpotence(const Harness& harness, const F& f, const N& n, const many<A>& as) {
        return equality(
            harness,
            "nilpotence", 
            "f(a,a)", [=](A a){ return f(a, a);  },
            "n     ", [=](A a){ return n; },
            as);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool left_involutivity(const Harness& harness, const F& f, const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            "left involutivity", 
            "f(a, f(a,b))", [=](A a, B b){ return f(a, f(a, b)); },
            "b           ", [=](A a, B b){ return b; },
            as, bs);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool right_involutivity(const Harness& harness, const F& f, const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            "right involutivity", 
            "f(f(a,b), b)", [=](A a, B b){ return f(f(a, b), b); },
            "a           ", [=](A a, B b){ return a; },
            as, bs);
    }

    template<typename Harness, typename Cross, typename Add, typename N, typename A, typename B, typename C>
    bool jacobi_identity(
            const Harness& harness, const Cross& cross, const Add& add, const N& n,
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            "right involutivity", 
            "a×(b×c) + b×(c×a) + c×(a×b)", [=](A a, B b, C c){ return add( cross(a, cross(b, c)), cross(b, cross(c, a)), cross(c, cross(a, b))); },
            "n                          ", [=](A a, B b, C c){ return n; },
            as, bs, cs);
    }


    template<typename Harness, typename Valid, typename F, typename A, typename B, typename C>
    bool codomain(const Harness& harness, const Valid& valid, const F& f, const many<A>& as) {
        return predicate(
            harness,
            "codomain", 
            [=](A a){ 
                return valid(
                    f(a)
                );
            }, as);
    }

    template<typename Harness, typename Valid, typename F, typename A, typename B, typename C>
    bool codomain(const Harness& harness, const Valid& valid, const F& f, const many<A>& as, const many<B>& bs) {
        return predicate(
            harness,
            "codomain", 
            [=](A a, B b){ 
                return valid(
                    f(a, b)
                );
            }, as, bs);
    }

    template<typename Harness, typename Valid, typename F, typename A, typename B, typename C>
    bool codomain(const Harness& harness, const Valid& valid, const F& f, const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return predicate(
            harness,
            "codomain", 
            [=](A a, B b, C c){ 
                return valid(
                    f(a, b, c)
                );
            }, as, bs, cs);
    }


    template<typename Harness, typename LT, typename Add, typename Distance, typename A, typename B, typename C>
    bool triangle_inequality(
            const Harness& harness, const LT& lt, const Add& add, const Distance& distance, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return predicate(
            harness,
            "right involutivity", 
            [=](A a, B b, C c){
                return lt(distance(a, c), add(distance(a, b), distance(b, c)));
            }, as, bs, cs);
    }

    template<typename Harness, typename Distance, typename A, typename B, typename C>
    bool triangle_inequality(
            const Harness& harness, const Distance& distance, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return predicate(
            harness,
            "right involutivity", 
            [=](A a, B b, C c){
                return distance(a, c) < distance(a, b) + distance(b, c);
            }, as, bs, cs);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool reflexivity(const Harness& harness, const Relation& r, const many<A>& as) {
        return predicate(
            harness,
            "reflexivity", 
            [=](A a){ 
                return r(a, a);
            }, as);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool irreflexivity(const Harness& harness, const Relation& r, const many<A>& as) {
        return predicate(
            harness,
            "irreflexivity", 
            [=](A a){ 
                return !r(a, a);
            }, as);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool symmetry(const Harness& harness, const Relation& r, const many<A>& as, const many<B>& bs) {
        return predicate(
            harness,
            "symmetry", 
            [=](A a, B b){ 
                return 
                    r(a, b) == 
                    r(b, a);
            }, as, bs);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool antisymmetry(const Harness& harness, const Relation& r, const many<A>& as, const many<B>& bs) {
        return predicate(
            harness,
            "antisymmetry", 
            [=](A a, B b){ 
                return 
                    r(a, b) != 
                    r(b, a);
            }, as, bs);
    }

    template<typename Harness, typename Relation, typename A, typename B, typename C>
    bool transitivity(const Harness& harness, const Relation& r, const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return predicate(
            harness,
            "transitivity", 
            [=](A a, B b, C c){ 
                return r(a, b) && r(b, c)? r(a, c) : true;
            }, as, bs, cs);
    }

    template<typename Harness, typename Relation, typename A, typename B, typename C>
    bool antitransitivity(const Harness& harness, const Relation& r, const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return predicate(
            harness,
            "antitransitivity", 
            [=](A a, B b, C c){ 
                return r(a, b) && r(b, c)? !r(a, c) : true;
            }, as, bs, cs);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool totality(const Harness& harness, const Relation& r, const many<A>& as, const many<B>& bs) {
        return predicate(
            harness,
            "totality", 
            [=](A a, B b){ 
                return r(a, b) || r(b, a);
            }, as, bs);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool strict_totality(const Harness& harness, const Relation& r, const many<A>& as, const many<B>& bs) {
        return predicate(
            harness,
            "strict totality", 
            [=](A a, B b){ 
                return r(a, b) ^ r(b, a);
            }, as, bs);
    }

}

