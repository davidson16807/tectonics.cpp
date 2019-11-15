
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
	vec2 a  = triangle_vertex1;
	vec2 b  = triangle_vertex2;
	vec2 c  = triangle_vertex3;
	vec2 x  = point_position;
	vec2 ab = a-b;
	vec2 bc = b-c;
	vec2 ca = c-a;
	vec2 ab_normal = vec2(-ab.y, ab.x); 
	vec2 bc_normal = vec2(-bc.y, bc.x); 
	vec2 ca_normal = vec2(-ca.y, ca.x); 
	bool is_within_ab = dot(x-b, ab_normal) > 0.f;
	bool is_within_bc = dot(x-c, bc_normal) > 0.f;
	bool is_within_ca = dot(x-a, ca_normal) > 0.f;
	if (is_within_ab == is_within_bc && is_within_bc == is_within_ca)
	{
		closest_approach = point_position;
		return true;
	}
	vec3 closest_approach_to_ab;
	vec3 closest_approach_to_bc;
	vec3 closest_approach_to_ca;
	try_2d_relation_between_point_and_line_segment( a, b, x, closest_approach_to_ab );
	try_2d_relation_between_point_and_line_segment( b, c, x, closest_approach_to_bc );
	try_2d_relation_between_point_and_line_segment( c, a, x, closest_approach_to_ca );
	float distance_to_ab = length(closest_approach_to_ab - x);
	float distance_to_bc = length(closest_approach_to_bc - x);
	float distance_to_ca = length(closest_approach_to_ca - x);
	float min_distance = min(distance_to_ab, min(distance_to_bc, distance_to_ca));
	closest_approach = 
		distance_to_ab == min_distance? closest_approach_to_ab : 
		distance_to_bc == min_distance? closest_approach_to_bc : 
		                                closest_approach_to_ca;
	return false;
}
FUNC(bool) try_2d_relation_between_lines(
    IN (vec2)  line1_reference,
    IN (vec2)  line1_direction,
    IN (vec2)  line2_reference,
    IN (vec2)  line2_direction,
    OUT(vec2)  intersection
){
	vec2 a0 =           line1_reference;
	vec2 an = normalize(line1_direction);
	vec2 b0 =           line2_reference;
	vec2 bn = normalize(line2_direction);

	/*
	scale bn by the closest distance from b0 to a (AKA, the length of the rejection)
	*/
	vec2 rejection = b0-a0 - dot(b0-a0, an) * an;
	b0 - bn * length(rejection) / dot(bn, normalize(rejection));

	float similarity = dot(an, bn);
	return abs(abs(similarity)-1) < EPSILON;
}
FUNC(bool) try_2d_relation_between_line_and_line_segment(
    IN (vec3)  line_segment_start,
    IN (vec3)  line_segment_stop,
    IN (vec3)  point_position, 
    OUT(vec3)  closest_approach
){
	
}
FUNC(bool) try_2d_relation_between_line_and_ray(
    IN (vec3)  ray_origin,
    IN (vec3)  ray_direction,
    IN (vec3)  point_position, 
    OUT(vec3)  closest_approach
){

}
FUNC(bool) try_2d_relation_between_line_and_circle(
    IN (vec3)  line1_reference,
    IN (vec3)  line1_direction,
    IN (vec3)  circle_origin, 
    IN (float) circle_radius,
    OUT(vec3)  closest_approach,
    OUT(vec3)  entrance,
    OUT(vec3)  exit
){

}
FUNC(bool) try_2d_relation_between_line_and_triangle(
    IN (vec3)  line1_reference,
    IN (vec3)  line1_direction,
    IN (vec3)  point_position, 
    OUT(vec3)  closest_approach,
    OUT(vec3)  entrance,
    OUT(vec3)  exit
){

}
FUNC(bool) try_3d_relation_between_lines(
    IN (vec3)  line1_reference,
    IN (vec3)  line1_direction,
    IN (vec3)  line2_reference,
    IN (vec3)  line2_direction,
    OUT(vec3)  line1_closest_approach,
    OUT(vec3)  line2_closest_approach,
){
	vec3 a0  =           line1_reference;
	vec3 an  = normalize(line1_direction);
	vec3 b0  =           line2_reference;
	vec3 bn  = normalize(line2_direction);
	vec3 cn  = normalize(cross(an,bn));

	// solution from: https://math.stackexchange.com/a/3436386/142030
	// also see closest-approach-between-lines-visualized.scad 
	vec3 b_reject = b0-a0 - dot(b0-a0, an) * an - dot(b0-a0, cn) * cn;
	line2_closest_approach = b0-bn * length(b_reject) / dot(bn,normalize(b_reject));

	vec3 a_reject = a0-b0 - dot(a0-b0, bn) * bn - dot(a0-b0, cn) * cn;
	line2_closest_approach = a0-an * length(a_reject) / dot(an,normalize(a_reject));
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