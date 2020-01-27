

// NOTE: GRANDFATHERED
bool try_relation_between_ray_and_sphere(
    float  sphere_radius,
    float  z2,
    float  xz, 
    float distance_to_entrance,
    float distance_to_exit
){
    float sphere_radius2 = sphere_radius * sphere_radius;

    float distance_from_closest_approach_to_exit = sqrt(max(sphere_radius2 - z2, 1e-10));
    distance_to_entrance = xz - distance_from_closest_approach_to_exit;
    distance_to_exit     = xz + distance_from_closest_approach_to_exit;

    return (distance_to_exit > 0. && z2 < sphere_radius*sphere_radius);
}





bool try_distance_along_line_to_union(
    bool is_intersection1, float entrance1,
    bool is_intersection2, float entrance2,
    float entrance
) {
    entrance = !is_intersection1 ? entrance2 : !is_intersection2 ? entrance1 : min(entrance1, entrance2);
    return is_intersection1 || is_intersection2;
}

bool try_distances_along_line_to_negation(
    bool is_intersection1, float entrance1, float exit1, 
    bool is_intersection2, float entrance2, float exit2,
    float entrance,
    float exit
) {
    if (!is_intersection1)
    {
      return false;
    }
    entrance = is_intersection2 || entrance1 < entrance2 ? entrance1 : max(exit2,     entrance1);
    exit     = is_intersection2 || exit1     < exit2     ? exit1     : max(entrance2, exit1    );
    return !is_intersection2 || entrance1 < entrance2 || exit2 < exit1;
}

bool try_distances_along_line_to_union(
    bool is_intersection1, float entrance1, float exit1, 
    bool is_intersection2, float entrance2, float exit2,
    float entrance,
    float exit
) {
    entrance = !is_intersection1 ? entrance2 : !is_intersection2 ? entrance1 : min(entrance1, entrance2);
    exit     = !is_intersection1 ? exit2     : !is_intersection2 ? exit1     : max(exit1,     exit2    );
    return is_intersection1 || is_intersection2;
}

bool try_distances_along_line_to_intersection(
    bool is_intersection1, float entrance1, float exit1, 
    bool is_intersection2, float entrance2, float exit2,
    float entrance,
    float exit
) {
    entrance = is_intersection1 && is_intersection2 ? max(entrance1, entrance2) : 0.f;
    exit     = is_intersection1 && is_intersection2 ? min(exit1,     exit2    ) : 0.f;
    return is_intersection1 && is_intersection2 && entrance < exit;
}

// 2D FUNCTIONS RETURNING SINGLE INTERSECTIONS / CLOSEST APPROACHES
/*
A0 line reference
A  line direction, normalized
B0 point position
*/
float get_distance_along_2d_line_nearest_to_point(vec2 A0, vec2 A, vec2 B0){
    return dot(B0-A0, A);
}
/*
A0 line reference
A  line direction, normalized
B0 line reference
B  line direction, normalized
*/
bool try_distance_along_2d_line_to_line(vec2 A0, vec2 A, vec2 B0, vec2 B, float intersection){
    vec2 D = B0-A0;             // offset
    vec2 R = D - dot(D, A) * A; // rejection
    intersection = length(R) / dot(B, normalize(-R));
    return abs(abs(dot(A, B))-1.f) > 0.f;
}
/*
A0 line reference
A  line direction, normalized
B0 ray origin
B  ray direction, normalized
*/
bool try_distance_along_2d_line_to_ray(vec2 A0, vec2 A, vec2 B0, vec2 B, float intersection){
    // INTUITION: same as the line-line intersection, but now results are only valid if distance > 0
    vec2  D  = B0-A0;                             // offset
    vec2  R  = D - dot(D,A) * A;                  // rejection
    float xB = length(R) / dot(B, normalize(-R)); // distance along B
    float xA = xB / dot(B,A);                     // distance along A
    intersection = xB;
    return abs(abs(dot(A, B))-1.f) > 0.f && xA > 0.f;
}
/*
A0 line reference
A  line direction, normalized
B0 line segment endpoint 1
B1 line segment endpoint 2
*/
bool try_distance_along_2d_line_to_line_segment(vec2 A0, vec2 A, vec2 B1, vec2 B2, float intersection){
    // INTUITION: same as the line-line intersection, but now results are only valid if 0 < distance < |B2-B1|
    vec2  B  = normalize(B2-B1);
    vec2  D  = B1-A0;                             // offset
    vec2  R  = D - dot(D,A) * A;                  // rejection
    float xB = length(R) / dot(B, normalize(-R)); // distance along B
    float xA = xB / dot(B,A);                     // distance along A
    intersection = xB;
    return abs(abs(dot(A, B))-1.f) > 0.f && 
           0. < xA && xA < length(B2-B1);
}

