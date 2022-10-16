# 3rd party libraries
import glm
from .. import Polynomial

def spline_sample_factors(degrees, x:float):
    vec = [None, None, glm.vec2, glm.vec3, glm.vec4][len(degrees)]
    return vec(*[pow(x, exponent) for exponent in degrees])

def spline_derivative_factors(degrees, x:float):
    vec = [None, None, glm.vec2, glm.vec3, glm.vec4][len(degrees)]
    return vec(*[exponent * pow(x, exponent-1) for exponent in degrees])

def spline_2nd_derivative_factors(degrees, x:float):
    vec = [None, None, glm.vec2, glm.vec3, glm.vec4][len(degrees)]
    return vec(*[(exponent-1) * exponent * pow(x, exponent-2) for exponent in degrees])

def spline_integral_factors(degrees, x:float):
    vec = [None, None, glm.vec2, glm.vec3, glm.vec4][len(degrees)]
    return vec(*[pow(x, exponent+1) / (exponent+1) for exponent in degrees])



def cubic_spline(
        xa:float,     xb:float,
        ya:float,     yb:float,
        ya_ddx:float, yb_ddx:float):
    b = xb-xa
    exponents = glm.vec2(2,3)
    known_outputs = glm.vec2(
        yb - ya - ya_ddx*b,
        yb_ddx  - ya_ddx
    )
    known_factors = glm.transpose(
        glm.mat2(
            spline_sample_factors    (exponents, b),
            spline_derivative_factors(exponents, b)
        )
    )
    k2k3 = glm.inverse(known_factors) * known_outputs
    return Polynomial(ya, ya_ddx, k2k3.x, k2k3.y)


def quartic_spline_with_known_midpoint(
        xa:float,     xb:float, xc:float,
        ya:float,     yb:float, yc:float,
        ya_ddx:float,              yc_ddx:float):
    b = xb-xa
    c = xc-xa
    exponents = glm.vec3(2,3,4)
    known_outputs = glm.vec3(
        yb - ya - ya_ddx*b,
        yc - ya - ya_ddx*c,
        yc_ddx  - ya_ddx
    )
    known_factors = glm.transpose(
        glm.mat3(
            spline_sample_factors    (exponents, b),
            spline_sample_factors    (exponents, c),
            spline_derivative_factors(exponents, c)
        )
    )
    k2k3k4 = glm.inverse(known_factors) * known_outputs
    return Polynomial(ya, ya_ddx, k2k3k4.x, k2k3k4.y, k2k3k4.z)


def quintic_spline_with_2_known_midpoints(
        xa:float,     xb:float, xc:float, xd:float,
        ya:float,     yb:float, yc:float, yd:float,
        ya_ddx:float,                           yd_ddx:float):
    b = xb-xa
    c = xc-xa
    d = xd-xa
    exponents = glm.vec4(2,3,4,5)
    known_outputs = glm.vec4(
        yb - ya - ya_ddx*b,
        yc - ya - ya_ddx*c,
        yd - ya - ya_ddx*d,
        yd_ddx  - ya_ddx
    )
    known_factors = glm.transpose(
        glm.mat4(
            spline_sample_factors    (exponents, b),
            spline_sample_factors    (exponents, c),
            spline_sample_factors    (exponents, d),
            spline_derivative_factors(exponents, d)
        )
    )
    k2k3k4k5 = glm.inverse(known_factors) * known_outputs
    return Polynomial(ya, ya_ddx, k2k3k4k5.x, k2k3k4k5.y, k2k3k4k5.z, k2k3k4k5.w)



def quintic_spline(
        xa:float,       xb:float,
        ya:float,       yb:float,
        ya_ddx:float,   yb_ddx:float,
        ya_d2dx2:float, yb_d2dx:float):
    b = xb-xa
    exponents = glm.vec3(3,4,5)
    known_outputs = glm.vec3(
        yb      - ya - ya_ddx*b -      ya_d2dx2*b*b,
        yb_ddx       - ya_ddx   -  2.0*ya_d2dx2*b,
        yb_d2dx      -          -      ya_d2dx2
    )
    known_factors = glm.transpose(
        glm.mat3(
            spline_sample_factors        (exponents, b),
            spline_derivative_factors    (exponents, b),
            spline_2nd_derivative_factors(exponents, b)
        )
    )
    k3k4k5 = glm.inverse(known_factors) * known_outputs
    return Polynomial(ya, ya_ddx, ya_d2dx2, k3k4k5.x, k3k4k5.y, k3k4k5.z)


def quintic_spline_with_known_midpoint(
        xa:float,     xb:float,     xc:float,
        ya:float,     yb:float,     yc:float,
        ya_ddx:float, yb_ddx:float, yc_ddx:float):
    b = xb-xa
    c = xc-xa
    exponents = glm.vec4(2,3,4,5)
    known_outputs = glm.vec4(
        yb - ya - ya_ddx*b,
        yc - ya - ya_ddx*c,
        yb_ddx  - ya_ddx,
        yc_ddx  - ya_ddx
    )
    known_factors = glm.transpose(
        glm.mat4(
            spline_sample_factors    (exponents, b),
            spline_sample_factors    (exponents, c),
            spline_derivative_factors(exponents, b),
            spline_derivative_factors(exponents, c)
        )
    )
    k2k3k4k5 = glm.inverse(known_factors) * known_outputs
    return Polynomial(ya, ya_ddx, k2k3k4k5.x, k2k3k4k5.y, k2k3k4k5.z, k2k3k4k5.w)

