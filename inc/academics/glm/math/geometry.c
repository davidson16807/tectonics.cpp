
// TODO: try to get this to work with structs!
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