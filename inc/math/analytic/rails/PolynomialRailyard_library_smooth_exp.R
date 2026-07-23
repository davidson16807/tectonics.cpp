

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
		Mpe = function(parameters) {
			max(abs(comparison$pe(parameters)))
		},
		# max error
		Me = function(parameters) {
			max(abs(comparison$e(parameters)))
		}
	)
}



# linear Newton polynomial of a function's derivative
pddx = function(x1,dydx1, x2,dydx2){
    dydx21 = (dydx2-dydx1) / (x2-x1)
    function(x){ dydx1 + dydx21 * (x-x1) }
}

# integral of a linear Newton polynomial of a function's derivative
p = function(x1,dydx1, x2,dydx2, C){
    dydx21 = (dydx2-dydx1) / (x2-x1)
    function(x){ C + (dydx1 - dydx21*x1)*x + dydx21*x*x/2 }
    # function(x){ C + dydx1*(x-x1) + dydx21 * (x-x1)*(x-x1)/2 }
}

# 1 piece of 2nd degree
xmax = 1.0
X = (0:(xmax*100))/100
exp1p1d = list(
	initialize=function(){ c(exp(xmax/2),xmax/2) },
	decode=function(parameters) {parameters},
	predict=function(parameters){
		pddxθ = parameters[1]
		θ     = parameters[2]
		function(x) {
			ifelse(x<θ, 
				pddx(0,1,        θ,pddxθ)(x), 
				pddx(θ,pddxθ, xmax,exp(xmax))(x))
		}
	}
)

comparison1 = SampledFunctionComparison(X, exp, exp1p1d$predict)
costs = SampleCosts(comparison1)
parameters1 = optim(exp1p1d$initialize(), costs$Me)$par
comparison1$plot(exp1p1d$initialize(), parameters1)
costs$Me(parameters1)
parameters1

# 1 piece of 2nd degree
exp1p2d = list(
	initialize=function(){ c(1) },
	decode=function(parameters) {parameters},
	predict=function(parameters){
		C     = parameters[1]
		pddxθ = parameters1[1]
		θ     = parameters1[2]
		exp1p2d = function(x) {
			p1  = p(0,1, θ,pddxθ, C)
			p2a = p(θ,pddxθ, 1,exp(1), 0)
			p2b = p(θ,pddxθ, 1,exp(1), p1(θ)-p2a(θ))
			ifelse(x<θ, p1(x), p2b(x))
		}
	}
)

# 1 piece of 2nd degree
comparison2 = SampledFunctionComparison(X, exp, exp1p2d$predict)
costs = SampleCosts(comparison2)
parameters2 = optim(exp1p2d$initialize(), costs$Me)$par
comparison2$plot(exp1p2d$initialize(), parameters2)
costs$Me(parameters2)
parameters2




