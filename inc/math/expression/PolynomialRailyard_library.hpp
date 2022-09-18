#pragma once

#include <cmath>

namespace math {
namespace spline {
    /*
    The `spline` namespace provides piecewise polynomial approximations for functions such as erf.
    This can be useful if trying to performantly calculate operations like integration or convolution 
    on arbitrary functions.

    See "Spline_library.R" for the R script that generated these approximations.
    */

    template<typename T, typename T2> 
    PolynomialRailyard<T,0,1> linear_spline(
        const std::vector<T2> x,
        const std::vector<T2> y
    ){
        assert(x.size() == y.size());
        assert(x.size() >= 1);
        using F = Polynomial<T,0,1>;
        using G = Railcar<T,F>;
        const T oo = std::numeric_limits<T>::max();
        std::vector<G> pieces;
        pieces.push_back(G(-oo, T(x[0]), F(T(y[0]))));
        for (std::size_t i=1; i<x.size(); i++)
        {
            assert(x[i] > x[i-1]);
            pieces.push_back(G(
                T(x[i-1]), T(x[i]), 
                linear_newton_polynomial(
                    T(x[i-1]), T(x[i]),   
                    T(y[i-1]), T(y[i])    
                )));
        }
        std::size_t last = y.size()-1;
        pieces.push_back(G(T(x[last]), oo, F(T(y[last]))));
        return PolynomialRailyard<T,0,1>(pieces);
    }


    /*
    `bump` is the Alan Zucconi "bump" function.
    It approximates a bell curve using a single quadratic piece.
    When the function is composed with others using operations like convolution,
    the low degree of the polynomial helps to prevent numerical overflow,
    and the low number of pieces propagates to whatever function is composed from it.
    This however comes at a cost of accuracy and smoothness.
    It is recommended only when you want a bell like function 
    without caring as to the kind of bell function.
    Adapted from GPU Gems and Alan Zucconi
    from https://www.alanzucconi.com/2017/07/15/improving-the-rainbow/
    */
    template<typename T> 
    PolynomialRailyard<T,0,2> bump(
        const T edge0, 
        const T edge1, 
        const T height
    ){
        const T center = (edge1 + edge0) / 2.0f;
        const T width  = (edge1 - edge0) / 2.0f;
        auto offset  = Polynomial<T,0,1>{-center/width, 1.0f/width};
        using F = Polynomial<T,0,2>;
        using G = Railcar<T,F>;
        return PolynomialRailyard<T,0,2>{ G(edge0, edge1, height * (1.0f - offset * offset)) };
    }

    /*
    `quadratic_3piece_gaussian` is a 3 piece quadratic approximation of the "Gaussian" function, exp(-x²). 
    The low degree of the polynomial helps prevent numerical overflow when the function is used in composition,
    and the number of pieces provides additional accuracy over `bump`, albeit at the cost of data locality.
    It guarantees that error will not exceed ±0.015 throughout the entire range of the function.
    */
    template<typename T> 
    PolynomialRailyard<T,0,2> quadratic_3piece_gaussian(){
        const T a(0.670f);
        const T b(1.670f);
        const T c(2.167f);
        auto F = [](T x){ return std::exp(-x*x); };
        using G = Railcar<T, Polynomial<T,0,2>>;
        return PolynomialRailyard<T,0,2>{
            G(-c,-a, quadratic_newton_polynomial(-c, -b,  -a,  0.0f, F(-b),   F(-a))),
            G(-a, a, quadratic_newton_polynomial(-a, 0.0f, a, F(-a), F(0.0f), F( a))),
            G( a, c, quadratic_newton_polynomial( a,  b,   c, F( a), F( b),   0.0f ))
        };
    }

    /*
    `quadratic_4piece_erf` is a 4 piece quadratic approximation of the "erf" function, erf(x) = ∫exp(-x²)dx. 
    The low degree of the polynomial helps prevent numerical overflow when the function is used in composition.
    It guarantees that error will not exceed ±0.022 throughout the entire range of the function.
    */
    template<typename T> 
    PolynomialRailyard<T,0,2> quadratic_4piece_erf(){
        const T  a(1.132f);
        const T  b(1.691f);
        const T oo(std::numeric_limits<T>::max()); // infinity
        auto F = [](T x){ return std::erf(x); };
        using G = Railcar<T, Polynomial<T,0,2>>;
        return PolynomialRailyard<T,0,2>{
            G(-oo,-b,   Polynomial<T,0,2>{-1.0f, 0.0f, 0.0f}),
            G(-b, 0.0f, quadratic_newton_polynomial(-b,   -a, 0.0f, -1.0f,    F(-a),  F(0.0f) )),
            G( 0.0f, b, quadratic_newton_polynomial( 0.0f, a, b,     F(0.0f), F(a),   1.0f    )),
            G( b, oo,   Polynomial<T,0,2>{1.0f, 0.0f, 0.0f})
        };
    }

