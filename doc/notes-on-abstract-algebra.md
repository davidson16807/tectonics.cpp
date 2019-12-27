What is a group?

It is a:

Total
Unital
iNverse
Associative
structure

(TUNA structure for short)

we can modify this abbreviation by adding additional letters, namely "C" for the commutative property, so TUNAC is an abelian group (AKA a commutative group)

we can also take away letters, for instance:
UNA 	groupoid
TNA 	inverse semigroup
TUA 	monoid
TUN 	loop

This naming convention helps me because I don't have to remember what the traditional names of the structures are. The traditional names seem to be mostly a historical accident, so I don't think I'm losing much in the process by abandoning them. 

here's a list of common algebras as best I understand them:
integer addition	TUNAC	
rubik's cube		TUNA
stack 				TUNA
queue shift			TUNA
queue push 			TUNA
orbit state vectors TUNA
orbit elements 		TUNA
cooking 			TUA
integer max() 		TA 
32 bit float addition UNAC (operations involving NA do not behave according to rules)
morphology 			TUA

Grothendieck group: 
a way to trick a TUAC structure into thinking its TUNAC

example
1-2=-1:
(1,0)+
(0,2)=

(1,2)~
(0,1)=
1+1=2+0

So for instance, dilation within binary morphology is an operation that has no obvious way to undo, so it only forms a TUAC structure. We might not easily be able to undo the dilation operation, but we sure can keep track of when we want it to occur. To do this, we create a separate raster that's supposed to represent a negation. Everytime we want to negate a dilation, we simply dilate to that second raster instead. So instead of inverse dilation using rasters, we are dilating one raster within a pair. If two raster pairs are equivalent, there will be some number of dilations k that we can perform on one pair of rasters to make it equivalent to the other. We have no way to map the raster pairs back to a single raster, but we do have the next best thing. 

