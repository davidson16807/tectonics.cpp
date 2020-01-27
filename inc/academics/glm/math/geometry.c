
// See: http://www.lighthouse3d.com/tutorials/maths/ray-sphere-intersection/
void get_relation_between_ray_and_point(
    in vec3   point_position, 
    in vec3   ray_origin, 
    in vec3   V, 
    out float z2,
    out float xz 
){
    vec3 P = point_position - ray_origin;
    
    xz = dot(P, V);
    z2 = dot(P, P) - xz * xz;
}

// NOTE: GRANDFATHERED
bool try_relation_between_ray_and_sphere(
    in float  sphere_radius,
    in float  z2,
    in float  xz, 
    out float distance_to_entrance,
    out float distance_to_exit
){
    float sphere_radius2 = sphere_radius * sphere_radius;

    float distance_from_closest_approach_to_exit = sqrt(max(sphere_radius2 - z2, 1e-10));
    distance_to_entrance = xz - distance_from_closest_approach_to_exit;
    distance_to_exit     = xz + distance_from_closest_approach_to_exit;

    return (distance_to_exit > 0. && z2 < sphere_radius*sphere_radius);
}

float get_perimeter_of_circle(
    in float radius
) {
    return 2.*PI*radius;
}
float get_area_of_circle(
    in float radius
) {
    return PI*radius*radius;
}
float get_perimeter_of_triangle(
    in vec2 vertex1,
    in vec2 vertex2,
    in vec2 vertex3,
) {
    return length(vertex1-vertex2) + length(vertex2-vertex3) + length(vertex3-vertex1);
}
float get_area_of_triangle(
    in vec2 vertex1,
    in vec2 vertex2,
    in vec2 vertex3,
) {
    // half the magnitude of the cross product
	return 0.5f * abs((vertex1.x*(vertex2.y-vertex3.y) + vertex2.x*(vertex3.y-vertex1.y)+ vertex3.x*(vertex1.y-vertex2.y)));
}
float get_surface_area_of_sphere(
    in float radius
) {
    return 4.*PI*radius*radius;
}
float get_volume_of_sphere(
    in float radius
) {
    return 4./3.*PI*radius*radius*radius;
}
float get_surface_area_of_tetrahedron(
    in vec3 vertex1,
    in vec3 vertex2,
    in vec3 vertex3,
    in vec3 vertex4
) {
    // each face is half the magnitude of the cross product
    return 0.5f * (
        length(cross(vertex1-vertex2, vertex1-vertex3)) + 
        length(cross(vertex1-vertex2, vertex1-vertex4)) +
        length(cross(vertex1-vertex3, vertex1-vertex4)) +
        length(cross(vertex2-vertex3, vertex2-vertex4)) 
    );
}
float get_volume_of_tetrahedron(
    in vec3 vertex1,
    in vec3 vertex2,
    in vec3 vertex3,
    in vec3 vertex4
) {
    // 1/6 the volume of a parallelipiped, which is the scalar triple product of its edges
    return dot(cross(vertex1-vertex2, vertex1-vertex3), vertex1-vertex4) / 6.f;
}


