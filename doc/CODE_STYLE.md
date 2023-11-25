Code style

names in general    use whole words unless otherwise indicated
namespaces          singular, single lower case short words, e.g. `crust` or `genes`, usually implying a genitive case, adjective, or determiner
directories         singular, single lower case short words, e.g. `index` or `test`
classes             `PascalCase` , err toward verbosity
data structures     `snake_case` and brief to encourage treating the structure like a primitive, 
                    `PascalCase` and verbose otherwise
"class categories"  `PascalCase`, do not suffix with `*er`, `*tor`, etc.¹, consider suffixing with `*ing`, `*tion`, etc. 
                    same goes for classes representing curried functions
Macros              `SCREAMING_CASE`, err toward verbosity
global variables    `SCREAMING_CASE`, err toward verbosity
impure methods      `camelCase`², multiple words, brevity is allowed³
impure functions    `camelCase`², multiple words, err toward verbosity
regular methods     `snake_case`, brevity is allowed³
regular functions   `snake_case`, err toward verbosity
parameters          `snake_case`, err toward verbosity
private variables   `snake_case`, err toward verbosity
local variables     `snake_case`, brevity is allowed³
booleans            `snake_case`, prefix with `is_*`, `has_*`, `can_*`, etc.
boolean vectors     `snake_case`, prefix with `are_*`, `have_*`, `can_*`, etc. 
rasters             same rules as if it were the underlying `value_type`
scalars             single lowercase letters if brevity or abstraction demands it⁵, otherwise determined by rules above
vectors             single uppercase letters if brevity or abstraction demands it⁵, otherwise determined by rules above
matrices            single uppercase letters if brevity or abstraction demands it⁵, otherwise determined by rules above
products            concatenated names, if brevity or abstraction demands it⁵, otherwise determined by rules above
other operators     underscore delimited names, if brevity or abstraction demands it⁵, otherwise determined by rules above
template parameters single uppercase letters if brevity or abstraction demands it⁵, or to conform with `glm`, 
                    otherwise same rules as for data structures, do not prefix with `T` if so⁴
constants           determined by rules above⁶

NOTES:
¹ `*er`/`*tor` will be mistaken as evoking an object-oriented mindset, which is not what we're going for.
  The important thing to remind is not that the class ought to be something with agency that does things,
  but that it represents intercomposable, conceptually pure functions that together define some subject matter.
² Does not apply to conceptually pure functions, only functions that are impure by even a weak definition.
  The idea is that we need some way to draw attention to dangerous things⁷.
  Most of the object oriented code that's out in the wild is already rife with side effects, 
  so sticking with `camelCase` conventions will allow 3rd party code to remain consistent with our own conventions,
  while still allowing us to warn ourselves or others that the code they're calling has not been checked for purity. 
³ Methods already have a lot of context built up from the name of their owners,
  and it's typically worthwhile to remember things like intercomposability relationships between methods of the same class.
  Aside from methods, most things that face an API user must be verbose, like parameters or function names.
  Private variables are still used in a scope that's broad enough to be treated just like anything else.
  Local variables are used frequently and are the only thing whose scope is limited enough to permit brevity.
⁴ This was recommend in a C++ lightning talk, whose title I can no longer remember.
  Template parameters behave just like regular parameters so they are low risk,
  and we only draw attention to things that are dangerous⁷.
  Dropping the prefix has resulted in some dramatic improvements to readability without sacrafices to understanding,
  see various grid classes like `dymaxion::Grid.hpp` for examples of this.
⁵ There absolutely are times where this is needed. For examples where brevity is needed, see `units/si.hpp`,
  or the code that's responsible for atmospheric scattering in tectonics.js.
  For examples where abstraction is needed, see code for geometric intersections in tectonics.glsl.
  A lookup table should be provided in comments if abbreviation is done for brevity.
  Subscripting is okay (e.g. `Tout`) but not multiletter abbreviation (e.g. `tempout`).
  Multiletter abbreviation is a holdout from the days of FORTRAN and global variables,
  when there were more variables in a local scope than letters to describe them.
  If you really need abbreviation that badly, then commit to it.
⁶ Constants have no particular convention, their case is determined by other properties. 
  Constants place a strict compile-time guarantee that a variable will not be modified during runtime.
  We can rely on these guarantees to reason with code, so they are helpful.
  We only draw attention to things that are dangerous⁷.
  Nonconstant globals on the other hand would require `SCREAMING_CASE`, 
  but we forbid their use so this should not occur in practice.
  Globals introduce side effects to code and make the code difficult to reason with.
  Constants are good, globals are evil. 
⁷ no such thing as an "everything's okay alarm"