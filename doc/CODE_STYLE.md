Code style

namespaces          lower case short words, e.g. `crust` or `genes`
classes             `PascalCase` 
data structures     `snake_case` to encourage treating the structure like a primitive, 
                    `PascalCase` otherwise
Macros              `SCREAMING_CASE`
global variables    `SCREAMING_CASE`
methods             `camelCase`
regular functions   `snake_case`
local variables     `snake_case`
private variables   `snake_case`
constants           `snake_case`
directories         `snake_case`
vectors (glsl)      single capital letters if used in an abstract way
matrices (glsl)     single capital letters if used in an abstract way

NOTES:
* Constants have no particular convention, their case is determined by other properties. 
  Constants place a strict guarantee that a variable will not be modified during runtime,
  and we can rely on these guarantees to reason with code.
  Nonconstant globals on the other hand would require `SCREAMING_CASE`, 
  but we forbid their use so this should not occur in practice.
  Globals introduce side effects to code and make the code difficult to reason with.
  Constants are good, globals are evil. 