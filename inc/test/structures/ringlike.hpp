#pragma once

// std libraries
#include <string>

// in-house libraries
#include "grouplike.hpp"

namespace test {

    /*
    NOTE1: Unlike tests for properties, tests for structure are bulky and are expected to be called 
    multiple times on multiple kinds of input (See math/analytic/Polynomial_test.cpp for an example).
    to save us trouble, we implement classes that store parameters that are likely not to change.
    */

    template<typename Zero, typename One, typename Add, typename Mult>
    struct Rig{
        const CommutativeMonoid <Zero, Add>  addition;
        const Monoid <One,  Mult> multiplication;
        const std::string zero_name; const Zero zero;   
        const std::string one_name;  const One  one;   
        const std::string add_name;  const Add  add;   
        const std::string mult_name; const Mult mult;
        Rig(
            const std::string zero_name, const Zero& zero, 
            const std::string one_name,  const One&  one, 
            const std::string add_name,  const Add&  add, 
            const std::string mult_name, const Mult& mult
        ): 
            addition      (zero_name, zero, add_name,  add),
            multiplication(one_name,  one,  mult_name, mult),
            zero_name (zero_name), zero (zero), 
            one_name  (one_name),  one  (one), 
            add_name  (add_name),  add  (add), 
            mult_name (mult_name), mult (mult)
        {}
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {
            return 

            addition      .valid(adapter, as) &&
            multiplication.valid(adapter, as) &&
            // binary_annihilation(adapter, zero_name, zero, mult_name, mult, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs, typename Cs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs, const Cs& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
    };

    template<typename Zero, typename Add, typename Sub, typename Mult>
    struct Rng{
        const Group  <Zero, Add, Sub>  addition;
        const SemiGroup <Mult>      multiplication;
        const std::string zero_name; const Zero zero;   
        const std::string add_name;  const Add  add;   
        const std::string sub_name;  const Sub  sub;   
        const std::string mult_name; const Mult mult;
        Rng(
            const std::string zero_name, const Zero& zero, 
            const std::string add_name,  const Add&  add, 
            const std::string sub_name,  const Sub&  sub, 
            const std::string mult_name, const Mult& mult
        ): 
            addition      (zero_name, zero, add_name,  add, sub_name,  sub),
            multiplication(mult_name, mult),
            zero_name (zero_name), zero (zero), 
            add_name  (add_name),  add  (add), 
            sub_name  (sub_name),  sub  (sub), 
            mult_name (mult_name), mult (mult)
        {}
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {
            return 

            addition      .valid(adapter, as) &&
            multiplication.valid(adapter, as) &&
            // binary_annihilation(adapter, zero_name, zero, mult_name, mult, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs, typename Cs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs, const Cs& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
    };

    template<typename Zero, typename One, typename Add, typename Sub, typename Mult>
    struct Ring{
        const Group  <Zero, Add, Sub>  addition;
        const Monoid <One,  Mult>      multiplication;
        const std::string zero_name; const Zero zero;   
        const std::string one_name;  const One  one;   
        const std::string add_name;  const Add  add;   
        const std::string sub_name;  const Sub  sub;   
        const std::string mult_name; const Mult mult;
        Ring(
            const std::string zero_name, const Zero& zero, 
            const std::string one_name,  const One&  one, 
            const std::string add_name,  const Add&  add, 
            const std::string sub_name,  const Sub&  sub, 
            const std::string mult_name, const Mult& mult
        ): 
            addition  (zero_name, zero, add_name,  add, sub_name,  sub),
            multiplication(one_name,  one,  mult_name, mult),
            zero_name (zero_name), zero (zero), 
            one_name  (one_name),  one  (one), 
            add_name  (add_name),  add  (add), 
            sub_name  (sub_name),  sub  (sub), 
            mult_name (mult_name), mult (mult)
        {}
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {
            return 

            addition      .valid(adapter, as) &&
            multiplication.valid(adapter, as) &&
            // binary_annihilation(adapter, zero_name, zero, mult_name, mult, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs, typename Cs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs, const Cs& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
    };


    template<typename Zero, typename One, typename Add, typename Mult>
    struct CommutativeRig{
        const CommutativeMonoid  <Zero, Add>  addition;
        const CommutativeMonoid <One,  Mult>  multiplication;
        const std::string zero_name; const Zero zero;   
        const std::string one_name;  const One  one;   
        const std::string add_name;  const Add  add;   
        const std::string mult_name; const Mult mult;
        CommutativeRig(
            const std::string zero_name, const Zero& zero, 
            const std::string one_name,  const One&  one, 
            const std::string add_name,  const Add&  add, 
            const std::string mult_name, const Mult& mult
        ): 
            addition      (zero_name, zero, add_name,  add),
            multiplication(one_name,  one,  mult_name, mult),
            zero_name (zero_name), zero (zero), 
            one_name  (one_name),  one  (one), 
            add_name  (add_name),  add  (add), 
            mult_name (mult_name), mult (mult)
        {}
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {
            return 

            addition      .valid(adapter, as) &&
            multiplication.valid(adapter, as) &&
            // binary_annihilation(adapter, zero_name, zero, mult_name, mult, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs, typename Cs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs, const Cs& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
    };

    template<typename Zero, typename Add, typename Sub, typename Mult>
    struct CommutativeRng{
        const CommutativeGroup  <Zero, Add, Sub>  addition;
        const CommutativeSemiGroup <Mult>      multiplication;
        const std::string zero_name; const Zero zero;   
        const std::string add_name;  const Add  add;   
        const std::string sub_name;  const Sub  sub;   
        const std::string mult_name; const Mult mult;
        CommutativeRng(
            const std::string zero_name, const Zero& zero, 
            const std::string add_name,  const Add&  add, 
            const std::string sub_name,  const Sub&  sub, 
            const std::string mult_name, const Mult& mult
        ): 
            addition      (zero_name, zero, add_name,  add, sub_name,  sub),
            multiplication(mult_name, mult),
            zero_name (zero_name), zero (zero), 
            add_name  (add_name),  add  (add), 
            sub_name  (sub_name),  sub  (sub), 
            mult_name (mult_name), mult (mult)
        {}
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {
            return 

            addition      .valid(adapter, as) &&
            multiplication.valid(adapter, as) &&
            // binary_annihilation(adapter, zero_name, zero, mult_name, mult, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs, typename Cs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs, const Cs& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

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
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {
            return 

            addition      .valid(adapter, as) &&
            multiplication.valid(adapter, as) &&
            // binary_annihilation(adapter, zero_name, zero, mult_name, mult, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs, typename Cs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs, const Cs& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

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
        template<typename Adapter, typename As>
        bool valid(const Adapter& adapter, const As& as) const {
            return 

            addition      .valid(adapter, as) &&
            multiplication.valid(adapter, as) &&
            // binary_annihilation(adapter, zero_name, zero, mult_name, mult, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs) const {
            return 

            addition      .valid(adapter, as, bs) &&
            multiplication.valid(adapter, as, bs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
        template<typename Adapter, typename As, typename Bs, typename Cs>
        bool valid(const Adapter& adapter, const As& as, const Bs& bs, const Cs& cs) const {
            return 

            addition      .valid(adapter, as, bs, cs) &&
            multiplication.valid(adapter, as, bs, cs) &&

            binary_distributivity(adapter, mult_name, mult, add_name, add, as, bs, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, as, cs, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, as, cs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, bs, cs, as) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, as, bs) &&
            binary_distributivity(adapter, mult_name, mult, add_name, add, cs, bs, as) &&

            true; // added so lines above can be easily swapped
        }
    };


}

