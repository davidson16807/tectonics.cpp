#pragma once

// 3rd party libraries
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2.hpp>
#include <glm/mat3.hpp>
#include <glm/mat4.hpp>

// in-house libraries
#include "Polynomial.hpp"

namespace math {

    namespace {

        template<typename T, glm::qualifier Q>
        glm::vec2<T,Q> spline_sample_factors(glm::vec2<T,Q> exponents, float x)
        {
            return glm::vec2<T,Q>(
                pow(x, exponents.x),
                pow(x, exponents.y)
            );
        }

        template<typename T, glm::qualifier Q>
        glm::vec2<T,Q> spline_derivative_factors(glm::vec2<T,Q> exponents, float x)
        {
            return glm::vec2<T,Q>(
                exponents.x * pow(x, exponents.x-T(1)),
                exponents.y * pow(x, exponents.y-T(1))
            );
        }

        template<typename T, glm::qualifier Q>
        glm::vec2<T,Q> spline_2nd_derivative_factors(glm::vec2<T,Q> exponents, float x)
        {
            return glm::vec2<T,Q>(
                (exponents.x-T(1)) * exponents.x * pow(x, exponents.x-T(2)),
                (exponents.y-T(1)) * exponents.y * pow(x, exponents.y-T(2))
            );
        }

        template<typename T, glm::qualifier Q>
        glm::vec2<T,Q> spline_integral_factors(glm::vec2<T,Q> exponents, float x)
        {
            return glm::vec2<T,Q>(
                pow(x, exponents.x+T(1)) / (exponents.x+T(1)),
                pow(x, exponents.y+T(1)) / (exponents.y+T(1))
            );
        }


        template<typename T, glm::qualifier Q>
        glm::vec3<T,Q> spline_sample_factors(glm::vec3<T,Q> exponents, float x)
        {
            return glm::vec3<T,Q>(
                pow(x, exponents.x),
                pow(x, exponents.y),
                pow(x, exponents.z)
            );
        }

        template<typename T, glm::qualifier Q>
        glm::vec3<T,Q> spline_derivative_factors(glm::vec3<T,Q> exponents, float x)
        {
            return glm::vec3<T,Q>(
                exponents.x * pow(x, exponents.x-T(1)),
                exponents.y * pow(x, exponents.y-T(1)),
                exponents.z * pow(x, exponents.z-T(1))
            );
        }

        template<typename T, glm::qualifier Q>
        glm::vec3<T,Q> spline_2nd_derivative_factors(glm::vec3<T,Q> exponents, float x)
        {
            return glm::vec3<T,Q>(
                (exponents.x-T(1)) * exponents.x * pow(x, exponents.x-T(2)),
                (exponents.y-T(1)) * exponents.y * pow(x, exponents.y-T(2)),
                (exponents.z-T(1)) * exponents.z * pow(x, exponents.z-T(2))
            );
        }

        template<typename T, glm::qualifier Q>
        glm::vec3<T,Q> spline_integral_factors(glm::vec3<T,Q> exponents, float x)
        {
            return glm::vec3<T,Q>(
                pow(x, exponents.x+T(1)) / (exponents.x+T(1)),
                pow(x, exponents.y+T(1)) / (exponents.y+T(1)),
                pow(x, exponents.z+T(1)) / (exponents.z+T(1))
            );
        }

        template<typename T, glm::qualifier Q>
        glm::vec4<T,Q> spline_sample_factors(glm::vec4<T,Q> exponents, float x)
        {
            return glm::vec4<T,Q>(
                pow(x, exponents.x),
                pow(x, exponents.y),
                pow(x, exponents.z),
                pow(x, exponents.w)
            );
        }

        template<typename T, glm::qualifier Q>
        glm::vec4<T,Q> spline_derivative_factors(glm::vec4<T,Q> exponents, float x)
        {
            return glm::vec4<T,Q>(
                exponents.x * pow(x, exponents.x-T(1)),
                exponents.y * pow(x, exponents.y-T(1)),
                exponents.z * pow(x, exponents.z-T(1)),
                exponents.w * pow(x, exponents.w-T(1))
            );
        }

        template<typename T, glm::qualifier Q>
        glm::vec4<T,Q> spline_2nd_derivative_factors(glm::vec4<T,Q> exponents, float x)
        {
            return glm::vec4<T,Q>(
                (exponents.x-T(1)) * exponents.x * pow(x, exponents.x-T(2)),
                (exponents.y-T(1)) * exponents.y * pow(x, exponents.y-T(2)),
                (exponents.z-T(1)) * exponents.z * pow(x, exponents.z-T(2)),
                (exponents.w-T(1)) * exponents.w * pow(x, exponents.w-T(2))
            );
        }

