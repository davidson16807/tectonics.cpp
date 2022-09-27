
#namespace
SampleDatasetComparison = function(input, output, predict){
	list(
		pe = function(parameters){
			(output-predict(parameters)(input))/output
		},
		e = function(parameters){
			output-predict(parameters)(input)
		},
		plot = function(parameters, parameters2=NULL, type='p'){
			plot(input, output, type=type)
			points(input, predict(parameters)(input), type=type, col=2)
			if(!is.null(parameters2)){
				points(input, predict(parameters)(input), type=type, col=3)
			}
		}
	)
}

#namespace
SampledFunctionComparison = function(samples, truth, predict){
	list(
		pe = function(parameters){
			(truth(samples)-predict(parameters)(samples))/truth(samples)
		},
		e = function(parameters){
			truth(samples)-predict(parameters)(samples)
		},
		plot = function(parameters, parameters2=NULL){
			plot(samples, truth(samples), type='l')
			points(samples, predict(parameters)(samples), type='l', col=2)
			if(!is.null(parameters2)){
				points(samples, predict(parameters2)(samples), type='l', col=3)
			}
		}
	)
}

#namespace
SampleCosts = function(comparison){
	list(
		# mean percent error
		mpe = function(parameters) {
			mean(abs(comparison$pe(parameters)))
		},
		# mean error
		me = function(parameters) {
			mean(abs(comparison$e(parameters)))
		},
		# max percent error
		mxpe = function(parameters) {
			max(abs(comparison$pe(parameters)))
		},
		# max error
		mxe = function(parameters) {
			max(abs(comparison$e(parameters)))
		}
	)
}



# linear Newton polynomial
p1 = function(x1,x2, y1,y2){
    y21 = (y2-y1) / (x2-x1)
    function(x){ y1 + y21 * (x-x1) }
}

# quadratic Newton polynomial
p2 = function(x1,x2,x3, y1,y2,y3){
    y21  = (y2-y1)   / (x2-x1)
    y32  = (y3-y2)   / (x3-x2)
    y321 = (y32-y21) / (x3-x1)
    function(x){ y1 + y21*(x-x1) + y321*(x-x1)*(x-x2) }
}

# cubic Newton polynomial
p3 = function(x1,x2,x3,x4, y1,y2,y3,y4){
    y21   = (y2-y1)     / (x2-x1);
    y32   = (y3-y2)     / (x3-x2);
    y43   = (y4-y3)     / (x4-x3);
    y321  = (y32-y21)   / (x3-x1);
    y432  = (y43-y32)   / (x4-x2);
    y4321 = (y432-y321) / (x4-x1);
    function(x) {
	   y1 + y21*(x-x1) + y321*(x-x1)*(x-x2) + y4321*(x-x1)*(x-x2)*(x-x3)
    }
}


# integral of a linear Newton polynomial of a function's derivative
intp1 = function(x1,dydx1, x2,dydx2, m, C){
    dydx21 = (dydx2-dydx1) / (x2-x1)
    function(x){ C + m*dydx1*x + m*dydx21*(x-x1)*(x-x1)/2 }
}



# 1st order continuous, 2 pieces of 2nd degree
xω = 2.0
x = (0:(xω*100))/100
xθ = xω/2
exp1p1d = function(x) {
	ifelse(x<xθ, 
		p1(0,1,        xθ,exp(xθ))(x), 
		p1(xθ,exp(xθ), xω,exp(xω))(x))
}

# 2nd order continuous, 2 pieces of 2nd degree
exp1p2d = function(x) {
	p1a = intp1(0,1,        xθ,exp(xθ), 1, 1)
	p2a = intp1(xθ,exp(xθ), xω,exp(xω), 1, 0)
	p2b = intp1(xθ,exp(xθ), xω,exp(xω), 1, p1a(xθ)-p2a(xθ))
	m   = (exp(xω)-1)/(p2b(xω)-1)
	p1c = intp1(0,1,        xθ,exp(xθ), m, 1)
	p2c = intp1(xθ,exp(xθ), xω,exp(xω), m, (m*(p1a(xθ)-p2a(xθ)-1)+1))
	ifelse(x<xθ, p1c(x), p2c(x))
}

