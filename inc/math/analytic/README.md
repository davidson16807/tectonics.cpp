"analytic" contains classes of callable objects that represent common closed form mathematical expressions.
The intent is to represent functions as "glass-box" objects that can be submitted to mathematical operations and analysis.
This reduces the amount of "pointer chasing" that would be done were we to compose black-box C++ functions using lambdas.