import glm
import math

class Properties:
    def __init__(self, vernal_equinox_direction: glm.vec3,
                 north_pole_direction: glm.vec3,
                 gravitational_parameter: float):
        self.vernal_equinox_direction = glm.normalize(vernal_equinox_direction)
        self.north_pole_direction = glm.normalize(north_pole_direction)
        self.gravitational_parameter = gravitational_parameter

    # Angular momentum vector
    def get_angular_momentum_vector_from_position_and_velocity(self, position: glm.vec3, velocity: glm.vec3):
        return glm.cross(position, velocity)

    # Node vector
    def get_node_vector_from_momentum_vector(self, momentum: glm.vec3):
        return glm.cross(self.north_pole_direction, momentum)

    # Eccentricity vector
    def get_eccentricity_vector_from_position_and_velocity(self, position: glm.vec3, velocity: glm.vec3):
        v2 = glm.dot(velocity, velocity)
        r = glm.length(position)
        mu = self.gravitational_parameter
        return ((v2 - mu / r) * position - glm.dot(position, velocity) * velocity) / mu

    # Inclination (i)
    def get_inclination_from_momentum_vector(self, momentum: glm.vec3):
        return math.acos(glm.dot(momentum, self.north_pole_direction) / glm.length(momentum))

    # Longitude of ascending node (Ω)
    def get_longitude_of_ascending_node_from_node_vector(self, node: glm.vec3):
        return math.acos(glm.dot(node, self.vernal_equinox_direction) / glm.length(node))

    # Argument of periapsis (ω)
    def get_argument_of_periapsis_from_node_and_eccentricity(self, node: glm.vec3, eccentricity: glm.vec3):
        return math.acos(glm.dot(node, eccentricity) / (glm.length(node) * glm.length(eccentricity)))

    # True anomaly (ν₀)
    def get_true_anomaly_from_position_and_eccentricity(self, position: glm.vec3, eccentricity: glm.vec3):
        return math.acos(glm.dot(position, eccentricity) / (glm.length(position) * glm.length(eccentricity)))

    # Argument of latitude (u₀)
    def get_argument_of_latitude_from_position_and_node(self, position: glm.vec3, node: glm.vec3):
        return math.acos(glm.dot(position, node) / (glm.length(position) * glm.length(node)))

    # True longitude (ℓ₀)
    def get_true_longitude(self, longitude_of_ascending_node: float, argument_of_latitude: float):
        return longitude_of_ascending_node + argument_of_latitude

    # Semi-latus rectum (p) from momentum
    def get_semi_latus_rectum_from_momentum_vector(self, momentum_vector: glm.vec3):
        return glm.dot(momentum_vector, momentum_vector) / self.gravitational_parameter

    # Semi-latus rectum (p) from a, e
    def get_semi_latus_rectum_from_semi_major_axis_and_eccentricity(self, semi_major_axis: float, eccentricity: float):
        return semi_major_axis * (1.0 - eccentricity * eccentricity)

    # Semi-major axis (a)
    def get_semi_major_axis_from_semi_latus_rectum_and_eccentricity(self, semi_latus_rectum: float, eccentricity: float):
        return semi_latus_rectum / (1.0 - eccentricity * eccentricity)

    # Eccentric anomaly (E) from true anomaly (ν)
    def get_eccentric_anomaly_from_true_anomaly(self, true_anomaly: float, eccentricity: float, iterations: int = 10):
        pi = math.pi
        return math.acos(-(math.cos(pi - true_anomaly) - eccentricity) / (1.0 - eccentricity * math.cos(pi - true_anomaly)))

    # True anomaly (ν) from eccentric anomaly (E)
    def get_true_anomaly_from_eccentric_anomaly(self, eccentric_anomaly: float, eccentricity: float):
        return math.acos((math.cos(eccentric_anomaly) - eccentricity) / (1.0 - eccentricity * math.cos(eccentric_anomaly)))

    # Solve Eccentric anomaly (E) from Mean anomaly (M)
    def solve_eccentric_anomaly_from_mean_anomaly(self, mean_anomaly: float, eccentricity: float, iterations: int = 10):
        e = eccentricity
        E = mean_anomaly
        M = mean_anomaly
        for _ in range(iterations):
            M_E = E - e * math.sin(E)
            dMdE = 1.0 - e * math.cos(E)
            error = M - M_E
            E = E + error / dMdE
        return E

    # Mean anomaly (M)
    def get_mean_anomaly_from_eccentric_anomaly(self, eccentric_anomaly: float, eccentricity: float):
        return eccentric_anomaly - eccentricity * math.sin(eccentric_anomaly)

    # Perifocal position vector
    def get_perifocal_position(self, semi_latus_rectum: float, eccentricity: float, true_anomaly: float):
        P = self.vernal_equinox_direction
        Q = glm.cross(self.north_pole_direction, self.vernal_equinox_direction)
        radius = semi_latus_rectum / (1.0 + eccentricity * math.cos(true_anomaly))
        return radius * (math.cos(true_anomaly) * P + math.sin(true_anomaly) * Q)

    # Perifocal velocity vector
    def get_perifocal_velocity(self, semi_latus_rectum: float, eccentricity: float, true_anomaly: float):
        P = self.vernal_equinox_direction
        Q = glm.cross(self.north_pole_direction, self.vernal_equinox_direction)
        noneccentric_speed = math.sqrt(self.gravitational_parameter / semi_latus_rectum)
        return noneccentric_speed * (-math.sin(true_anomaly) * P + (eccentricity + math.cos(true_anomaly)) * Q)
