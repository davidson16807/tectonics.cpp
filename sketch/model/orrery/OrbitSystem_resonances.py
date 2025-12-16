
'''
This file is a prototype for the `resonances()` function in OrbitSystem.hpp
'''

def residue(pq): 
	return pq - round(pq)

'''
Resonance is expressed as a ratio p:p+q where p is the "index" and q is the "order".
Index and order reflect the most important aspects of the resonance to astronomers.
The largest order q of any resonance of serious interest to astronomers appears to be 4.
The index p can be anything, but it determines how often the planets assume the same absolute positions in space,
and we have to track absolute positions to accurately estimate insolation (not just relative positions, determined by the order),
so it affects the sample count of the resonance, and therefore it must be held low for performance reasons.
'''
def closest_resonance_index(fraction):
	resonances = sorted([(p,p+q) for p in range(1,4) for q in range(0,4+1)], key=lambda p:p[0]/p[1])
	fractions = [pq[0]/pq[1] for pq in resonances]
	return (
		resonances[bisect.bisect_left(fractions, fraction)] if fraction<1.0 else 
		reversed(resonances[bisect.bisect_left(fractions, 1.0/fraction)])
	)

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

def periods(periods_, resonances_, resonance_count, max_index):
	period_range = [(float('inf'),0)]*resonance_count
	for node, resonance in resonances_.items():
		period_range[resonance] = (
			min(period_range[resonance][0], periods_[node]), 
			max(period_range[resonance][1], periods_[node])
		)
	resonance_periods = [closest_resonance_index(lo/hi, max_index)*lo
		for (lo, hi) in period_range]
	return resonance_periods

phi = 1.618033
pi = 3.14159265358979
periods_ = [1.0, 2.0*pi, phi, 2.0, pi, 2.0*phi, 3.0*pi]
resonances_, resonance_count = resonances(periods_, 10)
for i in range(len(periods_)):
	print(i, resonances_[i])

resonance_periods = periods(periods_, resonances_, resonance_count, 20)
print(resonances, resonance_count)