// 2D FUNCTIONS RETURNING MULTIPLE INTERSECTIONS / CLOSEST APPROACHES
/*
A0 line reference
A  line direction, normalized
B0 circle origin
B  circle radius
*/
bool try_distances_along_2d_line_to_circle(vec2 A0, vec2 A, vec2 B0, float r, float entrance, float exit){
    vec2  D  = B0-A0;
    float xz = dot(D, A);
    float z2 = dot(D, D) - xz * xz;
    float y2 = r*r - z2;
    float dxr = sqrt(max(y2, 1e-10));
    entrance = xz - dxr;
    exit     = xz + dxr;
    return y2 > 0.;
}
/*
A0 line reference
A  line direction, normalized
B1 vertex position 1
B2 vertex position 2
*/
bool try_distances_along_2d_line_to_triangle(vec2 A0, vec2 A, vec2 B1, vec2 B2, vec2 B3, float entrance, float exit){
    float x1; bool hit1 = try_distance_along_2d_line_to_line_segment(A0, A, B1, B2, x1);
    float x2; bool hit2 = try_distance_along_2d_line_to_line_segment(A0, A, B2, B3, x1);
    float x3; bool hit3 = try_distance_along_2d_line_to_line_segment(A0, A, B3, B1, x1);
    entrance = min(x1, min(x2, x3));
    exit     = max(x1, max(x2, x3));
    return hit1 || hit2 || hit3;
}

// 3D FUNCTIONS RETURNING SINGLE INTERSECTIONS / CLOSEST APPROACHES
/*
A0 line reference
A  line direction, normalized
B0 point position
*/
float get_distance_along_3d_line_nearest_to_point(vec3 A0, vec3 A, vec3 B0){
    return dot(B0-A0, A);
}
/*
A0 line reference
A  line direction, normalized
B0 line reference
B  line direction, normalized
*/
bool try_distance_along_3d_line_nearest_to_line(vec3 A0, vec3 A, vec3 B0, vec3 B, float closest_approach){
    vec3 D  = B0-A0;                            // offset
    vec3 C  = normalize(cross(B,A));            // cross
    vec3 R = D - dot(D, A) * A - dot(D, C) * C; // rejection
    closest_approach = length(R) / -dot(B, normalize(R));
    return abs(abs(dot(A, B))-1.f) > 0.f;
}
/*
A0 line reference
A  line direction, normalized
B0 ray origin
B  ray direction, normalized
*/
bool try_distance_along_3d_line_nearest_to_ray(vec3 A0, vec3 A, vec3 B0, vec3 B, float closest_approach){
    vec3  D  = B0-A0;                             // offset
    vec3  R  = D - dot(D,A) * A;                  // rejection
    float xB = length(R) / dot(B, normalize(-R)); // distance along B
    float xA = xB / dot(B,A);                     // distance along A
    closest_approach = xB;
    return abs(abs(dot(A, B))-1.f) > 0.f && xA > 0.f;
}
/*
A0 line reference
A  line direction, normalized
B1 line segment endpoint 1
B2 line segment endpoint 2
*/
bool try_distance_along_3d_line_nearest_to_line_segment(vec3 A0, vec3 A, vec3 B0, vec3 B1, float closest_approach){
    vec3  B  = normalize(B1-B0);
    vec3  D  = B0-A0;                             // offset
    vec3  R  = D - dot(D,A) * A;                  // rejection
    float xB = length(R) / dot(B, normalize(-R)); // distance along B
    float xA = xB / dot(B,A);                     // distance along A
    closest_approach = xB;
    return abs(abs(dot(A, B))-1.f) > 0.f && 
        0. < xA && xA < length(B1-B0);
}
/*
A0 line reference
A  line direction, normalized
B0 plane reference
N  plane surface normal, normalized
*/
float get_distance_along_3d_line_to_plane(vec3 A0, vec3 A, vec3 B0, vec3 N){
    return -dot(A0-B0, N) / dot(A,N);
}
/*
A0 line reference
A  line direction, normalized
B0 circle origin
N  circle surface normal, normalized
r  circle radius
*/
bool try_distance_along_3d_line_to_circle(vec3 A0, vec3 A, vec3 B0, vec3 N, float r, float intersection){
    // intersection(plane, sphere)
    float t = get_distance_along_3d_line_to_plane(A0, A, B0, N);
    intersection = t;
    return is_3d_point_in_sphere(A0+A*t, B0, r);
}
/*
A0 line reference
A  line direction, normalized
B1 vertex position 1
B2 vertex position 2
B3 vertex position 3
*/
bool try_distance_along_3d_line_to_triangle(vec3 A0, vec3 A, vec3 B1, vec3 B2, vec3 B3, float intersection){
    // intersection(face plane, edge plane, edge plane, edge plane)
    vec3  B0 = (B1 + B2 + B3) / 3.;
    vec3  N  = normalize(cross(B1-B2, B2-B3));
    float t  = get_distance_along_3d_line_to_plane(A0, A, B0, N);
    vec3  At = A0+A*t;
    intersection = t;
    vec3 B2B1hat = normalize(B2-B1);
    vec3 B3B2hat = normalize(B3-B2);
    vec3 B1B3hat = normalize(B1-B3);
    return dot(normalize(At-B1), B2B1hat) > dot(-B1B3hat, B2B1hat)
        && dot(normalize(At-B2), B3B2hat) > dot(-B2B1hat, B3B2hat)
        && dot(normalize(At-B3), B1B3hat) > dot(-B3B2hat, B1B3hat);
}

