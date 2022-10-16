
float linear_polynomial_call(in float[2] p, in float x)
{
    return p[0] + x*p[1];
}

float quadratic_polynomial_call(in float[3] p, in float x)
{
    return p[0] + x*(p[1] + x*p[2]);
}

float cubic_polynomial_call(in float[4] p, in float x)
{
    return p[0] + x*(p[1] + x*(p[2] + x*p[3]));
}

float quartic_polynomial_call(in float[5] p, in float x)
{
    return p[0] + x*(p[1] + x*(p[2] + x*(p[3] + x*p[4])));
}

float quintic_polynomial_call(in float[6] p, in float x)
{
    return p[0] + x*(p[1] + x*(p[2] + x*(p[3] + x*(p[4] + x*p[5]))));
}


float[3] linear_polynomial_integral(in float[2] p, in float x)
{
    return float[3](0.0, p[0], p[1]/2.0);
}

float[4] quadratic_polynomial_integral(in float[3] p, in float x)
{
    return float[4](0.0, p[0], p[1]/2.0, p[2]/3.0);
}

float[5] cubic_polynomial_integral(in float[4] p, in float x)
{
    return float[5](0.0, p[0], p[1]/2.0, p[2]/3.0, p[3]/4.0);
}

float[6] quartic_polynomial_integral(in float[5] p, in float x)
{
    return float[6](0.0, p[0], p[1]/2.0, p[2]/3.0, p[3]/4.0, p[4]/5.0);
}

float[7] quintic_polynomial_integral(in float[6] p, in float x)
{
    return float[7](0.0, p[0], p[1]/2.0, p[2]/3.0, p[3]/4.0, p[4]/5.0, p[5]/6.0);
}


float linear_polynomial_derivative(in float[2] p, in float x)
{
    return p[1];
}

float[2] quadratic_polynomial_derivative(in float[3] p, in float x)
{
    return float[2](p[1], 2.0*p[2]);
}

float[3] cubic_polynomial_derivative(in float[4] p, in float x)
{
    return float[3](p[1], 2.0*p[2], 3.0*p[3]);
}

float[4] quartic_polynomial_derivative(in float[5] p, in float x)
{
    return float[4](p[1], 2.0*p[2], 3.0*p[3], 4.0*p[4]);
}

float[5] quintic_polynomial_derivative(in float[6] p, in float x)
{
    return float[5](p[1], 2.0*p[2], 3.0*p[3], 4.0*p[4], 5.0*p[5]);
}


float[2] linear_polynomial_compose(in float[2] p, in float x0)
{
    return float[2](
        p[0]+p[1]*x0, 
             p[1]);
}
float[3] quadratic_polynomial_compose(in float[3] p, in float x0)
{
    return float[3](
        p[0] +p[1]*x0 +p[2]*x0*x0, 
              p[1]    +p[2]*x0,
                       p[2]
    );
}
float[4] cubic_polynomial_compose_offset(in float[4] p, in float x0)
{
    return float[4](
        p[0] +p[1]*x0 +    p[2]*x0*x0 +    p[3]*x0*x0*x0,
              p[1]    +2.0*p[2]*x0    +3.0*p[3]*x0*x0,
                           p[2]       +3.0*p[3]*x0,
                                           p[3]
    );
}
float[5] quartic_polynomial_compose_offset(in float[5] p, in float x0)
{
    return float[5](
        p[0] +p[1]*x0 +    p[2]*x0*x0 +    p[3]*x0*x0*x0 +    p[4]*x0*x0*x0*x0,
              p[1]    +2.0*p[2]*x0    +3.0*p[3]*x0*x0    +4.0*p[4]*x0*x0*x0,
                           p[2]       +3.0*p[3]*x0       +6.0*p[4]*x0*x0,
                                           p[3]          +4.0*p[4]*x0,
                                                              p[4]
    );
}
float[6] quintic_polynomial_compose_offset(in float[6] p, in float x0)
{
    return float[6](
        p[0] +p[1]*x0 +    p[2]*x0*x0 +    p[3]*x0*x0*x0 +    p[4]*x0*x0*x0*x0 +    p[5]*x0*x0*x0*x0*x0,
              p[1]    +2.0*p[2]*x0    +3.0*p[3]*x0*x0    +4.0*p[4]*x0*x0*x0    +5.0*p[5]*x0*x0*x0*x0,
                           p[2]       +3.0*p[3]*x0       +6.0*p[4]*x0*x0      +10.0*p[5]*x0*x0*x0,
                                           p[3]          +4.0*p[4]*x0         +10.0*p[5]*x0*x0,
                                                              p[4]             +5.0*p[5]*x0,
                                                                                    p[5]
    );
}


