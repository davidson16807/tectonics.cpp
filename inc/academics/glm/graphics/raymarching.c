float BIG = 1e20;
float SMALL = 1e-20;
int   MAX_LIGHT_COUNT = 9;

/*
"approx_air_column_density_ratio_along_2d_ray_for_curved_world" 
  calculates column density ratio of air for a ray emitted from the surface of a world to a desired distance, 
  taking into account the curvature of the world.
It does this by making a quadratic approximation for the height above the surface.
The derivative of this approximation never reaches 0, and this allows us to find a closed form solution 
  for the column density ratio using integration by substitution.
"x_start" and "x_stop" are distances along the ray from closest approach.
  If there is no intersection, they are the distances from the closest approach to the upper bound.
  Negative numbers indicate the rays are firing towards the ground.
"z2" is the closest distance from the ray to the center of the world, squared.
"r" is the radius of the world.
"H" is the scale height of the atmosphere.
*/
float approx_air_column_density_ratio_along_2d_ray_for_curved_world(
    in float x_start, 
    in float x_stop, 
    in float z2, 
    in float r, 
    in float H
){

    /*
    GUIDE TO VARIABLE NAMES:
    "x*" distance along the ray from closest approach
    "z*" distance from the center of the world at closest approach
    "r*" distance ("radius") from the center of the world
    "h*" distance ("height") from the center of the world
    "*b" variable at which the slope and intercept of the height approximation is sampled
    "*0" variable at which the surface of the world occurs
    "*1" variable at which the top of the atmosphere occurs
    "*2" the square of a variable
    "d*dx" a derivative, a rate of change over distance along the ray
    */

    // "a" is the factor by which we "stretch out" the quadratic height approximation
    //   this is done to ensure we do not divide by zero when we perform integration by substitution
    float a = 0.45;
    // "b" is the fraction along the path from the surface to the top of the atmosphere 
    //   at which we sample for the slope and intercept of our height approximation
    float b = 0.45;

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
    return min abs(sigma,BIG);
}
/*
"try_approx_air_column_density_ratio_along_ray" is an all-in-one convenience wrapper 
  for approx_air_column_density_ratio_along_ray_2d() and approx_reference_air_column_density_ratio_along_ray.
Just pass it the origin and direction of a 3d ray and it will find the column density ratio along its path, 
  or return false to indicate the ray passes through the surface of the world.
*/
float approx_air_column_density_ratio_along_3d_ray_for_curved_world (
    in vec3  P, 
    in vec3  V,
    in float x,
    in float r, 
    in float H
){
    float xz = dot(-P,V);           // distance ("radius") from the ray to the center of the world at closest approach, squared
    float z2 = dot( P,P) - xz * xz; // distance from the origin at which closest approach occurs
    return approx_air_column_density_ratio_along_2d_ray_for_curved_world( 0.-xz, x-xz, z2, r, H );
}

/*
TODO: multiple scattering events
TODO: support for light sources from within atmosphere
*/
vec3 get_rgb_intensity_of_light_scattered_from_air_for_curved_world(
    in vec3  view_origin,     in vec3 view_direction,
    in vec3  world_position,  in float world_radius,
    in vec3 [MAX_LIGHT_COUNT] light_directions, 
    in vec3 [MAX_LIGHT_COUNT] light_rgb_intensities,
    in int                    light_count,
    in vec3 background_rgb_intensity,
    in float atmosphere_scale_height,
    in vec3 beta_ray, in vec3 beta_mie, in vec3  beta_abs
){
    /*
    // GUIDE TO VARIABLE NAMES:

    For an excellent introduction to what we're try to do here, see Alan Zucconi: 
      https://www.alanzucconi.com/2017/10/10/atmospheric-scattering-3/
    We will be using most of the same terminology and variable names.

    Uppercase letters indicate vectors.
    Lowercase letters indicate scalars.
    Going for terseness because I tried longhand names and trust me, you can't read them.
    "x*"     distance along a ray, either from the ray origin or from closest approach
    "z*"     distance from the center of the world to closest approach
    "r*"     a distance ("radius") from the center of the world
    "h*"     a distance ("height") from the surface of the world
    "*v*"    property of the view ray, the ray cast from the viewer to the object being viewed
    "*l*"    property of the light ray, the ray cast from the object to the light source
    "*2"     the square of a variable
    "*_i"    property of an iteration within the raymarch
    "beta*"  a scattering coefficient, the number of e-foldings in light intensity per unit distance
    "gamma*" a phase factor, the fraction of light that's scattered in a certain direction
    "rho*"   a density ratio, the density of air relative to surface density
    "sigma*" a column density ratio, the density of a column of air relative to surface density
    "I*"     intensity of source lighting for each color channel
    "E*"     intensity of light cast towards the viewer for each color channel
    "*_ray"  property of rayleigh scattering
    "*_mie"  property of mie scattering
    "*_abs"  property of absorption
    */

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
                * exp(-h/H) * beta_gamma * dx
                * exp(-beta_sum * (sigma_l + sigma_v));
            // first line above is incoming fraction: the fraction of light that scatters towards camera
            // second line above is outgoing fraction: the fraction of light that scatters away from camera
        }

        xvi  += dx;
    }

    // now calculate the intensity of light that traveled straight in from the background, and add it to the total
    sigma_v  = approx_air_column_density_ratio_along_2d_ray_for_curved_world(-xv, xv_stop-xv_start-xv, zv2, r, H );
    E += I_back * exp(-beta_sum * sigma_v);

    return E;
}