// 3D FUNCTIONS RETURNING MULTIPLE INTERSECTIONS / CLOSEST APPROACHES
/*
A0 line reference
A  line direction, normalized
B0 sphere origin
r  sphere radius
*/
bool try_distances_along_3d_line_to_sphere(vec3 A0, vec3 A, vec3 B0, float r, float entrance, float exit){
    float xz = dot(B0-A0, A);
    float z  = length(A0+A*xz - B0);
    float y2  = r*r-z*z;
    float dxr = sqrt(max(y2, 1e-10));
    entrance = xz - dxr;
    exit     = xz + dxr;
    return y2 > 0.;
}
/*
A0 line reference
A  line direction, normalized
B0 ellipsoid center
R  ellipsoid radius along each coordinate axis
*/
bool try_distance_along_3d_line_to_ellipsoid( vec3 A0, vec3 A, vec3 B0, vec3 R, float entrance )
{
    // NOTE: shamelessly copy pasted, all credit goes to Inigo: 
    // https://www.iquilezles.org/www/articles/intersectors/intersectors.htm
    vec3 Or = (A0-B0)/R;
    vec3 Ar = A/R;
    float ArAr = dot( Ar, Ar );
    float OrAr = dot( Or, Ar );
    float OrOr = dot( Or, Or );
    float h = OrAr*OrAr - ArAr*(OrOr-1.0);
    entrance = (-OrAr-sqrt(h)) / ArAr;
    return h >= 0.0;
}
/*
A0 line reference
A  line direction, normalized
B1 vertex position 1
B2 vertex position 2
B3 vertex position 3
B4 vertex position 4
*/
bool try_distances_along_3d_line_to_tetrahedron(vec3 A0, vec3 A, vec3 B1, vec3 B2, vec3 B3, vec3 B4, float entrance, float exit){
    float t1; bool hit1 = try_distance_along_3d_line_to_triangle(A0,A,B1,B2,B3, t1);
    float t2; bool hit2 = try_distance_along_3d_line_to_triangle(A0,A,B2,B3,B4, t2);
    float t3; bool hit3 = try_distance_along_3d_line_to_triangle(A0,A,B3,B4,B1, t3);
    float t4; bool hit4 = try_distance_along_3d_line_to_triangle(A0,A,B4,B1,B2, t4);
    float t_in, t_out;  bool hit;
    hit = try_distances_along_line_to_union(hit1,t1,  t1,    hit2,t2,t2, t_in, t_out);
    hit = try_distances_along_line_to_union(hit, t_in,t_out, hit3,t3,t3, t_in, t_out);
    hit = try_distances_along_line_to_union(hit, t_in,t_out, hit4,t4,t4, t_in, t_out);
    entrance = t_in;
    exit     = t_out;
    return hit;
}
/*
A0 line reference
A  line direction, normalized
B0 cylinder reference
B  cylinder direction, normalized
r  cylinder radius
*/
bool try_distances_along_3d_line_to_infinite_cylinder(vec3 A0, vec3 A, vec3 B0, vec3 B, float r, float entrance, float exit){
    // INTUITION: simplify the problem by using a coordinate system based around the line and the tube center
    // see closest-approach-between-line-and-cylinder-visualized.scad
    // implementation shamelessly copied from Inigo: 
    // https://www.iquilezles.org/www/articles/intersectors/intersectors.htm
    vec3  D = A0 - B0;
    float BA = dot(B,A);
    float BD = dot(B,D);
    float a = 1.0 - BA*BA;
    float b = dot( D, A ) - BD*BA;
    float c = dot( D, D ) - BD*BD - r*r;
    float h = sqrt(max(b*b - a*c,0.f));
    entrance =(-b+h)/a;
    exit     =(-b-h)/a;
    return h>0.0;
}
/*
A0 line reference
A  line direction, normalized
B1 cylinder endpoint 1
B2 cylinder endpoing 2
r  cylinder radius
*/
bool try_distances_along_3d_line_to_cylinder(vec3 A0, vec3 A, vec3 B1, vec3 B2, float r, float entrance, float exit){
    vec3 B = normalize(B2-B1);
    float a1 = get_distance_along_3d_line_to_plane(A0,A,B1,B);
    float a2 = get_distance_along_3d_line_to_plane(A0,A,B2,B);
    float a_in  =  min(a1,a2);
    float a_out =  max(a1,a2);
    float b_in, b_out; bool b_hits = try_distances_along_3d_line_to_infinite_cylinder(A0,A, B1,B, r, b_in, b_out);
    float c_in, c_out; bool c_hits = try_distances_along_line_to_intersection(b_hits, b_in, b_out, true, a_in, a_out, c_in, c_out);
    entrance = max(b_out, min(a1,a2));
    exit     = min(b_in,  max(a1, a2));
    return b_hits && entrance < exit;
}

