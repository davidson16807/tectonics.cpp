
'''
This file is a prototype for the `resonances()` function in OrbitSystem.hpp
'''

def residue(pq): 
	return pq - round(pq)

'''
Resonance is expressed as a ratio p:p+q where p is the "index" and q is the "order".
Index and order reflect the most important aspects of the resonance to astronomers.
p+q determines the number of times the larger cycle must repeat for all positions to reset.
'''
def closest_resonance_index(fraction, max_index):
	return min(list(range(1,max_index)), key=lambda p: abs(residue(p*fraction)))

def resonance_index_error(p, fraction):
	return p*fraction

print(closest_resonance_index(1.666,10))

def resonances(periods, max_index):
	resonances = {}
	resonance_count = 0
	for j in range(len(periods)):
		for i in range(j):
			fraction = periods[i]/periods[j]
			p = closest_resonance_index(fraction, max_index)
			if abs(residue(p*fraction)) < 1e-3:
				if i in resonances:
					resonance_id = resonances[i]
				else:
					resonance_id = resonance_count
					resonance_count+=1
				resonances[i] = resonance_id
				resonances[j] = resonance_id
	return resonances, resonance_count

def periods(periods, resonances, resonance_count, max_index):
	period_range = [(float('inf'),0)]*resonance_count
	for node, resonance in resonances.items():
		period_range[resonance] = (
			min(period_range[resonance][0], periods[node]), 
			max(period_range[resonance][1], periods[node])
		)
	p = closest_resonance_index(fraction, max_index)
	residue(p*fraction)

phi = 1.618033
pi = 3.14159265358979
periods = [1.0, 2.0*pi, phi, 2.0, pi, 2.0*phi, 3.0*pi]
resonances, resonance_count = resonances(periods, 10)
print(resonances, resonance_count)
