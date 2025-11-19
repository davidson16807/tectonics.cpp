
import math

from pyglm import glm

from State import State, sape, state_distance
from Properties import Properties
from ElementsAndState import ElementsAndState
from Elements import Elements
from Universals import Universals
from UniversalPropagator import UniversalPropagator

G = 6.6743015e-11 # m³kg⁻¹s⁻²
mass_of_didymos_dimorphos = 5.4e11 # kg
mass_of_earth_moon = 6.0457e24 # kg
mass_of_jupiter = 1.898e27 # kg
mass_of_saturn = 5.683e26 # kg
mass_of_uranus = 8.681e25 # kg
mass_of_neptune = 1.024e26 # kg
mass_of_pluto_charon = 1.3e22 + 1.5e21 # kg
mass_of_sun = 1.32712440018e20/G # kg
mass_of_galaxy = 1.262e41 * G # kg, back calculated to achieve period of 250 million years
au = 1.5e11

propagator = UniversalPropagator(G, max_refinement_count=100, laguerre_method_n = 5)
properties = Properties(glm.vec3(1,0,0), glm.vec3(0,0,1), G)
converter = ElementsAndState(properties)

# def plot(orbit, period, sample_count = 10000):
# 	# ts = range(-int(period/2), int(period/2), int(period/sample_count))
# 	ts = range(1, int(period), int(period/sample_count))
# 	states=[propagator.state(orbit, t) for t in ts]
# 	rs = [state.position for state in states]
# 	vs = [state.velocity for state in states]
# 	'''
# 	we need the plot to display the same bounds for everything 
# 	so that orbits don't appear squished
# 	'''
# 	m = max(max([i for r in rs for i in [r.x,r.y,r.z]]),
# 		   -min([i for r in rs for i in [r.x,r.y,r.z]]))
# 	px.scatter_3d(
# 		x=[0]+[r.x for r in rs], 
# 		y=[0]+[r.y for r in rs],
# 		z=[0]+[r.z for r in rs],
# 		color=[2,1]+[0 for r in rs[1:]],
# 		range_x=[-m,m],
# 		range_y=[-m,m],
# 		range_z=[-m,m],
# 	).show()

# # satellite = orbit = Universals(3.986004418e14/G, 0.0,
# # 	glm.vec3(10e6,0.0,0.0),
# # 	glm.vec3(0.0,9.2e3,0.0)) # from example 2.2 in Conway & Prussing
# # plot(satellite, 60*60*24*30, sample_count = 3000) # currently doesn't work

# halley = Universals(mass_of_sun, 0.0, glm.vec3(0.59*au,0.0,0.0), glm.vec3(0.0,70.56e3,0.0))
# plot(halley, 60*60*24*365*80, sample_count = 6000)

# moon = orbit = Universals(3.986004418e14/G, 0.0, glm.vec3(384399e3,0.0,0.0), glm.vec3(0.0,1.022e3,0.0))
# plot(moon, 60*60*24*30)

# earth = Universals(mass_of_sun, 0.0, glm.vec3(au,0.0,0.0), glm.vec3(0.0,29.7e3,0.0))
# plot(earth, 60*60*24*365)

# eccentric_solar = Universals(mass_of_sun, 0.0,	glm.vec3(0.59*au,0.0,0.0), glm.vec3(0.0,54e3,0.0))
# plot(eccentric_solar, 60*60*24*365*80)

# hyperbolic_solar = Universals(mass_of_sun, 0.0,glm.vec3(0.59*au,0.0,0.0), glm.vec3(0.0,55e3,0.0))
# plot(hyperbolic_solar, 60*60*24*365*80)

# breakpoint()

mars = Universals(mass_of_sun, 0.0,
	glm.vec3(1.3814*au,0.0,0.0), 
	glm.vec3(0.0,24.07e3,0.0))
# plot(mars, 60*60*24*365*3)

# plot(moon, 60*60*24*30)