plot(x, exp(x), type='l', col=1)
lines(x, exp1p2d(x), col=2)
max(abs(exp(x)-exp1p2d(x))/exp(x))

gauss = function(x){ exp(-x^2) }

# 1st order continuous, 3 pieces of 2nd degree
gauss3p2d = list(
	initialize=function(){ c(-0.4,0.0,-0.7) },
	decode=function(parameters) {
		cumsum(exp(parameters))
	},
	predict=function(parameters){
		transformed = cumsum(exp(parameters))
		a = transformed[1]
		b = transformed[2]
		c = transformed[3]
		function(xsigned) {
			x = abs(xsigned)
			ifelse(x<a, p2(-a,0,a, gauss(-a),gauss(0),gauss(a))(x), 
				ifelse(x<c, p2(a,b,c, gauss(a), gauss(b), 0)(x), 0))
		}
	}
)

comparison = SampledFunctionComparison(((-400):400)/100, gauss, gauss3p2d$predict)
costs = SampleCosts(comparison)
parameters = optim(gauss3p2d$initialize(), costs$mxe)$par
comparison$plot(gauss3p2d$initialize(), parameters)
gauss3p2d$decode(parameters)
costs$mxe(parameters)
parameters



# 1st order continuous, 3 pieces of 3rd degree
gauss2p3d = list(
	initialize=function(){ c(-1.4, -1.0, -0.9, -0.4, -0.6) },
	decode=function(parameters) {
		cumsum(exp(parameters))
	},
	predict=function(parameters){
		transformed = cumsum(exp(parameters))
		a = transformed[1]
		b = transformed[2]
		c = transformed[3]
		d = transformed[4]
		e = transformed[5]
		function(xsigned) {
			x = abs(xsigned)
			ifelse(x<b, p3(-b,-a,a,b, gauss(-b),gauss(-a),gauss(a),gauss(b))(x), 
				ifelse(x<e, p3(b,c,d,e, gauss(b), gauss(c), gauss(d), 0)(x), 0))
		}
	}
)

comparison = SampledFunctionComparison(((-400):400)/100, gauss, gauss2p3d$predict)
costs = SampleCosts(comparison)
parameters = optim(gauss2p3d$initialize(), costs$mxe)$par
comparison$plot(gauss2p3d$initialize(), parameters)
gauss2p3d$decode(parameters)
costs$mxe(parameters)
parameters





erf = function(x) { 2 * pnorm(x * sqrt(2)) - 1 }

# 1st order continuous, 6 pieces of 2nd degree
erf4p2d = list(
	initialize=function(){ c(-0.85,-0.5,-0.7,-0.85) },
	decode=function(parameters) {
		cumsum(exp(parameters))
	},
	predict=function(parameters){
		transformed = cumsum(exp(parameters))
		a = transformed[1]
		b = transformed[2]
		c = transformed[3]
		d = transformed[4]
		function(xsigned) {
			x = abs(xsigned)
			sign(xsigned) * ifelse(x<b, p2(0,a,b, erf(0),erf(a),erf(b))(x), 
												ifelse(x<d, p2(b,c,d, erf(b), erf(c), 1)(x), 1))
		}
	}
)

comparison = SampledFunctionComparison(((-400):400)/100, erf, erf4p2d$predict)
costs = SampleCosts(comparison)
parameters = optim(erf4p2d$initialize(), costs$mxe)$par
comparison$plot(erf4p2d$initialize(), parameters)
erf4p2d$decode(parameters)
costs$mxe(parameters)
parameters




# 1st order continuous, 4 pieces of 2nd degree
erf2p2d = list(
	initialize=function(){ c(0.1,-0.6) },
	decode=function(parameters) {
		cumsum(exp(parameters))
	},
	predict=function(parameters){
		transformed = cumsum(exp(parameters))
		a = transformed[1]
		b = transformed[2]
		function(xsigned) {
			x = abs(xsigned)
			sign(xsigned) * ifelse(x<b, p2(0,a,b, erf(0),erf(a),1)(x),  1)
		}
	}
)

