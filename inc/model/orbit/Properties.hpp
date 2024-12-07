#pragma once

namespace orbit{

  template<typename acceleration_per_area_density>
  struct Properties
  {
    const acceleration_per_area_density gravitational_constant;
    Properties(const acceleration_per_area_density gravitational_constant):
      gravitational_constant(gravitational_constant)
    {}
    template<typename mass, typename length>
    auto gravity(const mass point_mass, const length radius) const
    {
      return gravitational_constant * point_mass / (radius * radius);
    }
  };
  template<typename acceleration_per_area_density>
  Properties<acceleration_per_area_density> properties(const acceleration_per_area_density gravitational_constant)
  {
    return Properties<acceleration_per_area_density>(gravitational_constant);
  }
    
}
