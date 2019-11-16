
// See: http://www.lighthouse3d.com/tutorials/maths/ray-sphere-intersection/
FUNC(void) get_relation_between_ray_and_point(
    IN(vec3)   point_position, 
    IN(vec3)   ray_origin, 
    IN(vec3)   V, 
    OUT(float) z2,
    OUT(float) xz 
){
    VAR(vec3) P = point_position - ray_origin;
    
    xz = dot(P, V);
    z2 = dot(P, P) - xz * xz;
}

// NOTE: GRANDFATHERED
FUNC(bool) try_relation_between_ray_and_sphere(
    IN(float)  sphere_radius,
    IN(float)  z2,
    IN(float)  xz, 
    OUT(float) distance_to_entrance,
    OUT(float) distance_to_exit
){
    VAR(float) sphere_radius2 = sphere_radius * sphere_radius;

    VAR(float) distance_from_closest_approach_to_exit = sqrt(max(sphere_radius2 - z2, 1e-10));
    distance_to_entrance = xz - distance_from_closest_approach_to_exit;
    distance_to_exit     = xz + distance_from_closest_approach_to_exit;

    return (distance_to_exit > 0. && z2 < sphere_radius*sphere_radius);
}

/*
DESIGN PRINCIPLES:
* Functionally pure: 
  Output is determined solely from the input, global variables are never used
  "underscored_lower_case" is used to indicate functional purity
* Performant:
  Suitable for use in shader code
* Language agnostic: 
  Transpiles to any C-family language that supports pass-by-reference and glm style functionality (C, C++, GLSL)
  Javascript cannot be used because it does not allow glm style operator overloads.
  R cannot be used because it does not allow passing parameters by reference
  Avoids language specific features like function overloading
  Avoids language specific patterns or concepts (e.g. function chaining, OOP, unless that conflicts with the above)
* Versatile:
  Allows use with external data structures without requiring use of a "bridge" between data structures.
  Avoids use of custom data structures, only glm vectors and primitives may be used.
* Obvious: 
  Behavior can be deduced from the function signature alone,
  Function names err on the side of verbosity, 
  Function names explain their exact intent,
  They do not abbreviate and do not avoid using prepositions or conjunctions
  Function names can be reconstructed after seeing only a few examples (unless that conflicts with the above)
  Behavior is consistant across all functions expressing geometric relations
  Spatial input makes no assumptions about the reference frame, 
  VecNs are used whereever possible to reference a position or direction in space
  No concepts or standards need to be introduced to the user before usage (e.g. function chaining, sentinel values, other standards)
  No data structures need to be introduced to the user before usage, with the exception of vecNs
*/
FUNC(float) get_perimeter_of_circle(
    IN(float) radius
) {
    return 2.*PI*radius;
}
FUNC(float) get_area_of_circle(
    IN(float) radius
) {
    return PI*radius*radius;
}
FUNC(float) get_perimeter_of_triangle(
    IN(vec2) vertex1,
    IN(vec2) vertex2,
    IN(vec2) vertex3,
) {
    return length(vertex1-vertex2) + length(vertex2-vertex3) + length(vertex3-vertex1);
}
FUNC(float) get_area_of_triangle(
    IN(vec2) vertex1,
    IN(vec2) vertex2,
    IN(vec2) vertex3,
) {
	return abs((vertex1.x*(vertex2.y-vertex3.y) + vertex2.x*(vertex3.y-vertex1.y)+ vertex3.x*(vertex1.y-vertex2.y))/2.f);
}
FUNC(float) get_surface_area_of_sphere(
    IN(float) radius
) {
    return 4.*PI*radius*radius;
}
FUNC(float) get_volume_of_sphere(
    IN(float) radius
) {
    return 4./3.*PI*radius*radius*radius;
}
FUNC(bool) try_2d_relation_between_point_and_line(
    IN (vec2)  point_position, 
    IN (vec2)  line_reference,
    IN (vec2)  line_direction,
    OUT(vec2)  closest_approach
){
    /*
    set line_reference as the origin, 
    project the point_position vector onto line_direction,
    then convert back to original reference frame
    */
    closest_approach = 
        line_reference + 
        dot(point_position - line_reference, normalize(line_direction)) * normalize(line_direction);
    return length(closest_approach - point_position) < EPSILON;
}
FUNC(bool) try_2d_relation_between_point_and_line_segment(
    IN (vec2)  point_position, 
    IN (vec2)  line_segment_start,
    IN (vec2)  line_segment_stop,
    OUT(vec2)  closest_approach
){
    /*
    set line_segment_start as the origin, 
    project the point_position vector onto line_segment_stop-line_segment_start,
    then convert back to original reference frame
    */
	vec2 line_segment_offset = line_segment_stop - line_segment_start;
	float line_segment_length = length(line_segment_offset);
	vec2 line_segment_direction = normalize(line_segment_offset);
	vec2 distance_to_closest_approach = clamp(dot(point_position - line_segment_start, line_segment_direction), 0.f, line_segment_length);
    closest_approach = line_segment_start + distance_to_closest_approach * line_segment_direction;
    return length(closest_approach - point_position) < EPSILON;
}
FUNC(bool) try_2d_relation_between_point_and_ray(
    IN (vec2)  point_position, 
    IN (vec2)  ray_origin,
    IN (vec2)  ray_direction,
    OUT(vec2)  closest_approach
){
    /*
    set ray_origin as the origin, 
    project the point_position vector onto ray_direction,
    then convert back to original reference frame
    */
	vec2 distance_to_closest_approach = max(dot(point_position - ray_origin, ray_direction), 0.f);
    closest_approach = ray_origin + distance_to_closest_approach * ray_direction;
    return length(closest_approach - point_position) < EPSILON;
}
FUNC(bool) try_2d_relation_between_point_and_circle(
    IN (vec2)  point_position, 
    IN (vec2)  circle_origin,
    IN (float) circle_radius,
    OUT(vec2)  closest_approach
){
	// scale normalized offset to a distance that's not to exceed circle radius
	closest_approach = normalize(point_position - circle_origin) * min(circle_radius, length(point_position - circle_origin));
    return length(closest_approach - point_position) < circle_radius;
}
FUNC(bool) try_2d_relation_between_point_and_triangle(
    IN (vec2)  point_position, 
    IN (vec2)  triangle_vertex1,
    IN (vec2)  triangle_vertex2,
    IN (vec2)  triangle_vertex3,
    OUT(vec2)  closest_approach
){
	/*
	Find the dot product between surface normal of each edge 
	and the position of the point relative to one of the edge vertices.
	An intersection occurs if they're either all positive or negative. 
	Then compare the point to each edge as a line segment to find the closest approach. 
	*/
	vec2 A  = triangle_vertex1;
	vec2 B  = triangle_vertex2;
	vec2 C  = triangle_vertex3;
	vec2 D  = point_position;
	vec2 AB = A-B;
	vec2 BC = B-C;
	vec2 CA = C-A;
	vec2 AB_normal = vec2(-AB.y, AB.x); 
	vec2 BC_normal = vec2(-BC.y, BC.x); 
	vec2 CA_normal = vec2(-CA.y, CA.x); 
	bool is_D_within_AB = dot(D-B, AB_normal) > 0.f;
	bool is_D_within_BC = dot(D-C, BC_normal) > 0.f;
	bool is_D_within_CA = dot(D-A, CA_normal) > 0.f;
	if (is_D_within_AB == is_D_within_BC && is_D_within_BC == is_D_within_CA)
	{
		closest_approach = point_position;
		return true;
	}
	vec2 closest_approach_to_AB;
	vec2 closest_approach_to_BC;
	vec2 closest_approach_to_CA;
	try_2d_relation_between_point_and_line_segment( A, B, x, closest_approach_to_AB );
	try_2d_relation_between_point_and_line_segment( B, C, x, closest_approach_to_BC );
	try_2d_relation_between_point_and_line_segment( C, A, x, closest_approach_to_CA );
	float distance_to_AB = length(closest_approach_to_AB - x);
	float distance_to_BC = length(closest_approach_to_BC - x);
	float distance_to_CA = length(closest_approach_to_CA - x);
	float min_distance = min(distance_to_AB, min(distance_to_BC, distance_to_CA));
	closest_approach = 
		distance_to_AB == min_distance? closest_approach_to_AB : 
		distance_to_BC == min_distance? closest_approach_to_BC : 
		                                closest_approach_to_CA;
	return false;
}
FUNC(bool) try_2d_relation_between_lines(
    IN (vec2)  line1_reference,
    IN (vec2)  line1_direction,
    IN (vec2)  line2_reference,
    IN (vec2)  line2_direction,
    OUT(vec2)  intersection
){
	vec2 A0 =           line1_reference;
	vec2 A  = normalize(line1_direction);
	vec2 B0 =           line2_reference;
	vec2 B  = normalize(line2_direction);

	/*
	scale B by the closest distance from B0 to a (AKA, the length of the rejection)
	*/
	vec2 rejection = B0-A0 - dot(B0-A0, A) * A;
	float t = -length(rejection) / dot(B, normalize(rejection))
	intersection = B0 + B*t;

	float similarity = dot(A, B);
	return abs(abs(similarity)-1) < EPSILON;
}
FUNC(bool) try_2d_relation_between_line_and_line_segment(
    IN (vec2)  line_reference,
    IN (vec2)  line_direction,
    IN (vec2)  line_segment_start,
    IN (vec2)  line_segment_stop,
    OUT(vec2)  line_closest_approach,
    OUT(vec2)  line_segment_closest_approach
){
	vec2 A0 =          line1_reference;
	vec2 A  = normalize(line1_direction);
	vec2 B0 =          line_segment_start;
	vec2 B1 =          line_segment_stop;
	vec2 B  = normalize(B1-B0);
	float B_length = length(B1-B0);

	/*
	scale B by the closest distance from B0 to a (AKA, the length of the rejection)
	*/
	vec2 A0B0_rejecting_A = B0-A0 - dot(B0-A0, A) * A;
	float t = -length(A0B0_rejecting_A) / dot(B, normalize(A0B0_rejecting_A));
	line_segment_closest_approach = B0 + B*clamp(t, 0.f, B_length);

	return try_2d_relation_between_point_and_line(
		ray_closest_approach,
		line_reference,
		line_direction,
		line_closest_approach
	);
}
FUNC(bool) try_2d_relation_between_line_and_ray(
    IN (vec2)  line_reference,
    IN (vec2)  line_direction,
    IN (vec2)  ray_origin,
    IN (vec2)  ray_direction,
    OUT(vec2)  line_closest_approach,
    OUT(vec2)  ray_closest_approach
){
	vec2 A0 =           line1_reference;
	vec2 A  = normalize(line1_direction);
	vec2 B0 =           line2_reference;
	vec2 B  = normalize(line2_direction);

	vec2 A0B0_rejecting_A = B0-A0 - dot(B0-A0, A) * A;
	float t = -length(A0B0_rejecting_A) / dot(B, normalize(A0B0_rejecting_A));
	ray_closest_approach = B0 + B*max(t, 0.f);

	return try_2d_relation_between_point_and_line(
		ray_closest_approach,
		line_reference,
		line_direction,
		line_closest_approach
	);
}
FUNC(bool) try_2d_relation_between_line_and_circle(
    IN (vec2)  line_reference,
    IN (vec2)  line_direction,
    IN (vec2)  circle_origin, 
    IN (float) circle_radius,
    OUT(vec2)  line_closest_approach,
    OUT(vec2)  circle_closest_approach,
    OUT(vec2)  entrance,
    OUT(vec2)  exit
){
    /**/
    vec2  A0 =           line_reference;
    vec2  A  = normalize(line_direction);
    vec2  B  =           point_position;
    vec2  r  =           circle_radius;
	float xz = dot(B-A0, A);
	float z2 = dot(B, B) - x*x;
    float xr = sqrt(max(r*r - z2, 1e-10));
    line_closest_approach   = A0 + A*xz;
    circle_closest_approach = B  + r*normalize(A0 - B);
    entrance = A0 + A*(xz-xr);
    exit     = A0 + A*(xz+xr);
    return z2 < r*r;
}
FUNC(bool) try_2d_relation_between_line_and_triangle(
    IN (vec2)  line1_reference,
    IN (vec2)  line1_direction,
    IN (vec2)  triangle_vertex1, 
    IN (vec2)  triangle_vertex2, 
    IN (vec2)  triangle_vertex3, 
    OUT(vec2)  line_closest_approach,
    OUT(vec2)  triangle_closest_approach,
    OUT(vec2)  entrance,
    OUT(vec2)  exit
){
	vec2 A  = triangle_vertex1;
	vec2 B  = triangle_vertex2;
	vec2 C  = triangle_vertex3;
    vec2 D0 = line_reference;
    vec2 D  = normalize(line_direction);

	vec2 DAB AB_closest; bool is_AB_intersecting = try_2d_relation_between_line_and_line_segment(D0,D,A,B, AB_intersection);
	vec2 DBC BC_closest; bool is_BC_intersecting = try_2d_relation_between_line_and_line_segment(D0,D,B,C, BC_intersection);
	vec2 DCA CA_closest; bool is_CA_intersecting = try_2d_relation_between_line_and_line_segment(D0,D,C,A, CA_intersection);

	if (!is_AB_intersecting && !is_BC_intersecting && !is_CA_intersecting) { return false; }

	float min_distance;
	if (is_AB_intersecting)	{  }
}
FUNC(bool) try_3d_relation_between_lines(
    IN (vec3)  line1_reference,
    IN (vec3)  line1_direction,
    IN (vec3)  line2_reference,
    IN (vec3)  line2_direction,
    OUT(vec3)  line1_closest_approach,
    OUT(vec3)  line2_closest_approach,
){
	vec3 A0  =           line1_reference;
	vec3 A   = normalize(line1_direction);
	vec3 B0  =           line2_reference;
	vec3 B   = normalize(line2_direction);
	vec3 C   = normalize(cross(A,B));

	// solution from: https://math.stackexchange.com/a/3436386/142030
	// also see closest-approach-between-lines-visualized.scad 
	vec3 B_reject = B0-A0 - dot(B0-A0, A) * A - dot(B0-A0, C) * C;
	line2_closest_approach = B0 - B*length(B_reject) / dot(B,normalize(B_reject));

	vec3 A_reject = A0-B0 - dot(A0-B0, B) * B - dot(A0-B0, C) * C;
	line2_closest_approach = A0 - A*length(A_reject) / dot(A,normalize(A_reject));
}
FUNC(bool) try_3d_relation_between_ray_and_sphere(
    IN (vec3)  ray_origin,
    IN (vec3)  ray_direction,
    IN (vec3)  sphere_origin, 
    IN (float) sphere_radius, 
    OUT(vec3)  ray_closest_approach,
    OUT(vec3)  sphere_closest_approach,
    OUT(vec3)  entrance,
    OUT(vec3)  exit
){

}