        template<typename T, glm::qualifier Q>
        glm::vec4<T,Q> spline_integral_factors(glm::vec4<T,Q> exponents, float x)
        {
            return glm::vec4<T,Q>(
                pow(x, exponents.x+T(1)) / (exponents.x+T(1)),
                pow(x, exponents.y+T(1)) / (exponents.y+T(1)),
                pow(x, exponents.z+T(1)) / (exponents.z+T(1)),
                pow(x, exponents.w+T(1)) / (exponents.w+T(1))
            );
        }

    }


template<typename T>
Polynomial<T,0,3> cubic_spline(
        const T xa,     const T xb,
        const T ya,     const T yb,
        const T ya_ddx, const T yb_ddx){
    T b = xb-xa;
    glm::vec2<T,glm::defaultp> exponents = glm::vec2<T,glm::defaultp>(2,3);
    glm::vec2<T,glm::defaultp> known_outputs = glm::vec2<T,glm::defaultp>(
        yb - ya - ya_ddx*b,
        yb_ddx  - ya_ddx
    );
    glm::mat2x2<T,glm::defaultp> known_factors = glm::transpose(
        glm::mat2x2<T,glm::defaultp>(
            spline_sample_factors    (exponents, b),
            spline_derivative_factors(exponents, b)
        )
    );
    glm::vec2<T,glm::defaultp> k2k3 = glm::inverse(known_factors) * known_outputs;
    return Polynomial<T,0,3>{ya, ya_ddx, k2k3.x, k2k3.y};
}

template<typename T>
Polynomial<T,0,4> quartic_spline_with_known_midpoint(
        const T xa,     const T xb, const T xc,
        const T ya,     const T yb, const T yc,
        const T ya_ddx,             const T yc_ddx){
    T b = xb-xa;
    T c = xc-xa;
    glm::vec3<T,glm::defaultp> exponents = glm::vec3<T,glm::defaultp>(2,3,4);
    glm::vec3<T,glm::defaultp> known_outputs = glm::vec3<T,glm::defaultp>(
        yb - ya - ya_ddx*b,
        yc - ya - ya_ddx*c,
        yc_ddx  - ya_ddx
    );
    mat3 known_factors = glm::transpose(
        mat3(
            spline_sample_factors    (exponents, b),
            spline_sample_factors    (exponents, c),
            spline_derivative_factors(exponents, c)
        )
    );
    glm::vec3<T,glm::defaultp> k2k3k4 = glm::inverse(known_factors) * known_outputs;
    return Polynomial<T,0,4>{ya, ya_ddx, k2k3k4.x, k2k3k4.y, k2k3k4.z};
}

template<typename T>
Polynomial<T,0,5> quintic_spline_with_2_known_midpoints(
        const T xa,     const T xb, const T xc, const T xd,
        const T ya,     const T yb, const T yc, const T yd,
        const T ya_ddx,                         const T yd_ddx){
    T b = xb-xa;
    T c = xc-xa;
    T d = xd-xa;
    glm::vec4<T,glm::defaultp> exponents = glm::vec4<T,glm::defaultp>(2,3,4,5);
    glm::vec4<T,glm::defaultp> known_outputs = glm::vec4<T,glm::defaultp>(
        yb - ya - ya_ddx*b,
        yc - ya - ya_ddx*c,
        yd - ya - ya_ddx*d,
        yd_ddx  - ya_ddx
    );
    mat4 known_factors = glm::transpose(
        mat4(
            spline_sample_factors    (exponents, b),
            spline_sample_factors    (exponents, c),
            spline_sample_factors    (exponents, d),
            spline_derivative_factors(exponents, d)
        )
    );
    glm::vec4<T,glm::defaultp> k2k3k4k5 = glm::inverse(known_factors) * known_outputs;
    return Polynomial<T,0,5>{ya, ya_ddx, k2k3k4k5.x, k2k3k4k5.y, k2k3k4k5.z, k2k3k4k5.w};
}


template<typename T>
Polynomial<T,0,5> quintic_spline(
        const T xa,       const T xb,
        const T ya,       const T yb,
        const T ya_ddx,   const T yb_ddx,
        const T ya_d2dx2, const T yb_d2dx){
    T b = xb-xa;
    glm::vec3<T,glm::defaultp> exponents = glm::vec3<T,glm::defaultp>(3,4,5);
    glm::vec3<T,glm::defaultp> known_outputs = glm::vec3<T,glm::defaultp>(
        yb      - ya - ya_ddx*b -      ya_d2dx2*b*b,
        yb_ddx       - ya_ddx   - T(2)*ya_d2dx2*b,
        yb_d2dx      -          -      ya_d2dx2
    );
    mat3 known_factors = glm::transpose(
        mat3(
            spline_sample_factors        (exponents, b),
            spline_derivative_factors    (exponents, b),
            spline_2nd_derivative_factors(exponents, b)
        )
    );
    glm::vec3<T,glm::defaultp> k3k4k5 = glm::inverse(known_factors) * known_outputs;
    return Polynomial<T,0,5>{ya, ya_ddx, ya_d2dx2, k3k4k5.x, k3k4k5.y, k3k4k5.z};
}

template<typename T>
Polynomial<T,0,5> quintic_spline_with_known_midpoint(
        const T xa,     const T xb,     const T xc,
        const T ya,     const T yb,     const T yc,
        const T ya_ddx, const T yb_ddx, const T yc_ddx){
    T b = xb-xa;
    T c = xc-xa;
    glm::vec4<T,glm::defaultp> exponents = glm::vec4<T,glm::defaultp>(2,3,4,5);
    glm::vec4<T,glm::defaultp> known_outputs = glm::vec4<T,glm::defaultp>(
        yb - ya - ya_ddx*b,
        yc - ya - ya_ddx*c,
        yb_ddx  - ya_ddx,
        yc_ddx  - ya_ddx
    );
    mat4 known_factors = glm::transpose(
        mat4(
            spline_sample_factors    (exponents, b),
            spline_sample_factors    (exponents, c),
            spline_derivative_factors(exponents, b),
            spline_derivative_factors(exponents, c)
        )
    );
    glm::vec4<T,glm::defaultp> k2k3k4k5 = glm::inverse(known_factors) * known_outputs;
    return Polynomial<T,0,5>{ya, ya_ddx, k2k3k4k5.x, k2k3k4k5.y, k2k3k4k5.z, k2k3k4k5.w};
}






}
