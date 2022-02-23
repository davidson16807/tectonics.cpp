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

    template<int degree> 
    spline(const std::vector<float> x, const std::vector<float> y) = delete;

    Spline<0,1> linear_spline(
        const std::vector<float> x,
        const std::vector<float> y
    ){
        assert(x.size() == y.size());
        assert(x.size() >= 1);
        float slope;
        pieces.push_back(Piecewise<Polynomial<Plo,Phi>>(
            std::numeric_limits<float>::min(), x[0], 
            Polynomial<0,1>{y[0],0}));
        for (int i=1; i<x.size(); i++)
        {
            slope = (y[i]-y[i-1])/(x[i]-x[i-1]);
            pieces.push_back(Piecewise<Polynomial<Plo,Phi>>(
                x[i-1], x[i], 
                linear_newton_polynomial(
                    x[i-1], y[i-1], 
                    x[i],   y[i]    
                )));
        }
        pieces.push_back(Piecewise<Polynomial<Plo,Phi>>(
            x[y.size()-1], std::numeric_limits<float>::max(), 
            Polynomial<0,1>{y[y.size()-1],0}));
        return Spline<0,1>(pieces);
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
    Spline<0,2> bump(
        const float edge0, 
        const float edge1, 
        const float height
    ){
        const float center = (edge1 + edge0) / 2.0f;
        const float width  = (edge1 - edge0) / 2.0f;
        auto offset  = Polynomial<0,1>{-center/width, 1.0f/width};
        using PiecewisePolynomial = Piecewise<Polynomial<0,2>>;
        return Spline<0,2>{ PiecewisePolynomial(edge0, edge1, height * (1.0f - offset * offset)) };
    }

    /*
    `quadratic_3piece_gaussian` is a 3 piece quadratic approximation of the "Gaussian" function, exp(-x²). 
    The low degree of the polynomial helps prevent numerical overflow when the function is used in composition,
    and the number of pieces provides additional accuracy over `bump`, albeit at the cost of data locality.
    It guarantees that error will not exceed ±0.015 throughout the entire range of the function.
    */
    Spline<0,2> quadratic_3piece_gaussian(){
        const float a(0.670f);
        const float b(1.670f);
        const float c(2.167f);
        auto F = [](float x){ return std::exp(-x*x); }
        using PiecewisePolynomial = Piecewise<Polynomial<0,2>>;
        return Spline<0,2>{
            PiecewisePolynomial(-c,-a, quadratic_newton_polynomial(-c, -b,  -a,  0.0f, F(-b),   F(-a))),
            PiecewisePolynomial(-a, a, quadratic_newton_polynomial(-a, 0.0f, a, F(-a), F(0.0f), F( a))),
            PiecewisePolynomial( a, c, quadratic_newton_polynomial( a,  b,   c, F( a), F( b),   0.0f ))
        };
    }

    /*
    `quadratic_4piece_erf` is a 4 piece quadratic approximation of the "erf" function, erf(x) = ∫exp(-x²)dx. 
    The low degree of the polynomial helps prevent numerical overflow when the function is used in composition.
    It guarantees that error will not exceed ±0.022 throughout the entire range of the function.
    */
    Spline<0,2> quadratic_4piece_erf(){
        const float  a(1.132f);
        const float  b(1.691f);
        const float oo(std::numeric_limits<float>::max()); // infinity
        auto F = [](float x){ return std::erf(x); }
        using PiecewisePolynomial = Piecewise<Polynomial<0,2>>;
        return Spline<0,2>{
            PiecewisePolynomial(-oo,-b,   Polynomial<0,2>{-1.0f, 0.0f, 0.0f}),
            PiecewisePolynomial(-b, 0.0f, quadratic_newton_polynomial(-b,   -a, 0.0f, -1.0f,    F(-a),  F(0.0f) )),
            PiecewisePolynomial( 0.0f, b, quadratic_newton_polynomial( 0.0f, a, b,     F(0.0f), F(a),   1.0f    )),
            PiecewisePolynomial( b, oo,   Polynomial<0,2>{1.0f, 0.0f, 0.0f})
        };
    }

    /*
    `quadratic_4piece_tanh` is a 4 piece quadratic approximation of the "tanh" function. 
    The low degree of the polynomial helps prevent numerical overflow when the function is used in composition.
    It guarantees that error will not exceed ±0.04 throughout the entire range of the function.
    */
    Spline<0,2> quadratic_4piece_tanh(){
        const float  a(1.260f);
        const float  b(2.052f);
        const float oo(std::numeric_limits<float>::max()); // infinity
        auto F = [](float x){ return std::tanh(x); }
        using PiecewisePolynomial = Piecewise<Polynomial<0,2>>;
        return Spline<0,2>{
            PiecewisePolynomial(-oo,-b,   Polynomial<0,2>{-1.0f, 0.0f, 0.0f}),
            PiecewisePolynomial(-b, 0.0f, quadratic_newton_polynomial(-b,   -a, 0.0f, -1.0f,    F(-a),  F(0.0f) )),
            PiecewisePolynomial( 0.0f, b, quadratic_newton_polynomial( 0.0f, a, b,     F(0.0f), F(a),   1.0f    )),
            PiecewisePolynomial( b, oo,   Polynomial<0,2>{1.0f, 0.0f, 0.0f})
        };
    }

    /*
    `quadratic_3piece_sech2` is a 3 piece quadratic approximation of the "sech²" function, sech²(x) = d/dx tanh(x). 
    The low degree of the polynomial helps prevent numerical overflow when the function is used in composition,
    and the number of pieces provides additional accuracy over `bump`, albeit at the cost of data locality.
    It guarantees that error will not exceed ±0.03 throughout the entire range of the function.
    */
    Spline<0,2> quadratic_3piece_sech2(){
        const float a(0.717f);
        const float b(1.766f);
        const float c(2.533f);
        auto F = [](float x){ return 1.0f/(std::cosh(x)*std::cosh(x)); }
        using PiecewisePolynomial = Piecewise<Polynomial<0,2>>;
        return Spline<0,2>{
            PiecewisePolynomial(-c,-a, quadratic_newton_polynomial(-c, -b,  -a,  0.0f, F(-b),   F(-a))),
            PiecewisePolynomial(-a, a, quadratic_newton_polynomial(-a, 0.0f, a, F(-a), F(0.0f), F( a))),
            PiecewisePolynomial( a, c, quadratic_newton_polynomial( a,  b,   c, F( a), F( b),   0.0f ))
        };
    }

    /*
    `quadratic_6piece_ddxsech2` is a 3 piece quadratic approximation for the derivative of the "sech²" function:
        d/dx sech²(x) = d²/dx² tanh(x) = -2tanh(x)/cosh(x)². 
    The low degree of the polynomial helps prevent numerical overflow when the function is used in composition,
    and the number of pieces provides additional accuracy over `bump`, albeit at the cost of data locality.
    It guarantees that error will not exceed ±0.024 throughout the entire range of the function.
    */
    Spline<0,2> quadratic_4piece_ddxsech2(){
        const float  a(0.464f);
        const float  b(0.812f);
        const float  c(2.236f);
        const float  d(2.982f);
        auto F = [](float x){ return -2.0f*std::tanh(x)/(std::cosh(x)*std::cosh(x)); }
        using PiecewisePolynomial = Piecewise<Polynomial<0,2>>;
        return Spline<0,2>{
            PiecewisePolynomial(-d, -b,   quadratic_newton_polynomial( 0.0f,-c,-b,     F(0.0f), F(-c), F(-b)   )),
            PiecewisePolynomial(-b, 0.0f, quadratic_newton_polynomial(-b,   -a, 0.0f,  F(-b),   F(-a), 0.0f    )),
            PiecewisePolynomial( 0.0f, b, quadratic_newton_polynomial( 0.0f, a, b,     0.0f,    F(a),  F(b)    )),
            PiecewisePolynomial( b,  d,   quadratic_newton_polynomial( b,    c, 0.0f,  F(b),    F(c),  0.0f    )),
        };
    }
}}