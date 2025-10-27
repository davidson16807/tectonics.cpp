
from dataclasses import dataclass

@dataclass
class Elements:
	semi_major_axis: int;
	eccentricity: int;
	inclination: int;
	longitude_of_ascending_node: int;
	argument_of_periapsis: int;
	mean_anomaly: int;
