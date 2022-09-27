
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
			plot(c(input,input), c(output, predict(parameters)(input)), col=0)
			points(input, output, type=type)
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



# linear Newton polynomial of a function's derivative
dpdx = function(x1,dydx1, x2,dydx2){
    dydx21 = (dydx2-dydx1) / (x2-x1)
    function(x){ dydx1 + dydx21 * (x-x1) }
}

# integral of a linear Newton polynomial of a function's derivative
p = function(x1,dydx1, x2,dydx2, C){
    dydx21 = (dydx2-dydx1) / (x2-x1)
    function(x){ C + dydx1*x + dydx21 * (x-x1)*(x-x1)/2 }
}


# 1 piece of 2nd degree
xmax = 1.0
exp1p1d = list(
	initialize=function(){ c(exp(xmax/2),xmax/2) },
	decode=function(parameters) {parameters},
	predict=function(parameters){
		dpdxθ = parameters[1]
		θ     = parameters[2]
		function(x) {
			ifelse(x<θ, 
				dpdx(0,1,        θ,dpdxθ)(x), 
				dpdx(θ,dpdxθ, xmax,exp(xmax))(x))
		}
	}
)

comparison1 = SampledFunctionComparison((0:(xmax*100))/100, exp, exp1p1d$predict)
costs = SampleCosts(comparison1)
parameters1 = optim(exp1p1d$initialize(), costs$mxe)$par
comparison1$plot(exp1p1d$initialize(), parameters1)
costs$mxe(parameters1)
parameters1

# 1 piece of 2nd degree
exp1p2d = list(
	initialize=function(){ c(1) },
	decode=function(parameters) {parameters},
	predict=function(parameters){
		C     = parameters[1]
		dpdxθ = parameters1[1]
		θ     = parameters1[2]
		exp1p2d = function(x) {
			p1 = p(0,1, θ,dpdxθ, C)
			p2a = p(θ,dpdxθ, 1,exp(1), 0)
			p2b = p(θ,dpdxθ, 1,exp(1), p1(θ)-p2a(θ))
			ifelse(x<θ, p1(x), p2b(x))
		}
	}
)

# 1 piece of 2nd degree
comparison2 = SampledFunctionComparison((0:(xmax*100))/100, exp, exp1p2d$predict)
costs = SampleCosts(comparison2)
parameters2 = optim(exp1p2d$initialize(), costs$mxe)$par
comparison2$plot(exp1p2d$initialize(), parameters2)
costs$mxe(parameters2)
parameters2