    /*
    `quadratic_4piece_tanh` is a 4 piece quadratic approximation of the "tanh" function. 
    The low degree of the polynomial helps prevent numerical overflow when the function is used in composition.
    It guarantees that error will not exceed ±0.04 throughout the entire range of the function.
    */
    template<typename T> 
    PolynomialRailyard<T,0,2> quadratic_4piece_tanh(){
        const T  a(1.260f);
        const T  b(2.052f);
        const T oo(std::numeric_limits<T>::max()); // infinity
        auto F = [](T x){ return std::tanh(x); };
        using G = Railcar<T, Polynomial<T,0,2>>;
        return PolynomialRailyard<T,0,2>{
            G(-oo,-b,   Polynomial<T,0,2>{-1.0f, 0.0f, 0.0f}),
            G(-b, 0.0f, quadratic_newton_polynomial(-b,   -a, 0.0f, -1.0f,    F(-a),  F(0.0f) )),
            G( 0.0f, b, quadratic_newton_polynomial( 0.0f, a, b,     F(0.0f), F(a),   1.0f    )),
            G( b, oo,   Polynomial<T,0,2>{1.0f, 0.0f, 0.0f})
        };
    }

    /*
    `quadratic_3piece_sech2` is a 3 piece quadratic approximation of the "sech²" function, sech²(x) = d/dx tanh(x). 
    The low degree of the polynomial helps prevent numerical overflow when the function is used in composition,
    and the number of pieces provides additional accuracy over `bump`, albeit at the cost of data locality.
    It guarantees that error will not exceed ±0.03 throughout the entire range of the function.
    */
    template<typename T> 
    PolynomialRailyard<T,0,2> quadratic_3piece_sech2(){
        const T a(0.717f);
        const T b(1.766f);
        const T c(2.533f);
        auto F = [](T x){ return 1.0f/(std::cosh(x)*std::cosh(x)); };
        using G = Railcar<T, Polynomial<T,0,2>>;
        return PolynomialRailyard<T,0,2>{
            G(-c,-a, quadratic_newton_polynomial(-c, -b,  -a,  0.0f, F(-b),   F(-a))),
            G(-a, a, quadratic_newton_polynomial(-a, 0.0f, a, F(-a), F(0.0f), F( a))),
            G( a, c, quadratic_newton_polynomial( a,  b,   c, F( a), F( b),   0.0f ))
        };
    }

    /*
    `quadratic_6piece_ddxsech2` is a 3 piece quadratic approximation for the derivative of the "sech²" function:
        d/dx sech²(x) = d²/dx² tanh(x) = -2tanh(x)/cosh(x)². 
    The low degree of the polynomial helps prevent numerical overflow when the function is used in composition,
    and the number of pieces provides additional accuracy over `bump`, albeit at the cost of data locality.
    It guarantees that error will not exceed ±0.024 throughout the entire range of the function.
    */
    template<typename T> 
    PolynomialRailyard<T,0,2> quadratic_4piece_ddxsech2(){
        const T  a(0.464f);
        const T  b(0.812f);
        const T  c(2.236f);
        const T  d(2.982f);
        auto F = [](T x){ return -2.0f*std::tanh(x)/(std::cosh(x)*std::cosh(x)); };
        using G = Railcar<T, Polynomial<T,0,2>>;
        return PolynomialRailyard<T,0,2>{
            G(-d, -b,   quadratic_newton_polynomial( 0.0f,-c,-b,     F(0.0f), F(-c), F(-b)   )),
            G(-b, 0.0f, quadratic_newton_polynomial(-b,   -a, 0.0f,  F(-b),   F(-a), 0.0f    )),
            G( 0.0f, b, quadratic_newton_polynomial( 0.0f, a, b,     0.0f,    F(a),  F(b)    )),
            G( b,  d,   quadratic_newton_polynomial( b,    c, 0.0f,  F(b),    F(c),  0.0f    )),
        };
    }
}}