/*
DESIGN PRINCIPLES:
* Functionally pure: 
  Output is determined solely from the input, global variables are never used
  "underscored_lower_case" is used to indicate functional purity
* Performant:
  Suitable for use in shader code
* Compositional:
  Given several functions that return intersections for a simpler geometry,
   it should be easy and performant to construct similar functions for more complex geometry.
  It is much easier to achieve this objective if functions return 
   the distance along lines at which intersection occurs. 
  If a complex geometry is created from the union of two simpler geometries,
   intersections with the complex geometry can then be easily and performantly found
   by taking the min() and max() of the distances that intersect the simpler geometries. 
  If no intersection is found with a simpler geometry, the function should return NAN,
   since GLSL defines that min(NAN,x) = x and max(NAN,x) = x
  If a complex geometry is created from the intersection of two simpler geometries,
   intersections can be easily and performantly found by testing 
   whether the intersection with one geometry is also a part of the other geometry.
  This requires that there be a separate set of functions testing whether 
   a point is bounded by a certain region. 
* Limited Scope:
  Only two kinds of functions should be implemented.
  The first kind finds the intersection or closest approach between a line and another shape (point, line, plane, volume)
  The second kind determines whether a point exists within another shape (namely areas in 2d and volumes in 3d).
  This is the minimum needed to find line intersections with geometries defined by intersections and unions, as mentioned above.
  Implementing other kinds of functions (e.g. finding the intersection between two volumes) is not likely to be useful within a shader
* Language agnostic: 
  Transpiles to any C-family language that supports pass-by-reference and glm style functionality (C, C++, GLSL)
  Javascript cannot be used because it does not allow glm style operator overloads.
  R cannot be used because it does not allow passing parameters by reference
  Avoids language specific features like function overloading
  Avoids language specific patterns or concepts (e.g. function chaining, OOP, unless that conflicts with the above)
* Versatile:
  Allows use with external data structures without requiring conversion between data structures.
  Avoids use of custom data structures, only glm vectors and primitives may be used.
* Obvious: 
  Behavior can be deduced from the function signature alone,
  Function names err on the side of verbosity, 
  Function names explain their exact intent,
  They do not abbreviate and do not avoid using prepositions or conjunctions
  Function names can be reconstructed after seeing only a few examples (unless that conflicts with the above)
  Behavior is consistant across all functions expressing geometric relations
  Spatial input makes no assumptions about the reference frame, 
  No concepts or standards need to be introduced to the user before usage (e.g. function chaining, sentinel values, other standards)
  No data structures need to be introduced to the user before usage, with the exception of vecNs
*/

// 2D FUNCTIONS CHECKING IF POINT IS IN REGION
bool is_2d_point_inside_circle(in vec2 A0, in vec2 B0, in float r)
{
	return length(B0-A0) < r;
}
// NOTE: in this case, N only needs to indicate the direction facing outside, 
//  it need not be perfectly normal to B
bool is_2d_point_inside_region_bounded_by_line(in vec2 A0, in vec2 B0, in vec2 N)
{
	return dot(A0-B0, N) < 0.;
}

// 2D FUNCTIONS RETURNING SINGLE INTERSECTIONS / CLOSEST APPROACHES
float get_distance_along_2d_line_nearest_to_point(in vec2 A0, in vec2 A, in vec2 B0)
{
	return dot(B0-A0, A);
}
float get_distance_along_2d_line_intersecting_line(in vec2 A0, in vec2 A, in vec2 B0, in vec2 B)
{
    vec2 D = B0-A0;             // offset
	vec2 R = D - dot(D, A) * A; // rejection
	return abs(abs(dot(A, B))-1) > 0? length(R) / dot(B, normalize(-R))  :  NAN;
}
float get_distance_along_2d_line_intersecting_ray(in vec2 A0, in vec2 A, in vec2 B0, in vec2 B)
{
    vec2  D  = B0-A0;                             // offset
	vec2  R  = D - dot(D,A) * A;                  // rejection
	float xB = length(R) / dot(B, normalize(-R)); // distance along B
	float xA = xB / dot(B,A);                     // distance along A
	return abs(abs(dot(A, B))-1) > 0 && xA > 0.? xB :  NAN;
}
float get_distance_along_2d_line_intersecting_line_segment(in vec2 A0, in vec2 A, in vec2 B0, in vec2 B1)
{
	vec2  B  = normalize(B1-B0);
    vec2  D  = B0-A0;                             // offset
	vec2  R  = D - dot(D,A) * A;                  // rejection
	float xB = length(R) / dot(B, normalize(-R)); // distance along B
	float xA = xB / dot(B,A);                     // distance along A
	return abs(abs(dot(A, B))-1) > 0. && 
	       0. < xA && xA < length(B1-B0)? xB : NAN;
}

// 2D FUNCTIONS RETURNING MULTIPLE INTERSECTIONS / CLOSEST APPROACHES
void get_distances_along_2d_line_intersecting_circle(in vec2 A0, in vec2 A, in vec2 B0, in float r, out float entrance, out float exit)
{
    vec2  O  = B0-A0;
    float xz = dot(O, A);
    float z2 = dot(O, O) - xz * xz;
    float y2 = r*r - z2;
    float dxr = sqrt(max(y2, 1e-10));
    entrance = xz - dxr;
    exit     = xz + dxr;
	return y2 > 0.;
}
void get_distances_along_2d_line_intersecting_triangle(in vec2 A0, in vec2 A, in vec2 B1, in vec2 B2, in vec2 B3, out float entrance, out float exit)
{
	float x1 = get_distance_along_2d_line_intersecting_line_segment(A0, A, B1, B2);
	float x2 = get_distance_along_2d_line_intersecting_line_segment(A0, A, B2, B3);
	float x3 = get_distance_along_2d_line_intersecting_line_segment(A0, A, B3, B1);
	entrance = min(x1, min(x2, x3));
	exit     = max(x1, max(x2, x3));
}

