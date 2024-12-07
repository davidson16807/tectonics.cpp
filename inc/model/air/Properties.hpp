#pragma once

namespace air{

  template<typename energy_per_temperature>
  struct Properties
  {
    const energy_per_temperature boltzmann_constant;
    Properties(const energy_per_temperature boltzmann_constant):
      boltzmann_constant(boltzmann_constant)
    {}
    template<typename temperature, typename mass, typename acceleration>
    auto scale_height(const temperature atmosphere_temperature, const mass molecular_mass, const acceleration gravity) const
    {
      return boltzmann_constant * atmosphere_temperature / (molecular_mass * gravity);
    }
  };
  template<typename energy_per_temperature>
  Properties<energy_per_temperature> properties(const energy_per_temperature boltzmann_constant)
  {
    return Properties<energy_per_temperature>(boltzmann_constant);
  }

}
