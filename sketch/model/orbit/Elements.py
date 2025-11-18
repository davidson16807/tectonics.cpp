
from dataclasses import dataclass

@dataclass
class Elements:
    semi_major_axis: int;
    eccentricity: int;
    inclination: int;
    longitude_of_ascending_node: int;
    argument_of_periapsis: int;
    mean_anomaly: int;
    def copy(self) -> 'Elements':
        return Elements(**self.__dict__)
    def advance(self, anomaly_offset):
        return Elements(
            self.semi_major_axis, 
            self.eccentricity, 
            self.inclination, 
            self.longitude_of_ascending_node, 
            self.argument_of_periapsis, 
            self.mean_anomaly + anomaly_offset
        )
