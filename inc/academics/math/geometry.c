FUNC(float) get_surface_area_of_sphere(
    IN(float) radius
) {
    return 4.*PI*radius*radius;
}

// NOTE: GRANDFATHERED
FUNC(bool) try_get_relation_between_ray_and_sphere(
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
DESIGN PRINCIPLES FOR INTERSECTION FUNCTIONS:
* functional purity: 
  output is determined solely from the input, global variables are never used
  underscored_lower_case is used to indicate functional purity
* performant:        
  suitable for use in shader code
* language agnostic: 
  transpiles to any C-family language that supports pass-by-reference data structures (C, C++, GLSL, JS)
  only trivial data structures are to be used (e.g. vec3 from GLSL/glm)
  no language specific features like function overloading
  no language specific patterns or concepts (e.g. function chaining, OOP, unless that conflicts with the above)
  since some languages don't support passing primitives by reference, 
  a boolean is returned that indicates when an intersection exists,
  hence we do still use the "try_get" pattern from C#
* obvious usage: 
  behavior can be deduced from the function signature alone,
  function names err on the side of verbosity, 
  function names explain their exact intent and do not avoid prepositions or conjunctions
  function names can be reconstructed after seeing only a few examples (unless that conflicts with the above)
  spatial input makes no assumptions about the reference frame, 
  vecNs are used whereever possible to reference a position or direction in space
  no concepts or standards need to be introduced to the user before usage (e.g. function chaining, sentinel values, other standards)
  no data structures need to be introduced to the user before usage, with the exception of vecNs
*/

FUNC(bool) try_get_3d_relation_between_line_and_point(
    IN (vec3)  line_reference,
    IN (vec3)  line_direction,
    IN (vec3)  point_position, 
    OUT(vec3)  closest_approach
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
FUNC(bool) try_get_3d_relation_between_line_segment_and_point(
    IN (vec3)  line_segment_start,
    IN (vec3)  line_segment_stop,
    IN (vec3)  point_position, 
    OUT(vec3)  closest_approach
){
    /*
    set line_segment_start as the origin, 
    project the point_position vector onto line_segment_stop,
    then convert back to original reference frame
    */
    closest_approach = 
        line_reference + 
        dot(point_position - line_reference, normalize(line_direction)) * normalize(line_direction);
    return length(closest_approach - point_position) < EPSILON;
}
FUNC(bool) try_get_3d_relation_between_ray_and_point(
    IN (vec3)  ray_origin,
    IN (vec3)  ray_direction,
    IN (vec3)  point_position, 
    OUT(vec3)  closest_approach
){

}
FUNC(bool) try_get_3d_relation_between_lines(
    IN (vec3)  line_reference,
    IN (vec3)  line_direction,
    IN (vec3)  point_position, 
    OUT(vec3)  intersection
){

}
FUNC(bool) try_get_3d_relation_between_line_segment_and_line(
    IN (vec3)  line_segment_start,
    IN (vec3)  line_segment_stop,
    IN (vec3)  point_position, 
    OUT(vec3)  intersection
){

}
FUNC(bool) try_get_3d_relation_between_ray_and_line(
    IN (vec3)  ray_origin,
    IN (vec3)  ray_direction,
    IN (vec3)  point_position, 
    OUT(vec3)  intersection
){

}
FUNC(bool) try_get_3d_relation_between_ray_and_sphere(
    IN (vec3)  ray_origin,
    IN (vec3)  ray_direction,
    IN (vec3)  sphere_origin, 
    IN (float) sphere_radius, 
    OUT(vec3)  closest_approach,
    OUT(vec3)  entrance,
    OUT(vec3)  exit
){

}