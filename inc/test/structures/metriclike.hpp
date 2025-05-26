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

    template<typename F>
    struct PreMetric{
        const std::string f_name;    const F f; 
        PreMetric(
            const std::string f_name,    const F& f
        ): 
            f_name(f_name),       f(f)
        {}
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {

            auto ge0 = [](double x)           {return x>=0.0;};

            return 

            discernability(adapter,           as)         &&
            determinism  (adapter,                  f_name, f, as, as) &&
            codomain     (adapter, "positive", ge0, f_name, f, as, as) &&
            nilpotence   (adapter, "0", 0.0,        f_name, f, as)     &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {

            auto ge0 = [](double x)           {return x>=0.0;};

            return 

            determinism  (adapter,                  f_name, f, as, bs) &&
            codomain     (adapter, "positive", ge0, f_name, f, as, bs) &&
            codomain     (adapter, "positive", ge0, f_name, f, bs, as) &&

            true; // added so lines above can be easily swapped
        }

    };

    template<typename F>
    struct SemiMetric{
        const std::string f_name;    const F f; 
        SemiMetric(
            const std::string f_name,    const F& f
        ): 
            f_name(f_name),       f(f)
        {}
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {

            auto ge0 = [](double x)           {return x>=0.0;};

            return 

            discernability(adapter,           as)         &&
            determinism  (adapter,                  f_name, f, as, as) &&
            codomain     (adapter, "positive", ge0, f_name, f, as, as) &&
            nilpotence   (adapter, "0", 0.0,        f_name, f, as)     &&
            commutativity(adapter,                  f_name, f, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {

            auto ge0 = [](double x)           {return x>=0.0;};

            return 

            determinism  (adapter,                  f_name, f, as, bs) &&
            codomain     (adapter, "positive", ge0, f_name, f, as, bs) &&
            codomain     (adapter, "positive", ge0, f_name, f, bs, as) &&
            commutativity(adapter,                  f_name, f, as, bs) &&
            commutativity(adapter,                  f_name, f, bs, as) &&

            true; // added so lines above can be easily swapped
        }

    };

    template<typename F>
    struct MetaMetric{
        const std::string f_name;    const F f; 
        /* 
        NOTE: a metric by definition maps 𝕏→ℝ, so `add` and `leq` do not need to be passed in
        we define `add` and `leq` as returning doubles since this allows any other numeric primitives 
        to be automatically cast without loss of precision
        */
        MetaMetric(
            const std::string f_name,    const F& f
        ): 
            f_name(f_name),       f(f)
        {}
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {

            auto add = [](double x, double y) {return x+y; };
            auto leq = [](double x, double y) {return x<=y; };
            auto ge0 = [](double x)           {return x>=0.0;};

            return 

            discernability(adapter,           as)         &&
            determinism  (adapter,                  f_name, f, as, as) &&
            codomain     (adapter, "positive", ge0, f_name, f, as, as) &&
            commutativity(adapter,                  f_name, f, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {

            auto add = [](double x, double y) {return x+y; };
            auto leq = [](double x, double y) {return x<=y; };
            auto ge0 = [](double x)           {return x>=0.0;};

            return 

            determinism  (adapter,                  f_name, f, as, bs) &&
            codomain     (adapter, "positive", ge0, f_name, f, as, bs) &&
            codomain     (adapter, "positive", ge0, f_name, f, bs, as) &&
            commutativity(adapter,                  f_name, f, as, bs) &&
            commutativity(adapter,                  f_name, f, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, as, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, bs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, as, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, bs, bs) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs, typename Cs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs, const Cs& cs) const {

            auto add = [](double x, double y) {return x+y; };
            auto leq = [](double x, double y) {return x+y; };

            return 

            triangle_inequality(adapter, leq, add,  f_name, f, as, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, as, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, as, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, bs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, bs, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, cs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, cs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, cs, cs) &&

            triangle_inequality(adapter, leq, add,  f_name, f, bs, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, as, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, as, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, bs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, bs, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, cs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, cs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, cs, cs) &&

            triangle_inequality(adapter, leq, add,  f_name, f, cs, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, as, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, as, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, bs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, bs, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, cs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, cs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, cs, cs) &&

            true; // added so lines above can be easily swapped
        }

    };

    template<typename F>
    struct Metric{
        const std::string f_name;    const F f; 
        /* 
        NOTE: a metric by definition maps 𝕏→ℝ, so `add` and `leq` do not need to be passed in
        we define `add` and `leq` as returning doubles since this allows any other numeric primitives 
        to be automatically cast without loss of precision
        */
        Metric(
            const std::string f_name,    const F& f
        ): 
            f_name(f_name),       f(f)
        {}
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {

            auto add = [](double x, double y) {return x+y; };
            auto leq = [](double x, double y) {return x<=y; };
            auto ge0 = [](double x)           {return x>=0.0;};

            return 

            discernability(adapter,           as)         &&
            determinism  (adapter,                  f_name, f, as, as) &&
            codomain     (adapter, "positive", ge0, f_name, f, as, as) &&
            nilpotence   (adapter, "0", 0.0,        f_name, f, as)     &&
            commutativity(adapter,                  f_name, f, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {

            auto add = [](double x, double y) {return x+y; };
            auto leq = [](double x, double y) {return x<=y; };
            auto ge0 = [](double x)           {return x>=0.0;};

            return 

            determinism  (adapter,                  f_name, f, as, bs) &&
            codomain     (adapter, "positive", ge0, f_name, f, as, bs) &&
            codomain     (adapter, "positive", ge0, f_name, f, bs, as) &&
            commutativity(adapter,                  f_name, f, as, bs) &&
            commutativity(adapter,                  f_name, f, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, as, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, bs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, as, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, bs, bs) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs, typename Cs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs, const Cs& cs) const {

            auto add = [](double x, double y) {return x+y; };
            auto leq = [](double x, double y) {return x+y; };

            return 

            triangle_inequality(adapter, leq, add,  f_name, f, as, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, as, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, as, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, bs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, bs, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, cs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, cs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, as, cs, cs) &&

            triangle_inequality(adapter, leq, add,  f_name, f, bs, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, as, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, as, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, bs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, bs, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, cs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, cs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, bs, cs, cs) &&

            triangle_inequality(adapter, leq, add,  f_name, f, cs, as, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, as, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, as, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, bs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, bs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, bs, cs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, cs, as) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, cs, bs) &&
            triangle_inequality(adapter, leq, add,  f_name, f, cs, cs, cs) &&

            true; // added so lines above can be easily swapped
        }

    };

}

