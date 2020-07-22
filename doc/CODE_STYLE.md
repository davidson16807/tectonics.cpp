Code style

namespaces          lower case short words, e.g. `rock` or `air`, distinct from class names
classes             `PascalCase` 
data structures     `snake_case` to encourage treating the structure like a primitive, 
                    `PascalCase` otherwise
Macros              `SCREAMING_CASE`
global variables    `SCREAMING_CASE`
methods             `camelCase`
regular functions   `snake_case`
local variables     `snake_case`
private variables   `snake_case`
constants           no visual distinction
directories         `snake_case`
vectors (glsl)      single capital letters if used in an abstract way
matrices (glsl)     single capital letters if used in an abstract way

MOST IMPORTANT:
* It is important to clearly identify functions that are "regular"
  (i.e. the function is either pure or, if it is a method or has output parameters, 
  the value of return values and output parameters can be determined strictly 
  from the input parameters and parent object).
  If any function violates this, it's a bug. 
  We absolutely need the ability to quickly identify functional purity. 
  We need this ability both when reading an invocation and a method signature.
  We need to know what we can trust.
  This can be accomplished by using `snake_case` and/or appending `get` 
  if and only if that function or method is regular.
* Constants have no particular convention, their case is determined by other properties. 
  Constants place a strict guarantee that a variable will not be modified during runtime,
  and we can rely on these guarantees to reason with code.
  Globals on the other hand require SCREAMING_CASE. 
  Globals introduce side effects to code and make the code difficult to reason with.
  Constants are good, globals are evil. 
