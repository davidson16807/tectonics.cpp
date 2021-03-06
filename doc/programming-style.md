Program with the intent to offer guarantees about your code. 
	Enforce your guarantees if you can using your programming language of choice, 
	 otherwise document your guarantees as profusely as possible. 
	There's nothing wrong with inline documentation of guarantees if it makes the documentation process easier.
	If you write a function, the easiest way to follow this rule is to guarantee the function is free from side effects.
	If your function is performance limited, guarantee that it will not exceed a certain number of computations. 
	For instance, you can adapt your model resolution to work with a certain number of grid cells, or adapt your raymarch algorithm to work within a certain number of steps.
	If you pass N to a function, guarantee that the function will provide sensible results for all values of N.
	This is a tall order if you set out to do this with a bunch of if statements, so don't.
	Express floating point behavior in terms of relations, not conditional logic.
	Likewise, if you pass a data structure to a function, guarantee it will provide sensible outputs for all possible configurations of the data structure. 
Strive for transparency. 
	Make it apparent that there is nothing up your sleeves. The developer using your code should have the sense there is no ugly surprise behavior that is encapsulated by your code and that he is free to modify state in whatever wacky way he wants, whenever he wants. If he does not feel comfortable doing so, you may not be offering enough guarantees about your code. 
	This leads to my next point...
Design state to be orthogonal, and clearly separate independant state from derived state
	If you have derived attributes that could get out of sync with the rest of your data structure, those attributes need to go somewhere else. Create a pure function that generates the derived attribute when given a data structure as input, as much as possible. It's slower than using a memo, but unless it's performing out of order memory access you won't care.
	If some derived attributes require extensive calculation, create a pure function that returns a dictionary of interdependant memos. This reduces the amount of state you have to work with while still allowing you to use memos.
	This may not always be practical, though, so if you are storing derived state together with independant state in the same class, at least make sure to separate them physically and/or label them as derived/independant
Prefer non-piecewise functions to conditional logic or polymorphism
    Non-piecewise functions often capture the higher level understanding that conditional logic can't. 
    It also often offers the guarantee that a sensible solution is always available regardless of input.
Use an object oriented practice only when you can strongly justify its use in your specific use case. 
	There are four commonly accepted Object Oriented practices: abstraction, inheritence, encapsulation, and polymorphism.
	Abstraction is generally a good thing, but it can be applied in many ways that do not involve objects. 
	Inheritance forces you to split a common concern across two very distantly located regions of code.
	Inheritance is very often a bad idea, and it can very often be replaced by composition.
    Encapsulation can be used effectively when, for instance, creating managing memory,
      but when applied without thinking, it encourages the creation of side effects that are deliberately obscured.
    Encapsulation can also often be accomplished using namespaces or functions. 
    Encapsulation should only be used when the object is behaviorally equivalent to an implementation that is free from side effects. 
    Polymorphism can be used effectively to manage conditional logic that consistantly reappears across several functions, 
      but you should entertain ways to consolidate conditional logic into simple, non-piecewise functions.
    In summary:
	There are four object oriented practices to consider.
	Each practice can be applied in isolation to the other. 
	Do not apply a practice unless you can strongly justify its use in your specific use case.
	If you can justify a practice in one region of code, do not take that as license to apply it everywhere throughout the code base.
	Object Oriented Practices must be adopted on a case-by-case basis,
	By default, your objects should be nothing more than big dumb data structures, 
	  totally free of methods or private attributes. 
And most importantly: think for yourself. 
	In case you haven't already noticed, I am not advocating you adopt a strictly functional programming or data oriented design. 
	I am not telling you that your methods cannot handle file I/O or today's date 
	(although if you are generating random numbers, you really should accept your rng as a parameter to your function)
	If I could best summarize my recommendation, I think it would be a pragmatically minded procedural programming paradigm.
	It should place heavy emphasis on pure functions, and should require purity in virtually all low level utility code. 
	It should decompose object oriented programming into its core practices, and apply them piecemeal only when appropriate.
	It should be transparent and should heavily document guarantees that are not otherwise explicitly stated in code, 
	 whether in code comments or otherwise.
	It should be hard to goof up state.
	It should make use of data oriented design when performance is critical.
	It should make use of lambdas where performance is not critical.
	It should make use of OOP when practical. 
