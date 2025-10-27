
import glm
from dataclasses import dataclass

@dataclass
class Universals:
    gravitational_parameter: float
    time_offset: float
    initial_position: glm.vec3
    initial_velocity: glm.vec3
    def __str__(self):
        return ("{"
                f"μ={self.gravitational_parameter}, "
                f"t₀={self.time_offset}, "
                f"r₀={list(self.initial_position)}, "
                f"v₀={list(self.initial_velocity)}"
                "}")