/*
A0 line reference
A  line direction, normalized
B1 capsule endpoint 1
B2 capsule endpoing 2
r  capsule radius
*/
bool try_distances_along_3d_line_to_capsule(vec3 A0, vec3 A, vec3 B1, vec3 B2, float r, float entrance, float exit){
    float a_in, a_out; bool a_hits = try_distances_along_3d_line_to_cylinder(A0,A, B1,B2, r, a_in, a_out);
    float b_in, b_out; bool b_hits = try_distances_along_3d_line_to_sphere  (A0,A, B1,    r, b_in, b_out);
    float c_in, c_out; bool c_hits = try_distances_along_3d_line_to_sphere  (A0,A,    B2, r, c_in, c_out);
    float d_in, d_out; bool d_hits = try_distances_along_line_to_union   (a_hits, a_in, a_out, b_hits, b_in, b_out, d_in, d_out);
    float e_in, e_out; bool e_hits = try_distances_along_line_to_union   (d_hits, d_in, d_out, c_hits, c_in, c_out, e_in, e_out);
    entrance = e_in;
    exit     = e_out;
    return e_hits;
}
/*
A0 line reference
A  line direction, normalized
B1 ring endpoint 1
B2 ring endpoing 2
ro ring outer radius
ri ring inner radius
*/
bool try_distances_along_3d_line_to_ring(vec3 A0, vec3 A, vec3 B1, vec3 B2, float ro, float ri, float entrance, float exit){
    
    float a_in, a_out; bool a_hits = try_distances_along_3d_line_to_cylinder(A0, A, B1, B2, ro, a_in, a_out);
    float b_in, b_out; bool b_hits = try_distances_along_3d_line_to_cylinder(A0, A, B1, B2, ri, b_in, b_out);
    float c_in, c_out; bool c_hits = try_distances_along_line_to_negation   (a_hits, a_in, a_out, b_hits, b_in, b_out, c_in, c_out);
    entrance = c_in;
    exit     = c_out;
    return c_hits;
}



