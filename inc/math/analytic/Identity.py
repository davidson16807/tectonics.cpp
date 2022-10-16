
scalar_types = {int,float}

class Identity:
    def __init__(self):
        pass
    def __call__(self, x):
        return x
    def __str__(self):
        return 'x'
    def __repr__(self):
        return str(self)
    def __add__(self, other):
        if type(other) in scalar_types:
            return Shifting(other)
        if type(other) in {Identity}:
            return Scaling(2)
        if type(other) in {Scaling}:
            return Scaling(other.factor+1)
        if type(other) in {Shifting}:
            return polynomial(self)+polynomial(other)
    def __sub__(self, other):
        if type(other) in scalar_types:
            return Shifting(-other)
        if type(other) in {Identity}:
            return 0
        if type(other) in {Scaling}:
            return 0 if other.factor==1 else polynomial(self)-polynomial(other)
        if type(other) in {Shifting}:
            return -other.offset
    def __mul__(self, other):
        if type(other) in scalar_types:
            return Scaling(other)
        if type(other) in {Identity}:
            return polynomial(self)*polynomial(other)
        if type(other) in {Scaling}:
            return polynomial(self)*polynomial(other)
        if type(other) in {Shifting}:
            return polynomial(self)*polynomial(other)
    def __div__(self, other):
        if type(other) in scalar_types:
            return Scaling(1/other)
        if type(other) in {Identity}:
            return polynomial(self)/polynomial(other)
        if type(other) in {Scaling}:
            return 1 if other.factor==1 else polynomial(self)/polynomial(other)
        if type(other) in {Shifting}:
            return polynomial(self)/polynomial(other)
    def __radd__(self, other):
        return self + other
    def __rsub__(self, other):
        return (self*-1) + other
    def __rmul__(self, other):
        return self * other
    def __rdiv__(self, other):
        return (self**-1)*other
    def __pow__(self, n):
        if type(other) in {int}:
            if other == 0:
                return 1
            else:
                return polynomial(self)**other
        else:
            raise ValueError(f'Attempted exponentiation between identity function and an unrecognized type, {type(other).__name__}')
    def __rpow__(self, n):
        raise ValueError('Exponentiation by an identity function is not supported.')
