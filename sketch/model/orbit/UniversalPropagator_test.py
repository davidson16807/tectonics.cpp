
import math

from pyglm import glm

from State import State, state_distance
from Properties import Properties
from ElementsAndState import ElementsAndState
from Elements import Elements
from Universals import Universals
from UniversalPropagator import UniversalPropagator

G = 6.6743015e-11 # m³kg⁻¹s⁻²
m_sun = 1.32712440018e20/G # kg
m_galaxy = 1.262e41 * G # kg, back calculated to achieve period of 250 million years
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

# halley = Universals(m_sun, 0.0, glm.vec3(0.59*au,0.0,0.0), glm.vec3(0.0,70.56e3,0.0))
# plot(halley, 60*60*24*365*80, sample_count = 6000)

# moon = orbit = Universals(3.986004418e14/G, 0.0, glm.vec3(384399e3,0.0,0.0), glm.vec3(0.0,1.022e3,0.0))
# plot(moon, 60*60*24*30)

# earth = Universals(m_sun, 0.0, glm.vec3(au,0.0,0.0), glm.vec3(0.0,29.7e3,0.0))
# plot(earth, 60*60*24*365)

# eccentric_solar = Universals(m_sun, 0.0,	glm.vec3(0.59*au,0.0,0.0), glm.vec3(0.0,54e3,0.0))
# plot(eccentric_solar, 60*60*24*365*80)

# hyperbolic_solar = Universals(m_sun, 0.0,glm.vec3(0.59*au,0.0,0.0), glm.vec3(0.0,55e3,0.0))
# plot(hyperbolic_solar, 60*60*24*365*80)

# breakpoint()

mars = Universals(m_sun, 0.0,
	glm.vec3(1.3814*au,0.0,0.0), 
	glm.vec3(0.0,24.07e3,0.0))
# plot(mars, 60*60*24*365*3)

# plot(moon, 60*60*24*30)

'''
test cases:
	elliptic:
		circular:
			circular lunar satellite
			circular earth satellite
		eccentric lunar satellite
		eccentric earth satellite
		eccentric solar satellite
	x	all planets
		the sun around the galactic core
	hyperbolic:
		hyperbolic lunar satellite
		hyperbolic earth satellite
		hyperbolic solar satellite
'''

solar_elliptics = [
	Elements(1.49598023e11, 0.0167, 0.0, 0.0, 0.0, 0.0), # Earth
    # Universals.from_state(m_sun, converter.get_state_from_elements(Elements(5.790905e10,   0.2056, 0.0, 0.0, 0.0, 0.0), m_sun)), # Mercury
    # Universals.from_state(m_sun, converter.get_state_from_elements(Elements(1.082080e11,   0.0068, 0.0, 0.0, 0.0, 0.0), m_sun)), # Venus
    # Universals.from_state(m_sun, converter.get_state_from_elements(Elements(1.49598023e11, 0.0167, 0.0, 0.0, 0.0, 0.0), m_sun)), # Earth
    # Universals.from_state(m_sun, converter.get_state_from_elements(Elements(2.279392e11,   0.0934, 0.0, 0.0, 0.0, 0.0), m_sun)), # Mars
    # Universals.from_state(m_sun, converter.get_state_from_elements(Elements(7.78570e11,    0.0489, 0.0, 0.0, 0.0, 0.0), m_sun)), # Jupiter
    # Universals.from_state(m_sun, converter.get_state_from_elements(Elements(1.43353e12,    0.0565, 0.0, 0.0, 0.0, 0.0), m_sun)), # Saturn
    # Universals.from_state(m_sun, converter.get_state_from_elements(Elements(2.87246e12,    0.046,  0.0, 0.0, 0.0, 0.0), m_sun)), # Uranus
    # Universals.from_state(m_sun, converter.get_state_from_elements(Elements(4.49506e12,    0.009,  0.0, 0.0, 0.0, 0.0), m_sun)), # Neptune
    # Universals.from_state(m_galaxy, converter.get_state_from_elements(Elements(4.49506e12,    0.009,  0.0, 0.0, 0.0, 0.0), m_sun)), # Neptune

]

# breakpoint()

def test_involute():
	for elements in solar_elliptics:
		T = properties.period_from_semi_major_axis(elements.semi_major_axis, m_sun)
		universals = Universals.from_state(m_sun, converter.get_state_from_elements(elements, m_sun))
		assert state_distance(propagator.state(universals,T), State(universals.initial_position, universals.initial_velocity)) < 2e-6

'''
for any orbit:

	group:
		advance(o,0) = o
		advance(advance(o,a),b) = advance(advance(o,b),a) 
		advance(advance(o,a),-a) = o
	commutativity:
		advance(advance(o,a),b) = advance(o,a+b)
	behavioral equality (trivial):
		advance(o,0) = state(o)

	identity:
		state(o,0) = o
	state continuity:
		d(state(o,ε), state(o,0)) < d(state(o,2ε), state(o,0))
	invariance under reference time:
		state(o,t) ≈ state(tare(o),t)

	tare idempotence:
		tare(tare(o)) = tare(o)

	gravitational parameter preservation:
		advance(o,t).gravitational_parameter == orbit.gravitational_parameter
		tare(o).gravitational_parameter == orbit.gravitational_parameter

	energy is conserved:
		energy(advance(o,t)) == energy(o)
	angular momentum is conserved
		angular momentum(advance(o,t)) == angular momentum(o)

for known elliptic orbits:
	congruence:
		state(o,n*T) = o for any n∈ℕ
	involute: 
		advance(advance(o, T/2), T/2) = o
	orbit: 
		advance(advance(advance(o, T/3), T/3), T/3) = o etc.
	state(periapsis,T/2).position = apoapsis
	t=0 minimizes glm.length(o↦state(periapsis,t).position)
	t=0 maximizes glm.length(o↦state(periapsis,t).velocity)
	t=0 maximizes glm.length(o↦state(apoapsis,t).position)
	t=0 minimizes glm.length(o↦state(apoapsis,t).velocity)

for circular orbits:
	glm.length(advance(o,t).position) is constant for any t

given: 
	o 	orbit
	T 	period
	d 	distance()

'''