expddx = exp
# 6 pieces of 2nd degree
exp2 = list(
	initialize=function(){ rep(seq(-4,4,length.out=17), each=2) },
	decode=function(parameters) {
		θ =        parameters[(0:16)*2+1]
		d = expddx(parameters[(0:16)*2+2])
		return(c(θ,d))
	},
	predict=function(parameters){
		function(x) {
			θ =        parameters[(0:16)*2+1]
			d = expddx(parameters[(0:16)*2+2])
			p1   = p(θ[1],d[1],θ[2],d[2],0)
			p2_  = p(θ[2],d[2],θ[3],d[3],0);     p2  = p(θ[2],d[2],θ[3],d[3],    p1(θ[2])  -p2_(θ[2]))
			p3_  = p(θ[3],d[3],θ[4],d[4],0);     p3  = p(θ[3],d[3],θ[4],d[4],    p2(θ[3])  -p3_(θ[3]))
			p4_  = p(θ[4],d[4],θ[5],d[5],0);     p4  = p(θ[4],d[4],θ[5],d[5],    p3(θ[4])  -p4_(θ[4]))
			p5_  = p(θ[5],d[5],θ[6],d[6],0);     p5  = p(θ[5],d[5],θ[6],d[6],    p4(θ[5])  -p5_(θ[5]))
			p6_  = p(θ[6],d[6],θ[7],d[7],0);     p6  = p(θ[6],d[6],θ[7],d[7],    p5(θ[6])  -p6_(θ[6]))
			p7_  = p(θ[7],d[7],θ[8],d[8],0);     p7  = p(θ[7],d[7],θ[8],d[8],    p6(θ[7])  -p7_(θ[7]))
			p8_  = p(θ[8],d[8],θ[9],d[9],0);     p8  = p(θ[8],d[8],θ[9],d[9],    p7(θ[8])  -p8_(θ[8]))
			p9_  = p(θ[9],d[9],θ[10],d[10],0);   p9  = p(θ[9],d[9],θ[10],d[10],  p8(θ[9])  -p9_(θ[9]))
			p10_ = p(θ[10],d[10],θ[11],d[11],0); p10 = p(θ[10],d[10],θ[11],d[11],p9(θ[10]) -p10_(θ[10]))
			p11_ = p(θ[11],d[11],θ[12],d[12],0); p11 = p(θ[11],d[11],θ[12],d[12],p10(θ[11])-p11_(θ[11]))
			p12_ = p(θ[12],d[12],θ[13],d[13],0); p12 = p(θ[12],d[12],θ[13],d[13],p11(θ[12])-p12_(θ[12]))
			p13_ = p(θ[13],d[13],θ[14],d[14],0); p13 = p(θ[13],d[13],θ[14],d[14],p12(θ[13])-p13_(θ[13]))
			p14_ = p(θ[14],d[14],θ[15],d[15],0); p14 = p(θ[14],d[14],θ[15],d[15],p13(θ[14])-p14_(θ[14]))
			p15_ = p(θ[15],d[15],θ[16],d[16],0); p15 = p(θ[15],d[15],θ[16],d[16],p14(θ[15])-p15_(θ[15]))
			p16_ = p(θ[16],d[16],θ[17],d[17],0); p16 = p(θ[16],d[16],θ[17],d[17],p15(θ[16])-p16_(θ[16]))
			return (
				(       x<=θ[2])  *p1(x)+
				(θ[2]<x&x<=θ[3])  *p2(x)+
				(θ[3]<x&x<=θ[4])  *p3(x)+
				(θ[4]<x&x<=θ[5])  *p4(x)+
				(θ[5]<x&x<=θ[6])  *p5(x)+
				(θ[6]<x&x<=θ[7])  *p6(x)+
				(θ[7]<x&x<=θ[8])  *p7(x)+
				(θ[8]<x&x<=θ[9])  *p8(x)+
				(θ[9]<x&x<=θ[10]) *p9(x)+
				(θ[10]<x&x<=θ[11])*p10(x)+
				(θ[11]<x&x<=θ[12])*p11(x)+
				(θ[12]<x&x<=θ[13])*p12(x)+
				(θ[13]<x&x<=θ[14])*p13(x)+
				(θ[14]<x&x<=θ[15])*p14(x)+
				(θ[15]<x         )*p15(x)
			)
		}
	}
)

X = seq(-4, 4, length.out=1000)
comparison1 = SampledFunctionComparison(X, exp, exp2$predict)
costs = SampleCosts(comparison1)
parameters1 = optim(exp2$initialize(), costs$Mpe)$par
comparison1$plot(exp2$initialize(), parameters1)
costs$Mpe(parameters1)
exp2$decode(parameters1)
parameters1



rho = function(x) {exp(-sqrt(x))}
rhoddx = function(x) {-exp(-sqrt(x))/(2*sqrt(x))}

