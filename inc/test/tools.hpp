#pragma once

// std libraries
#include <vector> // std::vector
#include <string> // std::string

// in-house libraries
#include "iterate.hpp"

namespace test {

    template<typename T>
    using many = std::vector<T>;

    template<typename Harness, typename F, typename A>
    bool determinism(const Harness& harness, const F& f, const std::vector<A>& as) {
        return iterate(
            "determinism", 
            [=](auto x){ return harness.print(x); },
            [=](A a){ 
                return harness.equal(
                    f(a), 
                    f(a)); 
            }, as);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool determinism(const Harness& harness, const F& f, const std::vector<A>& as, const std::vector<B>& bs) {
        return iterate(
            "determinism", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b){ 
                return harness.equal(
                    f(a,b), 
                    f(a,b)); 
            }, as, bs);
    }

    template<typename Harness, typename F, typename A, typename B, typename C>
    bool determinism(const Harness& harness, const F& f, const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return iterate(
            "determinism", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b, C c){ 
                return harness.equal(
                    f(a,b,c), 
                    f(a,b,c)); 
            }, as, bs, cs);
    }

    template<typename Harness, typename F, typename A, typename E>
    bool identity(const Harness& harness, const F& f, const E& e, const std::vector<A>& as) {
        return 
            left_identity (harness, f, e, as) &&
            right_identity(harness, f, e, as);
    }

    template<typename Harness, typename F, typename A, typename E>
    bool left_identity(const Harness& harness, const F& f, const E& e, const std::vector<A>& as) {
        return iterate(
            "left identity", 
            [=](auto x){ return harness.print(x); },
            [=](A a){ 
                return harness.equal(
                    f(e, a), 
                    a
                ); 
            }, as);
    }

    template<typename Harness, typename F, typename A, typename E>
    bool right_identity(const Harness& harness, const F& f, const E& e, const std::vector<A>& as) {
        return iterate(
            "right identity", 
            [=](auto x){ return harness.print(x); },
            [=](A a){ 
                return harness.equal(
                    f(a, e), 
                    a
                ); 
            }, as);
    }

    template<typename Harness, typename F, typename A, typename N>
    bool dominance(const Harness& harness, const F& f, const N& n, const std::vector<A>& as) {
        return 
            left_dominance (harness, f, n, as) &&
            right_dominance(harness, f, n, as);
    }

    template<typename Harness, typename F, typename A, typename N>
    bool left_dominance(const Harness& harness, const F& f, const N& n, const std::vector<A>& as) {
        return iterate(
            "left dominance", 
            [=](auto x){ return harness.print(x); },
            [=](A a){ 
                return harness.equal(
                    f(n, a), 
                    n
                ); 
            }, as);
    }

    template<typename Harness, typename F, typename A, typename N>
    bool right_dominance(const Harness& harness, const F& f, const N& n, const std::vector<A>& as) {
        return iterate(
            "right dominance", 
            [=](auto x){ return harness.print(x); },
            [=](A a){ 
                return harness.equal(
                    f(a, n), 
                    n
                );
            }, as);
    }

    template<typename Harness, typename F, typename G, typename A, typename B>
    bool inverse(const Harness& harness, const F& f, const G& g, const std::vector<A>& as) {
        return 
            left_inverse(harness, f, g, as) &&
            left_inverse(harness, g, f, as);
    }

    template<typename Harness, typename F, typename Finv, typename A, typename B>
    bool left_inverse(const Harness& harness, const Finv& finv, const F& f, const std::vector<A>& as) {
        return iterate(
            "left inverse", 
            [=](auto x){ return harness.print(x); },
            [=](A a){ 
                return harness.equal(
                    finv(f(a)),
                    a
                ); 
            }, as);
    }

    template<typename Harness, typename F, typename A, typename B, typename C>
    bool associativity(const Harness& harness, const F& f, const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return iterate(
            "associativity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b, C c){ 
                return harness.equal(
                    f(f(a,   b), c),
                    f(  a, f(b, c))
                );
            }, as, bs, cs);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool commutativity(const Harness& harness, const F& f, const std::vector<A>& as, const std::vector<B>& bs) {
        return iterate(
            "commutativity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b){ 
                return harness.equal(
                    f(a, b), 
                    f(b, a)
                );
            }, as, bs);
    }

    template<typename Harness, typename Invert, typename F, typename A, typename B>
    bool anticommutativity(const Harness& harness, const F& f, const Invert& inv, const std::vector<A>& as, const std::vector<B>& bs) {
        return iterate(
            "commutativity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b){
                return harness.equal(
                    invert(f(a, b)), 
                           f(b, a)
                );
            }, as, bs);
    }

    template<typename Harness, typename F, typename Over, typename A, typename B, typename C>
    bool distributivity(const Harness& harness, const F& f, const Over& over, const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return 
            left_distributivity (harness, f, over, as, bs, cs) &&
            right_distributivity(harness, f, over, as, bs, cs);
    }

    template<typename Harness, typename F, typename Over, typename A, typename B, typename C>
    bool right_distributivity(const Harness& harness, const F& f, const Over& over, const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return iterate(
            "right distributivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b, C c){ 
                return harness.equal(
                    f(over(a, b), c),
                    over(f(a, c), f(b, c))
                );
            }, as, bs, cs);
    }

    template<typename Harness, typename F, typename Over, typename A, typename B, typename C>
    bool left_distributivity(const Harness& harness, const F& f, const Over& over, const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return iterate(
            "left distributivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b, C c){ 
                return harness.equal(
                    f(c, over(a, b)),
                    over(f(c, a), f(c, b))
                );
            }, as, bs, cs);
    }

    template<typename Harness, typename Valid, typename F, typename A, typename B, typename C>
    bool codomain(const Harness& harness, const Valid& valid, const F& f, const std::vector<A>& as) {
        return iterate(
            "codomain", 
            [=](auto x){ return harness.print(x); },
            [=](A a){ 
                return valid(
                    f(a)
                );
            }, as);
    }

    template<typename Harness, typename Valid, typename F, typename A, typename B, typename C>
    bool codomain(const Harness& harness, const Valid& valid, const F& f, const std::vector<A>& as, const std::vector<B>& bs) {
        return iterate(
            "codomain", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b){ 
                return valid(
                    f(a, b)
                );
            }, as, bs);
    }

    template<typename Harness, typename Valid, typename F, typename A, typename B, typename C>
    bool codomain(const Harness& harness, const Valid& valid, const F& f, const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return iterate(
            "codomain", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b, C c){ 
                return valid(
                    f(a, b, c)
                );
            }, as, bs, cs);
    }

    template<typename Harness, typename F, typename A>
    bool idempotence(const Harness& harness, const F& f, const std::vector<A>& as) {
        return iterate(
            "idempotence", 
            [=](auto x){ return harness.print(x); },
            [=](A a){
                return harness.equal(
                    f(a, a),
                    a
                );
            }, as);
    }

    template<typename Harness, typename F, typename E, typename A>
    bool idempotence(const Harness& harness, const F& f, const E& e, const std::vector<A>& as) {
        return iterate(
            "idempotence", 
            [=](auto x){ return harness.print(x); },
            [=](A a){
                return harness.equal(
                    f(a, a),
                    e
                );
            }, as);
    }

    template<typename Harness, typename F, typename N, typename A>
    bool nilpotence(const Harness& harness, const F& f, const N& n, const std::vector<A>& as) {
        return iterate(
            "nilpotence", 
            [=](auto x){ return harness.print(x); },
            [=](A a){
                return harness.equal(
                    f(a, a),
                    n
                );
            }, as);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool left_involutivity(const Harness& harness, const F& f, const std::vector<A>& as, const std::vector<B>& bs) {
        return iterate(
            "left involutivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b){
                return harness.equal(
                    f(a, f(a, b)),
                    b
                );
            }, as, bs);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool right_involutivity(const Harness& harness, const F& f, const std::vector<A>& as, const std::vector<B>& bs) {
        return iterate(
            "right involutivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b){
                return harness.equal(
                    f(f(a, b), b),
                    a
                );
            }, as, bs);
    }

    template<typename Harness, typename Cross, typename Add, typename N, typename A, typename B, typename C>
    bool jacobi_identity(
            const Harness& harness, const Cross& cross, const Add& add, const N& n,
            const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return iterate(
            "right involutivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b, C c){
                return harness.equal( 
                    add( cross(a, cross(b, c)), cross(b, cross(c, a)), cross(c, cross(a, b)) ),
                    0);
            }, as, bs, cs);
    }

    template<typename Harness, typename LT, typename Add, typename Distance, typename A, typename B, typename C>
    bool triangle_inequality(
            const Harness& harness, const LT& lt, const Add& add, const Distance& distance, 
            const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return iterate(
            "right involutivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b, C c){
                return lt(distance(a, c), add(distance(a, b), distance(b, c)));
            }, as, bs, cs);
    }

    template<typename Harness, typename Distance, typename A, typename B, typename C>
    bool triangle_inequality(
            const Harness& harness, const Distance& distance, 
            const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return iterate(
            "right involutivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b, C c){
                return distance(a, c) < distance(a, b) + distance(b, c);
            }, as, bs, cs);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool reflexivity(const Harness& harness, const Relation& r, const std::vector<A>& as) {
        return iterate(
            "reflexivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a){ 
                return r(a, a);
            }, as);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool irreflexivity(const Harness& harness, const Relation& r, const std::vector<A>& as) {
        return iterate(
            "irreflexivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a){ 
                return !r(a, a);
            }, as);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool symmetry(const Harness& harness, const Relation& r, const std::vector<A>& as, const std::vector<B>& bs) {
        return iterate(
            "symmetry", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b){ 
                return 
                    r(a, b) == 
                    r(b, a);
            }, as, bs);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool antisymmetry(const Harness& harness, const Relation& r, const std::vector<A>& as, const std::vector<B>& bs) {
        return iterate(
            "antisymmetry", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b){ 
                return 
                    r(a, b) != 
                    r(b, a);
            }, as, bs);
    }

    template<typename Harness, typename Relation, typename A, typename B, typename C>
    bool transitivity(const Harness& harness, const Relation& r, const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return iterate(
            "transitivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b, C c){ 
                return r(a, b) && r(b, c)? r(a, c) : true;
            }, as, bs, cs);
    }

    template<typename Harness, typename Relation, typename A, typename B, typename C>
    bool antitransitivity(const Harness& harness, const Relation& r, const std::vector<A>& as, const std::vector<B>& bs, const std::vector<C>& cs) {
        return iterate(
            "antitransitivity", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b, C c){ 
                return r(a, b) && r(b, c)? !r(a, c) : true;
            }, as, bs, cs);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool totality(const Harness& harness, const Relation& r, const std::vector<A>& as, const std::vector<B>& bs) {
        return iterate(
            "totality", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b){ 
                return r(a, b) || r(b, a);
            }, as, bs);
    }

    template<typename Harness, typename Relation, typename A, typename B>
    bool strict_totality(const Harness& harness, const Relation& r, const std::vector<A>& as, const std::vector<B>& bs) {
        return iterate(
            "strict totality", 
            [=](auto x){ return harness.print(x); },
            [=](A a, B b){ 
                return r(a, b) ^ r(b, a);
            }, as, bs);
    }

}

