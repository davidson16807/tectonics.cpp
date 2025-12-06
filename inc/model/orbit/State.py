
from pyglm import glm
from dataclasses import dataclass

@dataclass
class State:
    position: glm.vec3
    velocity: glm.vec3
    def __str__(self):
        return "{"+f"r={list(self.position)}, v={list(self.velocity)}"+"}"
    @staticmethod
    def from_universals(universals):
        return State(
            universals.initial_position, 
            universals.initial_velocity
        )


'''
`sape` returns "symmetric absolute percent error" for vectors
'''
def sape(a,b): 
    average = (glm.length(a)+glm.length(b))/2
    return glm.length(a-b)/average if average > 0 else 0

def state_distance(state1, state2):
	return sape(state1.position, state2.position) + sape(state1.velocity, state2.velocity)