vec3 get_rgb_fraction_of_light_transmitted_through_air_for_curved_world(
    in vec3  segment_origin, in vec3  segment_direction, in float segment_length,
    in vec3  world_position, in float world_radius,      in float atmosphere_scale_height,
    in vec3  beta_ray,       in vec3  beta_mie,          in vec3  beta_abs
){
    vec3  O = world_position;
    float r = world_radius;
    float H = atmosphere_scale_height;
    /*
    "sigma" is the column density of air, relative to the surface of the world, that's along the light's path of travel,
      we use it to estimate the amount of light that's filtered by the atmosphere before reaching the surface
      see https://www.alanzucconi.com/2017/10/10/atmospheric-scattering-1/ for an awesome introduction
    */
    float sigma  = approx_air_column_density_ratio_along_3d_ray_for_curved_world (segment_origin-world_position, segment_direction, segment_length, r, H);
    // "I_surface" is the intensity of light that reaches the surface after being filtered by atmosphere
    return exp(-sigma * (beta_ray + beta_mie + beta_abs));
}

vec3 get_rgb_intensity_of_light_scattered_from_fluid_for_flat_world(
    in float cos_view_angle, 
    in float cos_light_angle, 
    in float cos_scatter_angle, 
    in float ocean_depth,
    in vec3  refracted_light_rgb_intensity,
    in vec3  beta_ray,       in vec3  beta_mie,          in vec3  beta_abs
){
    float NV = cos_view_angle;
    float NL = cos_light_angle;
    float LV = cos_scatter_angle;

    vec3 I = refracted_light_rgb_intensity;

    /*
    "gamma_*" variables indicate the fraction of scattered sunlight that scatters to a given angle (indicated by its cosine).
    it is also known as the "phase factor"
    It varies
    see mention of "gamma" by Alan Zucconi: https://www.alanzucconi.com/2017/10/10/atmospheric-scattering-3/
    */
    float gamma_ray = get_fraction_of_rayleigh_scattered_light_scattered_by_angle(LV);
    float gamma_mie = get_fraction_of_mie_scattered_light_scattered_by_angle(LV);

    vec3  beta_gamma = beta_ray * gamma_ray + beta_mie * gamma_mie;
    vec3  beta_sum   = beta_ray + beta_mie + beta_abs;

    /*
    "sigma_v"  is the column density, relative to the surface, that's along the view ray.
    "sigma_l" is the column density, relative to the surface, that's along the light ray.
    "sigma_ratio" is the column density ratio of the full path of light relative to the distance along the incoming path
    Since water is treated as incompressible, the density remains constant, 
      so they are effectively the distances traveled along their respective paths.
    TODO: model vector of refracted light within ocean
    */
    float sigma_v  = ocean_depth / NV;
    float sigma_l = ocean_depth / NL;
    float sigma_ratio = 1. + NV/NL;

    return I 
        *     beta_gamma
        * (exp(-sigma_v * sigma_ratio * beta_sum) - 1.)
        /               (-sigma_ratio * beta_sum);
    // first line above is incoming fraction: the fraction of light that scatters towards camera
    // second line above is outgoing fraction: the fraction of light that scatters away from camera
}

vec3 get_rgb_fraction_of_light_transmitted_through_fluid_for_flat_world(
    in float cos_incident_angle, in float ocean_depth,
    in vec3  beta_ray,           in vec3  beta_mie,          in vec3  beta_abs
){
    float sigma  = ocean_depth / cos_incident_angle;
    return exp(-sigma * (beta_ray + beta_mie + beta_abs));
}