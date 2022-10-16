

vec2 spline_sample_factors(vec2 exponents, float x)
{
    return vec2(
        pow(x, exponents.x),
        pow(x, exponents.y)
    );
}

vec2 spline_derivative_factors(vec2 exponents, float x)
{
    return vec2(
        exponents.x * pow(x, exponents.x-1.0),
        exponents.y * pow(x, exponents.y-1.0)
    );
}

vec2 spline_2nd_derivative_factors(vec2 exponents, float x)
{
    return vec2(
        (exponents.x-1.0) * exponents.x * pow(x, exponents.x-2.0),
        (exponents.y-1.0) * exponents.y * pow(x, exponents.y-2.0)
    );
}

vec2 spline_integral_factors(vec2 exponents, float x)
{
    return vec2(
        pow(x, exponents.x+1.0) / (exponents.x+1.0),
        pow(x, exponents.y+1.0) / (exponents.y+1.0)
    );
}


vec3 spline_sample_factors(vec3 exponents, float x)
{
    return vec3(
        pow(x, exponents.x),
        pow(x, exponents.y),
        pow(x, exponents.z)
    );
}

vec3 spline_derivative_factors(vec3 exponents, float x)
{
    return vec3(
        exponents.x * pow(x, exponents.x-1.0),
        exponents.y * pow(x, exponents.y-1.0),
        exponents.z * pow(x, exponents.z-1.0)
    );
}

vec3 spline_2nd_derivative_factors(vec3 exponents, float x)
{
    return vec3(
        (exponents.x-1.0) * exponents.x * pow(x, exponents.x-2.0),
        (exponents.y-1.0) * exponents.y * pow(x, exponents.y-2.0),
        (exponents.z-1.0) * exponents.z * pow(x, exponents.z-2.0)
    );
}

vec3 spline_integral_factors(vec3 exponents, float x)
{
    return vec3(
        pow(x, exponents.x+1.0) / (exponents.x+1.0),
        pow(x, exponents.y+1.0) / (exponents.y+1.0),
        pow(x, exponents.z+1.0) / (exponents.z+1.0)
    );
}

vec4 spline_sample_factors(vec4 exponents, float x)
{
    return vec4(
        pow(x, exponents.x),
        pow(x, exponents.y),
        pow(x, exponents.z),
        pow(x, exponents.w)
    );
}

vec4 spline_derivative_factors(vec4 exponents, float x)
{
    return vec4(
        exponents.x * pow(x, exponents.x-1.0),
        exponents.y * pow(x, exponents.y-1.0),
        exponents.z * pow(x, exponents.z-1.0),
        exponents.w * pow(x, exponents.w-1.0)
    );
}

vec4 spline_2nd_derivative_factors(vec4 exponents, float x)
{
    return vec4(
        (exponents.x-1.0) * exponents.x * pow(x, exponents.x-2.0),
        (exponents.y-1.0) * exponents.y * pow(x, exponents.y-2.0),
        (exponents.z-1.0) * exponents.z * pow(x, exponents.z-2.0),
        (exponents.w-1.0) * exponents.w * pow(x, exponents.w-2.0)
    );
}

vec4 spline_integral_factors(vec4 exponents, float x)
{
    return vec4(
        pow(x, exponents.x+1.0) / (exponents.x+1.0),
        pow(x, exponents.y+1.0) / (exponents.y+1.0),
        pow(x, exponents.z+1.0) / (exponents.z+1.0),
        pow(x, exponents.w+1.0) / (exponents.w+1.0)
    );
}


float[4] cubic_spline(
        in float xa,     in float xb,
        in float ya,     in float yb,
        in float ya_ddx, in float yb_ddx){
    float b = xb-xa;
    vec2 exponents = vec2(2,3);
    vec2 known_outputs = vec2(
        yb - ya - ya_ddx*b,
        yb_ddx  - ya_ddx
    );
    mat2 known_factors = transpose(
        mat2(
            spline_sample_factors    (exponents, b),
            spline_derivative_factors(exponents, b)
        )
    );
    vec2 k2k3 = inverse(known_factors) * known_outputs;
    return float[4](ya, ya_ddx, k2k3.x, k2k3.y);
}

