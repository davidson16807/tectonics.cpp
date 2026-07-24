from typing import List
from Polynomial import Polynomial, Shifting, quadratic_taylor_series

'''
`BudykoSellers2dModel` is an energy balance model
that solves for temperature so as to balance emission and heat flux with insolation
A method is provided so that heat flux can be estimated
using the maximum entropy production principle (MEPP).

`BudykoSellers2dModel` runs much faster than a full climate model, 
however it does not represent other aspects of climate like precipitation.

`BudykoSellers2dModel` runs faster than `BudykoSellersDiffusive2dModel`
since it requires no out-of-order traversal,
however it has a much more crude, nondiffusive model of heat flux.

For more information, see papers by Budyko (1969) or Sellers (1969).
'''

class BudykoSellers2dModel:

    def __init__(self, emission_approximator):
        self.emission_approximator = emission_approximator

    def temperatures(self,
        heat_flow_for_temperature: float, 
        mean_temperature: float, 
        instellations: List[float]
    ): 
        return [
            for instellation in instellations
            for solution in solutions(
                - self.emission_approximator(mean_temperature)
                - heat_flow_for_temperature * Shifting(-mean_temperature)
                + instellation
            ) if solution > 0
        ]

    def mean_temperature(temperatures):
        return sum(temperatures)/len(temperatures)

    # entropy production given an energy flux and two temperature extremes,
    def entropy_production(self, 
        heat_flow_for_temperature: float, 
        mean_temperature: float, 
        temperatures: List[float]
    ):
        return sum([
            abs(heat_flow_for_temperature * (1.0/temperature - 1.0/mean_temperature))
            for temperature in temperatures
        ])

stefan_boltzmann = # W/K⁴
BudykoSellers2dModel(quadratic_taylor_series(Polynomial({4:stefan_boltzmann})))