// 3D FUNCTIONS CHECKING IF POINT IS IN REGION
bool is_3d_point_inside_sphere(in vec3 A0, in vec3 B0, in float r)
{
	return length(B0-A0) < r;
}
bool is_3d_point_inside_region_bounded_by_plane(in vec3 A0, in vec3 B0, in vec3 N)
{
	return dot(A0-B0, N) < 0.;
}

// 3D FUNCTIONS RETURNING SINGLE INTERSECTIONS / CLOSEST APPROACHES
float get_distance_along_3d_line_nearest_to_point(in vec3 A0, in vec3 A, in vec3 B0)
{
	return dot(B0-A0, A);
}
float get_distance_along_3d_line_nearest_to_line(in vec3 A0, in vec3 A, in vec3 B0, in vec3 B)
{
    vec3 D  = B0-A0;                            // offset
    vec3 R = D - dot(D, A) * A - dot(D, C) * C; // rejection
	return abs(abs(dot(A, B))-1) > 0.? length(R) / -dot(B, normalize(R))  :  NAN;
}
float get_distance_along_3d_line_nearest_to_ray(in vec3 A0, in vec3 A, in vec3 B0, in vec3 B)
{
    vec3  D  = B0-A0;                             // offset
	vec3  R  = D - dot(D,A) * A;                  // rejection
	float xB = length(R) / dot(B, normalize(-R)); // distance along B
	float xA = xB / dot(B,A);                     // distance along A
	return abs(abs(dot(A, B))-1) > 0. && xA > 0.?  xB :  NAN;
}
float get_distance_along_3d_line_nearest_to_line_segment(in vec3 A0, in vec3 A, in vec3 B0, in vec3 B1)
{
	vec3  B  = normalize(B1-B0);
    vec3  D  = B0-A0;                             // offset
	vec3  R  = D - dot(D,A) * A;                  // rejection
	float xB = length(R) / dot(B, normalize(-R)); // distance along B
	float xA = xB / dot(B,A);                     // distance along A
	return abs(abs(dot(A, B))-1) > 0. && 
		0. < xA && xA < length(B1-B0)?  xB  :  NAN;
}
float get_distance_along_3d_line_intersecting_plane(in vec3 A0, in vec3 A, in vec3 B0, in vec3 N)
{
    return abs(dot(A, N)) > 0.? dot(B0-A0, N) / dot(A,N)  :  NAN;
}
float get_distance_along_3d_line_intersecting_circle(in vec3 A0, in vec3 A, in vec3 B0, in vec3 N, in float r)
{
	// intersection(plane, sphere)
	float x;
    x = get_distance_along_3d_line_intersecting_plane(A0, A, B0, N);
    x = is_3d_point_inside_sphere(A0+A*x, B0, r)? x : NAN;
    return x;
}
float get_distance_along_3d_line_intersecting_triangle(in vec3 A0, in vec3 A, in vec3 B1, in vec3 B2, in vec3 B3)
{
	// intersection(face plane, edge plane, edge plane, edge plane)
	vec3  B0 = (B1 + B2 + B3) / 3.;
	vec3  N  = normalize(cross(B1-B2, B2-B3));
    float x  = get_distance_along_3d_line_intersecting_plane(A0, A, B0, N);
    vec3  Ax = A0+A*x;
    x = is_3d_point_inside_region_bounded_by_plane(Ax, B1, cross(B1-B2,N)) == 
	    is_3d_point_inside_region_bounded_by_plane(Ax, B2, cross(B2-B3,N)) == 
	    is_3d_point_inside_region_bounded_by_plane(Ax, B3, cross(B3-B1,N)) ? x : NAN;
    return x;
}