float[5] quartic_spline_with_known_midpoint(
        in float xa,     in float xb, in float xc,
        in float ya,     in float yb, in float yc,
        in float ya_ddx,              in float yc_ddx){
    float b = xb-xa;
    float c = xc-xa;
    vec3 exponents = vec3(2,3,4);
    vec3 known_outputs = vec3(
        yb - ya - ya_ddx*b,
        yc - ya - ya_ddx*c,
        yc_ddx  - ya_ddx
    );
    mat3 known_factors = transpose(
        mat3(
            spline_sample_factors    (exponents, b),
            spline_sample_factors    (exponents, c),
            spline_derivative_factors(exponents, c)
        )
    );
    vec3 k2k3k4 = inverse(known_factors) * known_outputs;
    return float[5](ya, ya_ddx, k2k3k4.x, k2k3k4.y, k2k3k4.z);
}

float[6] quintic_spline_with_2_known_midpoints(
        in float xa,     in float xb, in float xc, in float xd,
        in float ya,     in float yb, in float yc, in float yd,
        in float ya_ddx,                           in float yd_ddx){
    float b = xb-xa;
    float c = xc-xa;
    float d = xd-xa;
    vec4 exponents = vec4(2,3,4,5);
    vec4 known_outputs = vec4(
        yb - ya - ya_ddx*b,
        yc - ya - ya_ddx*c,
        yd - ya - ya_ddx*d,
        yd_ddx  - ya_ddx
    );
    mat4 known_factors = transpose(
        mat4(
            spline_sample_factors    (exponents, b),
            spline_sample_factors    (exponents, c),
            spline_sample_factors    (exponents, d),
            spline_derivative_factors(exponents, d)
        )
    );
    vec4 k2k3k4k5 = inverse(known_factors) * known_outputs;
    return float[6](ya, ya_ddx, k2k3k4k5.x, k2k3k4k5.y, k2k3k4k5.z, k2k3k4k5.w);
}


float[6] quintic_spline(
        in float xa,       in float xb,
        in float ya,       in float yb,
        in float ya_ddx,   in float yb_ddx,
        in float ya_d2dx2, in float yb_d2dx){
    float b = xb-xa;
    vec3 exponents = vec3(3,4,5);
    vec3 known_outputs = vec3(
        yb      - ya - ya_ddx*b -      ya_d2dx2*b*b,
        yb_ddx       - ya_ddx   -  2.0*ya_d2dx2*b,
        yb_d2dx      -          -      ya_d2dx2
    );
    mat3 known_factors = transpose(
        mat3(
            spline_sample_factors        (exponents, b),
            spline_derivative_factors    (exponents, b),
            spline_2nd_derivative_factors(exponents, b)
        )
    );
    vec3 k3k4k5 = inverse(known_factors) * known_outputs;
    return float[6](ya, ya_ddx, ya_d2dx2, k3k4k5.x, k3k4k5.y, k3k4k5.z);
}

float[6] quintic_spline_with_known_midpoint(
        in float xa,     in float xb,     in float xc,
        in float ya,     in float yb,     in float yc,
        in float ya_ddx, in float yb_ddx, in float yc_ddx){
    float b = xb-xa;
    float c = xc-xa;
    vec4 exponents = vec4(2,3,4,5);
    vec4 known_outputs = vec4(
        yb - ya - ya_ddx*b,
        yc - ya - ya_ddx*c,
        yb_ddx  - ya_ddx,
        yc_ddx  - ya_ddx
    );
    mat4 known_factors = transpose(
        mat4(
            spline_sample_factors    (exponents, b),
            spline_sample_factors    (exponents, c),
            spline_derivative_factors(exponents, b),
            spline_derivative_factors(exponents, c)
        )
    );
    vec4 k2k3k4k5 = inverse(known_factors) * known_outputs;
    return float[6](ya, ya_ddx, k2k3k4k5.x, k2k3k4k5.y, k2k3k4k5.z, k2k3k4k5.w);
}
