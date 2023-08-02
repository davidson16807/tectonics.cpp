#pragma once

// std libraries
#include <vector> // std::vector

// in-house libraries
#include "equality.hpp"
#include "predicate.hpp"

namespace test {

    template<typename T>
    using many = std::vector<T>;

    template<typename Adapter, typename F, typename A>
    bool determinism(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " must be able to be called repeatedly without changing the output", 
            "[the first call] ", f,
            "[the second call]", f,
            as);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool determinism(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return equality(
            adapter,
            f_name + " must be able to be called repeatedly without changing the output", 
            "[the first call] ", f,
            "[the second call]", f,
            as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B, typename C>
    bool determinism(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return equality(
            adapter,
            f_name + " must be able to be called repeatedly without changing the output", 
            "[the first call] ",f,
            "[the second call]",f,
            as, bs, cs);
    }

    template<typename Adapter, typename F, typename E, typename A>
    bool left_identity(const Adapter& adapter, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must have a \"left identity\" value, " + e_name + " [denoted \"e\"], that when passed on the left will always return the other value", 
            "f(e,a)",[=](A a){ return f(e, a); },
            "a     ",[=](A a){ return a; },
            as);
    }

    template<typename Adapter, typename F, typename E, typename A>
    bool right_identity(const Adapter& adapter, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must have a \"right identity\" value, " + e_name + " [denoted \"e\"], that when passed on the right will always return the other value", 
            "f(a,e)", [=](A a){ return f(a, e); },
            "f(a)  ", [=](A a){ return a; },
            as);
    }

    template<typename Adapter, typename F, typename E, typename A>
    bool identity(const Adapter& adapter, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return 
            left_identity (adapter, e_name, e, f_name, f, as) &&
            right_identity(adapter, e_name, e, f_name, f, as);
    }

    // template<typename Adapter, typename F, typename E, typename A>
    // bool identity(const Adapter& adapter, 
    //     const std::string e_name, const E& e,
    //     const std::string f_name, const F& f 
    // ) {
    //     return equality(
    //         adapter,
    //         f_name + " [denoted \"f\"] must have an \"identity\" value, " + e_name + "[denoted \"e\"], that when passed will always return itself", 
    //         "f(e,a)",[=](A a){ return f(e, a); },
    //         "a     ",[=](A a){ return a; },
    //         std::vector<E>{e});
    // }

