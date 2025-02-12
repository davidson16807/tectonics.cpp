from math import sqrt
import glm 

class Atmospheres:
  def __init__(self, boltzmann_constant):
    self.boltzmann_constant = boltzmann_constant
  def scale_height(self, atmosphere_temperature, molecular_mass, gravity):
    return self.boltzmann_constant * atmosphere_temperature / (molecular_mass * gravity);

class PointMasses:
  def __init__(self, gravitational_constant):
    self.gravitational_constant = gravitational_constant
  def gravity(self, point_mass, radius):
    return self.gravitational_constant * point_mass / (radius * radius);

class Spheres:
  def __init__(self, pi):
    self.pi = pi
  def surface_area(self, radius):
      return 4*self.pi*radius*radius;
  def volume(self, radius):
    return 4/3*self.pi*radius*radius*radius;
  def is_point_inside(self, point, origin, radius):
    return glm.length(point-origin) < radius;
  def distance_from_surface_to_point(self, point, origin, radius):
    return glm.length(point-origin) - radius;

class Stars:
  def __init__(
    self,
    sphere,
    stephan_boltzmann_constant,
    solar_mean_molecular_mass,
    solar_mass,
    solar_radius,
    solar_luminosity,
    solar_core_temperature
  ):
    self.sphere = sphere
    self.stephan_boltzmann_constant = stephan_boltzmann_constant
    self.solar_mean_molecular_mass = solar_mean_molecular_mass
    self.solar_mass = solar_mass
    self.solar_radius = solar_radius
    self.solar_luminosity = solar_luminosity
    self.solar_core_temperature = solar_core_temperature
  def luminosity_estimate(self, star_mass):
      return self.solar_luminosity * pow(star_mass/self.solar_mass, 3.5);
  def radius_estimate(self, star_mass):
      return self.solar_radius * pow(star_mass/self.solar_mass, 0.57);
  def surface_temperature_estimate(self, star_mass):
      return sqrt(sqrt(self.intensity_estimate(star_mass) / self.stephan_boltzmann_constant));
  def core_temperature_estimate(self, star_mass, star_mean_molecular_mass) :
      return (
          self.solar_core_temperature 
          * (star_mean_molecular_mass / self.solar_mean_molecular_mass)
          * (star_mass / self.solar_mass)
          / (self.radius_estimate(star_mass) / self.solar_radius)
      )
  def intensity_estimate(self, star_mass):
      return self.luminosity_estimate(star_mass) / self.sphere.surface_area(self.radius_estimate(star_mass));




SPEED_OF_LIGHT = 299792458. * METER / SECOND;
BOLTZMANN_CONSTANT = 1.3806485279e-23 * JOULE / KELVIN;
PLANCK_CONSTANT = 6.62607004e-34 * JOULE * SECOND;
STEPHAN_BOLTZMANN_CONSTANT = 5.670373e-8 * WATT / (METER*METER* KELVIN*KELVIN*KELVIN*KELVIN);

# see Lawson 2004, "The Blackbody Fraction, Infinite Series and Spreadsheets"
# we only do a single iteration with n=1, because it doesn't have a noticeable effect on output
def solve_fraction_of_light_emitted_by_black_body_below_wavelength(
    wavelength, 
    temperature
): 
    iterations = 2.;
    h = PLANCK_CONSTANT;
    k = BOLTZMANN_CONSTANT;
    c = SPEED_OF_LIGHT;

    float L = wavelength;
    float T = temperature;

    float C2 = h*c/k;
    float z  = C2 / (L*T);
    float z2 = z*z;
    float z3 = z2*z;
    
    float sum = 0.;
    float n2=0.;
    float n3=0.;
    for (float n=1.; n <= iterations; n++) {
        n2 = n*n;
        n3 = n2*n;
        sum += (z3 + 3.*z2/n + 6.*z/n2 + 6./n3) * exp(-n*z) / n;
    }
    return 15.*sum/(PI*PI*PI*PI);
}

def solve_fraction_of_light_emitted_by_black_body_between_wavelengths(
    lo, 
    hi, 
    temperature
):
    return  solve_fraction_of_light_emitted_by_black_body_below_wavelength(hi, temperature) - 
            solve_fraction_of_light_emitted_by_black_body_below_wavelength(lo, temperature);
}

# This calculates the radiation (in watts/m^2) that's emitted 
# by a single object using the Stephan-Boltzmann equation
def get_intensity_of_light_emitted_by_black_body(
    temperature
):
    float T = temperature;
    return STEPHAN_BOLTZMANN_CONSTANT * T*T*T*T;
}

def solve_intensity3_of_light_emitted_by_black_body_between_wavelengths(
    lo3,
    hi3,
    temperature
):
    return get_intensity_of_light_emitted_by_black_body(temperature)
         * vec3(
             solve_fraction_of_light_emitted_by_black_body_between_wavelengths(lo3.x, hi3.x, temperature),
             solve_fraction_of_light_emitted_by_black_body_between_wavelengths(lo3.y, hi3.y, temperature),
             solve_fraction_of_light_emitted_by_black_body_between_wavelengths(lo3.z, hi3.z, temperature)
           );
}

# from Carl Hansen et al., "Stellar Interiors"
def get_fraction_of_radius_for_star_with_temperature(
  temperature, 
  core_temperature
):
    return sqrt(max(1.0 - (temperature / core_temperature), 0.0));
}


gravitational_constant     = (6.6743015e-11); # meter³/(kilogram*second²)
stephan_boltzmann_constant = (5.670373e-8);   # watt/(meter²*kelvin⁴)
boltzmann_constant         = (1.380649e-23);  # joule/kelvin
dalton = (1.660538921e-27); # kilogram

solar_mass        = ( 1.9891e30 );
solar_radius      = ( 695.7e6 );
solar_luminosity  = ( 3.828e26 );
solar_temperature = ( 5772.0 );

atmosphere = Atmospheres(boltzmann_constant);
point = PointMasses(gravitational_constant);
star = Stars(
  Spheres(3.14159265358979),
  stephan_boltzmann_constant,
  0.6*dalton,
  solar_mass,
  solar_radius,
  solar_luminosity,
  15e6
);

m = solar_mass; atmosphere.scale_height(star.surface_temperature_estimate(m), 0.6*dalton, point.gravity(m, star.radius_estimate(m)))