/*
A0 line reference
A  line direction, normalized
B0 cone vertex
B  cone direction, normalized
cosb cosine of cone half angle
*/
bool try_distance_along_3d_line_to_infinite_cone(vec3 A0, vec3 A, vec3 B0, vec3 B, float cosb, float entrance)
{

    vec3 D = A0 - B0;

    float a = dot(A,B)*dot(A,B) - cosb*cosb;
    float b = 2. * (dot(A,B)*dot(D,B) - dot(A,D)*cosb*cosb);
    float c = dot(D,B)*dot(D,B) - dot(D,D)*cosb*cosb;

    float det = b*b - 4.*a*c;
    if (det < 0.) 
    {
        return false
    };

    det = sqrt(det);
    float t1 = (-b - det) / (2. * a);
    float t2 = (-b + det) / (2. * a);

    // This is a bit messy; there ought to be a more elegant solution.
    float t = t1;
    if (t < 0. || t2 > 0. && t2 < t) 
    {
        t = t2;
    } else {
        t = t1;
    }

    vec3 cp = A0 + t*A - B0;
    float h = dot(cp, B);

    entrance = t;
    return t > 0. && h > 0.;
}
/*
A0 line reference
A  line direction, normalized
B0 cone vertex
B  cone direction, normalized
r  cone radius
h  cone height
*/
bool try_distance_along_3d_line_to_cone(vec3 A0, vec3 A, vec3 B0, vec3 B, float r, float h, float entrance)
{
    float a_in; bool a_hits = try_distance_along_3d_line_to_circle(A0,A, B0+B*h,B, r, a_in);
    float b_in; bool b_hits = try_distance_along_3d_line_to_infinite_cone(A0,A, B0,B, cos(atan(r/h)), b_in);
    b_hits = b_hits && dot(A0 + b_in*A - B0, B) <= h;
    float c_in, c_out; bool c_hits = try_distance_along_line_to_union (a_hits, a_in, b_hits, b_in, c_in);
    entrance = c_in;
    return c_hits;
}
/*
A0 line reference
A  line direction, normalized
B1 cone endpoint 1
B2 cone endpoint 2
r1 cone endpoint 1 radius
r2 cone endpoint 2 radius
*/
bool try_distance_along_3d_line_to_capped_cone
(
    vec3 A0, vec3 A, 
    vec3 B1, vec3 B2, float r1, float r2, 
    float entrance
){
    float dh   = length(B2-B1);
    float dr   = r2-r1;
    float rmax = max(r2,r1);
    float rmin = min(r2,r1);
    float hmax = rmax * dr/dh;
    float hmin = rmin * dr/dh;
    vec3  B    = sign(dr) * normalize(B2-B1);
    vec3  Bmax = (r2>r1? B2:B1);
    vec3  B0   = Bmax - B*hmax;
    vec3  Bmin = Bmax - B*hmin;

    float a_in; bool a_hits = try_distance_along_3d_line_to_circle(A0,A, Bmax, B, rmax, a_in);
    float b_in; bool b_hits = try_distance_along_3d_line_to_circle(A0,A, Bmin, B, rmin, b_in);
    float c_in; bool c_hits = try_distance_along_3d_line_to_infinite_cone(A0,A, B0,B, cos(atan(rmax/hmax)), c_in);
    float c_h = dot(A0 + c_in*A - B0, B);
    c_hits = c_hits && hmin <= c_h && c_h <= hmax; 
    float d_in; bool d_hits = try_distance_along_line_to_union (a_hits, a_in,  b_hits, b_in, d_in);
    float e_in; bool e_hits = try_distance_along_line_to_union (a_hits, a_in,  b_hits, b_in, e_in);
    entrance = c_in;
    return c_hits;
}