// 3D FUNCTIONS RETURNING MULTIPLE INTERSECTIONS / CLOSEST APPROACHES
void get_distances_along_3d_line_intersecting_sphere(in vec3 A0, in vec3 A, in vec3 B0, in float r, out float entrance, out float exit)
{
    float xz = dot(B0-A0, A);
    float z  = length(A0+A*xz - B0);
    float y2  = r*r-z*z;
    float dxr = sqrt(max(y2, 1e-10));
    entrance = y2 > 0.?  xz - dxr  :  NAN;
    exit     = y2 > 0.?  xz + dxr  :  NAN;
}
void get_distances_along_3d_line_intersecting_tetrahedron(in vec3 A0, in vec3 A, in vec3 B1, in vec3 B2, in vec3 B3, in vec3 B4, out float entrance, out float exit)
{
	float x1 = get_distance_along_3d_line_intersecting_triangle(A0,A,B1,B2,B3);
	float x2 = get_distance_along_3d_line_intersecting_triangle(A0,A,B2,B3,B4);
	float x3 = get_distance_along_3d_line_intersecting_triangle(A0,A,B3,B4,B1);
	float x4 = get_distance_along_3d_line_intersecting_triangle(A0,A,B4,B1,B2);
    entrance = min(x1, min(x2, min(x3, x4)));
    exit     = max(x1, max(x2, max(x3, x4)));
}
void get_distances_along_3d_line_intersecting_infinite_cylinder(in vec3 A0, in vec3 A, in vec3 B0, in vec3 B, in float r, out float entrance, out float exit)
{
    // simplify the problem by using a coordinate system based around the line and the tube center
    // see closest-approach-between-line-and-cylinder-visualized.scad
    vec3  J   = B;
    vec3  K   = normalize(cross(A,B));
    vec3  I   =-normalize(cross(K,B));
    vec3  O   = A0-B0;
    float z   = dot(O,K);
    float xz  = dot(O,I);
    float AK  = dot(A,I);
    float y2  = r*r-z*z;
    float dxr = sqrt(max(y2, 1e-10));
    entrance  = y2 > 0.?  (xz-dxr) / AK  :  NAN;
    exit      = y2 > 0.?  (xz+dxr) / AK  :  NAN;
}
void get_distances_along_3d_line_intersecting_tube(in vec3 A0, in vec3 A, in vec3 B0, in vec3 B1, in float r, out float entrance, out float exit)
{
	vec3  B   = normalize(B1-B0);
	float xr0, xr1, xB0, xB1; 
	// intersection(cylinder, plane, plane)
	get_distances_along_3d_line_intersecting_infinite_cylinder (A0, A, B0, B, r, xr0, xr1); 
	xr0 = is_3d_point_inside_region_bounded_by_plane           (A0+A*xr0, B0, B)? xr0 : NAN;
	xr1 = is_3d_point_inside_region_bounded_by_plane           (A0+A*xr1, B1,-B)? xr1 : NAN;
	entrance  = min(xr0, xr1);
	exit      = max(xr0, xr1);
}
void get_distances_along_3d_line_intersecting_cylinder(in vec3 A0, in vec3 A, in vec3 B0, in vec3 B1, in float r, out float entrance, out float exit)
{
	vec3  B   = normalize(B1-B0);
	float xr0, xr1, xB0, xB1; 
	// union(tube, circle, circle)
	get_distances_along_3d_line_intersecting_tube        (A0, A, B0, B, r, xr0, xr1); 
	xB0 = get_distance_along_3d_line_intersecting_circle (A0, A, B0,-B, r);
	xB1 = get_distance_along_3d_line_intersecting_circle (A0, A, B1, B, r);
	entrance  = min(xr0, min(xr1, min(xB0, xB1)));
	exit      = max(xr0, max(xr1, max(xB0, xB1)));
}
void get_distances_along_3d_line_intersecting_capsule(in vec3 A0, in vec3 A, in vec3 B0, in vec3 B1, in float r, out float entrance, out float exit)
{
	vec3 B = normalize(B1-B0);
	float xr0, xr1, xB0, xB1; 
	// union(tube, sphere, sphere)
	get_distances_along_3d_line_intersecting_tube        (A0, A, B0, B, r, xr0, xr1); 
	xB0 = get_distance_along_3d_line_intersecting_sphere (A0, A, B0, r); 
	xB1 = get_distance_along_3d_line_intersecting_sphere (A0, A, B1, r);
	entrance = min(xr0, min(xr1, min(xB0, xB1)));
	exit     = max(xr0, max(xr1, max(xB0, xB1)));
}
