
from pyglm import glm
from dataclasses import dataclass

class Universals:
    def __init__(self, combined_mass, time_offset, initial_position, initial_velocity):
        self.combined_mass = combined_mass
        self.time_offset = time_offset
        self.initial_position = initial_position
        self.initial_velocity = initial_velocity
    def angular_momentum_vector(self):
        return glm.cross(self.initial_position, self.initial_velocity)
    def __repr__(self):
        return (f"Universals("
                f"{self.combined_mass}, "
                f"{self.time_offset}, "
                f"{list(self.initial_position)}, "
                f"{list(self.initial_velocity)})")
    def __str__(self):
        return ("{"
                f"μ={self.combined_mass}, "
                f"t₀={self.time_offset}, "
                f"r₀={list(self.initial_position)}, "
                f"v₀={list(self.initial_velocity)}"
                "}")
    @staticmethod
    def from_state(combined_mass: float, state):
        return Universals(
            combined_mass, 0, 
            state.position, 
            state.velocity
        )
