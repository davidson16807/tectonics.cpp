
float test_variable_declaration(in float x)
{
    float u = x;
    return x;
}
float test_assignment_expression(in float x)
{
    float u;
    u = x;
    return u;
}
float test_constant(in float x, in float k)
{
    return k;
}
float test_x(in float x)
{
    return x;
}
float test_mult_by_constant(in float x, in float k)
{
    return k*x;
}
float test_add(in float x)
{
    float u = 2.*x*x;
    float v = 3.*x*x*x;
    return u+v;
}
float test_sub(in float x)
{
    float u = 2.*x*x;
    float v = 3.*x*x*x;
    return u-v;
}
float test_mult(in float x)
{
    float u = 2.*x*x;
    float v = 3.*x*x*x;
    return u*v;
}
float test_div(in float x)
{
    float u = 2.*x*x;
    float v = 3.*x*x*x;
    return u/v;
}
float test_inverse(in float x)
{
    float u = 2.*x*x;
    return 1.0f/u;
}
float test_sqrt(in float x)
{
    float u = 2.*x*x;
    return sqrt(u);
}
float test_chain_rule(in float x)
{
    float u = 2.*x*x;
    return f(u);
}
float test_log(in float x)
{
    float u = 2.*x*x;
    return log(u);
}
float test_exp(in float x)
{
    float u = 2.*x*x;
    return exp(u);
}
float test_pow_constant(in float x, in float k)
{
    float u = 2.*x*x;
    return pow(k, u);
}
float test_pow_constant(in float x)
{
    float u = 2.*x*x;
    float v = 3.*x*x*x;
    return pow(u, v);
}
float test_sin(in float x)
{
    float u = 2.*x*x;
    return sin(u);
}
float test_cos(in float x)
{
    float u = 2.*x*x;
    return cos(u);
}
float test_tan(in float x)
{
    float u = 2.*x*x;
    return tan(u);
}
float test_asin(in float x)
{
    float u = 2.*x*x;
    return asin(u);
}
float test_acos(in float x)
{
    float u = 2.*x*x;
    return acos(u);
}
float test_atan(in float x)
{
    float u = 2.*x*x;
    return atan(u);
}
float test_abs(in float x)
{
    float u = 2.*x*x;
    return abs(u);
}
float test_min(in float x, in float y)
{
    float u = 2.*x*x;
    float v = 2.*y*y;
    return min(u, v);
}
float test_length(in vec2 X)
{
    vec2 U = 2.*X*X;
    return length(U);
}

float test_dot(in vec2 X)
{
    vec2 U = 2.*X*X;
    vec2 V = 0.5*X*X*X;
    return dot(U,V);
}
float test_ternary(in float x)
{
    float u = 2.*x*x;
    float v = 0.5*x*x*x;
    return u < v ? u:v;
}
float test_if_statement(in float x)
{
    float u = 2.*x*x;
    float v = 0.5*x*x*x;
    if (u < v)
    {
        return u;
    } else {
        return v;
    }
}

float test_vec_x_float(in vec2 X)
{
    vec2 U = 2.*X*X;
    return sqrt(U.x);
}
float test_vec_0_float(in vec2 X)
{
    vec2 U = 2.*X*X;
    return sqrt(U[0]);
}

float test_constant(in vec2 X)
{
    vec2 U = vec2(1.f);
    return sqrt(U.x);
}
float ellipsoid(in vec3 A0, in vec3 B0, in vec3 R)
{
    vec3  V  = A0-B0;
    float u = length((V/R).x);
    float v = length(V/(R*R));
    return u*(u-1.0)/v;
}

float test_constructor(in float X)
{
    vec2 U = vec2(x);
    return sqrt(U.x);
}
float test_primitive_output_parameter(in float x, out float y)
{
    y = 2.f*x*x;
}
vec3 test_unsupported_swizzling(in vec3 X)
{
    return X.zyx;
}
vec2 test_unsupported_type(in vec3 X)
{
    return X.zyx;
}
float test_return_statement_type_check(in float x)
{
    return true;
}
float test_variable_declaration_type_check(in float x)
{
    bool u = x;
    return x;
}
float test_assignment_expression_type_check(in float x)
{
    bool u;
    u = x;
    return x;
}