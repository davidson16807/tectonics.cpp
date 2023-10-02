#pragma once

// std libraries
#include <vector> // std::vector

// in-house libraries
#include "equality.hpp"

namespace test {

    template<typename T>
    using many = std::vector<T>;

    template<typename Adapter, typename A>
    bool discernability(const Adapter& adapter, 
        const A& a
    ) {
        for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t j = 0; j < i; ++j) {
            if (adapter.equal(a[i],a[j])) {
                std::cout << std::endl;
                std::cout << "Test failed:" << std::endl;
                std::cout << "  sample values should be appreciably different and adapter.equal(…) must be able to be detect appreciable differences" << std::endl;
                std::cout << "  a₁ : " << indent(adapter.print(a[i]), "  ") << " [from index "<< i <<"]" << std::endl;
                std::cout << "  a₂ : " << indent(adapter.print(a[j]), "  ") << " [from index "<< j <<"]" << std::endl;
                return false; 
            }
            if (adapter.print(a[i]) == adapter.print(a[j])) {
                std::cout << std::endl;
                std::cout << "Test failed:" << std::endl;
                std::cout << "  sample values should be appreciably different and adapter.print(…) must be able to be depict appreciable differences" << std::endl;
                std::cout << "  a₁ : " << indent(adapter.print(a[i]), "  ") << " [from index "<< i <<"]" << std::endl;
                std::cout << "  a₂ : " << indent(adapter.print(a[j]), "  ") << " [from index "<< j <<"]" << std::endl;
                return false; 
            }
        }}
        return true;
    }

    template<typename Adapter, typename F, typename A>
    bool determinism(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as
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
        const A& as, const B& bs
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
        const A& as, const B& bs, const C& cs
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
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must have a \"left identity\", " + e_name + " [denoted \"e\"], that when passed on the left always returns the right argument" +
            "\nsuch that: \n  f(e,a) = a\n",
            [=](auto a){
                auto fea = f(e,a);
                return Results(adapter.equal(fea, a),
                    "f(e,a) : " + adapter.print(fea)+"\n" +
                    "e : " + adapter.print(e)
                );
            }, as);
    }

    template<typename Adapter, typename F, typename E, typename A>
    bool right_identity(const Adapter& adapter, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must have a \"left identity\", " + e_name + " [denoted \"e\"], that when passed on the right always returns the left argument" +
            "\nsuch that: \n  f(a,e) = a\n",
            [=](auto a){
                auto fae = f(a,e);
                return Results(adapter.equal(fae, a),
                    "f(a,e) : " + adapter.print(fae)+"\n" +
                    "e : " + adapter.print(e)
                );
            }, as);
    }

    template<typename Adapter, typename F, typename E, typename A>
    bool identity(const Adapter& adapter, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const A& as
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
    //         f_name + " [denoted \"f\"] must have an \"identity\", " + e_name + "[denoted \"e\"], that when passed will always return itself", 
    //         "f(e,a)",[=](auto a){ return f(e, a); },
    //         "a     ",[=](auto a){ return a; },
    //         std::vector<E>{e});
    // }

    template<typename Adapter, typename F, typename A, typename Z>
    bool left_annihilation(const Adapter& adapter, 
        const std::string z_name, const Z& z, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must have a \"left annihilator\", " + z_name + " [denoted \"z\"], that when passed on the left will always return itself" + 
            "\nsuch that: \n  f(a,z) = a\n",
            [=](auto a){
                auto fza = f(z,a);
                return Results(adapter.equal(fza, a),
                    "f(a,z) : " + adapter.print(fza)+"\n" +
                    "z : " + adapter.print(z)
                );
            }, as);
    }

    template<typename Adapter, typename F, typename A, typename Z>
    bool right_annihilation(const Adapter& adapter, 
        const std::string z_name, const Z& z, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must have an \"right annihilator\", " + z_name + " [denoted \"z\"], that when passed on the right will always return itself" + 
            "\nsuch that: \n  f(a,z) = a\n",
            [=](auto a){
                auto faz = f(a,z);
                return Results(adapter.equal(faz, a),
                    "f(a,z) : " + adapter.print(faz)+"\n" +
                    "z : " + adapter.print(z)
                );
            }, as);
    }

    template<typename Adapter, typename F, typename A, typename Z>
    bool annihilation(const Adapter& adapter, 
        const std::string z_name, const Z& z, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return 
            left_annihilation (adapter, z_name, z, f_name, f, as) &&
            right_annihilation(adapter, z_name, z, f_name, f, as);
    }

    template<typename Adapter, typename F, typename Finv, typename A>
    bool left_invertibility(const Adapter& adapter, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must permit input to be reconstructed by a \"left inverse\" function, " + finv_name + " [denoted \"f⁻¹\"]" +
            "\nsuch that: \n  f⁻¹(f(a)) = a\n",
            [=](auto a){
                auto fa = f(a);
                auto finvfa = finv(fa);
                return Results(adapter.equal(finvfa, a),
                    "f⁻¹(f(a)) : " + adapter.print(finvfa)+"\n" +
                    "f(a) : " + adapter.print(fa)
                );
            }, as);
    }

    template<typename Adapter, typename F, typename G, typename A>
    bool invertibility(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const A& as
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
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must permit input to be reconstructed by a \"left inverse\" function, " + finv_name + " [denoted \"f⁻¹\"]" +
            "\nsuch that: \n  f(f⁻¹(e,a), a) = e\n",
            [=](auto a){
                auto finvea = finv(e,a);
                auto ffinveaa = f(finvea, a);
                return Results(adapter.equal(ffinveaa, e),
                    "f(f⁻¹(e,a),a) : " + adapter.print(ffinveaa)+"\n" +
                    "f⁻¹(e,a) : " + adapter.print(finvea)+"\n" +
                    "e : " + adapter.print(e)
                );
            }, as);
    }

    template<typename Adapter, typename E, typename Finv, typename F, typename A>
    bool right_invertibility(const Adapter& adapter, 
        const std::string e_name,    const E& e, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must permit input to be reconstructed by a \"right inverse\" function, " + finv_name + " [denoted \"f⁻¹\"]" +
            "\nsuch that: \n  f(a, f⁻¹(e,a)) = e\n",
            [=](auto a){
                auto finvea = finv(e,a);
                auto fafinvea = f(a, finvea);
                return Results(adapter.equal(fafinvea, e),
                    "f(a,f⁻¹(e,a)) : " + adapter.print(fafinvea)+"\n" +
                    "f⁻¹(e,a) : " + adapter.print(finvea)+"\n" +
                    "e : " + adapter.print(e)
                );
            }, as);
    }

    template<typename Adapter, typename E, typename Finv, typename F, typename A>
    bool invertibility(const Adapter& adapter, 
        const std::string e_name,    const E& e, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return 
            left_invertibility (adapter, e_name, e, finv_name, finv, f_name, f, as) &&
            right_invertibility(adapter, e_name, e, finv_name, finv, f_name, f, as);
    }

    template<typename Adapter, typename F, typename A, typename B, typename C>
    bool associativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must allow invocations to be calculated in any order without changing results"+
            "\nsuch that: \n  f(f(a,b), c) = f(a, f(b,c))\n",
            [=](auto a, auto b, auto c){
                auto fab = f(a,b);
                auto fbc = f(b,c);
                auto ffab_c = f(f(a,b), c);
                auto fa_fbc = f(a, f(b,c));
                return Results(adapter.equal(ffab_c, fa_fbc),
                    "f(f(a,b), c) : " + adapter.print(ffab_c) + "\n" +
                    "f(a, f(b,c)) : " + adapter.print(fa_fbc) + "\n" +
                    "f(a,b) : " + adapter.print(fab) + "\n" +
                    "f(b,c) : " + adapter.print(fbc) + "\n");
            }, as, bs, cs);
    }

    template<typename Adapter, typename F2, typename F3, typename A, typename B, typename C>
    bool left_associativity(const Adapter& adapter, 
        const std::string f2_name, const F2& f2, 
        const std::string f3_name, const F3& f3, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(adapter, 
            f3_name + " [denoted \"f\"] must calculate starting from the leftmost parameters using "+f2_name+ 
            "\nsuch that: \n  f(f(a,b), c) = f(a,b,c)\n",
            [=](auto a, auto b, auto c){
                auto fab = f2(a,b);
                auto ffab_c = f2(fab, c);
                auto fabc = f3(a,b,c);
                return Results(adapter.equal(ffab_c, fabc),
                    "f(a, b, c) : " + adapter.print(fabc) + "\n" +
                    "f(f(a,b), c) : " + adapter.print(ffab_c) + "\n" +
                    "f(a,b) : " + adapter.print(fab) + "\n"
                );
            }, as, bs, cs);
    }

    template<typename Adapter, typename F2, typename F3, typename A, typename B, typename C>
    bool right_associativity(const Adapter& adapter, 
        const std::string f2_name, const F2& f2, 
        const std::string f3_name, const F3& f3, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(adapter, 
            f3_name + " [denoted \"f\"] must calculate starting from the rightmost parameters using "+f2_name+ 
            "\nsuch that: \n  f(a,f(b,c)) = f(a,b,c)\n",
            [=](auto a, auto b, auto c){
                auto fbc = f2(b,c);
                auto fa_fbc = f2(a, fbc);
                auto fabc = f3(a,b,c);
                return Results(adapter.equal(fa_fbc, fabc),
                    "f(a, b, c) : " + adapter.print(fabc) + "\n" +
                    "f(a,f(b,c)) : " + adapter.print(fa_fbc) + "\n" +
                    "f(b,c) : " + adapter.print(fbc) + "\n"
                );
            }, as, bs, cs);
    }

    template<typename Adapter, typename F, typename A>
    bool power_associativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must allow invocations with a single parameter to be calculated in any order without changing results" + 
            "\nsuch that: \n  f(f(a,a), a) = f(a, f(a,a))\n",
            [=](auto a){
                auto faa = f(a,a);
                auto fa_faa = f(a, faa);
                auto ffaa_a = f(f(a,a),a);
                return Results(adapter.equal(fa_faa, ffaa_a),
                    "f(a, f(a,a)) : " + adapter.print(fa_faa) + "\n" +
                    "f(f(a,a), a) : " + adapter.print(ffaa_a) + "\n" +
                    "f(a,a) : " + adapter.print(faa) + "\n"
                );
            }, as);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool left_alternativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must under certain conditions allow invocations to be calculated in any order without changing results" + 
            "\nsuch that: \n  f(f(a,a), b) = f(a, f(a,b))\n",
            [=](auto a, auto b, auto c){
                auto fab = f(a,b);
                auto faa = f(a,a);
                auto fa_fab = f(a, fab);
                auto ffaa_b = f(faa,b);
                return Results(adapter.equal(fa_fab, ffaa_b),
                    "f(a, f(a,b)) : " + adapter.print(fa_fab) + "\n" +
                    "f(f(a,a), b) : " + adapter.print(ffaa_b) + "\n" +
                    "f(a,a) : " + adapter.print(faa) + "\n"
                    "f(a,b) : " + adapter.print(fab) + "\n"
                );
            }, as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool right_alternativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must under certain conditions allow invocations to be calculated in any order without changing results" + 
            "\nsuch that: \n  f(f(a,b), b) = f(a, f(b,b))\n",
            [=](auto a, auto b){
                auto fab = f(a,b);
                auto fbb = f(b,b);
                auto fa_fbb = f(a, fbb);
                auto ffab_b = f(fab,b);
                return Results(adapter.equal(fa_fbb, ffab_b),
                    "f(a, f(b,b)) : " + adapter.print(fa_fbb) + "\n" +
                    "f(f(a,b), b) : " + adapter.print(ffab_b) + "\n" +
                    "f(a,b) : " + adapter.print(fab) + "\n"
                    "f(b,b) : " + adapter.print(fbb) + "\n"
                );
            }, as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool alternativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs
    ) {
        return 
            left_alternativity (adapter, f_name, f, as, bs) &&
            right_alternativity(adapter, f_name, f, as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool flexibility(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must under certain conditions allow invocations to be calculated in any order without changing results" + 
            "\nsuch that: \n  f(f(a,b), a) = f(a, f(b,a))\n",
            [=](auto a, auto b){
                auto fab = f(a,b);
                auto fba = f(b,a);
                auto fa_fba = f(a, fba);
                auto ffab_a = f(fab,a);
                return Results(adapter.equal(fa_fba, ffab_a),
                    "f(a, f(b,a)) : " + adapter.print(fa_fba) + "\n" +
                    "f(f(a,b), a) : " + adapter.print(ffab_a) + "\n" +
                    "f(a,b) : " + adapter.print(fab) + "\n" +
                    "f(b,a) : " + adapter.print(fba) + "\n"
                );
            }, as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool commutativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must allow arguments to be passed in any order without changing results", 
            "f(a,b)", [=](auto a, auto b){ return f(a, b); },
            "f(b,a)", [=](auto a, auto b){ return f(b, a); },
            as, bs);
    }

    template<typename Adapter, typename Invert, typename F, typename A, typename B>
    bool anticommutativity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string inv_name, const Invert& inv, 
        const A& as, const B& bs
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must return the opposite value if its arguments are flipped, where opposite is determined by "+inv_name+" [denoted \"-\"]", 
            "-f(a,b)", [=](auto a, auto b){ return inv(f(a, b)); },
            " f(b,a)", [=](auto a, auto b){ return f(b, a); },
            as, bs);
    }

    template<typename Adapter, typename F, typename G, typename A, typename B, typename C>
    bool left_distributivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must allow left parameters to distribute over another function [denoted \"g\"] while still producing the same results" + 
            "\nsuch that: \n  f(c, g(a, b)) = g(f(c,a), f(c,b))\n",
            [=](auto a, auto b, auto c){
                auto gab = g(a,b);
                auto fca = f(c,a);
                auto fcb = f(c,b);
                auto f_c_gab = f(c, gab);
                auto g_fca_fcb = g(fca,fcb);
                return Results(adapter.equal(f_c_gab, g_fca_fcb),
                    "f(c, g(a, b))     : " + adapter.print(f_c_gab) + "\n" +
                    "g(f(c,a), f(c,b)) : " + adapter.print(g_fca_fcb) + "\n" +
                    "g(a,b) : " + adapter.print(gab) + "\n" +
                    "f(c,a) : " + adapter.print(fca) + "\n" +
                    "f(c,b) : " + adapter.print(fcb) + "\n"
                );
            }, as, bs, cs);
    }

    template<typename Adapter, typename F, typename G, typename A, typename B, typename C>
    bool right_distributivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must allow right parameters to distribute over another function [denoted \"g\"] while still producing the same results" + 
            "\nsuch that: \n  f(g(a,b), c) = g(f(a,c), f(b,c))\n",
            [=](auto a, auto b, auto c){
                auto gab = g(a,b);
                auto fac = f(a,c);
                auto fbc = f(b,c);
                auto f_gab_c = f(c, gab);
                auto g_fac_fbc = g(fac,fbc);
                return Results(adapter.equal(f_gab_c, g_fac_fbc),
                    "f(g(a,b), c)     : " + adapter.print(f_gab_c) + "\n" +
                    "g(f(a,c), f(b,c)) : " + adapter.print(g_fac_fbc) + "\n" +
                    "g(a,b) : " + adapter.print(gab) + "\n" +
                    "f(a,c) : " + adapter.print(fac) + "\n" +
                    "f(b,c) : " + adapter.print(fbc) + "\n"
                );
            }, as, bs, cs);
    }

    template<typename Adapter, typename F, typename G, typename A, typename B, typename C>
    bool distributivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const A& as, const B& bs, const C& cs
    ) {
        return 
            left_distributivity (adapter, f_name, f, g_name, g, as, bs, cs) &&
            right_distributivity(adapter, f_name, f, g_name, g, as, bs, cs);
    }



    template<typename Adapter, typename F, typename Add1, typename Add2, typename A, typename B>
    bool additivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string add1_name, const Add1& add1, 
        const std::string add2_name, const Add2& add2, 
        const A& as, const B& bs
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must allow the output of a sum, using \""+add1_name+"\" to be used in place of the the sum of the output, using \"" + add2_name + "\"" +
            "\nsuch that: \n  f(a+b) = f(a)+f(b)\n",
            [=](auto a, auto b){
                auto ab = add1(a,b);
                auto fab = f(ab);
                auto fa = f(a);
                auto fb = f(b);
                auto fa_fb = add2(fa,fb);
                return Results(adapter.equal(fab, fa_fb),
                    "f(a+b)    : " + adapter.print(fab) + "\n" +
                    "f(a)+f(b) : " + adapter.print(fa_fb) + "\n" +
                    "a+b  : " + adapter.print(ab) + "\n" +
                    "f(a) : " + adapter.print(fa) + "\n" +
                    "f(b) : " + adapter.print(fb) + "\n"
                );
            }, as, bs);
    }

    template<typename Adapter, typename F, typename Mult1, typename Mult2, typename A, typename B>
    bool homogeneity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string mult1_name, const Mult1& mult1, 
        const std::string mult2_name, const Mult2& mult2, 
        const A& as, const B& bs
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must allow the output of a sum (using "+mult1_name+") to be used in place of the the sum of the output (using " + mult2_name + ")" +
            "\nsuch that: \n  f(a*b) = a*f(b)\n",
            [=](auto a, auto b){
                auto ab = mult1(a,b);
                auto fab = f(ab);
                auto fb = f(b);
                auto afb = mult2(a,fb);
                return Results(adapter.equal(fab, afb),
                    "f(a*b) : " + adapter.print(fab) + "\n" +
                    "a*f(b) : " + adapter.print(afb) + "\n" +
                    "a*b  : " + adapter.print(ab) + "\n" +
                    "f(b) : " + adapter.print(fb) + "\n"
                );
            }, as, bs);
    }


    template<typename Adapter, typename F, typename G, typename FG, typename A>
    bool composition(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const std::string fg_name, const FG& fg, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] after "+g_name+" [denoted \"g\"] must be equivalent to " + fg_name + "[denoted \"fg\"]"
            "\nsuch that: \n  f(g(a)) = fg(a)\n",
            [=](auto a){
                auto ga = g(a);
                auto f_ga = f(ga);
                auto fg_a = fg(a);
                return Results(adapter.equal(f_ga, fg_a),
                    "f(g(a)) : " + adapter.print(f_ga) + "\n" +
                    "fg(a) : " + adapter.print(fg_a) + "\n" +
                    "g(a) : " + adapter.print(ga) + "\n"
                );
            }, as);
    }


    template<typename Adapter, typename F, typename A>
    bool unary_idempotence(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must return the same output if invoked a second time" + 
            "\nsuch that: \n  f(f(a)) = f(a)\n",
            [=](auto a){
                auto fa = f(a);
                auto ffa = f(fa);
                return Results(adapter.equal(ffa, fa),
                    "f(a)  : " + adapter.print(fa) + "\n" +
                    "f(f(a)) : " + adapter.print(ffa) + "\n"
                );
            }, as);
    }

    template<typename Adapter, typename F, typename A>
    bool binary_idempotence(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must return input if both parameters are the same", 
            "f(a,a)", [=](auto a){ return f(a, a); },
            "a     ", [=](auto a){ return a; },
            as);
    }

    template<typename Adapter, typename F, typename E, typename A>
    bool binary_idempotence(const Adapter& adapter, 
        const std::string e_name, const E& e, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must return the identity "+ e_name +" [denoted \"e\"] if both parameters are the same", 
            "f(a,a)", [=](auto a){ return f(a,a);  },
            "e     ", [=](auto a){ return e; },
            as);
    }

    template<typename Adapter, typename F, typename N, typename A>
    bool nilpotence(const Adapter& adapter, 
        const std::string n_name, const N& n, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return equality(
            adapter,
            f_name + " [denoted \"f\"] must return the index "+ n_name +" [denoted \"n\"] if both parameters are the same", 
            "f(a,a)", [=](auto a){ return f(a, a);  },
            "n     ", [=](auto a){ return n; },
            as);
    }

    template<typename Adapter, typename F, typename A>
    bool involutivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must return the original input if invoked a second time" + 
            "\nsuch that: \n  f(f(a)) = a\n",
            [=](auto a){
                auto fa = f(a);
                auto ffa = f(fa);
                return Results(adapter.equal(ffa, a),
                    "f(a)  : " + adapter.print(fa) + "\n" +
                    "f(f(a)) : " + adapter.print(ffa) + "\n"
                );
            }, as);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool left_involutivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must return the original input if invoked a second time with the same left parameter" + 
            "\nsuch that: \n  f(a, f(a,b)) = b\n",
            [=](auto a, auto b){
                auto fab = f(a,b);
                auto fafab = f(a,fab);
                return Results(adapter.equal(fafab, b),
                    "f(a,b)  : " + adapter.print(fab) + "\n" +
                    "f(a, f(a,b)) : " + adapter.print(fafab) + "\n"
                );
            }, as, bs);
    }

    template<typename Adapter, typename F, typename A, typename B>
    bool right_involutivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must return the original input if invoked a second time with the same left parameter" + 
            "\nsuch that: \n  f(f(a,b), b) = a\n",
            [=](auto a, auto b){
                auto fab = f(a,b);
                auto ffabb = f(a,fab);
                return Results(adapter.equal(ffabb, b),
                    "f(a,b)  : " + adapter.print(fab) + "\n" +
                    "f(f(a,b), b) : " + adapter.print(ffabb) + "\n"
                );
            }, as, bs);
    }

    template<typename Adapter, typename F, typename G, typename A, typename B>
    bool involutivity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs
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
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(adapter, 
            cross_name + " [denoted \"×\"] and "+ add_name +" [denoted \"+\"] must satisfy the jacobi identity for index "+ n_name+ " [denoted \"n\"]" + 
            "\nsuch that: \n  a×(b×c) + b×(c×a) + c×(a×b) = n\n",
            [=](auto a, auto b, auto c){
                auto ab = cross(a,b);
                auto bc = cross(b,c);
                auto ca = cross(c,a);
                auto a_bc = cross(a,bc);
                auto b_ca = cross(b,ca);
                auto c_ab = cross(c,ab);
                auto abc_bca = add(a_bc,b_ca);
                auto abc_bca_cab = add(abc_bca, c_ab);
                return Results(adapter.equal(abc_bca_cab, n),
                    "a×b:                     " + adapter.print(ab) + "\n" +
                    "b×c:                     " + adapter.print(bc) + "\n" +
                    "c×a:                     " + adapter.print(ca) + "\n" +
                    "a×(b×c):                 " + adapter.print(a_bc) + "\n" +
                    "b×(c×a):                 " + adapter.print(b_ca) + "\n" +
                    "c×(a×b):                 " + adapter.print(c_ab) + "\n" +
                    "a×(b×c)+b×(c×a):         " + adapter.print(abc_bca) + "\n" +
                    "a×(b×c)+b×(c×a)+c×(a×b): " + adapter.print(abc_bca_cab) + "\n" +
                    "n:           " + adapter.print(n) + "\n"
                );
            }, as, bs, cs);
    }

    /* 
    NOTE: the following functions (invariance, continuity, conservation, preservation, congruence) effectively test for the same thing,
    however some differ in how they are invoked, and some differ in how they should most effectively communicate the concept to the user,
    so in either case we implement different functions that vary only in how they present information to the user.
    */

    template<typename Adapter, typename F, typename G, typename A>
    bool invariance(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must be invariant to " + g_name + " [denoted \"g\"]" + 
            "\nsuch that: \n  f(g(a)) = f(a)\n",
            [=](auto a){
                auto fa = f(a);
                auto ga = g(a);
                auto fga = f(ga);
                return Results(adapter.equal(fga, fa),
                    "f(a)  : " + adapter.print(fa) + "\n" +
                    "g(a)  : " + adapter.print(ga) + "\n" +
                    "f(g(a)) : " + adapter.print(fga) + "\n"
                );
            }, as);
    }

    template<typename Adapter, typename F, typename G, typename A>
    bool continuity(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must be continuous so that imperceptible changes to input [denoted \"g\"] will produce imperceptible changes to output" + 
            "\nsuch that: \n  f(g(a)) = f(a)\n",
            [=](auto a){
                auto fa = f(a);
                auto ga = g(a);
                auto fga = f(ga);
                return Results(adapter.equal(fga, fa),
                    "f(a)  : " + adapter.print(fa) + "\n" +
                    "g(a)  : " + adapter.print(ga) + "\n" +
                    "f(g(a)) : " + adapter.print(fga) + "\n"
                );
            }, as);
    }

    template<typename Adapter, typename F, typename G, typename A>
    bool conservation(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must conserve " + g_name + " [denoted \"g\"]" + 
            "\nsuch that: \n  f(g(a)) = f(a)\n",
            [=](auto a){
                auto fa = f(a);
                auto ga = g(a);
                auto fga = f(ga);
                return Results(adapter.equal(fga, fa),
                    "f(a)  : " + adapter.print(fa) + "\n" +
                    "g(a)  : " + adapter.print(ga) + "\n" +
                    "f(g(a)) : " + adapter.print(fga) + "\n"
                );
            }, as);
    }

    template<typename Adapter, typename F, typename G, typename A>
    bool preservation(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must preserve " + g_name + " [denoted \"g\"]" + 
            "\nsuch that: \n  f(g(a)) = f(a)\n",
            [=](auto a){
                auto fa = f(a);
                auto ga = g(a);
                auto fga = f(ga);
                return Results(adapter.equal(fga, fa),
                    "f(a)  : " + adapter.print(fa) + "\n" +
                    "g(a)  : " + adapter.print(ga) + "\n" +
                    "f(g(a)) : " + adapter.print(fga) + "\n"
                );
            }, as);
    }

    template<typename Adapter, typename F, typename G, typename A>
    bool congruence(const Adapter& adapter, 
        const std::string f_name, const F& f, 
        const std::string g_name, const G& g, 
        const A& as
    ) {
        return predicate(adapter, 
            f_name + " [denoted \"f\"] must be congruent over " + g_name + " [denoted \"g\"]" + 
            "\nsuch that: \n  f(g(a)) = f(a)\n",
            [=](auto a){
                auto fa = f(a);
                auto ga = g(a);
                auto fga = f(ga);
                return Results(adapter.equal(fga, fa),
                    "f(a)  : " + adapter.print(fa) + "\n" +
                    "g(a)  : " + adapter.print(ga) + "\n" +
                    "f(g(a)) : " + adapter.print(fga) + "\n"
                );
            }, as);
    }





    template<typename Adapter, typename Valid, typename F, typename A>
    bool codomain(const Adapter& adapter, 
        const std::string validity_name, const Valid& valid, 
        const std::string f_name, const F& f, 
        const A& as
    ) {
        return predicate(
            adapter,
            f_name + " must only produce output that is " + validity_name,
            [=](auto a){
                auto fa = f(a);
                std::string diagnostics = "f(a) : " + adapter.print(fa);
                return Results(valid(fa), diagnostics); }, 
            as);
    }

    template<typename Adapter, typename Valid, typename F, typename A, typename B>
    bool codomain(const Adapter& adapter, 
        const std::string validity_name, const Valid& valid, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs
    ) {
        return predicate(
            adapter,
            f_name + " must only produce output that is " + validity_name, 
            [=](auto a, auto b){
                auto fab = f(a,b);
                std::string diagnostics = "f(a,b) : " + adapter.print(fab);
                return Results(valid(fab), diagnostics); }, 
            as, bs);
    }

    template<typename Adapter, typename Valid, typename F, typename A, typename B, typename C>
    bool codomain(const Adapter& adapter, 
        const std::string validity_name, const Valid& valid, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(
            adapter,
            f_name + " must only produce output that is " + validity_name, 
            [=](auto a, auto b, auto c){
                auto fabc = f(a,b,c);
                std::string diagnostics = "f(a,b,c) : " + adapter.print(fabc);
                return Results(valid(fabc), diagnostics); }, 
            as, bs, cs);
    }




    template<typename Adapter, typename LessThanEqual, typename Add, typename F, typename A, typename B, typename C>
    bool triangle_inequality(const Adapter& adapter, 
        const LessThanEqual& leq, const Add& add, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(
            adapter,
            f_name + " must satisfy the triangle inequality, so for any pair of arguments the smallest sum of invocations that use those arguments is a single invocation", 
            [=](auto a, auto b, auto c){
                auto fac = f(a,c);
                auto fab = f(a,b);
                auto fbc = f(b,c);
                auto fab_fac = add(fab, fac);
                std::string diagnostics;
                diagnostics += "such that: \n  f(a,c) ≤ f(a,b)+f(a,c)\n";
                diagnostics += "f(a,c) : " + adapter.print(fac) + "\n";
                diagnostics += "f(a,b)+f(a,c) : " + adapter.print(fab_fac) + "\n";
                diagnostics += "f(a,b) : " + adapter.print(fab) + "\n";
                diagnostics += "f(b,c) : " + adapter.print(fbc) + "\n";
                return Results(leq(fac, fab_fac), diagnostics); 
            }, 
            as, bs, cs);
    }





    template<typename Adapter, typename Finv, typename F, typename A, typename B, typename C>
    bool left_cancellative(const Adapter& adapter, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(
            adapter,
            f_name + " [denoted \"f\"] must imply that two arguments are equal if invoking them both on the left with with the same right argument produces the same result", 
            [=](auto a, auto b, auto c){
                auto fac = f(a,c);
                auto fab = f(a,b);
                std::string diagnostics;
                diagnostics += "such that: \n  f(a,b)=f(a,c) implies b=c \n";
                diagnostics += "f(a,c) : " + adapter.print(fac) + "\n";
                diagnostics += "f(a,b) : " + adapter.print(fab) + "\n";
                return Results(adapter.equal(f(a,b), f(a,c))? adapter.equal(b,c) : true, diagnostics); 
            }, 
            as, bs, cs);
    }

    template<typename Adapter, typename Finv, typename F, typename A, typename B, typename C>
    bool right_cancellative(const Adapter& adapter, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name,    const F& f, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(
            adapter,
            f_name + " [denoted \"f\"] must imply that two arguments are equal if invoking them both on the right with with the same left argument produces the same result", 
            [=](auto a, auto b, auto c){
                auto fca = f(c,a);
                auto fba = f(b,a);
                std::string diagnostics;
                diagnostics += "such that: \n  f(b,a)=f(c,a) implies b=c \n";
                diagnostics += "f(c,a) : " + adapter.print(fca) + "\n";
                diagnostics += "f(b,a) : " + adapter.print(fba) + "\n";
                return Results(adapter.equal(f(a,b), f(a,c))? adapter.equal(b,c) : true, diagnostics); 
            }, 
            as, bs, cs);
    }

    template<typename Adapter, typename Finv, typename F, typename A, typename B, typename C>
    bool cancellative(const Adapter& adapter, 
        const std::string finv_name, const Finv& finv, 
        const std::string f_name, const F& f, 
        const A& as, const B& bs, const C& cs
    ) {
        return 
            left_cancellative (adapter, finv_name, finv, f_name, f, as) &&
            right_cancellative(adapter, finv_name, finv, f_name, f, as);
    }



    template<typename Adapter, typename R, typename A, typename B>
    bool reflexivity(const Adapter& adapter, 
        const std::string r_name, const R& r, 
        const A& as
    ) {
        return predicate(
            adapter,
            "reflexivity", 
            [=](auto a){
                return Results(r(a, a), "such that: \n  r(a,a) for each a \n"); 
            }, as);
    }

    template<typename Adapter, typename R, typename A, typename B>
    bool irreflexivity(const Adapter& adapter, 
        const std::string r_name, const R& r, 
        const A& as
    ) {
        return predicate(
            adapter,
            "irreflexivity", 
            [=](auto a){
                return Results(!r(a, a), "such that: \n  r(a,a) for each a \n");
            }, as);
    }

    template<typename Adapter, typename R, typename A, typename B>
    bool symmetry(const Adapter& adapter, 
        const std::string r_name, const R& r, 
        const A& as, const B& bs
    ) {
        return predicate(
            adapter,
            "symmetry", 
            [=](auto a, auto b){
                auto rab = r(a, b);
                auto rba = r(b, a);
                std::string diagnostics;
                diagnostics += "such that: \n  r(a,b) = r(b,a)\n";
                diagnostics += "r(a,b) : " + adapter.print(rab) + "\n";
                diagnostics += "r(b,a) : " + adapter.print(rba) + "\n";
                return Results(rab==rba, diagnostics);
            }, as, bs);
    }

    template<typename Adapter, typename R, typename A, typename B>
    bool antisymmetry(const Adapter& adapter, 
            std::string r_name, const R& r, 
        const A& as, const B& bs
    ) {
        return predicate(
            adapter,
            "antisymmetry", 
            [=](auto a, auto b){
                auto rab = r(a, b);
                auto rba = r(b, a);
                std::string diagnostics;
                diagnostics += "such that: \n  r(a,b) = r(b,a)\n";
                diagnostics += "r(a,b) : " + adapter.print(rab) + "\n";
                diagnostics += "r(b,a) : " + adapter.print(rba) + "\n";
                return Results(rab!=rba, diagnostics);
            }, as, bs);
    }

    template<typename Adapter, typename R, typename A, typename B, typename C>
    bool transitivity(const Adapter& adapter, 
            std::string r_name, const R& r, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(
            adapter,
            "transitivity", 
            [=](auto a, auto b, auto c){
                auto rab = r(a, b);
                auto rba = r(b, a);
                auto rbc = r(b, c);
                auto rac = r(a, c);
                std::string diagnostics;
                diagnostics += "such that: \n  r(a,b) ∧ r(b,a) implies r(a,c)\n";
                diagnostics += "r(a,b) : " + adapter.print(rab) + "\n";
                diagnostics += "r(b,a) : " + adapter.print(rba) + "\n";
                return Results(rab&&rbc?rac:true, diagnostics); 
            }, as, bs, cs);
    }

    template<typename Adapter, typename R, typename A, typename B, typename C>
    bool antitransitivity(const Adapter& adapter, 
            std::string r_name, const R& r, 
        const A& as, const B& bs, const C& cs
    ) {
        return predicate(
            adapter,
            "antitransitivity", 
            [=](auto a, auto b, auto c){
                auto rab = r(a, b);
                auto rbc = r(b, c);
                auto rac = r(a, c);
                std::string diagnostics;
                diagnostics += "such that: \n  r(a,b) ∧ r(b,c) implies ¬r(a,c)\n";
                diagnostics += "r(a,b) : " + adapter.print(rab) + "\n";
                diagnostics += "r(b,c) : " + adapter.print(rbc) + "\n";
                diagnostics += "r(a,c) : " + adapter.print(rbc) + "\n";
                return Results(rab&&rbc?!rac:true, diagnostics); 
            }, as, bs, cs);
    }

    template<typename Adapter, typename R, typename A, typename B>
    bool totality(const Adapter& adapter, 
            std::string r_name, const R& r, 
        const A& as, const B& bs
    ) {
        return predicate(
            adapter,
            "totality", 
            [=](auto a, auto b){
                auto rab = r(a, b);
                auto rba = r(b, a);
                std::string diagnostics;
                diagnostics += "such that: \n  r(a,b) = r(b,a)\n";
                diagnostics += "r(a,b) : " + adapter.print(rab) + "\n";
                diagnostics += "r(b,a) : " + adapter.print(rba) + "\n";
                return Results(rab||rba, diagnostics); 
            }, as, bs);
    }

    template<typename Adapter, typename R, typename A, typename B>
    bool strict_totality(const Adapter& adapter, 
            std::string r_name, const R& r, 
        const A& as, const B& bs
    ) {
        return predicate(
            adapter,
            "strict totality", 
            [=](auto a, auto b){
                auto rab = r(a, b);
                auto rba = r(b, a);
                std::string diagnostics;
                diagnostics += "such that: \n  r(a,b) iff ¬r(b,a)\n";
                diagnostics += "r(a,b) : " + adapter.print(rab) + "\n";
                diagnostics += "r(b,a) : " + adapter.print(rba) + "\n";
                return Results(rab^rba, diagnostics); 
            }, as, bs);
    }

}
