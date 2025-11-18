
import math

from pyglm import glm

from State import State, state_distance
from Properties import Properties
from ElementsAndState import ElementsAndState
from Elements import Elements
from Universals import Universals
from UniversalPropagator import UniversalPropagator

G = 6.6743015e-11 # m³kg⁻¹s⁻²
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

pi = 3.141592653589793238462643383279
degrees = 2*pi/360

hyperbolic_periapses = [
	
]

elliptic_periapses = [
	(mass_of_earth_moon, Elements(384e6,  0.0549, 0.0*degrees, 0.0, 0.0, 0.0)), # Moon
	(mass_of_saturn, Elements(186e6,    0.0196, 1.53*degrees, 0.0, 0.0, 0.0)), # Mimas
	(mass_of_saturn, Elements(238.4e6,  0.0047, 0.02*degrees, 0.0, 0.0, 0.0)), # Enceladus
	(mass_of_saturn, Elements(295e6,    0.0001, 1.09*degrees, 0.0, 0.0, 0.0)), # Tethys
	(mass_of_saturn, Elements(377.7e6,  0.0022, 0.02*degrees, 0.0, 0.0, 0.0)), # Dione
	(mass_of_saturn, Elements(527.2e6,  0.001,  0.35*degrees, 0.0, 0.0, 0.0)), # Rhea
	(mass_of_saturn, Elements(1221.9e6, 0.0288, 0.33*degrees, 0.0, 0.0, 0.0)), # Titan
	(mass_of_saturn, Elements(1481.5e6, 0.0274, 0.43*degrees, 0.0, 0.0, 0.0)), # Hyperion
	(mass_of_saturn, Elements(3561.7e6, 0.0283, 157*degrees,  0.0, 0.0, 0.0)), # Iapetus
	(mass_of_jupiter, Elements(421.8e6,   0.0041, 0.050*degrees, 0.0, 0.0, 0.0)), # Io
	(mass_of_jupiter, Elements(671.1e6,   0.0090, 0.470*degrees, 0.0, 0.0, 0.0)), # Europa
	(mass_of_jupiter, Elements(1070.4e6,  0.0013, 0.200*degrees, 0.0, 0.0, 0.0)), # Ganymede
	(mass_of_jupiter, Elements(1882.7e6,  0.0074, 0.192*degrees, 0.0, 0.0, 0.0)), # Callisto
    (mass_of_sun, Elements(5.790905e10,   0.2056, 7.005*degrees, 0.0, 0.0, 0.0)), # Mercury
    (mass_of_sun, Elements(1.082080e11,   0.0068, 3.3947*degrees, 0.0, 0.0, 0.0)), # Venus
    (mass_of_sun, Elements(1.49598023e11, 0.0167, 0.000*degrees, 0.0, 0.0, 0.0)), # Earth
    (mass_of_sun, Elements(2.279392e11,   0.0934, 1.851*degrees, 0.0, 0.0, 0.0)), # Mars
    (mass_of_sun, Elements(7.78570e11,    0.0489, 1.305*degrees, 0.0, 0.0, 0.0)), # Jupiter
    (mass_of_sun, Elements(1.43353e12,    0.0565, 2.484*degrees, 0.0, 0.0, 0.0)), # Saturn
    (mass_of_sun, Elements(2.87246e12,    0.046,  0.770*degrees, 0.0, 0.0, 0.0)), # Uranus
    (mass_of_sun, Elements(4.49506e12,    0.009,  1.769*degrees, 0.0, 0.0, 0.0)), # Neptune
    (mass_of_sun, Elements(39.482*au,     0.2488, 17.14*degrees, 0.0, 0.0, 0.0)), # Pluto
    # mass_of_galaxy, Elements(), # sun around galaxy
]

def test_nth_period_congruence_modulo_n(): 
	# also checks Stepanov's "circular orbit" property, determinism, and an identity
	n = 10 # samples of orbit
	for k in range(n): # starting position
		for (mass, elements) in elliptic_periapses:
			elements = elements.advance(2*pi*k/n)
			# TODO: FIX DIV0 WHERE t=0
			for i in range(1,n+1):
				T = properties.period_from_semi_major_axis(elements.semi_major_axis, mass)
				universals = Universals.from_state(mass, converter.get_state_from_elements(elements, mass))
				for j in range(0,3+1):
					assert state_distance(
						propagator.state(universals,T*(i/n)),
						propagator.state(universals,T*(i/n+j)), 
					) < 3e-5

