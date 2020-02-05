
float test_min(in float x)
{
    float u = 2.*x*x;
    return sqrt(u);
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
float test_sin(in float x)
{
    float u = 2.*x*x;
    return sin(u);
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
float test_if_statement(in vec2 x)
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


vec2 test_vec_x_vec(in float u)
{
    vec2 G = 2.f*u;
    vec2 H = 2.f*G.x;
    return G;
}
vec2 test_vec_0_vec(in float u)
{
    vec2 G = 2.f*u;
    vec2 H = 2.f*G[0];
    return G;
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
