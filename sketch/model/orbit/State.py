
import glm
from dataclasses import dataclass

@dataclass
class State:
    position: glm.vec3
    velocity: glm.vec3
    def __str__(self):
        return "{"+f"r={list(self.position)}, v={list(self.velocity)}"+"}"

def state_distance(state1, state2):
	return glm.distance(state1.position, state2.position) + glm.distance(state1.velocity, state2.velocity)