float[2] linear_newton_polynomial(
    in float x1, 
    in float x2,
    in float y1, 
    in float y2
){
    float dfdx = (y2-y1) / (x2-x1);
    return float[2](y1-x1*dfdx, dfdx);
}

float[3] quadratic_newton_polynomial(
    in float x1, 
    in float x2, 
    in float x3,
    in float y1, 
    in float y2, 
    in float y3
){
    float dydx_12  = (y2-y1) / (x2-x1);
    float dydx_23  = (y3-y2) / (x3-x2);
    float dy2dx2 = (dydx_23-dydx_12) / (x3-x1);
    return float[3](
        y1 - dydx_12*x1 + dy2dx2*x1*x2,
             dydx_12    - dy2dx2*x1    
                        - dy2dx2*x2,
             dy2dx2
    );
}

float[4] cubic_newton_polynomial(
    in float x1, 
    in float x2, 
    in float x3,
    in float x4,
    in float y1, 
    in float y2, 
    in float y3,
    in float y4
){
    float dydx_12 = (y2-y1) / (x2-x1);
    float dydx_23 = (y3-y2) / (x3-x2);
    float dydx_34 = (y4-y3) / (x4-x3);
    float dy2dx2_13 = (dydx_23-dydx_12) / (x3-x1);
    float dy2dx2_24 = (dydx_34-dydx_23) / (x4-x2);
    float dy3dx3    = (dy2dx2_24-dy2dx2_13) / (x4-x1);
    return float[4](
        y1 - dydx_12*x1 + dy2dx2_13*x1*x2 - dy3dx3*x1*x2*x3,
             dydx_12    - dy2dx2_13*x1
                        - dy2dx2_13*x2    + dy3dx3*x1*x2
                                          + dy3dx3*x1*x3
                                          + dy3dx3*x2*x3,
                          dy2dx2_13       - dy3dx3*x1
                                          - dy3dx3*x2
                                          - dy3dx3*x3,
                                            dy3dx3
    );
}


/* 
NOTE: The following is an alternate implementation to "cubic_spline()".
It avoids a call to inverse() that should in principle make it faster,
and it provides a good way to verify other spline functions, 
however its implementation is more complex and cannot be easily adapted to higher order splines.
*/
float[4] cubic_algebraic_spline(
    in float x0, // lower bound of spline
    in float x1, // upper bound of spline
    in float y0, // value of y at x1
    in float y1, // value of y at x1
    in float d0, // derivative of y at x0
    in float d1  // derivative of y at x1
){
    /*
    First, we construct a new coordinate system based around (x0,y0) as the origin. 
    In this coordinate system, the coordinates (x1,y1) are denoted (X,Y).
    */
    float Y = y1-y0;
    float X = x1-x0;
    /*
    in the new coordinate system, we know that x0=0 and y0=0, 
    so we can adopt a simpler system of equations:
       Y =  aX² +  cX³
      d1 = 2aX  + 3cX²
    we divide Y and d1 by X² and X respectively to get:
       Y =  a +  cX
      d1 = 2a + 3cX
    */
    float u = (Y-d0*X)/(X*X);
    float v = (d1-d0)/X;
    /*
    We then solutions for this system of equations, which results in a polynomial Ax+Bx²+Cx³:
    */
    float A = d0;
    float B = (3.0*u-v);
    float C = ((v-2.0*u)/X);
    /*
    The polynomial above is composed with (x-x0) and the result is offset by y0, 
    which returns us to the original coordinate system.
    */
    return float[4](
        y0 -A*x0 +B*x0*x0  -C*x0*x0*x0,
            A    -B*x0*2.0 +C*x0*x0*3.0,
                 +B        -C*x0*3.0,
                           +C
    );
    // return y0 + 
    //     compose(
    //         A*x + B*x2 + C*x3,
    //         x-x0
    //     )
}
