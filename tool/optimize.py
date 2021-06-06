import random
import numpy as np

class OptimizationBasis:
    def __init__(self, get_output, get_string):
        self.get_output = get_output
        self.get_string = get_string
    def get_cost(self, params, io):
        return (sum([abs(o - self.get_output(i,params))/o  for (i,o) in io]) / len(io))


def gradient_descent_algorithm(basis, io, x, mobility=3e-6, delta=1e-5):
    def gradient(a, cost, delta=1e-5):
        return np.array([ (cost(x+dx_i)-cost(x-dx_i)) / (2.*np.linalg.norm(dx_i)) for dx_i in delta*np.identity(len(x)) ])
    def gradient_descent(x, cost, mobility=3e-6, delta=1e-8):
        return x - mobility * gradient(x, cost, delta)
    def cost(solution):
        return basis.get_cost(solution, io)
    best, best_cost = x, float('inf')
    grad_cost = 0.*x
    try:
        while True:
            grad_cost = gradient(x, cost, delta)
            cost_x = cost(x)
            best, best_cost = x, cost_x if cost_x < best_cost else best, best_cost
            x = gradient_descent(x, cost, mobility, delta)
            print(basis.get_string(x))
            print('cost: ', cost_x)
            print('∇cost:', grad_cost)
    except KeyboardInterrupt as e:
        return best
        

def genetic_algorithm(basis, io, solutions, survival_rate=0.5):
    def mate(a,b):
        return np.array([random.choice([ai,bi]) for ai,bi in zip(a,b)])
    def mutate(a, rate=0.01):
        return np.array([ai + random.gauss(0,0.2) for ai in a])
    def select(solutions):
        return solutions[int(random.paretovariate(1)) % len(solutions)]
    def cost(solution):
        return basis.get_cost(solution, io)
    try:
        while True:
            solutions = sorted(solutions, key=cost)
            cutoff = int(survival_rate*len(solutions))
            solutions[cutoff:len(solutions)] = [
                mutate(mate(select(solutions), select(solutions)))
                for i in solutions[cutoff:len(solutions)]
            ]
            print(basis.get_string(solutions[0]))
            print('cost: ', cost(solutions[0]), '\n')
    except KeyboardInterrupt as e:
        return solutions
