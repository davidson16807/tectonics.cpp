from math import pi, sqrt

'''
see Jupp & Cox (2010), "MEP and planetary climates: 
insights from a two-box climate model containing atmospheric dynamics".

This approach solves for wind velocity and heat flux analytically
using nondimensionalized equations for a 2-box climate model
and the maximum entropy production principle (MEPP).

It runs in O(1) time but works only for a single hemisphere
of a planet with a single convection cell
'''

class JuppCoxAnalytic2BoxMeppModel:

    def __init__(self, stefan_boltzmann):
        self.stefan_boltzmann = stefan_boltzmann
        # geometric constant defined after (2.1)
        self.gamma = (3.0 * sqrt(3.0) - pi) / (3.0 * pi)

    def emission_slope(self, emission_admittance, typical_temperature): # B, 2.6
        return 4.0 * emission_admittance * self.stefan_boltzmann * typical_temperature**3

    def emission_intercept(self, emission_admittance, typical_temperature): # A, 2.6
        return -3.0 * emission_admittance * self.stefan_boltzmann * typical_temperature**4

    def xi(self, 
            density, heat_capacity, gravity, scale_height, emission_slope, world_radius): # ξ, 3.1
        return (
            sqrt(12.0 * self.gamma) * density * heat_capacity * sqrt(gravity*scale_height**3) 
            / (emission_slope * world_radius)
        )

    def omega(self, rotation_rate, world_radius, gravity, scale_height): # ω, 3.1
        return (
            rotation_rate * world_radius / 
            (sqrt(12.0 * self.gamma * gravity * scale_height))
        )

    def eta(self, world_radius, scale_height): # η, 3.1
        return (
            sqrt(3.0) * scale_height / world_radius
        )

    def zeta(self, omega): # ζ, 3.2
        return omega + sqrt(1.0 + omega**2)

    # positive solution for X under 2-box assumption, 3.11
    def X(self, omega, tan_wind_angle, xi): 
        return (
            sqrt(omega * (1.0 + tan_wind_angle**2) / (xi * tan_wind_angle))
            * (1.0 + sqrt(1.0 
               + tan_wind_angle*xi / (omega * (1.0 + tan_wind_angle**2))
               + tan_wind_angle**2 / (omega**2 * (1.0 + tan_wind_angle**2))
            ))
        )

    # X→fₐ, 3.9
    def unitless_wind_flux_for_X(self, X):
        return X**-2

    # X→tₑₚ, 3.9
    def unitless_equator_pole_temp_change_for_X(self, X):
        return 1.0 - X**-2

    # X→u, 3.9
    def unitless_wind_speed_for_X(self, X, xi, omega, tan_wind_angle, zeta):
        return sqrt(tan_wind_angle/(xi*omega)) * zeta / X

    # X→cd, 3.9
    def unitless_drag_for_X(self, X, xi, omega, tan_wind_angle):
        return X * sqrt(xi*omega**3 / tan_wind_angle**3)

    # X→tₛₐ, 3.9
    def unitless_surface_air_temp_change_for_X(self, X, tan_wind_angle, xi, omega):
        return tan_wind_angle / (2.0 * xi * omega * X**2)

    # σ̇, 3.12, maximized when the result of the model is correct,
    # needed to verify correctness of model
    def unitless_entropy_production(self, 
            unitless_wind_flux, unitless_equator_pole_temp_change): 
        return 4 * unitless_wind_flux * unitless_equator_pole_temp_change

    # Cd→cd, 3.2
    def deunitize_drag(self, drag, eta):
        return drag / (4.0 * eta)

    # cd→Cd, 3.2, needed to process results of the model
    def unitize_drag(self, unitless_drag, eta):
        return unitless_drag * (4.0 * eta)

    # Fₐ→fₐ, 3.3, needed for σ̇
    def deunitize_wind_flux(self, wind_flux, flux_change): 
        return 2.0 * wind_flux / flux_change

    # fₐ→Fₐ, 3.3, needed to process results of model
    def unitize_wind_flux(self, unitless_wind_flux, flux_change): 
        return unitless_wind_flux * flux_change / 2.0

    # U→u, 3.3, needed for σ̇
    def deunitize_wind_speed(self, wind_speed, zeta, gravity, scale_height): 
        return (
            2.0 * zeta * wind_speed 
            / (sqrt(self.gamma * gravity * scale_height))
        )

    # u→U, 3.3, needed to process results of model
    def unitize_wind_speed(self, unitless_wind_speed, zeta, gravity, scale_height): 
        return (
            unitless_wind_speed * sqrt(self.gamma * gravity * scale_height) 
            / (2.0 * zeta)
        )

    # T→t for any temperature or temperature change T, 3.3, needed for σ̇
    def deunitize_temperature(self, 
            temperature, emission_slope, flux_change): 
        return emission_slope * temperature / flux_change

    # t→T for any temperature or temperature change T, 3.3, needed to process results of model
    def unitize_temperature(self, 
            unitless_temperature, emission_slope, flux_change): 
        return unitless_temperature * flux_change / emission_slope

