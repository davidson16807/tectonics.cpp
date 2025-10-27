
from pyglm import glm

from Elements import Elements
from State import State

class ElementsAndState:
    def __init__(self, properties):
        self.properties = properties

    def get_elements_from_state(self, state):
        angular_momentum_vector = self.properties.get_angular_momentum_vector_from_position_and_velocity(state.position, state.velocity)
        eccentricity_vector = self.properties.get_eccentricity_vector_from_position_and_velocity(state.position, state.velocity)
        node_vector = self.properties.get_node_vector_from_momentum_vector(angular_momentum_vector)
        inclination = self.properties.get_inclination_from_momentum_vector(angular_momentum_vector)
        longitude_of_ascending_node = self.properties.get_longitude_of_ascending_node_from_node_vector(node_vector)
        argument_of_periapsis = self.properties.get_argument_of_periapsis_from_node_and_eccentricity(node_vector, eccentricity_vector)
        true_anomaly = self.properties.get_true_anomaly_from_position_and_eccentricity(state.position, eccentricity_vector)
        eccentricity = glm.length(eccentricity_vector)
        eccentric_anomaly = self.properties.get_eccentric_anomaly_from_true_anomaly(true_anomaly, eccentricity)
        mean_anomaly = self.properties.get_mean_anomaly_from_eccentric_anomaly(eccentric_anomaly, eccentricity)
        semi_latus_rectum = self.properties.get_semi_latus_rectum_from_momentum_vector(angular_momentum_vector)
        semi_major_axis = self.properties.get_semi_major_axis_from_semi_latus_rectum_and_eccentricity(semi_latus_rectum, eccentricity)

        return Elements(
            semi_major_axis,
            eccentricity,
            inclination,
            longitude_of_ascending_node,
            argument_of_periapsis,
            mean_anomaly
        )

    def get_state_from_elements(self, elements):
        I = self.properties.vernal_equinox_direction
        K = self.properties.north_pole_direction

        eccentric_anomaly = self.properties.solve_eccentric_anomaly_from_mean_anomaly(elements.mean_anomaly, elements.eccentricity)
        true_anomaly = self.properties.get_true_anomaly_from_eccentric_anomaly(eccentric_anomaly, elements.eccentricity)
        semi_latus_rectum = self.properties.get_semi_latus_rectum_from_semi_major_axis_and_eccentricity(elements.semi_major_axis, elements.eccentricity)
        perifocal_position = self.properties.get_perifocal_position(semi_latus_rectum, elements.eccentricity, true_anomaly)
        perifocal_velocity = self.properties.get_perifocal_velocity(semi_latus_rectum, elements.eccentricity, true_anomaly)

        # PyGLM matrix rotation (mat4)
        perifocal_to_reference_matrix = glm.mat4(1.0)
        perifocal_to_reference_matrix = glm.rotate(perifocal_to_reference_matrix, elements.longitude_of_ascending_node, K)
        perifocal_to_reference_matrix = glm.rotate(perifocal_to_reference_matrix, elements.inclination, I)
        perifocal_to_reference_matrix = glm.rotate(perifocal_to_reference_matrix, elements.argument_of_periapsis, K)

        position = perifocal_to_reference_matrix * glm.vec4(perifocal_position, 1.0)
        velocity = perifocal_to_reference_matrix * glm.vec4(perifocal_velocity, 1.0)

        # NOTE: we convert to vec4 since it is the only thing glm allows to multiply with mat4
        return State(glm.vec3(position), glm.vec3(velocity))