def test_apsis_extrema(): 
	# TODO: vary starting position in the orbit
	for (mass, elements) in elliptic_periapses:
		T = properties.period_from_semi_major_axis(elements.semi_major_axis, mass)
		universals = Universals.from_state(mass, converter.get_state_from_elements(elements, mass))
		e = 0.03
		for sign in [1,-1]:
			# apoapsis position
			assert (
				glm.length(propagator.state(universals,(T/2)).position) >
				glm.length(propagator.state(universals,(T/2)*(1+sign*e)).position)
			)
			# apoapsis velocity
			assert (
				glm.length(propagator.state(universals,(T/2)).velocity) <
				glm.length(propagator.state(universals,(T/2)*(1+sign*e)).velocity)
			)
			# TODO: FIX DIV0 WHERE t=0
			# # periapsis position
			# assert (
			# 	glm.length(propagator.state(universals,0).position) <
			# 	glm.length(propagator.state(universals,sign*T*e).position)
			# )
			# # periapsis velocity
			# assert (
			# 	glm.length(propagator.state(universals,0).velocity) >
			# 	glm.length(propagator.state(universals,sign*T*e).velocity)
			# )
		elements.mean_anomaly = pi # apoapsis
		universals = Universals.from_state(mass, converter.get_state_from_elements(elements, mass))
		e = 0.03
		for sign in [1,-1]:
			# periapsis position
			assert (
				glm.length(propagator.state(universals,(T/2)).position) <
				glm.length(propagator.state(universals,(T/2)*(1+sign*e)).position)
			)
			# periapsis velocity
			assert (
				glm.length(propagator.state(universals,(T/2)).velocity) >
				glm.length(propagator.state(universals,(T/2)*(1+sign*e)).velocity)
			)
			# TODO: FIX DIV0 WHERE t=0
			# # apoapsis position
			# assert (
			# 	glm.length(propagator.state(universals,0).position) >
			# 	glm.length(propagator.state(universals,sign*T*e).position)
			# )
			# # apoapsis velocity
			# assert (
			# 	glm.length(propagator.state(universals,0).velocity) <
			# 	glm.length(propagator.state(universals,sign*T*e).velocity)
			# )

def test_circular_orbit_radius_conservation():
	for (mass, elements) in elliptic_periapses:
		elements = elements.copy()
		elements.eccentricity = 0 # circular orbits only
		T = properties.period_from_semi_major_axis(elements.semi_major_axis, mass)
		universals = Universals.from_state(mass, converter.get_state_from_elements(elements, mass))
		n = 10 # orbit samples
		# TODO: FIX DIV0 WHERE t=0
		for i in range(1,n+1):
			assert abs(glm.length(propagator.state(universals,T*i/n).position) / glm.length(universals.initial_position) - 1) < 1e-6
			assert abs(glm.length(propagator.state(universals,T*i/n).velocity) / glm.length(universals.initial_velocity) - 1) < 1e-6

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

	t=0 minimizes o↦glm.length(state(periapsis,t).position)
	t=0 maximizes o↦glm.length(state(periapsis,t).velocity)

for known elliptic orbits:
x	congruence:
		state(o,n*T) = o for any n∈ℕ
x	orbit/involute/identity: 
		advance(advance(advance(o, T/n), T/n), T/n) = o etc.
x	t=T/2 maximizes o↦glm.length(state(periapsis,t).position)
x	t=T/2 minimizes o↦glm.length(state(periapsis,t).velocity)
x	t=0 maximizes o↦glm.length(state(apoapsis,t).position)
x	t=0 minimizes o↦glm.length(state(apoapsis,t).velocity)
x	t=T/2 minimizes o↦glm.length(state(apoapsis,t).position)
x	t=T/2 maximizes o↦glm.length(state(apoapsis,t).velocity)

for circular orbits:
	glm.length(advance(o,t).position) is constant for any t

given: 
	o 	orbit
	T 	period
	d 	distance()

'''