comparison = SampledFunctionComparison(((-400):400)/100, erf, erf2p2d$predict)
costs = SampleCosts(comparison)
parameters = optim(erf2p2d$initialize(), costs$mxe)$par
comparison$plot(erf2p2d$initialize(), parameters)
erf2p2d$decode(parameters)
costs$mxe(parameters)
parameters





# 1st order continuous, 4 pieces of 2nd degree
tanh2p2d = list(
	initialize=function(){ c(0.2,-0.2) },
	decode=function(parameters) {
		cumsum(exp(parameters))
	},
	predict=function(parameters){
		transformed = cumsum(exp(parameters))
		a = transformed[1]
		b = transformed[2]
		function(xsigned) {
			x = abs(xsigned)
			sign(xsigned) * ifelse(x<b, p2(0,a,b, tanh(0),tanh(a),1)(x),  1)
		}
	}
)

comparison = SampledFunctionComparison(((-400):400)/100, tanh, tanh2p2d$predict)
costs = SampleCosts(comparison)
parameters = optim(tanh2p2d$initialize(), costs$mxe)$par
comparison$plot(tanh2p2d$initialize(), parameters)
tanh2p2d$decode(parameters)
costs$mxe(parameters)
parameters




sech2 = function(x){ 1/(cosh(x)*cosh(x)) }

# 1st order continuous, 3 pieces of 2nd degree
sech23p2d = list(
	initialize=function(){ c(-0.3,0.05,-0.3) },
	decode=function(parameters) {
		cumsum(exp(parameters))
	},
	predict=function(parameters){
		transformed = cumsum(exp(parameters))
		a = transformed[1]
		b = transformed[2]
		c = transformed[3]
		function(xsigned) {
			x = abs(xsigned)
			ifelse(x<a, p2(-a,0,a, sech2(-a),sech2(0),sech2(a))(x), 
				ifelse(x<c, p2(a,b,c, sech2(a), sech2(b), 0)(x), 0))
		}
	}
)

comparison = SampledFunctionComparison(((-400):400)/100, sech2, sech23p2d$predict)
costs = SampleCosts(comparison)
parameters = optim(sech23p2d$initialize(), costs$mxe)$par
comparison$plot(sech23p2d$initialize(), parameters)
sech23p2d$decode(parameters)
costs$mxe(parameters)
parameters



ddxsech2 = function(x){ -2*tanh(x)/(cosh(x)*cosh(x)) }

ddxsech24p2d = list(
	initialize=function(){ c(-0.7,-1.0,0.3,-0.3) },
	decode=function(parameters) {
		cumsum(exp(parameters))
	},
	predict=function(parameters){
		transformed = cumsum(exp(parameters))
		a = transformed[1]
		b = transformed[2]
		c = transformed[3]
		d = transformed[4]
		function(xsigned) {
			x = abs(xsigned)
			sign(xsigned) * ifelse(x<b, p2(0,a,b, ddxsech2(0),ddxsech2(a),ddxsech2(b))(x), 
												ifelse(x<d, p2(b,c,d, ddxsech2(b), ddxsech2(c), 0)(x), 0))
		}
	}
)

comparison = SampledFunctionComparison(((-400):400)/100, ddxsech2, ddxsech24p2d$predict)
costs = SampleCosts(comparison)
parameters = optim(ddxsech24p2d$initialize(), costs$mxe)$par
comparison$plot(ddxsech24p2d$initialize(), parameters)
ddxsech24p2d$decode(parameters)
costs$mxe(parameters)
parameters


inv = function(x) { 1/x }

# 1st order continuous, 5 pieces of 1st degree
inv5p1d = list(
	initialize=function(){ c(0.03, 0.1, 0.2, 0.4, 0.8) },
	decode=function(parameters) {
		(parameters)
	},
	predict=function(parameters){
		transformed = (parameters)
		a = transformed[1]
		b = transformed[2]
		c = transformed[3]
		d = transformed[4]
		e = transformed[5]
		function(xsigned) {
			x = abs(xsigned)
			sign(xsigned) * ifelse(x<b, p1(a,b, inv(a), inv(b))(x), 
			                ifelse(x<c, p1(b,c, inv(b), inv(c))(x), 
							ifelse(x<d, p1(c,d, inv(c), inv(d))(x), 
							ifelse(x<e, p1(d,e, inv(d), inv(e))(x), 
							ifelse(x<1, p1(e,1, inv(e),      1)(x), 
									1)))))
		}
	}
)

