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
    bool determinism(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " must be able to be called repeatedly without changing the output", 
            "[the first call] ", [=](A a){ return f(a); },
            "[the second call]", [=](A a){ return f(a); },
            as);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool determinism(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            f_name + " must be able to be called repeatedly without changing the output", 
            "[the first call] ", [=](A a, B b){ return f(a,b); },
            "[the second call]", [=](A a, B b){ return f(a,b); },
            as, bs);
    }

    template<typename Harness, typename F, typename A, typename B, typename C>
    bool determinism(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            f_name + " must be able to be called repeatedly without changing the output", 
            "[the first call] ",[=](A a, B b, C c){ return f(a,b,c); },
            "[the second call]",[=](A a, B b, C c){ return f(a,b,c); },
            as, bs, cs);
    }

    template<typename Harness, typename F, typename E, typename A>
    bool identity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string e_name, const E& e, 
            const many<A>& as) {
        return 
            left_identity (harness, f_name, f, e_name, e, as) &&
            right_identity(harness, f_name, f, e_name, e, as);
    }

    template<typename Harness, typename F, typename E, typename A>
    bool identity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string e_name, const E& e) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must have an \"identity\" value, " + e_name + "[denoted \"e\"], that when passed will always return itself", 
            "f(e,a)",[=](A a){ return f(e, a); },
            "a     ",[=](A a){ return a; },
            std::vector<E>{e});
    }

    template<typename Harness, typename F, typename E, typename A>
    bool left_identity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string e_name, const E& e, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must have an \"identity\" value, " + e_name + "[denoted \"e\"], that when passed on the left will always return the other value", 
            "f(e,a)",[=](A a){ return f(e, a); },
            "a     ",[=](A a){ return a; },
            as);
    }

    template<typename Harness, typename F, typename E, typename A>
    bool right_identity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string e_name, const E& e, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must have an \"identity\" value, " + e_name + "[denoted \"e\"], that when passed on the right will always return the other value", 
            "f(a,e)", [=](A a){ return f(a, e); },
            "f(a)  ", [=](A a){ return a; },
            as);
    }

    template<typename Harness, typename F, typename A, typename N>
    bool dominance(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string n_name, const N& n, 
            const many<A>& as) {
        return 
            left_dominance (harness, f_name, f, n_name, n, as) &&
            right_dominance(harness, f_name, f, n_name, n, as);
    }

    template<typename Harness, typename F, typename A, typename N>
    bool left_dominance(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string n_name, const N& n, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must have a \"dominant\" value, " + n_name + " [denoted \"n\"], that when passed on the left will always return itself", 
            "f(n,a)", [=](A a){ return f(n, a); },
            "n     ", [=](A a){ return n; },
            as);
    }

    template<typename Harness, typename F, typename A, typename N>
    bool right_dominance(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string n_name, const N& n, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must have a \"dominant\" value, " + n_name + " [denoted \"n\"], that when passed on the right will always return itself", 
            "f(a,n)", [=](A a){ return f(a, n);  },
            "n     ", [=](A a){ return n; },
            as);
    }

    template<typename Harness, typename F, typename G, typename A>
    bool invertibility(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string g_name, const G& g, 
            const many<A>& as) {
        return 
            left_invertibility(harness, f_name, f, f_name, g, as) &&
            left_invertibility(harness, g_name, g, g_name, f, as);
    }

    template<typename Harness, typename F, typename Finv, typename A>
    bool left_invertibility(const Harness& harness, 
            const std::string finv_name, const Finv& finv, 
            const std::string f_name, const F& f, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must permit input to be reconstructed by an \"inverse\" function, " + finv_name + " [denoted \"f⁻¹\"]", 
            "f⁻¹(f(a))", [=](A a)->A{  return finv(f(a)); },
            "a        ", [=](A a)->A{  return a; },
            as);
    }

    template<typename Harness, typename F, typename A, typename B, typename C>
    bool associativity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must allow invocations to be calculated in any order without changing results", 
            "f(f(a,b), c)", [=](A a, B b, C c){ return f(f(a,b), c); },
            "f(a, f(b,c))", [=](A a, B b, C c){ return f(a, f(b,c)); },
            as, bs, cs);
    }

    template<typename Harness, typename F2, typename F3, typename A, typename B, typename C>
    bool left_associativity(const Harness& harness, 
            const std::string f2_name, const F2& f2, 
            const std::string f3_name, const F3& f3, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            f3_name + " [denoted \"f\"] must calculate starting from the leftmost parameters", 
            "f(f(a,b), c)", [=](A a, B b, C c){ return f2(f2(a, b), c); },
            "f(  a,b,  c)", [=](A a, B b, C c){ return f3(   a, b,  c); },
            as, bs, cs);
    }

    template<typename Harness, typename F2, typename F3, typename A, typename B, typename C>
    bool right_associativity(const Harness& harness, 
            const std::string f2_name, const F2& f2, 
            const std::string f3_name, const F3& f3, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            f3_name + " [denoted \"f\"] must calculate starting from the rightmost parameters", 
            "f(a, f(b,c))", [=](A a, B b, C c){ return f2(a, f2(b, c)); },
            "f(a,   b,c)",  [=](A a, B b, C c){ return f3(a,    b, c);  },
            as, bs, cs);
    }

    template<typename Harness, typename F, typename A, typename B, typename C>
    bool power_associativity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must allow invocations with a single parameter to be calculated in any order without changing results", 
            "f(f(a,a), a)", [=](A a){ return f(f(a,a), a); },
            "f(a, f(a,a))", [=](A a){ return f(a, f(a,a)); },
            as);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool alternativity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs) {
        return 
            left_alternativity (harness, f_name, f, as, bs) &&
            right_alternativity(harness, f_name, f, as, bs);
    }

    template<typename Harness, typename F, typename A, typename B, typename C>
    bool left_alternativity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must under certain conditions allow invocations to be calculated in any order without changing results", 
            "f(f(a,a), b)", [=](A a, B b){ return f(f(a,a), b); },
            "f(a, f(a,b))", [=](A a, B b){ return f(a, f(a,b)); },
            as, bs);
    }

    template<typename Harness, typename F, typename A, typename B, typename C>
    bool right_alternativity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must under certain conditions allow invocations to be calculated in any order without changing results", 
            "f(f(a,b), b)", [=](A a, B b){ return f(f(a,b), b); },
            "f(a, f(b,b))", [=](A a, B b){ return f(a, f(b,b)); },
            as, bs);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool flexibility(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must under certain conditions allow invocations to be calculated in any order without changing results", 
            "f(f(a,b), a)", [=](A a, B b){ return f(f(a,b), a); },
            "f(a,f(b, a))", [=](A a, B b){ return f(a,f(b, a)); },
            as, bs);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool commutativity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must allow arguments to be passed in any order without changing results", 
            "f(a,b)", [=](A a, B b){ return f(a, b); },
            "f(b,a)", [=](A a, B b){ return f(b, a); },
            as, bs);
    }

    template<typename Harness, typename Invert, typename F, typename A, typename B>
    bool anticommutativity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string inv_name, const Invert& inv, 
            const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must return the opposite value if its arguments are flipped, where opposite is determined by "+inv_name+" [denoted \"-\"]", 
            "-f(a,b)", [=](A a, B b){ return invert(f(a, b)); },
            " f(b,a)", [=](A a, B b){ return f(b, a); },
            as, bs);
    }

    template<typename Harness, typename F, typename G, typename A, typename B, typename C>
    bool distributivity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string g_name, const G& g, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return 
            left_distributivity (harness, f_name, f, g_name, g, as, bs, cs) &&
            right_distributivity(harness, f_name, f, g_name, g, as, bs, cs);
    }

    template<typename Harness, typename F, typename G, typename A, typename B, typename C>
    bool right_distributivity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string g_name, const G& g, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must distribute over another function [denoted \"g\"] and produce the same results", 
            "f(g(a,b), c)     ", [=](A a, B b, C c){ return f(g(a, b), c);       },
            "g(f(a,c), f(b,c))", [=](A a, B b, C c){ return g(f(a, c), f(b, c)); },
            as, bs, cs);
    }

    template<typename Harness, typename F, typename G, typename A, typename B, typename C>
    bool left_distributivity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string g_name, const G& g, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must distribute over another function [denoted \"g\"] and produce the same results", 
            "f(c, g(a, b))    ", [=](A a, B b, C c){ return f(c, g(a, b));       },
            "g(f(c,a), f(c,b))", [=](A a, B b, C c){ return g(f(c, a), f(c, b)); },
            as, bs, cs);
    }

    template<typename Harness, typename F, typename A>
    bool idempotence(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must return input if both parameters are the same", 
            "f(a,a)", [=](A a){ return f(a, a); },
            "a     ", [=](A a){ return a; },
            as);
    }

    template<typename Harness, typename F, typename E, typename A>
    bool idempotence(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string e_name, const E& e, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must return the identity "+ e_name +" [denoted \"e\"] if both parameters are the same", 
            "f(a,a)", [=](A a){ return f(a,a);  },
            "e     ", [=](A a){ return e; },
            as);
    }

    template<typename Harness, typename F, typename N, typename A>
    bool nilpotence(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string n_name, const N& n, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must return the index "+ n_name +" [denoted \"n\"] if both parameters are the same", 
            "f(a,a)", [=](A a){ return f(a, a);  },
            "n     ", [=](A a){ return n; },
            as);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool involutivity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as) {
        return equality(
            harness,
            "left involutivity", 
            f_name + " [denoted \"f\"] must return the original input if invoked a second time", 
            "f(f(a))", [=](A a){ return f(f(a)); },
            "a      ", [=](A a){ return a; },
            as);
    }

    template<typename Harness, typename F, typename G, typename A, typename B, typename C>
    bool involutivity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs) {
        return 
            left_involutivity (harness, f_name, f, as, bs) &&
            right_involutivity(harness, f_name, f, as, bs);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool left_involutivity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            "left involutivity", 
            f_name + " [denoted \"f\"] must return the original input if invoked a second time with the same left parameter", 
            "f(a, f(a,b))", [=](A a, B b){ return f(a, f(a, b)); },
            "b           ", [=](A a, B b){ return b; },
            as, bs);
    }

    template<typename Harness, typename F, typename A, typename B>
    bool right_involutivity(const Harness& harness, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must return the original input if invoked a second time with the same right parameter", 
            "f(f(a,b), b)", [=](A a, B b){ return f(f(a, b), b); },
            "a           ", [=](A a, B b){ return a; },
            as, bs);
    }

    template<typename Harness, typename Cross, typename Add, typename N, typename A, typename B, typename C>
    bool jacobi_identity(
            const Harness& harness, 
            const std::string cross_name, const Cross& cross, 
            const std::string add_name, const Add& add, 
            const std::string n_name, const N& n,
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return equality(
            harness,
            cross_name + " [denoted \"×\"] and "+ add_name +" [denoted \"+\"] must satisfy the jacobi identity for index "+ n_name+ " [denoted \"n\"]", 
            "a×(b×c) + b×(c×a) + c×(a×b)", [=](A a, B b, C c){ return add( cross(a, cross(b, c)), cross(b, cross(c, a)), cross(c, cross(a, b))); },
            "n                          ", [=](A a, B b, C c){ return n; },
            as, bs, cs);
    }

    template<typename Harness, typename F, typename G, typename A>
    bool invariance(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string g_name, const G& g, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must be invariant to " + g_name + " [denoted \"g\"]", 
            "f(g(a))", [=](A a){ return f(g(a)); },
            "f(a)   ", [=](A a){ return f(a); },
            as);
    }

    template<typename Harness, typename F, typename G, typename A>
    bool conservation(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string g_name, const G& g, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must conserve " + g_name + " [denoted \"g\"]", 
            "g(f(a))", [=](A a){ return g(f(a)); },
            "g(a)   ", [=](A a){ return g(a); },
            as);
    }

    template<typename Harness, typename F, typename G, typename A>
    bool preservation(const Harness& harness, 
            const std::string f_name, const F& f, 
            const std::string g_name, const G& g, 
            const many<A>& as) {
        return equality(
            harness,
            f_name + " [denoted \"f\"] must preserve " + g_name + " [denoted \"g\"]", 
            "g(f(a))", [=](A a){ return g(f(a)); },
            "g(a)   ", [=](A a){ return g(a); },
            as);
    }





    template<typename Harness, typename Valid, typename F, typename A>
    bool codomain(const Harness& harness, 
            const std::string validity_name, const Valid& valid, 
            const std::string f_name, const F& f, 
            const many<A>& as) {
        return predicate(
            harness,
            f_name + " must only produce output " + validity_name,
            [=](A a){ 
                return valid(
                    f(a)
                );
            }, as);
    }

    template<typename Harness, typename Valid, typename F, typename A, typename B>
    bool codomain(const Harness& harness, 
            const std::string validity_name, const Valid& valid, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs) {
        return predicate(
            harness,
            f_name + " must only produce output " + validity_name, 
            [=](A a, B b){ 
                return valid(
                    f(a, b)
                );
            }, as, bs);
    }

    template<typename Harness, typename Valid, typename F, typename A, typename B, typename C>
    bool codomain(const Harness& harness, 
            const std::string validity_name, const Valid& valid, 
            const std::string f_name, const F& f, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return predicate(
            harness,
            f_name + " must only produce output " + validity_name, 
            [=](A a, B b, C c){ 
                return valid(
                    f(a, b, c)
                );
            }, as, bs, cs);
    }

    template<typename Harness, typename LT, typename Add, typename Distance, typename A, typename B, typename C>
    bool triangle_inequality(const Harness& harness, 
            const LT& lt, const Add& add, const Distance& distance, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return predicate(
            harness,
            "triangle inequality", 
            [=](A a, B b, C c){
                return lt(distance(a, c), add(distance(a, b), distance(b, c)));
            }, as, bs, cs);
    }


    template<typename Harness, typename Distance, typename A, typename B, typename C>
    bool triangle_inequality(const Harness& harness, 
            const Distance& distance, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return predicate(
            harness,
            "triangle inequality", 
            [=](A a, B b, C c){
                return distance(a, c) < distance(a, b) + distance(b, c);
            }, as, bs, cs);
    }





    template<typename Harness, typename R, typename A, typename B>
    bool reflexivity(const Harness& harness, 
            const std::string r_name, const R& r, 
            const many<A>& as) {
        return predicate(
            harness,
            "reflexivity", 
            [=](A a){ 
                return r(a, a);
            }, as);
    }

    template<typename Harness, typename R, typename A, typename B>
    bool irreflexivity(const Harness& harness, 
            const std::string r_name, const R& r, 
            const many<A>& as) {
        return predicate(
            harness,
            "irreflexivity", 
            [=](A a){ 
                return !r(a, a);
            }, as);
    }

    template<typename Harness, typename R, typename A, typename B>
    bool symmetry(const Harness& harness, 
            const std::string r_name, const R& r, 
            const many<A>& as, const many<B>& bs) {
        return predicate(
            harness,
            "symmetry", 
            [=](A a, B b){ 
                return 
                    r(a, b) == 
                    r(b, a);
            }, as, bs);
    }

    template<typename Harness, typename R, typename A, typename B>
    bool antisymmetry(const Harness& harness, 
            std::string r_name, const R& r, 
            const many<A>& as, const many<B>& bs) {
        return predicate(
            harness,
            "antisymmetry", 
            [=](A a, B b){ 
                return 
                    r(a, b) != 
                    r(b, a);
            }, as, bs);
    }

    template<typename Harness, typename R, typename A, typename B, typename C>
    bool transitivity(const Harness& harness, 
            std::string r_name, const R& r, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return predicate(
            harness,
            "transitivity", 
            [=](A a, B b, C c){ 
                return r(a, b) && r(b, c)? r(a, c) : true;
            }, as, bs, cs);
    }

    template<typename Harness, typename R, typename A, typename B, typename C>
    bool antitransitivity(const Harness& harness, 
            std::string r_name, const R& r, 
            const many<A>& as, const many<B>& bs, const many<C>& cs) {
        return predicate(
            harness,
            "antitransitivity", 
            [=](A a, B b, C c){ 
                return r(a, b) && r(b, c)? !r(a, c) : true;
            }, as, bs, cs);
    }

    template<typename Harness, typename R, typename A, typename B>
    bool totality(const Harness& harness, 
            std::string r_name, const R& r, 
            const many<A>& as, const many<B>& bs) {
        return predicate(
            harness,
            "totality", 
            [=](A a, B b){ 
                return r(a, b) || r(b, a);
            }, as, bs);
    }

    template<typename Harness, typename R, typename A, typename B>
    bool strict_totality(const Harness& harness, 
            std::string r_name, const R& r, 
            const many<A>& as, const many<B>& bs) {
        return predicate(
            harness,
            "strict totality", 
            [=](A a, B b){ 
                return r(a, b) ^ r(b, a);
            }, as, bs);
    }






}

