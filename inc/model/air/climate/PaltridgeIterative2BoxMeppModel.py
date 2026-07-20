
class PaltridgeIterative2BoxMeppModel:

    def __init__(self, boltzmann, stefan_boltzmann):
        pass

    def equilibrium_temperature(heat):
        return (heat/STEPHAN_BOLTZMANN_CONSTANT) ** 0.25; 

    # entropy production given an energy flux and two temperature extremes,
    def entropy_production(heat_flux, hot_instellation, cold_instellation, emission_coefficient):
        return (heat_flux
            /(BOLTZMANN_CONSTANT * 
                ( self.equilibrium_temperature((cold_instellation+heat_flux)/emission_coefficient) 
                - self.equilibrium_temperature((hot_instellation-heat_flux)/emission_coefficient))
            )
        )

    '''
    calculates entropic heat flow within a 2 box temperature model  
    calculates heat flow within a 2 box temperature model 
    using the Max Entropy Production Principle and Gradient Descent
    for more information, see Lorenz et al. 2001: 
    "Titan, Mars and Earth : Entropy Production by Latitudinal Heat Transport"
    `annealing_factor` reduce step_size by this fraction for each iteration 
    '''
    def heat_flow(self, hot_instellation, cold_instellation, emission_coefficient=1.0, iterations=10, annealing_factor = 0.8):
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
