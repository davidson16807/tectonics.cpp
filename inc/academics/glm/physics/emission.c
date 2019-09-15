
FUNC(vec3) solve_rgb_intensity_of_light_emitted_by_black_body(
    IN(float) temperature
){
    return get_intensity_of_light_emitted_by_black_body(temperature)
         * vec3(
             solve_fraction_of_light_emitted_by_black_body_between_wavelengths(600e-9*METER, 700e-9*METER, temperature),
             solve_fraction_of_light_emitted_by_black_body_between_wavelengths(500e-9*METER, 600e-9*METER, temperature),
             solve_fraction_of_light_emitted_by_black_body_between_wavelengths(400e-9*METER, 500e-9*METER, temperature)
           );
}