    template<typename Adapter, typename F, typename A, typename Z>
    bool left_annihilation(const Adapter& adapter, 
        const std::string z_name, const Z& z, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must have a \"left annihilator\", " + z_name + " [denoted \"z\"], that when passed on the left will always return itself", 
            "f(z,a)", [=](A a){ return f(z, a); },
            "z     ", [=](A a){ return z; },
            as);
    }

    template<typename Adapter, typename F, typename A, typename Z>
    bool right_annihilation(const Adapter& adapter, 
        const std::string z_name, const Z& z, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must have an \"right annihilator\", " + z_name + " [denoted \"z\"], that when passed on the right will always return itself", 
            "f(a,z)", [=](A a){ return f(a, z);  },
            "z     ", [=](A a){ return z; },
            as);
    }

    template<typename Adapter, typename F, typename A, typename Z>
    bool annihilation(const Adapter& adapter, 
        const std::string z_name, const Z& z, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return 
            left_annihilation (adapter, z_name, z, f_name, f, as) &&
            right_annihilation(adapter, z_name, z, f_name, f, as);
    }

    template<typename Adapter, typename F, typename Finv, typename A>
    bool left_invertibility(const Adapter& adapter, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must permit input to be reconstructed by a \"left inverse\" function, " + finv_name + " [denoted \"f⁻¹\"]", 
            "f⁻¹(f(a))", [=](A a){ return finv(f(a)); },
            "a        ", [=](A a){ return a; },
            as);
    }

    template<typename Adapter, typename F, typename G, typename A>
    bool invertibility(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const many<A>& as
    ) {
        return 
            left_invertibility(adapter, f_name, f, f_name, g, as) &&
            left_invertibility(adapter, g_name, g, g_name, f, as);
    }

    template<typename Adapter, typename E, typename Finv, typename F, typename A>
    bool left_invertibility(const Adapter& adapter, 
        const std::string e_name,    const E& e, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must permit input to be reconstructed by a \"left inverse\" function, " + finv_name + " [denoted \"f⁻¹\"]", 
            "f(f⁻¹(e,a),a)", [=](A a){ return f(finv(e,a), a); },
            "e            ", [=](A a){ return e; },
            as);
    }

    template<typename Adapter, typename E, typename Finv, typename F, typename A>
    bool right_invertibility(const Adapter& adapter, 
        const std::string e_name,    const E& e, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must permit input to be reconstructed by a \"right inverse\" function, " + finv_name + " [denoted \"f⁻¹\"]", 
            "f(a, f⁻¹(e,a))", [=](A a){ return f(a, finv(e,a)); },
            "e             ", [=](A a){ return e; },
            as);
    }

    template<typename Adapter, typename E, typename Finv, typename F, typename A>
    bool invertibility(const Adapter& adapter, 
        const std::string e_name,    const E& e, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return 
            left_invertibility (adapter, e_name, e, finv_name, finv, f_name, f, as) &&
            right_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as);
    }

    template<typename Adapter, typename F, typename A, typename B, typename C>
    bool associativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must allow invocations to be calculated in any order without changing results", 
            "f(f(a,b), c)", [=](A a, B b, C c){ return f(f(a,b), c); },
            "f(a, f(b,c))", [=](A a, B b, C c){ return f(a, f(b,c)); },
            as, bs, cs);
    }

    template<typename Adapter, typename F2, typename F3, typename A, typename B, typename C>
    bool left_associativity(const Adapter& adapter, 
        const std::string f2_name, const F2& f2, 
        const std::string f3_name, const F3& f3, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return equality(
            adapter,
            f3_name + " [denoted \"f\"] must calculate starting from the leftmost parameters", 
            "f(f(a,b), c)", [=](A a, B b, C c){ return f2(f2(a, b), c); },
            "f(  a,b,  c)", [=](A a, B b, C c){ return f3(   a, b,  c); },
            as, bs, cs);
    }

    template<typename Adapter, typename F2, typename F3, typename A, typename B, typename C>
    bool right_associativity(const Adapter& adapter, 
        const std::string f2_name, const F2& f2, 
        const std::string f3_name, const F3& f3, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return equality(
            adapter,
            f3_name + " [denoted \"f\"] must calculate starting from the rightmost parameters", 
            "f(a, f(b,c))", [=](A a, B b, C c){ return f2(a, f2(b, c)); },
            "f(a,   b,c)",  [=](A a, B b, C c){ return f3(a,    b, c);  },
            as, bs, cs);
    }

    template<typename Adapter, typename F, typename A>
    bool power_associativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must allow invocations with a single parameter to be calculated in any order without changing results", 
            "f(f(a,a), a)", [=](A a){ return f(f(a,a), a); },
            "f(a, f(a,a))", [=](A a){ return f(a, f(a,a)); },
            as);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool left_alternativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must under certain conditions allow invocations to be calculated in any order without changing results", 
            "f(f(a,a), b)", [=](A a, B b){ return f(f(a,a), b); },
            "f(a, f(a,b))", [=](A a, B b){ return f(a, f(a,b)); },
            as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool right_alternativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must under certain conditions allow invocations to be calculated in any order without changing results", 
            "f(f(a,b), b)", [=](A a, B b){ return f(f(a,b), b); },
            "f(a, f(b,b))", [=](A a, B b){ return f(a, f(b,b)); },
            as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool alternativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return 
            left_alternativity (adapter, f_name, f, as, bs) &&
            right_alternativity(adapter, f_name, f, as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool flexibility(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must under certain conditions allow invocations to be calculated in any order without changing results", 
            "f(f(a,b), a)", [=](A a, B b){ return f(f(a,b), a); },
            "f(a,f(b, a))", [=](A a, B b){ return f(a,f(b, a)); },
            as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool commutativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must allow arguments to be passed in any order without changing results", 
            "f(a,b)", [=](A a, B b){ return f(a, b); },
            "f(b,a)", [=](A a, B b){ return f(b, a); },
            as, bs);
    }

    template<typename Adapter, typename Invert, typename F, typename A, typename B>
    bool anticommutativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string inv_name, const Invert& inv, 
        const many<A>& as, const many<B>& bs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must return the opposite value if its arguments are flipped, where opposite is determined by "+inv_name+" [denoted \"-\"]", 
            "-f(a,b)", [=](A a, B b){ return invert(f(a, b)); },
            " f(b,a)", [=](A a, B b){ return f(b, a); },
            as, bs);
    }

    template<typename Adapter, typename F, typename G, typename A, typename B, typename C>
    bool left_distributivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must allow left parameters to distribute over another function [denoted \"g\"] while still producing the same results", 
            "f(c, g(a, b))    ", [=](A a, B b, C c){ return f(c, g(a, b));       },
            "g(f(c,a), f(c,b))", [=](A a, B b, C c){ return g(f(c, a), f(c, b)); },
            as, bs, cs);
    }

    template<typename Adapter, typename F, typename G, typename A, typename B, typename C>
    bool right_distributivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must allow right parameters to distribute over another function [denoted \"g\"] while still producing the same results", 
            "f(g(a,b), c)     ", [=](A a, B b, C c){ return f(g(a, b), c);       },
            "g(f(a,c), f(b,c))", [=](A a, B b, C c){ return g(f(a, c), f(b, c)); },
            as, bs, cs);
    }

    template<typename Adapter, typename F, typename G, typename A, typename B, typename C>
    bool distributivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return 
            left_distributivity (adapter, f_name, f, g_name, g, as, bs, cs) &&
            right_distributivity(adapter, f_name, f, g_name, g, as, bs, cs);
    }

    template<typename Adapter, typename F, typename A>
    bool idempotence(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must return input if both parameters are the same", 
            "f(a,a)", [=](A a){ return f(a, a); },
            "a     ", [=](A a){ return a; },
            as);
    }

    template<typename Adapter, typename F, typename E, typename A>
    bool idempotence(const Adapter& adapter, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must return the identity "+ e_name +" [denoted \"e\"] if both parameters are the same", 
            "f(a,a)", [=](A a){ return f(a,a);  },
            "e     ", [=](A a){ return e; },
            as);
    }

    template<typename Adapter, typename F, typename N, typename A>
    bool nilpotence(const Adapter& adapter, 
        const std::string n_name, const N& n, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must return the index "+ n_name +" [denoted \"n\"] if both parameters are the same", 
            "f(a,a)", [=](A a){ return f(a, a);  },
            "n     ", [=](A a){ return n; },
            as);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool involutivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            "left involutivity", 
            f_name + " [denoted \"f\"] must return the original input if invoked a second time", 
            "f(f(a))", [=](A a){ return f(f(a)); },
            "a      ", [=](A a){ return a; },
            as);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool left_involutivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return equality(
            adapter,
            "left involutivity", 
            f_name + " [denoted \"f\"] must return the original input if invoked a second time with the same left parameter", 
            "f(a, f(a,b))", [=](A a, B b){ return f(a, f(a, b)); },
            "b           ", [=](A a, B b){ return b; },
            as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool right_involutivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must return the original input if invoked a second time with the same right parameter", 
            "f(f(a,b), b)", [=](A a, B b){ return f(f(a, b), b); },
            "a           ", [=](A a, B b){ return a; },
            as, bs);
    }

    template<typename Adapter, typename F, typename G, typename A, typename B>
    bool involutivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return 
            left_involutivity (adapter, f_name, f, as, bs) &&
            right_involutivity(adapter, f_name, f, as, bs);
    }

    template<typename Adapter, typename Cross, typename Add, typename N, typename A, typename B, typename C>
    bool jacobi_identity(
        const Adapter& adapter, 
        const std::string n_name, const N& n,
        const std::string cross_name, const Cross& cross, 
        const std::string add_name, const Add& add, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return equality(
            adapter,
            cross_name + " [denoted \"×\"] and "+ add_name +" [denoted \"+\"] must satisfy the jacobi identity for index "+ n_name+ " [denoted \"n\"]", 
            "a×(b×c) + b×(c×a) + c×(a×b)", [=](A a, B b, C c){ return add( cross(a, cross(b, c)), cross(b, cross(c, a)), cross(c, cross(a, b))); },
            "n                          ", [=](A a, B b, C c){ return n; },
            as, bs, cs);
    }

    template<typename Adapter, typename F, typename G, typename A>
    bool invariance(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must be invariant to " + g_name + " [denoted \"g\"]", 
            "f(g(a))", [=](A a){ return f(g(a)); },
            "f(a)   ", [=](A a){ return f(a); },
            as);
    }

    template<typename Adapter, typename F, typename G, typename A>
    bool conservation(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must conserve " + g_name + " [denoted \"g\"]", 
            "g(f(a))", [=](A a){ return g(f(a)); },
            "g(a)   ", [=](A a){ return g(a); },
            as);
    }

    template<typename Adapter, typename F, typename G, typename A>
    bool preservation(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const many<A>& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must preserve " + g_name + " [denoted \"g\"]", 
            "g(f(a))", [=](A a){ return g(f(a)); },
            "g(a)   ", [=](A a){ return g(a); },
            as);
    }





    template<typename Adapter, typename Valid, typename F, typename A>
    bool codomain(const Adapter& adapter, 
        const std::string validity_name, const Valid& valid, 
        const std::string f_name, const F& f, 
        const many<A>& as
    ) {
        return predicate(
            adapter,
            f_name + " must only produce output that is " + validity_name,
            [=](A a){ return valid( f(a) ); }, 
            as);
    }

    template<typename Adapter, typename Valid, typename F, typename A, typename B>
    bool codomain(const Adapter& adapter, 
        const std::string validity_name, const Valid& valid, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs
    ) {
        return predicate(
            adapter,
            f_name + " must only produce output that is " + validity_name, 
            [=](A a, B b){  return valid( f(a, b) ); }, 
            as, bs);
    }

    template<typename Adapter, typename Valid, typename F, typename A, typename B, typename C>
    bool codomain(const Adapter& adapter, 
        const std::string validity_name, const Valid& valid, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return predicate(
            adapter,
            f_name + " must only produce output that is " + validity_name, 
            [=](A a, B b, C c){  return valid( f(a, b, c) ); }, 
            as, bs, cs);
    }

    template<typename Adapter, typename LessThanEqual, typename Add, typename F, typename A, typename B, typename C>
    bool triangle_inequality(const Adapter& adapter, 
        const LessThanEqual& leq, const Add& add, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return predicate(
            adapter,
            f_name + " must satisfy the triangle inequality, so for any set of arguments the smallest sum of invocations using those arguments is a single invocation", 
            [=](A a, B b, C c){ return leq(f(a, c), add(f(a, b), f(b, c))); }, 
            as, bs, cs);
    }





    template<typename Adapter, typename Finv, typename F, typename A, typename B, typename C>
    bool left_cancellative(const Adapter& adapter, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return predicate(
            adapter,
            f_name + " [denoted \"f\"] must imply that two arguments are equal if invoking them both on the left with with the same right argument produces the same result", 
            [=](A a, B b, C c){ return adapter.equal(f(a,b), f(a,c))? adapter.equal(b,c) : true; }, 
            as, bs, cs);
    }

    template<typename Adapter, typename Finv, typename F, typename A, typename B, typename C>
    bool right_cancellative(const Adapter& adapter, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return predicate(
            adapter,
            f_name + " [denoted \"f\"] must imply that two arguments are equal if invoking them both on the right with with the same left argument produces the same result", 
            [=](A a, B b, C c){ return adapter.equal(f(b,a), f(c,a))? adapter.equal(b,c) : true; }, 
            as, bs, cs);
    }

    template<typename Adapter, typename Finv, typename F, typename A, typename B, typename C>
    bool cancellative(const Adapter& adapter, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name, const F& f, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return 
            left_cancellative (adapter, finv_name, finv, f_name, f, as) &&
            right_cancellative(adapter, finv_name, finv, f_name, f, as);
    }



    template<typename Adapter, typename R, typename A, typename B>
    bool reflexivity(const Adapter& adapter, 
        const std::string r_name, const R& r, 
        const many<A>& as
    ) {
        return predicate(
            adapter,
            "reflexivity", 
            [=](A a){ return r(a, a); }, as);
    }

    template<typename Adapter, typename R, typename A, typename B>
    bool irreflexivity(const Adapter& adapter, 
        const std::string r_name, const R& r, 
        const many<A>& as
    ) {
        return predicate(
            adapter,
            "irreflexivity", 
            [=](A a){ return !r(a, a); }, as);
    }

    template<typename Adapter, typename R, typename A, typename B>
    bool symmetry(const Adapter& adapter, 
        const std::string r_name, const R& r, 
        const many<A>& as, const many<B>& bs
    ) {
        return predicate(
            adapter,
            "symmetry", 
            [=](A a, B b){ 
                return 
                    r(a, b) == 
                    r(b, a);
            }, as, bs);
    }

    template<typename Adapter, typename R, typename A, typename B>
    bool antisymmetry(const Adapter& adapter, 
            std::string r_name, const R& r, 
        const many<A>& as, const many<B>& bs
    ) {
        return predicate(
            adapter,
            "antisymmetry", 
            [=](A a, B b){ 
                return 
                    r(a, b) != 
                    r(b, a);
            }, as, bs);
    }

    template<typename Adapter, typename R, typename A, typename B, typename C>
    bool transitivity(const Adapter& adapter, 
            std::string r_name, const R& r, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return predicate(
            adapter,
            "transitivity", 
            [=](A a, B b, C c){ 
                return r(a, b) && r(b, c)? r(a, c) : true;
            }, as, bs, cs);
    }

    template<typename Adapter, typename R, typename A, typename B, typename C>
    bool antitransitivity(const Adapter& adapter, 
            std::string r_name, const R& r, 
        const many<A>& as, const many<B>& bs, const many<C>& cs
    ) {
        return predicate(
            adapter,
            "antitransitivity", 
            [=](A a, B b, C c){ 
                return r(a, b) && r(b, c)? !r(a, c) : true;
            }, as, bs, cs);
    }

    template<typename Adapter, typename R, typename A, typename B>
    bool totality(const Adapter& adapter, 
            std::string r_name, const R& r, 
        const many<A>& as, const many<B>& bs
    ) {
        return predicate(
            adapter,
            "totality", 
            [=](A a, B b){ 
                return r(a, b) || r(b, a);
            }, as, bs);
    }

    template<typename Adapter, typename R, typename A, typename B>
    bool strict_totality(const Adapter& adapter, 
            std::string r_name, const R& r, 
        const many<A>& as, const many<B>& bs
    ) {
        return predicate(
            adapter,
            "strict totality", 
            [=](A a, B b){ 
                return r(a, b) ^ r(b, a);
            }, as, bs);
    }






}