# 6 pieces of 2nd degree
xmax = 16.0
X = (0:(xmax*100))/100
rho2 = list(
	initialize=function(){ c(
		      0.0014,
		0.02, 0.055,
		0.30, 0.48,
		1.5,  1.8,
		4.2,  4.8,
		10.0, 12.0,
		16.0, 16.0)},
	decode=function(parameters) {
		d0 = rhoddx(parameters[1])
		θ1 =         parameters[2]
		d1 = rhoddx(parameters[3])
		θ2 =         parameters[4]
		d2 = rhoddx(parameters[5])
		θ3 =         parameters[6]
		d3 = rhoddx(parameters[7])
		θ4 =         parameters[8]
		d4 = rhoddx(parameters[9])
		θ5 =         parameters[10]
		d5 = rhoddx(parameters[11])
		θ6 =         parameters[12]
		d6 = rhoddx(parameters[13])
		return (c(d0,θ1,d1,θ2,d2,θ3,d3,θ4,d4,θ5,d5,θ6,d6))
	},
	predict=function(parameters){
		function(x) {
			d0 = rhoddx(parameters[1])
			θ1 =        parameters[2]
			d1 = rhoddx(parameters[3])
			θ2 =        parameters[4]
			d2 = rhoddx(parameters[5])
			θ3 =        parameters[6]
			d3 = rhoddx(parameters[7])
			θ4 =        parameters[8]
			d4 = rhoddx(parameters[9])
			θ5 =        parameters[10]
			d5 = rhoddx(parameters[11])
			θ6 =        parameters[12]
			d6 = rhoddx(parameters[13])
			p0  = p(0,d0,θ1,d1,1)
			p10 = p(θ1,d1,θ2,d2,0); p1 = p(θ1,d1,θ2,d2,p0(θ1)-p10(θ1))
			p20 = p(θ2,d2,θ3,d3,0); p2 = p(θ2,d2,θ3,d3,p1(θ2)-p20(θ2))
			p30 = p(θ3,d3,θ4,d4,0); p3 = p(θ3,d3,θ4,d4,p2(θ3)-p30(θ3))
			p40 = p(θ4,d4,θ5,d5,0); p4 = p(θ4,d4,θ5,d5,p3(θ4)-p40(θ4))
			p50 = p(θ5,d5,θ6,d6,0); p5 = p(θ5,d5,θ6,d6,p4(θ5)-p50(θ5))
			return (
				(     x<=θ1)*p0(x)+
				(θ1<x&x<=θ2)*p1(x)+
				(θ2<x&x<=θ3)*p2(x)+
				(θ3<x&x<=θ4)*p3(x)+
				(θ4<x&x<=θ5)*p4(x)+
				(θ5<x      )*p5(x)
			)
		}
	}
)

X = (0:(xmax*100))/100
comparison1 = SampledFunctionComparison(X, rho, rho2$predict)
costs = SampleCosts(comparison1)
parameters1 = optim(rho2$initialize(), costs$Me)$par
comparison1$plot(rho2$initialize(), parameters1)
costs$Me(parameters1)
rho2$decode(parameters1)








sqrtddx = function(x) {1/(2*sqrt(x))}