comparison = SampledFunctionComparison(seq(0.03, 1, length.out=100), inv, inv5p1d$predict)
costs = SampleCosts(comparison)
parameters = optim(inv5p1d$initialize(), costs$mxpe)$par
comparison$plot(inv5p1d$initialize(), parameters)
inv5p1d$decode(inv5p1d$initialize())
inv5p1d$decode(parameters)
costs$mxpe(inv5p1d$initialize())
costs$mxpe(parameters)
parameters




# 1st order continuous, 5 pieces of 1st degree
inv5p1d = list(
	initialize=function(){ c(0.03, 0.1, 0.2, 0.4, 0.8, 0,0,0,0) },
	decode=function(parameters) {
		(parameters)
	},
	predict=function(parameters){
		transformed = (parameters)
		a = transformed[1]
		b = transformed[2]
		c = transformed[3]
		d = transformed[4]
		e = transformed[5]
		b1 = transformed[6]
		c1 = transformed[7]
		d1 = transformed[8]
		e1 = transformed[9]
		function(xsigned) {
			x = abs(xsigned)
			sign(xsigned) * ifelse(x<b, p1(a,b, inv(a),    inv(b)+b1)(x), 
			                ifelse(x<c, p1(b,c, inv(b)+b1, inv(c)+c1)(x), 
							ifelse(x<d, p1(c,d, inv(c)+c1, inv(d)+d1)(x), 
							ifelse(x<e, p1(d,e, inv(d)+d1, inv(e)+e1)(x), 
							ifelse(x<1, p1(e,1, inv(e)+e1,         1)(x), 
									1)))))
		}
	}
)

comparison = SampledFunctionComparison(seq(0.03, 1, length.out=300), inv, inv5p1d$predict)
costs = SampleCosts(comparison)
parameters = optim(inv5p1d$initialize(), costs$mxpe)$par
comparison$plot(inv5p1d$initialize(), parameters)
inv5p1d$decode(inv5p1d$initialize())
inv5p1d$decode(parameters)
costs$mxpe(inv5p1d$initialize())
costs$mxpe(parameters)
parameters


# 1st order continuous, 4 pieces of 1st degree
inv4p1d = list(
	initialize=function(){ c(0.03, 0.1, 0.2, 0.5, -1, -0.2, 0) },
	decode=function(parameters) {
		a = parameters[1]
		b = parameters[2]
		c = parameters[3]
		d = parameters[4]
		b1 = parameters[5]
		c1 = parameters[6]
		d1 = parameters[7]
		c(
			paste(a,'<x<',b,':',inv(a),    ',', inv(b)+b1),
			paste(b,'<x<',c,':',inv(b)+b1, ',', inv(c)+c1),
			paste(c,'<x<',d,':',inv(c)+c1, ',', inv(d)+d1),
			paste(d,'<x<',1,':',inv(d)+d1, ',',         1)
		)
	},
	predict=function(parameters){
		a = parameters[1]
		b = parameters[2]
		c = parameters[3]
		d = parameters[4]
		b1 = parameters[5]
		c1 = parameters[6]
		d1 = parameters[7]
		function(xsigned) {
			x = abs(xsigned)
			sign(xsigned) * ifelse(x<b, p1(a,b, inv(a),    inv(b)+b1)(x), 
			                ifelse(x<c, p1(b,c, inv(b)+b1, inv(c)+c1)(x), 
							ifelse(x<d, p1(c,d, inv(c)+c1, inv(d)+d1)(x), 
							ifelse(x<1, p1(d,1, inv(d)+d1,         1)(x), 
									1))))
		}
	}
)

comparison = SampledFunctionComparison(seq(0.03, 1, length.out=300), inv, inv4p1d$predict)
costs = SampleCosts(comparison)
parameters = optim(inv4p1d$initialize(), costs$mxpe)$par
comparison$plot(inv4p1d$initialize(), parameters)
inv4p1d$decode(inv4p1d$initialize())
inv4p1d$decode(parameters)
costs$mxpe(inv4p1d$initialize())
costs$mxpe(parameters)
parameters
