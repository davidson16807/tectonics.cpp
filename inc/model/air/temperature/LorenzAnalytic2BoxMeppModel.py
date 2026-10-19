
'''
`LorenzAnalytic2boxMeppModel` Analytically solves for temperature and heat flux
using a 2-box climate model and the maximum entropy production principle (MEPP).

`LorenzAnalytic2boxMeppModel` runs faster than `LorenzIterative2boxModel`
but it loses some accuracy by modeling thermal emission as a lower-order polynomial.

For more information, see Lorenz et al. (2001):
"Titan, Mars and Earth : Entropy Production by Latitudinal Heat Transport"
'''
class LorenzAnalytic2boxMeppModel:

    def __init__(self, emission_approximator, stefan_boltzmann):
        self.stefan_boltzmann = stefan_boltzmann
        self.emission_approximator = emission_approximator

    def equilibrium_temperature(self, heat):
        return (heat/self.stefan_boltzmann) ** 0.25; 

    # entropy production given an energy flux and two temperature extremes,
    def entropy_production(self, 
            heat_flux, hot_instellation, cold_instellation, emission_coefficient):
        return (heat_flux*
            (1.0/self.equilibrium_temperature((cold_instellation+heat_flux)/emission_coefficient) 
           - 1.0/self.equilibrium_temperature((hot_instellation-heat_flux)/emission_coefficient))
        )

    '''
    calculates entropic heat flux within a 2 box temperature model 
    using the Max Entropy Production Principle 
    `annealing_factor` reduce step_size by this fraction for each iteration 
    insights from a two-box climate model containing atmospheric dynamics".
    '''
    def heat_flux(self, mean_temperature, hot_instellation, cold_instellation):
        f = Identity()
        T = self.emission_approximator(mean_temperature)
        H = Polynomial({0:hot_instellation,  1: 1.0})
        C = Polynomial({0:cold_instellation, 1:-1.0})
        TH = compose(T,H)
        TC = compose(T,C)
        return [flux
            for flux in solutions(TH*TH * (TC - f*TC.derivative())  -  TC*TC * (TH - f*TH.derivative()))
            if type(flux) == complex() and flux > 0
        ]