# 6 pieces of 2nd degree
xmax = 16.0
X = (0:(xmax*100))/100
sqrt2 = list(
	initialize=function(){ c(
		      0.0019,
		0.024,0.058,
		0.30, 0.44,
		1.5,  1.8,
		4.5,  5.0,
		11.5, 12.8,
		16.0, 16.0,
		32.0, 32.0,
		64.0, 64.0,
		128,  128,
		256,  256,
		512,  512,
		1024, 1024)},
	decode=function(parameters) {
		d0 = sqrtddx(parameters[1])
		θ1 =         parameters[2]
		d1 = sqrtddx(parameters[3])
		θ2 =         parameters[4]
		d2 = sqrtddx(parameters[5])
		θ3 =         parameters[6]
		d3 = sqrtddx(parameters[7])
		θ4 =         parameters[8]
		d4 = sqrtddx(parameters[9])
		θ5 =         parameters[10]
		d5 = sqrtddx(parameters[11])
		θ6 =         parameters[12]
		d6 = sqrtddx(parameters[13])
		θ7 =         parameters[14]
		d7 = sqrtddx(parameters[15])
		θ8 =         parameters[16]
		d8 = sqrtddx(parameters[17])
		θ9 =         parameters[18]
		d9 = sqrtddx(parameters[19])
		θ10=         parameters[20]
		d10= sqrtddx(parameters[21])
		θ11=         parameters[22]
		d11= sqrtddx(parameters[23])
		θ12=         parameters[24]
		d12= sqrtddx(parameters[25])
		return (c(d0,θ1,d1,θ2,d2,θ3,d3,θ4,d4,θ5,d5,θ6,d6,θ7,d7,θ8,d8,θ9,d9,θ10,d10,θ11,d11,θ12,d12))
	},
	predict=function(parameters){
		function(x) {
			d0 = sqrtddx(parameters[1])
			θ1 =         parameters[2]
			d1 = sqrtddx(parameters[3])
			θ2 =         parameters[4]
			d2 = sqrtddx(parameters[5])
			θ3 =         parameters[6]
			d3 = sqrtddx(parameters[7])
			θ4 =         parameters[8]
			d4 = sqrtddx(parameters[9])
			θ5 =         parameters[10]
			d5 = sqrtddx(parameters[11])
			θ6 =         parameters[12]
			d6 = sqrtddx(parameters[13])
			θ7 =         parameters[14]
			d7 = sqrtddx(parameters[15])
			θ8 =         parameters[16]
			d8 = sqrtddx(parameters[17])
			θ9 =         parameters[18]
			d9 = sqrtddx(parameters[19])
			θ10=         parameters[20]
			d10= sqrtddx(parameters[21])
			θ11=         parameters[22]
			d11= sqrtddx(parameters[23])
			θ12=         parameters[24]
			d12= sqrtddx(parameters[25])
			p0   = p(0,d0,θ1,d1,0)
			p1_  = p(θ1,d1,θ2,d2,0);     p1  = p(θ1,d1,θ2,d2,    p0(θ1)  - p1_(θ1))
			p2_  = p(θ2,d2,θ3,d3,0);     p2  = p(θ2,d2,θ3,d3,    p1(θ2)  - p2_(θ2))
			p3_  = p(θ3,d3,θ4,d4,0);     p3  = p(θ3,d3,θ4,d4,    p2(θ3)  - p3_(θ3))
			p4_  = p(θ4,d4,θ5,d5,0);     p4  = p(θ4,d4,θ5,d5,    p3(θ4)  - p4_(θ4))
			p5_  = p(θ5,d5,θ6,d6,0);     p5  = p(θ5,d5,θ6,d6,    p4(θ5)  - p5_(θ5))
			p6_  = p(θ6,d6,θ7,d7,0);     p6  = p(θ6,d6,θ7,d7,    p5(θ6)  - p6_(θ6))
			p7_  = p(θ7,d7,θ8,d8,0);     p7  = p(θ7,d7,θ8,d8,    p6(θ7)  - p7_(θ7))
			p8_  = p(θ8,d8,θ9,d9,0);     p8  = p(θ8,d8,θ9,d9,    p7(θ8)  - p8_(θ8))
			p9_  = p(θ9,d9,θ10,d10,0);   p9  = p(θ9,d9,θ10,d10,  p8(θ9)  - p9_(θ9))
			p10_ = p(θ10,d10,θ11,d11,0); p10 = p(θ10,d10,θ11,d11,p9(θ10) - p10_(θ10))
			p11_ = p(θ11,d11,θ12,d12,0); p11 = p(θ11,d11,θ12,d12,p10(θ11)- p11_(θ11))
			return (
				(      x<=θ1) *p0(x)+
				(θ1 <x&x<=θ2) *p1(x)+
				(θ2 <x&x<=θ3) *p2(x)+
				(θ3 <x&x<=θ4) *p3(x)+
				(θ4 <x&x<=θ5) *p4(x)+
				(θ5 <x&x<=θ6) *p5(x)+
				(θ6 <x&x<=θ7) *p6(x)+
				(θ7 <x&x<=θ8) *p7(x)+
				(θ8 <x&x<=θ9) *p8(x)+
				(θ9 <x&x<=θ10)*p9(x)+
				(θ10<x&x<=θ11)*p10(x)+
				(θ11<x       )*p11(x)
			)
		}
	}
)

X = (0:(xmax*100))/100
comparison1 = SampledFunctionComparison(X, sqrt, sqrt2$predict)
costs = SampleCosts(comparison1)
parameters1 = optim(sqrt2$initialize(), costs$Me)$par
comparison1$plot(sqrt2$initialize(), parameters1)
costs$Me(parameters1)
sqrt2$decode(parameters1)
