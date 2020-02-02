float guess_distance_of_3d_point_to_ellipsoid(in vec3 A0, in vec3 B0, in vec3 R)
{
    vec3  V  = A0-B0;
    float u = length(V/R);
    float v = length(V/(R*R));
    return u*(u-1.0)/v;
}
