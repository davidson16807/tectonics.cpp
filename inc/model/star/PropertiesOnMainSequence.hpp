#pragma once

namespace star{

  template<
    typename Spherelike,
    typename mass,
    typename length,
    typename luminosity,
    typename temperature,
    typename intensity_per_temperature4
  >
  struct PropertiesOnMainSequence
  {
    const Spherelike sphere;
    const intensity_per_temperature4 stephan_boltzmann_constant;
    const mass solar_mean_molecular_mass;
    const mass solar_mass;
    const length solar_radius;
    const luminosity solar_luminosity;
    const temperature solar_core_temperature;
    PropertiesOnMainSequence(
      const Spherelike sphere,
      const intensity_per_temperature4 stephan_boltzmann_constant,
      const mass solar_mean_molecular_mass,
      const mass solar_mass,
      const length solar_radius,
      const luminosity solar_luminosity,
      const temperature solar_core_temperature
    ):
      sphere(sphere),
      stephan_boltzmann_constant(stephan_boltzmann_constant),
      solar_mean_molecular_mass(solar_mean_molecular_mass),
      solar_mass(solar_mass),
      solar_radius(solar_radius),
      solar_luminosity(solar_luminosity),
      solar_core_temperature(solar_core_temperature)
    {}
    // the approximations below are ripped from Artifexian's video on stars:
    // https://www.youtube.com/watch?v=x55nxxaWXAM
    // we only need rough guesses for these properties, so they should be good enough
    luminosity luminosity_estimate(const mass star_mass) const
    {
        return solar_luminosity * pow(star_mass/solar_mass, 3.5);
    }
    length radius_estimate(const mass star_mass) const
    {
        return solar_radius * pow(star_mass/solar_mass, 0.57);
    }
    temperature surface_temperature_estimate(const mass star_mass) const
    {
        return sqrt(sqrt(intensity_estimate(star_mass) / stephan_boltzmann_constant));
    }
    temperature core_temperature_estimate(const mass star_mass, const mass star_mean_molecular_mass)  const
    {
        return (
            solar_core_temperature 
            * (star_mean_molecular_mass / solar_mean_molecular_mass)
            * (star_mass / solar_mass)
            / (radius_estimate(star_mass) / solar_radius)
        );
    }
    auto intensity_estimate(const mass star_mass) const
    {
        return luminosity_estimate(star_mass) / sphere.surface_area(radius_estimate(star_mass));
    }
    // from Carl Hansen et al., "Stellar Interiors"
  };
  template<
    typename Spherelike,
    typename mass,
    typename length,
    typename luminosity,
    typename temperature,
    typename intensity_per_temperature4
  >
  auto properties_on_main_sequence(
      const Spherelike sphere,
      const intensity_per_temperature4 stephan_boltzmann_constant,
      const mass solar_mass,
      const mass solar_mean_molecular_mass,
      const length solar_radius,
      const luminosity solar_luminosity,
      const temperature solar_core_temperature
  ){
    return PropertiesOnMainSequence<Spherelike,mass,length,luminosity,temperature,intensity_per_temperature4>
    (
      sphere,
      stephan_boltzmann_constant,
      solar_mass,
      solar_mean_molecular_mass,
      solar_radius,
      solar_luminosity,
      solar_core_temperature
    );
  }

}
