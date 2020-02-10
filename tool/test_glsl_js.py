import pypeg2
import pypeg2glsl
import pypeg2js
import glsl2js

test='''
const float BIG = 1e20;
const float SMALL = 1e-20;
const int   MAX_LIGHT_COUNT = 9;

// "approx_air_column_density_ratio_along_2d_ray_for_curved_world" 
//   calculates column density ratio of air for a ray emitted from the surface of a world to a desired distance, 
//   taking into account the curvature of the world.
// It does this by making a quadratic approximation for the height above the surface.
// The derivative of this approximation never reaches 0, and this allows us to find a closed form solution 
//   for the column density ratio using integration by substitution.
// "x_start" and "x_stop" are distances along the ray from closest approach.
//   If there is no intersection, they are the distances from the closest approach to the upper bound.
//   Negative numbers indicate the rays are firing towards the ground.
// "z2" is the closest distance from the ray to the center of the world, squared.
// "r" is the radius of the world.
// "H" is the scale height of the atmosphere.
float approx_air_column_density_ratio_along_2d_ray_for_curved_world(
    float x_start, 
    float x_stop, 
    float z2, 
    float r, 
    float H
){

    // GUIDE TO VARIABLE NAMES:
    //  "x*" distance along the ray from closest approach
    //  "z*" distance from the center of the world at closest approach
    //  "r*" distance ("radius") from the center of the world
    //  "h*" distance ("height") from the center of the world
    //  "*b" variable at which the slope and intercept of the height approximation is sampled
    //  "*0" variable at which the surface of the world occurs
    //  "*1" variable at which the top of the atmosphere occurs
    //  "*2" the square of a variable
    //  "d*dx" a derivative, a rate of change over distance along the ray

    // "a" is the factor by which we "stretch out" the quadratic height approximation
    //   this is done to ensure we do not divide by zero when we perform integration by substitution
    const float a = 0.45;
    // "b" is the fraction along the path from the surface to the top of the atmosphere 
    //   at which we sample for the slope and intercept of our height approximation
    const float b = 0.45;

    float x0 = sqrt(max(r *r -z2, 0.));
    // if ray is obstructed
    if (x_start < x0 && -x0 < x_stop && z2 < r*r)
    {
        // return ludicrously big number to represent obstruction
        return BIG;
    }
    
    float r1      = r + 6.*H;
    float x1      = sqrt(max(r1*r1-z2, 0.));
    float xb      = x0+(x1-x0)*b;
    float rb2     = xb*xb + z2;
    float rb      = sqrt(rb2);
    float d2hdx2  = z2 / sqrt(rb2*rb2*rb2);
    float dhdx    = xb / rb; 
    float hb      = rb - r;
    float dx0     = x0          -xb;
    float dx_stop = abs(x_stop )-xb;
    float dx_start= abs(x_start)-xb;
    float h0      = (0.5 * a * d2hdx2 * dx0      + dhdx) * dx0      + hb;
    float h_stop  = (0.5 * a * d2hdx2 * dx_stop  + dhdx) * dx_stop  + hb;
    float h_start = (0.5 * a * d2hdx2 * dx_start + dhdx) * dx_start + hb;

    float rho0  = exp(-h0/H);
    float sigma = 
        sign(x_stop ) * max(H/dhdx * (rho0 - exp(-h_stop /H)), 0.) 
      - sign(x_start) * max(H/dhdx * (rho0 - exp(-h_start/H)), 0.);

    // NOTE: we clamp the result to prevent the generation of inifinities and nans, 
    // which can cause graphical artifacts.
    return min(abs(sigma),BIG);
}
// "try_approx_air_column_density_ratio_along_ray" is an all-in-one convenience wrapper 
//   for approx_air_column_density_ratio_along_ray_2d() and approx_reference_air_column_density_ratio_along_ray.
// Just pass it the origin and direction of a 3d ray and it will find the column density ratio along its path, 
//   or return false to indicate the ray passes through the surface of the world.
float approx_air_column_density_ratio_along_3d_ray_for_curved_world (
    vec3  P, 
    vec3  V,
    float x,
    float r, 
    float H
){
    float xz = dot(-P,V);           // distance ("radius") from the ray to the center of the world at closest approach, squared
    float z2 = dot( P,P) - xz * xz; // distance from the origin at which closest approach occurs
    return approx_air_column_density_ratio_along_2d_ray_for_curved_world( 0.-xz, x-xz, z2, r, H );
}

// TODO: multiple scattering events
// TODO: support for light sources from within atmosphere
vec3 get_rgb_intensity_of_light_scattered_from_air_for_curved_world(
    vec3  view_origin,     vec3 view_direction,
    vec3  world_position,  float world_radius,
    vec3[MAX_LIGHT_COUNT] light_directions, 
    vec3[MAX_LIGHT_COUNT] light_rgb_intensities,
    int                    light_count,
    vec3 background_rgb_intensity,
    float atmosphere_scale_height,
    vec3 beta_ray, vec3 beta_mie, vec3  beta_abs
){
    // For an excellent introduction to what we're try to do here, see Alan Zucconi: 
    //   https://www.alanzucconi.com/2017/10/10/atmospheric-scattering-3/
    // We will be using most of the same terminology and variable names.

    // GUIDE TO VARIABLE NAMES:
    //  Uppercase letters indicate vectors.
    //  Lowercase letters indicate scalars.
    //  Going for terseness because I tried longhand names and trust me, you can't read them.
    //  "x*"     distance along a ray, either from the ray origin or from closest approach
    //  "z*"     distance from the center of the world to closest approach
    //  "r*"     a distance ("radius") from the center of the world
    //  "h*"     a distance ("height") from the surface of the world
    //  "*v*"    property of the view ray, the ray cast from the viewer to the object being viewed
    //  "*l*"    property of the light ray, the ray cast from the object to the light source
    //  "*2"     the square of a variable
    //  "*_i"    property of an iteration within the raymarch
    //  "beta*"  a scattering coefficient, the number of e-foldings in light intensity per unit distance
    //  "gamma*" a phase factor, the fraction of light that's scattered in a certain direction
    //  "rho*"   a density ratio, the density of air relative to surface density
    //  "sigma*" a column density ratio, the density of a column of air relative to surface density
    //  "I*"     intensity of source lighting for each color channel
    //  "E*"     intensity of light cast towards the viewer for each color channel
    //  "*_ray"  property of rayleigh scattering
    //  "*_mie"  property of mie scattering
    //  "*_abs"  property of absorption

    vec3  P = view_origin - world_position;
    vec3  V = view_direction;
    vec3  I_back = background_rgb_intensity;
    float r = world_radius;
    float H = atmosphere_scale_height;

    const float STEP_COUNT = 16.;// number of steps taken while marching along the view ray

    float xv  = dot(-P,V);           // distance from view ray origin to closest approach
    float zv2 = dot( P,P) - xv * xv; // squared distance from the view ray to the center of the world at closest approach

    float xv_in_air;      // distance along the view ray at which the ray enters the atmosphere
    float xv_out_air;     // distance along the view ray at which the ray exits the atmosphere
    float xv_in_world;    // distance along the view ray at which the ray enters the surface of the world
    float xv_out_world;   // distance along the view ray at which the ray enters the surface of the world

    //   We only set it to 3 scale heights because we are using this parameter for raymarching, and not a closed form solution
    bool is_scattered  = try_get_relation_between_ray_and_sphere(r + 12.*H, zv2, xv, xv_in_air,   xv_out_air  );
    bool is_obstructed = try_get_relation_between_ray_and_sphere(r,         zv2, xv, xv_in_world, xv_out_world);

    // if view ray does not interact with the atmosphere
    // don't bother running the raymarch algorithm
    if (!is_scattered){ return I_back; }

    // cosine of angle between view and light directions
    float VL; 

    // "gamma_*" indicates the fraction of scattered sunlight that scatters to a given angle (indicated by its cosine, A.K.A. "VL").
    // It only accounts for a portion of the sunlight that's lost during the scatter, which is irrespective of wavelength or density
    float gamma_ray;
    float gamma_mie;
    // "beta_*" indicates the rest of the fractional loss.
    // it is dependant on wavelength, and the density ratio, which is dependant on height
    // So all together, the fraction of sunlight that scatters to a given angle is: beta(wavelength) * gamma(angle) * density_ratio(height)
    vec3  beta_sum   = beta_ray + beta_mie + beta_abs;
    vec3  beta_gamma;
    
    float xv_start = max(xv_in_air, 0.);
    float xv_stop  = is_obstructed? xv_in_world : xv_out_air;
    float dx       = (xv_stop - xv_start) / STEP_COUNT;
    float xvi      = xv_start - xv + 0.5 * dx;

    vec3  L;           // unit vector pointing to light source
    vec3  I;           // vector indicating intensity of light source for each color channel
    float xl;          // distance from light ray origin to closest approach
    float zl2;         // squared distance ("radius") of the light ray at closest for a single iteration of the view ray march
    float r2;          // squared distance ("radius") from the center of the world for a single iteration of the view ray march
    float h;           // distance ("height") from the surface of the world for a single iteration of the view ray march
    float sigma_v;     // columnar density encountered along the view ray,  relative to surface density
    float sigma_l;     // columnar density encountered along the light ray, relative to surface density
    vec3  E = vec3(0); // total intensity for each color channel, found as the sum of light intensities for each path from the light source to the camera

    for (float i = 0.; i < STEP_COUNT; ++i)
    {
        r2  = xvi*xvi+zv2;
        h   = sqrt(r2) - r;
        sigma_v = approx_air_column_density_ratio_along_2d_ray_for_curved_world(-xv, xvi, zv2, r, H );

        for (int j = 0; j < MAX_LIGHT_COUNT; ++j)
        {
            if (j >= light_count) { break; }
            L   = light_directions[j];
            I   = light_rgb_intensities[j];
            VL  = dot(V, L);
            xl  = dot(P+V*(xvi+xv),-L);
            zl2 = r2 - xl*xl; 
            sigma_l = approx_air_column_density_ratio_along_2d_ray_for_curved_world(-xl, 3.*r, zl2, r, H );
            gamma_ray = get_fraction_of_rayleigh_scattered_light_scattered_by_angle(VL);
            gamma_mie = get_fraction_of_mie_scattered_light_scattered_by_angle(VL);
            beta_gamma= beta_ray * gamma_ray + beta_mie * gamma_mie;

            E += I
                // incoming fraction: the fraction of light that scatters towards camera
                * exp(-h/H) * beta_gamma * dx
                // outgoing fraction: the fraction of light that scatters away from camera
                * exp(-beta_sum * (sigma_l + sigma_v));
        }

        xvi  += dx;
    }

    // now calculate the intensity of light that traveled straight in from the background, and add it to the total
    sigma_v  = approx_air_column_density_ratio_along_2d_ray_for_curved_world(-xv, xv_stop-xv_start-xv, zv2, r, H );
    E += I_back * exp(-beta_sum * sigma_v);

    return E;
}

struct LineIntersectionTest
{
	bool hits;
	float enter;
	float exit;
};

'''


glsl = pypeg2.parse(test, pypeg2glsl.glsl) 
print('ORIGINAL-------------------')
print(pypeg2.compose(glsl, pypeg2glsl.glsl))

js = glsl2js.get_js(glsl, pypeg2js.javascript)
print('MODIFIED-------------------')
print(pypeg2.compose(js, pypeg2js.javascript))

'''
parsed[-2].parameters[-1].type.content
glsl2js.get_type_lookups(glsl)
'''
