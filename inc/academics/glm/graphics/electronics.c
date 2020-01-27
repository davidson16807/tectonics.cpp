
/*
"GAMMA" is the constant that's used to map between 
rgb signals sent to a monitor and their actual intensity
*/
const float GAMMA = 2.2;

vec3 get_rgb_intensity_of_rgb_signal(
    in vec3 signal
){
    return vec3(
        pow(signal.x, GAMMA),
        pow(signal.y, GAMMA),
        pow(signal.z, GAMMA)
    );
}
vec3 get_rgb_signal_of_rgb_intensity(
    in vec3 intensity
){
    return vec3(
        pow(intensity.x, 1./GAMMA),
        pow(intensity.y, 1./GAMMA),
        pow(intensity.z, 1./GAMMA)
    );
}
