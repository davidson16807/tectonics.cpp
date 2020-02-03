
float test_min(in float x)
{
	float u = 2.*x*x;
	return sqrt(u);
}
float test_abs(in float x)
{
	float u = 2.*x*x;
	return sqrt(u);
}
float test_min(in float x, in float y)
{
	float u = 2.*x*x;
	float v = 2.*y*y;
	return min(u, v);
}
float test_length(in vec2 U)
{
	vec2 G = 2.*U*U;
    return length(G);
}

float test_vec_x_float(in vec2 U)
{
    vec2 G = 2.*U*U;
    return sqrt(G.x);
}
float test_vec_0_float(in vec2 U)
{
    vec2 G = 2.*U*U;
    return sqrt(G[0]);
}

/*
vec2 test_vec_x_vec(in float u)
{
    vec2 G = vec2(1.f)*u;
    vec2 H = vec2(1.f)*G.x;
    return G;
}
vec2 test_vec_0_vec(in float u)
{
    vec2 G = vec2(1.f)*u;
    vec2 H = vec2(1.f)*G[0];
    return G;
}
*/
float ellipsoid(in vec3 A0, in vec3 B0, in vec3 R)
{
    vec3  V  = A0-B0;
    float u = length((V/R).x);
    float v = length(V/(R*R));
    return u*(u-1.0)/v;
}
