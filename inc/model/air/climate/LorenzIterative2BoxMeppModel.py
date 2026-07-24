
'''
`LorenzIterative2boxModel` iteratively solves for temperature and heat flow
using a 2-box climate model and the maximum entropy production principle (MEPP).

`LorenzIterative2boxModel` runs slower than `LorenzAnalytic2boxModel`
however it does not require emission to approximate a linear response to temperature.
Either still run extremely fast compared to a full climate model, 
but they both only find estimates of polar and equatorial temperature,
and it is not possible to extract more information with the same approach.

For more information, see Lorenz et al. (2001):
"Titan, Mars and Earth : Entropy Production by Latitudinal Heat Transport"
'''
class LorenzIterative2boxMeppModel:

    def __init__(self, stefan_boltzmann):
        self.stefan_boltzmann = stefan_boltzmann

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
    calculates entropic heat flow within a 2 box temperature model 
    using the Max Entropy Production Principle 
    `annealing_factor` reduce step_size by this fraction for each iteration 
    insights from a two-box climate model containing atmospheric dynamics".
    '''
    def heat_flow(self, 
            hot_instellation, cold_instellation, emission_coefficient=1.0, 
            iterations=10, annealing_factor = 0.8):
        def sign(x):
            return -1 if x < 0 else (1 if x > 0 else 0)
        Ih = hot_instellation
        Ic = cold_instellation
        beta = emission_coefficient
        F = (Ih-Ic)/4 # heat flow 
        dF = F/iterations # "dF" is a measure for how much F changes with each iteration 
        for i in range(iterations):
            dN = self.entropy_production(F-dF, Ih, Ic, beta) - self.entropy_production(F+dF, Ih, Ic, beta)
            F -= dF * sign(dN)
            dF *= annealing_factor
        return F
